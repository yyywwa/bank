#include"../Header/account.h"

creditAccount::creditAccount(const date& outDate,const std::string& outId,double outCredit,double outRate,double outFee)
:account(outId,outRate){
    _amr.change(outDate, 0);
	if (outRate > 0.0006) {
		throw "The interest is too high";
	}
	if (outRate < 0.0001) {
		throw "The interest is too low";
	}
	if(outCredit < 1000 ){
		throw "The credit is too low";
	}
	if (outFee > 500) {
		throw "The fee is too high";
	}
	if (outFee < 5) {
		throw "The fee  is too low";
	}
    _credit = outCredit;
    _fee    = outFee;
}

void creditAccount::reAccount(const date& outDate){
	if(getBalance() >= 0)
		_amr.change(outDate,0);
	else
		_amr.change(outDate,getBalance());
}

void creditAccount::deposit(const date& outDate,double amount){
	if(amount < 0)
		throw "bad amount";
	
	_amr.reset(outDate,0);
	double debt = round(_amr.getSum()*getRate());
	_amr.clearSum();
	
	add(amount+debt);
	reAccount(outDate);
}

void creditAccount::withdraw(const date& outDate,double amount){
	if(amount < 0)
		throw "bad amount";
	amount *= -1;

	_amr.reset(outDate,0);
	double debt = round(_amr.getSum()*getRate());

	if(getBalance() + debt + amount >= -_credit){
		_amr.clearSum();
		add(amount + debt);
		reAccount(outDate);
	}
	else{
		throw "Lack of balance";
	}
}

void creditAccount::settle(date& outDate){
	_amr.reset(outDate,0);
	double debt = round(_amr.getSum()*getRate());
	_amr.clearSum();

	if(outDate.getMonth() == 1 && outDate.getDay() == 1)
		add(debt - _fee);
	if(outDate.getMonth() != 1 && outDate.getDay() == 1)
		add(debt);	
	reAccount(outDate);
}

const double creditAccount::getAvailableCredit() { return getBalance() > 0 ? _credit : _credit + getBalance(); }

void creditAccount::show(){
	std::cout << getId() << '\t' << "Balance: " << getBalance()
		<< '\t' << "Available credit:" << getAvailableCredit();
}

