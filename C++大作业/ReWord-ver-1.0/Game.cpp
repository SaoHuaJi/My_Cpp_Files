# include "Game.h"
# include "head.h"


// ������
void Game::MainMenu(int* flag){
    int CEflag;
    string temp_flag;
    while(1){
        cout<<"�𾴵��û����ã�������Ҫ��ʲô��"<<endl;
        cout<<"����0ѡ���¼"<<endl;
        cout<<"����1ѡ��ע��"<<endl;
        cout<<"����2�鿴���а�"<<endl;
        cout<<"����-1�˳�����"<<endl;
        cin>>temp_flag;

        if((temp_flag=="0")||(temp_flag=="1")||(temp_flag=="2")){
            *flag = atoi(temp_flag.c_str());
        }else if(temp_flag=="-1"){
            *flag = atoi(temp_flag.c_str());
            cout<<"�������˳����ڴ��´��ټ���"<<endl;
            break;
        }else{
            cout<<"��������Ч���֡�"<<endl;
            continue;
        }
        Switch(*flag);

        if(*flag==-1){
            cout<<"�������˳����ڴ��´��ټ���"<<endl;
            break;
        }else if(*flag==1){
            CEflag = this->user->get_CEflag();
            while(1){
                *flag = welcomeUser(CEflag);
                if(*flag==0){
                    this->user->UpdateUser();
                    cout<<"���ѳɹ��˳���¼�����ڷ���������..."<<endl;
                    system("pause");
                    break;
                }else if(*flag==1){
                    this->user->ShowValue();
                }else if(*flag==2){
                    this->user->SetValue();
                }else{this->user->Play();}
            }
        }else{cout<<"�ɹ����������档"<<endl;}
    }
    return ;
}
// �л�����
// �ļ�������-1
// ע��/��¼��Ϸ���1
// ������������0
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
// ע��
// �����ļ��������򷵻�-1,
// ����ע�᷵��0��ע��ɹ�����1
int Game::UserRegister(){
    cout<<"************"<<endl;
    cout<<"* Register *"<<endl;
    cout<<"************"<<endl;
    int CEflag;
    int file_flag = 1;
    string temp_name;
    string temp_pwd;
    string temp_flag;

    while(file_flag==1){getNP(temp_name, temp_pwd, file_flag);}
    if(file_flag==-1){return -1;}
    if(file_flag==2){return 0;}
    if(file_flag==1){return 0;}

    // ���˻�Ϊ����ȷ���û�����
    while(1){
        cout<<"�ǳƿ��ã��������ǳ����߻��Ǵ����ߣ�"<<endl;
        cout<<"����0��ʾ�����ߣ�����1��ʾ������"<<endl;
        cin>>temp_flag;
        if((temp_flag=="0")||(temp_flag=="1")){
            CEflag = atoi(temp_flag.c_str());
            break;
        }else{cout<<"��������Ч���֡�"<<endl;}
    }
    
    // �û������а����ݸ���
    wUcsv(CEflag, temp_name, temp_pwd, this->user_path);
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
    cout<<"ע��ɹ����������Զ�Ϊ����¼��"<<endl;
    return 1;
}
// ��¼
// �����ļ��������򷵻�-1
// ��¼�������෵��0����¼�ɹ�����1
int Game::UserLogin(){
    cout<<"*********"<<endl;
    cout<<"* Login *"<<endl;
    cout<<"*********"<<endl;
    int CEflag;
    int file_flag = 0;
    string temp_name;
    string temp_pwd;
    
    getNP(temp_name, temp_pwd, file_flag);
    return file_flag;
}
// �鿴���а�
// �ļ������ڷ���-1�����򷵻�0
int Game::Rank(){
    int flag;
    string path;
    string name;
    string temp;

    while(1){
        cout<<"��ѡ����Ҫ�鿴�����а�"<<endl;
        cout<<"����-1����������"<<endl;
        cout<<"����0�鿴�����а�"<<endl;
        cout<<"����1�鿴��������"<<endl;
        cin>>temp;
        if(temp=="1"){
            while(1){
                cout<<"��ѡ����Ҫ�鿴�����а�"<<endl;
                cout<<"����0������һ������"<<endl;
                cout<<"����1�鿴�����ߵȼ����а�"<<endl;
                cout<<"����2�鿴�����߳��������а�"<<endl;
                cout<<"����3�鿴��ս�ߵȼ����а�"<<endl;
                cout<<"����4�鿴��ս�߾������а�"<<endl;
                cout<<"����5�鿴��ս��ͨ���ؿ����а�"<<endl;
                cin>>temp;
                if(temp=="0"){
                    cout<<"���ڷ�����һ������..."<<endl;
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
                    cout<<"��������Ч���֡�"<<endl;
                    continue;
                }
                break ;
            }
            if(!flag){continue;}
            cout<<"�����������ѯ���û����ǳƣ�"<<endl;
            cin>>name;
            flag = getRank(name, path);
            if(flag==-1){
                return -1;
            }else if(flag==0){
                cout<<"δ��ѯ���û�"<<name<<"�����ݣ����ڷ�����һ������..."<<endl;
                system("pause");
            }else{
                cout<<"�û�"<<name<<"�������ǵ�"<<flag<<"����"<<endl;
                system("pause");
            }
            continue ;
        }else if(temp=="0"){
            while(1){
                cout<<"��ѡ����Ҫ�鿴�����а�"<<endl;
                cout<<"����0������һ������"<<endl;
                cout<<"����1�鿴�����ߵȼ����а�"<<endl;
                cout<<"����2�鿴�����߳��������а�"<<endl;
                cout<<"����3�鿴��ս�ߵȼ����а�"<<endl;
                cout<<"����4�鿴��ս�߾������а�"<<endl;
                cout<<"����5�鿴��ս��ͨ���ؿ����а�"<<endl;
                cin>>flag;
                if(temp=="0"){
                    flag = atoi(temp.c_str());
                    cout<<"���ڷ�����һ������..."<<endl;
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
                    cout<<"��������Ч���֡�"<<endl;
                    continue;
                }
                break ;
            }
            if(!flag){continue;}
            flag = PAR(path);
            if(flag==-1){
                return -1;
            }else{
                cout<<"���а������ϣ�"<<endl;
                system("pause");
            }
        }else if(temp=="-1"){
            return 0;
        }else{cout<<"��������Ч���֡�"<<endl;}
    }
}
// ��ȡ�ǳƺ����룬���ؽ������flag������
// flagΪ1˵����ע��ʱ���ã�Ϊ0˵���ǵ�¼ʱ����
// �ļ������ڷ���-1�������˳�����2
// ע��ʱ�����˳�����1��ע����Ϸ���0
// ��¼ʱ�����˳�����0����¼��Ϸ���1
void Game::getNP(string &name, string &pwd, int &flag){
    cout<<"�˻��ǳ�����quit�ɷ���������"<<endl;
    cout<<"�������˻��ǳƣ�"<<endl;
    cin>>name;
    cout<<"�������˻����룺"<<endl;
    cin>>pwd;
    if(name=="quit"){
        flag = 2;
        return ;
    }
    if(flag==1){
        flag = isInCSV(name, this->user_path);
    }else{flag = isInCSV(name, pwd, this->user_path);}
    return ;
}
// �ж��ǳ��Ƿ��������ļ�����ڷ���1�����򷵻�0
// �����ļ��������򷵻�-1
// ��Ϊע��ʱ������
int Game::isInCSV(string temp_name, string path){
    string line;
    string temp;
    int flag;
    ifstream infile(path, ios::in|ios::app);

    flag = isIFE(infile);
    // ���ļ��������ж�ȡ�����˻����ݱȽ��û���
    if(flag==-1){return -1;}
    if(flag>0){
        infile.seekg(0, ios::beg);
        while(getline(infile, line)){
            istringstream sin(line);    // �������ַ���line���뵽�ַ�����sin��
            getline(sin, temp, ',');    // ���ַ�����sin�е�һ������ǰ���ַ�����temp��
            getline(sin, temp, ',');    // ���ַ�����sin�еڶ�������ǰ���ַ�����temp��
            if(temp_name==temp){
                cout<<"�˻��ǳ��ѱ�ʹ�ã����������ã�"<<endl;
                infile.close();
                return 1;
            }
            getline(sin, temp);         // ���ַ�����sin��ʣ�ಿ���ַ�����temp��
        }
    }
    infile.close();
    return 0;
}
// �ж��˺��Ƿ��������ļ�����ڷ���1�����򷵻�0
// �����ļ��������򷵻�-1
// ��Ϊ��¼ʱ������
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
    // ���ļ��������ж�ȡ�����˻����ݱȽ��û���
    if(flag==-1){return -1;}
    if(flag>0){
        while(last_try){
            infile.seekg(0, ios::beg);
            while(getline(infile, line)){
                istringstream sin(line);    // �������ַ���line���뵽�ַ�����sin��
                getline(sin, temp, ',');    // ���ַ�����sin�е�һ������ǰ���ַ�����temp��
                getline(sin, name, ',');    // ���ַ�����sin�еڶ�������ǰ���ַ�����name��
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
                    cout<<"��¼�ɹ���"<<endl;
                    if(CEflag){
                        cout<<"��ӭ����������"<<name<<"��"<<endl;
                        this->user = new Examiner(
                            temp_name, temp_pwd, CEflag, experience, level, nop);
                    }else{
                        cout<<"��ӭ������ս��"<<name<<"��"<<endl;
                        this->user = new Challenger(
                            temp_name, temp_pwd, CEflag, experience, level, nop);
                    }
                    infile.close();
                    return 1;
                    }
                }
                getline(sin, temp);         // ���ַ�����sin��ʣ�ಿ���ַ�����temp��
            }
            cout<<"������ǳƲ���ȷ�����������롣"<<endl;
            cout<<"�������Գ���"<<last_try<<"�ε�¼��"<<endl;
            cout<<"�������˻��ǳƣ�"<<endl;
            cin>>temp_name;
            cout<<"�������˻����룺"<<endl;
            cin>>temp_pwd;
            if(temp_name=="quit"){return 2;}
            last_try--;
        }
    }else{
        cout<<"�û����ݿ��������ݣ���ע����ٵ�¼��"<<endl;
        infile.close();
        return 0;
    }
    cout<<"���Դ������࣬�����µ�¼��"<<endl;
    cout<<"*******************************************"<<endl;
    infile.close();
    return 0;
}
// users.csvĩ���������
// ÿ�нṹ��CEflag, name, pwd, experience, level, nop
void Game::wUcsv(int CEflag, string temp_name, string temp_pwd, string path){
    // ���û�����д��users.csv��������ӦUser�����
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
// ��ȡcsv���һ��������ȷ�����û�����
// ���ļ�����-1���ļ�Ϊ�շ���0
// ���򷵻����������û�������
int Game::rcsvl(string path){
    streampos fsize;
    int R;
    ifstream infile(path, ios::in|ios::app);

    R = isIFE(infile);
    if(R>0){
        infile.seekg(-4, ios::end); // ��ǰ�������һ�еĻ��з�
        while(infile.peek()!=infile.widen('\n')){infile.seekg(-1, ios::cur);}
        infile.seekg(2, ios::cur);  // ������������ڶ��еĻ��з�
        infile>>R;
    }
    if(R>=0){infile.close();}
    return R;
}
// ���û�����num׷����csv
// �������flagΪ1˵����level���У���ʼ����Ϊ1
void Game::wRcsv(int num, string name, string path, int flag){
    ofstream outfile(path, ios::out|ios::app);
    outfile<<num<<','
            <<name<<','
            <<flag<<endl;
    outfile.close();
    return ;
}
// ��ȡcsv�������������
// �ļ��������򷵻�-1�����򷵻�0
int Game::PAR(string path){
    int flag;
    string line;
    string temp;
    ifstream infile(path, ios::in|ios::app);

    flag = isIFE(infile);
    if(flag==-1){
        return -1;
    }else if(flag==0){
        cout<<"���а�����ʱû�����ݣ���ע���ϰ�ɣ�"<<endl;
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
// ��ȡĳ���û��ľ�������
// �ļ������ڷ���-1���Ҳ�������0
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
// �ж��ļ��Ƿ���ڻ�Ϊ��
// �ļ���������ر��ļ�������-1
// �ļ�Ϊ�շ���0�����򷵻�1
int Game::isIFE(ifstream &infile){
    streampos fsize;

    // �����ļ���ȡʧ�����˳�
    if(!infile){
        cout<<"�ļ���ȡʧ�ܣ����������ļ��Ƿ���ȷ��"<<endl;
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
// ��ӭ�û���¼
// ����0�˳���¼
// ����1�鿴����
// ����2�޸��ǳƻ�����
// ����3ʵ�����й���
int Game::welcomeUser(int CEflag){
    string temp;
    int flag = 1;
    if(CEflag){
        cout<<"��ӭ������������"
            <<this->user->get_name()
            <<"��"<<endl;
            while(1){
                cout<<"������Ҫ��ʲô��"<<endl;
                cout<<"����0����������"<<endl;
                cout<<"����1�鿴��ǰ��������"<<endl;
                cout<<"����2�޸��˻��ǳƺ�����"<<endl;
                cout<<"����3��������"<<endl;
                // ֱ����flag��ȡ�����������벻Ϊ���ֻ��bug��ѭ��
                // �����atoiת��flag���ַ������Ϊ0
                cin>>temp;
                if((temp=="0")||(temp=="1")||(temp=="2")||(temp=="3")){
                    flag = atoi(temp.c_str());
                    break;
                }else{cout<<"��������Ч���֡�"<<endl;}
            }
    }else{
        cout<<"��ӭ��������ս��"
            <<this->user->get_name()
            <<"��"<<endl;
            while(1){
                cout<<"������Ҫ��ʲô��"<<endl;
                cout<<"����0����������"<<endl;
                cout<<"����1�鿴��ǰ��������"<<endl;
                cout<<"����2�޸��˻��ǳƺ�����"<<endl;
                cout<<"����3��������"<<endl;
                cin>>temp;
                if((temp=="0")||(temp=="1")||(temp=="2")||(temp=="3")){
                    flag = atoi(temp.c_str());
                    break;
                }else{cout<<"��������Ч���֡�"<<endl;}
            }
    }
    return flag;
}
