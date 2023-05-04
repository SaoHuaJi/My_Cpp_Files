# include "LocalAccount.h"

// 获取用户昵称
string User::get_name(){return this->name;}
// 设置用户新昵称
void User::set_name(string new_name){this->name = new_name;}
// 获取用户密码
string User::get_pwd(){return this->pwd;}
// 设置用户新密码
void User::set_pwd(string new_pwd){this->pwd = new_pwd;}
// 获取用户出题者判断标志
int User::get_CEflag(){return this->CEflag;}
// 设置用户出题者判断标志
void User::set_CEflag(int flag){this->CEflag = flag;}
// 获取用户等级
int User::get_level(){return this->level;}
// 设置用户等级
void User::set_level(int new_level){this->level = new_level;}
// 等级+1
void User::level_up(){this->level++;}
// 获取用户经验
int User::get_experience(){return this->experience;}
// 经验+n
void User::exp_up(int n){
    this->experience += n;
    while(1){
        int max_exp = (int)(exp(this->level));
        if(this->experience>=max_exp){
            level_up();
        }else{return ;}
    }
}
// 获取用户排名
// 文件出错返回-1，否则返回排名
int User::getRank(string path){
    int flag;
    string line;
    string temp_name = this->get_name();
    string name;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    if(!infile){
        cout<<"文件读取失败！请检查数据文件是否正确。"<<endl;
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
    cout<<"该用户无排名数据！请检查数据文件是否正确。"<<endl;
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
        cout<<"文件读取失败！请检查数据文件是否正确。"<<endl;
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
            cout<<"昵称已存在，请重新选择昵称！"<<endl;
            infile.close();
            return 0;
        }
        getline(sin, temp);
    }
    cout<<"昵称可用，新昵称设置完毕！"<<endl;
    infile.close();
    return 1;
}
// 修改用户昵称/密码
void User::SetValue(){
    int flag;
    string temp;
    while(1){
        cout<<"输入-1返回上一个界面"<<endl;
        cout<<"输入0修改昵称"<<endl;
        cout<<"输入1修改密码"<<endl;
        cin>>temp;
        if(temp=="-1"){
            return ;
        }else if(temp=="0"){
            cout<<"请输入新昵称："<<endl;
            cin>>temp;
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
            cout<<"请输入新密码："<<endl;
            cin>>temp;
            this->ModifyUCSV(flag, temp, "./Datas/users.csv");
            this->set_pwd(temp);
            cout<<"新密码设置完毕！"<<endl;
            system("pause");
            return ;
        }else{cout<<"请输入有效数字。"<<endl;}
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
        values.push_back(num);
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
        cout<<"输入-1返回上一个界面"<<endl;
        cout<<"输入0留在上一关刷经验"<<endl;
        cout<<"输入1继续闯关"<<endl;
        cin>>temp;
        if(temp=="-1"){
            break;
        }else if(temp=="0"){
            flag = gaming(0);
        }else if(temp=="1"){
            flag = gaming(1);
        }else{cout<<"请输入有效数字！"<<endl;}
    }
    UpdateRank(this->get_level(), "./Datas/CRL.csv");
    UpdateRank(this->get_experience(), "./Datas/CRE.csv");
    UpdateRank(this->plevel, "./Datas/CRP.csv");
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
    cout<<"闯关时输入-1即可退出闯关，准备好了吗？"<<endl;
    system("pause");
    string word;
    string inword;
    clock_t start, end;
    int len;
    int flag = 0;
    int n = (int)(log2(this->get_level()+1));
    for(int j=1; j<=n; j++){
        len = (int)(log2(this->get_experience()));
        cout<<"接下来是这关的第"<<j<<"个单词"<<endl;
        cout<<"请注意下面显示的单词："<<endl;
        word = getRandomWord(len, this->dict->getroot());
        len = word.size();
        this->dict->temp_show(word, 20*len);
        start = clock();
        for(int i=3; i>0; i--){
            cout<<"请输入刚才显示的单词："<<endl;
            getline(cin, inword);
            if(inword=="-1"){
                cout<<"关卡已退出..."<<endl;
                system("pause");
                flag = -1;
                break;
            }else if(inword==word){
                cout<<"正解！干得漂亮！"<<endl;
                exp_up(1);
                flag = 1;
                break;
            }else{
                cout<<"哎呀~记错了呀~"<<endl;
                cout<<"您还有"<<i<<"次机会Orz"<<endl;
                flag = 0;
            }
        }
        end = clock();
        if(flag==0){
            cout<<"不要灰心，再接再厉吧！"<<endl;
            return 0;
        }
    }
    ptime += (int)(double(end-start)/CLOCKS_PER_SEC);
    cout<<"恭喜通关！你太棒了！"<<endl;
    return 1;
}
// mode为0循环挑战上一关
// mode为1挑战新关卡
// 关卡出错返回-1
// 关卡退出返回0
int Challenger::gaming(int mode){
    if((mode==0)&&(this->plevel==0)){
        cout<<"您尚未通过任何关卡，请挑战新关卡吧！"<<endl;
        return -1;
    }
    if(this->dict->getroot()->child_num()==0){
        cout<<"词典数据为空！无法挑战，正在退出..."<<endl;
        return -1;
    }else{
        int flag = 1;
        int ptime = 0;
        clock_t start, end;
        if(mode){
            while(flag){
                flag = this_level(ptime);
                if(flag){
                    this->plevel++;
                    // 通关新关卡额外经验奖励由闯关时间和关卡数共同决定
                    exp((int)(this->plevel/10+20/ptime));
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
    cout<<"以下是您的属性面板"<<endl;
    cout<<"昵称：         "<<this->get_name()<<endl;
    cout<<"密码：         "<<this->get_pwd()<<endl;
    cout<<"等级：         "<<this->get_level()<<endl;
    cout<<"经验：         "<<this->get_experience()<<endl;
    cout<<"通过关卡数：    "<<plevel<<endl;
    cout<<"等级排行榜排名："<<CRL<<endl;
    cout<<"经验排行榜排名："<<CRE<<endl;
    cout<<"关卡排行榜排名："<<CRP<<endl;
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
            temp = to_string(this->plevel);
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
    while(1){
        cout<<"来为题库增添新词吧！"<<endl;
        cout<<"（输入-1可以返回上一个界面）"<<endl;
        cout<<"（输入0可以查看当前词库）"<<endl;
        getline(cin, input);    // 直接cin无法接收空格
        if(input=="-1"){break;}
        if(input=="0"){
            flag = atoi(input.c_str());
            this->dict->show_all_words(this->dict->getroot(), flag);
            system("pause");
        }
        flag = this->dict->isLegal(input);
        if(flag){flag = this->dict->Insert(input);}
        if(flag==1){
            this->num++;
            exp_up(10);
        }else if(flag==0){
            exp_up(1);
        }else{cout<<"这句话是咋被看到的？"<<endl;}
    }
    this->dict->wDict("./Datas/Words.dat");
    UpdateRank(this->get_level(), "./Datas/ERL.csv");
    UpdateRank(this->num, "./Datas/ERN.csv");
    return ;
}
// 展示出题者属性面板
void Examiner::ShowValue(){
    cout<<"以下是您的属性面板"<<endl;
    cout<<"昵称：         "<<this->get_name()<<endl;
    cout<<"密码：         "<<this->get_pwd()<<endl;
    cout<<"等级：         "<<this->get_level()<<endl;
    cout<<"经验：         "<<this->get_experience()<<endl;
    cout<<"累计出题数：    "<<num<<endl;
    cout<<"等级排行榜排名："<<ERL<<endl;
    cout<<"题数排行榜排名："<<ERN<<endl;
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
            temp = to_string(this->num);
            outstr += temp+"\n";
            getline(sin, temp);
        }else{outstr += line+"\n";}
    }
    infile.close();
    ofstream outfile("./Datas/users.csv", ios::out|ios::trunc);
    cout<<"The outstr is\n"<<outstr<<endl;
    outfile.write(outstr.c_str(), outstr.size());
    outfile.close();
    return ;
}
