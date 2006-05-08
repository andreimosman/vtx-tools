#ifndef _APPRADIUS_H
#define _APPRADIUS_H

#include <pqxx/pqxx>

#include <aplicativo.h>

using namespace std;
using namespace MOSMAN;
using namespace pqxx;

namespace MOSMAN {
	namespace radius_constants {
		static const string log_ok     = "OK";
		static const string log_erro   = "E";
		static const string log_info   = "I";
		static const string log_alerta = "AL";
	};



	class AplicativoRadius : public Aplicativo {

	   protected:
		  connection *conn;
		  bool auth;
		  bool acct;
		  //bool quiet;

		  /**
		   * Variaveis de autenticacao
		   */
		  string username;
		  string password;
		  string foneinfo;

		  /**
		   * Variaveis de accounting
		   */
		  bool entrada;
		  bool saida;
		  string session;
		  string bytes_in;
		  string bytes_out;
		  string nas;
		  string ip_addr;
		  string tempo;
		  string terminate_cause;

	   public:
		  AplicativoRadius::AplicativoRadius();
		  virtual int init(const string &,int,char**,char **);
		  bool conecta_bd();
		  bool conecta_bd(const string &);
		  int loop();
		  int executa();

		  bool autenticando();
		  bool registrando();

		protected:
			void log(const string& tipo, string& username, string& mensagem, string& caller_id, string& nas);
			void acctStart(string& session_id, string& username, string& caller_id, string& nas, string& framed_ip_address);
			void acctStop(string& session_id,string& tempo, string& terminate_cause, string& bytes_in, string& bytes_out);

	};
};

#endif
