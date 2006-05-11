
#include <map>
#include <vector>
#include <string>

#include "apppppoe.h"
//include "atuador.h"
//include "atuador_bandalarga.h"

#include "sistema_operacional.h"
#include "so_freebsd.h"

#include "general.h"

#include "virtex.h"

#include <pqxx/pqxx>

#include <sys/types.h>
#include <signal.h>

#include <iostream>
#include <fstream>

using namespace std;
using namespace MOSMAN;
using namespace pqxx;

AplicativoPPPoE::AplicativoPPPoE() {

}




bool AplicativoPPPoE::conecta_bd() {

   // Monta a string de conexão.
   string strConn;

   string host = this->cfg["pgsql"]["host"];
   string user = this->cfg["pgsql"]["user"];
   string pass = this->cfg["pgsql"]["pass"];
   string base = this->cfg["pgsql"]["base"];

   if( host.size() ) {
	   strConn += "host=" + host + " ";
   }

   if( user.size() ) {
	   strConn += "user=" + user + " ";
   }

   if( pass.size() ) {
	   strConn += "password=" + pass + " ";
   }

   if( base.size() ) {
	   strConn += "dbname=" + base + " ";
   }

   this->linkup = false;
   this->linkdown = false;
   this->bWho = false;
   this->bKick = false;
   this->bRC = false;

   return(this->conecta_bd(strConn));


}

// Conecta no banco de dados (postgresql)
bool AplicativoPPPoE::conecta_bd(const string &strConn) {
   this->conn = new connection(strConn);
   return true;
}




int AplicativoPPPoE::init(const string &arquivo,int argc,char** argv,char** env) {

    Aplicativo::init(arquivo,argc,argv,env);
    this->conecta_bd();

	this->ncfg.parseIni(NETWORK_INI);
	this->pcfg.parseIni(PPPOE_INI);

    char ch;

    //this->quiet = false;

	while((ch=getopt(argc,argv,"WUDKRi:a:u:p:")) != -1 ) {
		switch(ch) {
			/**
			 * Parametros do tipo da execucao
			 */
			case 'W':
				this->bWho = true;
				break;
			case 'U':
				this->linkup = true;
				break;
			case 'D':
				this->linkdown = true;
				break;
			case 'K':
				this->bKick = true;
				break;
			case 'R':
				this->bRC = true;
				break;


			/**
			 * Parametros gerais
			 */
			case 'u':
				this->username = optarg;
				break;
			case 'a':
				/** Somente para UP */
				this->hisaddr = optarg;
				break;
			case 'i':
				this->interface = optarg;
				break;
			case 'p':
				/** Somente para UP */
				this->pid = optarg;
				break;

		}

	}

}

// Executa (atualmente apenas no boot)
int AplicativoPPPoE::executa() {


	/**
	 * Define todos os nas que estão deisponiveis
	 */

	for( Config::iterator i = pcfg.begin(); i != pcfg.end(); i++ ) {

		string sessao = i->first;

		string id_nas = this->pcfg[sessao]["id_nas"];
		//string disponivel = this->pcfg[sessao]["enabled"];

		this->nas_disponiveis[ id_nas ] = "";

		if (atoi(this->pcfg[sessao]["enabled"].c_str()))
			this->nas_disponiveis[ id_nas ] = sessao;

		//cout << endl << this->pcfg[sessao]["enabled"] << endl << this->nas_disponiveis[ id_nas ] << endl;

	}


	/**
	 * Who
	 */
	if( this->bWho ) {
		this->print_who(this->username);
	}

	/**
	 * kick
	 */

	 if( this->bKick && this->username == "") {
	 			return -1;
	 }

	 if ( this->bKick ) {
		 this->kick(this->username);
	 }

	 /**
	  * rcstart, rcstop
	  */

	 if ( this->bRC ) {

		 if( !this->linkup && !this->linkdown ) {
			 this->linkup = true;
			 this->linkdown = true;
		 }

		 if( this->linkdown ) {
			 this->rcstop();
		 }

		 if( this->linkup ) {
			 this->rcstart();
		 }

		 return 0;

	 }


	/**
	 * Verificacoes
	 */


	if( ( this->linkingUP() && this->linkingDOWN() ) || ( ! this->linkingUP() && ! this->linkingDOWN() ) ) {
	   // Erro
	   return -1;
	}

	if( (this->linkingUP() && ( this->username == "" || this->hisaddr == "" || this->interface == "" || this->pid == "" ))
            || (this->linkingDOWN() && ( this->username == "" || this->interface == "" )) ) {
		return -1;
	}



	/**
	 * Pega as informacoes do username
	 */

	string dominio_padrao = this->cfg["geral"]["dominio_padrao"];

	//Cria a string de sql pra consulta necessária no banco
	string sSQL;

	sSQL  = "SELECT ";
	sSQL += "	cn.id_conta, cbl.mac, cbl.upload_kbps, cbl.download_kbps, cbl.id_nas ";
	sSQL += "FROM ";
	sSQL +=	"	cntb_conta_bandalarga cbl INNER JOIN cntb_conta cn USING(username, tipo_conta) ";
	sSQL += "WHERE ";
	sSQL += "	cn.username = '" + this->username + "' ";
	sSQL += " 	AND cn.dominio = '" + dominio_padrao + "' ";
	sSQL += "	AND cn.tipo_conta = 'BL' ";


	work *t = new work(*this->conn,"pppoe"); // Inicia a transacao
   	result r = t->exec(sSQL);
   	t->commit();

   	delete t;


	if( r.size() == 0) {
		//Erro
		return -1;
	}

	string id_conta			=	r[0]["id_conta"].c_str();
	string upload_kbps 		=	r[0]["upload_kbps"].c_str();
	string download_kbps 	=	r[0]["download_kbps"].c_str();
	string mac				=	r[0]["mac"].c_str();
	string id_nas			= 	r[0]["id_nas"].c_str();
	string ip_cliente		=	this->hisaddr.c_str();
	string srv_ip			= 	this->cfg["geral"]["srv_ip"].c_str();
	string ip_ext			=	"";

	if( this->ncfg["bge0"]["type"] == "external")
		ip_ext				=	this->ncfg["bge0"]["ipaddr"].c_str();


	SistemaOperacional *so = new SOFreeBSD();


	if( this->linkingUP() ) {
		//string sSQL = "";

		if( this->username.size() == 0 || this->hisaddr.size() == 0 || this->interface.size() == 0 || this->pid.size() == 0 ) {
			return -1;
		}

		this->kick(this->username);

		/**
		cout << "Linking UP (Carregar configuracoes)" << endl;
		cout << "------------------------------------" << endl;
		cout << "User...: " << this->username << endl;
		cout << "Int....: " << this->interface << endl;
		cout << "IP.....: " << this->hisaddr << endl;
		cout << "PID....: " << this->pid << endl;
		*/


		if (this->nas_disponiveis[ id_nas ] == "") {
			this->kick(this->pid);
			exit(-1);
		}

		// IPFW ADD / IPFW PIPE
		so->adicionaRegraBW(atoi(id_conta.c_str()),SO_FW_IP_BASERULE,SO_FW_IP_BASEPIPE_IN,SO_FW_IP_BASEPIPE_OUT,this->cfg["geral"]["int_if"],this->cfg["geral"]["ext_if"],ip_cliente,mac,atoi(upload_kbps.c_str()),atoi(download_kbps.c_str()),this->username);

		//Insere a entrada do usuario
		sSQL  = "INSERT INTO rttb_pppoe_conectado ( ";
		sSQL += "   pid, username, interface, ip, srv_ip, id_nas, ip_ext ";
		sSQL += ") VALUES (";
		sSQL += "   '" + this->pid + "',";
		sSQL += "   '" + this->username + "', ";
		sSQL += "   '" + this->interface + "', ";
		sSQL += "   '" + this->hisaddr + "', ";
		sSQL += "   '" + srv_ip + "', ";
		sSQL += "   '" + id_nas + "', ";
		sSQL += "   '" + ip_ext + "' ";
		sSQL += ") ";

		//cout << sSQL << endl;

		t = new work(*this->conn,"pppoe_entrada"); // Inicia a transacao
   		result r = t->exec(sSQL);
   		t->commit();


	}

	if( this->linkingDOWN() ) {
		if( this->username.size() == 0 || this->interface.size() == 0 ) {
		   return -1;
		}

		/**
		cout << "Linking DOWN (excluir configuracoes) " << endl;
		cout << "------------------------------------" << endl;
		cout << "User...: " << this->username << endl;
		cout << "Int....: " << this->interface << endl;
		*/

		// IPFW DELETE

		so->deletaRegraBW(atoi(id_conta.c_str()),SO_FW_IP_BASERULE,SO_FW_IP_BASEPIPE_IN,SO_FW_IP_BASEPIPE_OUT);

		//Deleta a entrada da tabela
		sSQL  = "DELETE FROM rttb_pppoe_conectado ";
		sSQL += "WHERE ";
		sSQL += "   username = '" + this->username +"' ";
		sSQL += "   AND interface = '" + this->interface + "' ";

		t = new work(*this->conn,"pppoe_saida"); // Inicia a transacao
   		result r = t->exec(sSQL);
   		t->commit();

   		//cout << endl << "linking " << endl;

	}

	delete t;
	return 0;


}

// Loop consulta a radius
int AplicativoPPPoE::loop() {

}

bool AplicativoPPPoE::linkingUP() {
	return this->linkup;
}

bool AplicativoPPPoE::linkingDOWN() {
	return this->linkdown;
}

lista AplicativoPPPoE::who(string username="") {

	// LOOP NO RESULT E JOGA NA LIST
	string sSQL;

	sSQL = "SELECT srv_ip, username, interface, ip, pid, id_nas, ip_ext FROM rttb_pppoe_conectado WHERE srv_ip = '" + this->cfg["geral"]["srv_ip"] + "' ";

	if(username != "") {
		sSQL += " AND username = '"+username+"' ";
	}

	sSQL += " ORDER BY username,interface DESC ";

	//cout << sSQL << endl << endl;

	work *t = new work(*this->conn, "pppoe");
	result r = t->exec(sSQL);

	lista retorno;

	for (result::size_type i = 0; i != r.size(); i++) {
		//lista = r[i];
		reg registro;

		registro["srv_ip"]    = r[i]["srv_ip"].c_str();
		registro["username"]  = r[i]["username"].c_str();
		registro["interface"] = r[i]["interface"].c_str();
		registro["ip"]        = r[i]["ip"].c_str();
		registro["pid"]       = r[i]["pid"].c_str();
		registro["id_nas"]	  = r[i]["id_nas"].c_str();
		registro["ip_ext"]	  = r[i]["ip_ext"].c_str();

		retorno.push_back(registro);
	}

	t->commit();
	delete t;

	return retorno;



}

void AplicativoPPPoE::print_who(string username="") {
	lista l = this->who(username);

	cout.width(20);
	cout << left << "Usuario";
	cout.width(7);
	cout << left << "Iface";
	cout.width(15);
	cout << left << "IP";
	cout.width(7);
	cout << left << "PID";
	cout.width(8);
	cout << left << "ID_NAS";
	cout.width(15);
	cout << left << "IP EXTERNO";




	cout << endl;
	cout.width(80);
	cout << left << "------------------------------------------------------------------------------" ;
	cout << endl;

	for(int i=0;i<l.size();i++) {
		//cout << "Username: " << l[i]["username"] << endl;

		cout.width(20);
		cout << left << l[i]["username"];
		cout.width(7);
		cout << left << l[i]["interface"];
		cout.width(15);
		cout << left << l[i]["ip"];
		cout.width(7);
		cout << left << l[i]["pid"] ;
		cout.width(8);
		cout << left << l[i]["id_nas"];
		cout.width(15);
		cout << left << l[i]["ip_ext"];

		cout << endl;

	}

}


void AplicativoPPPoE::kick(string username="") {
	//int kicked;
	lista l = this->who(username);

	for (int i=0; i<l.size(); i++) {
		kill(atoi(l[i]["pid"].c_str()), SIGHUP);
	}
}

void AplicativoPPPoE::kick(int pid) {
	//int kicked;

	kill((int)pid, SIGHUP);
}

void AplicativoPPPoE::rcstart() {

	SistemaOperacional *so = new SOFreeBSD();

	string endereco = "/mosman/virtex/run/";

	so->installDir( endereco );

	//Inicia todos os servidores
	for( Config::iterator i = pcfg.begin(); i != pcfg.end(); i++ ) {

		string sessao = i->first;

		if (this->ncfg[sessao]["status"] == "up") {
			string comando = "" + endereco + " -d -P /mosman/virtex/run/" + this->pcfg[sessao]["id_nas"].c_str() + ".pid -p '*' -l pppoe-in " + sessao;
			so->executa( comando );
			//cout << comando << endl;
		}
	}

	delete so;

}

void AplicativoPPPoE::rcstop() {

	string endereco = "/mosman/virtex/run/";

	this->kick("");

	int pid;

	for( Config::iterator i = pcfg.begin(); i != pcfg.end(); i++ ) {

		string sessao = i->first;

		string id_nas = this->pcfg[sessao]["id_nas"];

		string arquivo = endereco + "" + id_nas + ".pid";

		ifstream in(arquivo.c_str());

		if(!in) {
			cout << "arquivo " << arquivo << endl;
			continue;
		}


		in >> pid;

		cout << "PPPoE " << pid << "Desligado" << endl;

		kick(pid);

		in.close();

	}

}
