#include <iostream>
#include <sstream>
#include <algorithm>

class Group{
public:

    std::string offset;
    std::string range;
    std::string access;

    long hexToDec(std::string hexAdd){                                                            //! Convert hex number to decimal number
        long decAdd;
        std::stringstream ss;

        ss << std::hex << hexAdd;
        ss >> decAdd;

        return decAdd;
    }

    std::string decToHex(long decAdd){                                                            //! Convert decimal number to hex number
        std::string hexAdd;
        std::stringstream ss;

        ss << std::hex << decAdd;
        ss >> hexAdd;

        std::transform(hexAdd.begin(),hexAdd.end(),hexAdd.begin(), ::toupper);
        return hexAdd;
    }

    Group searching(std::string line);                                                             //! Searching group in line from file
};
