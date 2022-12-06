#include"../Header/account.h"

savingAccount::savingAccount(const date& outDate, std::string& outId, double outRate)
:account(outId,outRate){
	_amr.change(outDate, 0);
	if (outRate > 0.24) {
		throw "The interest is too high";
	}
	if (outRate < 0.03) {
		throw "Interest is too low";
	}
}

void savingAccount::deposit(const date& outDate,double amount){
    if(amount < 0)
        throw "Bad amount";
    add(amount);
    _amr.reset(outDate,getBalance());
}

void savingAccount::withdraw(const date& outDate,double amount){
    if(amount < 0)
        throw "Bad amount";
    amount *= -1;
    if(getBalance() >= -amount){
        add(amount);
        _amr.reset(outDate,getBalance());
    } 
    else{
        throw "Lack of balance";
    }
}

void savingAccount::settle(date& outDate){
    if(outDate.getMonth() == 1 && outDate.getDay() == 1){
        bool _if_leepYear = _amr.reset(outDate,0);
        double amount = round(_amr.getSum()/(365+_if_leepYear)*getRate());
        add(amount);
        _amr.change(outDate,getBalance());
    }
}

void savingAccount::show(){
    std::cout << getId() << '\t' << "Balance:" << ' ' << getBalance();
}