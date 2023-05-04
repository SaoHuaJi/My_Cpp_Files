# ifndef THESERVER_H
# define THESERVER_H
# define user_path "./Datas/users.csv"
# include "./include/head.h"

// ��ʼ��socket�������ͻ�������
// ������0�����򷵻�1
int InitAndListen(SOCKET &ListenSocket);
// �ͷ�socket
void freesocket(SOCKET &Socket);
// ��ͻ��˷�����Ϣ
void Send(SOCKET &ClientSocket, string &temp);
// �ӿͻ��˽�����Ϣ
// ���ؽ��յ�����Ϣ�ĳ���
int Recv(SOCKET &ClientSocket, char *RecvBuf, int &recvByte);
// �ж��ļ��Ƿ���ڻ�Ϊ��
// �ļ���������ر��ļ�������-1
// �ļ�Ϊ�շ���0�����򷵻�1
int isIFE(ifstream &infile);
// ����Words.dat
void SendWords(SOCKET &ClientSocket);
// ����Words.dat
void RecvWords(SOCKET &SocketClient);
// �ж��ǳ��Ƿ��������ļ����Ϊע��ʱ������
// �����ļ��������򷵻�"File Error"��flagΪ-1
// �˺��Ѵ����򷵻�"Fail"��flagΪ1
// �˺ſɴ����򷵻�"Success"��flagΪ0
string isInCSV(string &temp_name, string path, int &flag);
// �ж��˺��Ƿ��������ļ����Ϊ��¼ʱ������
// �����ļ��������򷵻�"File Error"��flagΪ-1
// �˺Ų������򷵻�"Name Error"��flagΪ0
// ���벻��ȷ�򷵻�"Password Error"��flagΪ0
// ������ȷ�򷵻��˺���Ϣ��flagΪ1
string isInCSV(string &temp_name, string &temp_pwd, string path, int &flag);
// users.csvĩ���������
// ÿ�нṹ��CEflag, name, pwd, experience, level, nop
void wUcsv(int CEflag, string temp_name, string temp_pwd, string path);
// ��ȡcsv���һ��������ȷ�����û�����
// ���ļ�����-1���ļ�Ϊ�շ���0
// ���򷵻����������û�������
int rcsvl(string path);
// ���û�����num׷����csv
// �������flagΪ1˵����level���У���ʼ����Ϊ1
void wRcsv(int num, string name, string path, int flag);
// ��ȡcsv�������������
// �ļ������ڷ���"-1"���Ҳ�������"0"
string PAR(string path);
// ��ȡĳ���û��ľ�������
// �ļ������ڷ���"-1"���Ҳ�������"0"
string getRank(string temp_name, string path);
// �����ǳƲ鿴��Ӧ�û���Ϣ
string check_user(string name);
// ������ǳ��Ƿ����
// �ļ������ڷ���"-1"
// ���ǳƲ����÷���"0"
// ���ǳƿ��÷���"1"
string check_name(string new_name);
// �޸�users.csv�е��ǳƻ�����Ϊstr
// flagΪ1�޸����룬flagΪ0�޸��ǳ�
// C++������ȡ�ļ����޷�ֱ���޸�ĳһ������
// �����ݴ���string��Ӧ�޸ĺ��ٸ����ļ�
void ModifyUCSV(int &flag, string name, string &str);
// �޸������ļ��е��ǳ�Ϊstr
void ModifyRCSV(string name, string &str, string path);
// �����û����ݼ�����
void Update(string &temp);
// �����û������ļ�
void UpdateUser(string &temp);
// �����û������ļ�
void UpdateRank(int new_num, string name, string path);
// ������̣߳�DWORD������Ϊunsigned int��WINAPIΪһ���꣬��ʾ�������÷�ʽ
// ���ݽ��յ��Ŀͻ�����Ϣ���ض�Ӧֵ
DWORD WINAPI ServerThread(LPVOID lpParameter);

# endif