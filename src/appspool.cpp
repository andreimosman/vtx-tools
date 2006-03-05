
#include "appspool.h"
#include "atuador.h"
#include "atuador_bandalarga.h"

#include <pqxx/pqxx>

using namespace std;
using namespace MOSMAN;
using namespace pqxx;

AplicativoSpool::AplicativoSpool() {

}




bool AplicativoSpool::conecta_bd() {

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



   return(this->conecta_bd(strConn));


}

// Conecta no banco de dados (postgresql)
bool AplicativoSpool::conecta_bd(const string &strConn) {
   this->conn = new connection(strConn);
   return true;
}




int AplicativoSpool::init(const string &arquivo,int argc,char** argv,char** env) {
    cout << "INIT\n";
    Aplicativo::init(arquivo,argc,argv,env);


    this->conecta_bd();

    //this->conn = new connection("dbname=virtex");

    //cout << "DB: " << this->cfg["pgsql"]["host"] << "\n";



}

// Loop consulta a spool
int AplicativoSpool::loop() {

   // Query da spool
   string srv_ip = this->cfg["geral"]["srv_ip"];		// Ip da propria maquina para processamento da spool
   string sSQL = "SELECT id_spool,tipo,destino,op,id_conta,parametros,status FROM sptb_spool WHERE destino = '" + srv_ip + "' AND status = 'A' FOR UPDATE";

   while(1) {

	   //
	   work *t = new work(*this->conn,"spool"); // Inicia a transacao

	   //cout << "RODANDO SQL: " << sSQL << "\n";

	   result r = t->exec(sSQL);

	   for (result::size_type i = 0; i != r.size(); ++i) {
		   // Trabalha com os valores...
		   Atuador *a;

		   string id_spool = r[i]["id_spool"].c_str();
		   string tipo = r[i]["tipo"].c_str();
		   string op = r[i]["op"].c_str();
		   string id_conta = r[i]["id_conta"].c_str();
		   string parametros = r[i]["parametros"].c_str();


		   if( tipo=="BL" ){
			   a = new AtuadorBandalarga();
		   } else if(tipo=="E"){

		   } else if(tipo=="H"){

		   }

		   cout << "RODANDO ATUADOR (1)\n";


		   if( a == NULL ) {
			   // Erro: ATUADOR DESCONHECIDO

		   } else {
			   a->processa(op,id_conta,parametros);
			   delete a;
		   }

		   cout << "ATUADOR OK\n";



		   //cout << "ID SPOOL..: " << id_spool << "\n";
		   //cout << "TIPO......: " << tipo << "\n";
		   //cout << "OP........: " << op << "\n";
		   //cout << "ID CONTA..: " << id_conta << "\n";
		   //cout << "PARAMETROS.: " << parametros << "\n";



	   }


	   t->commit(); // Commita a transação


	   delete t;	// Libera a memória usada pela transacao.

	   // Dorme 30 segundos
	   sleep(30);


   }

}
