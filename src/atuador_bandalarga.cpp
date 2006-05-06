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

   SistemaOperacional *soImpl;
   string so = this->cfg["geral"]["SO"];

   // cout << "SO: " << this->cfg["geral"]["SO"] << "\n";

   // Instancia as implementações específicas do sistema operacional
   //if( so == "FreeBSD" ) {
	   soImpl = new SOFreeBSD();
   //} else {
	   // Erro de SO desconhecido.
   //}

   if( op == "a" ) {
	   /**
	    * ADICIONAR
	    * Parametros:
	    * 	rede,mac,up,down,user
	    */

	   string rede = params[0];
	   string mac  = params[1];
	   string up   = params[2];
	   string down = params[3];
	   string user = params[4];

	   Rede r(rede);

	   string ip_interface = r.obtemPrimeiroIP();
	   string mascara = r.obtemMascara();
	   string ip_cliente   = r.obtemProximoIP();

	   // Executa as implementações específicas do sistema operacional
	   soImpl->ifConfig(this->cfg["geral"]["int_if"],ip_interface,mascara);
	   soImpl->adicionaRegraBW(atoi(id_conta.c_str()),SO_FW_IP_BASERULE,SO_FW_IP_BASEPIPE_IN,SO_FW_IP_BASEPIPE_OUT,this->cfg["geral"]["int_if"],this->cfg["geral"]["ext_if"],ip_cliente,mac,atoi(up.c_str()),atoi(down.c_str()),user);

   } else if ( op == "x" ) {
	   /**
	    * EXCLUIR
	    * Parametros:
	    *
	    */
	   //string iface  = params[0];
	   //string ipaddr = params[1];
	   string ipaddr = params[0];
	   if( soImpl != NULL ) {
	   	// Tira o IP
	   	soImpl->ifUnConfig(this->cfg["geral"]["int_if"],ipaddr);

	   	// Apaga a regra do firewall
	   	soImpl->deletaRegraBW(atoi(id_conta.c_str()),SO_FW_IP_BASERULE,SO_FW_IP_BASEPIPE_IN,SO_FW_IP_BASEPIPE_OUT);
	   }
   } else {
	   // Erro de operacao desconhecida.

   }

}
