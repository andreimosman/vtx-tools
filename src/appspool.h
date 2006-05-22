#ifndef _APPSPOOL_H
#define _APPSPOOL_H

#include <aplicativo.h>
#include "general.h"

using namespace std;
using namespace MOSMAN;
using namespace pqxx;

class AplicativoSpool : public Aplicativo {

   protected:

      bool doBoot;
      bool doFork;
      bool quiet;

      Config tcfg;

      reg nas_disponivel;

   public:
      AplicativoSpool::AplicativoSpool();
      virtual int init(const string &,int,char**,char **);
      int loop();
      int executa();

      bool estaBootando();
      bool fazerFork();

      int conecta_bd();	// Conecta ao banco usando os parametros do arquivo de configuração


};

#endif
