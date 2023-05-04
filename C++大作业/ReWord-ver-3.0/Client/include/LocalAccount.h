# ifndef LOCALACCOUNT_H
# define LOCALACCOUNT_H
# include "Dict.h"

class User{
private:
    string name = "NULL";   // 用户昵称
    string pwd = "NULL";    // 用户密码
    int CEflag = 0;         // 出题者判断标志，1为出题者，0为挑战者
    int experience = 0;     // 用户经验
    int level = 1;          // 用户等级
    int nop = 0;            // 出题者为出题数，挑战者为通过关卡数
public:
    Dict* dict = new Dict();

    User() = default;
    User(string new_name, string pwd, int flag=0,
        int experience=0, int level=1):
        name(new_name), pwd(pwd), CEflag(flag),
        experience(experience), level(level){}
    virtual ~User(){if(!dict){delete dict;}}
    // 获取用户昵称
    string get_name(){return this->name;}
    // 设置用户新昵称
    void set_name(string new_name){this->name = new_name;}
    // 获取用户密码
    string get_pwd(){return this->pwd;}
    // 设置用户新密码
    void set_pwd(string new_pwd){this->pwd = new_pwd;}
    // 获取用户出题者判断标志
    int get_CEflag(){return this->CEflag;}
    // 设置用户出题者判断标志
    void set_CEflag(int flag){this->CEflag = flag;}
    // 获取用户等级
    int get_level(){return this->level;}
    // 设置用户等级
    void set_level(int new_level){this->level = new_level;}
    // 等级+1
    void level_up(){this->level += 1;}
    // 获取用户经验
    int get_experience(){return this->experience;}
    // 设置用户经验
    void set_experience(int new_exp){this->experience = new_exp;}
    // 获取用户nop
    int get_nop(){return this->nop;}
    // 设置用户nop
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
    // 设置挑战者等级排名
    void Challenger::set_CRL(int R){this->CRL = R;}
    // 获取挑战者等级排名
    int Challenger::get_CRL(){return this->CRL;}
    // 设置挑战者经验排名
    void Challenger::set_CRE(int R){this->CRE = R;}
    // 获取挑战者经验排名
    int Challenger::get_CRE(){return this->CRE;}
    // 设置挑战者关卡数排名
    void Challenger::set_CRP(int R){this->CRP = R;}
    // 获取挑战者关卡数排名
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
    // 设置出题者等级排名
    void set_ERL(int R){this->ERL = R;}
    // 获取出题者等级排名
    int get_ERL(){return this->ERL;}
    // 设置出题者出题数排名
    void set_ERN(int R){this->ERN = R;}
    // 获取出题者出题数排名
    int get_ERN(){return this->ERN;}

    void Play();
    void ShowValue();
    void UpdateUser();
};

# endif