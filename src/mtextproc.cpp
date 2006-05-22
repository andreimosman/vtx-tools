
#include <map>
#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>

#include <stdlib.h>
#include <string>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>

#include "general.h"
#include "mtextproc.h"

using namespace std;
using namespace boost;
using namespace MOSMAN;

/**
 * Inicializacoes
 */
MTextProc::MTextProc(){

}

/**
 * Operacao com variaveis
 */
string& MTextProc::operator[](string &var){
   return( this->vars[var] );
}
string& MTextProc::operator[](const char *str){
   string var = str;
   return( this->vars[var] );
}


typedef std::map<std::string, std::string::difference_type, std::less<std::string> > map_type;


/**
 * Processa o texto em in jogando a saida para oout
 */
int MTextProc::processa(istream& in,ostream& out){

   //regex reVAR("([%][^\\s^%]+[%])");
   typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
   
   char_separator<char> sep("%","%",keep_empty_tokens);
    
   while( !in.eof() ) {
      // Obtem a linha
      char buff[4096];
      in.getline(buff,4096);

      if( !strlen(buff) ) continue;
      
      string text=buff;
      
      
      tokenizer tok(text,sep);
      
      int cnt = 0;
      
      for(tokenizer::iterator beg=tok.begin(); beg!=tok.end();++beg) {
         //cout << *beg << "\n";
         
         //out << *beg;

         //out << "TOKEN: " << *beg  << endl;
         //out << "COUNT: " << cnt << endl;
         //out << "CNT%2: " << cnt%2 << endl;

         
         if( *beg == "%" ) {
            cnt++;
            continue;
         }
         
         if( cnt%2 ) {
            //cout << "VARIAVEL: " << *beg << endl;
            //out << "={" << *beg << "=" << this->vars[ *beg ] << "}=";
            out << this->vars[ *beg ];
         } else {
            out << *beg;
         }
         


      }
      
      out << endl;
      
      
      
   }
}

void MTextProc::setVars(reg& vars) {
   this->vars = vars;
}
