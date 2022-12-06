#include"../Header/account.h"

double account::_total = 0;  

account::account(const std::string& outId,double outRate)
:_id(outId),_rate(outRate),_balance(0){}

void account::add(double amount){_balance += amount;_total += amount;}

double  account::round(double amount){return floor(amount*100+0.5)/100;}

std::string account::getId()const {return _id;}

const double account::getRate()const{return _rate;}

const double account::getBalance()const{return _balance;}

const double account::getTotal(){return account::_total;}