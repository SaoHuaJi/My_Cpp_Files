# ifndef THECLIENT_H
# define THECLIENT_H
# include "./include/head.h"
# include "./include/LocalAccount.h"
# include "./include/LocalAccount.cpp"
# include "./include/Dict.h"
# include "./include/Dict.cpp"

// 初始化socket
// 出错返回0，否则返回1
int InitSocket(SOCKET &SocketClient);
// 释放socket
void freesocket(SOCKET &Socket);
// 从服务端接收消息
// 返回接收到的消息的长度
int Recv(SOCKET &SocketClient, char *RecvBuf, int &recvByte);
// 游戏主界面
// 选择登录flag为0
// 选择注册flag为1
// 选择查榜flag为2
// 选择退出flag为-1
// 其余输入flag为-2
int MainMenu();
// 发送Words.dat
void SendWords(SOCKET &ClientSocket);
// 接收Words.dat
void RecvWords(SOCKET &SocketClient);
// 游戏主体
void Game(SOCKET &SocketClient, User *&user);
// 退出登录时更新数据
void Update(SOCKET &SocketClient, User *&user);
// 欢迎用户登录
// 返回-1退出登录
// 返回0查看自身属性
// 返回1查看他人属性
// 返回2修改昵称或密码
// 返回3实现特有功能
int welcomeUser(User *&user);
// 登录申请，向服务端发送"Login,name,pwd"
// 主动退出返回-1，否则返回0
int LRequest(SOCKET &SocketClient, string &temp);
// 注册申请，向服务端发送"Register,name,pwd"
// 主动退出返回-1，否则返回0
int RRequest(SOCKET &SocketClient, string &temp, string &temp_name, string &temp_pwd);
// 登录函数
// 登录成功则user为登录对象，否则为NULL
void Login(SOCKET &SocketClient, User *&user);
// 注册函数
// 注册成功则user为登录对象，否则为NULL
void Register(SOCKET &SocketClient, User *&user);
// 查看排行榜
void Rank(SOCKET &SocketClient);
// 修改用户昵称/密码
void SetValue(SOCKET &SocketClient, User *&user);

# endif