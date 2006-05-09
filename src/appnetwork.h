#ifndef _APPNETWORK_H
#define _APPNETWORK_H

#include <pqxx/pqxx>

#include <aplicativo.h>

using namespace std;
using namespace MOSMAN;
using namespace pqxx;

namespace MOSMAN {

	class AplicativoNetwork : public Aplicativo {

	   protected:
		  connection *conn;
		  bool bRC;

	   public:

	      Config ncfg;		// Configuração da rede

		  AplicativoNetwork::AplicativoNetwork();
		  virtual int init(const string &,int,char**,char **);
		  bool conecta_bd();
		  bool conecta_bd(const string &);
		  int loop();
		  int executa();
/**
		protected:
			void log(const string& tipo, string& username, string& mensagem, string& caller_id, string& nas);
			void acctStart(string& session_id, string& username, string& caller_id, string& nas, string& framed_ip_address);
			void acctStop(string& session_id,string& tempo, string& terminate_cause, string& bytes_in, string& bytes_out);
*/
	};
};

#endif
