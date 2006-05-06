#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <boost/regex.hpp>

#include <atuador_redesuporte.h>
#include <rede.h>


#include <sistema_operacional.h>
#include <so_freebsd.h>

#include <config.h>


using namespace std;
using namespace MOSMAN;

using namespace boost;


AtuadorRedeSuporte::AtuadorRedeSuporte(Config &cfg) {
	this->cfg = cfg;
}

int AtuadorRedeSuporte::processa(const string &op,const string &id_conta,const string &parametros) {
   // cout << "Rodando atuador banda larga: " << op << ", " << id_conta << ", " << parametros << "\n";

   SistemaOperacional *soImpl;
   string so = this->cfg["geral"]["SO"];

   // Instancia as implementações específicas do sistema operacional
   if( so == "FreeBSD" ) {
	   soImpl = new SOFreeBSD();
   } else {
	   // Erro de SO desconhecido.
   }

   if( soImpl == NULL ) {
	   return -1;
   }

   string rede = parametros;


   Rede r(parametros);

   string ip_interface = r.obtemUltimoIP();
   string mascara = r.obtemMascara();
   //string ip_cliente   = r.obtemProximoIP();

   if( op == "a" ) {
	   // Adicionar

	   // Executa as implementações específicas do sistema operacional
	   soImpl->ifConfig(this->cfg["geral"]["int_if"],ip_interface,mascara);
	   soImpl->adicionaRegraSP(atoi(id_conta.c_str()),SO_FW_IP_SUP_BASERULE,rede,this->cfg["geral"]["ext_if"]);


   } else if ( op == "x" ) {
	   // Excluir

	   // Executa as implementações específicas do sistema operacional
	   soImpl->ifUnConfig(this->cfg["geral"]["int_if"],ip_interface);
	   soImpl->deletaRegraSP(atoi(id_conta.c_str()),SO_FW_IP_SUP_BASERULE);

   } else {
	   // Erro de operacao desconhecida.

   }

}
