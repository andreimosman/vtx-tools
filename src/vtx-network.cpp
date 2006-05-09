
#include <string>
#include <iostream>

#include <virtex.h>
#include <appnetwork.h>


using namespace std;
using namespace MOSMAN;

int main(int argc,char **argv,char **env) {

	AplicativoNetwork app;
	app.init(NETWORK_INI,argc,argv,env);
	//app.executa();

	//if( app.fazerFork() ) {
	//	app.daemon();
	//} else {
		exit(app.executa());
	//}


	//cout << "DBHOST: " << app.cfg["pgsql"]["host"] << "\n";


}
