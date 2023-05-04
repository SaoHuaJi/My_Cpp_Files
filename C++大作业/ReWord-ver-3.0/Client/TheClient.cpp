# include "TheClient.h"

int main(){
    SOCKET SocketClient;
    InitSocket(SocketClient);
    
    // 设置超时等待时间
    timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    // 将套接字放入文件描述符集合以便后续的select等操作
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
            cout<<"—————程序已退出！期待下次再见—————"<<endl;
            break;
        }else if(flag==-2){
            cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;
            continue;
        }else{
            /*
            int WSAAPI select(
                [in]      int           nfds,           可忽略。仅包含nfds参数以便与Berkeley套接字兼容
                [in, out] fd_set        *readfds,       指向一组要检查的套接字是否可读的可选指针
                [in, out] fd_set        *writefds,      指向一组要检查的套接字是否可写的可选指针
                [in, out] fd_set        *exceptfds,     指向一组要检查错误的套接字的可选指针
                [in]      const timeval *timeout        选择等待的最大时间，以TIMEVAL结构的形式提供。值为NULL可阻止操作
            );
            */
            if(select(0, &fdSocket, &fdSocket, NULL, &timeout)==0){
                cout<<"服务器正忙，请稍候重试..."<<endl;
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
            }else{cout<<"我都不知道你是怎么能看到这句话的Orz"<<endl;}
        }
    }
    freesocket(SocketClient);
    WSACleanup();
    return 0;
}

int InitSocket(SOCKET &SocketClient){
    // 初始化套接字动态库
    // 声明调用2.2版本的winsock
    WORD sockVersion = MAKEWORD(2,2);   // MAKEWORD合并两个byte为一个word（无符号16位整形数）
    WSAData wsd;    // 用于接收Windows套接字实现的详细信息
    if(WSAStartup(sockVersion, &wsd)!=0){
        cout<<"启动Winsock DLL失败！程序结束..."<<endl;
        WSACleanup();   // 关闭网络库
        return 0;
    }
    
    // 创建套接字
    // AF_INET——IPv4地址系列，SOCK_STREAM——流格式，0——由服务提供者为你选择合适的协议类型
    SocketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    // 返回引用新套接字的描述符
    if(SocketClient==INVALID_SOCKET){
        cout<<"套接字创建失败！错误代码："<<WSAGetLastError()<<endl;
        WSACleanup();   // 关闭网络库
        return 0;
    }

    // 设置要连接的服务端的IP和端口等
    sockaddr_in SeverAddr;
    memset(&SeverAddr, 0, sizeof(SeverAddr));
    SeverAddr.sin_family = AF_INET;                     // IPv4地址族
    /*
    如果是局域网下的不同主机间的socket通信则需指定服务端的IP
    cout<<"请输入要连接的服务端IP号："<<endl;
    string SeverIP;
    cin>>ServerIP;
    SeverAddr.sin_addr.s_addr = inet_addr(SeverIP.c_str());
    */
    SeverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 绑定本机服务端ip地址
    // 端口往往本地是大端而网络是小端
    SeverAddr.sin_port = htons(PORT);                   // 需为系统未使用的端口
    cout<<SeverAddr.sin_addr.s_addr<<endl;

    // 开始连接，SOMAXCONN——由系统决定合适的挂起连接队列最大长度
    if(connect(SocketClient, (sockaddr*)&SeverAddr, sizeof(SeverAddr))==SOCKET_ERROR){
        cout<<"连接失败！错误代码："<<WSAGetLastError()<<endl;
        freesocket(SocketClient);  // 释放socket
        WSACleanup();   // 关闭网络库
        return 0;
    }
    cout<<"服务端连接成功..."<<endl;
    return 1;
}

void freesocket(SOCKET &Socket){
    if(Socket!=INVALID_SOCKET){
        cout<<"关闭套接字"<<Socket<<endl;
        closesocket(Socket);
        Socket = INVALID_SOCKET;
    }
    return ;
}

int Recv(SOCKET &SocketClient, char *RecvBuf, int &recvByte){
    recvByte = 0;
    while(1){
        // memset(void *str, int c, size_t n)将str指向的字符串的前n个字符替换为c
        memset(RecvBuf, 0, MaxBufSize);
        recvByte = recv(SocketClient, RecvBuf, MaxBufSize, 0);
        if(recvByte==-1){cout<<"recv失败！错误代码："<<WSAGetLastError()<<endl;}
        if(recvByte>0){return recvByte;}
    }
}

int MainMenu(){
    string temp_flag;
    int flag;
    cout<<"************************************"<<endl;
    cout<<"*  尊敬的用户您好，请问您要干什么？"<<endl;
    cout<<"************************************"<<endl;
    cout<<"*  输入0选择登录"<<endl;
    cout<<"*  输入1选择注册"<<endl;
    cout<<"*  输入2查看排行榜"<<endl;
    cout<<"*  输入-1退出程序"<<endl;
    cout<<"************************************"<<endl;
    cin>>temp_flag;
    // 缓冲区不清空会对后面的getline()造成影响
    // cin.sync()实现效果与编译器有关故不使用
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
        send(ClientSocket, RecvBuf, infile.gcount(), 0);    // gcount()返回上一次未经格式化的输入操作提取的字符数。
        if(infile.gcount()==0){
            send(ClientSocket, "Send Over", strlen("Send Over"), 0);
            break;
        }
        if(infile.gcount()<sizeof(RecvBuf)){
            // 为避免后续的Update()结果被一并放入缓冲区传输，这里等待一次服务端的接收结束信号
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
            cout<<"—————您已成功退出登录！正在返回主界面—————"<<endl;
            system("pause");
            break;
        }else if(flag==0){
            name = user->get_name();
            if(user->get_CEflag()){
                // 设置出题者等级排名
                temp = "Rank,";
                temp.append("./Datas/ERL.csv");
                temp.append(",");
                temp.append(name);
                send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
                Recv(SocketClient, RecvBuf, recvByte);
                user->set_ERL(atoi(RecvBuf));
                // 设置出题者出题数排名
                temp = "Rank,";
                temp.append("./Datas/ERN.csv");
                temp.append(",");
                temp.append(name);
                send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
                Recv(SocketClient, RecvBuf, recvByte);
                user->set_ERN(atoi(RecvBuf));
                user->ShowValue();
            }else{
                // 设置挑战者等级排名
                temp = "Rank,";
                temp.append("./Datas/CRL.csv");
                temp.append(",");
                temp.append(name);
                send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
                Recv(SocketClient, RecvBuf, recvByte);
                user->set_CRL(atoi(RecvBuf));
                // 设置挑战者经验排名
                temp = "Rank,";
                temp.append("./Datas/CRE.csv");
                temp.append(",");
                temp.append(name);
                send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
                Recv(SocketClient, RecvBuf, recvByte);
                user->set_CRE(atoi(RecvBuf));
                // 设置挑战者通关数排名
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
            cout<<"——————————————————"<<endl;
            cout<<"请输入要查询的用户的昵称"<<endl;
            cout<<"——————————————————"<<endl;
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
        cout<<"欢迎回来！出题者"
            <<user->get_name()
            <<"！"<<endl;
            while(1){
                cout<<"请问您要做什么？"<<endl;
                cout<<"************************************"<<endl;
                cout<<"*  输入-1返回主界面"<<endl;
                cout<<"*  输入0查看当前自身属性"<<endl;
                cout<<"*  输入1查看其他人的属性"<<endl;
                cout<<"*  输入2修改账户昵称和密码"<<endl;
                cout<<"*  输入3继续出题"<<endl;
                cout<<"************************************"<<endl;
                // 直接用flag获取输入则若输入不为数字会陷入无应答死循环
                // 如果用atoi转入flag则字符都会变为0
                cin>>temp;
                if((temp=="-1")||(temp=="0")||(temp=="1")||(temp=="2")||(temp=="3")){
                    flag = atoi(temp.c_str());
                    break;
                }else{cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;}
            }
    }else{
        cout<<"************************************"<<endl;
        cout<<"欢迎回来！挑战者"
            <<user->get_name()
            <<"！"<<endl;
            while(1){
                cout<<"请问您要做什么？"<<endl;
                cout<<"************************************"<<endl;
                cout<<"*  输入-1返回主界面"<<endl;
                cout<<"*  输入0查看当前自身属性"<<endl;
                cout<<"*  输入1查看其他人的属性"<<endl;
                cout<<"*  输入2修改账户昵称和密码"<<endl;
                cout<<"*  输入3继续闯关"<<endl;
                cout<<"************************************"<<endl;
                cin>>temp;
                if((temp=="-1")||(temp=="0")||(temp=="1")||(temp=="2")||(temp=="3")){
                    flag = atoi(temp.c_str());
                    break;
                }else{cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;}
            }
    }
    return flag;
}

int LRequest(SOCKET &SocketClient, string &temp){
    string temp_name;
    string temp_pwd;
    string out = NULL;
    cout<<"——————————————————"<<endl;
    cout<<"账户昵称输入quit可返回主界面"<<endl;
    cout<<"——————————————————"<<endl;
    cout<<"请输入账户昵称："<<endl;
    getline(cin, temp_name);
    if(temp_name=="quit"){
        send(SocketClient, "RQ,0,0", strlen("RQ,0,0"), 0);
        return -1;
    }
    cout<<"——————————————————"<<endl;
    cout<<"请输入账户密码："<<endl;
    getline(cin, temp_pwd);
    if(temp=="Register,"){out = temp_name+","+temp_pwd;}
    temp.append(temp_name);
    temp.append(",");
    temp.append(temp_pwd);
    send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
    cout<<"——————————————————"<<endl;
    temp = out;
    return 0;
}

int RRequest(SOCKET &SocketClient, string &temp, string &temp_name, string &temp_pwd){
    cout<<"——————————————————"<<endl;
    cout<<"账户昵称输入quit可返回主界面"<<endl;
    cout<<"——————————————————"<<endl;
    cout<<"请输入账户昵称："<<endl;
    getline(cin, temp_name);
    if(temp_name=="quit"){
        send(SocketClient, "RQ,0,0", strlen("RQ,0,0"), 0);
        return -1;
    }
    cout<<"——————————————————"<<endl;
    cout<<"请输入账户密码："<<endl;
    getline(cin, temp_pwd);
    temp.append(temp_name);
    temp.append(",");
    temp.append(temp_pwd);
    send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
    cout<<"——————————————————"<<endl;
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
        // 获取用户输入的昵称及密码并发送给服务端
        recvByte = LRequest(SocketClient, temp);
        if(recvByte==-1){break;}
        // 等待服务端登录结果
        Recv(SocketClient, RecvBuf, recvByte);
        if(recvByte==0){
            cout<<"数据接收失败！"<<endl;
            break;
        }
        temp = RecvBuf;
        if(temp=="Name Error"){
            cout<<"！！！！！~~账户昵称不存在！请注册后再登录~~！！！！！"<<endl;
            last--;
            cout<<"（您还可以尝试"<<last<<"次登录）"<<endl;
            continue;
        }else if(temp=="Password Error"){
            cout<<"！！！！！~~密码与账户不匹配！请重新尝试~~！！！！！"<<endl;
            last--;
            cout<<"（您还可以尝试"<<last<<"次登录）"<<endl;
            continue;
        }else if(temp=="File Error"){
            cout<<"！！！！！~~服务端文件缺失！请稍后再试~~！！！！！"<<endl;
            break;
        }else if(temp=="Over Try"){
            cout<<"—————尝试次数过多！请重新登录—————"<<endl;
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
        // 获取用户输入的昵称及密码并发送给服务端
        recvByte = RRequest(SocketClient, temp, name, pwd);
        if(recvByte==-1){break;}
        // 等待服务端登录结果
        Recv(SocketClient, RecvBuf, recvByte);
        if(recvByte==0){
            cout<<"数据接收失败！"<<endl;
            break;
        }
        temp = RecvBuf;
        if(temp=="Fail"){
            cout<<"！！！！！~~账户昵称已被使用！请重新设置~~！！！！！"<<endl;
            continue;
        }else if(temp=="File Error"){
            cout<<"！！！！！~~服务端文件缺失！请稍后再试~~！！！！！"<<endl;
            break;
        }else if(temp=="Success"){
            // 若账户为新则确定用户类型
            while(1){
                cout<<"——————————————————"<<endl;
                cout<<"昵称可用，请选择您的身份"<<endl;
                cout<<"——————————————————"<<endl;
                cout<<"输入0表示闯关者"<<endl;
                cout<<"输入1表示出题者"<<endl;
                cout<<"——————————————————"<<endl;
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
                            cout<<"—————注册成功！接下来自动为您登录—————"<<endl;
                            break;
                        }
                    }
                    break;
                }else{cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;}
            }
            break;
        }else{
            cout<<"如果看到了这句话就说明数据传输出错了Orz"<<endl;
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
        cout<<"——————————————————"<<endl;
        cout<<"请选择您要查看的排行榜"<<endl;
        cout<<"——————————————————"<<endl;
        cout<<"输入-1返回主界面"<<endl;
        cout<<"输入0查看总排行榜"<<endl;
        cout<<"输入1查看个人排名"<<endl;
        cout<<"——————————————————"<<endl;
        cin>>temp;
        if(temp=="1"){
            while(1){
                cout<<"——————————————————"<<endl;
                cout<<"请选择您要查看的排行榜"<<endl;
                cout<<"——————————————————"<<endl;
                cout<<"输入0返回上一个界面"<<endl;
                cout<<"输入1查看出题者等级排行榜"<<endl;
                cout<<"输入2查看出题者出题数排行榜"<<endl;
                cout<<"输入3查看挑战者等级排行榜"<<endl;
                cout<<"输入4查看挑战者经验排行榜"<<endl;
                cout<<"输入5查看挑战者通过关卡排行榜"<<endl;
                cout<<"——————————————————"<<endl;
                cin>>temp;
                if(temp=="0"){
                    cout<<"—————正在返回上一个界面—————"<<endl;
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
                    cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;
                    continue;
                }
                break ;
            }
            if(temp=="0"){continue;}
            cout<<"——————————————————"<<endl;
            cout<<"请输入您想查询的用户的昵称："<<endl;
            cout<<"——————————————————"<<endl;
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
                cout<<"！！！！！~~服务端文件缺失！请稍后再试~~！！！！！"<<endl;
                return ;
            }else if(temp=="0"){
                cout<<"！！！！！~~未查询到用户"<<name<<"的数据，正在返回上一个界面~~！！！！！"<<endl;
                system("pause");
            }else{
                cout<<"—————用户"<<name<<"的排名是第"<<temp<<"名！—————"<<endl;
                system("pause");
            }
            continue ;
        }else if(temp=="0"){
            while(1){
                cout<<"——————————————————"<<endl;
                cout<<"请选择您要查看的排行榜"<<endl;
                cout<<"——————————————————"<<endl;
                cout<<"输入0返回上一个界面"<<endl;
                cout<<"输入1查看出题者等级排行榜"<<endl;
                cout<<"输入2查看出题者出题数排行榜"<<endl;
                cout<<"输入3查看挑战者等级排行榜"<<endl;
                cout<<"输入4查看挑战者经验排行榜"<<endl;
                cout<<"输入5查看挑战者通过关卡排行榜"<<endl;
                cout<<"——————————————————"<<endl;
                cin>>temp;
                if(temp=="0"){
                    cout<<"—————正在返回上一个界面—————"<<endl;
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
                    cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;
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
                cout<<"！！！！！~~服务端文件缺失！请稍后再试~~！！！！！"<<endl;
                return ;
            }else{
                cout<<temp<<endl;
                system("pause");
            }
        }else if(temp=="-1"){
            return ;
        }else{cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;}
    }
}

void SetValue(SOCKET &SocketClient, User *&user){
    int flag;
    int recvByte;
    char RecvBuf[MaxBufSize];
    string temp;
    while(1){
        cout<<"************************************"<<endl;
        cout<<"*  输入-1返回上一个界面"<<endl;
        cout<<"*  输入0修改昵称"<<endl;
        cout<<"*  输入1修改密码"<<endl;
        cout<<"************************************"<<endl;
        cin>>temp;
        // 我也不知道为什么这里用cin.ignore(MAXSIZE, '/n');会陷入无应答死循环
        cin.get();  // 有缓冲区未清空风险
        if(temp=="-1"){
            return ;
        }else if(temp=="0"){
            string new_name;
            cout<<"——————————————————"<<endl;
            cout<<"请输入新昵称："<<endl;
            cout<<"——————————————————"<<endl;
            getline(cin, new_name);
            temp = "CN,"+to_string(user->get_CEflag())+","+user->get_name()+","+new_name;
            send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
            Recv(SocketClient, RecvBuf, recvByte);
            temp = RecvBuf;
            if(temp=="-1"){
                temp = "！！！！！~~用户数据文件读取失败！请检查文件是否正确~~！！！！！";
            }else if(temp=="0"){
                temp = "！！！！！~~昵称已存在！请重新选择昵称~~！！！！！";
                continue;
            }else{
                temp = "==========昵称可用！新昵称设置完毕==========";
                user->set_name(new_name);
            }
            cout<<temp<<endl;
            system("pause");
            return ;
        }else if(temp=="1"){
            string pwd;
            flag = atoi(temp.c_str());
            cout<<"——————————————————"<<endl;
            cout<<"请输入新密码："<<endl;
            cout<<"——————————————————"<<endl;
            getline(cin, pwd);
            temp = "CP,"+to_string(user->get_CEflag())+","+user->get_name()+","+pwd;
            send(SocketClient, temp.c_str(), strlen(temp.c_str()), 0);
            user->set_pwd(pwd);
            cout<<"==========新密码设置完毕！=========="<<endl;
            system("pause");
            return ;
        }else{cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;}
    }
}
