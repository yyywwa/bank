#include"../Come_true/veclist.cpp"
#include<iostream>
#include<sstream>
#include<fstream>
#include<stdexcept>
#include<string>
#include<cmath>
class date{
private:
	int year, month, day;
	static date temp;
public:
	static int monthDay[12];
	date();
	date(int, int, int);
	date(const date&);
	void show();
	bool leapYear(int)const;
	int operator-(const date&)const;
	void operator=(const date&);
	bool ifLeap();
	int getYear();
	int getMonth();
	int getDay();
	int getMaxDay();
	void changeDay(int outDay);
	bool friend operator <(const date&, const date&);
	bool friend operator==(const date&, const date&);
	friend std::ostream& operator<<(std::ostream& out, const date& a);
	friend void dateSwap(date& a,date& b);
};

class accumulator{
private:
	date _lastdate;
	double _value = 0;
	double _sum = 0;
public:
	accumulator();
	accumulator(const date& outDate, double outValue);
	double const getSum();
	void change(const date& outDate, double outValue);
	void clearSum();
	bool reset(const date& outDate, double outValue);
};

//desc -- description
class account{
private:  
    std::string _id;
    double _balance;
	double _rate;
    static double _total;
public:
    account(const std::string& outId,double outRate);
	virtual void deposit(const date& outDate, double amount) = 0;
	virtual void withdraw(const date& outDate,double amount) = 0;
	virtual void settle(date& outDate) = 0;
	virtual void show() = 0;
	void add(double amount);
	double round(double amount);
	std::string getId()const;
	const double getRate()const;
	const double getBalance()const;
	static const double getTotal();
};

class dateBill{
private:
	date _date;
	veclist<std::string> _desc;
public:
	dateBill();
	dateBill(const date& outDate,const std::string& outDesc);
	void pushDesc(const std::string& outDesc);
	bool operator==(const dateBill& _dateBill1);
	bool operator==(const date& outDate);
	void show();
	const date& getDate();
	veclist<std::string>& getDesc();
};

class bill{	
private:
	account* _master = nullptr;
	veclist<dateBill>  _dateBill;
public:
	bill();
	bill(account* outMaster);
	void pushBill(const date& outDate,const std::string& outDesc);
	void show(veclist<dateBill>::size_type it);
	void show();
	veclist<dateBill>& getDateBill();
	veclist<dateBill>::size_type size();
	const date& getDate(veclist<dateBill>::size_type it);
};

class creditAccount:public account{
private:
	accumulator _amr;
	double _credit,_fee;
public:
	creditAccount(const date& outDate,const std::string& outId,double outCredit,double outRate,double outFee);
	void reAccount(const date& outDate);
	virtual void deposit(const date& outDate,double amount);
	virtual void withdraw(const date& outDate,double amount);
	virtual void settle(date& outDate);
	virtual void show();
	const double getAvailableCredit();
};

class savingAccount:public account{
private:
	accumulator _amr;
public:
	savingAccount(const date& outDate, std::string& outId, double outRate);
	virtual void deposit(const date& outDate, double amount);
	virtual void withdraw(const date& outDate, double amount);
	virtual void settle(date& outDate);
	virtual void show();
};

class accountException:public std::runtime_error{
private:
	account* _master;//*出异常的对象
public:
	accountException(const char* error,account* outAcc);
	account* getAcc();
	const char* what()const throw();
	void show();
};

class myFile{	
private:
	std::ifstream inf;
	std::ofstream outf;
public:
	myFile(const std::string& fileName);
	~myFile();
	void writeFile(const std::string& lineCmd);
	std::ifstream& getInf();
	std::ofstream& getOutf();
	bool is_open_read();
	bool is_open_write();
};

class cmdFile:public myFile{
public:
	cmdFile(const std::string& fileName);
	void readCmdFile(bool (*fuction) (std::istream&));
};

class recordCmd{
private:
	std::stringbuf _buf;
	char _buffer[50];
	std::string _record;
	std::istream _itm;//* itm --istream 缩写
public:
	recordCmd();
	void readCmd(std::istream& in);
	void clearBuff();
	void clearIstream();
	void record();
	std::istream& getIstream();
	std::string& getRecord();
	bool ifEnd();
};