
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

   this->doBoot = false;
   this->doFork = true;
   this->quiet = false;



   return(this->conecta_bd(strConn));


}

// Conecta no banco de dados (postgresql)
bool AplicativoSpool::conecta_bd(const string &strConn) {
   this->conn = new connection(strConn);
   return true;
}




int AplicativoSpool::init(const string &arquivo,int argc,char** argv,char** env) {

    Aplicativo::init(arquivo,argc,argv,env);
    this->conecta_bd();

    char ch;

    this->quiet = false;

	while((ch=getopt(argc,argv,"qBb")) != -1 ) {
		switch(ch) {

			case 'B':
			case 'b':

				this->doBoot = true;
				this->doFork = false;
				break;

			case 'q':
				this->quiet = true;
				break;

		}

	}

}

// Executa (atualmente apenas no boot)
int AplicativoSpool::executa() {

	if( this->estaBootando() ) {

		// Obtem as informações deste NAS
		string sSQL;

		sSQL  = "SELECT id_nas,nome,ip,secret,tipo_nas FROM cftb_nas WHERE ip='" + this->cfg["geral"]["srv_ip"] + "' AND tipo_nas='I'";
		work *t = new work(*this->conn,"boot");

		string id_nas;
		string nome_nas;

		result r = t->exec(sSQL);

		for (result::size_type i = 0; i != r.size(); ++i) {
			id_nas   = r[i]["id_nas"].c_str();
			nome_nas = r[i]["nome"].c_str();
		}

		if( id_nas != "" ) {
			// Seleciona os registros de banda larga para este NAS que esteja disponivel

			sSQL  = "SELECT ";
			sSQL += "   c.id_conta,c.username,cbl.rede,cbl.mac,upload_kbps,download_kbps ";
			sSQL += "FROM ";
			sSQL += "   cntb_conta c INNER JOIN cntb_conta_bandalarga cbl USING(username,dominio,tipo_conta) ";
			sSQL += "WHERE ";
			sSQL += "   cbl.id_nas = " + id_nas + " ";
			sSQL += "   AND c.status = 'A' ";
			sSQL += "   AND cbl.rede is not null ";

			result r = t->exec(sSQL);

			Atuador *a = new AtuadorBandalarga(this->cfg);

			for (result::size_type i = 0; i != r.size(); ++i) {

				string id_conta = r[i]["id_conta"].c_str();
				string username = r[i]["username"].c_str();
				string rede     = r[i]["rede"].c_str();
				string mac      = r[i]["mac"].c_str();
				string upload	= r[i]["upload_kbps"].c_str();
				string download = r[i]["download_kbps"].c_str();


				string parametros = rede + "," + mac + "," + upload + "," + download;

				if( !this->quiet ) {
					cout << "Liberando acesso: " << username + "|" + rede + "|" + mac << endl;
				}

				a->processa("a",id_conta,parametros);

			}

			delete a;

		}

	}

}

// Loop consulta a spool
int AplicativoSpool::loop() {

   // Query da spool
   string srv_ip = this->cfg["geral"]["srv_ip"];		// Ip da propria maquina para processamento da spool
   string sSQL = "SELECT id_spool,tipo,destino,op,id_conta,parametros,status FROM sptb_spool WHERE destino = '" + srv_ip + "' AND status = 'A' FOR UPDATE";

   while(1) {

	   work *t = new work(*this->conn,"spool"); // Inicia a transacao
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
			   a = new AtuadorBandalarga(this->cfg);
		   } else if(tipo=="E"){

		   } else if(tipo=="H"){

		   }

		   if( a == NULL ) {
			   // Erro: ATUADOR DESCONHECIDO

		   } else {
			   a->processa(op,id_conta,parametros);
			   delete a;
			   string uSQL = "UPDATE sptb_spool SET status='OK' WHERE id_spool = '" + id_spool + "' ";
			   result u = t->exec(uSQL);
		   }

	   }


	   t->commit(); // Commita a transação


	   delete t;	// Libera a memória usada pela transacao.

	   // Dorme 30 segundos
	   sleep(30);


   }

}

bool AplicativoSpool::estaBootando() {
	return this->doBoot;
}

bool AplicativoSpool::fazerFork() {
	return this->doFork;
}



