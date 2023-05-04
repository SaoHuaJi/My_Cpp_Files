# ifndef THECLIENT_H
# define THECLIENT_H
# include "./include/head.h"
# include "./include/LocalAccount.h"
# include "./include/LocalAccount.cpp"
# include "./include/Dict.h"
# include "./include/Dict.cpp"

// ��ʼ��socket
// ������0�����򷵻�1
int InitSocket(SOCKET &SocketClient);
// �ͷ�socket
void freesocket(SOCKET &Socket);
// �ӷ���˽�����Ϣ
// ���ؽ��յ�����Ϣ�ĳ���
int Recv(SOCKET &SocketClient, char *RecvBuf, int &recvByte);
// ��Ϸ������
// ѡ���¼flagΪ0
// ѡ��ע��flagΪ1
// ѡ����flagΪ2
// ѡ���˳�flagΪ-1
// ��������flagΪ-2
int MainMenu();
// ����Words.dat
void SendWords(SOCKET &ClientSocket);
// ����Words.dat
void RecvWords(SOCKET &SocketClient);
// ��Ϸ����
void Game(SOCKET &SocketClient, User *&user);
// �˳���¼ʱ��������
void Update(SOCKET &SocketClient, User *&user);
// ��ӭ�û���¼
// ����-1�˳���¼
// ����0�鿴��������
// ����1�鿴��������
// ����2�޸��ǳƻ�����
// ����3ʵ�����й���
int welcomeUser(User *&user);
// ��¼���룬�����˷���"Login,name,pwd"
// �����˳�����-1�����򷵻�0
int LRequest(SOCKET &SocketClient, string &temp);
// ע�����룬�����˷���"Register,name,pwd"
// �����˳�����-1�����򷵻�0
int RRequest(SOCKET &SocketClient, string &temp, string &temp_name, string &temp_pwd);
// ��¼����
// ��¼�ɹ���userΪ��¼���󣬷���ΪNULL
void Login(SOCKET &SocketClient, User *&user);
// ע�ắ��
// ע��ɹ���userΪ��¼���󣬷���ΪNULL
void Register(SOCKET &SocketClient, User *&user);
// �鿴���а�
void Rank(SOCKET &SocketClient);
// �޸��û��ǳ�/����
void SetValue(SOCKET &SocketClient, User *&user);

# endif