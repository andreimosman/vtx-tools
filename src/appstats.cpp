#include <exception>

#include "appstats.h"
#include "sistema_operacional.h"
#include "so_freebsd.h"

#include "general.h"
#include "mif.h"


#include <pqxx/pqxx>

//#include <mif.h>

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

	this->doBoot = false;
	this->doFork = true;
	this->quiet = false;

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

	}*/
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

	cout << "Erro 1" << endl;
	// Acessa a funcao de estatisticas

	bag stats = soImpl->obtemEstatisticas();

	cout << "erro 2" << endl;

	/**
	* Seleciona os usuarios do VIRTEX (ativos e inativos)
	*/

	string sql = "SELECT username,up_kbps,down_kbps FROM cntb_conta_bandalarga";

	work *t = new work(*this->conn,"stats");

	result r = t->exec(sql);

	lista clientes; // Vector de reg.

	// Configuracoes do MRTG
	string mrtg = "/usr/local/bin/mrtg";

	string arqbase="/mosman/virtex/etc/mrtg-base.cfg";

	string workdir="/mosman/virtex/stats";
	string refresh="300";
	string interval="5";

	string cat="/bin/cat";



	MIF m;



	for (result::size_type i = 0; i != r.size(); ++i) {
	  // Dados cadastrados
	  reg registro;
	  registro["username"] = r[i]["username"].c_str();
	  registro["up_kbps"] = r[i]["up_kbps"].c_str();
	  registro["down_kbps"] = r[i]["down_kbps"].c_str();

	  registro["workdir"]  = workdir;
	  registro["refresh"]  = refresh;
	  registro["interval"] = interval;

	  string arqst=workdir+"/"+registro["username"]+".coleta";

	  long maxup = atol( registro["up_kbps"].c_str() ) * 1000 / 8;
	  long maxdn = atol( registro["down_kbps"].c_str() ) * 1000 / 8;

	  char maxbytes[128];
	  snprintf((char *)&maxbytes,128,"%d",(maxup>maxdn?maxup:maxdn));

	  registro["maxbytes"]=maxbytes;
	  registro["target"] = "`" + cat + " " + arqst + "`";


	  string arqcf= workdir + "/" + registro["username"] + ".cfg";
	  // Verifica se o arquivo de configuracoes existe:
	  FILE *fd=fopen(arqcf.c_str(),"r");
	  if(!fd) {
		 // Criar o arquivo
		 m.criaConf(registro,arqbase,arqcf);
	  } else {
		 fclose(fd);
  	  }



  	  // Faz a coleta das estatisticas do firewall
	  string user = registro["username"];

	  if(stats[user]["up"] == "")
	    stats[user]["up"] = "0";

	  if(stats[user]["down"] == "")
	    stats[user]["down"] = "0";

	  /**
	  * Faz a coleta das estatisticas gerais
	  * para uso do MRTG.
	  */
	  ofstream out;
	  out.open(arqst.c_str());
	  m.mrtgStat(registro["username"],stats,out);
	  out.close();

	  clientes.push_back(registro);

	}

	delete t;



	/**
    * Chama MRTG e faz a atualizacao das de estatisticas
    */

	m.stats(clientes,workdir,mrtg);

}



// Loop consulta a spool
int AplicativoStats::loop() {

	while(true) {
	  this->executa();
	  sleep(60);        // minuto em minuto
	  // Executa de 5 em 5 minutos
	}

}

/**/

bool AplicativoStats::estaBootando() {
	return this->doBoot;
}

bool AplicativoStats::fazerFork() {
	return this->doFork;
}


