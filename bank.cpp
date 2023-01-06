#include "./bank.h"

#ifndef _BANK_CPP
#define _BANK_CPP
//*---------------------------DATE------------------------------------------------------

int date::monthDay[12] = { 0,31,59,90,120,151,181,212,243,273,304,334 };

bool date::leapYear(int i)const { return (i % 4 == 0 && (i % 100 != 0 || i % 400 == 0)) ? true : false; }

date::date() { year = month = day = 0; }

date::date(int year, int month, int day) {
	if (year <= 0) { throw "Invalid date"; }
	this->year = year;
	if (month < 1 || month > 12) { throw "Invalid date"; }
	this->month = month;
	if (day < 1 || day > getMaxDay()) { throw "Invalid date"; }
	this->day = day;
}

bool date::ifLeap() { return leapYear(year); }

date::date(const date& A) { year = A.year;    month = A.month;    day = A.day; }

int date::operator-(const date& A)const {
	int Day = ((year - A.year) * 365) + (monthDay[month - 1] - monthDay[A.month - 1]) + (day - A.day);
	if (A.year <= year) {
		for (int i = A.year; i < year; ++i)
			if (leapYear(i)) ++Day;
	}
	else {
		for (int i = year; i < A.year; ++i)
			if (leapYear(i)) --Day;
	}                                                 //计算闰年误差
	if (month > 2 && leapYear(year)) ++Day;
	if (A.month > 2 && leapYear(A.year)) --Day;        //计算闰月误差
	return Day;
}

std::ostream& operator<<(std::ostream& out, const date& A) { out << A.year << '-' << A.month << '-' << A.day; return out; }

void dateSwap(date& a, date& b){
	int t_year = a.year;
	int t_month = a.month;
	int t_day = a.day;

	a.year = b.year;
	a.month = b.month;
	a.day = b.day;

	b.year = t_year;
	b.month = t_month;
	b.day = t_day;
}

void date::operator=(const date& A) { year = A.year;    month = A.month;    day = A.day; }

void date::show() { std::cout << *this; }

void date::changeDay(int outDay){
	if(outDay < 0 || outDay > 31)
		throw "Invalid day";
	day = outDay;
}
int date::getDay() { return day; } int date::getYear() { return year; } int date::getMonth() { return month; }

int date::getMaxDay() {
	if (month == 1) {
		return 31;
	}
	if (month == 2) {
		return 28 + leapYear(year);
	}
	if (month == 12) {
		return 31;
	}
	return (monthDay[month] - monthDay[month - 1]);
}

bool operator<(const date& a, const date& b) { if (a.year - b.year < 0)return true; else return false; return a - b < 0 ? true : false; }

bool operator==(const date& a, const date& b) { return (a.year == b.year && a.month == b.month && a.day == b.day); }

//*---------------------------DATE------------------------------------------------------

//*---------------------------ACCOUMULATOR------------------------------------------------------

accumulator::accumulator() {}

accumulator::accumulator(const date& outDate, double outValue) {
	_lastdate = outDate;
	_value    = outValue;}

void accumulator::change(const date& outDate, double outValue) {
	_lastdate = outDate;
	_value 	  = outValue;}

void accumulator::clearSum() { _sum = 0; }

bool accumulator::reset(const date& outDate, double outValue) {
	_sum += (-(_lastdate - outDate) * _value);
	bool tp = _lastdate.ifLeap();
	_lastdate = outDate;
	_value = outValue;
	return tp;
}

double const accumulator::getSum() { return _sum;}

//*---------------------------ACCOUMULATOR------------------------------------------------------


//*---------------------------ACCOUNT------------------------------------------------------

double account::_total = 0;  

account::account(const std::string& outId,double outRate)
:_id(outId),_rate(outRate),_balance(0){}

void account::add(double amount){_balance += amount;_total += amount;}

double  account::round(double amount){return floor(amount*100+0.5)/100;}

std::string account::getId()const {return _id;}

const double account::getRate()const{return _rate;}

const double account::getBalance()const{return _balance;}

const double account::getTotal(){return account::_total;}

//*---------------------------ACCOUNT------------------------------------------------------

//*---------------------------DATE_BILL------------------------------------------------------

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
        std::cout<<"date: "<<_date<<" "<<*i<<std::endl;
}

const date& dateBill::getDate(){return _date;}

mystd::veclist<std::string>& dateBill::getDesc(){return _desc;}

//*---------------------------DATE_BILL------------------------------------------------------

//*---------------------------BILL------------------------------------------------------

bill::bill(){}

bill::bill(account* outMaster):_master(outMaster){}

void bill::pushBill(const date& outDate,const std::string& outDesc){
    auto it = _dateBill.find(_dateBill.begin(),_dateBill.end(),outDate);
    if(it == _dateBill.end())
        _dateBill.push_back(new dateBill(outDate,outDesc));
    else
        (*it).pushDesc(outDesc);
}

void bill::show(mystd::veclist<dateBill>::size_type it){
    std::cout<<"---------------------------------------------"<<std::endl;
    std::cout<<"User id: "<<_master->getId()<<std::endl;
    _dateBill[it].show();
    std::cout<<"---------------------------------------------"<<std::endl;
}

void bill::show(){
    std::cout<<"---------------------------------------------"<<std::endl;
    std::cout<<"User id: "<<_master->getId()<<std::endl;
    for(auto it = _dateBill.begin();it!=_dateBill.end();++it)
        (*it).show();
    std::cout<<"---------------------------------------------"<<std::endl;
}

mystd::veclist<dateBill>& bill::getDateBill(){return _dateBill;}

mystd::veclist<dateBill>::size_type bill::size(){return _dateBill.size();}

const date& bill::getDate(mystd::veclist<dateBill>::size_type it){return _dateBill[it].getDate();}

//*---------------------------BILL------------------------------------------------------

//*---------------------------CREDIT_ACCOUT------------------------------------------------------

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

//*---------------------------CREDIT_ACCOUT------------------------------------------------------

//*---------------------------SAVING_ACCOUT------------------------------------------------------

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

//*---------------------------SAVING_ACCOUT------------------------------------------------------

//*-----------------------------ACCOUNT_EXCEPTION----------------------------------------------------

accountException::accountException(const char* error,account* outAcc)
:std::runtime_error(error),_master(outAcc){}

account* accountException::getAcc(){return _master;}

const char* accountException::what()const throw(){return runtime_error::what();}

void accountException::show(){
    std::cout<<"----------------------------------"<<std::endl;
    std::cout<<"Id :"<<_master->getId()<<" error: "<<what()<<std::endl;
    std::cout<<"----------------------------------"<<std::endl;
}

accountException::~accountException() {
		std::cout<<"delete accex"<<'\n';
}

//*-----------------------------ACCOUNT_EXCEPTION----------------------------------------------------

//*-----------------------------MY_FILE----------------------------------------------------


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

bool myFile::is_open_read(){return inf.is_open();}

bool myFile::is_open_write(){return outf.is_open();}

//*-----------------------------MY_FILE----------------------------------------------------

//*-----------------------------CMD_FILE----------------------------------------------------

cmdFile::cmdFile(const std::string& fileName):myFile(fileName){}

void cmdFile::readCmdFile(bool (*fuction) (std::istream&) ){
    std::ifstream &_inf = getInf();
    while(_inf.peek()!= EOF){
        fuction(_inf);
    }
}

//*-----------------------------CMD_FILE----------------------------------------------------

//*-----------------------------RECORD_CMD----------------------------------------------------


recordCmd::recordCmd():_itm(&_buf){}

void recordCmd::readCmd(std::istream& in){
    in.getline(_buffer,50);
    for(int i = 0;i < 50;++i){
        if(_buffer[i] == '\0'){
            _buffer[i] = '\n';
            break;
        }
    }
    _buf.str(_buffer);
}

void recordCmd::clearBuff(){
    for(int i = 0;i < 50;++i)
        _buffer[i] = '\0';
}

void recordCmd::clearIstream(){
    _itm.clear();//*清除异常状态
}


void recordCmd::record(){
    _record.append(_buffer);
}

std::istream& recordCmd::getIstream(){
    return _itm;
}

std::string& recordCmd::getRecord(){
    return _record;
}

bool recordCmd::ifEnd(){
    return _buffer[0] == 'e'; 
}

//*-----------------------------RECORD_CMD----------------------------------------------------

#endif