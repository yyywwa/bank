#include"../Header/account.h"

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