

#include <map>
#include <iostream>
#include <istream>
#include <ostream>

#include "general.h"

using namespace std;

namespace MOSMAN {

	/**
	 * Classe base para procesamento de textos
	 */
	
	class MTextProc {
		private:
			reg vars;	// Saco de variaveis
	
		public:
			MTextProc();
			string& operator[](string &);
			string& operator[](const char var[]);
			int processa(istream&,ostream&);
			void setVars(reg& vars);
			
			
	
	};

};

	
