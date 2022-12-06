#include"../Header/account.h"

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
