#ifndef MFW__CONFIG_H
#define MFW__CONFIG_H

/**
 * config.h
 * Manipulacao de arquivos de configuracao.
 *
 */


#include <map>
#include <string>

using namespace std;


// Simbolos utilizados pelo parser
#define SIMB_SEM_SIMB		0	/** Sem simbolo - uma string */
#define SIMB_INI_SESS 		1	/** Inicio de sessao */
#define SIMB_FIM_SESS 		2	/** Fim de sessao */
#define SIMB_OP_IGUAL 		3	/** Operador 'Igual' */
#define SIMB_COMENT		4	/** Comentario */
#define SIMB_CLRF		5	/** CLRF */

namespace MOSMAN {

   class Atributo {

   };

   class Sessao {

	   public:
	      typedef map<string,string> mapaAtributos;

	   protected:
	      mapaAtributos atributos;

	   public:
	      Sessao();
	      ~Sessao();
	      string& obtem(const std::string&);
	      string& operator[](const std::string&);


   };

   class Config {

	   public:
	      typedef map<string,Sessao> mapaSessoes;
		  typedef mapaSessoes::iterator	iterator;

	   protected:
	      mapaSessoes sessoes;
	      string arquivo;


	   public:
	      Config();
	      Config(const std::string&);
	      ~Config();
	      bool parseIni(const string&);
	      Sessao& obtem(const std::string&);
	      Sessao& operator[](const std::string& );
	      iterator begin();
	      iterator end();


   };


};






#endif
