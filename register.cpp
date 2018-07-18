#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include "register.h"

using namespace std;

bool Register::first_print = true;

void Register::print(int width, Register r, string coreAddress){
    string str, floor;

    if (width > 23){                                                                                                                //! Created string with blank space and floor equal in length of name column
        for(int i = 0;i < width + 8 ; i++){
            str = str + " ";
            floor = floor + "_";
        }
    }else{
        for(int i = 0;i < 26 ; i++){
            str = str + " ";
            floor = floor + "_";
        }
    }

    if (coreAddress == "none"){                                                                                                     //! Printed table for standart *.ph file
        if(first_print == true){                                                                                            //! Printed table header
            cout << " " << floor << "________________________________________________________________________________" << endl;
            cout << "|" << str.substr(0,5) <<"Register name" << str.substr(0,str.length()-18) << "|";
            cout << str.substr(0,9) << "Access" << str.substr(0,9) << "|";
            cout << str.substr(0,7) << "Address" << str.substr(0,7) << "|";
            cout << str.substr(0,5) << "Offset" << str.substr(0,5) << "|";
            cout << str.substr(0,5) << "Range" << str.substr(0,5) << "|" << endl;
            cout << "|" << floor << "|________________________|_____________________|________________|_______________|" << endl;

            first_print = false;
        }

        cout << "|" << str.substr(0,3) << r.name << str.substr(0,str.length()-r.name.length()-3) << "|";                    //! print Register name
        cout << str.substr(0,5) << r.access << str.substr(0,24-r.access.length()-5) << "|";                                 //! print Register access
        cout << str.substr(0,5) << r.address << str.substr(0,21-r.address.length()-5) << "|";                               //! print Register address
        cout << str.substr(0,6) << r.offset << str.substr(0,16-r.offset.length()-6) << "|";                                 //! print Register offset
        cout << str.substr(0,6) << r.range << str.substr(0,15-r.range.length()-6) << "|" << endl;                           //! print Register range
        cout << "|" << floor << "|________________________|_____________________|________________|_______________|" << endl;
    }else if (coreAddress == "spr"){                                                                                                //! Printed table for Core SPR *.ph file
        if(r.offset.length() < 7){
            string s;
            for(int i = 0; i < 7-r.offset.length(); i++){
                s = s + "0";
            }
            r.offset.insert(2,s);
        }
        if(first_print == true){                                                                                            //! Printed table header
            cout << " " << floor << "_______________________________________________________________________________________________________" << endl;
            cout << "|" << str.substr(0,5) <<"Register name" << str.substr(0,str.length()-18) << "|";
            cout << str.substr(0,9) << "Access" << str.substr(0,9) << "|";
            cout << str.substr(0,7) << "Address" << str.substr(0,7) << "|";
            cout << "  " << "OP0" << "  |";
            cout << "  " << "OP1" << "  |";
            cout << "  " << "CRn" << "  |";
            cout << "  " << "CRm" << "  |";
            cout << "  " << "OP2" << "  |";
            cout << str.substr(0,5) << "Range" << str.substr(0,5) << "|" << endl;
            cout << "|" << floor << "|________________________|_____________________|_______|_______|_______|_______|_______|_______________|" << endl;

            first_print = false;
        }

        cout << "|" << str.substr(0,3) << r.name << str.substr(0,str.length()-r.name.length()-3) << "|";                        //! print Register name
        cout << str.substr(0,5) << r.access << str.substr(0,24-r.access.length()-5) << "|";                                     //! print Register access
        cout << str.substr(0,5) << r.address << str.substr(0,21-r.address.length()-5) << "|";                                   //! print Register address
        cout << "   " << r.offset[2] << "   |";
        cout << "   " << r.offset[3] << "   |";
        cout << "   " << r.offset[4] << "   |";
        cout << "   " << r.offset[5] << "   |";
        cout << "   " << r.offset[6] << "   |";
        cout << str.substr(0,6) << r.range << str.substr(0,15-r.range.length()-6) << "|" << endl;                               //! print Register range
        cout << "|" << floor << "|________________________|_____________________|_______|_______|_______|_______|_______|_______________|" << endl;
    }else if(coreAddress == "cp14/15"){                                                                                                     //! Printed table for Core CP14/15 *.ph file
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
            cout << "  " << "OP1" << "  |";
            cout << "  " << "OP2" << "  |";
            cout << "  " << "CRm" << "  |";
            cout << "  " << "CRn" << "  |";
            cout << str.substr(0,5) << "Range" << str.substr(0,5) << "|" << endl;
            cout << "|" << floor << "|________________________|_____________________|_______|_______|_______|_______|_______________|" << endl;

            first_print = false;
        }

        cout << "|" << str.substr(0,3) << r.name << str.substr(0,str.length()-r.name.length()-3) << "|";                        //! print Register name
        cout << str.substr(0,5) << r.access << str.substr(0,24-r.access.length()-5) << "|";                                     //! print Register access
        cout << str.substr(0,5) << r.address << str.substr(0,21-r.address.length()-5) << "|";                                   //! print Register address
        cout << "   " << r.offset[2] << "   |";
        cout << "   " << r.offset[3] << "   |";
        cout << "   " << r.offset[4] << "   |";
        cout << "   " << r.offset[5] << "   |";                                                                                 //! print Register offset
        cout << str.substr(0,6) << r.range << str.substr(0,15-r.range.length()-6) << "|" << endl;                               //! print Register range
        cout << "|" << floor << "|________________________|_____________________|_______|_______|_______|_______|_______________|" << endl;
    }
}



