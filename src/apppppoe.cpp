
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
	
	if( this->linkingUP() && this->linkingDOWN() ) {
	   // Erro
	   return -1;
	}
	
	SistemaOperacional *so = new SOFreeBSD();
	
	if( this->linkingUP() ) {
		if( this->username.size() == 0 || this->hisaddr.size() == 0 || this->interface.size() == 0 || this->pid.size() == 0 ) {
			return -1;
		}
		cout << "Linking UP (Carregar configuracoes)" << endl;
		cout << "------------------------------------" << endl;
		cout << "User...: " << this->username << endl;
		cout << "Int....: " << this->interface << endl;
		cout << "IP.....: " << this->hisaddr << endl;
		cout << "PID....: " << this->pid << endl;
		
		// Pegar as informacoes do banco de dados
		
		// IPFW ADD / IPFW PIPE
		
		
	} 
	
	if( this->linkingDOWN() ) {
		if( this->username.size() == 0 || this->interface.size() == 0 ) {
		   return -1;
		}
		cout << "Linking DOWN (excluir configuracoes) " << endl;
		cout << "------------------------------------" << endl;
		cout << "User...: " << this->username << endl;
		cout << "Int....: " << this->interface << endl;	
		
		// IPFW DELETE
		
	}
	
	cout << "-----------" << endl;

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



