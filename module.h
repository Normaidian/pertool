#include <string>
#include <list>


using namespace std;

class Module{

    public:
        int id;
        string name;
        string baseaddress;
        string fileaddress;
        string memoryClass;
        list <string> prameters;

    public:
        Module(int id, string name, string baseaddress, string fileaddress, string memoryClass, list <string> prameters){
            this -> id = id;
            this -> name = name;
            this -> baseaddress = baseaddress;
            this -> fileaddress = fileaddress;
            this -> memoryClass = memoryClass;
            this -> prameters = prameters;
        }


};
