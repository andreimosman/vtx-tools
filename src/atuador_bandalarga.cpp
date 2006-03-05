#include <string>
#include <iostream>
#include <atuador_bandalarga.h>
#include <rede.h>

using namespace std;
using namespace MOSMAN;

AtuadorBandalarga::AtuadorBandalarga() {
}

int AtuadorBandalarga::processa(const string &op,const string &id_conta,const string &parametros) {
   cout << "Rodando atuador banda larga: " << op << ", " << id_conta << ", " << parametros << "\n";

   //cout << "X\n";

   Rede rede1("192.168.0.3/25");
   cout << "192.168.0.3/25\n--------------------------\n";
   cout << "WC....: " << rede1.obtemWildcard() << "\n";
   cout << "NM....: " << rede1.obtemMascara() << "\n";
   cout << "NW....: " << rede1.obtemEnderecoRede() << "\n";
   cout << "BC....: " << rede1.obtemEnderecoBroadcast() << "\n";
   cout << "PIP...: " << rede1.obtemPrimeiroIP() << "\n";
   cout << "UIP...: " << rede1.obtemUltimoIP() << "\n";

   Rede rede2("192.168.120.17/23");
   cout << "192.168.120.17/23\n--------------------------\n";
   cout << "WC....: " << rede2.obtemWildcard() << "\n";
   cout << "NM....: " << rede2.obtemMascara() << "\n";
   cout << "NW....: " << rede2.obtemEnderecoRede() << "\n";
   cout << "BC....: " << rede2.obtemEnderecoBroadcast() << "\n";
   cout << "PIP...: " << rede2.obtemPrimeiroIP() << "\n";
   cout << "UIP...: " << rede2.obtemUltimoIP() << "\n";


   Rede rede3("10.30.0.1/12");
   cout << "10.30.0.1/12\n--------------------------\n";
   cout << "WC....: " << rede3.obtemWildcard() << "\n";
   cout << "NM....: " << rede3.obtemMascara() << "\n";
   cout << "NW....: " << rede3.obtemEnderecoRede() << "\n";
   cout << "BC....: " << rede3.obtemEnderecoBroadcast() << "\n";
   cout << "PIP...: " << rede3.obtemPrimeiroIP() << "\n";
   cout << "UIP...: " << rede3.obtemUltimoIP() << "\n";


   //rede.obtemUltimoIP();





   // Atuador no FreeBSD




}
