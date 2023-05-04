# include "Game.h"
# include "head.h"


// 主界面
void Game::MainMenu(int* flag){
    int CEflag;
    string temp_flag;
    while(1){
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

        if((temp_flag=="0")||(temp_flag=="1")||(temp_flag=="2")){
            *flag = atoi(temp_flag.c_str());
        }else if(temp_flag=="-1"){
            *flag = atoi(temp_flag.c_str());
            cout<<"—————程序已退出！期待下次再见—————"<<endl;
            break;
        }else{
            cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;
            continue;
        }
        Switch(*flag);

        if(*flag==-1){
            cout<<"—————程序已退出！期待下次再见—————"<<endl;
            break;
        }else if(*flag==1){
            CEflag = this->user->get_CEflag();
            while(1){
                *flag = welcomeUser(CEflag);
                if(*flag==-1){
                    this->user->UpdateUser();
                    cout<<"—————您已成功退出登录！正在返回主界面—————"<<endl;
                    system("pause");
                    break;
                }else if(*flag==0){
                    if(this->user->get_CEflag()){
                        this->user->set_ERL();
                        this->user->set_ERN();
                    }else{
                        this->user->set_CRL();
                        this->user->set_CRE();
                        this->user->set_CRP();
                    }
                    this->user->ShowValue();
                }else if(*flag==1){
                    cout<<"——————————————————"<<endl;
                    cout<<"请输入要查询的用户的昵称"<<endl;
                    cout<<"——————————————————"<<endl;
                    cin.ignore(MAXSIZE, '\n');
                    getline(cin, temp_flag);
                    this->user->check_user(temp_flag);
                }else if(*flag==2){
                    this->user->SetValue();
                }else{this->user->Play();}
            }
        }else{cout<<"—————成功返回主界面—————"<<endl;}
    }
    return ;
}
// 切换功能
// 文件出错返回-1
// 注册/登录完毕返回1
// 正常结束返回0
void Game::Switch(int &flag){
    if(flag==0){
        flag = this->UserLogin();
    }else if(flag==1){
        flag = this->UserRegister();
    }else if(flag==2){
        flag = this->Rank();
    }else{flag = 0;}
    return ;
}
// 注册
// 数据文件不存在则返回-1,
// 放弃注册返回0，注册成功返回1
int Game::UserRegister(){
    cout<<"************************************"<<endl;
    cout<<"*        -----Register-----        *"<<endl;
    cout<<"************************************"<<endl;
    int CEflag;
    int file_flag = 1;
    string temp_name;
    string temp_pwd;
    string temp_flag;

    while(file_flag==1){getNP(temp_name, temp_pwd, file_flag);}
    if(file_flag==-1){return -1;}
    if((file_flag==1)||(file_flag==2)){return 0;}

    // 若账户为新则确定用户类型
    while(1){
        cout<<"——————————————————"<<endl;
        cout<<"昵称可用，请选择您的身份"<<endl;
        cout<<"——————————————————"<<endl;
        cout<<"输入0表示闯关者"<<endl;
        cout<<"输入1表示出题者"<<endl;
        cout<<"——————————————————"<<endl;
        cin>>temp_flag;
        if((temp_flag=="0")||(temp_flag=="1")){
            CEflag = atoi(temp_flag.c_str());
            break;
        }else{cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;}
    }
    
    // 用户及排行榜数据更新
    if(CEflag){
        this->rank_path = "./Datas/ERL.csv";
        file_flag = rcsvl(this->rank_path);
        file_flag++;
        wRcsv(file_flag, temp_name, this->rank_path, 1);
        this->rank_path = "./Datas/ERN.csv";
        wRcsv(file_flag, temp_name, this->rank_path, 0);
    }else{
        this->rank_path = "./Datas/CRL.csv";
        file_flag = rcsvl(this->rank_path);
        file_flag++;
        wRcsv(file_flag, temp_name, this->rank_path, 1);
        this->rank_path = "./Datas/CRE.csv";
        wRcsv(file_flag, temp_name, this->rank_path, 0);
        this->rank_path = "./Datas/CRP.csv";
        wRcsv(file_flag, temp_name, this->rank_path, 0);
    }
    wUcsv(CEflag, temp_name, temp_pwd, this->user_path);
    cout<<"—————注册成功！接下来自动为您登录—————"<<endl;
    return 1;
}
// 登录
// 数据文件不存在则返回-1
// 登录次数过多返回0，登录成功返回1
int Game::UserLogin(){
    cout<<"***********************************"<<endl;
    cout<<"*         -----Login-----         *"<<endl;
    cout<<"***********************************"<<endl;
    int CEflag;
    int file_flag = 0;
    string temp_name;
    string temp_pwd;
    
    getNP(temp_name, temp_pwd, file_flag);
    return file_flag;
}
// 查看排行榜
// 文件不存在返回-1，否则返回0
int Game::Rank(){
    int flag;
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
                    flag = atoi(temp.c_str());
                    path = "./Datas/ERL.csv";
                    this->rank_path = path;
                }else if(temp=="2"){
                    flag = atoi(temp.c_str());
                    path = "./Datas/ERN.csv";
                    this->rank_path = path;
                }else if(temp=="3"){
                    flag = atoi(temp.c_str());
                    path = "./Datas/CRL.csv";
                    this->rank_path = path;
                }else if(temp=="4"){
                    flag = atoi(temp.c_str());
                    path = "./Datas/CRE.csv";
                    this->rank_path = path;
                }else if(temp=="5"){
                    flag = atoi(temp.c_str());
                    path = "./Datas/CRP.csv";
                    this->rank_path = path;
                }else{
                    cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;
                    continue;
                }
                break ;
            }
            if(!flag){continue;}
            cout<<"——————————————————"<<endl;
            cout<<"请输入您想查询的用户的昵称："<<endl;
            cout<<"——————————————————"<<endl;
            cin.ignore(MAXSIZE, '\n');
            getline(cin, name);
            flag = getRank(name, path);
            if(flag==-1){
                return -1;
            }else if(flag==0){
                cout<<"！！！！！~~未查询到用户"<<name<<"的数据，正在返回上一个界面~~！！！！！"<<endl;
                system("pause");
            }else{
                cout<<"—————用户"<<name<<"的排名是第"<<flag<<"名！—————"<<endl;
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
                    flag = atoi(temp.c_str());
                    cout<<"—————正在返回上一个界面—————"<<endl;
                    system("pause");
                }else if(temp=="1"){
                    flag = atoi(temp.c_str());
                    path = "./Datas/ERL.csv";
                    this->rank_path = path;
                }else if(temp=="2"){
                    flag = atoi(temp.c_str());
                    path = "./Datas/ERN.csv";
                    this->rank_path = path;
                }else if(temp=="3"){
                    flag = atoi(temp.c_str());
                    path = "./Datas/CRL.csv";
                    this->rank_path = path;
                }else if(temp=="4"){
                    flag = atoi(temp.c_str());
                    path = "./Datas/CRE.csv";
                    this->rank_path = path;
                }else if(temp=="5"){
                    flag = atoi(temp.c_str());
                    path = "./Datas/CRP.csv";
                    this->rank_path = path;
                }else{
                    cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;
                    continue;
                }
                break ;
            }
            if(!flag){continue;}
            flag = PAR(path);
            if(flag==-1){
                return -1;
            }else{
                cout<<"—————排行榜输出完毕！—————"<<endl;
                system("pause");
            }
        }else if(temp=="-1"){
            return 0;
        }else{cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;}
    }
}
// 获取昵称和密码，返回结果存在flag参数里
// flag为1说明是注册时调用，为0说明是登录时调用
// 文件不存在返回-1，主动退出返回2
// 注册时正常退出返回1，注册完毕返回0
// 登录时正常退出返回0，登录完毕返回1
void Game::getNP(string &name, string &pwd, int &flag){
    cout<<"——————————————————"<<endl;
    cout<<"账户昵称输入quit可返回主界面"<<endl;
    cout<<"——————————————————"<<endl;
    cout<<"请输入账户昵称："<<endl;
    getline(cin, name);
    if(name=="quit"){
        flag = 2;
        return ;
    }
    cout<<"——————————————————"<<endl;
    cout<<"请输入账户密码："<<endl;
    getline(cin, pwd);
    if(flag==1){
        flag = isInCSV(name, this->user_path);
    }else{flag = isInCSV(name, pwd, this->user_path);}
    return ;
}
// 判断昵称是否在数据文件里，存在返回1，否则返回0
// 数据文件不存在则返回-1
// 此为注册时所调用
int Game::isInCSV(string temp_name, string path){
    string line;
    string temp;
    int flag;
    ifstream infile(path, ios::in|ios::app);

    flag = isIFE(infile);
    // 若文件不空则按行读取已有账户数据比较用户名
    if(flag==-1){return -1;}
    if(flag>0){
        infile.seekg(0, ios::beg);
        while(getline(infile, line)){
            istringstream sin(line);    // 将整行字符串line读入到字符串流sin中
            getline(sin, temp, ',');    // 将字符串流sin中第一个逗号前的字符读入temp中
            getline(sin, temp, ',');    // 将字符串流sin中第二个逗号前的字符读入temp中
            if(temp_name==temp){
                cout<<"！！！！！~~账户昵称已被使用！请重新设置~~！！！！！"<<endl;
                infile.close();
                return 1;
            }
            getline(sin, temp);         // 将字符串流sin中剩余部分字符读入temp中
        }
    }
    infile.close();
    return 0;
}
// 判断账号是否在数据文件里，存在返回1，否则返回0
// 数据文件不存在则返回-1
// 此为登录时所调用
int Game::isInCSV(string &temp_name, string &temp_pwd, string path){
    int CEflag;
    int experience;
    int level;
    int nop;
    int last_try = 4;
    int flag;
    string name;
    string pwd;
    string line;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    flag = isIFE(infile);
    // 若文件不空则按行读取已有账户数据比较用户名
    if(flag==-1){return -1;}
    if(flag>0){
        while(last_try){
            infile.seekg(0, ios::beg);
            while(getline(infile, line)){
                istringstream sin(line);    // 将整行字符串line读入到字符串流sin中
                getline(sin, temp, ',');    // 将字符串流sin中第一个逗号前的字符读入temp中
                getline(sin, name, ',');    // 将字符串流sin中第二个逗号前的字符读入name中
                if(temp_name==name){
                    CEflag = atoi(temp.c_str());
                    getline(sin, pwd, ',');
                    if(temp_pwd!=pwd){
                        break;
                    }else{
                    getline(sin, temp, ',');
                    experience = atoi(temp.c_str());
                    getline(sin, temp, ',');
                    level = atoi(temp.c_str());
                    getline(sin, temp);
                    nop = atoi(temp.c_str());
                    cout<<"—————登录成功！—————"<<endl;
                    if(CEflag){
                        cout<<"欢迎您，出题者"<<name<<"！"<<endl;
                        cout<<"——————————————————"<<endl;
                        this->user = new Examiner(
                            temp_name, temp_pwd, CEflag, experience, level, nop);
                    }else{
                        cout<<"欢迎您，挑战者"<<name<<"！"<<endl;
                        cout<<"——————————————————"<<endl;
                        this->user = new Challenger(
                            temp_name, temp_pwd, CEflag, experience, level, nop);
                    }
                    infile.close();
                    return 1;
                    }
                }
                getline(sin, temp);         // 将字符串流sin中剩余部分字符读入temp中
            }
            cout<<"！！！！！~~密码或昵称不正确，请重新输入~~！！！！！"<<endl;
            cout<<"——————————————————"<<endl;
            cout<<"（还可以尝试"<<last_try<<"次登录）"<<endl;
            cout<<"——————————————————"<<endl;
            cout<<"请输入账户昵称："<<endl;
            getline(cin, temp_name);
            if(temp_name=="quit"){return 2;}
            cout<<"——————————————————"<<endl;
            cout<<"请输入账户密码："<<endl;
            getline(cin, temp_pwd);
            last_try--;
        }
    }else{
        cout<<"—————用户数据库中无数据！请注册后再登录—————"<<endl;
        infile.close();
        return 0;
    }
    cout<<"—————尝试次数过多！请重新登录—————"<<endl;
    infile.close();
    return 0;
}
// users.csv末端添加数据
// 每行结构：CEflag, name, pwd, experience, level, nop
void Game::wUcsv(int CEflag, string temp_name, string temp_pwd, string path){
    // 新用户数据写入users.csv并创建对应User类对象
    ofstream outfile(path, ios::out|ios::app);
    if(CEflag){
        this->user = new Examiner(temp_name, temp_pwd, CEflag);
    }else{this->user = new Challenger(temp_name, temp_pwd, CEflag);}
    outfile<<CEflag<<','
            <<temp_name<<','
            <<temp_pwd<<','
            <<0<<','
            <<1<<','
            <<0<<endl;
    outfile.close();
    return ;
}
// 读取csv最后一行内容来确定新用户排名
// 无文件返回-1，文件为空返回0
// 否则返回排名最后的用户的排名
int Game::rcsvl(string path){
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
                infile.seekg(0, ios::beg);
                break;
            }
            infile.seekg(-1, ios::cur);
        }
        infile>>R;
    }
    if(R>=0){infile.close();}
    return R;
}
// 新用户排名num追加入csv
// 输入参数flag为1说明是level排行，初始数据为1
void Game::wRcsv(int num, string name, string path, int flag){
    ofstream outfile(path, ios::out|ios::app);
    outfile<<num<<','
            <<name<<','
            <<flag<<endl;
    outfile.close();
    return ;
}
// 读取csv并输出所有数据
// 文件不存在则返回-1，否则返回0
int Game::PAR(string path){
    int flag;
    string line;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    flag = isIFE(infile);
    if(flag==-1){
        return -1;
    }else if(flag==0){
        cout<<"—————排行榜上暂时没有数据！快注册上榜吧—————"<<endl;
    }else{
        infile.seekg(0, ios::beg);
        while(getline(infile, line)){
            istringstream sin(line);
            getline(sin, temp, ',');
            cout<<temp<<"  ";
            getline(sin, temp, ',');
            cout<<temp<<"  ";
            getline(sin, temp);
            cout<<temp<<endl;
        }
    }
    infile.close();
    return 0;
}
// 获取某个用户的具体排名
// 文件不存在返回-1，找不到返回0
int Game::getRank(string temp_name, string path){
    int flag;
    string line;
    string name;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    flag = isIFE(infile);
    if(flag==-1){
        return -1;
    }else if(flag==0){
        infile.close();
        return 0;
    }else{
        infile.seekg(0, ios::beg);
        while(getline(infile, line)){
            istringstream sin(line);
            getline(sin, temp, ',');
            getline(sin, name, ',');
            if(temp_name==name){
                flag = atoi(temp.c_str());
                infile.close();
                return flag;
            }else{getline(sin, temp);}
        }
        infile.close();
        return 0;
    }
    
}
// 判断文件是否存在或为空
// 文件不存在则关闭文件并返回-1
// 文件为空返回0，否则返回1
int Game::isIFE(ifstream &infile){
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
// 欢迎用户登录
// 返回-1退出登录
// 返回0查看自身属性
// 返回1查看他人属性
// 返回2修改昵称或密码
// 返回3实现特有功能
int Game::welcomeUser(int CEflag){
    string temp;
    int flag = 0;
    if(CEflag){
        cout<<"************************************"<<endl;
        cout<<"欢迎回来！出题者"
            <<this->user->get_name()
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
            <<this->user->get_name()
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
