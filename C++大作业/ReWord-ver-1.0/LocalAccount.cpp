# include "LocalAccount.h"

// ��ȡ�û��ǳ�
string User::get_name(){return this->name;}
// �����û����ǳ�
void User::set_name(string new_name){this->name = new_name;}
// ��ȡ�û�����
string User::get_pwd(){return this->pwd;}
// �����û�������
void User::set_pwd(string new_pwd){this->pwd = new_pwd;}
// ��ȡ�û��������жϱ�־
int User::get_CEflag(){return this->CEflag;}
// �����û��������жϱ�־
void User::set_CEflag(int flag){this->CEflag = flag;}
// ��ȡ�û��ȼ�
int User::get_level(){return this->level;}
// �����û��ȼ�
void User::set_level(int new_level){this->level = new_level;}
// �ȼ�+1
void User::level_up(){this->level++;}
// ��ȡ�û�����
int User::get_experience(){return this->experience;}
// ����+n
void User::exp_up(int n){
    this->experience += n;
    while(1){
        int max_exp = (int)(exp(this->level));
        if(this->experience>=max_exp){
            level_up();
        }else{return ;}
    }
}
// ��ȡ�û�����
// �ļ�������-1�����򷵻�����
int User::getRank(string path){
    int flag;
    string line;
    string temp_name = this->get_name();
    string name;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    if(!infile){
        cout<<"�ļ���ȡʧ�ܣ����������ļ��Ƿ���ȷ��"<<endl;
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
    cout<<"���û����������ݣ����������ļ��Ƿ���ȷ��"<<endl;
    return -1;
}
// ������ǳ��Ƿ����
// �ļ������ڷ���-1
// ���ǳƲ����÷���0
// ���ǳƿ��÷���1
int User::check_name(string new_name, string path){
    int flag;
    string line;
    string name;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    if(!infile){
        cout<<"�ļ���ȡʧ�ܣ����������ļ��Ƿ���ȷ��"<<endl;
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
            cout<<"�ǳ��Ѵ��ڣ�������ѡ���ǳƣ�"<<endl;
            infile.close();
            return 0;
        }
        getline(sin, temp);
    }
    cout<<"�ǳƿ��ã����ǳ�������ϣ�"<<endl;
    infile.close();
    return 1;
}
// �޸��û��ǳ�/����
void User::SetValue(){
    int flag;
    string temp;
    while(1){
        cout<<"����-1������һ������"<<endl;
        cout<<"����0�޸��ǳ�"<<endl;
        cout<<"����1�޸�����"<<endl;
        cin>>temp;
        if(temp=="-1"){
            return ;
        }else if(temp=="0"){
            cout<<"���������ǳƣ�"<<endl;
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
            cout<<"�����������룺"<<endl;
            cin>>temp;
            this->ModifyUCSV(flag, temp, "./Datas/users.csv");
            this->set_pwd(temp);
            cout<<"������������ϣ�"<<endl;
            system("pause");
            return ;
        }else{cout<<"��������Ч���֡�"<<endl;}
    }
}
// �޸�users.csv�е��ǳƻ�����Ϊstr
// flagΪ1�޸����룬flagΪ0�޸��ǳ�
// C++������ȡ�ļ����޷�ֱ���޸�ĳһ������
// �����ݴ���string��Ӧ�޸ĺ��ٸ����ļ�
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
// �޸������ļ��е��ǳ�Ϊstr
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
// �����û������ļ�
void User::UpdateRank(int new_num, string path){
    string temp;
    int tempn;
    string name = this->get_name();
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
        values.push_back(num);
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

// ��ս�ߴ���
void Challenger::Play(){
    string temp;
    int flag;
    this->dict->rDict(this->dict->getpath());
    while(1){
        cout<<"����-1������һ������"<<endl;
        cout<<"����0������һ��ˢ����"<<endl;
        cout<<"����1��������"<<endl;
        cin>>temp;
        if(temp=="-1"){
            break;
        }else if(temp=="0"){
            flag = gaming(0);
        }else if(temp=="1"){
            flag = gaming(1);
        }else{cout<<"��������Ч���֣�"<<endl;}
    }
    UpdateRank(this->get_level(), "./Datas/CRL.csv");
    UpdateRank(this->get_experience(), "./Datas/CRE.csv");
    UpdateRank(this->plevel, "./Datas/CRP.csv");
    return ;
}
// ��ȡ���Ȳ�����n��һ���������
string Challenger::getRandomWord(int len, TrieNode* root){
    random_device seed;     // ��Ӳ����ȡ�����������
    mt19937 gen(seed());    // �ñ�׼÷ɭ��ת�㷨���unsigned int�����gen
    uniform_int_distribution<> distrib(1, 1000);    //  �������ɢ��[1, 1000]
    string outstr = "";
    char ch;
    int cnum;
    int temp;
    int judge;
    int flag = 0;
    int i, j;
    while(1){
        // ���������Ϊlen
        for(i=0; i<len; i++){
            cnum = root->child_num();   // ��ȡ��ǰ�����ӽ����
            // Ҷ�ӽ���Ȼ�ǵ��ʽ�β
            if(cnum==0){
                return outstr;
            }else{
                temp = distrib(gen);
                // �������ʽ�β�Ľ����ʳ�Ϊ��β�������
                if(root->isE){
                    judge = (int)(1000*i/(len-1));
                    if(temp<judge){
                        flag++;
                        return outstr;
                    }
                }
                temp = temp%cnum+1; // �������ɢ���ӽ������Χ��
                for(j=0; j<54; j++){
                    if(root->next[j]!=NULL){
                        temp--;     // temp����0ʱ��Ϊ�����֧
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
        // ���ѡ���ĵ��ʳ��ȱ�len��
        // Ϊ���ⲻ���ڱ�len����С�ĵ��ʣ�����һ������ѭ������
        // δ������outstr�����root��ԭ����ѡ��
        if(flag==0){
            temp = distrib(gen);
            judge =100;
            if(temp<judge){
                while(1){
                    cnum = root->child_num();
                    // ���������������ʽ�β�����
                    if(root->isE){
                        return outstr;
                    }else{
                        temp = temp%cnum+1;
                        for(j=0; j<54; j++){
                            if(root->next[j]!=NULL){
                                temp--;     // temp����0ʱ��Ϊ�����֧
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
// ��ս�ؿ�
// �ؿ��˳�����0
// �ؿ�ͨ������1
int Challenger::this_level(int &ptime){
    cout<<"����ʱ����-1�����˳����أ�׼��������"<<endl;
    system("pause");
    string word;
    string inword;
    clock_t start, end;
    int len;
    int flag = 0;
    int n = (int)(log2(this->get_level()+1));
    for(int j=1; j<=n; j++){
        len = (int)(log2(this->get_experience()));
        cout<<"����������صĵ�"<<j<<"������"<<endl;
        cout<<"��ע��������ʾ�ĵ��ʣ�"<<endl;
        word = getRandomWord(len, this->dict->getroot());
        len = word.size();
        this->dict->temp_show(word, 20*len);
        start = clock();
        for(int i=3; i>0; i--){
            cout<<"������ղ���ʾ�ĵ��ʣ�"<<endl;
            getline(cin, inword);
            if(inword=="-1"){
                cout<<"�ؿ����˳�..."<<endl;
                system("pause");
                flag = -1;
                break;
            }else if(inword==word){
                cout<<"���⣡�ɵ�Ư����"<<endl;
                exp_up(1);
                flag = 1;
                break;
            }else{
                cout<<"��ѽ~�Ǵ���ѽ~"<<endl;
                cout<<"������"<<i<<"�λ���Orz"<<endl;
                flag = 0;
            }
        }
        end = clock();
        if(flag==0){
            cout<<"��Ҫ���ģ��ٽ������ɣ�"<<endl;
            return 0;
        }
    }
    ptime += (int)(double(end-start)/CLOCKS_PER_SEC);
    cout<<"��ϲͨ�أ���̫���ˣ�"<<endl;
    return 1;
}
// modeΪ0ѭ����ս��һ��
// modeΪ1��ս�¹ؿ�
// �ؿ�������-1
// �ؿ��˳�����0
int Challenger::gaming(int mode){
    if((mode==0)&&(this->plevel==0)){
        cout<<"����δͨ���κιؿ�������ս�¹ؿ��ɣ�"<<endl;
        return -1;
    }
    if(this->dict->getroot()->child_num()==0){
        cout<<"�ʵ�����Ϊ�գ��޷���ս�������˳�..."<<endl;
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
                    // ͨ���¹ؿ����⾭�齱���ɴ���ʱ��͹ؿ�����ͬ����
                    exp((int)(this->plevel/10+20/ptime));
                }else{break;}
            }
        }else{
            while(flag){flag = this_level(ptime);}
        }
        return 0;
    }
}
// չʾ��ս���������
void Challenger::ShowValue(){
    cout<<"�����������������"<<endl;
    cout<<"�ǳƣ�         "<<this->get_name()<<endl;
    cout<<"���룺         "<<this->get_pwd()<<endl;
    cout<<"�ȼ���         "<<this->get_level()<<endl;
    cout<<"���飺         "<<this->get_experience()<<endl;
    cout<<"ͨ���ؿ�����    "<<plevel<<endl;
    cout<<"�ȼ����а�������"<<CRL<<endl;
    cout<<"�������а�������"<<CRE<<endl;
    cout<<"�ؿ����а�������"<<CRP<<endl;
    system("pause");
    return ;
}
// ������ս�ߵȼ��������ļ�
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

// �����߳���
void Examiner::Play(){
    string input;
    int flag;

    this->dict->rDict(this->dict->getpath());
    while(1){
        cout<<"��Ϊ��������´ʰɣ�"<<endl;
        cout<<"������-1���Է�����һ�����棩"<<endl;
        cout<<"������0���Բ鿴��ǰ�ʿ⣩"<<endl;
        getline(cin, input);    // ֱ��cin�޷����տո�
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
        }else{cout<<"��仰��զ�������ģ�"<<endl;}
    }
    this->dict->wDict("./Datas/Words.dat");
    UpdateRank(this->get_level(), "./Datas/ERL.csv");
    UpdateRank(this->num, "./Datas/ERN.csv");
    return ;
}
// չʾ�������������
void Examiner::ShowValue(){
    cout<<"�����������������"<<endl;
    cout<<"�ǳƣ�         "<<this->get_name()<<endl;
    cout<<"���룺         "<<this->get_pwd()<<endl;
    cout<<"�ȼ���         "<<this->get_level()<<endl;
    cout<<"���飺         "<<this->get_experience()<<endl;
    cout<<"�ۼƳ�������    "<<num<<endl;
    cout<<"�ȼ����а�������"<<ERL<<endl;
    cout<<"�������а�������"<<ERN<<endl;
    system("pause");
    return ;
}
// ���³����ߵȼ��������ļ�
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
