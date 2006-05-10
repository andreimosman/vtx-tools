#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <vector>
#include <general.h>
#include <mif.h>
#include <so_freebsd.h>
#include <map>

#include <list>
#include <boost/regex.hpp>


using namespace std;
using namespace MOSMAN;
using namespace boost;

SOFreeBSD::SOFreeBSD() {
}

int SOFreeBSD::executa(string& comando) {
	cout << comando << endl;
	//return(system(comando.c_str()));
	return(0);
}

bag& SOFreeBSD::obtemEstatisticas() {
	// Coleta as estatísticas individuais do sistema operacional
	//bag stats;
	this->stats.clear();

	/******************************************
	 * Coleta os dados do IPFW                *
	 ******************************************/

	string ipfw = "/sbin/ipfw";
	string comando = ipfw + " -b show";

	int pid = getpid();
	char ipfwTMP[1024];
	snprintf((char *)&ipfwTMP,1024,"/tmp/ipfw-stats-%d",pid);

	FILE *pd=popen(comando.c_str(),"r");

	char buffer[1024];


	if( pd ) {
	   ofstream tmp;
	   tmp.open(ipfwTMP,ios::out);


	   while(!feof(pd)) {
	      fgets((char*)&buffer,1024,pd);

	      tmp << buffer;
	   }

	   tmp.close();

	}
	pclose(pd);

	/******************************************
	 * Processar os dados do IPFW             *
	 ******************************************/

	ifstream in;
	in.open((char *)&ipfwTMP,ios::in);
	regex *re;
	while(!in.eof()) {
		re = new regex("\\s+");
		// Pega a linha
		char buff[4096];
		in.getline(buff,4096);

		if( !strlen(buff) ) continue;

		string linha(buff);

		// Fazer split dos parametros
		list<string> l;
		regex_split(back_inserter(l),linha,*re);

		vector<string> params;

		while(l.size()) {
			linha = *(l.begin());
			l.pop_front();
			params.push_back(linha);
		}

		string id    	= params[0];
		string pacotes 	= params[1];
		string bytes	= params[2];
		string rule		= params[3];


		//bool comentario=false;
		string comentario = params[ params.size() - 1 ];

		string usuario;
		string tipo;

		if( rule == "pipe" ) {
			// Regra de banda, coletar as informações e jogar no map.
			re = new regex("::");

			l.clear();
			params.clear();

			regex_split(back_inserter(l),comentario,*re);

			while(l.size()) {
				comentario = *(l.begin());
				l.pop_front();
				params.push_back(comentario);
			}

			usuario = params[0];
			tipo    = params[1];

			this->stats[usuario][tipo] = bytes;

		}


	}

	in.close();

	unlink(ipfwTMP);

	/******************************************
	 * Exportar os dados no formato do VA     *
	 ******************************************/

	/**

	char arquivo[1024];
	snprintf((char *)&arquivo,1024,"/tmp/stats-%d",pid);

	ofstream arqstats;
	arqstats.open(arquivo,ios::out);
	for( bag::iterator r = stats.begin(); r != stats.end(); *r++ ) {
		string linha = r->first + "," + r->second["up"] + "," + r->second["down"];
		arqstats << linha << endl;
	}

	arqstats.close();
	*/

	// Retorna o nome do arquivo temporario
	// para processamento das estatisticas
	//return(string(arquivo));


	return(this->stats);

}

int SOFreeBSD::ifConfig(string& iface,string& ip,string &mascara) {
	string comando = "/sbin/ifconfig " + iface + " inet alias " + ip + " netmask " + mascara;
	return(this->executa(comando));
}

int SOFreeBSD::ifUnConfig(string& iface,string &ip) {
	string comando = "/sbin/ifconfig " + iface + " delete " + ip;
	return(this->executa(comando));
}

// queue
int SOFreeBSD::obtemNumSlotsIdeal(int banda) {
   float kbps_geral = 600.00;
   float pipes = 50.00;
   float b = banda;

   if(banda>kbps_geral) return (int)pipes;

   return( (int)round(pipes/(kbps_geral/b)) );
}

int SOFreeBSD::adicionaRegraBW(int id,int baserule,int basepipe_in,int basepipe_out,string& int_iface,string& ext_iface,string& ip,string& mac,int upload_kbps,int download_kbps,string& username) {
	string ipfw = "/sbin/ipfw -q";
	string comando;
	// Regra de controle de mac


	char rule[1024];
	char pipe_in[1024];
	char pipe_out[1024];

	sprintf(rule,"%d",baserule + id);
	sprintf(pipe_in,"%d",basepipe_in + id);
	sprintf(pipe_out,"%d",basepipe_out + id);

	///////////////////////////////////////////
	// VERIFICACAO DE MAC                    //
	///////////////////////////////////////////

	if( mac != "" ) {
		comando = ipfw + " add " + rule + " deny layer2 src-ip " + ip + " not MAC any " + mac + " via " + int_iface;
		this->executa(comando);
		//comando = ipfw + " add " + rule + " deny layer2 dst-ip " + ip + " not MAC " + mac + " any via " + int_iface;
		//this->executa(comando);
	}

	///////////////////////////////////////////
	// ADICIONA AS REGRAS                    //
	///////////////////////////////////////////

	// upload
	comando = ipfw + " add " + rule + " pipe " + pipe_in + " ip from " + ip + " to any // " +username+"::up";
	this->executa(comando);
	// download
	comando = ipfw + " add " + rule + " pipe " + pipe_out + " ip from any to " + ip + " // "+username+"::down";
	this->executa(comando);

	///////////////////////////////////////////
	// CONFIGURA OS PIPES                    //
	///////////////////////////////////////////
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

int SOFreeBSD::deletaRegraBW(int id,int baserule, int basepipe_in,int basepipe_out) {

	string ipfw = "/sbin/ipfw -q";
	string comando;
	// Regra de controle de mac

	char rule[1024];
	char pipe_in[1024];
	char pipe_out[1024];

	sprintf(rule,"%d",baserule + id);
	sprintf(pipe_in,"%d",basepipe_in + id);
	sprintf(pipe_out,"%d",basepipe_out + id);

	// Apaga regras
	comando = ipfw + " delete " + rule;
	this->executa(comando);

	// Apaga os pipes
	comando = ipfw + " pipe " + pipe_in + " delete";
	this->executa(comando);

	comando = ipfw + " pipe " + pipe_out + " delete";
	this->executa(comando);

}

int SOFreeBSD::adicionaRegraSP(int id,int baserule,string& rede,string &ext_iface) {
	string comando;
	string ipfw = "/sbin/ipfw -q";

	char rule[1024];
	sprintf(rule,"%d",baserule + id);


	comando = ipfw + " add " + rule + " allow ip from " + rede + " to any keep-state";
	this->executa(comando);
	comando = ipfw + " add " + rule + " allow ip from any to " + rede + " recv " + ext_iface + " keep-state ";
	this->executa(comando);

}

int SOFreeBSD::deletaRegraSP(int id,int baserule) {
	string ipfw = "/sbin/ipfw -q";
	char rule[1024];
	sprintf(rule,"%d",baserule + id);

	string comando = ipfw + " delete " + rule;
	this->executa(comando);

}

int SOFreeBSD::setNAT(string &iface){
	string pfctl = "/sbin/pfctl";
	string comando = pfctl + " -Nf- ";
	FILE *p = popen(comando.c_str(),"w");

	string op;
	op = "no nat on " + iface + " from " + iface + " to any ";
	fprintf(p,"%s\n",(const char *)op.c_str());

	op = "nat on " + iface + " from {10.0.0.0/8,172.16.0.0/12,192.168.0.0/16} to any -> (" + iface + ") ";
	fprintf(p,"%s\n",(const char *)op.c_str());

	pclose(p);
}

int SOFreeBSD::unsetNAT(string &iface){
	string pfctl = "/sbin/pfctl";

	// Flush
	string comando = pfctl + " -F nat ";
	return(this->executa(comando));
}

int SOFreeBSD::routeAdd(string rede,string destino) {
	string route = "/sbin/route";

	string comando = route + " add -net " + rede + " " + destino;
	return(this->executa(comando));

}

int SOFreeBSD::routeDelete(string rede) {
	string route = "/sbin/route";

	string comando = route + " delete -net " + rede + " 2>&1 > /dev/null ";
	return(this->executa(comando));

}

int SOFreeBSD::installDir(string target) {
	string install = "/usr/bin/install";

	string comando = install + " -d " + target;
	return(this->executa(comando));


}

