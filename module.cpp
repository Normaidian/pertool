#include "iostream"
#include "module.h"

using namespace std;


int Module::makeID(){
    static int id;

    ++id;

    return id;
}

