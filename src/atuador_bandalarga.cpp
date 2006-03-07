#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <boost/regex.hpp>

#include <atuador_bandalarga.h>
#include <rede.h>


#include <sistema_operacional.h>
#include <so_freebsd.h>

#include <config.h>


using namespace std;
using namespace MOSMAN;

using namespace boost;


AtuadorBandalarga::AtuadorBandalarga(Config &cfg) {
	this->cfg = cfg;
}

int AtuadorBandalarga::processa(const string &op,const string &id_conta,const string &parametros) {
   // cout << "Rodando atuador banda larga: " << op << ", " << id_conta << ", " << parametros << "\n";

   if( op == "a" ) {
	   // Adicionar
	   string s = parametros;
	   vector<string> params;

	   // Fazer split dos parametros
	   list<string>	l;
	   regex re(",");
	   regex_split(back_inserter(l),s,re);

	   while(l.size()) {
		   s = *(l.begin());
		   l.pop_front();
		   //cout << "SPLITED: " << s << endl;
		   params.push_back(s);
	   }

	   string rede = params[0];
	   string mac  = params[1];
	   string up   = params[2];
	   string down = params[3];

	   Rede r(rede);

	   string ip_interface = r.obtemPrimeiroIP();
	   string mascara = r.obtemMascara();
	   string ip_cliente   = r.obtemProximoIP();

	   SistemaOperacional *soImpl;
	   string so = this->cfg["geral"]["SO"];

	   // cout << "SO: " << this->cfg["geral"]["SO"] << "\n";

	   // Instancia as implementações específicas do sistema operacional
	   if( so == "FreeBSD" ) {
		   soImpl = new SOFreeBSD();
	   } else {
		   // Erro de SO desconhecido.
	   }

	   if( soImpl != NULL ) {
		   // Executa as implementações específicas do sistema operacional
		   soImpl->ifConfig(this->cfg["geral"]["int_if"],ip_interface,mascara);
		   soImpl->adicionaRegraBW(atoi(id_conta.c_str()),SO_FW_IP_BASERULE,SO_FW_IP_BASEPIPE_IN,SO_FW_IP_BASEPIPE_OUT,this->cfg["geral"]["int_if"],this->cfg["geral"]["ext_if"],ip_cliente,mac,atoi(up.c_str()),atoi(down.c_str()));

	   }




   } else if ( op == "x" ) {
	   // Excluir

   } else {
	   // Erro de operacao desconhecida.

   }





/**


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

*/



   // Atuador no FreeBSD




}
