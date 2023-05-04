# include "TheClient.h"

int main(){
    SOCKET SocketClient;
    InitSocket(SocketClient);
    
    // ���ó�ʱ�ȴ�ʱ��
    timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    // ���׽��ַ����ļ������������Ա������select�Ȳ���
    fd_set fdSocket;
    FD_ZERO(&fdSocket);
    FD_SET(SocketClient, &fdSocket);

    char info[1024];
    char RecvBuf[MaxBufSize];
    int recvByte;
    int flag;

    while(1){
        flag = MainMenu();
        if(flag==-1){
            send(SocketClient, "Quit,0,0", strlen("Quit,0,0"), 0);
            cout<<"�����������������˳����ڴ��´��ټ�����������"<<endl;
            break;
        }else if(flag==-2){
            cout<<"����������~~��������Ч����~~����������"<<endl;
            continue;
        }else{
            /*
            int WSAAPI select(
                [in]      int           nfds,           �ɺ��ԡ�������nfds�����Ա���Berkeley�׽��ּ���
                [in, out] fd_set        *readfds,       ָ��һ��Ҫ�����׽����Ƿ�ɶ��Ŀ�ѡָ��
                [in, out] fd_set        *writefds,      ָ��һ��Ҫ�����׽����Ƿ��д�Ŀ�ѡָ��
                [in, out] fd_set        *exceptfds,     ָ��һ��Ҫ��������׽��ֵĿ�ѡָ��
                [in]      const timeval *timeout        ѡ��ȴ������ʱ�䣬��TIMEVAL�ṹ����ʽ�ṩ��ֵΪNULL����ֹ����
            );
            */
            if(select(0, &fdSocket, &fdSocket, NULL, &timeout)==0){
                cout<<"��������æ�����Ժ�����..."<<endl;
                continue;
            }
            if(flag==0){
                send(SocketClient, "RW,0,0", strlen("RW,0,0"), 0);
                RecvWords(SocketClient);
                User *user = new User();
                Login(SocketClient, user);
                if(user!=NULL){Game(SocketClient, user);}
            }else if(flag==1){
                send(SocketClient, "RW,0,0", strlen("RW,0,0"), 0);
                RecvWords(SocketClient);
                User *user = new User();
                Register(SocketClient, user);
                if(user!=NULL){Game(SocketClient, user);}
            }else if(flag==2){
                Rank(SocketClient);
            }else{cout<<"�Ҷ���֪��������ô�ܿ�����仰��Orz"<<endl;}
        }
    }
    freesocket(SocketClient);
    WSACleanup();
    return 0;
}

int InitSocket(SOCKET &SocketClient){
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
    SocketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    // �����������׽��ֵ�������
    if(SocketClient==INVALID_SOCKET){
        cout<<"�׽��ִ���ʧ�ܣ�������룺"<<WSAGetLastError()<<endl;
        WSACleanup();   // �ر������
        return 0;
    }

    // ����Ҫ���ӵķ���˵�IP�Ͷ˿ڵ�
    sockaddr_in SeverAddr;
    memset(&SeverAddr, 0, sizeof(SeverAddr));
    SeverAddr.sin_family = AF_INET;                     // IPv4��ַ��
    /*
    ����Ǿ������µĲ�ͬ�������socketͨ������ָ������˵�IP
    cout<<"������Ҫ���ӵķ����IP�ţ�"<<endl;
    string SeverIP;
    cin>>ServerIP;
    SeverAddr.sin_addr.s_addr = inet_addr(SeverIP.c_str());
    */
    SeverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // �󶨱��������ip��ַ
    // �˿����������Ǵ�˶�������С��
    SeverAddr.sin_port = htons(PORT);                   // ��Ϊϵͳδʹ�õĶ˿�
    cout<<SeverAddr.sin_addr.s_addr<<endl;

    // ��ʼ���ӣ�SOMAXCONN������ϵͳ�������ʵĹ������Ӷ�����󳤶�
    if(connect(SocketClient, (sockaddr*)&SeverAddr, sizeof(SeverAddr))==SOCKET_ERROR){
        cout<<"����ʧ�ܣ�������룺"<<WSAGetLastError()<<endl;
        freesocket(SocketClient);  // �ͷ�socket
        WSACleanup();   // �ر������
        return 0;
    }
    cout<<"��������ӳɹ�..."<<endl;
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

int Recv(SOCKET &SocketClient, char *RecvBuf, int &recvByte){
    recvByte = 0;
    while(1){
        // memset(void *str, int c, size_t n)��strָ����ַ�����ǰn���ַ��滻Ϊc
        memset(RecvBuf, 0, MaxBufSize);
        recvByte = recv(SocketClient, RecvBuf, MaxBufSize, 0);
        if(recvByte==-1){cout<<"recvʧ�ܣ�������룺"<<WSAGetLastError()<<endl;}
        if(recvByte>0){return recvByte;}
    }
}

int MainMenu(){
    string temp_flag;
    int flag;
    cout<<"************************************"<<endl;
    cout<<"*  �𾴵��û����ã�������Ҫ��ʲô��"<<endl;
    cout<<"************************************"<<endl;
    cout<<"*  ����0ѡ���¼"<<endl;
    cout<<"*  ����1ѡ��ע��"<<endl;
    cout<<"*  ����2�鿴���а�"<<endl;
    cout<<"*  ����-1�˳�����"<<endl;
    cout<<"************************************"<<endl;
    cin>>temp_flag;
    // ����������ջ�Ժ����getline()���Ӱ��
    // cin.sync()ʵ��Ч����������йعʲ�ʹ��
    cin.ignore(MAXSIZE, '\n');

    if((temp_flag=="-1")||(temp_flag=="0")||(temp_flag=="1")||(temp_flag=="2")){
        flag = atoi(temp_flag.c_str());
        return flag;
    }else{return -2;}
}

void SendWords(SOCKET &ClientSocket){
    char RecvBuf[MaxBufSize];
    memset(RecvBuf, 0, MaxBufSize);
    string temp;
    send(ClientSocket, "NW,0,0", strlen("NW,0,0"), 0);
    ifstream infile("./Datas/Words.dat", ios::binary|ios::app);
    int count = 0;
    int recvByte;
    infile.seekg(0, ios::beg);
    while(1){
        infile.read(RecvBuf, MaxBufSize);
        send(ClientSocket, RecvBuf, infile.gcount(), 0);    // gcount()������һ��δ����ʽ�������������ȡ���ַ�����
        if(infile.gcount()==0){
            send(ClientSocket, "Send Over", strlen("Send Over"), 0);
            break;
        }
        if(infile.gcount()<sizeof(RecvBuf)){
            // Ϊ���������Update()�����һ�����뻺�������䣬����ȴ�һ�η���˵Ľ��ս����ź�
            Recv(ClientSocket, RecvBuf, recvByte);
            break;
        }
        count += infile.gcount();
    }
    infile.close();
    return ;
}

void RecvWords(SOCKET &SocketClient){
    int count = 0;
    int recvByte;
    char RecvBuf[MaxBufSize];
    memset(RecvBuf, 0, MaxBufSize);
    ofstream outfile("./Datas/Words.dat", ios::binary|ios::trunc);
    while(1){
        recvByte = recv(SocketClient, RecvBuf, MaxBufSize, 0);
        outfile.write(RecvBuf, recvByte);
        if(recvByte<sizeof(RecvBuf)){break;}
        if(RecvBuf=="Over"){break;}
        count += recvByte;
    }
    outfile.close();
    return ;
}

void Game(SOCKET &SocketClient, User *&user){
    int flag;
    int recvByte;
    char RecvBuf[MaxBufSize];
    string temp;
    string name;
    while(1){
        flag = welcomeUser(user);
        if(flag==-1){
            Update(SocketClient, user);
            delete user;
            user = NULL;
            cout<<"�������������ѳɹ��˳���¼�����ڷ��������桪��������"<<endl;
            system("pause");
            break;
        }else if(flag==0){
            name = user->get_name();
            if(user->get_CEflag()){
                // ���ó����ߵȼ�����
                temp = "Rank,";
                temp.append("./Datas/ERL.csv");
                temp.append(",");
                temp.append(name);
                send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
                Recv(SocketClient, RecvBuf, recvByte);
                user->set_ERL(atoi(RecvBuf));
                // ���ó����߳���������
                temp = "Rank,";
                temp.append("./Datas/ERN.csv");
                temp.append(",");
                temp.append(name);
                send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
                Recv(SocketClient, RecvBuf, recvByte);
                user->set_ERN(atoi(RecvBuf));
                user->ShowValue();
            }else{
                // ������ս�ߵȼ�����
                temp = "Rank,";
                temp.append("./Datas/CRL.csv");
                temp.append(",");
                temp.append(name);
                send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
                Recv(SocketClient, RecvBuf, recvByte);
                user->set_CRL(atoi(RecvBuf));
                // ������ս�߾�������
                temp = "Rank,";
                temp.append("./Datas/CRE.csv");
                temp.append(",");
                temp.append(name);
                send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
                Recv(SocketClient, RecvBuf, recvByte);
                user->set_CRE(atoi(RecvBuf));
                // ������ս��ͨ��������
                temp = "Rank,";
                temp.append("./Datas/CRP.csv");
                temp.append(",");
                temp.append(name);
                send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
                Recv(SocketClient, RecvBuf, recvByte);
                user->set_CRP(atoi(RecvBuf));
                user->ShowValue();
            }
        }else if(flag==1){
            cout<<"������������������������������������"<<endl;
            cout<<"������Ҫ��ѯ���û����ǳ�"<<endl;
            cout<<"������������������������������������"<<endl;
            cin.ignore(MAXSIZE, '\n');
            getline(cin, name);
            temp = "CU,";
            temp.append(name);
            send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
            Recv(SocketClient, RecvBuf, recvByte);
            temp = RecvBuf;
            cout<<temp<<endl;
            system("pause");
        }else if(flag==2){
            SetValue(SocketClient, user);
        }else{
            user->Play();
            if(user->get_CEflag()){SendWords(SocketClient);}
            Update(SocketClient, user);
        }
    }
    return ;
}

void Update(SOCKET &SocketClient, User *&user){
    string temp = "Update,";
    temp.append(to_string(user->get_CEflag()));
    temp.append(",");
    temp.append(user->get_name());
    temp.append(",");
    temp.append(user->get_pwd());
    temp.append(",");
    temp.append(to_string(user->get_experience()));
    temp.append(",");
    temp.append(to_string(user->get_level()));
    temp.append(",");
    temp.append(to_string(user->get_nop()));
    cout<<"sending temp: "<<temp<<endl;
    send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
    return ;
}

int welcomeUser(User *&user){
    int CEflag = user->get_CEflag();
    string temp;
    int flag = 0;
    if(CEflag){
        cout<<"************************************"<<endl;
        cout<<"��ӭ������������"
            <<user->get_name()
            <<"��"<<endl;
            while(1){
                cout<<"������Ҫ��ʲô��"<<endl;
                cout<<"************************************"<<endl;
                cout<<"*  ����-1����������"<<endl;
                cout<<"*  ����0�鿴��ǰ��������"<<endl;
                cout<<"*  ����1�鿴�����˵�����"<<endl;
                cout<<"*  ����2�޸��˻��ǳƺ�����"<<endl;
                cout<<"*  ����3��������"<<endl;
                cout<<"************************************"<<endl;
                // ֱ����flag��ȡ�����������벻Ϊ���ֻ�������Ӧ����ѭ��
                // �����atoiת��flag���ַ������Ϊ0
                cin>>temp;
                if((temp=="-1")||(temp=="0")||(temp=="1")||(temp=="2")||(temp=="3")){
                    flag = atoi(temp.c_str());
                    break;
                }else{cout<<"����������~~��������Ч����~~����������"<<endl;}
            }
    }else{
        cout<<"************************************"<<endl;
        cout<<"��ӭ��������ս��"
            <<user->get_name()
            <<"��"<<endl;
            while(1){
                cout<<"������Ҫ��ʲô��"<<endl;
                cout<<"************************************"<<endl;
                cout<<"*  ����-1����������"<<endl;
                cout<<"*  ����0�鿴��ǰ��������"<<endl;
                cout<<"*  ����1�鿴�����˵�����"<<endl;
                cout<<"*  ����2�޸��˻��ǳƺ�����"<<endl;
                cout<<"*  ����3��������"<<endl;
                cout<<"************************************"<<endl;
                cin>>temp;
                if((temp=="-1")||(temp=="0")||(temp=="1")||(temp=="2")||(temp=="3")){
                    flag = atoi(temp.c_str());
                    break;
                }else{cout<<"����������~~��������Ч����~~����������"<<endl;}
            }
    }
    return flag;
}

int LRequest(SOCKET &SocketClient, string &temp){
    string temp_name;
    string temp_pwd;
    string out = NULL;
    cout<<"������������������������������������"<<endl;
    cout<<"�˻��ǳ�����quit�ɷ���������"<<endl;
    cout<<"������������������������������������"<<endl;
    cout<<"�������˻��ǳƣ�"<<endl;
    getline(cin, temp_name);
    if(temp_name=="quit"){
        send(SocketClient, "RQ,0,0", strlen("RQ,0,0"), 0);
        return -1;
    }
    cout<<"������������������������������������"<<endl;
    cout<<"�������˻����룺"<<endl;
    getline(cin, temp_pwd);
    if(temp=="Register,"){out = temp_name+","+temp_pwd;}
    temp.append(temp_name);
    temp.append(",");
    temp.append(temp_pwd);
    send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
    cout<<"������������������������������������"<<endl;
    temp = out;
    return 0;
}

int RRequest(SOCKET &SocketClient, string &temp, string &temp_name, string &temp_pwd){
    cout<<"������������������������������������"<<endl;
    cout<<"�˻��ǳ�����quit�ɷ���������"<<endl;
    cout<<"������������������������������������"<<endl;
    cout<<"�������˻��ǳƣ�"<<endl;
    getline(cin, temp_name);
    if(temp_name=="quit"){
        send(SocketClient, "RQ,0,0", strlen("RQ,0,0"), 0);
        return -1;
    }
    cout<<"������������������������������������"<<endl;
    cout<<"�������˻����룺"<<endl;
    getline(cin, temp_pwd);
    temp.append(temp_name);
    temp.append(",");
    temp.append(temp_pwd);
    send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
    cout<<"������������������������������������"<<endl;
    return 0;
}

void Login(SOCKET &SocketClient, User *&user){
    int last = 5;
    char RecvBuf[MaxBufSize];
    int recvByte;
    int CEflag;
    int experience;
    int level;
    int nop;
    string temp;
    string name;
    string pwd;
    while(1){
        temp = "Login,";
        cout<<"***********************************"<<endl;
        cout<<"*         -----Login-----         *"<<endl;
        cout<<"***********************************"<<endl;
        // ��ȡ�û�������ǳƼ����벢���͸������
        recvByte = LRequest(SocketClient, temp);
        if(recvByte==-1){break;}
        // �ȴ�����˵�¼���
        Recv(SocketClient, RecvBuf, recvByte);
        if(recvByte==0){
            cout<<"���ݽ���ʧ�ܣ�"<<endl;
            break;
        }
        temp = RecvBuf;
        if(temp=="Name Error"){
            cout<<"����������~~�˻��ǳƲ����ڣ���ע����ٵ�¼~~����������"<<endl;
            last--;
            cout<<"���������Գ���"<<last<<"�ε�¼��"<<endl;
            continue;
        }else if(temp=="Password Error"){
            cout<<"����������~~�������˻���ƥ�䣡�����³���~~����������"<<endl;
            last--;
            cout<<"���������Գ���"<<last<<"�ε�¼��"<<endl;
            continue;
        }else if(temp=="File Error"){
            cout<<"����������~~������ļ�ȱʧ�����Ժ�����~~����������"<<endl;
            break;
        }else if(temp=="Over Try"){
            cout<<"�������������Դ������࣡�����µ�¼����������"<<endl;
            break;
        }else{
            istringstream sin(temp);
            getline(sin, temp, ',');
            CEflag = atoi(temp.c_str());
            getline(sin, name, ',');
            getline(sin, pwd, ',');
            getline(sin, temp, ',');
            experience = atoi(temp.c_str());
            getline(sin, temp, ',');
            level = atoi(temp.c_str());
            getline(sin, temp);
            nop = atoi(temp.c_str());
            delete user;
            user = NULL;
            if(CEflag){
                user = new Examiner(name, pwd, CEflag, experience, level, nop);
            }else{user = new Challenger(name, pwd, CEflag, experience, level, nop);}
            break;
        }
    }
    return ;
}

void Register(SOCKET &SocketClient, User *&user){
    char RecvBuf[MaxBufSize];
    int recvByte;
    int CEflag;
    string temp;
    string name;
    string pwd;
    while(1){
        temp = "Register,";
        cout<<"************************************"<<endl;
        cout<<"*        -----Register-----        *"<<endl;
        cout<<"************************************"<<endl;
        // ��ȡ�û�������ǳƼ����벢���͸������
        recvByte = RRequest(SocketClient, temp, name, pwd);
        if(recvByte==-1){break;}
        // �ȴ�����˵�¼���
        Recv(SocketClient, RecvBuf, recvByte);
        if(recvByte==0){
            cout<<"���ݽ���ʧ�ܣ�"<<endl;
            break;
        }
        temp = RecvBuf;
        if(temp=="Fail"){
            cout<<"����������~~�˻��ǳ��ѱ�ʹ�ã�����������~~����������"<<endl;
            continue;
        }else if(temp=="File Error"){
            cout<<"����������~~������ļ�ȱʧ�����Ժ�����~~����������"<<endl;
            break;
        }else if(temp=="Success"){
            // ���˻�Ϊ����ȷ���û�����
            while(1){
                cout<<"������������������������������������"<<endl;
                cout<<"�ǳƿ��ã���ѡ���������"<<endl;
                cout<<"������������������������������������"<<endl;
                cout<<"����0��ʾ������"<<endl;
                cout<<"����1��ʾ������"<<endl;
                cout<<"������������������������������������"<<endl;
                cin>>temp;
                if((temp=="0")||(temp=="1")){
                    CEflag = atoi(temp.c_str());
                    while(1){
                        send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
                        Recv(SocketClient, RecvBuf, recvByte);
                        if((string)RecvBuf=="Created"){
                            delete user;
                            user = NULL;
                            if(CEflag){
                                user = new Examiner(name, pwd);
                            }else{user = new Challenger(name, pwd);}
                            cout<<"����������ע��ɹ����������Զ�Ϊ����¼����������"<<endl;
                            break;
                        }
                    }
                    break;
                }else{cout<<"����������~~��������Ч����~~����������"<<endl;}
            }
            break;
        }else{
            cout<<"�����������仰��˵�����ݴ��������Orz"<<endl;
            break;
        }
    }
    return ;
}

void Rank(SOCKET &SocketClient){
    char RecvBuf[MaxBufSize];
    int recvByte;
    string path;
    string name;
    string temp;

    while(1){
        cout<<"������������������������������������"<<endl;
        cout<<"��ѡ����Ҫ�鿴�����а�"<<endl;
        cout<<"������������������������������������"<<endl;
        cout<<"����-1����������"<<endl;
        cout<<"����0�鿴�����а�"<<endl;
        cout<<"����1�鿴��������"<<endl;
        cout<<"������������������������������������"<<endl;
        cin>>temp;
        if(temp=="1"){
            while(1){
                cout<<"������������������������������������"<<endl;
                cout<<"��ѡ����Ҫ�鿴�����а�"<<endl;
                cout<<"������������������������������������"<<endl;
                cout<<"����0������һ������"<<endl;
                cout<<"����1�鿴�����ߵȼ����а�"<<endl;
                cout<<"����2�鿴�����߳��������а�"<<endl;
                cout<<"����3�鿴��ս�ߵȼ����а�"<<endl;
                cout<<"����4�鿴��ս�߾������а�"<<endl;
                cout<<"����5�鿴��ս��ͨ���ؿ����а�"<<endl;
                cout<<"������������������������������������"<<endl;
                cin>>temp;
                if(temp=="0"){
                    cout<<"�������������ڷ�����һ�����桪��������"<<endl;
                    system("pause");
                }else if(temp=="1"){
                    path = "./Datas/ERL.csv";
                }else if(temp=="2"){
                    path = "./Datas/ERN.csv";
                }else if(temp=="3"){
                    path = "./Datas/CRL.csv";
                }else if(temp=="4"){
                    path = "./Datas/CRE.csv";
                }else if(temp=="5"){
                    path = "./Datas/CRP.csv";
                }else{
                    cout<<"����������~~��������Ч����~~����������"<<endl;
                    continue;
                }
                break ;
            }
            if(temp=="0"){continue;}
            cout<<"������������������������������������"<<endl;
            cout<<"�����������ѯ���û����ǳƣ�"<<endl;
            cout<<"������������������������������������"<<endl;
            cin.ignore(MAXSIZE, '\n');
            getline(cin, name);
            temp = "Rank,";
            temp.append(path);
            temp.append(",");
            temp.append(name);
            send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
            Recv(SocketClient, RecvBuf, recvByte);
            temp = RecvBuf;
            if(temp=="-1"){
                cout<<"����������~~������ļ�ȱʧ�����Ժ�����~~����������"<<endl;
                return ;
            }else if(temp=="0"){
                cout<<"����������~~δ��ѯ���û�"<<name<<"�����ݣ����ڷ�����һ������~~����������"<<endl;
                system("pause");
            }else{
                cout<<"�����������û�"<<name<<"�������ǵ�"<<temp<<"��������������"<<endl;
                system("pause");
            }
            continue ;
        }else if(temp=="0"){
            while(1){
                cout<<"������������������������������������"<<endl;
                cout<<"��ѡ����Ҫ�鿴�����а�"<<endl;
                cout<<"������������������������������������"<<endl;
                cout<<"����0������һ������"<<endl;
                cout<<"����1�鿴�����ߵȼ����а�"<<endl;
                cout<<"����2�鿴�����߳��������а�"<<endl;
                cout<<"����3�鿴��ս�ߵȼ����а�"<<endl;
                cout<<"����4�鿴��ս�߾������а�"<<endl;
                cout<<"����5�鿴��ս��ͨ���ؿ����а�"<<endl;
                cout<<"������������������������������������"<<endl;
                cin>>temp;
                if(temp=="0"){
                    cout<<"�������������ڷ�����һ�����桪��������"<<endl;
                    system("pause");
                }else if(temp=="1"){
                    path = "./Datas/ERL.csv";
                }else if(temp=="2"){
                    path = "./Datas/ERN.csv";
                }else if(temp=="3"){
                    path = "./Datas/CRL.csv";
                }else if(temp=="4"){
                    path = "./Datas/CRE.csv";
                }else if(temp=="5"){
                    path = "./Datas/CRP.csv";
                }else{
                    cout<<"����������~~��������Ч����~~����������"<<endl;
                    continue;
                }
                break ;
            }
            if(temp=="0"){continue;}
            temp = "PAR,";
            temp.append(path);
            send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
            Recv(SocketClient, RecvBuf, recvByte);
            temp = RecvBuf;
            if(temp=="File Error"){
                cout<<"����������~~������ļ�ȱʧ�����Ժ�����~~����������"<<endl;
                return ;
            }else{
                cout<<temp<<endl;
                system("pause");
            }
        }else if(temp=="-1"){
            return ;
        }else{cout<<"����������~~��������Ч����~~����������"<<endl;}
    }
}

void SetValue(SOCKET &SocketClient, User *&user){
    int flag;
    int recvByte;
    char RecvBuf[MaxBufSize];
    string temp;
    while(1){
        cout<<"************************************"<<endl;
        cout<<"*  ����-1������һ������"<<endl;
        cout<<"*  ����0�޸��ǳ�"<<endl;
        cout<<"*  ����1�޸�����"<<endl;
        cout<<"************************************"<<endl;
        cin>>temp;
        // ��Ҳ��֪��Ϊʲô������cin.ignore(MAXSIZE, '/n');��������Ӧ����ѭ��
        cin.get();  // �л�����δ��շ���
        if(temp=="-1"){
            return ;
        }else if(temp=="0"){
            string new_name;
            cout<<"������������������������������������"<<endl;
            cout<<"���������ǳƣ�"<<endl;
            cout<<"������������������������������������"<<endl;
            getline(cin, new_name);
            temp = "CN,"+to_string(user->get_CEflag())+","+user->get_name()+","+new_name;
            send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
            Recv(SocketClient, RecvBuf, recvByte);
            temp = RecvBuf;
            if(temp=="-1"){
                temp = "����������~~�û������ļ���ȡʧ�ܣ������ļ��Ƿ���ȷ~~����������";
            }else if(temp=="0"){
                temp = "����������~~�ǳ��Ѵ��ڣ�������ѡ���ǳ�~~����������";
                continue;
            }else{
                temp = "==========�ǳƿ��ã����ǳ��������==========";
                user->set_name(new_name);
            }
            cout<<temp<<endl;
            system("pause");
            return ;
        }else if(temp=="1"){
            string pwd;
            flag = atoi(temp.c_str());
            cout<<"������������������������������������"<<endl;
            cout<<"�����������룺"<<endl;
            cout<<"������������������������������������"<<endl;
            getline(cin, pwd);
            temp = "CP,"+to_string(user->get_CEflag())+","+user->get_name()+","+pwd;
            send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
            user->set_pwd(pwd);
            cout<<"==========������������ϣ�=========="<<endl;
            system("pause");
            return ;
        }else{cout<<"����������~~��������Ч����~~����������"<<endl;}
    }
}
