/**
 * Mosman Interchange Format
 *
 * Esta classe traz as funcionalidades referentes ao formato de troca de informacoes
 * comuns em todos os aplicativos Mosman.
 *
 * As classes descendentes de SistemaOperacional geram informacoes neste formato, a
 * partir dai as informacoes sao acessadas por outros aplicativos atraves desta classe.
 */

#ifndef _MIF_H
#define _MIF_H

#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include "general.h"
#include <boost/regex.hpp>


using namespace std;
using namespace boost;

namespace MOSMAN {

	class MIF {
   
   		public:
   			MIF();
   			/**
   			 * savestats
   			 *
   			 * Salva as estatisticas oara a lista de clientes
   			 * coletadas do sitema operacional em stats no
   			 * diretorio definido por path.
   			 *
   			 */
   			//int savestats(lista& clientes,bag& stats,string& path); 
   			int criaConf(reg& info,string& arquivobase,string& arquivodestino);
   			int mrtgStat(string& username,bag& stats,ostream& out);
   			int mrtgStat(string& username,bag& stats);
   			int stats(lista& clientes,string& path,string &mrtg);
   			
   			
   

	   };



};

#endif
