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
        cout << " __________________________________________________________________________________"  << endl;
        cout << "| ID |      Module name                |    Base address            | Memory Class |"  << endl;
        cout << "|____|_________________________________|____________________________|______________|"  << endl;

        for(list<Module>::iterator i = moduleList.begin(); i!=moduleList.end();++i){
            Module m = *i;
            cout << "| " << m.id << " |  " << m.name << "  |  " << m.baseaddress << "  |  " << m.memoryclass << "  |" << endl;
            cout << "|____|_________________________________|____________________________|______________|"  << endl;
        }
    }
};
