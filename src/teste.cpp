
//include <stdio>
#include <string>
#include <iostream>
//include <fstream>

#include <appteste.h>


using namespace std;
using namespace MOSMAN;

int main(int argc,char **argv) {
   //string arquivo = "teste.ini";
   //Config *cfg = new Config("teste.ini");
   //Config cfg("teste.ini");
   //cfg.parseIni(arquivo);
   // = new Config(arquivo);
   //cout << "------------------------------\n";

   //cout << "TESTE: " << cfg["geral"]["nome"] << "\n";
   //cfg["testemegafocker"] = new Sessao();
   //Sessao *t = cfg.obtem("geral");
   //cout << t["nome"] << "\n";



   AplicativoTeste app;
   //app.init("teste.ini",argc,argv);
   app.executa();
   app.daemon();


}
