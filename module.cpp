#include "iostream"
#include "module.h"

using namespace std;

int Module::actualId = 0;

int Module::makeID(){

    ++actualId;

    return actualId;
}

