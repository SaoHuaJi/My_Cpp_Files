# include "Dict.h"


// 获取子结点数目
int TrieNode::child_num(){
    int count = 0;
    for(int i=0; i<54; i++){if(this->next[i]!=NULL){count++;}}
    return count;
}

// 释放全部子结点
void Dict::destroy(TrieNode* node){
    if(!node){return ;}
    for(auto child:node->next){
        node = child;
        destroy(node);
    }
    delete node;
    return ;
}
// 插入新单词
// 单词非法返回-1
// 单词已存在返回0
// 单词创建成功返回1
int Dict:: Insert(string &word){
    int ptr;
    //若word为空，直接返回
    ptr = isLegal(word);
    TrieNode* node = root;
    for(char c : word){
        if(c==' '){
            ptr = 52;
        }else if(c=='-'){
            ptr = 53;
        }else if((c<='z')&&(c>='a')){
            ptr = c-'a';
        }else if((c<='Z')&&(c>='A')){
            ptr = c-'A'+26;
        }else{
            cout<<"！！！！！~~单词非法！请检查输入~~！！！！！"<<endl;
            return -1;
        }
        if(node->next[ptr]==NULL){node->next[ptr] = new TrieNode();}
        node = node->next[ptr];
    }
    if((ptr<0)||(ptr>51)){
        cout<<"！！！！！~~单词非法！请检查输入~~！！！！！"<<endl;
        return -1;
    }
    if(node->isE){
        cout<<"！！！！！~~单词已存在！请输入别的单词~~！！！！！"<<endl;
        return 0;
    }
    node->isE = true;
    return 1;
}
// 判断单词是否合法
// 单词非法返回-1，否则返回1
int Dict::isLegal(string &word){
    int ptr;
    //若word为空，直接返回
    if(word.size()==0){return 0;}
    TrieNode* node = root;
    for(char c : word){
        if(c==' '){
            ptr = 52;
        }else if(c=='-'){
            ptr = 53;
        }else if((c<='z')&&(c>='a')){
            ptr = c-'a';
        }else if((c<='Z')&&(c>='A')){
            ptr = c-'A'+26;
        }else{
            cout<<"！！！！！~~单词非法！请检查输入~~！！！！！"<<endl;
            return -1;
        }
    }
    if((ptr<0)||(ptr>51)){
        cout<<"！！！！！~~单词非法！请检查输入~~！！！！！"<<endl;
        return -1;
    }
    return 1;
}
// 显示str并在n毫秒后隐藏str
void Dict::temp_show(string str, int n){
    cout<<str;
    Sleep(n);
    for(int i=0; i<str.size(); i++){
        cout<<"\b";
    }
    for(int i=0; i<str.size(); i++){
        cout<<" ";
    }
    /*
    for(int i=0; i<str.size(); i++){
        cout<<"\b";
    }
    */
    cout<<endl;
    return ;
}
// 读取文件创建Trie树
void Dict::rDict(string path){
    ifstream infile(path, ios::in|ios::binary);
    if(!infile){
        cout<<"！！！！！~~词库文件不存在！正在新建空词库~~！！！！！"<<endl;
        infile.clear();
        infile.close();
        ofstream outfile(path, ios::out|ios::app);
        outfile.close();
        system("pause");
        return ;
    }
    this->root = deserialize_node(infile);
    infile.close();
}
// 存储Trie树至文件中
void Dict::wDict(string path){
    ofstream outfile(path, ios::out|ios::binary|ios::trunc);
    serialize_node(outfile, this->root);
    outfile.close();
}
// 先序遍历序列化
void Dict::serialize_node(ofstream& outfile, const TrieNode* node)const{
    // 若结点为叶子结点则直接从该结点指针指向位置将sizeof(node)个字节写入文件中
    if(!node){
        outfile.write(reinterpret_cast<const char*>(&node), sizeof(node));
        return;
    }
    // 结点存在子结点则写入该结点后写入该结点的词结尾判断标志
    outfile.write(reinterpret_cast<const char*>(&node), sizeof(node));
    outfile.write(reinterpret_cast<const char*>(&node->isE), sizeof(node->isE));
    // 对结点的子结点遍历写入
    for(auto child:node->next){
        serialize_node(outfile, child);
    }
}
// 反序列化
TrieNode* Dict::deserialize_node(ifstream& infile){
    TrieNode* node;
    infile.read(reinterpret_cast<char*>(&node), sizeof(node));
    if(!node){return NULL;}
    node = new TrieNode{};
    infile.read(reinterpret_cast<char*>(&node->isE), sizeof(node->isE));
    for(auto& child:node->next){child = deserialize_node(infile);}
    return node;
}
// 输出以root为根结点的Trie树中存储的所有单词
void Dict::show_all_words(TrieNode* root, int &count, string word){
    // 如果当前节点可作单词结尾则输出该单词
    char ch;
    if(root->isE){cout<<word<<endl;}
    // 遍历当前节点的所有子节点并将当前子节点的字符加入到单词中
    for(int i=0; i<54; i++){
        if(root->next[i]!=NULL){
            count++;
            if(i==52){
                ch = ' ';
            }else if(i==53){
                ch = '-';
            }else if(i<26){
                ch = 'a'+i;
            }else{ch = 'A'+i-26;}
            string newWord = word+ch;
            // 递归遍历该子节点
            show_all_words(root->next[i], count, newWord);
        }
    }
    if(count==0){cout<<"！！！！！~~词库中还没有单词 Orz 等会儿再来吧~~！！！！！"<<endl;}
    return ;
}
