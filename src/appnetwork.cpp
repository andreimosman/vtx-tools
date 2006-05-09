#include "appnetwork.h"

#include "virtex.h"

#include <pqxx/pqxx>

#include <boost/regex.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace MOSMAN;
using namespace pqxx;


AplicativoNetwork::AplicativoNetwork() {

}

bool AplicativoNetwork::conecta_bd() {

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

   //this->auth = false;
   //this->acct = false;

   //this->entrada = false;
   //this->saida = false;

   return(this->conecta_bd(strConn));


}

// Conecta no banco de dados (postgresql)
bool AplicativoNetwork::conecta_bd(const string &strConn) {
   this->conn = new connection(strConn);
   return true;
}

int AplicativoNetwork::init(const string &arquivo,int argc,char** argv,char** env) {
    Aplicativo::init(arquivo,argc,argv,env);
    this->conecta_bd();

    this->ncfg.parseIni(NETWORK_INI);


    char ch;

    //this->quiet = false;

	while((ch=getopt(argc,argv,"R")) != -1 ) {
		switch(ch) {
			/**
			 * Parametros do tipo da execucao
			 */
			case 'R':
				// RC (boot)
				this->bRC = true;
				break;
		}
	}


}

// Executa (atualmente apenas no boot)
int AplicativoNetwork::executa() {

	//cout << "Teste: " << ncfg["bge0"]["type"] << endl;

	// Script de inicializacao.
	if( this->bRC ) {

		// Varre o RC

		for( Config::iterator i = ncfg.begin(); i != ncfg.end(); i++ ) {
			// cout << "ACHOU ALGO: " << i->first << endl;

			string sessao = i->first;

			//cout << "SESSAO: " << sessao << endl;

			string status  = ncfg[sessao]["status"];
			string type    = ncfg[sessao]["type"];
			string ipaddr  = ncfg[sessao]["ipaddr"];
			string netmask = ncfg[sessao]["netmask"];
			string gateway = ncfg[sessao]["gateway"];
			string nat     = ncfg[sessao]["nat"];


			cout << "Interface.: " << sessao << endl;
			cout << "Status....: " << status << endl;
			cout << "Type......: " << type << endl;
			cout << "IPADDR....: " << ipaddr << endl;
			cout << "NETMASK...: " << netmask << endl;
			cout << "GATEWAY...: " << gateway << endl;
			cout << "NAT.......: " << nat << endl;
			cout << "----------------------------------" << endl;

			//cout << "" << << endl;
			//cout << "" << << endl;












		}








	}




}

int AplicativoNetwork::loop() {

}
