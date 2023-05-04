# include "TheServer.h"

int main(){
    SOCKET ListenSocket;
    if(InitAndListen(ListenSocket)==0){return 0;}
    cout<<"Listen socket is "<<ListenSocket<<endl;

    // ���ó�ʱ�ȴ�ʱ��
    timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    // ���׽��ַ����ļ������������Ա������select�Ȳ���
    fd_set fdSocket;
    FD_ZERO(&fdSocket);
    FD_SET(ListenSocket, &fdSocket);

    // ѭ�����տͻ����������󲢴��������߳�
    while(1){
		SOCKET *ClientSocket = new SOCKET;
		ClientSocket = (SOCKET*)malloc(sizeof(SOCKET));
        cout<<"new socket "<<*ClientSocket<<" created"<<endl;
        sockaddr_in ClientAddr;
        int SockAddrlen = sizeof(ClientAddr);
        /*
        SOCKET WSAAPI accept(
            [in]      SOCKET   s,           ��ʶ��ʹ��listen������������״̬���׽���
            [out]     sockaddr *addr,       ָ���������ʵ���ַ��ͨ�Ų�Ļ������Ŀ�ѡָ��
            [in, out] int      *addrlen     ָ�����addr����ָ��Ľṹ���ȵ������Ŀ�ѡָ��
        );
        accept�����������׽����ϳ��Դ������ӣ��ú������������ý���ֱ���µ����ӳ���
        ��δ����������accept������һ������ΪSOCKET��ֵ����ֵ�����׽��ֵ����������˷��ص�ֵ��ʵ�����ӵ��׽��ֵľ����
        */
        *ClientSocket = accept(ListenSocket, 0, 0);   // �Կͻ��˵ĵ�ַ�����Ȳ�����Ȥ������Ϊ0��NULL
        cout<<"���ӳɹ���"<<endl;
        // ����ʧ����������������򴴽����̷߳���ͻ���
        if(*ClientSocket==INVALID_SOCKET){
            cout<<"���ݽ���ʧ�ܣ�������룺"<<WSAGetLastError()<<endl;
            continue;
        }else{
            if(getpeername(*ClientSocket, (sockaddr *)&ClientAddr, &SockAddrlen)==-1){
                cout<<"getpeername error"<<endl;
                continue;
            }
            cout<<"һ��IPΪ"<<inet_ntoa(ClientAddr.sin_addr)<<"���˿�Ϊ"<<ntohs(ClientAddr.sin_port)<<"��socketΪ"<<*ClientSocket<<"�Ŀͻ��������ӵ�������"<<endl;
        }
        /*
        HANDLE CreateThread(
            [in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes,     ָ��SECURITY_ATTRIBUTES�ṹ��ָ�롣���˲���ΪNULL���޷��̳о��
            [in]            SIZE_T                  dwStackSize,            ���ֽ�Ϊ��λ�Ķ�ջ��ʼ��С�����˲���Ϊ�������߳̽�ʹ�ÿ�ִ���ļ���Ĭ�ϴ�С
            [in]            LPTHREAD_START_ROUTINE  lpStartAddress,         ָ�����߳�ִ�е�Ӧ�ó����庯����ָ��
            [in, optional]  __drv_aliasesMem LPVOID lpParameter,            ָ��Ҫ���ݸ��̵߳ı�����ָ��
            [in]            DWORD                   dwCreationFlags,        �����̴߳����ı�־�����˲���Ϊ���򴴽����̻߳���������
            [out, optional] LPDWORD                 lpThreadId              ָ������̱߳�ʶ���ı�����ָ�롣���˲���ΪNULL�򲻷����̱߳�ʶ��
        );
        */
        HANDLE hThread = CreateThread(NULL, 0, &ServerThread, (LPVOID)ClientSocket, 0, NULL);    // �����ɹ��򷵻����̵߳ľ�������򷵻�NULL
        if(hThread==NULL){cout<<"�̴߳���ʧ�ܣ�������룺"<<WSAGetLastError()<<endl;}
    }
    freesocket(ListenSocket);  // �ͷ�socket
    WSACleanup();   // �ر������
    return 0;
}

int InitAndListen(SOCKET &ListenSocket){
    // ��ʼ���׽��ֶ�̬��
    // ��������2.2�汾��winsock
    WORD sockVersion = MAKEWORD(2,2);   // MAKEWORD�ϲ�����byteΪһ��word���޷���16λ��������
    WSAData wsd;    // ���ڽ���Windows�׽���ʵ�ֵ���ϸ��Ϣ
    if(WSAStartup(sockVersion, &wsd)!=0){
        cout<<"����Winsock DLLʧ�ܣ��������..."<<endl;
        WSACleanup();   // �ر������
        return 0;
    }

    // �����׽���
    // AF_INET����IPv4��ַϵ�У�SOCK_STREAM��������ʽ��0�����ɷ����ṩ��Ϊ��ѡ����ʵ�Э������
    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);     // �����������׽��ֵ�������
    if(ListenSocket==INVALID_SOCKET){
        cout<<"�׽��ִ���ʧ�ܣ�������룺"<<WSAGetLastError()<<endl;
        WSACleanup();   // �ر������
        return 0;
    }

    // ���ü�����IP�Ͷ˿ڵ�
    sockaddr_in ListenAddr;
    memset(&ListenAddr, 0, sizeof(ListenAddr));
    ListenAddr.sin_family = AF_INET;            // IPv4��ַ��
    ListenAddr.sin_addr.s_addr = INADDR_ANY;    // ���󶨱�������ĳ���ض�ip
    ListenAddr.sin_port = htons(PORT);          // ��ϵͳ�������δʹ�õĶ˿�

    // ��socket�󶨾����ַ����λ�����ԣ���˿ںţ���λ������Ӧ�ã�
    /*
    int WSAAPI bind(
        [in] SOCKET         s,      ��ʶδ���׽��ֵ�������
        [in] const sockaddr *name,  ָ��Ҫ��������׽��ֵı��ص�ַ��sockaddr�ṹ��ָ��
        [in] int            namelen ���Ʋ���ָ���ֵ�ĳ��ȣ����ֽ�Ϊ��λ��
    );
    bind���������ص�ַ���׽������������δ���������򷵻��㡣���򷵻�SOCKET_ERROR
    */
    if(bind(ListenSocket, (LPSOCKADDR)&ListenAddr, sizeof(ListenAddr))==SOCKET_ERROR){
        /*
        bind�Ƿ���˵��õĺ������󶨵��Ƿ���˵�IP
        ���������ж�������������ж��IP
        ��ʱ��ѡ����bind������һ��IP��������ֻ���ո�IP��ĳ�˿ڵ�����
        ��bind�󶨵���INADDR_ANY�������з��͵�������������˿ڵ������ĸ�IP���յ������ݶ����������˽��̴���
        */
        cout<<"�׽��ְ󶨶˿�ʧ�ܣ�������룺"<<WSAGetLastError()<<endl;
        freesocket(ListenSocket);
        WSACleanup();   // �ر������
        return 0;
    }else{cout<<"�˿�"<<PORT<<"�󶨳ɹ���"<<endl;}

    // ��ʼ������SOMAXCONN������ϵͳ�������ʵĹ������Ӷ�����󳤶�
    /*
    int WSAAPI listen(
        [in] SOCKET s,          ��ʶ�󶨵�δ�����׽��ֵ�������
        [in] int    backlog     �������Ӷ��е���󳤶�
    );
    listen�������׽������������������ӵ�״̬
    */
    if(listen(ListenSocket, SOMAXCONN)==SOCKET_ERROR){
        cout<<"����ʧ�ܣ�������룺"<<WSAGetLastError()<<endl;
        freesocket(ListenSocket);  // �ͷ�socket
        WSACleanup();   // �ر������
        return 0;
    }
    cout<<"�����׼���������ȴ���������..."<<endl;
    return 1;
}

void freesocket(SOCKET &Socket){
    if(Socket!=INVALID_SOCKET){
        cout<<"�ر��׽���"<<Socket<<endl;
        closesocket(Socket);
        Socket = INVALID_SOCKET;
    }
    return ;
}

void Send(SOCKET &ClientSocket, string &temp){
    cout<<"��ͻ���"<<ClientSocket<<"���ͳ���Ϊ"<<strlen(temp.c_str())<<"����Ϣ��"<<endl;
    cout<<temp.c_str()<<endl;
    send(ClientSocket, temp.c_str(), strlen(temp.c_str()), 0);
    return ;
}

int Recv(SOCKET &ClientSocket, char *RecvBuf, int &recvByte){
    recvByte = 0;
    while(1){
        memset(RecvBuf, 0, MaxBufSize);    // ���û�����
        recvByte = recv(ClientSocket, RecvBuf, MaxBufSize, 0);
        cout<<"���յ����Կͻ���"<<ClientSocket<<"�ĳ���Ϊ"<<recvByte<<"����Ϣ��"<<endl;
        cout<<RecvBuf<<endl;
        if(recvByte==-1){cout<<"recvʧ�ܣ�������룺"<<WSAGetLastError()<<endl;}
        if(recvByte>0){return recvByte;}
    }
}

int isIFE(ifstream &infile){
    streampos fsize;

    // �����ļ���ȡʧ�����˳�
    if(!infile){
        cout<<"�����������ļ���ȡʧ�ܣ������ļ��Ƿ���ȷ����������"<<endl;
        // �ļ���ʧ��ʱ����clear��close���޷�����ifstream�Ĵ�����״̬
        infile.close();
        infile.clear();     // �������״̬
        return -1;
    }
    infile.seekg(0, ios::end);
    fsize = infile.tellg();
    if(fsize){return 1;}
    return 0;
}

void SendWords(SOCKET &ClientSocket){
    char RecvBuf[MaxBufSize];
    string temp;
    memset(RecvBuf, 0, MaxBufSize);
    ifstream infile("./Datas/Words.dat", ios::binary|ios::app);
    int flag = isIFE(infile);
    if(flag==-1){
        temp = "File Error";
        Send(ClientSocket, temp);
    }else if(flag==0){
        temp = "File Empty";
        Send(ClientSocket, temp);
        infile.close();
    }else{
        int count = 0;
        infile.seekg(0, ios::beg);
        while(1){
            infile.read(RecvBuf, MaxBufSize);
            send(ClientSocket, RecvBuf, infile.gcount(), 0);    // gcount()������һ��δ����ʽ�������������ȡ���ַ�����
            cout<<"RecvBuf is "<<RecvBuf<<" and recvByte is "<<infile.gcount()<<endl;
            if(infile.gcount()==0){
                send(ClientSocket, "Over", strlen("Over"), 0);
                break;
            }
            if(infile.gcount()<sizeof(RecvBuf)){break;}
            count += infile.gcount();
        }
        infile.close();
        cout << "Sended " << count << " bytes to client!" << endl;
    }
}

void RecvWords(SOCKET &SocketClient){
    int count = 0;
    int recvByte;
    char RecvBuf[MaxBufSize];
    memset(RecvBuf, 0, MaxBufSize);
    cout<<"Opening File"<<endl;
    ofstream outfile("./Datas/Words.dat", ios::binary|ios::trunc);
    while(1){
        recvByte = recv(SocketClient, RecvBuf, MaxBufSize, 0);
        outfile.write(RecvBuf, recvByte);
        cout<<"RecvBuf is "<<RecvBuf<<" and recvByte is "<<recvByte<<endl;
        if(RecvBuf=="Send Over"){break;}
        if(recvByte<sizeof(RecvBuf)){
            Send(SocketClient, (string)"Recv Over");
            break;
        }
        count += recvByte;
    }
    outfile.close();
    cout << "Received " << count << " bytes from server!" << endl;
}

string isInCSV(string &temp_name, string &temp_pwd, string path, int &flag){
    string name;
    string pwd;
    string line;
    ifstream infile(path, ios::in|ios::app);

    flag = isIFE(infile);
    // ���ļ��������ж�ȡ�����˻����ݱȽ��û���
    if(flag==-1){return "File Error";}
    if(flag>0){
        infile.seekg(0, ios::beg);
        while(getline(infile, line)){
            istringstream sin(line);    // �������ַ���line���뵽�ַ�����sin��
            getline(sin, name, ',');    // ���ַ�����sin�е�һ������ǰ���ַ�����name��
            getline(sin, name, ',');    // ���ַ�����sin�еڶ�������ǰ���ַ�����name��
            if(temp_name==name){
                getline(sin, pwd, ',');
                if(temp_pwd!=pwd){
                    infile.close();
                    flag = 0;
                    return "Password Error";
                }else{
                    temp_name = line;
                    infile.close();
                    flag = 1;
                    return line;
                }
                getline(sin, name);     // ���ַ�����sin��ʣ�ಿ���ַ�����temp��
            }
        }
    }
    infile.close();
    flag = 0;
    return "Name Error";
}

string isInCSV(string &temp_name, string path, int &flag){
    string line;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    flag = isIFE(infile);
    // ���ļ��������ж�ȡ�����˻����ݱȽ��û���
    if(flag==-1){return "File Error";}
    if(flag>0){
        infile.seekg(0, ios::beg);
        while(getline(infile, line)){
            istringstream sin(line);    // �������ַ���line���뵽�ַ�����sin��
            getline(sin, temp, ',');    // ���ַ�����sin�е�һ������ǰ���ַ�����temp��
            getline(sin, temp, ',');    // ���ַ�����sin�еڶ�������ǰ���ַ�����temp��
            if(temp_name==temp){
                infile.close();
                return "Fail";
            }
            getline(sin, temp);         // ���ַ�����sin��ʣ�ಿ���ַ�����temp��
        }
    }
    flag = 0;
    infile.close();
    return "Success";
}

void wUcsv(int CEflag, string temp_name, string temp_pwd, string path){
    // ���û�����д��users.csv
    ofstream outfile(path, ios::out|ios::app);
    outfile<<CEflag<<','
            <<temp_name<<','
            <<temp_pwd<<','
            <<0<<','
            <<1<<','
            <<0<<endl;
    outfile.close();
    return ;
}

int rcsvl(string path){
    streampos fsize;
    int R;
    ifstream infile(path, ios::in|ios::app);

    R = isIFE(infile);
    if(R>0){
        infile.seekg(-4, ios::end); // ��ǰ�������һ�еĻ��з�
        while(1){
            if(infile.peek()==infile.widen('\n')){
                infile.seekg(2, ios::cur);  // ������������ڶ��еĻ��з�
                break;
            }
            if(infile.tellg()==0){
                infile.seekg(0, ios::beg);  // ��Ҳ��֪��ΪʲôҪ����������Ŷ�Orz������tellg==0��
                break;
            }
            infile.seekg(-1, ios::cur);
        }
        infile>>R;
    }
    if(R>=0){infile.close();}
    return R;
}

void wRcsv(int num, string name, string path, int flag){
    ofstream outfile(path, ios::out|ios::app);
    outfile<<num<<','
            <<name<<','
            <<flag<<endl;
    outfile.close();
    return ;
}

string PAR(string path){
    int flag;
    string line;
    string temp;
    string out = "����\t�û�\t��ֵ\n";
    ifstream infile(path, ios::in|ios::app);

    flag = isIFE(infile);
    if(flag==-1){
        return "File Error";
    }else if(flag==0){
        return "�������������а�����ʱû�����ݣ���ע���ϰ�ɡ���������";
    }else{
        infile.seekg(0, ios::beg);
        while(getline(infile, line)){
            istringstream sin(line);
            getline(sin, temp, ',');
            out.append(temp);
            out.append("\t");
            getline(sin, temp, ',');
            out.append(temp);
            out.append("\t");
            getline(sin, temp);
            out.append(temp);
            out.append("\n");
        }
    }
    out.append("�������������а������ϣ�����������");
    infile.close();
    return out;
}

string getRank(string temp_name, string path){
    int flag;
    string line;
    string name;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    flag = isIFE(infile);
    if(flag==-1){
        return "-1";
    }else if(flag==0){
        infile.close();
        return "0";
    }else{
        infile.seekg(0, ios::beg);
        while(getline(infile, line)){
            istringstream sin(line);
            getline(sin, temp, ',');
            getline(sin, name, ',');
            if(temp_name==name){
                infile.close();
                return temp;
            }else{getline(sin, temp);}
        }
        infile.close();
        return "0";
    }
    
}

string check_user(string name){
    ifstream infile(user_path, ios::in);
    if(!infile){return "����������~~�û������ļ���ȡʧ�ܣ������ļ��Ƿ���ȷ~~����������";}
    string line;
    string CEflag;
    string temp;
    string out = "";
    while(getline(infile, line)){
        istringstream sin(line);
        getline(sin, CEflag, ',');
        getline(sin, temp, ',');
        if(temp==name){
            out.append("####################################\n");
            out.append("#  ����������ѯ�û����������\n");
            out.append("####################################\n");
            out.append("#  �ǳƣ�\t\t");
            out.append(name);
            out.append("\n");
            getline(sin, temp, ',');    // ��Ӧ����ʾ�����û�������
            getline(sin, temp, ',');
            out.append("#  ���飺\t\t");
            out.append(temp);
            out.append("\n");
            getline(sin, temp, ',');
            out.append("#  �ȼ���\t\t");
            out.append(temp);
            out.append("\n");
            getline(sin, temp);
            if(CEflag=="0"){
                out.append("#  ͨ���ؿ�����\t\t");
                out.append(temp);
                out.append("\n");
                out.append("#  �ȼ����а�������\t");
                out.append(getRank(name, "./Datas/CRL.csv"));
                out.append("\n");
                out.append("#  �������а�������\t");
                out.append(getRank(name, "./Datas/CRE.csv"));
                out.append("\n");
                out.append("#  �ؿ����а�������\t");
                out.append(getRank(name, "./Datas/CRP.csv"));
                out.append("\n");
            }else{
                out.append("#  �ۼƳ�������\t\t");
                out.append(temp);
                out.append("\n");
                out.append("#  �ȼ����а�������\t");
                out.append(getRank(name, "./Datas/ERL.csv"));
                out.append("\n");
                out.append("#  �������а�������\t");
                out.append(getRank(name, "./Datas/ERN.csv"));
                out.append("\n");
            }
            out.append("####################################");
            infile.close();
            return out;
        }else{getline(sin, temp);}
    }
    infile.close();
    return "����������~~����ѯ�û������ڣ���������~~����������";
}

string check_name(string new_name){
    int flag;
    string line;
    string name;
    string temp;
    ifstream infile(user_path, ios::in|ios::app);

    if(!infile){
        infile.close();
        infile.clear();
        return "-1";
    }
    infile.seekg(0, ios::beg);
    while(getline(infile, line)){
        istringstream sin(line);
        getline(sin, temp, ',');
        getline(sin, name, ',');
        if(new_name==name){
            infile.close();
            return "0";
        }
        getline(sin, temp);
    }
    infile.close();
    return "1";
}

void ModifyUCSV(int &flag, string name, string &str){
    string outstr = "";
    string temp_name;
    string temp;
    string line;
    string CEflag;
    ifstream infile(user_path, ios::in);
    while(getline(infile, line)){
        istringstream sin(line);
        getline(sin, CEflag, ',');
        getline(sin, temp_name, ',');
        if(temp_name==name){
            outstr += CEflag+",";
            if(flag){
                outstr += name+",";
                getline(sin, temp, ',');
                outstr += str+",";
            }else{
                outstr += str+",";
            }
            getline(sin, temp);
            outstr += temp+"\n";
        }else{outstr += line+"\n";}
    }
    infile.close();
    ofstream outfile(user_path, ios::out|ios::trunc);
    outfile.write(outstr.c_str(), outstr.size());
    outfile.close();
    return ;
}

void ModifyRCSV(string name, string &str, string path){
    string outstr = "";
    string temp_name;
    string temp;
    string line;
    string rank;
    ifstream infile(path, ios::in);
    while(getline(infile, line)){
        istringstream sin(line);
        getline(sin, rank, ',');
        getline(sin, temp_name, ',');
        if(temp_name==name){
            outstr += rank+",";
            outstr += str+",";
            getline(sin, temp);
            outstr += temp+"\n";
        }else{outstr += line+"\n";}
    }
    infile.close();
    ofstream outfile(path, ios::out|ios::trunc);
    outfile.write(outstr.c_str(), outstr.size());
    outfile.close();
    return ;
}

void Update(string &temp){
    UpdateUser(temp);
    string CEflag;
    string name;
    string pwd;
    istringstream sin(temp);
    getline(sin, CEflag, ',');
    getline(sin, name, ',');
    getline(sin, pwd, ',');
    if(CEflag=="1"){
        getline(sin, temp, ',');
        getline(sin, temp, ',');
        UpdateRank(atoi(temp.c_str()), name, "./Datas/ERL.csv");
        getline(sin, temp);
        UpdateRank(atoi(temp.c_str()), name, "./Datas/ERN.csv");
    }else{
        getline(sin, temp, ',');
        UpdateRank(atoi(temp.c_str()), name, "./Datas/CRE.csv");
        getline(sin, temp, ',');
        UpdateRank(atoi(temp.c_str()), name, "./Datas/CRL.csv");
        getline(sin, temp);
        UpdateRank(atoi(temp.c_str()), name, "./Datas/CRP.csv");
    }
}

void UpdateUser(string &temp){
    string CEflag;
    string name;
    string temp_name;
    string line;
    string outstr = "";
    istringstream sin(temp);
    getline(sin, CEflag, ',');
    getline(sin, name, ',');
    getline(sin, temp_name);
    ifstream infile(user_path, ios::in);
    while(getline(infile, line)){
        istringstream sin(line);
        getline(sin, CEflag, ',');
        getline(sin, temp_name, ',');
        if(temp_name==name){
            outstr += temp+"\n";
            getline(sin, temp_name);
        }else{outstr += line+"\n";}
    }
    infile.close();
    ofstream outfile(user_path, ios::out|ios::trunc);
    outfile.write(outstr.c_str(), outstr.size());
    outfile.close();
    return ;
}

void UpdateRank(int new_num, string name, string path){
    string temp;
    int tempn;
    int num;
    string line;
    vector<string> names;
    vector<int> values;
    // �ļ����ݶ���vector
    ifstream infile(path, ios::in);
    while(getline(infile, line)){
        istringstream sin(line);
        getline(sin, temp, ',');
        getline(sin, temp, ',');
        names.push_back(temp);
        getline(sin, temp);
        tempn = atoi(temp.c_str());
        values.push_back(tempn);
    }
    // �����û�����
    num = 0;
    while(names[num]!=name){num++;}
    values[num] = new_num;
    // vectorð������
    for(; num>0; num--){
        if(values[num-1]>values[num]){
            break;
        }else{
            tempn = values[num];
            values[num] = values[num-1];
            values[num-1] = tempn;
            temp = names[num];
            names[num] = names[num-1];
            names[num-1] = temp;
        }
    }
    infile.close();
    // vectorд���ļ�
    ofstream outfile(path, ios::out);
    for(num=0; num<values.size(); num++){
        outfile<<num+1<<','
                <<names[num]<<','
                <<values[num]<<endl;
    }
    outfile.close();
    return ;
}

DWORD WINAPI ServerThread(LPVOID lpParameter){
    SOCKET *ClientSocket = (SOCKET *)lpParameter;
    int last = 5;
	int recvByte;
    string temp;
    string request;
	char RecvBuf[MaxBufSize];
    while(1){
        // �ȴ��ͻ���������
        Recv(*ClientSocket, RecvBuf, recvByte);
        temp = RecvBuf;
        istringstream sin(temp);
        getline(sin, request, ',');
        if(request=="Login"){
            string temp_name;
            string temp_pwd;
            getline(sin, temp_name, ',');
            getline(sin, temp_pwd);
            cout<<"�û������ǳ�Ϊ"<<temp_name<<"���û���������Ϊ"<<temp_pwd<<endl;
            temp = isInCSV(temp_name, temp_pwd, user_path, recvByte);
            if(recvByte!=1){last--;}
            if(last==0){
                last = 5;
                temp = "Over Try";
            }
            Send(*ClientSocket, temp);
        }else if(request=="Register"){
            string temp_name;
            string temp_pwd;
            getline(sin, temp_name, ',');
            getline(sin, temp_pwd);
            cout<<"temp_name is "<<temp_name<<" and temp_pwd is "<<temp_pwd<<endl;
            temp = isInCSV(temp_name, user_path, recvByte);
            Send(*ClientSocket, temp);
            if(recvByte==0){
                while(1){
                    Recv(*ClientSocket, RecvBuf, recvByte);
                    temp = RecvBuf;
                    if(temp=="0"){
                        int file_flag = rcsvl("./Datas/CRL.csv");
                        file_flag++;
                        wRcsv(file_flag, temp_name, "./Datas/CRL.csv", 1);
                        wRcsv(file_flag, temp_name, "./Datas/CRE.csv", 0);
                        wRcsv(file_flag, temp_name, "./Datas/CRP.csv", 0);
                        wUcsv(0, temp_name, temp_pwd, user_path);
                        temp = "Created";
                        Send(*ClientSocket, temp);
                        break;
                    }else if(temp=="1"){
                        int file_flag = rcsvl("./Datas/ERL.csv");
                        file_flag++;
                        wRcsv(file_flag, temp_name, "./Datas/ERL.csv", 1);
                        wRcsv(file_flag, temp_name, "./Datas/ERN.csv", 0);
                        wUcsv(1, temp_name, temp_pwd, user_path);
                        temp = "Created";
                        Send(*ClientSocket, temp);
                        break;
                    }else{cout<<"�����������仰��˵�����ݴ��������Orz"<<endl;}
                }
            }
        }else if(request=="RQ"){
            last = 5;
        }else if(request=="CN"){
            string new_name;
            string name;
            string CEflag;
            int flag = 0;
            getline(sin, CEflag, ',');
            getline(sin, name, ',');
            getline(sin, new_name);
            temp = check_name(new_name);
            Send(*ClientSocket, temp);
            if(temp=="1"){
                ModifyUCSV(flag, name, new_name);
                if(CEflag=="1"){
                    ModifyRCSV(name, new_name, "./Datas/ERN.csv");
                    ModifyRCSV(name, new_name, "./Datas/ERL.csv");
                }else{
                    ModifyRCSV(name, new_name, "./Datas/CRP.csv");
                    ModifyRCSV(name, new_name, "./Datas/CRL.csv");
                    ModifyRCSV(name, new_name, "./Datas/CRE.csv");
                }
            }
        }else if(request=="CP"){
            string name;
            string CEflag;
            int flag = 1;
            getline(sin, CEflag, ',');
            getline(sin, name, ',');
            getline(sin, temp);
            ModifyUCSV(flag, name, temp);
        }else if(request=="RW"){
            SendWords(*ClientSocket);
        }else if(request=="NW"){
            RecvWords(*ClientSocket);
        }else if(request=="Quit"){
            break;
        }else if(request=="Rank"){
            string path;
            string temp_name;
            getline(sin, path, ',');
            getline(sin, temp_name);
            temp = getRank(temp_name, path);
            Send(*ClientSocket, temp);
        }else if(request=="PAR"){
            string path;
            getline(sin, path);
            temp = PAR(path);
            Send(*ClientSocket, temp);
        }else if(request=="CU"){
            string name;
            getline(sin, name);
            temp = check_user(name);
            Send(*ClientSocket, temp);
        }else if(request=="Update"){
            getline(sin, temp);
            Update(temp);
        }else{
            cout<<"���յ���������"<<request<<endl;
            send(*ClientSocket, "Wrong Request", strlen("Wrong Request"), 0);
        }
    }
	freesocket(*ClientSocket);
    free(ClientSocket);
    return 0;
}
