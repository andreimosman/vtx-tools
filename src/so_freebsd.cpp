#include <math.h>
#include <string>
#include <iostream>

#include <so_freebsd.h>


using namespace std;
using namespace MOSMAN;

SOFreeBSD::SOFreeBSD() {
}

int SOFreeBSD::executa(string& comando) {
	//cout << comando << endl;
	system(comando.c_str());
}

int SOFreeBSD::ifConfig(string& iface,string& ip,string &mascara) {
	string comando = "/sbin/ifconfig " + iface + " inet alias " + ip + " netmask " + mascara;
	return(this->executa(comando));
}

int SOFreeBSD::ifUnConfig(string& iface,string &ip) {
	string comando = "/sbin/ifconfig " + iface + " delete " + ip;
	return(this->executa(comando));
}

int SOFreeBSD::adicionaRegraBW(int id,int baserule,int basepipe_in,int basepipe_out,string& int_iface,string& ext_iface,string& ip,string& mac,int upload_kbps,int download_kbps) {
	string ipfw = "/sbin/ipfw -q";
	string comando;
	// Regra de controle de mac


	char rule[1024];
	char pipe_in[1024];
	char pipe_out[1024];

	sprintf(rule,"%d",baserule + id);
	sprintf(pipe_in,"%d",basepipe_in + id);
	sprintf(pipe_out,"%d",basepipe_out + id);


	// VERIFICACAO DE MAC
	if( mac != "" ) {
		comando = ipfw + " add " + rule + " deny layer2 src-ip " + ip + " not MAC any " + mac + " via " + int_iface;
		this->executa(comando);
		comando = ipfw + " add " + rule + " deny layer2 dst-ip " + ip + " not MAC " + mac + " any via " + int_iface;
		this->executa(comando);
	}

	// ADICIONA OS PIPES
	comando = ipfw + " add " + rule + " pipe " + pipe_in + " ip from " + ip + " to any out recv " + int_iface + " xmit " + ext_iface ;
	this->executa(comando);
	comando = ipfw + " add " + rule + " pipe " + pipe_out + " ip from any to " + ip + " out recv " + ext_iface + " xmit " + int_iface;
	this->executa(comando);

	// ADICIONA AS REGRAS
	comando = ipfw + " add " + rule + " allow ip from " + ip + " to any in recv " + int_iface;
	this->executa(comando);
	comando = ipfw + " add " + rule + " allow ip from " + ip + " to any out xmit " + ext_iface;
	this->executa(comando);

	comando = ipfw + " add " + rule + " allow ip from any to " + ip + " in recv " + ext_iface;
	this->executa(comando);
	comando = ipfw + " add " + rule + " allow ip from any to " + ip + " out xmit " + int_iface;
	this->executa(comando);

	// CONFIGURA OS PIPES
	char slots_in[1024];
	char slots_out[1024];
	sprintf(slots_in,"%d", this->obtemNumSlotsIdeal(download_kbps));
	sprintf(slots_out, "%d", this->obtemNumSlotsIdeal(upload_kbps));

	char upload[1024];
	char download[1024];
	sprintf(upload,"%d",upload_kbps);
	sprintf(download,"%d",download_kbps);

	comando = ipfw + " pipe " + pipe_out + " config bw " + upload + "Kbit/s queue " + slots_out;
	this->executa(comando);
	comando = ipfw + " pipe " + pipe_in + " config bw " + download + "Kbit/s queue " + slots_in;
	this->executa(comando);

}

// queue
int SOFreeBSD::obtemNumSlotsIdeal(int banda) {
   float kbps_geral = 600.00;
   float pipes = 50.00;
   float b = banda;

   if(banda>kbps_geral) return pipes;

   return( round(pipes/(kbps_geral/b)) );
}

