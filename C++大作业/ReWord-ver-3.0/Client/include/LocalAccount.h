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
    int nop = 0;            // ������Ϊ����������ս��Ϊͨ���ؿ���
public:
    Dict* dict = new Dict();

    User() = default;
    User(string new_name, string pwd, int flag=0,
        int experience=0, int level=1):
        name(new_name), pwd(pwd), CEflag(flag),
        experience(experience), level(level){}
    virtual ~User(){if(!dict){delete dict;}}
    // ��ȡ�û��ǳ�
    string get_name(){return this->name;}
    // �����û����ǳ�
    void set_name(string new_name){this->name = new_name;}
    // ��ȡ�û�����
    string get_pwd(){return this->pwd;}
    // �����û�������
    void set_pwd(string new_pwd){this->pwd = new_pwd;}
    // ��ȡ�û��������жϱ�־
    int get_CEflag(){return this->CEflag;}
    // �����û��������жϱ�־
    void set_CEflag(int flag){this->CEflag = flag;}
    // ��ȡ�û��ȼ�
    int get_level(){return this->level;}
    // �����û��ȼ�
    void set_level(int new_level){this->level = new_level;}
    // �ȼ�+1
    void level_up(){this->level += 1;}
    // ��ȡ�û�����
    int get_experience(){return this->experience;}
    // �����û�����
    void set_experience(int new_exp){this->experience = new_exp;}
    // ��ȡ�û�nop
    int get_nop(){return this->nop;}
    // �����û�nop
    void set_nop(int new_nop){this->nop = new_nop;}
    // nop+1
    void nop_up(){this->nop += 1;}

    void exp_up(int n);
    int getRank(string path, string temp_name);
    int check_name(string new_name, string path="./Datas/users.csv");
    void check_user(string name, string path="./Datas/users.csv");
    void SetValue();
    void ModifyUCSV(int &flag, string &str, string path);
    void ModifyRCSV(string &str, string path);
    void UpdateRank(int new_num, string path);
    virtual void set_CRL(int R){return ;}
    virtual int get_CRL(){return 0;}
    virtual void set_CRE(int R){return ;}
    virtual int get_CRE(){return 0;}
    virtual void set_CRP(int R){return ;}
    virtual int get_CRP(){return 0;}
    virtual void set_ERL(int R){return ;}
    virtual int get_ERL(){return 0;}
    virtual void set_ERN(int R){return ;}
    virtual int get_ERN(){return 0;}
    virtual void Play(){return ;}
    virtual void ShowValue(){return ;}
    virtual void UpdateUser(){return ;}
};

class Challenger:public User{
private:
    int CRE;
    int CRL;
    int CRP;
public:
    Challenger(string &new_name, string &pwd, int flag=0,
                int experience=0, int level=1, int nop=0){
        set_name(new_name);
        set_pwd(pwd);
        set_experience(experience);
        set_CEflag(flag);
        set_level(level);
        set_nop(nop);
    }
        ~Challenger(){}
    // ������ս�ߵȼ�����
    void Challenger::set_CRL(int R){this->CRL = R;}
    // ��ȡ��ս�ߵȼ�����
    int Challenger::get_CRL(){return this->CRL;}
    // ������ս�߾�������
    void Challenger::set_CRE(int R){this->CRE = R;}
    // ��ȡ��ս�߾�������
    int Challenger::get_CRE(){return this->CRE;}
    // ������ս�߹ؿ�������
    void Challenger::set_CRP(int R){this->CRP = R;}
    // ��ȡ��ս�߹ؿ�������
    int Challenger::get_CRP(){return this->CRP;}

    void Play();
    string getRandomWord(int len, TrieNode *root);
    int this_level(int &ptime);
    int gaming(int mode);
    void ShowValue();
    void UpdateUser();
};

class Examiner:public User{
private:
    int ERL;
    int ERN;
public:
    Examiner(){}
    Examiner(string &new_name, string &pwd, int flag=1,
            int experience=0, int level=1, int nop=0){
        set_name(new_name);
        set_pwd(pwd);
        set_CEflag(flag);
        set_experience(experience);
        set_level(level);
        set_nop(nop);
    }
    ~Examiner(){}
    // ���ó����ߵȼ�����
    void set_ERL(int R){this->ERL = R;}
    // ��ȡ�����ߵȼ�����
    int get_ERL(){return this->ERL;}
    // ���ó����߳���������
    void set_ERN(int R){this->ERN = R;}
    // ��ȡ�����߳���������
    int get_ERN(){return this->ERN;}

    void Play();
    void ShowValue();
    void UpdateUser();
};

# endif