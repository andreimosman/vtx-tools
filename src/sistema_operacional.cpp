#include <string>
#include <iostream>

#include <mif.h>
#include <sistema_operacional.h>

using namespace std;
using namespace MOSMAN;

SistemaOperacional::SistemaOperacional(){

}



int SistemaOperacional::ifConfig(string& iface,string& ip,string &mascara) {

}

int SistemaOperacional::ifUnConfig(string& iface,string &ip) {

}

int SistemaOperacional::adicionaRegraBW(int id,int baserule,int basepipe_in,int basepipe_out,string& int_iface,string& ext_iface,string& ip,string& mac,int upload_kbps,int download_kbps,string& username) {

}

int SistemaOperacional::deletaRegraBW(int id,int baserule, int basepipe_int,int basepipe_out) {

}

int SistemaOperacional::adicionaRegraSP(int id,int baserule,string& rede,string &ext_iface) {

}

int SistemaOperacional::deletaRegraSP(int id,int baserule) {

}

bag& SistemaOperacional::obtemEstatisticas() {

}

int SistemaOperacional::setNAT(string& iface) {

}

int SistemaOperacional::unsetNAT(string& iface) {

}
