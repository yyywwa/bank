#include"../Header/account.h"

accountException::accountException(const char* error,account* outAcc)
:std::runtime_error(error),_master(outAcc){}

account* accountException::getAcc(){return _master;}

const char* accountException::what()const throw(){return runtime_error::what();}

void accountException::show(){
    std::cout<<"----------------------------------"<<std::endl;
    std::cout<<"Id :"<<_master->getId()<<" error: "<<what()<<std::endl;
    std::cout<<"----------------------------------"<<std::endl;
}