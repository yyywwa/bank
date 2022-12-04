#ifndef ACCOUNT_H 
#define	ACCOUNT_H
#include<iostream>
#include<cmath>
#include<map>
#include<stdexcept>
#include<stdexcept>
#include<string>
class Date {
private:
	int year, month, day;
	static Date temp;
public:
	static int monthDay[12];
	Date();
	Date(int, int, int);
	Date(const Date&);
	void show();
	bool leapYear(int)const;
	static inline void valuation(std::istream&, int&);
	int operator-(const Date&)const;
	void operator=(const Date&);
	bool ifLeap();
	int getYear();
	int getMonth();
	int getDay();
	int getMaxDay();
	static Date& read(std::istream&);
	bool friend operator <(const Date&, const Date&);
	bool friend operator==(const Date&, const Date&);
	friend std::ostream& operator<<(std::ostream& out, const Date& A);
};




class Accumulator {
private:
	Date lastDate;
	double value = 0;
	double sum = 0;
public:
	Accumulator();
	Accumulator(const Date& date, double value);
	double const getSum();
	void change(const Date& date, double value);
	void rezero();
	bool reset(const Date& date, double value);
};




class Account;
class AccountRecord {
private:
	Date date;
	const Account* account;
	double amount;
	double balance;
	std::string desc;
public:
	AccountRecord(const Account*, const Date&, double, double, std::string);
	AccountRecord(const AccountRecord&);
	void show();
};




class Account {
private:
	static std::multimap<Date, AccountRecord> recordMap;
	std::string id;
	std::string passwd;
	double balance;
	static double total;
protected:
	Account();
	Account(std::string id);
	void record(const Date& date, double amount, std::string desc);
	virtual const void error(std::string msg) = 0;
public:
	virtual	void deposit(const Date& date, double amount, std::string desc) = 0;
	virtual	void withdraw(const Date& date, double amount, std::string desc) = 0;
	virtual void settle(Date& date) = 0;
	void round(double& amount);
	double roundT(double);
	virtual const void show() = 0;
	void setId(std::string id);
	const std::string getId()const;
	static void query(const Date&, const Date&);
	const double getBalance();
	static double getTotal();
};




class CreditAccount :public Account {
private:
	Accumulator acc;
	double credit, rate, fee;
public:
	CreditAccount(const Date& date, std::string id, double credit, double rate, double fee);
	const double getDebt();
	double reDebt(const Date&);
	void reCharge(const Date&, double amount, std::string& desc);
	virtual	void deposit(const Date& date, double amount, std::string desc);
	virtual void withdraw(const Date& date, double amount, std::string desc);
	virtual	void settle(Date& date);
	virtual const void error(std::string msg);
	virtual const void show();
	const double getCredit();
	const double getRate();
	const double getFee();
	const double getAvailableCredit();
};
class SavingsAccount :public Account {
private:
	Accumulator acc;
	double rate = 0;
public:
	SavingsAccount(const Date& date, std::string id, double rate);
	virtual void deposit(const Date& date, double amount, std::string desc);
	virtual void withdraw(const Date& date, double amount, std::string desc);
	virtual void settle(Date& date);
	virtual const void error(std::string msg) override;
	virtual const void show() override;
	const double getRate();
};

class AccountExceptionr:public std::runtime_error{
private:
	Account* acc;
public:
	AccountExceptionr(const char* error,Account* acc);
	Account* getAccount();
	const char* what() const throw();
};
#endif