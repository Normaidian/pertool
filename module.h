#include <string>


using namespace std;

class Module{

    private:
        int actualID = 0;

    public:
        int id;
        string name;
        string baseaddress;
        string fileaddress;
        string memoryClass;

    public:
        Module(string name, string baseaddress, string fileaddress, string memoryClass){
            this -> id = createID(actualID);
            this -> name = name;
            this -> baseaddress = baseaddress;
            this -> fileaddress = fileaddress;
            this -> memoryClass = memoryClass;
        }

        int createID(int id);

};
