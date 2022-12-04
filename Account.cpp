#include"./account.h"
Date Date::temp;
std::multimap<Date, AccountRecord> Account::recordMap;
AccountRecord::AccountRecord(const Account* acc, const Date& date, double amount, double balance, std::string desc) {
	account = acc;
	this->date = date;
	this->amount = amount;
	this->balance = balance;
	this->desc = desc;
}
AccountRecord::AccountRecord(const AccountRecord& A) {
	account = A.account;
	this->date = A.date;
	this->amount = A.amount;
	this->balance = A.balance;
	this->desc = A.desc;
}
void AccountRecord::show() {
	std::cout << date
		<< '\t' << '#' << account->getId()
		<< '\t' << amount
		<< '\t' << balance
		<< '\t' << desc << '\n';
}


double Account::total = 0;
Account::Account() { id = '\0'; balance = 0; }
Account::Account(std::string id) {
	this->id = id;
}
void Account::record(const Date& date, double amount, std::string desc) {
	balance += amount;
	total += amount;
	std::cout << date
		<< '\t' << '#' << id
		<< '\t' << amount
		<< '\t' << balance
		<< '\t' << desc << '\n';
	recordMap.insert(std::pair<Date, AccountRecord>(date, AccountRecord(this, date, amount, balance, desc)));
}
void Account::round(double& amount) {
	amount = floor(amount * 100 + 0.5) / 100;
}
double Account::roundT(double amount) {
	return floor(amount * 100 + 0.5) / 100;
}
const std::string Account::getId() const {
	return id;
}
const double Account::getBalance() {
	return balance;
}
void Account::query(const Date& a, const Date& b) {
	std::multimap<Date, AccountRecord>::iterator it = recordMap.begin();
	for (it; it != recordMap.end(); ++it) {
		if (a < it->first && it->first < b || a == it->first || b == it->first)
			it->second.show();
	}
}
double Account::getTotal() {
	return total;
}
void Account::setId(std::string id) {
	this->id = id;
}

Accumulator::Accumulator() {}
Accumulator::Accumulator(const Date& date, double value) {
	this->value = value;
	this->lastDate = date;
}
void Accumulator::change(const Date& date, double value) {
	this->lastDate = date;
	this->value = value;
}
void Accumulator::rezero() { sum = 0; }
bool Accumulator::reset(const Date& date, double value) {
	sum += (-(lastDate - date) * this->value);
	bool tp = lastDate.ifLeap();
	this->lastDate = date;
	this->value = value;
	return tp;
}
double const Accumulator::getSum() {
	return sum;
}

int Date::monthDay[12] = { 0,31,59,90,120,151,181,212,243,273,304,334 };
bool Date::leapYear(int i)const { return (i % 4 == 0 && (i % 100 != 0 || i % 400 == 0)) ? true : false; }
Date::Date() { year = month = day = 0; }
Date::Date(int year, int month, int day) {
	if (year <= 0) { throw "Invalid Date"; }
	this->year = year;
	if (month < 1 || month > 12) { throw "Invalid Date"; }
	this->month = month;
	if (day < 1 || day > getMaxDay()) { throw "Invalid Date"; }
	this->day = day;
}
bool Date::ifLeap() { return leapYear(year); }
Date::Date(const Date& A) { year = A.year;    month = A.month;    day = A.day; }
int Date::operator-(const Date& A)const {
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
std::ostream& operator<<(std::ostream& out, const Date& A) { out << A.year << '-' << A.month << '-' << A.day; return out; }
void Date::operator=(const Date& A) { year = A.year;    month = A.month;    day = A.day; }
void Date::show() { std::cout << *this; }
int Date::getDay() { return day; } int Date::getYear() { return year; } int Date::getMonth() { return month; }
int Date::getMaxDay() {
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
bool operator<(const Date& a, const Date& b) { if (a.year - b.year < 0)return true; else return false; return a - b < 0 ? true : false; }
bool operator==(const Date& a, const Date& b) { return (a.year == b.year && a.month == b.month && a.day == b.day); }
inline void Date::valuation(std::istream& in, int& a) { if (!(in >> a)) throw "Invalid Date"; }
Date& Date::read(std::istream& in) {
	valuation(in, temp.year);
	in.ignore(100, '/');     valuation(in, temp.month);
	in.ignore(100, '/');     valuation(in, temp.day);
	if (temp.year <= 0) { throw "Invalid Date"; }
	if (temp.month < 1 || temp.month > 12) { throw "Invalid Date"; }
	if (temp.day < 1 || temp.day > temp.getMaxDay()) { throw "Invalid Date"; }
	return temp;
}




CreditAccount::CreditAccount(const Date& date, std::string id, double credit, double rate, double fee) {
	acc.change(date, 0);
	setId(id);
	if (rate > 0.0006) {
		throw "The interest is too high";
	}
	if (rate < 0.0001) {
		throw "Interest is too low";
	}
	if (fee > 500) {
		throw "The fee is too high";
	}
	if (fee < 5) {
		throw "The fee  is too low";
	}
	this->credit = credit;
	this->rate = rate;
	this->fee = fee;
	std::cout << date << '\t' << '#' << id << " created" << '\n';
};
double CreditAccount::reDebt(const Date& date) {
	acc.reset(date, 0);
	double temp = acc.getSum() * rate;
	round(temp);
	acc.rezero();
	return temp;
}
void CreditAccount::reCharge(const Date& date, double amount, std::string& desc) {
	record(date, amount, desc);
	if (getBalance() >= 0) {
		acc.change(date, 0);
	}
	else {
		acc.change(date, getBalance());
	}
}
void CreditAccount::deposit(const Date& date, double amount, std::string desc) {
	double debt = reDebt(date);
	reCharge(date, amount + debt, desc);
}
void CreditAccount::withdraw(const Date& date, double amount, std::string desc) {
	amount *= -1;
	double debt = reDebt(date);
	if (getBalance() + amount + debt >= -credit) {
		reCharge(date, amount + debt, desc);
	}
	else {
		throw "Lack of balance";
	}
}
void CreditAccount::settle(Date& date) {
	double debt = reDebt(date);
	std::string desc("interest");
	if (date.getMonth() == 1 && date.getDay() == 1) {
		reCharge(date, debt - 50, desc);
		return;
	}
	if (date.getMonth() != 1 && date.getDay() == 1) {
		reCharge(date, debt, desc);
		return;
	}
}
const double CreditAccount::getDebt() { return roundT(acc.getSum() * rate); }
const double CreditAccount::getCredit() { return credit; }
const double CreditAccount::getRate() { return rate; }
const double CreditAccount::getFee() { return fee; }
const double CreditAccount::getAvailableCredit() { return getBalance() > 0 ? credit : credit + getBalance(); }
const void CreditAccount::error(std::string msg) { std::cout << "Can't do it " << msg; }
const void CreditAccount::show() {
	std::cout << getId() << '\t' << "Balance: " << getBalance()
		<< '\t' << "Available credit:" << getAvailableCredit();
}




SavingsAccount::SavingsAccount(const Date& date, std::string id, double rate) {
	acc.change(date, 0);
	setId(id);
	if (rate > 0.24) {
		throw "The interest is too high";
	}
	if (rate < 0.03) {
		throw "Interest is too low";
	}
	this->rate = rate;
	std::cout << date << '\t' << '#' << id << " created" << '\n';
}
void SavingsAccount::deposit(const Date& date, double amount, std::string desc) {
	record(date, amount, desc);
	acc.reset(date, getBalance());
}
void SavingsAccount::withdraw(const Date& date, double amount, std::string desc) {
	amount *= -1;
	if (getBalance() >= -amount) {
		record(date, amount, desc);
		acc.reset(date, getBalance());
	}
	else {
		throw "Lack of balance";
	}
}
void SavingsAccount::settle(Date& date) {
	if (date.getMonth() == 1 && date.getDay() == 1) {
		bool temp = acc.reset(date, 0);
		double amount = acc.getSum() / (365 + temp) * rate;
		round(amount);
		record(date, amount, "interest");
		acc.change(date, getBalance());
	}
}
const void SavingsAccount::error(std::string msg) {
	std::cout << "Can't do it " << msg;
}
const void SavingsAccount::show() {
	std::cout << getId() << '\t' << "Balance:" << ' ' << getBalance();
}
const double  SavingsAccount::getRate() {
	return rate;
}




AccountExceptionr::AccountExceptionr(const char* error,Account* acc):runtime_error(error),acc(acc){}
Account* AccountExceptionr::getAccount(){return acc;}
const char* AccountExceptionr::what()const throw(){return runtime_error::what();}