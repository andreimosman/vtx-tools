#include <string>
#include <map>
#include <fstream>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include <config.h>

using namespace std;
using namespace boost;
using namespace MOSMAN;


/***************************************************
 * CLASSE Config                                   *
 * Contem mapa de sessões                          *
 ***************************************************/




// Construtor
Config::Config() {
}

Config::Config(const string &arquivo) {
   this->arquivo = arquivo;
   this->parseIni(arquivo);
}

// Destrutor
Config::~Config() {

}



/**
 * Parser de arquivos.ini
 */
bool Config::parseIni(const string &arquivo) {
   fstream fd;

   // Abre o arquivo somente pra leitura
   fd.open(arquivo.c_str(), ios::in);

   if( !fd ) {
      // Erro ao abrir o arquivo
      cout << "Erro ao abrir o arquivo: " << arquivo << "\n";
      return false;
   }

   string buffer;	// Buffer
   char c;			// Caracter

   int  simbolo        = SIMB_SEM_SIMB;
   int  ultimo_simbolo = simbolo;


   string sessao="",ultima_sessao="";
   string atributo="";

   // Tratamento de erro
   bool st_erro=false;
   int num_linha = 1;
   string msg_erro;

   // Varre o arquivo
   while(fd.good() && !fd.eof() && (c=fd.get()) != '\0' && !st_erro) {
      // Analise Lexica (getSimbol)
      switch(c) {
         case '[':
            simbolo = SIMB_INI_SESS;
            break;
         case ']':
            simbolo = SIMB_FIM_SESS;
            break;
         case '=':
            simbolo = SIMB_OP_IGUAL;
            break;
         case ';':
         case '#':
            simbolo = SIMB_COMENT;
            break;
         case '\r':
         case '\n':
            simbolo = SIMB_CLRF;
            break;
         default:
            simbolo = SIMB_SEM_SIMB;
            buffer+= c;
            trim(buffer);
            break;
      }


      // Analise sintatica baseada em linha
      if( simbolo != SIMB_SEM_SIMB ) {
         switch(simbolo) {

            case SIMB_INI_SESS:
               break;

            case SIMB_FIM_SESS:
               // Se for o fim de uma sessao verifica se o ultimo simbolo foi um inicio;
               if( ultimo_simbolo == SIMB_INI_SESS ) {
                  // cout << "SESSAO: " << buffer << "\n";

                  ultima_sessao = sessao;
                  sessao = buffer;

                  //this->adiciona(sessao);
                  //this->sessoes[sessao] = new Sessao();

               } else {
                  // ERRO
               }
               break;

            case SIMB_OP_IGUAL:
               if( ultimo_simbolo == SIMB_CLRF ) {
                  // cout << "ATRIBUTO: " << buffer << "\n";
                  atributo=buffer;
               } else {
                  // ERRO
               }
               break;

            case SIMB_COMENT:
            case SIMB_CLRF:
               switch(ultimo_simbolo) {
                  case SIMB_OP_IGUAL:
                     //cout << "VALOR: " << buffer << "\n";
                     //Sessao *s=this->sessoes[sessao];
                     //this->sessoes[sessao][atributo] = new string(buffer);
                     //s[atributo] = new string(buffer);
                     this->sessoes[sessao][atributo] = buffer;
                     break;
                  case SIMB_COMENT:
                     break;
               }


               break;
         }

         ultimo_simbolo = simbolo;
         simbolo = SIMB_SEM_SIMB;

         buffer = "";



	  }

   }


}

//void Config::adiciona(const string& chave) {
//   string lchave = chave;
//   trim(lchave);			// Chave em lower case
//   to_lower(lchave);
//   this->sessoes.insert(pair<string,Sessao*>(lchave,new Sessao()));
//}


Sessao& Config::obtem(const std::string& chave) {
	string lchave = chave;
	trim(lchave);
	to_lower(lchave);

	return sessoes[lchave];
}

Sessao& Config::operator[](const std::string& chave) {
	return this->obtem(chave);
}




/***************************************************
 * CLASSE Sessao                                   *
 * Contem mapa de atributos                        *
 ***************************************************/

Sessao::Sessao() {
   //atributos = new mapaAtributo();
}

Sessao::~Sessao() {

}

string& Sessao::obtem(const std::string& chave) {
   string lchave = chave;
   trim(lchave);
   to_lower(lchave);
   return( this->atributos[lchave] );
}

string& Sessao::operator[](const std::string& chave) {
	return(this->obtem(chave));
}





