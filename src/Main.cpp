#include <iostream>
#include <fstream>

#include "../include/json.hpp"
#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Watchable.h"

using namespace std;

int main(int argc, char** argv){
	if(argc!=2)
	{
		cout << "usage splflix input_file" << endl;
		return 0;
	}
    Session s(argv[1]);
    s.start();
/*  Session* s = new Session(argv[1]);
    s->start();
    Session s2 = *s;
    delete(s);
    s2.start(); */
    return 0;
}