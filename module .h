using namespace std;

class Module{

    private:
        string name;
        string baseaddress;
        string fileaddress;
        string memoryClass;

    public:
        Module(string name, string baseaddress, string fileaddress, string memoryClass){
            this -> name = name;
            this -> baseaddress = baseaddress;
            this -> fileaddress = fileaddress;
            this -> memoryClass = memoryClass;
        }

}
