#include <string>
#include <list>

class Module{

    public:
        static int actualId;

        int id;
        std::string name;
        std::string baseaddress;
        std::string fileaddress;
        std::string memoryclass;
        std::list <std::string> prameters;

    public:
        Module(std::string name, std::string baseaddress, std::string fileaddress, std::string memoryclass, std::list <std::string> prameters){
            this -> id = makeID();
            this -> name = name;
            this -> baseaddress = baseaddress;
            this -> fileaddress = fileaddress;
            this -> memoryclass = memoryclass;
            this -> prameters = prameters;
        }

        int makeID();
        static void print(std::list<Module> moduleList){

            int unsigned length = 0;

            for(std::list<Module>::iterator i = moduleList.begin(); i!=moduleList.end();++i){
                Module m = *i;
                if (m.name.length() > length){
                    length = m.name.length();
                }
            }

            std::string floor = "_____", space = "     ";

            for (int unsigned i = 0; i < length; i++){
                floor = floor + "_";
                space = space + " ";
            }

            std::cout << " _______" << floor << "_____________________________________"  << std::endl;
            std::cout << "| ID   |      Module name" << space.substr(0,length - 12) << "|    Base address     | Memory Class |"  << std::endl;
            std::cout << "|______|" << floor << "|_____________________|______________|"  << std::endl;

            for(std::list<Module>::iterator i = moduleList.begin(); i!=moduleList.end();++i){
                Module m = *i;
                std::cout << "| " << m.id << space.substr(0,5 - std::to_string(m.id).length()) << "|   " << m.name << space.substr(0, length - m.name.length()) << "  |      " << m.baseaddress << "     |  " << m.memoryclass << space.substr(0,12 - m.memoryclass.length()) <<  "|" << std::endl;
                std::cout << "|______|" << floor << "|_____________________|______________|"  << std::endl;
            }
        }
};
