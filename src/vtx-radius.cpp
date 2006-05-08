
#include <string>
#include <iostream>

#include <virtex.h>
#include <appradius.h>


using namespace std;
using namespace MOSMAN;

int main(int argc,char **argv,char **env) {

	AplicativoRadius app;
	app.init(VIRTEX_INI,argc,argv,env);
	//app.executa();

	//if( app.fazerFork() ) {
	//	app.daemon();
	//} else {
		exit(app.executa());
	//}


	//cout << "DBHOST: " << app.cfg["pgsql"]["host"] << "\n";


}
