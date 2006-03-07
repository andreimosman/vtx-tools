#ifndef _ATUADOR_BANDALARGA_H
#define _ATUADOR_BANDALARGA_H



#include <atuador.h>

#include <config.h>

class AtuadorBandalarga : public Atuador {
	protected:
		Config cfg;


   public:
      AtuadorBandalarga(Config& cfg);
      int processa(const string&,const string&,const string&);


};


#endif
