#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <list>
#include "register.h"
#include "module.h"

using namespace std;

fstream file;
int width, numberOfParams = 0;
string line, fileAddress, baseAddress, tempForLine, tempGroupLine, coreAddress = "none", hexOrDec = "dec";

void registerTabel();
void moduleTabel();

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
Register r;
Group g;

int main(){
    do{
        try{
            system("cls");
            SetConsoleTextAttribute( hOut, 7);
            width = 0;
            int choice;

            cout << " ____________________________________"<< endl;
            cout << "|               MENU                 |" << endl;
            cout << "|____________________________________|" << endl;
            cout << "|    1 - Get table with modules      |" << endl;
            cout << "|____________________________________|" << endl;
            cout << "|    2 - Get table with registers    |" << endl;
            cout << "|____________________________________|" << endl;
            cout << "|    0 - Exit                        |" << endl;
            cout << "|____________________________________|" << endl;
            cout << "Select operation: ";
            cin >> choice;

                switch(choice){

                    case 1:
                        moduleTabel();
                    break;
                    case 2:
                        registerTabel();
                    break;
                    case 0:
                        exit(0);
                    break;
                    default:
                        throw(std::logic_error("---Wrong choice!---"));
                    break;
            }
        }catch(std::logic_error &e){
            SetConsoleTextAttribute( hOut, 12 );
            cout << e.what() << endl;
            SetConsoleTextAttribute( hOut, 7 );
            system("pause");
        }
    }while(true);
return 0;
}

void registerTabel(){
    //! Variables declaration
    bool insideIf = false, insideIfElse = false, insideFor = false;

    do{                                                                                                                 //! Checking corrections of file address
        SetConsoleTextAttribute( hOut, 10 );
        cout << endl << "File address: ";
        SetConsoleTextAttribute( hOut, 7);
        cin >> fileAddress;

        file.open(fileAddress.c_str(), ios::in);

        if(!file.good()){
            throw(std::logic_error("---Wrong file address!---"));
        }


        if(fileAddress.find("spr.") != string::npos){
            coreAddress = "spr";
        }else if((fileAddress.find("cp14.") != string::npos)||(fileAddress.find("cp15.") != string::npos)){
            coreAddress = "cp14/15";
        }else{
            coreAddress = "none";
        }
    }while(!file.good());


    if(coreAddress == "none"){
        SetConsoleTextAttribute( hOut, 10 );
        cout << "Base address: ";
        SetConsoleTextAttribute( hOut, 7 );
        cin >> baseAddress;

        if (baseAddress != "0" ){
            for (int i = 2; i < baseAddress.length()-2;i++){                                                                    //! Checking corrections of base address
                if(!isxdigit(baseAddress[i])){
                    throw(std::logic_error("---Wrong base address!---"));
                }
            }
        }

        SetConsoleTextAttribute( hOut, 10 );                                                                                //! Creating tab with values from *.p file
        cout << "Number of values from file *.p: ";
        SetConsoleTextAttribute( hOut, 7 );
        cin >> numberOfParams;

    }else{
        baseAddress = "0";
        numberOfParams = 0;
    }

    string tabParams[numberOfParams];

    for (int i =0; i < numberOfParams; i++){
        string value;
        SetConsoleTextAttribute( hOut, 10 );        cout << "Value of " << i+1 << " param: ";
        SetConsoleTextAttribute( hOut, 7 );
        cin >> value;

        tabParams[i] = value;
    }

    system("cls");

    while(getline(file, line)){

        for (int i = 1; i <= numberOfParams; i++){                                                                      //! Replace the parameter number with its value
            string param = "%(" + r.toString(i) + ")";

            while(line.find(param) != string::npos){
                line = line.replace(line.find(param),4,tabParams[i-1]);
            }
        }

        if((line.find("group.") != string::npos)){                                                                      //! creating Group object
                tempGroupLine = line;
                g = g.searching(line);
        }else if(line.find("width ") != string::npos){                                                                  //! setting values of width
            if(line.find("0x") != string::npos){
                if(r.hexToDec(line.substr(line.find("0x")+2,line.size())) > width){
                    width = r.hexToDec(line.substr(line.find("0x")+2,line.size()));
                }
            }else{
                if(atoi(line.substr(line.find("width ")+6,line.find(".")-line.find("width ")-1).c_str()) > width){
                    width = atoi(line.substr(line.find("width ")+6,line.find(".")-line.find("width ")-1).c_str());
                }
            }

            bool *pointer = &r.first_print;
            *pointer = true;
        }else if(line.find("base ") != string::npos){
            baseAddress = line.substr(line.find("base ") + 5, line.size() - line.find("base ") + 5);
        }else if((line.find("%for") != string::npos)){                                                                  //! entry to for condition
                tempForLine = line;
                insideFor = true;
        }else if((line.find("%endfor") != string::npos)){                                                               //! exit from for condition
                insideFor = false;
        }else if(line.find("endif")!= string::npos){                                                                    //! exit to IF condition
                insideIf = false;
                insideIfElse = false;
        }else if((line.find("else") != string::npos)||(line.find("elif")!=string::npos)){                               //! entry to ELSE/ELIF condition
                insideIfElse = true;
        }else if((line.find("if ") != string::npos)&&(line.find("bitfld")==string::npos)){                              //! entry to IF condition
            insideIf = true;
        }else if(((line.find("line.") != string::npos)||(line.find("hide.")!=string::npos))&&insideIfElse == false){    //! making register object and print it on screen
            if(insideFor == true){
                r.forOperations(line, tempForLine, tempGroupLine, width, baseAddress, insideIf, insideFor, coreAddress);
            }else{
                r.print(width,r.searching(line,g, baseAddress, insideIf, insideFor), coreAddress);
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
    main();
}

void moduleTabel(){


    SetConsoleTextAttribute( hOut, 10 );
    cout << endl << "File address: ";
    SetConsoleTextAttribute( hOut, 7);
    cin >> fileAddress;

    file.open(fileAddress.c_str(), ios::in);

    if((!file.good())&&(fileAddress.substr(fileAddress.length()-2,fileAddress.length()) != ".p")){
        throw(std::logic_error("---Wrong file address!---"));
    }

    int id = 0, counter = 0,maxCounter = 0;
    string name, tmpName, base, memoryClass, address;
    list <string> parameters;

    system("cls");

    while(getline(file,line)){

        if((line.find("tree.end") != string::npos)&&(counter == maxCounter)){
            Module module(id, name, base, address, memoryClass, parameters);
            cout <<  "id:          " << module.id
            <<endl<< "name:        " << module.name
            <<endl<< "baseAddress: " << module.baseaddress
            <<endl<< "fileAddress: " << module.fileaddress
            <<endl<< "memoryClass: " << module.memoryClass
            <<endl<< "________________________________________"<<endl;
            id++;
            --counter;
            parameters.clear();
            name = name.erase(name.find(tmpName),tmpName.size());
        }else if((line.find("tree.end") != string::npos)&&(counter != maxCounter)){
            --counter;
        }else if(line.find("tree") != string::npos){
            line = line.substr(line.find(' "') + 1,line.size());

            if (counter > 0){
                name = name + " - " + line.substr(0,line.find('"'));
                tmpName = " - " + line.substr(0,line.find('"'));
            }else{
                name = line.substr(0,line.find('"'));
                tmpName = line.substr(0,line.find('"'));
            }
            ++counter;
        }else if(line.find("base ") != string::npos){
            base = line.substr(line.find(":") + 1,line.length());
            memoryClass = line.substr(line.find("base ") + 5,line.find(":") - line.find("base ")-5);
        }else if(line.find("%include ") != string::npos){
            address = fileAddress.substr(0,fileAddress.find_last_of("/\\") + 1) + line.substr(line.find("%include ") + 9, line.find(".ph") - line.find("%include ") - 6);

            if(line.find(".ph ") != string::npos)
                line = line.substr(line.find(".ph") + 4, line.size());

            while(!line.empty()){
                string param = line.substr(0,line.find(" "));
                parameters.push_back(param);
                line = line.erase(0,line.find(param)+param.size()+1);
                cout << line.size() << "| |" << param << endl;
            }

            cout << endl;
            for(list<string>::iterator i = parameters.begin(); i!=parameters.end();++i){
                cout << *i << endl;
            }
            cout << endl;
        }

        if (counter > maxCounter){
            maxCounter = counter;
        }

        if((line.find("tree.end") != string::npos)&&(counter == 0)){
            maxCounter = 0;
            name = "";
        }
    }

    file.close();
    system("pause");
}
