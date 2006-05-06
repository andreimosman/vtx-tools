
#include <sys/time.h>

#include "general.h"
#include "mif.h"
#include "mtextproc.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <istream>



using namespace std;
using namespace MOSMAN;

MIF::MIF() {

}

/**
 * Cria um arquivo de configuracao com base no arquivo base e na bag info
 * dentro do arquivo destino
 */

int MIF::criaConf(reg& info,string& arquivobase,string& arquivodestino) {
   MTextProc tp;
   
   ifstream in;
   in.open(arquivobase.c_str());
   
   ofstream out;
   out.open(arquivodestino.c_str());
   
   tp.setVars(info);
   
   tp.processa(in,out);
   
}

/**
 * Joga as estatisticas no formato do MRTG para o usuario especificado 
 * na saida indicada.
 * Funcao para utilizacao no programa externo chamado pelo MRTG
 */

int MIF::mrtgStat(string& username,bag& stats,ostream& out) {
   out << stats[username]["down"] << endl;
   out << stats[username]["up"] << endl;
   out << "0" << endl;
   out << "0";
}

int MIF::mrtgStat(string& username,bag& stats) {
   this->mrtgStat(username,stats,cout);
}

/**
 * Chama o MRTG para a lista de clientes;
 * 
 */

int MIF::stats(lista& clientes,string& path,string &mrtg) {

   if( mrtg == "" ) {
      mrtg = "/usr/local/bin/mrtg";
   }

   for(int i=0;i<clientes.size();i++) {
      string arquivo = path + "/" + clientes[i]["username"] + ".cfg";
      
      FILE *fd = fopen(arquivo.c_str(),"r");
      
      if( fd ) {
         fclose(fd);
         string comando = mrtg + string(" ") + arquivo + string(" >/dev/null 2>&1 ");
         system(comando.c_str());
      }
   
   }


}

/**
int MIF::savestats(lista& clientes,reg& stats,string& path) {

   //string rrdtool = "/usr/local/bin/rrdtool";
   //string rrdupdate = "/usr/local/bin/rrdupdate";
   //string install = "/usr/bin/install";
   
   // Pega a timestam;
   
   time_t t;
   time((time_t *)&t);   
   char tstamp[32];
   snprintf((char *)&tstamp,32,"%d",t);

   for(int i=0;i<clientes.size();i++) {   
       //cout << "Salvando estatisticas para cliente '" << clientes[i] << "' " << endl;
       
       string arquivo = path + "/" + clientes[i] + ".rrd";
       
       string comando;
       
       // Verificar se o arquivo existe
       FILE *fd = fopen(arquivo.c_str(),"r");
       if( !fd ) {
          cout << "CRIANDO ARQUIVO " << arquivo << endl;
          // Cria o diretorio
          comando = install + " -d " + path;
          system(comando.c_str());
          

          // Criar arquivo
          //comando = rrdtool + " create " + arquivo + " DS:up:COUNTER:600:U:U DS:down:COUNTER:600:U:U RRA:AVERAGE:0.5:1:600 RRA:AVERAGE:0.5:6:700 RRA:AVERAGE:0.5:24:775 RRA:AVERAGE:0.5:288:797 RRA:MAX:0.5:1:600 RRA:MAX:0.5:6:700 RRA:MAX:0.5:24:775 RRA:MAX:0.5:288:797";
          //system(comando.c_str());
          
       } else {
          fclose(fd);
       }
       
       string up = stats[ clientes[i] ]["up"];
       string dn = stats[ clientes[i] ]["down"];
       
       if( up == "" ) up = "0";
       if( dn == "" ) dn = "0";
       
       // Atualizar arquivo rrd
       comando = rrdupdate + " " + arquivo + " --template up:down " + (char *)tstamp + ":" + up + ":" + dn;
       //cout << comando << endl;
       system(comando.c_str());
       
   }

}
*/
