
#include "appradius.h"
//include "atuador.h"
//include "atuador_bandalarga.h"

#include <pqxx/pqxx>

#include <boost/regex.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace MOSMAN;
using namespace pqxx;
//using namespace boost;

// Utilizado pelo foneinfo
#include <regex.h>


AplicativoRadius::AplicativoRadius() {

}




bool AplicativoRadius::conecta_bd() {

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

   this->auth = false;
   this->acct = false;

   this->entrada = false;
   this->saida = false;

   return(this->conecta_bd(strConn));


}

// Conecta no banco de dados (postgresql)
bool AplicativoRadius::conecta_bd(const string &strConn) {
   this->conn = new connection(strConn);
   return true;
}


int AplicativoRadius::init(const string &arquivo,int argc,char** argv,char** env) {

    Aplicativo::init(arquivo,argc,argv,env);
    this->conecta_bd();

    char ch;

    //this->quiet = false;

	while((ch=getopt(argc,argv,"ACu:w:f:ESs:I:O:n:i:t:c:")) != -1 ) {
		switch(ch) {
			/**
			 * Parametros do tipo da execucao
			 */
			case 'A':
				this->auth = true;
				break;
			case 'C':
				this->acct = true;
				break;

			/**
			 * Parametros de Autenticacao
			 */
			case 'u':
				this->username = optarg;
				break;
			case 'w':
				this->password = optarg;
				break;
			case 'f':
				this->foneinfo = optarg;
				break;

			/**
			 * Parametros de Registro (accounting)
			 */
			case 'E':
				this->entrada = true;
				break;
			case 'S':
				this->saida = true;
				break;
			case 's':
				this->session = optarg;
				break;
			case 'I':
				this->bytes_in = optarg;
				break;
			case 'O':
				this->bytes_out = optarg;
				break;
			case 'n':
				this->nas = optarg;
				break;
			case 'i':
				this->ip_addr = optarg;
				break;
			case 't':
				this->tempo = optarg;
				break;
			case 'c':
				this->terminate_cause = optarg;
				break;

		}

	}

}

// Executa (atualmente apenas no boot)
int AplicativoRadius::executa() {
	string mensagem;


	// Verifica o tipo de operação
	if( (this->registrando() && this->autenticando()) || (!this->registrando() && !this->autenticando()) ) {
		// Erro tem que ter um (e somente um) tipo de operacao.
		return -1;
	}

	// Autenticação do usuário
	if( this->autenticando() ) {
		if( this->username == "" ) {
			mensagem = "Username em branco";
			this->log(radius_constants::log_erro, this->username, mensagem, this->foneinfo, this->nas);
			return -1;
		}
		if( this->password == "" ) {
			mensagem = "Senha em branco";
			this->log(radius_constants::log_erro, this->username, mensagem, this->foneinfo, this->nas);
			return -1;
		}

		if( this->foneinfo == "" ) {
			mensagem = "Caller ID em branco";
			this->log(radius_constants::log_erro, this->username, mensagem, this->foneinfo, this->nas);
			return -1;
		}

		//cout << "Autenticando (AUTH)" << endl;
		//cout << "------------------------------------" << endl;
		//cout << "User...: " << this->username << endl;
		//cout << "Pass...: " << this->password << endl;
		//cout << "Fone...: " << this->foneinfo << endl;

		// Identificar tipo da conta (pelo foneinfo)

		boost::regex mac_pattern("^([0-9a-fA-F]{1,2}:){5}[0-9a-fA-F]{1,2}$");

		string tipo_conta;
		string dominio_padrao = this->cfg["geral"]["dominio_padrao"];


		boost::cmatch what;
		if(boost::regex_match(this->foneinfo.c_str(), what, mac_pattern)) {
			//cout << "MAC!!!" << endl;
			tipo_conta = "BL";
		} else {
			//cout << "FONE!!!" << endl;
			tipo_conta = "D";
		}


		string sSQL;

		// Obter usuario
		sSQL  = "SELECT ";
		sSQL += "	cnt.username,cnt.dominio,cnt.tipo_conta,cnt.senha_cript,cnt.id_cliente_produto,cbl.username,cd.username, ";
		sSQL += "	trim(cnt.status) as st_conta, trim(ctt.status) as st_contrato, ctt.id_cliente_produto as id_contrato, cbl.mac, ";
		sSQL += "   cd.foneinfo, cnt.id_conta, cbl.ipaddr ";
		sSQL += "FROM ";
		sSQL += "   cntb_conta_bandalarga cbl ";
		sSQL += "   RIGHT OUTER JOIN cntb_conta cnt USING(username,dominio,tipo_conta) ";
		sSQL += "   LEFT OUTER JOIN cntb_conta_discado cd USING(username,dominio,tipo_conta), ";
		sSQL += "   cbtb_contrato ctt INNER JOIN cbtb_cliente_produto cp USING(id_cliente_produto), ";
		sSQL += "   prtb_produto_discado pc RIGHT OUTER JOIN prtb_produto p USING(id_produto) ";
		sSQL += "   LEFT OUTER JOIN prtb_produto_bandalarga pbl USING(id_produto) ";
		sSQL += "WHERE ";
		sSQL += "	ctt.id_cliente_produto = cnt.id_cliente_produto ";
		sSQL += "	AND ctt.id_cliente_produto = cnt.id_cliente_produto ";
		sSQL += "   AND p.id_produto = cp.id_produto ";
		sSQL += "	AND cnt.tipo_conta = '" + tipo_conta + "' ";
		sSQL += "	AND cnt.username = '"+this->username+"' ";
		sSQL += "	AND cnt.dominio = '"+dominio_padrao+"' ";
		//sSQL += "	AND cnt.status = 'A' ";
		//sSQL += "   AND ctt.status = 'A' ";

		// Consulta o registro no banco de dados
		work *t = new work(*this->conn,"boot");

		result r = t->exec(sSQL);

		t->commit();


		/**
		 * Dados do banco
		 */
		string username;
		string senha_cript;
		string mac;
		string foneinfo;
		string id_conta;
		string ipaddr;
		string st_conta;
		string st_contrato;
		string id_contrato;

		for(result::size_type i = 0;i != r.size();i++) {
			//cout << "LINHA!" << endl;
			username    = r[i]["username"].c_str();
			senha_cript = r[i]["senha_cript"].c_str();
			mac         = r[i]["mac"].c_str();
			foneinfo    = r[i]["foneinfo"].c_str();
			id_conta 	= r[i]["id_conta"].c_str();
			ipaddr      = r[i]["ipaddr"].c_str();
			st_conta    = r[i]["st_conta"].c_str();
			st_contrato = r[i]["st_contrato"].c_str();
			id_contrato = r[i]["id_contrato"].c_str();


			/**
			cout << "User.....: " << username << endl;
			cout << "Pass.....: " << senha_cript << endl;
			cout << "Contrato.: " << id_contrato << endl;
			cout << "St. Cnt..: [" << st_contrato << "]" << endl;
			cout << "MAC......: " << mac << endl;
			cout << "Fone.....: " << foneinfo << endl;

			cout << "--------------------------------------" << endl;
			*/



		}

		// Usuario inexistente
		if( username == "" ) {
			mensagem = "Usuario inexistente";
			this->log(radius_constants::log_erro, this->username, mensagem, this->foneinfo, this->nas);
			return -1;
		}

		/**
		 * Verificação de status
		 */
		if( st_conta != "A" ) {
			mensagem = "Conta ";
			mensagem += st_conta == "S" ? "suspensa (pagamento)" : "bloqueada";
			this->log(radius_constants::log_erro, this->username, mensagem, this->foneinfo, this->nas);
			return -1;
		}

		if( st_contrato != "A" ) {
			mensagem = "Contrato ";
			mensagem += st_conta == "S" ? "suspenso (pagamento)" : "cancelado";
			this->log(radius_constants::log_erro, this->username, mensagem, this->foneinfo, this->nas);
		}

		/**
		 * Validação da senha
		 */

		// Extrai o sal (MD5: $1$45678901$)
		char salt[13]="0";
		strncpy(salt,senha_cript.c_str(),12);

		// Criptografa a senha recebida (com o salt da senha do bd)
		char sc[64]="0";
		strcpy(sc,crypt(this->password.c_str(),salt));

		// Compara
		if( strcmp(sc,senha_cript.c_str()) != 0 ) {
			// Senha errada
			mensagem = "Senha não confere";
			this->log(radius_constants::log_erro, this->username, mensagem, this->foneinfo, this->nas);
			return -1;
		}

		// Verificacoes especificas
		if( tipo_conta == "BL" ) {
			// Verificações e Validações de Banda Larga

			if( mac == "" ) {
				mensagem = "Registrando MAC";
				this->log(radius_constants::log_info, this->username, mensagem, this->foneinfo, this->nas);

				// MAC NAO CADASTRADO NO BANCO DE DADOS, CADASTRA;
				string uSQL;
				uSQL  = "UPDATE ";
				uSQL += "   cntb_conta_bandalarga ";
				uSQL += "SET ";
				uSQL += "   mac = '" + this->foneinfo + "' ";
				uSQL += "WHERE ";
				uSQL += "   username ilike '" + this->username + "' ";
				uSQL += "   AND dominio = '" + dominio_padrao + "' ";
				uSQL += "   AND tipo_conta = 'BL' ";


				// Nova transação
				t = new work(*this->conn,"boot");
				result u = t->exec(uSQL);
				t->commit();

			} else {
				// Verifica se o MAC bate com o cadastrado no sistema.
				if( this->foneinfo != mac ) {
					// MAC INVALIDO
					mensagem = "MAC não confere";
					this->log(radius_constants::log_erro, this->username, mensagem, this->foneinfo, this->nas);
					return -1;
				}
			}

			// Retorno para o radius
			cout << "Framed-IP-Address = " << ipaddr << endl;
			cout << "Framed-Compression = Van-Jacobson-TCP-IP" << endl;
			cout << "Framed-Protocol = PPP" << endl;
			cout << "Service-Type = Framed-User" << endl;

		} else {
			// Verificações e Validações de Discado

			// Verifica foneinfo.
			// TODO: Verificar se não existe nenhuma flag desabilitando a verificação do foneinfo
			if( foneinfo != "") {

				//regex fone_pattern(fi_pat);
				boost::regex fone_pattern(foneinfo.c_str(),boost::regex_constants::extended | boost::regex_constants::nosubs);

				//boost::cmatch m;
				std::string::const_iterator start, end;
				start = this->foneinfo.begin();
				end = this->foneinfo.end();
				boost::match_results<std::string::const_iterator> what;

				if( ! boost::regex_search(start, end, what, fone_pattern, boost::match_default) ) {
					mensagem = "Telefone não confere";
					this->log(radius_constants::log_erro, this->username, mensagem, this->foneinfo, this->nas);
					// Fone não bate com a regex cadastrada
					return -1;
				}

			}

		}

		return(0);

	}

	// Accounting
	if( this->registrando() ) {

		if( this->entrada && this->saida || (!entrada && !saida) ) {
			// O accouting tem que ser entrada ou saida e nao pode ser ambos
			return -1;
		}

		if( this->entrada ) {
			// ACCT START
			acctStart(this->session, this->username, this->foneinfo, this->nas, this->ip_addr);
		} else {
			// ACCT STOP
			acctStop(this->session,this->tempo,this->terminate_cause,this->bytes_in,this->bytes_out);
		}




		//cout << "Registrando (ACCT) " << endl;
		//cout << "------------------------------------" << endl;
		//cout << "START......: " << this->entrada << endl;
		//cout << "STOP.......: " << this->saida << endl;
		//cout << "SESSION....: " << this->session << endl;
		//cout << "BYTES IN...: " << this->bytes_in << endl;
		//cout << "BYTES OUT..: " << this->bytes_out << endl;
		//cout << "NAS........: " << this->nas << endl;
		//cout << "IP ADDR....: " << this->ip_addr << endl;
		//cout << "TEMPO......: " << this->tempo << endl;
		//cout << "TERM CAUSE.: " << this->terminate_cause << endl;


	}

}

// Loop consulta a radius
int AplicativoRadius::loop() {


}

bool AplicativoRadius::autenticando() {
	return this->auth;
}

bool AplicativoRadius::registrando() {
	return this->acct;
}


/**
 * LOGGING
 */
void AplicativoRadius::log(const string& tipo, string& username, string& mensagem, string& caller_id, string& nas) {
	string sSQL;

	//cout << tipo << ": " << mensagem << endl;

	//return;

	// Transaction
	work *t = new work(*this->conn,"boot");
	result r;

	// RDTB_LOG (Log geral)
	sSQL  = "INSERT INTO ";
	sSQL += "   rdtb_log(";
	sSQL += "      tipo,username,mensagem,caller_id ";
	sSQL += "   ) VALUES (";
	sSQL += "      '"+tipo+"', ";
	sSQL += "      '"+username+"', ";
	sSQL += "      '"+mensagem+"', ";
	sSQL += "      '"+caller_id+"' ";
	sSQL += "   ) ";

	r = t->exec(sSQL);

	// RDTB_ACCOUNTING (Log de Accounting)
	sSQL  = "INSERT INTO ";
	sSQL += "   rdtb_accounting( ";
	sSQL += "      session_id,username,logout,tempo,caller_id,nas,terminate_cause ";
	sSQL += "   ) VALUES ( ";
	sSQL += "      '"+tipo+":'||nextval('rdsq_id_accounting'), ";
	sSQL += "      '"+username+"', ";
	sSQL += "      now(), ";
	sSQL += "      0, ";
	sSQL += "      '"+caller_id+"', ";
	sSQL += "      '"+nas+"', ";
	sSQL += "      '"+mensagem+"' ";
	sSQL += "   ) ";

	r = t->exec(sSQL);

	t->commit();

}

/**
 * Accounting
 */

void AplicativoRadius::acctStart(string& session_id, string& username, string& caller_id, string& nas, string& framed_ip_address) {

	string sSQL;

	work *t = new work(*this->conn,"boot");
	result r;

	sSQL  = "INSERT INTO ";
	sSQL += "   rdtb_accounting( ";
	sSQL += "      session_id,username,login,caller_id,nas,framed_ip_address ";
	sSQL += "   ) VALUES ( ";
	sSQL += "      '"+session_id+"', ";
	sSQL += "      '"+username+"', ";
	sSQL += "      now(), ";
	sSQL += "      '"+caller_id+"', ";
	sSQL += "      '"+nas+"', ";
	sSQL += "      '"+framed_ip_address+"' ";
	sSQL += "   ) ";

	r = t->exec(sSQL);

	t->commit();

}

void AplicativoRadius::acctStop(string& session_id,string& tempo, string& terminate_cause, string& bytes_in, string& bytes_out) {

	work *t = new work(*this->conn,"boot");
	result r;

	string sSQL;

	sSQL  = "UPDATE ";
	sSQL += "   rdtb_accounting ";
	sSQL += "SET ";
	sSQL += "   logout=now(), ";
	sSQL += "   tempo='"+tempo+"', ";
	sSQL += "   terminate_cause='"+terminate_cause+"', ";
	sSQL += "   bytes_in='"+bytes_in+"', ";
	sSQL += "   bytes_out='"+bytes_out+"' ";
	sSQL += "WHERE   ";
	sSQL += "   session_id = '"+session_id+"'";

	r = t->exec(sSQL);
	t->commit();

}





