#include "./bank.h"

#ifndef _START_CPP
#define _START_CPP

#define _DATE_SEPARATOR_ '/'
using namespace std;

char option;//*(a)add account (d)deposit (w)withdraw (s)show 
            //*(c)change day (n)next month (q)query (e)exit 
int g_day;
mystd::veclist<account*>::size_type idx;
double amount,credit,rate,fee;
string id,desc;//*desc --Description行为描述
mystd::vector<account*> accounts;
account* ac_ptr;
mystd::veclist<bill> ac_bill;
mystd::veclist<accountException>ac_error;//*错误
date g_Date(2008,11,1);


void ifMoreInput(istream& cmd) {
    if(cmd.peek() != '\n')
        throw "To many input";
}

void ifLessInput(istream& cmd) {
    if(cmd.peek() == '\n')
        throw "To lass input";
}

void recordDesc(istream& cmd,mystd::vector<bill>::size_type& outIdx,date& outDate,string& outDesc) {
    cmd>>desc;
    ac_bill.at(outIdx).pushBill(outDate,outDesc);
    outDesc.clear();
}

void assigment(istream& cmd,double& a) {//*判断是否正确输入
    ifLessInput(cmd);
    if(!(cmd>>a))
        throw "Invalid value";
}

void assigment(istream& cmd,size_t& a) {
    ifLessInput(cmd);
    if(!(cmd>>a))
        throw "Invalid value";
}

void assigment(istream& cmd,int& a) {
    ifLessInput(cmd);
    if(!(cmd>>a))
        throw "Invalid value";
}

void addAccount(istream& cmd) {
    cmd>>option>>id;
    if(option == 's') {
        assigment(cmd,rate);
        ac_ptr = new savingAccount(g_Date,id,rate);   
    }
    else if(option == 'c') {
        assigment(cmd,credit);
        assigment(cmd,rate);
        assigment(cmd,fee);
        ac_ptr = new creditAccount(g_Date,id,credit,rate,fee);
    }
    else{
        throw "Input error";
    }
    ifMoreInput(cmd);
    accounts.push_back(ac_ptr);
    ac_bill.push_back(new bill(ac_ptr));
    ac_bill.back().pushBill(g_Date,"create");
}

void deposit(istream& cmd) {
    assigment(cmd,idx);
    assigment(cmd,amount);
    ifLessInput(cmd);
    try {
        accounts.at(idx)->deposit(g_Date,amount);
    }catch(const char* e) {
        ac_error.push_back(new accountException(e,accounts.at(idx)));
        throw ac_error.back().what();
    }
    recordDesc(cmd,idx,g_Date,desc);
}

void withdraw(istream& cmd) {
    assigment(cmd,idx);
    assigment(cmd,amount);
    ifLessInput(cmd);
    try {
        accounts.at(idx)->withdraw(g_Date,amount);
    }catch(const char* e) {
        ac_error.push_back(new accountException(e,accounts.at(idx)));
        throw ac_error.back().what();
    }
    recordDesc(cmd,idx,g_Date,desc);
}

void show(istream& cmd) {
    ifMoreInput(cmd);
    for(auto i = 0;i < accounts.size();++i) {
        cout<<"[" <<i<<"] ";
        accounts[i]->show();
        cout<<endl;
    }
}

void changeDay(istream& cmd) {
    assigment(cmd,g_day);
    ifMoreInput(cmd);
    if(g_day < g_Date.getDay() || g_day > g_Date.getMaxDay())
        throw  "Invalid day";
    g_Date.changeDay(g_day);
}

void nextMonth(istream& cmd) {
    ifMoreInput(cmd);
    if(g_Date.getMonth() == 12)
        g_Date = date(g_Date.getYear()+1,1,1);
    else
        g_Date = date(g_Date.getYear(),g_Date.getMonth()+1,1);
    for(int i = 0;i < accounts.size();++i)
        accounts[i]->settle(g_Date);

}

date readDate(istream& cmd) {
    int year,month,day;
    assigment(cmd,year);
    cmd.ignore(100,_DATE_SEPARATOR_);
    assigment(cmd,month);
    cmd.ignore(100,_DATE_SEPARATOR_);
    assigment(cmd,day);
    return date(year,month,day);
}

void query(istream& cmd) {
    assigment(cmd,idx);
    date first = readDate(cmd);
    ifLessInput(cmd);
    date last = readDate(cmd);
    if(last < first)
        dateSwap(first,last);
    for(auto i = 0;i < ac_bill.at(idx).size();++i)
        if(ac_bill[idx].getDate(i) < last && first < ac_bill[idx].getDate(i))
            ac_bill[idx].show(i);
}

bool ExT(istream& cmd) {
    ifMoreInput(cmd);
    return false;
}

bool executeCmd(istream& cmd) {
    cmd>>option;
    switch(option) {
        case 'a':
            addAccount(cmd);
            break;
        case 'd':
            deposit(cmd);
            break;
        case 'w':
            withdraw(cmd);
            break;
        case 's':
            show(cmd);
            break;
        case 'c':
            changeDay(cmd);
            break;
        case 'n':
            nextMonth(cmd);
            break;
        case 'q':
            query(cmd);
            break;
        case 'e':
            return ExT(cmd);
        default:
            throw "Input error";
    }
    cmd.get();//* 调整cmd的指针位置，不然read cmd file 的时候会重复读取最后一个字符
    return true;
}

#endif