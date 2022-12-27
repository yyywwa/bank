#include"../Header/account.h"

bill::bill(){}

bill::bill(account* outMaster):_master(outMaster){}

void bill::pushBill(const date& outDate,const std::string& outDesc){
    auto it = _dateBill.find(_dateBill.begin(),_dateBill.end(),outDate);
    if(it == -1)
        _dateBill.push_back(new dateBill(outDate,outDesc));
    else
        _dateBill[it].pushDesc(outDesc);
}

void bill::show(veclist<dateBill>::size_type it){
    std::cout<<"---------------------------------------------"<<std::endl;
    std::cout<<"User id: "<<_master->getId()<<std::endl;
    _dateBill.at(it).show();
    std::cout<<"---------------------------------------------"<<std::endl;
}

void bill::show(){
    std::cout<<"---------------------------------------------"<<std::endl;
    std::cout<<"User id: "<<_master->getId()<<std::endl;
    for(auto it = _dateBill.begin();it!=_dateBill.end();++it)
        _dateBill[it].show();
    std::cout<<"---------------------------------------------"<<std::endl;
}

veclist<dateBill>& bill::getDateBill(){return _dateBill;}

veclist<dateBill>::size_type bill::size(){return _dateBill.size();}

const date& bill::getDate(veclist<dateBill>::size_type it){return _dateBill.at(it).getDate();}