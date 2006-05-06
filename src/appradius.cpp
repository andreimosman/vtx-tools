
#include "appradius.h"
//include "atuador.h"
//include "atuador_bandalarga.h"

#include <pqxx/pqxx>

using namespace std;
using namespace MOSMAN;
using namespace pqxx;

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

	if( this->autenticando() ) {
		cout << "Autenticando (AUTH)" << endl;
		cout << "------------------------------------" << endl;
		cout << "User...: " << this->username << endl;
		cout << "Pass...: " << this->password << endl;
		cout << "Fone...: " << this->foneinfo << endl;
		
	} 
	
	if( this->registrando() ) {
		cout << "Registrando (ACCT) " << endl;
		cout << "------------------------------------" << endl;
		cout << "START......: " << this->entrada << endl;
		cout << "STOP.......: " << this->saida << endl;
		cout << "SESSION....: " << this->session << endl;
		cout << "BYTES IN...: " << this->bytes_in << endl;
		cout << "BYTES OUT..: " << this->bytes_out << endl;
		cout << "NAS........: " << this->nas << endl;
		cout << "IP ADDR....: " << this->ip_addr << endl;
		cout << "TEMPO......: " << this->tempo << endl;
		cout << "TERM CAUSE.: " << this->terminate_cause << endl;
		

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



