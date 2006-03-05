#ifndef _ATUADOR_BANDALARGA_H
#define _ATUADOR_BANDALARGA_H



#include <atuador.h>


class AtuadorBandalarga : public Atuador {

   public:
      AtuadorBandalarga();
      int processa(const string&,const string&,const string&);


};


#endif
