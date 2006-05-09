
#include "apppppoe.h"
//include "atuador.h"
//include "atuador_bandalarga.h"

#include "sistema_operacional.h"
#include "so_freebsd.h"

#include <pqxx/pqxx>

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

    char ch;

    //this->quiet = false;

	while((ch=getopt(argc,argv,"UDi:a:u:p:")) != -1 ) {
		switch(ch) {
			/**
			 * Parametros do tipo da execucao
			 */
			case 'U':
				this->linkup = true;
				break;
			case 'D':
				this->linkdown = true;
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
	sSQL += "	cn.id_conta, cbl.mac, cbl.upload_kbps, cbl.download_kbps ";
	sSQL += "FROM ";
	sSQL +=	"	cntb_conta_bandalarga cbl INNER JOIN cntb_conta cn USING(username, tipo_conta) ";
	sSQL += "WHERE ";
	sSQL += "	cn.username = '" + this->username + "' ";
	sSQL += " 	AND cn.dominio = '" + dominio_padrao + "' ";
	sSQL += "	AND cn.tipo_conta = 'BL' ";


	work *t = new work(*this->conn,"pppoe"); // Inicia a transacao
   	result r = t->exec(sSQL);


	if( r.size() == 0) {
		//Erro
		return -1;
	}

	string id_conta		=	r[0]["id_conta"].c_str();
	string upload_kbps 	=	r[0]["upload_kbps"].c_str();
	string download_kbps 	=	r[0]["download_kbps"].c_str();
	string mac		=	r[0]["mac"].c_str();
	string ip_cliente	=	this->hisaddr.c_str();


	SistemaOperacional *so = new SOFreeBSD();


	if( this->linkingUP() ) {
		//string sSQL = "";

		if( this->username.size() == 0 || this->hisaddr.size() == 0 || this->interface.size() == 0 || this->pid.size() == 0 ) {
			return -1;
		}

		/**
		cout << "Linking UP (Carregar configuracoes)" << endl;
		cout << "------------------------------------" << endl;
		cout << "User...: " << this->username << endl;
		cout << "Int....: " << this->interface << endl;
		cout << "IP.....: " << this->hisaddr << endl;
		cout << "PID....: " << this->pid << endl;
		*/

		// Pegar as informacoes do banco de dados


		// IPFW ADD / IPFW PIPE
		so->adicionaRegraBW(atoi(id_conta.c_str()),SO_FW_IP_BASERULE,SO_FW_IP_BASEPIPE_IN,SO_FW_IP_BASEPIPE_OUT,this->cfg["geral"]["int_if"],this->cfg["geral"]["ext_if"],ip_cliente,mac,atoi(upload_kbps.c_str()),atoi(download_kbps.c_str()),this->username);

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
	}

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



