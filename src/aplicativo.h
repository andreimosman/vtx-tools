#ifndef MFW__APLICATIVO_H
#define MFW__APLICATIVO_H

#include <map>
#include <string>
#include <fstream>
#include <iostream>

#include <pqxx/pqxx>

#include <config.h>

using namespace std;
using namespace pqxx;



namespace MOSMAN {

	class Aplicativo {

	   protected:
			string arquivo; 	// Arquivo de configuração utilizado
			int argc;			// Numero de argumentos passados na linha de comando
			char **argv;		// Argumentos enviados pela linha de comando
			char **env;		// Variáveis de ambiente
			connection_base *conn;


	   public:
			Config cfg;	// Ponteiro para o configurador.

			Aplicativo();
			virtual int init(const string&,int,char**,char**);	// Recebe arquivo de configuração, argc e argv;
			virtual int executa();		// Código principal do aplicativo;
			virtual int loop();			// Main loop do sistema (utilizado via fork);
			int daemon();					// Execucao via fork();

			bool conecta_bd(const string &host,const string& user, const string& pass, const string& base);
			bool conecta_bd(const string &);



	};




};













#endif
