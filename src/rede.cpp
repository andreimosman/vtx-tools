#include <string>
#include <iostream>

#include <vector>


#include <rede.h>


#include <boost/regex.hpp>

using namespace std;
using namespace boost;
using namespace MOSMAN;



// rede no formato 123.123.123.123/12
Rede::Rede(const string &rede) {

	regex fmt_ip("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)/\\d{1,2}$",regex::extended);
	regex re("/");

	cmatch m;
	if( !regex_match(rede.c_str(),m,fmt_ip) ) {
		this->valido = false;
		return;
	}

	sregex_token_iterator i(rede.begin(),rede.end(),re, -1);
	sregex_token_iterator fim;

   // Separa os bits da máscara
	vector<string> v;

	while( i != fim ) {
		v.insert(v.end(),*i++);
	}

	string strRede = v[0];


	// Valida o numero de bits da mascara
	////////////////
	int bits = atoi(v[1].c_str());

	if( bits > 32 ) {
		// Numero de bits da máscara maior que 32
		this->bitsMask = 0;
		this->valido = false;
		return;
	}

	this->bitsMask = bits;

    // Obtem o wildcard (Base pra máscara e broadcast)
    /////////////////
    this->wildcard = ((1<<(32-this->bitsMask))-1);


    // Obtem a netmask
    /////////////////
    this->netmask = this->msb2lsb(~this->wildcard);

    // Obtem o endereço passado no parametro.
    /////////////////
    this->addr = inet_addr(strRede.c_str());

}

in_addr_t Rede::network() {
	return( this->addr & this->netmask );
}

in_addr_t Rede::broadcast() {
	return( this->network() | ~this->netmask );
}

// converte o armazenamento de MSB pra LSB
in_addr_t Rede::msb2lsb(in_addr_t val) {
   struct in_addr addr = inet_makeaddr(val,0);
   return(addr.s_addr);
}

string Rede::formata(in_addr_t endereco) {
	struct in_addr addr;
	addr.s_addr = endereco;
	return(inet_ntoa(addr));
}

// wildcard invertido escroto
string Rede::obtemWildcard() {
	return(this->formata(this->wildcard));
}

string Rede::obtemMascara(){
	return(this->formata(this->netmask));
}

string Rede::obtemEnderecoRede(){
	return(this->formata( this->network() ));
}

string Rede::obtemEnderecoBroadcast(){
	return(this->formata( this->broadcast() ));
}

string Rede::obtemPrimeiroIP(){
	// endereço de rede + 1
	in_addr_t primeiroIP = this->network() + (1<<24);
	return(this->formata(primeiroIP));
}

string Rede::obtemUltimoIP() {
	// broadcast - 1
	in_addr_t ultimoIP = this->broadcast() -(1<<24);
	return(this->formata(ultimoIP));
}


