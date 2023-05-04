# include "LocalAccount.h"


// ����+n
void User::exp_up(int n){
    this->experience += n;
    cout<<"==========����+"<<n<<"=========="<<endl;
    while(1){
        int max_exp = (int)(exp(this->level));
        if(this->experience>=max_exp){
            level_up();
        }else{return ;}
    }
}
// ��ȡ�û�����
// �ļ�������-1�����򷵻�����
int User::getRank(string path, string temp_name){
    int flag;
    string line;
    string name;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    if(!infile){
        cout<<"����������~~���������ļ������ڣ������ļ��Ƿ���ȷ~~����������"<<endl;
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
    cout<<"����������~~���û����������ݣ������ļ��������Ƿ���ȷ~~����������"<<endl;
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
        cout<<"����������~~�û������ļ���ȡʧ�ܣ������ļ��Ƿ���ȷ~~����������"<<endl;
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
            cout<<"����������~~�ǳ��Ѵ��ڣ�������ѡ���ǳ�~~����������"<<endl;
            infile.close();
            return 0;
        }
        getline(sin, temp);
    }
    cout<<"==========�ǳƿ��ã����ǳ��������=========="<<endl;
    infile.close();
    return 1;
}
// �����ǳƲ鿴��Ӧ�û���Ϣ
void User::check_user(string name, string path){
    ifstream infile(path, ios::in);
    if(!infile){
        cout<<"����������~~�û������ļ���ȡʧ�ܣ������ļ��Ƿ���ȷ~~����������"<<endl;
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
            cout<<"#  ����������ѯ�û����������"<<endl;
            cout<<"####################################"<<endl;
            cout<<"#  �ǳƣ�\t\t"<<name<<endl;
            getline(sin, temp, ',');    // ��Ӧ����ʾ�����û�������
            getline(sin, temp, ',');
            cout<<"#  ���飺\t\t"<<temp<<endl;
            getline(sin, temp, ',');
            cout<<"#  �ȼ���\t\t"<<temp<<endl;
            getline(sin, temp);
            if(CEflag=="0"){
                cout<<"#  ͨ���ؿ�����\t\t"<<temp<<endl;
                cout<<"#  �ȼ����а�������\t"<<getRank("./Datas/CRL.csv", name)<<endl;
                cout<<"#  �������а�������\t"<<getRank("./Datas/CRE.csv", name)<<endl;
                cout<<"#  �ؿ����а�������\t"<<getRank("./Datas/CRP.csv", name)<<endl;
            }else{
                cout<<"#  �ۼƳ�������\t\t"<<temp<<endl;
                cout<<"#  �ȼ����а�������\t"<<getRank("./Datas/ERL.csv", name)<<endl;
                cout<<"#  �������а�������\t"<<getRank("./Datas/ERN.csv", name)<<endl;
            }
            cout<<"####################################"<<endl;
            system("pause");
            infile.close();
            return ;
        }else{getline(sin, temp);}
    }
    cout<<"����������~~����ѯ�û������ڣ���������~~����������"<<endl;
    infile.close();
    return ;
}
// �޸��û��ǳ�/����
void User::SetValue(){
    int flag;
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
            cout<<"������������������������������������"<<endl;
            cout<<"���������ǳƣ�"<<endl;
            cout<<"������������������������������������"<<endl;
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
            cout<<"������������������������������������"<<endl;
            cout<<"�����������룺"<<endl;
            cout<<"������������������������������������"<<endl;
            getline(cin, temp);
            this->ModifyUCSV(flag, temp, "./Datas/users.csv");
            this->set_pwd(temp);
            cout<<"==========������������ϣ�=========="<<endl;
            system("pause");
            return ;
        }else{cout<<"����������~~��������Ч����~~����������"<<endl;}
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

// ��ս�ߴ���
void Challenger::Play(){
    string temp;
    int flag;
    this->dict->rDict(this->dict->getpath());
    while(1){
        cout<<"************************************"<<endl;
        cout<<"*  ����-1������һ������"<<endl;
        cout<<"*  ����0������һ��ˢ����"<<endl;
        cout<<"*  ����1��������"<<endl;
        cout<<"************************************"<<endl;
        cin>>temp;
        cin.ignore(MAXSIZE, '\n');
        if(temp=="-1"){
            break;
        }else if((temp=="0")||(temp=="1")){
            flag = atoi(temp.c_str());
            flag = gaming(flag);
        }else{cout<<"����������~~��������Ч����~~����������"<<endl;}
    }
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
    cout<<"������������������������������������"<<endl;
    cout<<"����ʱ����-1�����˳����أ�׼��������"<<endl;
    system("pause");
    cout<<"������������������������������������"<<endl;
    string word;
    string inword;
    clock_t start, end;
    int len;
    int n = (int)(log2(this->get_level()+1));
    for(int j=1; j<=n; j++){
        len = (int)(log2(this->get_experience()));
        cout<<"��������������������صĵ�"<<j<<"�����ʡ���������"<<endl;
        cout<<"������������ע��������ʾ�ĵ��ʣ�����������"<<endl;
        word = getRandomWord(len, this->dict->getroot());
        len = word.size();
        this->dict->temp_show(word, 500*len);
        start = clock();
        for(int i=3; i>0; i--){
            cout<<"����������������ղ���ʾ�ĵ��ʣ�����������"<<endl;
            getline(cin, inword);
            if(inword=="-1"){
                cout<<"�����������ؿ����˳�����������"<<endl;
                system("pause");
                return 0;
            }else if(inword==word){
                cout<<"���������������⣡�ɵ�Ư��������������"<<endl;
                exp_up(1);
                end = clock();
                ptime += (int)(double(end-start)/CLOCKS_PER_SEC);
                cout<<"������������ϲͨ�أ���̫���ˣ�����������"<<endl;
                return 1;
            }else{
                cout<<"������������ѽ~�Ǵ���ѽ~����������"<<endl;
                cout<<"����������������"<<i<<"�λ���Orz����������"<<endl;
            }
        }
        cout<<"������������Ҫ���ģ��ٽ������ɣ�����������"<<endl;
        return 0;
    }
    return 0;
}
// modeΪ0ѭ����ս��һ��
// modeΪ1��ս�¹ؿ�
// �ؿ�������-1
// �ؿ��˳�����0
int Challenger::gaming(int mode){
    if((mode==0)&&(this->get_nop()==0)){
        cout<<"����������~~����δͨ���κιؿ���������ս�¹ؿ�~~����������"<<endl;
        return -1;
    }
    if(this->dict->getroot()->child_num()==0){
        cout<<"����������~~�ʵ�����Ϊ�գ��޷���ս�������˳�~~����������"<<endl;
        return -1;
    }else{
        int flag = 1;
        int ptime = 0;
        if(mode){
            while(flag){
                flag = this_level(ptime);
                if(flag){
                    this->nop_up();
                    // ͨ���¹ؿ����⾭�齱���ɴ���ʱ��͹ؿ�����ͬ����
                    exp_up((int)(this->get_nop()/2+20/ptime));
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
    cout<<" (~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~("<<endl;
    cout<<" |         ��    ��    ��    ��         |"<<endl;
    cout<<" |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|"<<endl;
    cout<<" |  �ǳƣ�\t\t"<<this->get_name();
    for(int k=0; k<this->get_name().size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  ���룺\t\t"<<this->get_pwd();
    for(int k=0; k<this->get_pwd().size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  �ȼ���\t\t"<<this->get_level();
    for(int k=0; k<to_string(this->get_level()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  ���飺\t\t"<<this->get_experience();
    for(int k=0; k<to_string(this->get_experience()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  ͨ���ؿ�����\t"<<this->get_nop();
    for(int k=0; k<to_string(this->get_nop()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  �ȼ����а�������\t"<<CRL;
    for(int k=0; k<to_string(this->CRL).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  �������а�������\t"<<CRE;
    for(int k=0; k<to_string(this->CRE).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" )  ͨ�����а�������\t"<<CRP;
    for(int k=0; k<to_string(this->CRP).size(); k++){cout<<"\b";}
    cout<<"\t\t)"<<endl;
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
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

// �����߳���
void Examiner::Play(){
    string input;
    int flag;

    this->dict->rDict(this->dict->getpath());
    cin.ignore(MAXSIZE, '\n');
    while(1){
        cout<<"������������������������������������"<<endl;
        cout<<"��Ϊ��������´ʰɣ�"<<endl;
        cout<<"������������������������������������"<<endl;
        cout<<"����-1���Է�����һ������"<<endl;
        cout<<"����0���Բ鿴��ǰ�ʿ�"<<endl;
        cout<<"������������������������������������"<<endl;
        getline(cin, input);    // ֱ��cin�޷����տո�
        if(input=="-1"){break;}
        if(input=="0"){
            flag = atoi(input.c_str());
            cout<<"������������������������������������"<<endl;
            cout<<"��ǰ�ʿ������е�������"<<endl;
            cout<<"������������������������������������"<<endl;
            this->dict->show_all_words(this->dict->getroot(), flag);
            cout<<"������������������������������������"<<endl;
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
// չʾ�������������
void Examiner::ShowValue(){
    cout<<" (~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~("<<endl;
    cout<<" |         ��    ��    ��    ��         |"<<endl;
    cout<<" |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|"<<endl;
    cout<<" |  �ǳƣ�\t\t"<<this->get_name();
    for(int k=0; k<this->get_name().size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  ���룺\t\t"<<this->get_pwd();
    for(int k=0; k<this->get_pwd().size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  �ȼ���\t\t"<<this->get_level();
    for(int k=0; k<to_string(this->get_level()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  ���飺\t\t"<<this->get_experience();
    for(int k=0; k<to_string(this->get_experience()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  �ۼƳ�������\t"<<this->get_nop();
    for(int k=0; k<to_string(this->get_nop()).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" |  �ȼ����а�������\t"<<ERL;
    for(int k=0; k<to_string(this->ERL).size(); k++){cout<<"\b";}
    cout<<"\t\t|"<<endl;
    cout<<" )  �������а�������\t"<<ERN;
    for(int k=0; k<to_string(this->ERN).size(); k++){cout<<"\b";}
    cout<<"\t\t)"<<endl;
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
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
