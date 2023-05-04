# ifndef GAME_H
# define GAME_H
# include "LocalAccount.h"

class Game{
private:
    User* user = new User();
    string user_path = "./Datas/users.csv";
    string rank_path = "./Datas/CRL.csv";
public:
    Game(){
        delete user;
        cout<<"Game opened"<<endl;
    }
    void MainMenu(int* flag);
    void Switch(int &flag);
    int UserRegister();
    int UserLogin();
    int Rank();
    void getNP(string &name, string &pwd, int &flag);
    int isInCSV(string name, string path);
    int isInCSV(string &name, string &pwd, string path);
    void wUcsv(int CEflag, string temp_name, string temp_pwd, string path);
    int rcsvl(string path);
    void wRcsv(int num, string name, string path, int flag);
    int PAR(string path);
    int getRank(string name, string path);
    int isIFE(ifstream &infile);
    int welcomeUser(int CEflag);
    ~Game(){
        if(!user){delete user;}
        cout<<"Game end."<<endl;
    }
};

class RankNode{
public:
    string name;
    int level;
};

# endif