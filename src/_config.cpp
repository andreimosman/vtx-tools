
#include <string>
#include <inilib.h>
#include <config.h>


#include <fstream>
#include <iostream>



using namespace std;
using namespace INI;

using namespace MOSMAN;

/**
 * CLASSE Config
 */

Config::Config(const string &arquivo) {
   this->arquivo = arquivo;
   //this->cfg.file_read(arquivo);
   this->parseIni(arquivo);
   
}

Config::~Config() {

}

bool Config::parseIni(const string &arquivo) {
   fstream fd;
   
   // Abre o arquivo somente pra leitura
   fd.open(arquivo.c_str(), ios::in);
   
   if( !fd ) {
      // Erro ao abrir o arquivo
      return false;
   }
   
   string sessao;		// Nome da sessao
   char buffer[1024];		// Buffer (comeca por string vazia
   buffer[0]='\0';
   int bi=0;			// Buffer index
   char c;			// Caracter lido
   char expect;			// Indica o caracter esperado.
   
   // Estados
   bool st_ini_sess=false;	// Inicio de sessao
   bool st_fim_sess=false;	// Fim de sessao
   bool st_ini_linh=true;	// Inicio de linha
   bool st_fim_linh=false;	// Fim da linha (inicio de comentario) - indica o fim da linha antes de \n
   bool st_ini_attr=false;	// Inicio de atributo
   bool st_ini_atvl=false;	// Inicio de valor de atributo
   
   bool st_fim_buff=false;	// Indica o fim do buffer, hora de zerar a variavel.
   
   
   bool st_erro=false;
   
   
   
   int num_linhas = 1;
   
   
   
   string nome_sessao;
   string nome_atributo;
   string valor_atributo;
   
   
   string msg_erro;
   
   
   // Varre o arquivo
   while(fd.good() && (c=fd.get()) != '\0' && !st_erro) {
      cout << "|"<<c<<"|\n";
      switch(c) {
         case '[':
            cout << "GOT[\n";
            // Inicio de nome de sessao
            if( st_ini_linh ) {
               st_ini_attr = false;
            }
            
            if( st_ini_sess ) {
               // Se ja tem o inicio da sessao aberta eh erro.
               msg_erro = "Erro no inicio da sessao.";
               st_erro = true;
               break;	// Volta pro loop pra cair fora
            }
            st_ini_sess = true;
            
            break;

         case ']':
            cout << "GOT]\n";
            // Fim de nome de sessao
            if( !st_ini_sess || st_fim_sess ) {
               // Fim de sessao sem inicio ou ']' repetido
               msg_erro = "Erro no fim da sessao";
               st_erro = true;
               break;
            }
            
            buffer[bi] = '\0';
            st_fim_buff = true;

            st_ini_sess = false;
            st_fim_sess = true;
            nome_sessao = buffer;
            
            break;

         case '=':
            cout << "GOT=\n";
            // Fim de nome de parametro / Inicio de valor do parametro
            st_ini_attr=false;
            st_ini_atvl=true;
            
            buffer[bi] = '\0';
            st_fim_buff = true;
            
            nome_atributo = buffer;
            break;
            
         case ';':            
         case '#':
            cout << "GOT COMMENT\n";
            st_fim_linh = true; // Indica que a linha "interpretavel" terminou.

         case '\r':
            cout << "GOT CR\n";
            // Retorno de carro (formato windows)
            break;

         case '\n':
            cout << "GOT LF\n";
            // Grava o buffer
            buffer[bi] = '\0';
            
            if( st_ini_linh ) {
               num_linhas++;
               break;
            }
            
            // Verifica os erros
            if( st_ini_sess ) {
               // Inicio da sessao sem fechamento na mesma linha
               st_erro = true;
               msg_erro = "Sessao nao finalizada";
               break;
            }
            
            //string tmp(buffer);
            //string tmp = buffer;
            
            
            if( !st_ini_linh && st_ini_attr ) {
               // O atributo nao recebeu valor.
               st_erro = true;
               msg_erro = "Atributo nao recebeu valor";
               break;
            }
            
            // Grava o valor do atributo
            if( st_ini_atvl ) {
               valor_atributo = buffer;
            }
            
            // Zera os status pertinentes.
            st_fim_sess = false;
            st_ini_sess = false;
            st_ini_attr = false;
            st_ini_atvl = false;
            
            num_linhas++;
            
            // O proximo caractere eh inicio de linha
            st_ini_linh = true;
            
            break;

         default:
            // Caracter qualquer
            
            if( st_fim_sess && c != ' ' && c != '\t' ) {
               // Se for fim da sessao e tiver algum outro caracter que nao seja espaco
               // antes do termino da linha eh erro.
               msg_erro = "Caracter invalido apos fim de sessao";
               st_erro = true;
               continue;
            }
            
            // Se for o inicio da linha
            if( st_ini_linh && c != ' ' && c != '\t' ) {
               cout << "INICIO DA PORRA DA LINHA\n";
               st_ini_attr = true;
               st_ini_linh = false;
            }
            
            if( !st_fim_linh ) {
               // Se nao for o fim de uma linha (inicio de comentario) adiciona ao buffer
               
               buffer[bi++]=c;
               buffer[bi]='\0';		// Fim da string
               
            }
            
            if( st_ini_linh ) {
               st_ini_attr = true;
               st_ini_linh = false;
           }
      
      }
      
      if( st_fim_buff ) {
         bi=0;
         buffer[bi] = '\0';
         st_fim_buff=0;
         
         //cout << "Sess: " << nome_sessao << "\n";
         //cout << "Attr: " << nome_atributo << "\n";
         //cout << "AtVl: " << valor_atributo << "\n";
         //cout << "------------------------\n";
         cout << nome_sessao << ":" << nome_atributo << ":" << valor_atributo << "\n";
         
         
         
         
      }
      
   }
   
   fd.close();
   
   if( st_erro ) {
      cout << arquivo << " (" << num_linhas << "): " << msg_erro << "\n";
      return false;
   }
   
   
   return this;
   
   
}

Sessao Config::operator[](const string &sessao) {
   //section sec = this->cfg[sessao];
   //count << this->cfg[indice] << '\n';
}




/**
 * CLASSE Sessao
 */

Sessao::Sessao() {

}

Sessao::~Sessao() {

}

Atributo Sessao::operator[](const string &sessao) {

}

/**
 * CLASSE Atributo
 */

