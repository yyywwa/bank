#include"../Header/account.h"

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
