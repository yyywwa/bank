#include"../Header/account.h"

myFile::myFile(const std::string& fileName){
    inf.open(fileName);
    outf.open(fileName,std::ofstream::app);
}

myFile::~myFile(){
    inf.close();
    if(outf.is_open())
        outf.close();
}

void myFile::writeFile(const std::string& lineCmd){outf<<lineCmd;}

std::ifstream& myFile::getInf(){return inf;}

std::ofstream& myFile::getOutf(){return outf;}