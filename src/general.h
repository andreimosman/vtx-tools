
#ifndef _GENERAL_H
#define _GENERAL_H

#include <map>
#include <vector>
#include <string>


using namespace std;


namespace MOSMAN {
        typedef map<string,string>      reg;
        typedef map<string,reg>         bag;
        typedef vector<reg>      lista;
};

#endif
