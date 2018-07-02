#include <string>


using namespace std;

class Module{

    public:
        int id;
        string name;
        string baseaddress;
        string fileaddress;
        string memoryClass;

    public:
        Module(int id, string name, string baseaddress, string fileaddress, string memoryClass){
            this -> id = id;
            this -> name = name;
            this -> baseaddress = baseaddress;
            this -> fileaddress = fileaddress;
            this -> memoryClass = memoryClass;
        }


};
