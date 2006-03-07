
#ifndef _SISTEMA_OPERACIONAL_H
#define _SISTEMA_OPERACIONAL_H


#include <string>



using namespace std;

#define SO_FW_IP_BASERULE		10000
#define SO_FW_IP_BASEPIPE_IN 	18000
#define SO_FW_IP_BASEPIPE_OUT	26000

namespace MOSMAN {

	class SistemaOperacional {

		public:
			SistemaOperacional();
			virtual int ifConfig(string& iface,string& ip,string &mascara);
			virtual int ifUnConfig(string& iface,string &ip);

			// id (id_conta), baserule (para calculo), interface interna, interface externa, ip, mac
			// upload (em kbps), download (em kbps)
			virtual int adicionaRegraBW(int id,int baserule,int basepipe_in,int basepipe_out,string& int_iface,string& ext_iface,string& ip,string& mac,int upload_kbps,int download_kbps);


	};


};


#endif