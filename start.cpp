#include"./Header/account.h"
#define _DATE_SEPARATOR_ '/'
using namespace std;
char option;//(a)add account (d)deposit (w)withdraw (s)show 
            //(c)change day (n)next month (q)query (e)exit 
int day;
veclist<account*>::size_type idx;
double amount,credit,rate,fee;
string id,desc;//desc --Description行为描述
string lineCmd;//一整行命令
vector<account*> accounts;
veclist<bill> ac_bill;
veclist<accountException>ac_error;//错误
account* ac_ptr;
date Date(2008,11,1);


void ifMoreInput(istream& cmd){
    if(cmd.peek() != '\n')
        throw "To many input";
}
void ifLessInput(istream& cmd){
    if(cmd.peek() == '\n')
        throw "To lass input";
}
void record(istream& cmd,vector<bill>::size_type& outIdx,date& outDate,string& outDesc){
    cmd>>desc;
    ac_bill.at(outIdx).pushBill(outDate,outDesc);
    outDesc.clear();
}
void assigment(istream& cmd,double& a){//判断是否正确输入
    ifLessInput(cmd);
    if(!(cmd>>a))
        throw "Invalid value";
}
void assigment(istream& cmd,size_t& a){
    ifLessInput(cmd);
    if(!(cmd>>a))
        throw "Invalid value";
}
void assigment(istream& cmd,int& a){
    ifLessInput(cmd);
    if(!(cmd>>a))
        throw "Invalid value";
}
void addAccount(istream& cmd){
    cmd>>option>>id;
    if(option == 's'){
        assigment(cmd,rate);
        ac_ptr = new savingAccount(Date,id,rate);   
    }
    else if(option == 'c'){
        assigment(cmd,credit);
        assigment(cmd,rate);
        assigment(cmd,fee);
        ac_ptr = new creditAccount(Date,id,credit,rate,fee);
    }
    else{
        throw "Input error";
    }
    ifMoreInput(cmd);
    accounts.push_back(ac_ptr);
    ac_bill.push_back(new bill(ac_ptr));
    ac_bill.back().pushBill(Date,"crea");
}
void deposit(istream& cmd){
    assigment(cmd,idx);
    assigment(cmd,amount);
    ifLessInput(cmd);
    accounts.at(idx)->deposit(Date,amount);
    record(cmd,idx,Date,desc);
}
void withdraw(istream& cmd){
    assigment(cmd,idx);
    assigment(cmd,amount);
    ifLessInput(cmd);
    accounts.at(idx)->withdraw(Date,amount);
    record(cmd,idx,Date,desc);
}
void show(istream& cmd){
    ifMoreInput(cmd);
    for(auto i = 0;i < accounts.size();++i){
        cout<<"[" <<i<<"] ";
        accounts[i]->show();
        cout<<endl;
    }
}
void changeDay(istream& cmd){
    assigment(cmd,day);
    ifMoreInput(cmd);
    if(day < Date.getDay() || day > Date.getMaxDay())
        throw  "Invalid day";
    Date.changeDay(day);
}
void nextMonth(istream& cmd){
    ifMoreInput(cmd);
    if(Date.getMonth() == 12)
        Date = date(Date.getYear()+1,1,1);
    else
        Date = date(Date.getYear(),Date.getMonth()+1,1);
    for(int i = 0;i < accounts.size();++i)
        accounts[i]->settle(Date);

}
date readDate(istream& cmd){
    int year,month,day;
    assigment(cmd,year);
    cmd.ignore(100,_DATE_SEPARATOR_);
    assigment(cmd,month);
    cmd.ignore(100,_DATE_SEPARATOR_);
    assigment(cmd,day);
    return date(year,month,day);
}
void query(istream& cmd){
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
bool ExT(istream& cmd){
    ifMoreInput(cmd);
    return false;
}
bool executeCmd(istream& cmd){
    cmd>>option;
    switch(option){
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
    return true;
}

void readCmdFile(ifstream& inf){
    while(inf.peek()!= EOF){
        try{ 
            executeCmd(inf);
        }catch(const char* e){
            
        }
    }
}

void clearBuff(char* buff){
    for(int i = 0;i < 50;++i)
        *(buff+i) = '\0';
}

void copyBuff(istream& in,char* buff){
    in.getline(buff,50);
    for(int i = 0;i < 50;++i){
        if(buff[i] == '\0'){
            buff[i] = '\n';
            break;
        }
    }
}