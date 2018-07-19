#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include "group.h"

class Register{
public:

    static bool first_print;

    std::string offset;
    std::string name;
    std::string address;
    std::string range;
    std::string access;


    Register(std::string line, Group g,std::string baseAddress, bool insideIf, bool insideFor){                                                               //! Object constructor
        if(line.find("line.") != std::string::npos){
            line = line.substr(line.find("line."),line.size());

            this -> name = line.substr(line.find('"')+1,line.find(',')-line.find('"')-1);                                                     //! Line name
            this -> access = g.access;                                                                                                        //! Line access
            this -> offset = "0x" + decToHex(hexToDec(line.substr(line.find("0x"),line.find('"')-1-line.find("0x")))+hexToDec(g.offset));     //! Line offset
            this -> address =  "0x" + decToHex(hexToDec(offset) + hexToDec(baseAddress));                                                     //! Line address
            this -> range = line.substr(line.find(".")+1,line.find(" ")-line.find("."));                                                      //! Line range
        }else if(line.find("hide.") != std::string::npos){
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

    void print(int width,Register r, std::string coreAddress);                                                                                            //! Printed row in console

    static long hexToDec(std::string hexAdd){                                                                                                             //! Convert hex number to decimal number
        long decAdd;
        std::stringstream ss;

        ss << std::hex << hexAdd;
        ss >> decAdd;

        return decAdd;
    }
    static std::string decToHex(long decAdd){                                                                                                             //! Convert decimal number to hex number
        std::string hexAdd;
        std::stringstream ss;

        ss << std::hex << decAdd;
        ss >> hexAdd;

        std::transform(hexAdd.begin(),hexAdd.end(),hexAdd.begin(), ::toupper);
        return hexAdd;
    }
    static void searchOperations(std::string fileAddress, std::string baseAddress, std::list<std::string> params, std::string coreAddress, std::string topdir){                                  //! Searching registers in *.ph file
        std::fstream file;
        std::string line, tempGroupLine, tempForLine;
        bool insideFor, insideIf = false, insideIfElse;
        int width = 23;
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        Group g;

        file.open(fileAddress.c_str(), std::ios::in);

        while(getline(file, line)){
            int x = 0;

            for(std::list<std::string>::iterator i = params.begin(); i!=params.end();++i){
                ++x;
                std::string param = "%(" + std::to_string(x) + ")";

                while(line.find(param) != std::string::npos){
                    line = line.replace(line.find(param),4,*i);
                }
            }

            if((line.find("group.") != std::string::npos)){                                                                      //! Creating Group object
                    tempGroupLine = line;
                    g = g.searching(line);
            }else if((line.find("width ") != std::string::npos)&&(line.find("bitfld") == std::string::npos)&&(line.find("line") == std::string::npos)&&
                     (line.find("hexmask") == std::string::npos)&&(line.find("eventfld") == std::string::npos)&&(line.find("setclrfld") == std::string::npos)&&
                     (line.find("hide") == std::string::npos)&&(line.find("group") == std::string::npos)){                            //! Setting values of width
                if(line.find("0x") != std::string::npos){
                    width = Register::hexToDec(line.substr(line.find("0x")+2,line.size()));
                }else{
                    width = atoi(line.substr(line.find("width ")+6,line.find(".")-line.find("width ")-1).c_str());
                }

                first_print = true;
            }else if((line.find("base ") != std::string::npos)&&(line.find("bitfld") == std::string::npos)&&(line.find("line") == std::string::npos)&&
                     (line.find("hexmask") == std::string::npos)&&(line.find("eventfld") == std::string::npos)&&(line.find("setclrfld") == std::string::npos)&&
                     (line.find("hide") == std::string::npos)&&(line.find("group") == std::string::npos)){                            //! Changing value of base address
                baseAddress = line.substr(line.find("0x"), line.size() - line.find("0x"));
            }else if((line.find("%for") != std::string::npos)){                                                                  //! Entry to FOR condition
                    tempForLine = line;
                    insideFor = true;
            }else if((line.find("%endfor") != std::string::npos)){                                                               //! Exit from FOR condition
                    insideFor = false;
            }else if(line.find("endif")!= std::string::npos){                                                                    //! Exit from IF condition
                    insideIf = false;
                    insideIfElse = false;
            }else if((line.find("else") != std::string::npos)||(line.find("elif") != std::string::npos)){                               //! Entry to ELSE/ELIF condition
                    insideIfElse = true;
            }else if((line.find("if ") != std::string::npos)&&(line.find("bitfld") == std::string::npos)&&(line.find("line") == std::string::npos)&&
                     (line.find("hexmask") == std::string::npos)&&(line.find("eventfld") == std::string::npos)&&(line.find("setclrfld") == std::string::npos)){                              //! Entry to IF condition
                insideIf = true;
            }else if(((line.find("line.") != std::string::npos)||(line.find("hide.") != std::string::npos))&&insideIfElse == false){    //! Making register object and print it on screen
                if(insideFor == true){
                    forOperations(line, tempForLine, tempGroupLine, width, baseAddress, insideIf, insideFor, coreAddress);
                }else{
                    Register r(line, g, baseAddress, insideIf, insideFor);
                    r.print(width, r, coreAddress);
                }
            }
        }

        SetConsoleTextAttribute( hOut, 14 );
        std::cout << std::endl << "Comments:" << std::endl;
        std::cout << "    1. Registers with 1 stars (*) after name are inside for loop." << std::endl;
        std::cout << "    2. Registers with 2 stars (**) after name are inside if, sif or %if conditions." << std::endl;
        std::cout << "    3. Registers with 3 stars (***) after name are inside for loop and if, sif or %if conditions." << std::endl << std::endl;
        SetConsoleTextAttribute( hOut, 7 );

        file.close();
        system("pause");
    }
    static void forOperations(std::string line, std::string tempForLine, std::string tempGroupLine, int width, std::string baseAddress, bool insideIf, bool insideFor,std::string coreAddress){  //! Making registers from FOR operation
        int numberOfParam = 0;
        std::string tempLine = tempForLine;

        while(!tempLine.empty()){                                                                                                       //! Checking number of parameters
            if(tempLine.find(")") != std::string::npos){
                numberOfParam++;
                tempLine = tempLine.substr(tempLine.find(")")+1,tempLine.size());
            }
        }

        int iterations = atoi(tempForLine.substr(tempForLine.find("(")+1,tempForLine.find(")")-tempForLine.find("(")-1).c_str());
        tempForLine = tempForLine.substr(tempForLine.find(")")+1,tempForLine.size());

        std::string tabValues[numberOfParam-1][iterations];

        for(int i = 0; i < numberOfParam-1; i++){                                                                                       //! Creating table with parameters
            std::string params = tempForLine.substr(tempForLine.find("(")+1,tempForLine.find(")")-tempForLine.find("(")-1);
            bool insideList = false;

            if(params.find("list:") != std::string::npos){
                insideList = true;
                params = tempForLine.substr(tempForLine.find(':')+2,tempForLine.size()-tempForLine.find(':')-4);
            }else{
                params = tempForLine.substr(tempForLine.find("(")+1,tempForLine.find(")")-tempForLine.find("(")-1);
            }

            tabValues[i][0] = params.substr(0,params.find(","));
            std::string jump = params.substr(params.find(",")+1,params.size());

            for(int j = 1; j < iterations; j++){

                if(insideList == true){
                    if(j+1 == iterations){
                        tabValues[i][j-1] = params.substr(0,params.find(','));                        tabValues[i][j] = params.substr(params.find(',')+1,params.find('"')-params.find(','));
                    }else{
                        tabValues[i][j-1] = params.substr(0,params.find(','));
                    }
                    params = params.substr(params.find(",")+1,params.size());
                }else if(params.find("0x") != std::string::npos){
                    tabValues[i][j] ="0x" + decToHex(hexToDec(tabValues[i][j-1]) + hexToDec(jump));
                }else{
                    tabValues[i][j] = std::to_string(atoi(tabValues[i][j-1].c_str()) + atoi(jump.c_str()));
                }
            }

            tempForLine = tempForLine.substr(tempForLine.find(")")+1,tempForLine.size());
        }

        for(int j = 0; j < iterations; j++){                                                                                            //! Replace the parameter number with its value
            std::string tempLine = line;
            std::string tempGroup = tempGroupLine;

            for(int i = 1; i < numberOfParam; i++){
                std::string param = "$" + std::to_string(i);
                while(tempLine.find(param) != std::string::npos){
                    tempLine = tempLine.replace(tempLine.find(param),2,tabValues[i-1][j]);
                }
                while(tempGroup.find(param) != std::string::npos){
                    tempGroup = tempGroup.replace(tempGroup.find(param),2,tabValues[i-1][j]);
                }
            }
            Group g;
            Register r(tempLine, g.searching(tempGroup), baseAddress, insideIf, insideFor);
            r.print(width,r,coreAddress);
        }
    }
};
