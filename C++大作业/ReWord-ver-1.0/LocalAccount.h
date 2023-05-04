# ifndef LOCALACCOUNT_H
# define LOCALACCOUNT_H
# include "Dict.h"

class User{
private:
    string name = "NULL";   // �û��ǳ�
    string pwd = "NULL";    // �û�����
    int CEflag = 0;         // �������жϱ�־��1Ϊ�����ߣ�0Ϊ��ս��
    int experience = 0;     // �û�����
    int level = 1;          // �û��ȼ�
public:
    Dict* dict = new Dict();
    User(){cout<<"User constructed!"<<endl;};
    User(string new_name, string pwd, int flag=0,
        int experience=0, int level=1):
        name(new_name), pwd(pwd), CEflag(flag),
        experience(experience), level(level){
        cout<<"User constructed!"<<endl;
    }
    virtual ~User(){
        if(!dict){delete dict;}
        cout<<"User destructed!"<<endl;
    }
    string get_name();
    void set_name(string name);
    string get_pwd();
    void set_pwd(string pwd);
    int get_CEflag();
    void set_CEflag(int flag);
    int get_level();
    void set_level(int new_level);
    void level_up();
    int get_experience();
    void exp_up(int n);
    int getRank(string path);
    int check_name(string new_name, string path="./Datas/users.csv");
    virtual void check_user(string name, string path="./Datas/users.csv"){cout<<"�㲻Ӧ�ÿ�����仰"<<endl;}
    void SetValue();
    void ModifyUCSV(int &flag, string &str, string path);
    void ModifyRCSV(string &str, string path);
    virtual void UpdateUser(){cout<<"�㲻Ӧ�ÿ�����仰"<<endl;}
    void UpdateRank(int new_num, string path);
    virtual void Play(){cout<<"�㲻Ӧ�ÿ�����仰"<<endl;}
    virtual void ShowValue(){cout<<"�㲻Ӧ�ÿ�����仰"<<endl;}
};

class Challenger:public User{
private:
    int plevel = 0;
    int CRE;
    int CRL;
    int CRP;
public:
    Challenger(){cout<<"Challenger constructed!"<<endl;}
    Challenger(string new_name, string pwd, int flag=0,
                int experience=0, int level=1, int plevel=0){
        set_name(new_name);
        set_pwd(pwd);
        set_CEflag(flag);
        CRL = getRank("./Datas/CRL.csv");
        CRP = getRank("./Datas/CRP.csv");
        CRE = getRank("./Datas/CRE.csv");
        cout<<"Challenger constructed!"<<endl;
    }
    ~Challenger(){cout<<"Challenger destructed!"<<endl;}
    void check_user(string name, string path="./Datas/users.csv");
    void Play();
    string getRandomWord(int len, TrieNode *root);
    int this_level(int &ptime);
    int gaming(int mode);
    void ShowValue();
    void UpdateUser();
};

class Examiner:public User{
private:
    int num = 0;
    int ERL;
    int ERN;
public:
    Examiner(){cout<<"Examiner constructed!"<<endl;}
    Examiner(string new_name, string pwd, int flag=1,
            int experience=0, int level=1, int num=0){
        set_name(new_name);
        set_pwd(pwd);
        set_CEflag(flag);
        ERL = getRank("./Datas/ERL.csv");
        ERN = getRank("./Datas/ERN.csv");
        cout<<"Challenger constructed!"<<endl;
    }
    ~Examiner(){cout<<"Examiner destructed!"<<endl;}
    void check_user(string name, string path="./Datas/users.csv");
    void Play();
    void ShowValue();
    void UpdateUser();
};

# endif