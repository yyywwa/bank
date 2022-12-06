#include"../Header/account.h"

dateBill::dateBill(){}

dateBill::dateBill(const date& outDate,const std::string& outDesc){
    _date = outDate;
    _desc.push_back(new std::string(outDesc));
}

void dateBill::pushDesc(const std::string& outDesc){
    _desc.push_back(new std::string(outDesc));
}

bool dateBill::operator==(const dateBill& outDateBill){return _date==outDateBill._date;}

bool dateBill::operator==(const date& outDate){return  _date==outDate;}

void dateBill::show(){
    if(_desc.empty())
        std::cout<<"date: "<<_date<<" is NULL"<<std::endl;
    for(auto i = _desc.begin(); i != _desc.end();++i)
        std::cout<<"date: "<<_date<<" "<<_desc[i]<<std::endl;
}

const date& dateBill::getDate(){return _date;}

veclist<std::string>& dateBill::getDesc(){return _desc;}