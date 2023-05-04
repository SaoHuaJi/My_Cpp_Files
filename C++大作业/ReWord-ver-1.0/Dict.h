# ifndef DICT_H
# define DICT_H
# include "head.h"

class TrieNode{
public:
    bool isE = false;   // �жϵ�ǰ����Ƿ������Ϊ���ʵĽ�β
    TrieNode* next[54] = {NULL}; //��Сд��ĸ+�ո�+��-��

    TrieNode(){cout<<"TrieNode constructed!"<<endl;}
    ~TrieNode(){cout<<"TrieNode destructed!"<<endl;}
    int child_num();
};

class Dict{
private:
    TrieNode* root;
    string path = "./Datas/Words.dat";
public:
    Dict():root(new TrieNode()){}
    ~Dict(){destroy(root);}
    void show_all_words(TrieNode* root, int &count, string word="");
    void destroy(TrieNode* node);
    void setpath(string str){this->path = str;}
    string getpath(){return this->path;}
    TrieNode* getroot(){return this->root;}
    int Insert(string &word);
    int isLegal(string &word);
    void temp_show(string str, int n);
    void rDict(string path);
    void wDict(string path);
    void serialize_node(ofstream &ofs, const TrieNode* node)const;
    static TrieNode* deserialize_node(ifstream& ifs);
};

# endif