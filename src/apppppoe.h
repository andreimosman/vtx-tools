#ifndef _APPRADIUS_H
#define _APPRADIUS_H

#include <pqxx/pqxx>

#include <aplicativo.h>

#include "general.h"

using namespace std;
using namespace MOSMAN;
using namespace pqxx;

class AplicativoPPPoE : public Aplicativo {

   protected:
      connection *conn;
      bool linkup;
      bool linkdown;
      bool bWho;
      bool bKick;
      bool bRC;
      //bool quiet;

      reg nas_disponiveis;

      string username;
      string hisaddr;
      string interface;
      string pid;
      //string srv_ip;
      //string id_nas;
      //string ip_ext;

   public:
      AplicativoPPPoE::AplicativoPPPoE();
      virtual int init(const string &,int,char**,char **);
      bool conecta_bd();
      bool conecta_bd(const string &);
      int loop();
      int executa();

      bool linkingUP();
      bool linkingDOWN();

      Config pcfg;
      Config ncfg;

   protected:
      lista who(string username);
      void print_who(string username);
      void kick(string username);
      void kick(int pid);
      void rcstart();
      void rcstop();

};

#endif
