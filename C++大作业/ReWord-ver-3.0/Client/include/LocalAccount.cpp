# include "LocalAccount.h"


// 经验+n
void User::exp_up(int n){
    this->experience += n;
    cout<<"==========经验+"<<n<<"=========="<<endl;
    while(1){
        int max_exp = (int)(exp(this->level));
        if(this->experience>=max_exp){
            level_up();
        }else{return ;}
    }
}
// 获取用户排名
// 文件出错返回-1，否则返回排名
int User::getRank(string path, string temp_name){
    int flag;
    string line;
    string name;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    if(!infile){
        cout<<"！！！！！~~排名数据文件不存在！请检查文件是否正确~~！！！！！"<<endl;
        infile.close();
        infile.clear();
        return -1;
    }
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
    cout<<"！！！！！~~该用户无排名数据！请检查文件或输入是否正确~~！！！！！"<<endl;
    return -1;
}
// 检查新昵称是否可用
// 文件不存在返回-1
// 新昵称不可用返回0
// 新昵称可用返回1
int User::check_name(string new_name, string path){
    int flag;
    string line;
    string name;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    if(!infile){
        cout<<"！！！！！~~用户数据文件读取失败！请检查文件是否正确~~！！！！！"<<endl;
        infile.close();
        infile.clear();
        return -1;
    }
    infile.seekg(0, ios::beg);
    while(getline(infile, line)){
        istringstream sin(line);
        getline(sin, temp, ',');
        getline(sin, name, ',');
        if(new_name==name){
            cout<<"！！！！！~~昵称已存在！请重新选择昵称~~！！！！！"<<endl;
            infile.close();
            return 0;
        }
        getline(sin, temp);
    }
    cout<<"==========昵称可用！新昵称设置完毕=========="<<endl;
    infile.close();
    return 1;
}
// 根据昵称查看对应用户信息
void User::check_user(string name, string path){
    ifstream infile(path, ios::in);
    if(!infile){
        cout<<"！！！！！~~用户数据文件读取失败！请检查文件是否正确~~！！！！！"<<endl;
        return ;
    }
    string line;
    string CEflag;
    string temp;
    while(getline(infile, line)){
        istringstream sin(line);
        getline(sin, CEflag, ',');
        getline(sin, temp, ',');
        if(temp==name){
            cout<<"####################################"<<endl;
            cout<<"#  以下是所查询用户的属性面板"<<endl;
            cout<<"####################################"<<endl;
            cout<<"#  昵称：\t\t"<<name<<endl;
            getline(sin, temp, ',');    // 不应该显示其他用户的密码
            getline(sin, temp, ',');
            cout<<"#  经验：\t\t"<<temp<<endl;
            getline(sin, temp, ',');
            cout<<"#  等级：\t\t"<<temp<<endl;
            getline(sin, temp);
            if(CEflag=="0"){
                cout<<"#  通过关卡数：\t\t"<<temp<<endl;
                cout<<"#  等级排行榜排名：\t"<<getRank("./Datas/CRL.csv", name)<<endl;
                cout<<"#  经验排行榜排名：\t"<<getRank("./Datas/CRE.csv", name)<<endl;
                cout<<"#  关卡排行榜排名：\t"<<getRank("./Datas/CRP.csv", name)<<endl;
            }else{
                cout<<"#  累计出题数：\t\t"<<temp<<endl;
                cout<<"#  等级排行榜排名：\t"<<getRank("./Datas/ERL.csv", name)<<endl;
                cout<<"#  题数排行榜排名：\t"<<getRank("./Datas/ERN.csv", name)<<endl;
            }
            cout<<"####################################"<<endl;
            system("pause");
            infile.close();
            return ;
        }else{getline(sin, temp);}
    }
    cout<<"！！！！！~~所查询用户不存在！请检查输入~~！！！！！"<<endl;
    infile.close();
    return ;
}
// 修改用户昵称/密码
void User::SetValue(){
    int flag;
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
            cout<<"——————————————————"<<endl;
            cout<<"请输入新昵称："<<endl;
            cout<<"——————————————————"<<endl;
            getline(cin, temp);
            flag = check_name(temp);
            if(flag==0){continue;}
            if(flag==1){
                flag--;
                this->ModifyUCSV(flag, temp, "./Datas/users.csv");
                if(this->CEflag){
                    this->ModifyRCSV(temp, "./Datas/ERL.csv");
                    this->ModifyRCSV(temp, "./Datas/ERN.csv");
                }else{
                    this->ModifyRCSV(temp, "./Datas/CRL.csv");
                    this->ModifyRCSV(temp, "./Datas/CRP.csv");
                    this->ModifyRCSV(temp, "./Datas/CRE.csv");
                }
                this->set_name(temp);
                system("pause");
            }
            return ;
        }else if(temp=="1"){
            flag = atoi(temp.c_str());
            cout<<"——————————————————"<<endl;
            cout<<"请输入新密码："<<endl;
            cout<<"——————————————————"<<endl;
            getline(cin, temp);
            this->ModifyUCSV(flag, temp, "./Datas/users.csv");
            this->set_pwd(temp);
            cout<<"==========新密码设置完毕！=========="<<endl;
            system("pause");
            return ;
        }else{cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;}
    }
}
// 修改users.csv中的昵称或密码为str
// flag为1修改密码，flag为0修改昵称
// C++以流读取文件，无法直接修改某一行数据
// 故数据存入string对应修改后再覆盖文件
void User::ModifyUCSV(int &flag, string &str, string path){
    string outstr = "";
    string name = this->name;
    string temp_name;
    string temp;
    string line;
    string CEflag;
    ifstream infile(path, ios::in);
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
    ofstream outfile(path, ios::out|ios::trunc);
    outfile.write(outstr.c_str(), outstr.size());
    outfile.close();
    return ;
}
// 修改排名文件中的昵称为str
void User::ModifyRCSV(string &str, string path){
    string outstr = "";
    string name = this->name;
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
// 更新用户排名文件
void User::UpdateRank(int new_num, string path){
    string temp;
    int tempn;
    string name = this->get_name();
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

// 挑战者闯关
void Challenger::Play(){
    string temp;
    int flag;
    this->dict->rDict(this->dict->getpath());
    while(1){
        cout<<"************************************"<<endl;
        cout<<"*  输入-1返回上一个界面"<<endl;
        cout<<"*  输入0留在上一关刷经验"<<endl;
        cout<<"*  输入1继续闯关"<<endl;
        cout<<"************************************"<<endl;
        cin>>temp;
        cin.ignore(MAXSIZE, '\n');
        if(temp=="-1"){
            break;
        }else if((temp=="0")||(temp=="1")){
            flag = atoi(temp.c_str());
            flag = gaming(flag);
        }else{cout<<"！！！！！~~请输入有效数字~~！！！！！"<<endl;}
    }
    return ;
}
// 获取长度不超过n的一个随机单词
string Challenger::getRandomWord(int len, TrieNode* root){
    random_device seed;     // 从硬件获取真随机数种子
    mt19937 gen(seed());    // 用标准梅森旋转算法获得unsigned int随机数gen
    uniform_int_distribution<> distrib(1, 1000);    //  随机数分散到[1, 1000]
    string outstr = "";
    char ch;
    int cnum;
    int temp;
    int judge;
    int flag = 0;
    int i, j;
    while(1){
        // 单词最长长度为len
        for(i=0; i<len; i++){
            cnum = root->child_num();   // 获取当前结点的子结点数
            // 叶子结点必然是单词结尾
            if(cnum==0){
                return outstr;
            }else{
                temp = distrib(gen);
                // 可作单词结尾的结点概率成为结尾进行输出
                if(root->isE){
                    judge = (int)(1000*i/(len-1));
                    if(temp<judge){
                        flag++;
                        return outstr;
                    }
                }
                temp = temp%cnum+1; // 随机数分散到子结点数范围内
                for(j=0; j<54; j++){
                    if(root->next[j]!=NULL){
                        temp--;     // temp减至0时即为随机分支
                        if(temp==0){
                            if(j==52){
                                ch = ' ';
                            }else if(j==53){
                                ch = '-';
                            }else if(j<26){
                                ch = 'a'+j;
                            }else{ch = 'A'+j-26;}
                            outstr = outstr+ch;
                            root = root->next[j];
                            break;
                        }
                    }
                }
            }
        }
        // 随机选到的单词长度比len大
        // 为避免不存在比len长度小的单词，给予一定跳出循环概率
        // 未跳出则outstr清空且root还原重新选词
        if(flag==0){
            temp = distrib(gen);
            judge =100;
            if(temp<judge){
                while(1){
                    cnum = root->child_num();
                    // 超长单词遇到单词结尾就输出
                    if(root->isE){
                        return outstr;
                    }else{
                        temp = temp%cnum+1;
                        for(j=0; j<54; j++){
                            if(root->next[j]!=NULL){
                                temp--;     // temp减至0时即为随机分支
                                if(temp==0){
                                    if(j==52){
                                        ch = ' ';
                                    }else if(j==53){
                                        ch = '-';
                                    }else if(j<26){
                                        ch = 'a'+j;
                                    }else{ch = 'A'+j-26;}
                                    outstr = outstr+ch;
                                    root = root->next[j];
                                    break;
                                }
                            }
                        }
                    }
                }
            }else{
                outstr = "";
                root = this->dict->getroot();
            }
        }
    }
}
// 挑战关卡
// 关卡退出返回0
// 关卡通过返回1
int Challenger::this_level(int &ptime){
    cout<<"——————————————————"<<endl;
    cout<<"闯关时输入-1即可退出闯关，准备好了吗？"<<endl;
    system("pause");
    cout<<"——————————————————"<<endl;
    string word;
    string inword;
    clock_t start, end;
    int len;
    int n = (int)(log2(this->get_level()+1));
    for(int j=1; j<=n; j++){
        len = (int)(log2(this->get_experience()));
        cout<<"—————接下来是这关的第"<<j<<"个单词—————"<<endl;
        cout<<"—————请注意下面显示的单词：—————"<<endl;
        word = getRandomWord(len, this->dict->getroot());
        len = word.size();
        this->dict->temp_show(word, 500*len);
        start = clock();
        for(int i=3; i>0; i--){
            cout<<"—————请输入刚才显示的单词：—————"<<endl;
            getline(cin, inword);
            if(inword=="-1"){
                cout<<"—————关卡已退出—————"<<endl;
                system("pause");
                return 0;
            }else if(inword==word){
                cout<<"——————正解！干得漂亮！—————"<<endl;
                exp_up(1);
                end = clock();
                ptime += (int)(double(end-start)/CLOCKS_PER_SEC);
                cout<<"—————恭喜通关！你太棒了！—————"<<endl;
                return 1;
            }else{
                cout<<"—————哎呀~记错了呀~—————"<<endl;
                cout<<"—————您还有"<<i<<"次机会Orz—————"<<endl;
            }
        }
        cout<<"—————不要灰心，再接再厉吧！—————"<<endl;
        return 0;
    }
    return 0;
}
// mode为0循环挑战上一关
// mode为1挑战新关卡
// 关卡出错返回-1
// 关卡退出返回0
int Challenger::gaming(int mode){
    if((mode==0)&&(this->get_nop()==0)){
        cout<<"！！！！！~~您尚未通过任何关卡！请先挑战新关卡~~！！！！！"<<endl;
        return -1;
    }
    if(this->dict->getroot()->child_num()==0){
        cout<<"！！！！！~~词典数据为空！无法挑战，正在退出~~！！！！！"<<endl;
        return -1;
    }else{
        int flag = 1;
        int ptime = 0;
        if(mode){
            while(flag){
                flag = this_level(ptime);
                if(flag){
                    this->nop_up();
                    // 通关新关卡额外经验奖励由闯关时间和关卡数共同决定
                    exp_up((int)(this->get_nop()/2+20/ptime));
                }else{break;}
            }
        }else{
            while(flag){flag = this_level(ptime);}
        }
        return 0;
    }
}
// 展示挑战者属性面板
void Challenger::ShowValue(){
    cout<<" (~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~("<<endl;
    cout<<" |         属    性    面    板         |"<<endl;
    cout<<" |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|"<<endl;
    cout<<" |  昵称：\t\t"<<this->get_name();
    for(int k=0; k<this->get_name().size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  密码：\t\t"<<this->get_pwd();
    for(int k=0; k<this->get_pwd().size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  等级：\t\t"<<this->get_level();
    for(int k=0; k<to_string(this->get_level()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  经验：\t\t"<<this->get_experience();
    for(int k=0; k<to_string(this->get_experience()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  通过关卡数：\t"<<this->get_nop();
    for(int k=0; k<to_string(this->get_nop()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  等级排行榜排名：\t"<<CRL;
    for(int k=0; k<to_string(this->CRL).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  经验排行榜排名：\t"<<CRE;
    for(int k=0; k<to_string(this->CRE).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" )  通关排行榜排名：\t"<<CRP;
    for(int k=0; k<to_string(this->CRP).size(); k++){cout<<"\b";}
    cout<<"\t\t)"<<endl;
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    system("pause");
    return ;
}
// 更新挑战者等级等数据文件
void Challenger::UpdateUser(){
    string outstr = "";
    string name = this->get_name();
    string temp_name;
    string temp;
    string line;
    ifstream infile("./Datas/users.csv", ios::in);
    while(getline(infile, line)){
        istringstream sin(line);
        getline(sin, temp, ',');
        getline(sin, temp_name, ',');
        if(temp_name==name){
            outstr += temp+",";
            outstr += name+",";
            getline(sin, temp, ',');
            outstr += temp+",";
            temp = to_string(this->get_experience());
            outstr += temp+",";
            temp = to_string(this->get_level());
            outstr += temp+",";
            temp = to_string(this->get_nop());
            outstr += temp+"\n";
            getline(sin, temp);
        }else{outstr += line+"\n";}
    }
    infile.close();
    ofstream outfile("./Datas/users.csv", ios::out|ios::trunc);
    outfile.write(outstr.c_str(), outstr.size());
    outfile.close();
    return ;
}

// 出题者出题
void Examiner::Play(){
    string input;
    int flag;

    this->dict->rDict(this->dict->getpath());
    cin.ignore(MAXSIZE, '\n');
    while(1){
        cout<<"——————————————————"<<endl;
        cout<<"来为题库增添新词吧！"<<endl;
        cout<<"——————————————————"<<endl;
        cout<<"输入-1可以返回上一个界面"<<endl;
        cout<<"输入0可以查看当前词库"<<endl;
        cout<<"——————————————————"<<endl;
        getline(cin, input);    // 直接cin无法接收空格
        if(input=="-1"){break;}
        if(input=="0"){
            flag = atoi(input.c_str());
            cout<<"——————————————————"<<endl;
            cout<<"当前词库中所有单词如下"<<endl;
            cout<<"——————————————————"<<endl;
            this->dict->show_all_words(this->dict->getroot(), flag);
            cout<<"——————————————————"<<endl;
            system("pause");
            continue;
        }
        flag = this->dict->isLegal(input);
        if(flag==1){flag = this->dict->Insert(input);}
        if(flag==1){
            this->nop_up();
            exp_up(10);
        }else if(flag==0){
            exp_up(1);
        }else{continue;}
    }
    this->dict->wDict("./Datas/Words.dat");
    return ;
}
// 展示出题者属性面板
void Examiner::ShowValue(){
    cout<<" (~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~("<<endl;
    cout<<" |         属    性    面    板         |"<<endl;
    cout<<" |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|"<<endl;
    cout<<" |  昵称：\t\t"<<this->get_name();
    for(int k=0; k<this->get_name().size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  密码：\t\t"<<this->get_pwd();
    for(int k=0; k<this->get_pwd().size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  等级：\t\t"<<this->get_level();
    for(int k=0; k<to_string(this->get_level()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  经验：\t\t"<<this->get_experience();
    for(int k=0; k<to_string(this->get_experience()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  累计出题数：\t"<<this->get_nop();
    for(int k=0; k<to_string(this->get_nop()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  等级排行榜排名：\t"<<ERL;
    for(int k=0; k<to_string(this->ERL).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" )  题数排行榜排名：\t"<<ERN;
    for(int k=0; k<to_string(this->ERN).size(); k++){cout<<"\b";}
    cout<<"\t\t)"<<endl;
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    system("pause");
    return ;
}
// 更新出题者等级等数据文件
void Examiner::UpdateUser(){
    string outstr = "";
    string name = this->get_name();
    string temp_name;
    string temp;
    string line;
    ifstream infile("./Datas/users.csv", ios::in);
    while(getline(infile, line)){
        istringstream sin(line);
        getline(sin, temp, ',');
        getline(sin, temp_name, ',');
        if(temp_name==name){
            outstr += temp+",";
            outstr += name+",";
            getline(sin, temp, ',');
            outstr += temp+",";
            temp = to_string(this->get_experience());
            outstr += temp+",";
            temp = to_string(this->get_level());
            outstr += temp+",";
            temp = to_string(this->get_nop());
            outstr += temp+"\n";
            getline(sin, temp);
        }else{outstr += line+"\n";}
    }
    infile.close();
    ofstream outfile("./Datas/users.csv", ios::out|ios::trunc);
    outfile.write(outstr.c_str(), outstr.size());
    outfile.close();
    return ;
}
