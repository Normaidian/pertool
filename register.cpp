#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include "register.h"

using namespace std;

Register Register::searching(string line, Group g,string baseAddress, bool insideIf, bool insideFor){
    Register r;

    if(line.find("line.")!=string::npos){
        line = line.substr(line.find("line."),line.size());
        r.name = line.substr(line.find('"')+1,line.find(',')-line.find('"')-1);                                                     //! Line name
        r.access = g.access;                                                                                                        //! Line access
        r.offset = "0x" + decToHex(hexToDec(line.substr(line.find("0x"),line.find('"')-1-line.find("0x")))+hexToDec(g.offset));     //! Line offset
        r.address =  "0x" + decToHex(hexToDec(r.offset) + hexToDec(baseAddress));                                                   //! Line address
        r.range = line.substr(line.find(".")+1,line.find(" ")-line.find("."));                                                      //! Line range

        if (insideIf == true && insideFor == true){
            r.name = r.name + "***";
        }else if(insideFor == true){
            r.name = r.name + "*";
        }else if (insideIf == true){
            r.name = r.name + "**";
        }

    }else if(line.find("hide.")!=string::npos){
        line = line.substr(line.find("hide."),line.size());
        r.name = line.substr(line.find('"')+1,line.find(',')-line.find('"')-1);                                                     //! Line name
        r.access = g.access;                                                                                                        //! Line access
        r.offset = "0x" + decToHex(hexToDec(line.substr(line.find("0x"),line.find('"')-1-line.find("0x")))+hexToDec(g.offset));     //! Line offset
        r.address = "0x" + decToHex( hexToDec(r.offset) + hexToDec(baseAddress));                                                   //! Line address
        r.range = line.substr(line.find(".")+1,line.find(" ")-line.find("."));                                                      //! Line range

        if (insideIf == true){
            r.name = r.name + "**";
        }
    }
    return r;
}

void Register::print(int width, Register r, string coreAddress){
    string str, floor;

    //! Created string with blank space and floor equal in length of name column
    if (width > 23){
        for(int i = 0;i < width+6 ; i++){
            str = str + " ";
            floor = floor + "_";
        }
    }else{
        for(int i = 0;i < 23 ; i++){
            str = str + " ";
            floor = floor + "_";
        }
    }
    //! Printed table header

    if (coreAddress == "none"){
        if(first_print == true){
            cout << " " << floor << "________________________________________________________________________________" << endl;
            cout << "|" << str.substr(0,5) <<"Register name" << str.substr(0,str.length()-18) << "|";
            cout << str.substr(0,9) << "Access" << str.substr(0,9) << "|";
            cout << str.substr(0,7) << "Address" << str.substr(0,7) << "|";
            cout << str.substr(0,5) << "Offset" << str.substr(0,5) << "|";
            cout << str.substr(0,5) << "Range" << str.substr(0,5) << "|" << endl;
            cout << "|" << floor << "|________________________|_____________________|________________|_______________|" << endl;

            first_print = false;
        }

        cout << "|" << str.substr(0,3) << r.name << str.substr(0,str.length()-r.name.length()-3) << "|";                        //! print Register name
        cout << str.substr(0,5) << r.access << str.substr(0,24-r.access.length()-5) << "|";                                     //! print Register access
        cout << str.substr(0,5) << r.address << str.substr(0,21-r.address.length()-5) << "|";                                   //! print Register address
        cout << str.substr(0,6) << r.offset << str.substr(0,16-r.offset.length()-6) << "|";                                     //! print Register offset
        cout << str.substr(0,6) << r.range << str.substr(0,15-r.range.length()-6) << "|" << endl;                               //! print Register range
        cout << "|" << floor << "|________________________|_____________________|________________|_______________|" << endl;
    }else if (coreAddress == "spr"){
        if(r.offset.length() < 7){
            string s;
            for(int i = 0; i < 7-r.offset.length(); i++){
                s = s + "0";
            }
            r.offset.insert(2,s);
        }
        if(first_print == true){
            cout << " " << floor << "_______________________________________________________________________________________________________" << endl;
            cout << "|" << str.substr(0,5) <<"Register name" << str.substr(0,str.length()-18) << "|";
            cout << str.substr(0,9) << "Access" << str.substr(0,9) << "|";
            cout << str.substr(0,7) << "Address" << str.substr(0,7) << "|";
            cout << "  " << "OP0" << "  " << "|";
            cout << "  " << "OP1" << "  " << "|";
            cout << "  " << "CRn" << "  " << "|";
            cout << "  " << "CRm" << "  " << "|";
            cout << "  " << "OP2" << "  " << "|";
            cout << str.substr(0,5) << "Range" << str.substr(0,5) << "|" << endl;
            cout << "|" << floor << "|________________________|_____________________|_______|_______|_______|_______|_______|_______________|" << endl;

            first_print = false;
        }

        cout << "|" << str.substr(0,3) << r.name << str.substr(0,str.length()-r.name.length()-3) << "|";                        //! print Register name
        cout << str.substr(0,5) << r.access << str.substr(0,24-r.access.length()-5) << "|";                                     //! print Register access
        cout << str.substr(0,5) << r.address << str.substr(0,21-r.address.length()-5) << "|";                                   //! print Register address
        cout << "   " << r.offset[2] << "   " << "|";
        cout << "   " << r.offset[3] << "   " << "|";
        cout << "   " << r.offset[4] << "   " << "|";
        cout << "   " << r.offset[5] << "   " << "|";
        cout << "   " << r.offset[6] << "   " << "|";
        cout << str.substr(0,6) << r.range << str.substr(0,15-r.range.length()-6) << "|" << endl;                               //! print Register range
        cout << "|" << floor << "|________________________|_____________________|_______|_______|_______|_______|_______|_______________|" << endl;
    }else if(coreAddress == "cp14/15"){
        if(r.offset.length() < 6){
            string s;
            for(int i = 0; i < 6-r.offset.length(); i++){
                s = s + "0";
            }
            r.offset.insert(2,s);
        }

        if(first_print == true){
            cout << " " << floor << "_______________________________________________________________________________________________" << endl;
            cout << "|" << str.substr(0,5) <<"Register name" << str.substr(0,str.length()-18) << "|";
            cout << str.substr(0,9) << "Access" << str.substr(0,9) << "|";
            cout << str.substr(0,7) << "Address" << str.substr(0,7) << "|";
            cout << "  " << "OP1" << "  " << "|";
            cout << "  " << "OP2" << "  " << "|";
            cout << "  " << "CRm" << "  " << "|";
            cout << "  " << "CRn" << "  " << "|";
            cout << str.substr(0,5) << "Range" << str.substr(0,5) << "|" << endl;
            cout << "|" << floor << "|________________________|_____________________|_______|_______|_______|_______|_______________|" << endl;

            first_print = false;
        }

        cout << "|" << str.substr(0,3) << r.name << str.substr(0,str.length()-r.name.length()-3) << "|";                        //! print Register name
        cout << str.substr(0,5) << r.access << str.substr(0,24-r.access.length()-5) << "|";                                     //! print Register access
        cout << str.substr(0,5) << r.address << str.substr(0,21-r.address.length()-5) << "|";                                   //! print Register address
        cout << "   " << r.offset[2] << "   " << "|";
        cout << "   " << r.offset[3] << "   " << "|";
        cout << "   " << r.offset[4] << "   " << "|";
        cout << "   " << r.offset[5] << "   " << "|";                                                                                   //! print Register offset
        cout << str.substr(0,6) << r.range << str.substr(0,15-r.range.length()-6) << "|" << endl;                               //! print Register range
        cout << "|" << floor << "|________________________|_____________________|_______|_______|_______|_______|_______________|" << endl;
    }
}

void Register::forOperations(string line, string tempForLine, string tempGroupLine, int width, string baseAddress, bool insideIf, bool insideFor,string coreAddress){
    int numberOfParam = 0;
    string tempLine = tempForLine;

    while(!tempLine.empty()){                                                                                                       //! Checking number of parameters
        if(tempLine.find(")") != string::npos){
            numberOfParam++;
            tempLine = tempLine.substr(tempLine.find(")")+1,tempLine.size());
        }
    }

    int iterations = atoi(tempForLine.substr(tempForLine.find("(")+1,tempForLine.find(")")-tempForLine.find("(")-1).c_str());
    tempForLine = tempForLine.substr(tempForLine.find(")")+1,tempForLine.size());

    string tabValues[numberOfParam-1][iterations];

    for(int i = 0; i < numberOfParam-1; i++){                                                                                       //! Creating table with parameters
        string params = tempForLine.substr(tempForLine.find("(")+1,tempForLine.find(")")-tempForLine.find("(")-1);
        bool insideList = false;

        if(params.find("list:") != string::npos){
            insideList = true;
            params = tempForLine.substr(tempForLine.find(':')+2,tempForLine.size()-tempForLine.find(':')-4);
        }else{
            params = tempForLine.substr(tempForLine.find("(")+1,tempForLine.find(")")-tempForLine.find("(")-1);
        }

        tabValues[i][0] = params.substr(0,params.find(","));
        string jump = params.substr(params.find(",")+1,params.size());

        for(int j = 1; j < iterations; j++){

            if(insideList == true){
                if(j+1 == iterations){
                    tabValues[i][j-1] = params.substr(0,params.find(','));                    tabValues[i][j] = params.substr(params.find(',')+1,params.size());
                }else{
                    tabValues[i][j-1] = params.substr(0,params.find(','));
                }
                params = params.substr(params.find(",")+1,params.size());
            }else if(params.find("0x") != string::npos){
                tabValues[i][j] ="0x" + decToHex(hexToDec(tabValues[i][j-1]) + hexToDec(jump));
            }else{
                tabValues[i][j] = toString(atoi(tabValues[i][j-1].c_str()) + atoi(jump.c_str()));
            }
        }

        tempForLine = tempForLine.substr(tempForLine.find(")")+1,tempForLine.size());
    }

    for(int j = 0; j < iterations; j++){                                                                                            //! Replace the parameter number with its value
        string tempLine = line;
        string tempGroup = tempGroupLine;
        for(int i = 1; i < numberOfParam; i++){
            string param = "$" + toString(i);
            while(tempLine.find(param) != string::npos){
                tempLine = tempLine.replace(tempLine.find(param),2,tabValues[i-1][j]);
            }
            while(tempGroup.find(param) != string::npos){
                tempGroup = tempGroup.replace(tempGroup.find(param),2,tabValues[i-1][j]);
            }
        }
        Group g;
        g = g.searching(tempGroup);
        print(width,searching(tempLine,g,baseAddress,insideIf, insideFor),coreAddress);
    }
}

