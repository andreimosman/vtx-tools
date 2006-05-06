#ifndef _ATUADOR_REDESUPORTE_H
#define _ATUADOR_REDESUPORTE_H



#include <atuador.h>

#include <config.h>

class AtuadorRedeSuporte : public Atuador {
	protected:
		Config cfg;


   public:
      AtuadorRedeSuporte(Config& cfg);
      int processa(const string&,const string&,const string&);


};


#endif
