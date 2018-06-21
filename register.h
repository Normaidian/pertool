#include <iostream>
#include <algorithm>
#include <string>
#include "group.h"

using namespace std;

class Register{
public:

    bool first_print = true;

    string offset;
    string name;
    string address;
    string range;
    string access;

    unsigned long hexToDec(string hexAdd){                                                            //! Convert hex number to decimal number
        unsigned long decAdd;
        std::stringstream ss;

        ss << std::hex << hexAdd;
        ss >> decAdd;

        return decAdd;
    }

    string decToHex(unsigned long decAdd){                                                            //! Convert decimal number to hex number
        string hexAdd;
        std::stringstream ss;

        ss << std::hex << decAdd;
        ss >> hexAdd;

        std::transform(hexAdd.begin(),hexAdd.end(),hexAdd.begin(), ::toupper);
        return hexAdd;
    }

    string toString(int numberI){                                                           //! Convert INT to STRING
        string numberS;

        std::stringstream ss;

        ss << numberI;
        ss >> numberS;

        return numberS;
    }
    Register searching(string line, Group g, string baseAddress, bool insideIf, bool insideFor);                                                    //! Searching registers in line from file
    void print(int width,Register r, string coreAddress);                                                                                                               //! Printed row in console
    void forOperations(string line, string tempForLine, string tempGroupLine, int width, string baseAddress, bool insideIf, bool insideFor,string coreAddress);        //! Creating registers from FOR
};
