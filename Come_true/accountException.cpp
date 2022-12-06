#include"../Header/account.h"

accountException::accountException(const char* error,account* outAcc):std::runtime_error(error),_acc(outAcc){}

account* accountException::getAcc(){return _acc;}

const char* accountException::what()const throw(){return runtime_error::what();}