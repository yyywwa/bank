#include"./veclist.cpp"
#include"./account.h"
#include<sstream>
#include<fstream>
#include<stdexcept>
#include<string>
using namespace std;
void getline(std::istream& in, std::string& desc) {
	desc.clear();
	char temp[20];
	in.getline(temp, 20);
	desc.append(temp);
}


inline void valuation(istream& in, double& a) { if (!(in >> a)) { throw "Invalid number"; } }




class Accexception :public runtime_error {
private:
	Account* acc;
public:
	Accexception(const char* err, Account* ACC) :runtime_error(err), acc(ACC) {}
	Account* getAcc() { return acc; }
	const char* what()const throw() { return runtime_error::what(); }
};




char type;
int idx, day;
double amount, credit, rate, fee;
string id, desc, record;
Date date1, date2;
Account* account;
Date date(2008, 11, 1);
vector<Account*> accounts;
veclist<Accexception> ac_error;
void start(char cmd, istream& in) {
	switch (cmd) {
	case 'a':
		in >> type >> id;
		if (type == 's') {
			valuation(in, rate);
			account = new SavingsAccount(date, id, rate);
		}
		else if (type == 'c') {
			in >> credit >> rate >> fee;
			account = new CreditAccount(date, id, credit, rate, fee);
		}
		else {
			throw "Input error";
		}
		accounts.push_back(account);
		break;
	case 'd':
		in >> idx >> amount;
		getline(in, desc);
		accounts[idx]->deposit(date, amount, desc);
		break;
	case 'w':
		in >> idx >> amount;
		getline(in, desc);
		if (idx >= accounts.size()) {
			throw "NO user";
		}
		try {
			accounts[idx]->withdraw(date, amount, desc);
		}
		catch (const char* e) {
            Accexception* a_e= new Accexception(e,accounts[idx]);
			ac_error.push_back(a_e);
			throw ac_error.back();
		}
		break;
	case 's':
		if (in >> cmd) {
			throw "To many input";
		}
		for (int i = 0; i < accounts.size(); i++) {
			cout << "[" << i << "] ";
			accounts[i]->show();
			cout << endl;
		}
		break;
	case 'c':
		in >> day;
		if (day < date.getDay())
			throw  "You cannot specify a previous day";
		else if (day > date.getMaxDay()) {
			throw  "Invalid day";
		}
		else
			date = Date(date.getYear(), date.getMonth(), day);
		break;
	case 'n':
		if (in >> cmd) {
			throw "TO many input";
		}
		if (date.getMonth() == 12)
			date = Date(date.getYear() + 1, 1, 1);
		else
			date = Date(date.getYear(), date.getMonth() + 1, 1);
		for (int i = 0; i < accounts.size(); ++i) {
			accounts[i]->settle(date);
		}
		break;
	case 'q':
		date1 = Date::read(in);
		date2 = Date::read(in);
		Account::query(date1, date2);
		break;
	default:
		throw "Input error";
	}
}

void read(char& cmd, istream& inf) {
	while (inf >> cmd) {
		try {
			start(cmd, inf);
		}
		catch (const char* e) {
		}
	}
}
int main() {
	ifstream inf;
	ofstream outf;
	inf.open("./cmd.txt");
	outf.open("./cmd.txt", ofstream::app);
	char cmd;
	string record;
	read(cmd, inf);
	stringbuf buf;
	istream in(&buf);
	char buff[50];
	cout << "(a)add account (d)deposit (w)withdraw (s)show (c)change day (n)next month (q)query (e)exit" << endl;
	do {
		date.show();
		cout << "\tTotal: " << Account::getTotal() << "\tcommand> ";
		cin.getline(buff, 50);
		buf.str(buff);
		in.get(cmd);
		if (cmd == 'e')
			break;
		try {
			start(cmd, in);
			record.append(buff);
			record.push_back('\n');
			outf << record;
		}
		catch (const char* e) {
			cout << e << endl;
		}
		catch (Accexception& ac_error) {
			ac_error.getAcc()->show();
			cout << endl;
			cerr << ac_error.what() << endl;
		}
		record.clear();
		in.clear();
	} while (cmd != 'e');
	for (int i = 0; i < accounts.size(); ++i) {
		delete accounts[i];
	}
	inf.close();
	outf.close();
	return 0;
}