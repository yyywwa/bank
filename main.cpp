#include"./start.cpp"
int main(){
    stringbuf buf;
    istream recordIstream(&buf);
    char buff[50];

    myFile cmdFile("./TXT/cmd.txt");
    readCmdFile(cmdFile.getInf());

    bool com = true;
    while(com){
        try{
            cout<<Date<<" << ";
            clearBuff(buff);
            copyBuff(cin,buff);
            buf.str(buff);

            com = executeCmd(recordIstream);

            if(buff[0] == 'e')
                break;
            lineCmd.append(buff);
        }catch(const char* e){
            cout<<e<<endl;
            recordIstream.clear();
        }
    }

    for(auto it = accounts.begin();it!=accounts.end();++it)
        delete *it;
    cmdFile.writeFile(lineCmd);
    return 0;
}