#include"./start.cpp"
int main(){
    recordCmd record_cmd;

    cmdFile cmd_file("./TXT/cmd.txt");

    if(cmd_file.is_open_read())
        cmd_file.readCmdFile(executeCmd);
    else
        cout<<"Can't find cmd file,will create a new cmd file"<<endl;

    bool com = true;
    istream& _inf = record_cmd.getIstream();
    while(com){
        cout<<g_Date<<" << ";

        record_cmd.clearBuff();
        record_cmd.readCmd(cin);

        try{
            com = executeCmd(_inf);

            if(!record_cmd.ifEnd())
                record_cmd.record();//*如果没有异常且没有终止将命令保存
        }catch(const char* e){
            cout<<e<<endl;
            record_cmd.clearIstream();
        }
    }

    cmd_file.writeFile(record_cmd.getRecord());

    std::cout<<std::endl;
    for(auto it = ac_bill.begin();it!=ac_bill.end();++it)
        ac_bill[it].show();//* 显示所有用户账单
    std::cout<<std::endl;

    std::cout<<std::endl;
    for(auto it = ac_error.begin();it != ac_error.end();++it)
        ac_error[it].show();//* 显示所有用户取款和存款错误
    std::cout<<std::endl;

    for(auto it = accounts.begin();it!=accounts.end();++it)
        delete *it;//*手动释放new出来的内存

    return 0;
}