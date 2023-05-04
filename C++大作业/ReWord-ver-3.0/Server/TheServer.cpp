# include "TheServer.h"

int main(){
    SOCKET ListenSocket;
    if(InitAndListen(ListenSocket)==0){return 0;}
    cout<<"Listen socket is "<<ListenSocket<<endl;

    // 设置超时等待时间
    timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    // 将套接字放入文件描述符集合以便后续的select等操作
    fd_set fdSocket;
    FD_ZERO(&fdSocket);
    FD_SET(ListenSocket, &fdSocket);

    // 循环接收客户端连接请求并创建服务线程
    while(1){
		SOCKET *ClientSocket = new SOCKET;
		ClientSocket = (SOCKET*)malloc(sizeof(SOCKET));
        cout<<"new socket "<<*ClientSocket<<" created"<<endl;
        sockaddr_in ClientAddr;
        int SockAddrlen = sizeof(ClientAddr);
        /*
        SOCKET WSAAPI accept(
            [in]      SOCKET   s,           标识已使用listen函数处于侦听状态的套接字
            [out]     sockaddr *addr,       指向接收连接实体地址即通信层的缓冲区的可选指针
            [in, out] int      *addrlen     指向包含addr参数指向的结构长度的整数的可选指针
        );
        accept函数允许在套接字上尝试传入连接，该函数会阻塞调用进程直至新的连接出现
        若未发生错误则accept将返回一个类型为SOCKET的值，该值是新套接字的描述符。此返回的值是实际连接的套接字的句柄。
        */
        *ClientSocket = accept(ListenSocket, 0, 0);   // 对客户端的地址及长度不感兴趣，故设为0或NULL
        cout<<"连接成功！"<<endl;
        // 监听失败则继续监听，否则创建新线程服务客户端
        if(*ClientSocket==INVALID_SOCKET){
            cout<<"数据接收失败！错误代码："<<WSAGetLastError()<<endl;
            continue;
        }else{
            if(getpeername(*ClientSocket, (sockaddr *)&ClientAddr, &SockAddrlen)==-1){
                cout<<"getpeername error"<<endl;
                continue;
            }
            cout<<"一个IP为"<<inet_ntoa(ClientAddr.sin_addr)<<"，端口为"<<ntohs(ClientAddr.sin_port)<<"，socket为"<<*ClientSocket<<"的客户端已连接到服务器"<<endl;
        }
        /*
        HANDLE CreateThread(
            [in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes,     指向SECURITY_ATTRIBUTES结构的指针。若此参数为NULL则无法继承句柄
            [in]            SIZE_T                  dwStackSize,            以字节为单位的堆栈初始大小。若此参数为零则新线程将使用可执行文件的默认大小
            [in]            LPTHREAD_START_ROUTINE  lpStartAddress,         指向由线程执行的应用程序定义函数的指针
            [in, optional]  __drv_aliasesMem LPVOID lpParameter,            指向要传递给线程的变量的指针
            [in]            DWORD                   dwCreationFlags,        控制线程创建的标志，若此参数为零则创建后线程会立即运行
            [out, optional] LPDWORD                 lpThreadId              指向接收线程标识符的变量的指针。若此参数为NULL则不返回线程标识符
        );
        */
        HANDLE hThread = CreateThread(NULL, 0, &ServerThread, (LPVOID)ClientSocket, 0, NULL);    // 函数成功则返回新线程的句柄，否则返回NULL
        if(hThread==NULL){cout<<"线程创建失败！错误代码："<<WSAGetLastError()<<endl;}
    }
    freesocket(ListenSocket);  // 释放socket
    WSACleanup();   // 关闭网络库
    return 0;
}

int InitAndListen(SOCKET &ListenSocket){
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
    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);     // 返回引用新套接字的描述符
    if(ListenSocket==INVALID_SOCKET){
        cout<<"套接字创建失败！错误代码："<<WSAGetLastError()<<endl;
        WSACleanup();   // 关闭网络库
        return 0;
    }

    // 设置监听的IP和端口等
    sockaddr_in ListenAddr;
    memset(&ListenAddr, 0, sizeof(ListenAddr));
    ListenAddr.sin_family = AF_INET;            // IPv4地址族
    ListenAddr.sin_addr.s_addr = INADDR_ANY;    // 不绑定本主机的某个特定ip
    ListenAddr.sin_port = htons(PORT);          // 由系统随机分配未使用的端口

    // 给socket绑定具体地址（定位到电脑）与端口号（定位到具体应用）
    /*
    int WSAAPI bind(
        [in] SOCKET         s,      标识未绑定套接字的描述符
        [in] const sockaddr *name,  指向要分配给绑定套接字的本地地址的sockaddr结构的指针
        [in] int            namelen 名称参数指向的值的长度（以字节为单位）
    );
    bind函数将本地地址与套接字相关联。若未发生错误则返回零。否则返回SOCKET_ERROR
    */
    if(bind(ListenSocket, (LPSOCKADDR)&ListenAddr, sizeof(ListenAddr))==SOCKET_ERROR){
        /*
        bind是服务端调用的函数，绑定的是服务端的IP
        假设服务端有多个网卡，即会有多个IP
        此时可选择用bind绑定其中一个IP，则服务端只接收该IP上某端口的数据
        若bind绑定的是INADDR_ANY，则所有发送到服务器的这个端口的无论哪个IP接收到的数据都由这个服务端进程处理
        */
        cout<<"套接字绑定端口失败！错误代码："<<WSAGetLastError()<<endl;
        freesocket(ListenSocket);
        WSACleanup();   // 关闭网络库
        return 0;
    }else{cout<<"端口"<<PORT<<"绑定成功！"<<endl;}

    // 开始监听，SOMAXCONN——由系统决定合适的挂起连接队列最大长度
    /*
    int WSAAPI listen(
        [in] SOCKET s,          标识绑定的未连接套接字的描述符
        [in] int    backlog     挂起连接队列的最大长度
    );
    listen函数将套接字置于侦听传入连接的状态
    */
    if(listen(ListenSocket, SOMAXCONN)==SOCKET_ERROR){
        cout<<"监听失败！错误代码："<<WSAGetLastError()<<endl;
        freesocket(ListenSocket);  // 释放socket
        WSACleanup();   // 关闭网络库
        return 0;
    }
    cout<<"服务端准备就绪，等待连接请求..."<<endl;
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

void Send(SOCKET &ClientSocket, string &temp){
    cout<<"向客户端"<<ClientSocket<<"发送长度为"<<strlen(temp.c_str())<<"的消息："<<endl;
    cout<<temp.c_str()<<endl;
    send(ClientSocket, temp.c_str(), strlen(temp.c_str()), 0);
    return ;
}

int Recv(SOCKET &ClientSocket, char *RecvBuf, int &recvByte){
    recvByte = 0;
    while(1){
        memset(RecvBuf, 0, MaxBufSize);    // 重置缓冲区
        recvByte = recv(ClientSocket, RecvBuf, MaxBufSize, 0);
        cout<<"接收到来自客户端"<<ClientSocket<<"的长度为"<<recvByte<<"的消息："<<endl;
        cout<<RecvBuf<<endl;
        if(recvByte==-1){cout<<"recv失败！错误代码："<<WSAGetLastError()<<endl;}
        if(recvByte>0){return recvByte;}
    }
}

int isIFE(ifstream &infile){
    streampos fsize;

    // 数据文件读取失败则退出
    if(!infile){
        cout<<"—————文件读取失败！请检查文件是否正确—————"<<endl;
        // 文件打开失败时若先clear再close则无法消除ifstream的错误流状态
        infile.close();
        infile.clear();     // 清除流的状态
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
            send(ClientSocket, RecvBuf, infile.gcount(), 0);    // gcount()返回上一次未经格式化的输入操作提取的字符数。
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
    // 若文件不空则按行读取已有账户数据比较用户名
    if(flag==-1){return "File Error";}
    if(flag>0){
        infile.seekg(0, ios::beg);
        while(getline(infile, line)){
            istringstream sin(line);    // 将整行字符串line读入到字符串流sin中
            getline(sin, name, ',');    // 将字符串流sin中第一个逗号前的字符读入name中
            getline(sin, name, ',');    // 将字符串流sin中第二个逗号前的字符读入name中
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
                getline(sin, name);     // 将字符串流sin中剩余部分字符读入temp中
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
    // 若文件不空则按行读取已有账户数据比较用户名
    if(flag==-1){return "File Error";}
    if(flag>0){
        infile.seekg(0, ios::beg);
        while(getline(infile, line)){
            istringstream sin(line);    // 将整行字符串line读入到字符串流sin中
            getline(sin, temp, ',');    // 将字符串流sin中第一个逗号前的字符读入temp中
            getline(sin, temp, ',');    // 将字符串流sin中第二个逗号前的字符读入temp中
            if(temp_name==temp){
                infile.close();
                return "Fail";
            }
            getline(sin, temp);         // 将字符串流sin中剩余部分字符读入temp中
        }
    }
    flag = 0;
    infile.close();
    return "Success";
}

void wUcsv(int CEflag, string temp_name, string temp_pwd, string path){
    // 新用户数据写入users.csv
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
        infile.seekg(-4, ios::end); // 向前跳过最后一行的换行符
        while(1){
            if(infile.peek()==infile.widen('\n')){
                infile.seekg(2, ios::cur);  // 向后跳过倒数第二行的换行符
                break;
            }
            if(infile.tellg()==0){
                infile.seekg(0, ios::beg);  // 我也不知道为什么要加上这句结果才对Orz明明都tellg==0了
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
    string out = "排名\t用户\t数值\n";
    ifstream infile(path, ios::in|ios::app);

    flag = isIFE(infile);
    if(flag==-1){
        return "File Error";
    }else if(flag==0){
        return "—————排行榜上暂时没有数据！快注册上榜吧—————";
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
    out.append("—————排行榜输出完毕！—————");
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
    if(!infile){return "！！！！！~~用户数据文件读取失败！请检查文件是否正确~~！！！！！";}
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
            out.append("#  以下是所查询用户的属性面板\n");
            out.append("####################################\n");
            out.append("#  昵称：\t\t");
            out.append(name);
            out.append("\n");
            getline(sin, temp, ',');    // 不应该显示其他用户的密码
            getline(sin, temp, ',');
            out.append("#  经验：\t\t");
            out.append(temp);
            out.append("\n");
            getline(sin, temp, ',');
            out.append("#  等级：\t\t");
            out.append(temp);
            out.append("\n");
            getline(sin, temp);
            if(CEflag=="0"){
                out.append("#  通过关卡数：\t\t");
                out.append(temp);
                out.append("\n");
                out.append("#  等级排行榜排名：\t");
                out.append(getRank(name, "./Datas/CRL.csv"));
                out.append("\n");
                out.append("#  经验排行榜排名：\t");
                out.append(getRank(name, "./Datas/CRE.csv"));
                out.append("\n");
                out.append("#  关卡排行榜排名：\t");
                out.append(getRank(name, "./Datas/CRP.csv"));
                out.append("\n");
            }else{
                out.append("#  累计出题数：\t\t");
                out.append(temp);
                out.append("\n");
                out.append("#  等级排行榜排名：\t");
                out.append(getRank(name, "./Datas/ERL.csv"));
                out.append("\n");
                out.append("#  题数排行榜排名：\t");
                out.append(getRank(name, "./Datas/ERN.csv"));
                out.append("\n");
            }
            out.append("####################################");
            infile.close();
            return out;
        }else{getline(sin, temp);}
    }
    infile.close();
    return "！！！！！~~所查询用户不存在！请检查输入~~！！！！！";
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
    // 文件数据读入vector
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
    // 更新用户数据
    num = 0;
    while(names[num]!=name){num++;}
    values[num] = new_num;
    // vector冒泡排序
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
    // vector写入文件
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
        // 等待客户端请求功能
        Recv(*ClientSocket, RecvBuf, recvByte);
        temp = RecvBuf;
        istringstream sin(temp);
        getline(sin, request, ',');
        if(request=="Login"){
            string temp_name;
            string temp_pwd;
            getline(sin, temp_name, ',');
            getline(sin, temp_pwd);
            cout<<"用户输入昵称为"<<temp_name<<"，用户输入密码为"<<temp_pwd<<endl;
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
                    }else{cout<<"如果看到了这句话就说明数据传输出错了Orz"<<endl;}
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
            cout<<"接收到错误请求："<<request<<endl;
            send(*ClientSocket, "Wrong Request", strlen("Wrong Request"), 0);
        }
    }
	freesocket(*ClientSocket);
    free(ClientSocket);
    return 0;
}
