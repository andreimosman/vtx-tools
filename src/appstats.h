#ifndef _APPSTATS_H
#define _APPSTATS_H

#include <pqxx/pqxx>

#include <aplicativo.h>

using namespace std;
using namespace MOSMAN;
using namespace pqxx;

class AplicativoStats : public Aplicativo {

   protected:

      //bool doBoot;
      //bool doFork;
      //bool quiet;

   public:
      AplicativoStats::AplicativoStats();
      virtual int init(const string &,int,char**,char **);
      int loop();
      int executa();

      //bool estaBootando();
      //bool fazerFork();

      int conecta_bd();	// Conecta ao banco usando os parametros do arquivo de configuração

};

#endif
