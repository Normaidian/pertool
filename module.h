#include <string>
#include <list>


using namespace std;

class Module{

    public:

        int id;
        string name;
        string baseaddress;
        string fileaddress;
        string memoryclass;
        list <string> prameters;

    public:
        Module(string name, string baseaddress, string fileaddress, string memoryclass, list <string> prameters){
            this -> id = makeID();
            this -> name = name;
            this -> baseaddress = baseaddress;
            this -> fileaddress = fileaddress;
            this -> memoryclass = memoryclass;
            this -> prameters = prameters;
        }

    int makeID();
    static void print(list<Module> moduleList){

        int length = 0;

        for(list<Module>::iterator i = moduleList.begin(); i!=moduleList.end();++i){
            Module m = *i;
            if (m.name.length() > length){
                length = m.name.length();
            }
        }

        string floor = "_____", space = "     ";

        for (int i = 0; i < length; i++){
            floor = floor + "_";
            space = space + " ";
        }

        cout << " _____" << floor << "____________________________________________"  << endl;
        cout << "| ID |      Module name" << space.substr(0,length - 12) << "|    Base address            | Memory Class |"  << endl;
        cout << "|____|" << floor << "|____________________________|______________|"  << endl;

        for(list<Module>::iterator i = moduleList.begin(); i!=moduleList.end();++i){
            Module m = *i;
            if (std::to_string(m.id).size() < 2){
                cout << "| " << m.id << "  |   " << m.name << space.substr(0, length - m.name.length()) << "  |  " << m.baseaddress << "  |  " << m.memoryclass << "  |" << endl;
                cout << "|____|" << floor << "|____________________________|______________|"  << endl;
            }else{
                cout << "| " << m.id << " |   " << m.name << space.substr(0, length - m.name.length()) << "  |  " << m.baseaddress << "  |  " << m.memoryclass << "  |" << endl;
                cout << "|____|" << floor << "|____________________________|______________|"  << endl;
            }
        }
    }
};
