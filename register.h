#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include "group.h"

using namespace std;

class Register{
public:

    static bool first_print;

    string offset;
    string name;
    string address;
    string range;
    string access;


    Register(string line, Group g,string baseAddress, bool insideIf, bool insideFor){                                                               //! Object constructor
        if(line.find("line.")!=string::npos){
            line = line.substr(line.find("line."),line.size());

            this -> name = line.substr(line.find('"')+1,line.find(',')-line.find('"')-1);                                                     //! Line name
            this -> access = g.access;                                                                                                        //! Line access
            this -> offset = "0x" + decToHex(hexToDec(line.substr(line.find("0x"),line.find('"')-1-line.find("0x")))+hexToDec(g.offset));     //! Line offset
            this -> address =  "0x" + decToHex(hexToDec(offset) + hexToDec(baseAddress));                                                     //! Line address
            this -> range = line.substr(line.find(".")+1,line.find(" ")-line.find("."));                                                      //! Line range
        }else if(line.find("hide.")!=string::npos){
            line = line.substr(line.find("hide."),line.size());

            this -> name = line.substr(line.find('"')+1,line.find(',')-line.find('"')-1);                                                     //! Line name
            this -> access = g.access;                                                                                                        //! Line access
            this -> offset = "0x" + decToHex(hexToDec(line.substr(line.find("0x"),line.find('"')-1-line.find("0x")))+hexToDec(g.offset));     //! Line offset
            this -> address = "0x" + decToHex( hexToDec(offset) + hexToDec(baseAddress));                                                     //! Line address
            this -> range = line.substr(line.find(".")+1,line.find(" ")-line.find("."));                                                      //! Line range
        }

        if (insideIf == true && insideFor == true){
            name = name + "***";
        }else if(insideFor == true){
            name = name + "*";
        }else if (insideIf == true){
            name = name + "**";
        }
    }

    void print(int width,Register r, string coreAddress);                                                                                            //! Printed row in console

    static long hexToDec(string hexAdd){                                                                                                             //! Convert hex number to decimal number
        long decAdd;
        std::stringstream ss;

        ss << std::hex << hexAdd;
        ss >> decAdd;

        return decAdd;
    }
    static string decToHex(long decAdd){                                                                                                             //! Convert decimal number to hex number
        string hexAdd;
        std::stringstream ss;

        ss << std::hex << decAdd;
        ss >> hexAdd;

        std::transform(hexAdd.begin(),hexAdd.end(),hexAdd.begin(), ::toupper);
        return hexAdd;
    }
    static void searchOperations(string fileAddress, string baseAddress, list<string> params, string coreAddress, string topdir){                                  //! Searching registers in *.ph file
        fstream file;
        string line, tempGroupLine, tempForLine;
        bool insideFor, insideIf = false, insideIfElse;
        int width = 23;
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        Group g;

        file.open(fileAddress.c_str(), ios::in);

        while(getline(file, line)){
            int x = 0;

            for(list<string>::iterator i = params.begin(); i!=params.end();++i){
                ++x;
                string param = "%(" + to_string(x) + ")";

                while(line.find(param) != string::npos){
                    line = line.replace(line.find(param),4,*i);
                }
            }

            if((line.find("group.") != string::npos)){                                                                      //! Creating Group object
                    tempGroupLine = line;
                    g = g.searching(line);
            }else if((line.find("width ") != string::npos)&&(line.find("bitfld") == string::npos)&&(line.find("line") == string::npos)&&
                     (line.find("hexmask") == string::npos)&&(line.find("eventfld") == string::npos)&&(line.find("setclrfld") == string::npos)&&
                     (line.find("hide") == string::npos)&&(line.find("group") == string::npos)){                            //! Setting values of width
                if(line.find("0x") != string::npos){
                    width = Register::hexToDec(line.substr(line.find("0x")+2,line.size()));
                }else{
                    width = atoi(line.substr(line.find("width ")+6,line.find(".")-line.find("width ")-1).c_str());
                }

                first_print = true;
            }else if((line.find("base ") != string::npos)&&(line.find("bitfld") == string::npos)&&(line.find("line") == string::npos)&&
                     (line.find("hexmask") == string::npos)&&(line.find("eventfld") == string::npos)&&(line.find("setclrfld") == string::npos)&&
                     (line.find("hide") == string::npos)&&(line.find("group") == string::npos)){                            //! Changing value of base address
                baseAddress = line.substr(line.find("0x"), line.size() - line.find("0x"));
            }else if((line.find("%for") != string::npos)){                                                                  //! Entry to FOR condition
                    tempForLine = line;
                    insideFor = true;
            }else if((line.find("%endfor") != string::npos)){                                                               //! Exit from FOR condition
                    insideFor = false;
            }else if(line.find("endif")!= string::npos){                                                                    //! Exit from IF condition
                    insideIf = false;
                    insideIfElse = false;
            }else if((line.find("else") != string::npos)||(line.find("elif")!=string::npos)){                               //! Entry to ELSE/ELIF condition
                    insideIfElse = true;
            }else if((line.find("if ") != string::npos)&&(line.find("bitfld") == string::npos)&&(line.find("line") == string::npos)&&(line.find("hexmask") == string::npos)&&(line.find("eventfld") == string::npos)&&(line.find("setclrfld") == string::npos)){                              //! Entry to IF condition
                insideIf = true;
            }else if(((line.find("line.") != string::npos)||(line.find("hide.")!=string::npos))&&insideIfElse == false){    //! Making register object and print it on screen
                if(insideFor == true){
                    forOperations(line, tempForLine, tempGroupLine, width, baseAddress, insideIf, insideFor, coreAddress);
                }else{
                    Register r(line, g, baseAddress, insideIf, insideFor);
                    r.print(width, r, coreAddress);
                }
            }
        }

        SetConsoleTextAttribute( hOut, 14 );
        cout << endl << "Comments:" << endl;
        cout << "    1. Registers with 1 stars (*) after name are inside for loop." << endl;
        cout << "    2. Registers with 2 stars (**) after name are inside if, sif or %if conditions." << endl;
        cout << "    3. Registers with 3 stars (***) after name are inside for loop and if, sif or %if conditions." << endl << endl;
        SetConsoleTextAttribute( hOut, 7 );

        file.close();
        system("pause");
    }
    static void forOperations(string line, string tempForLine, string tempGroupLine, int width, string baseAddress, bool insideIf, bool insideFor,string coreAddress){  //! Making registers from FOR operation
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
                        tabValues[i][j-1] = params.substr(0,params.find(','));                        tabValues[i][j] = params.substr(params.find(',')+1,params.find('"')-params.find(','));
                    }else{
                        tabValues[i][j-1] = params.substr(0,params.find(','));
                    }
                    params = params.substr(params.find(",")+1,params.size());
                }else if(params.find("0x") != string::npos){
                    tabValues[i][j] ="0x" + decToHex(hexToDec(tabValues[i][j-1]) + hexToDec(jump));
                }else{
                    tabValues[i][j] = to_string(atoi(tabValues[i][j-1].c_str()) + atoi(jump.c_str()));
                }
            }

            tempForLine = tempForLine.substr(tempForLine.find(")")+1,tempForLine.size());
        }

        for(int j = 0; j < iterations; j++){                                                                                            //! Replace the parameter number with its value
            string tempLine = line;
            string tempGroup = tempGroupLine;
            for(int i = 1; i < numberOfParam; i++){
                string param = "$" + to_string(i);
                while(tempLine.find(param) != string::npos){
                    tempLine = tempLine.replace(tempLine.find(param),2,tabValues[i-1][j]);
                }
                while(tempGroup.find(param) != string::npos){
                    tempGroup = tempGroup.replace(tempGroup.find(param),2,tabValues[i-1][j]);
                }
            }
            Group g;
            Register r(tempLine, g.searching(tempGroup), baseAddress, insideIf, insideFor);
            r.print(width,r,coreAddress);
        }
    }
};
