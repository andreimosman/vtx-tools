#ifndef _APPSPOOL_H
#define _APPSPOOL_H

#include <pqxx/pqxx>

#include <aplicativo.h>

using namespace std;
using namespace MOSMAN;
using namespace pqxx;

class AplicativoSpool : public Aplicativo {

   protected:
      connection *conn;

   public:
      AplicativoSpool::AplicativoSpool();
      virtual int init(const string &,int,char**,char **);
      bool conecta_bd();
      bool conecta_bd(const string &);
      int loop();

};

#endif
