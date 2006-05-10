
#ifndef _SISTEMA_OPERACIONAL_H
#define _SISTEMA_OPERACIONAL_H


#include <string>
#include "general.h"
#include "mif.h"



using namespace std;

#define SO_FW_IP_SUP_BASERULE	2000

#define SO_FW_IP_BASERULE		10000
#define SO_FW_IP_BASEPIPE_IN 	18000
#define SO_FW_IP_BASEPIPE_OUT	26000





namespace MOSMAN {

	class SistemaOperacional {
		protected:
			bag stats;

		public:
			SistemaOperacional();
			virtual int ifConfig(string& iface,string& ip,string &mascara);
			virtual int ifUnConfig(string& iface,string &ip);

			// id (id_conta), baserule (para calculo), interface interna, interface externa, ip, mac
			// upload (em kbps), download (em kbps)
			// Regras de Cliente
			virtual int adicionaRegraBW(int id,int baserule,int basepipe_in,int basepipe_out,string& int_iface,string& ext_iface,string& ip,string& mac,int upload_kbps,int download_kbps,string& username);
			virtual int deletaRegraBW(int id,int baserule, int basepipe_int,int basepipe_out);

			// Regras de suporte e infra-estrutura
			virtual int adicionaRegraSP(int id,int baserule,string& rede,string &ext_iface);
			virtual int deletaRegraSP(int id,int baserule);


			virtual bag& obtemEstatisticas();

			// Configura NAT
			virtual int setNAT(string& iface);
			virtual int unsetNAT(string& iface);

			// Rotas
			virtual int routeAdd(string rede,string destino);
			virtual int routeDelete(string rede);

			// Install
			virtual int installDir(string target);

			//Executa
			virtual int executa(string& comando);


	};


};


#endif
