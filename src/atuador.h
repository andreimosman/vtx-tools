#ifndef _ATUADOR_H
#define _ATUADOR_H

#include <string>

#include <config.h>

using namespace std;
using namespace MOSMAN;

class Atuador {

   public:
   	  Atuador();
      virtual int processa(const string&,const string&,const string&);



};


#endif
