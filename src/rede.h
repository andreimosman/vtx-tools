

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <string>

using namespace std;


namespace MOSMAN {

   class Rede {
	   bool valido;
	   int bitsMask;			// Bits da máscara
	   in_addr_t addr;			// Endereço sem o /bits
	   in_addr_t wildcard;		// Wildcard
	   in_addr_t netmask;		// Netmask

	  //int obtemWildcard();

	  in_addr_t msb2lsb(in_addr_t val);
	  in_addr_t network();
	  in_addr_t broadcast();




      public:

         Rede(const string &);	// Recebe o endereço da rede no formato 111.111.111.111/11

         string formata(in_addr_t endereco);
         string obtemWildcard();
         string obtemMascara();
         string obtemEnderecoRede();
         string obtemEnderecoBroadcast();
         string obtemPrimeiroIP();
         string obtemUltimoIP();






   };

};


