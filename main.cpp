#include"./start.cpp"
int main(){
    ifstream inf;
    inf.open("./TXT/cmd.txt");
    readCmdFile(inf);
    inf.close();
    ofstream outf;
    outf.open("./TXT/cmd.txt",ofstream::app);

    stringbuf buf;
    istream recordIstream(&buf);
    char buff[50];

    bool com = true;
    while(com){
        try{
            cout<<Date<<" << ";
            copyBuff(cin,buff);
            buf.str(buff);
            com = executeCmd(recordIstream);
            if(buff[0] != 'e')
                lineCmd.append(buff);
            clearBuff(buff);
            cout<<"lineCmd= "<<lineCmd<<endl;
        }catch(const char* e){
            cout<<e<<endl;
        }
        recordIstream.clear();
        recordIstream.ignore(50,'\n');
    }
    outf<<lineCmd;
    outf.close();
    return 0;
}