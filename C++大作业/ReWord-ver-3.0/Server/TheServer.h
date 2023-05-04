# ifndef THESERVER_H
# define THESERVER_H
# define user_path "./Datas/users.csv"
# include "./include/head.h"

// 初始化socket并监听客户端请求
// 出错返回0，否则返回1
int InitAndListen(SOCKET &ListenSocket);
// 释放socket
void freesocket(SOCKET &Socket);
// 向客户端发送消息
void Send(SOCKET &ClientSocket, string &temp);
// 从客户端接收消息
// 返回接收到的消息的长度
int Recv(SOCKET &ClientSocket, char *RecvBuf, int &recvByte);
// 判断文件是否存在或为空
// 文件不存在则关闭文件并返回-1
// 文件为空返回0，否则返回1
int isIFE(ifstream &infile);
// 发送Words.dat
void SendWords(SOCKET &ClientSocket);
// 接收Words.dat
void RecvWords(SOCKET &SocketClient);
// 判断昵称是否在数据文件里，此为注册时所调用
// 数据文件不存在则返回"File Error"，flag为-1
// 账号已存在则返回"Fail"，flag为1
// 账号可创建则返回"Success"，flag为0
string isInCSV(string &temp_name, string path, int &flag);
// 判断账号是否在数据文件里，此为登录时所调用
// 数据文件不存在则返回"File Error"，flag为-1
// 账号不存在则返回"Name Error"，flag为0
// 密码不正确则返回"Password Error"，flag为0
// 密码正确则返回账号信息，flag为1
string isInCSV(string &temp_name, string &temp_pwd, string path, int &flag);
// users.csv末端添加数据
// 每行结构：CEflag, name, pwd, experience, level, nop
void wUcsv(int CEflag, string temp_name, string temp_pwd, string path);
// 读取csv最后一行内容来确定新用户排名
// 无文件返回-1，文件为空返回0
// 否则返回排名最后的用户的排名
int rcsvl(string path);
// 新用户排名num追加入csv
// 输入参数flag为1说明是level排行，初始数据为1
void wRcsv(int num, string name, string path, int flag);
// 读取csv并输出所有数据
// 文件不存在返回"-1"，找不到返回"0"
string PAR(string path);
// 获取某个用户的具体排名
// 文件不存在返回"-1"，找不到返回"0"
string getRank(string temp_name, string path);
// 根据昵称查看对应用户信息
string check_user(string name);
// 检查新昵称是否可用
// 文件不存在返回"-1"
// 新昵称不可用返回"0"
// 新昵称可用返回"1"
string check_name(string new_name);
// 修改users.csv中的昵称或密码为str
// flag为1修改密码，flag为0修改昵称
// C++以流读取文件，无法直接修改某一行数据
// 故数据存入string对应修改后再覆盖文件
void ModifyUCSV(int &flag, string name, string &str);
// 修改排名文件中的昵称为str
void ModifyRCSV(string name, string &str, string path);
// 更新用户数据及排名
void Update(string &temp);
// 更新用户数据文件
void UpdateUser(string &temp);
// 更新用户排名文件
void UpdateRank(int new_num, string name, string path);
// 服务端线程，DWORD本质上为unsigned int，WINAPI为一个宏，表示函数调用方式
// 根据接收到的客户端消息返回对应值
DWORD WINAPI ServerThread(LPVOID lpParameter);

# endif