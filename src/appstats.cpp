
#include <exception>

#include "appstats.h"
#include "sistema_operacional.h"
#include "so_freebsd.h"

#include <pqxx/pqxx>


using namespace std;
using namespace MOSMAN;
using namespace pqxx;

AplicativoStats::AplicativoStats() {

}

int AplicativoStats::conecta_bd() {
	string host = this->cfg["pgsql"]["host"];
	string user = this->cfg["pgsql"]["user"];
	string pass = this->cfg["pgsql"]["pass"];
	string base = this->cfg["pgsql"]["base"];

	return(Aplicativo::conecta_bd(host,user,pass,base));

}


int AplicativoStats::init(const string &arquivo,int argc,char** argv,char** env) {

    Aplicativo::init(arquivo,argc,argv,env);
    this->conecta_bd();

	//this->doBoot = false;
	//this->doFork = true;
	//this->quiet = false;

char ch;

	/**

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

	*/

}

// Executa (atualmente apenas no boot)
int AplicativoStats::executa() {


   SistemaOperacional *soImpl;
   string so = this->cfg["geral"]["SO"];

   // Instancia as implementações específicas do sistema operacional
   if( so == "FreeBSD" ) {
	   soImpl = new SOFreeBSD();
   } else {
	   // Erro de SO desconhecido.
   }

   if( soImpl == NULL ) {
	   return -1;
   }


   // Acessa a funcao de estatisticas

   //soImpl->geraEstatisticas();

















}

// Loop consulta a spool
int AplicativoStats::loop() {

}

/**

bool AplicativoStats::estaBootando() {
	return this->doBoot;
}

bool AplicativoStats::fazerFork() {
	return this->doFork;
}


*/
