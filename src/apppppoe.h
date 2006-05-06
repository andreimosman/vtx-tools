#ifndef _APPRADIUS_H
#define _APPRADIUS_H

#include <pqxx/pqxx>

#include <aplicativo.h>

using namespace std;
using namespace MOSMAN;
using namespace pqxx;

class AplicativoPPPoE : public Aplicativo {

   protected:
      connection *conn;
      bool linkup;
      bool linkdown;
      //bool quiet;
      
      string username;
      string hisaddr;
      string interface;
      string pid;
      
   public:
      AplicativoPPPoE::AplicativoPPPoE();
      virtual int init(const string &,int,char**,char **);
      bool conecta_bd();
      bool conecta_bd(const string &);
      int loop();
      int executa();

      bool linkingUP();
      bool linkingDOWN();


};

#endif
