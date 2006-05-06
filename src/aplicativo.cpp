


#include <map>
#include <string>
#include <fstream>
#include <iostream>


#include "aplicativo.h"




using namespace std;
using namespace MOSMAN;

Aplicativo::Aplicativo() {

}

int Aplicativo::init(const string &arquivo,int argc,char** argv,char **env) {
	this->arquivo = arquivo;
	this->argc = argc;
	this->argv = argv;
	this->env = env;
	//this->cfg = new Config(arquivo);
	this->cfg.parseIni(arquivo);

}

// Execução stand-alone
int Aplicativo::executa(){
   //cout << "NOME: " << cfg["geral"]["nome"] << "\n";
   cout << "DENTRO\n";
}

// Loop principal chamado pelo daemon
int Aplicativo::loop() {

}


// Rotina de inicalização do daemon e fork
int Aplicativo::daemon() {

	int pid;

	if( (pid=fork()) < 0 ) {
		// Erro, não foi possível bifurcar (fork).
		exit(-1);
	}

	if(pid!=0) {
		// Processo pai, finaliza
		exit(0);
	}

	// Executa o loop principal do aplicativo
	loop();

}

bool Aplicativo::conecta_bd(const string &host,const string& user, const string& pass, const string& base) {

   // Monta a string de conexão.
   string strConn;

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

   return(this->conecta_bd(strConn));


}

// Conecta no banco de dados (postgresql)
bool Aplicativo::conecta_bd(const string &strConn) {
	bool conectado = false;
	this->conn = new connection(strConn);

	return true;
}
