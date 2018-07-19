#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <list>
#include "register.h"
#include "module.h"

using namespace std;

int width, numberOfParams = 0;
string line, fileAddress, baseAddress, tempForLine, tempGroupLine, coreAddress = "none", hexOrDec = "dec", topdir;

void registerTabel();
void moduleTabel();

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

int main(){
    do{
        try{
            system("cls");
            SetConsoleTextAttribute( hOut, 7);
            width = 0;
            int choice;

            cout << " ___________________________________________________"<< endl;
            cout << "|                       MENU                        |" << endl;
            cout << "|___________________________________________________|" << endl;
            cout << "|    1 - Get table with registers (for *.ph file)   |" << endl;
            cout << "|___________________________________________________|" << endl;
            cout << "|    2 - Get table with modules   (for *.p file)    |" << endl;
            cout << "|___________________________________________________|" << endl;
            cout << "|    0 - Exit                                       |" << endl;
            cout << "|___________________________________________________|" << endl;
            cout << "Select operation: ";
            cin >> choice;

            if( !cin ){
                throw(std::logic_error("---Wrong choice!---"));
            }
            else{
                switch(choice){
                    case 1:
                        registerTabel();
                    break;
                    case 2:
                        moduleTabel();
                    break;
                    case 0:
                        exit(0);
                    break;
                    default:
                        throw(std::logic_error("---Wrong choice!---"));
                    break;
                }
            }
        }catch(std::logic_error &e){
            SetConsoleTextAttribute( hOut, 12 );
            cout << e.what() << endl;
            SetConsoleTextAttribute( hOut, 7 );
            cin.clear();
            cin.sync();
            system("pause");
        }
    }while(true);
return 0;
}

void registerTabel(){
    fstream file;

    SetConsoleTextAttribute( hOut, 10 );
    cout << endl << "*.ph file address: ";
    SetConsoleTextAttribute( hOut, 7);
    cin >> fileAddress;

    if((!file.good())||(fileAddress.substr(fileAddress.size() - 3, 3) != ".ph")){
        throw(std::logic_error("---Wrong file address!---"));
    }

    topdir = fileAddress.substr(0,10);

    if(fileAddress.find("spr.") != string::npos){
        coreAddress = "spr";
    }else if((fileAddress.find("cp14.") != string::npos)||(fileAddress.find("cp15.") != string::npos)){
        coreAddress = "cp14/15";
    }else{
        coreAddress = "none";
    }

    if(coreAddress == "none"){
        SetConsoleTextAttribute( hOut, 10 );
        cout << "Base address: ";
        SetConsoleTextAttribute( hOut, 7 );
        cin >> baseAddress;

        if (baseAddress != "0" ){
            for (int unsigned i = 2; i < baseAddress.length()-2;i++){                                                                    //! Checking corrections of base address
                if(!isxdigit(baseAddress[i])){
                    throw(std::logic_error("---Wrong base address!---"));
                }
            }
        }

        SetConsoleTextAttribute( hOut, 10 );                                                                                    //! Creating tab with values from *.p file
        cout << "Number of values from file *.p: ";
        SetConsoleTextAttribute( hOut, 7 );
        cin >> numberOfParams;

    }else{
        baseAddress = "0";
        numberOfParams = 0;
    }

    list<string> params;

    for (int i = 0; i < numberOfParams; i++){
        string value;
        SetConsoleTextAttribute( hOut, 10 );        cout << "Value of " << i+1 << " param: ";
        SetConsoleTextAttribute( hOut, 7 );
        cin >> value;

        params.push_back(value);
    }

    system("cls");

    Register::searchOperations(fileAddress,baseAddress,params,coreAddress,topdir);

    system("cls");
}

void moduleTabel(){
    fstream file;

    Module::actualId = 0;

    SetConsoleTextAttribute( hOut, 10 );
    cout << endl << "*.p file address: ";
    SetConsoleTextAttribute( hOut, 7);
    cin >> fileAddress;


    if((!file.good())||(fileAddress.substr(fileAddress.size() - 2, 2) != ".p")){
        throw(std::logic_error("---Wrong file address!---"));
    }

    file.open(fileAddress.c_str(), ios::in);

    topdir = fileAddress.substr(0,10);

    string name, tmpName, base, memoryClass, address;
    list <string> parameters;
    list <Module> modules;
    list <string> trees;
    bool isModule = false;

    system("cls");

    do{
        while(getline(file,line)){
            if((line.find("tree.end") != string::npos)&&(isModule == true)){

                string fullName;

                for(list<string>::iterator i = trees.begin(); i != trees.end(); ++i){
                    fullName = fullName + *i + " - ";
                }
                fullName.erase(fullName.length()-3,fullName.length());

                Module module(fullName, base, address, memoryClass, parameters);

                modules.push_back(module);
                trees.pop_back();
                parameters.clear();
                isModule = false;
            }else if((line.find("tree.end") != string::npos)){
                trees.pop_back();
            }else if(line.find("tree") != string::npos){
                line = line.substr(line.find('"') + 1,line.size());
                trees.push_back(line.substr(0,line.find('"')));
            }else if(line.find("base ") != string::npos){
                base = line.substr(line.find(":") + 1,line.length());
                memoryClass = line.substr(line.find("base ") + 5,line.find(":") - line.find("base ")-5);
            }else if(line.find("%include ") != string::npos){

                if(line.find("${TOPDIR}") != string::npos){
                    address = line.substr(line.find("%include ") + 9, line.find(".ph") - line.find("%include ") - 6);
                    address.replace(address.find("${TOPDIR}"),9,topdir);
                }else{
                    address = fileAddress.substr(0,fileAddress.find_last_of("/\\") + 1) + line.substr(line.find("%include ") + 9, line.find(".ph") - line.find("%include ") - 6);
                }

                if(line.find(".ph ") != string::npos){
                    line = line.substr(line.find(".ph") + 4, line.size());
                    line = line + " ";

                    while(!line.empty()){
                        string param = line.substr(0,line.find(" "));
                        parameters.push_back(param);
                        line = line.erase(0,line.find(param)+param.size()+1);
                    }
                }
                isModule = true;
            }
        }

        file.close();
        system("cls");

        Module::print(modules);

        int choose;
        SetConsoleTextAttribute( hOut, 10 );
        cout << endl << "Choose number of module | Select '0' to exit: ";
        SetConsoleTextAttribute( hOut, 7);
        cin >> choose;

        if(choose == 0)
            break;

        system("cls");

        for(list<Module>::iterator i = modules.begin(); i!=modules.end();++i){
            Module m = *i;

            if(choose == m.id){
                SetConsoleTextAttribute( hOut, 10 );
                cout << "CHOSEN MODULE: " ;
                SetConsoleTextAttribute( hOut, 7);
                cout << m.name << endl;
                SetConsoleTextAttribute( hOut, 10 );
                cout << "FILE LOCATION: ";
                SetConsoleTextAttribute( hOut, 7);
                cout << m.fileaddress << endl;
                SetConsoleTextAttribute( hOut, 10 );
                cout << "BASE ADDRESS:  ";
                SetConsoleTextAttribute( hOut, 7);
                cout << m.baseaddress << endl;
                SetConsoleTextAttribute( hOut, 10 );
                cout << "PARAMETERS:    ";
                SetConsoleTextAttribute( hOut, 7);

                for(list<string>::iterator i = m.prameters.begin(); i != m.prameters.end();++i){
                    cout << *i << " ";
                }
                cout << endl << endl;
                Register::searchOperations(m.fileaddress, m.baseaddress, m.prameters, "none",topdir);
            }
        }
    }while(true);
}
