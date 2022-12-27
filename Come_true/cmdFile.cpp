#include"../Header/account.h"

cmdFile::cmdFile(const std::string& fileName):myFile(fileName){}

void cmdFile::readCmdFile(bool (*fuction) (std::istream&) ){
    std::ifstream &_inf = getInf();
    while(_inf.peek()!= EOF){
        try{
            fuction(_inf);
        }catch(const char* e){
            std::cout<<e<<std::endl;
        }
    }
}
