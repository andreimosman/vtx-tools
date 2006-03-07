#ifndef _SO_FREEBSD_H
#define _SO_FREEBSD_H


#include <sistema_operacional.h>


namespace MOSMAN {


	class SOFreeBSD : public SistemaOperacional {
		private:
			int obtemNumSlotsIdeal(int banda);
			int executa(string& comando);

		public:
			SOFreeBSD();

			int ifConfig(string& iface,string& ip,string &mascara);
			int ifUnConfig(string& iface,string &ip);

			// id (id_conta), baserule (para calculo), interface interna, interface externa, ip, mac
			// upload (em kbps), download (em kbps)
			int adicionaRegraBW(int id,int baserule,int basepipe_in,int basepipe_out,string& int_iface,string& ext_iface,string& ip,string& mac,int upload_kbps,int download_kbps);


	};



};






#endif
