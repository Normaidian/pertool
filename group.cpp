#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "group.h"

using namespace std;

Group Group::searching(string line){
    Group g;

    if(line.find("hgroup.")!=string::npos){
        line = line.substr(line.find("hgroup."),line.size());
        g.offset = line.substr(line.find("0x"),line.find("++")-line.find("0x"));   //! Group offset
        g.range = line.substr(line.find(".")+1,line.find(" ")-line.find("."));     //! Group range
        g.access = "Hide ";                                                        //! Group access
    }else if(line.find("rgroup.")!=string::npos){
        line = line.substr(line.find("rgroup."),line.size());
        g.offset = line.substr(line.find("0x"),line.find("++")-line.find("0x"));   //! Group offset
        g.range = line.substr(line.find(".")+1,line.find(" ")-line.find("."));     //! Group range
        g.access = "Read (RO)";                                                    //! Group access
    }else if(line.find("wgroup.")!=string::npos){
        line = line.substr(line.find("wgroup."),line.size());
        g.offset = line.substr(line.find("0x"),line.find("++")-line.find("0x"));   //! Group offset
        g.range = line.substr(line.find(".")+1,line.find(" ")-line.find("."));     //! Group range
        g.access = "Write (WO)";                                                   //! Group access
    }else if(line.find("group.")!=string::npos){
        line = line.substr(line.find("group."),line.size());
        g.offset = line.substr(line.find("0x"),line.find("++")-line.find("0x"));   //! Group offset
        g.range = line.substr(line.find(".")+1,line.find(" ")-line.find("."));     //! Group range
        g.access = "Read/Write (RW)";                                              //! Group access
    }



    if(g.offset.find("+") != string::npos){

        g.offset = decToHex(hexToDec(g.offset.substr(0,g.offset.find("+"))) + hexToDec(g.offset.substr(g.offset.find("+")+1,g.offset.size())));
    }

    return g;
}
