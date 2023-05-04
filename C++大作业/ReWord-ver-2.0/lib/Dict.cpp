# include "Dict.h"


// ��ȡ�ӽ����Ŀ
int TrieNode::child_num(){
    int count = 0;
    for(int i=0; i<54; i++){if(this->next[i]!=NULL){count++;}}
    return count;
}

// �ͷ�ȫ���ӽ��
void Dict::destroy(TrieNode* node){
    if(!node){return ;}
    for(auto child:node->next){
        node = child;
        destroy(node);
    }
    delete node;
    return ;
}
// �����µ���
// ���ʷǷ�����-1
// �����Ѵ��ڷ���0
// ���ʴ����ɹ�����1
int Dict:: Insert(string &word){
    int ptr;
    //��wordΪ�գ�ֱ�ӷ���
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
            cout<<"����������~~���ʷǷ�����������~~����������"<<endl;
            return -1;
        }
        if(node->next[ptr]==NULL){node->next[ptr] = new TrieNode();}
        node = node->next[ptr];
    }
    if((ptr<0)||(ptr>51)){
        cout<<"����������~~���ʷǷ�����������~~����������"<<endl;
        return -1;
    }
    if(node->isE){
        cout<<"����������~~�����Ѵ��ڣ��������ĵ���~~����������"<<endl;
        return 0;
    }
    node->isE = true;
    return 1;
}
// �жϵ����Ƿ�Ϸ�
// ���ʷǷ�����-1�����򷵻�1
int Dict::isLegal(string &word){
    int ptr;
    //��wordΪ�գ�ֱ�ӷ���
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
            cout<<"����������~~���ʷǷ�����������~~����������"<<endl;
            return -1;
        }
    }
    if((ptr<0)||(ptr>51)){
        cout<<"����������~~���ʷǷ�����������~~����������"<<endl;
        return -1;
    }
    return 1;
}
// ��ʾstr����n���������str
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
// ��ȡ�ļ�����Trie��
void Dict::rDict(string path){
    ifstream infile(path, ios::in|ios::binary);
    if(!infile){
        cout<<"����������~~�ʿ��ļ������ڣ������½��մʿ�~~����������"<<endl;
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
// �洢Trie�����ļ���
void Dict::wDict(string path){
    ofstream outfile(path, ios::out|ios::binary|ios::trunc);
    serialize_node(outfile, this->root);
    outfile.close();
}
// ����������л�
void Dict::serialize_node(ofstream& outfile, const TrieNode* node)const{
    // �����ΪҶ�ӽ����ֱ�ӴӸý��ָ��ָ��λ�ý�sizeof(node)���ֽ�д���ļ���
    if(!node){
        outfile.write(reinterpret_cast<const char*>(&node), sizeof(node));
        return;
    }
    // �������ӽ����д��ý���д��ý��Ĵʽ�β�жϱ�־
    outfile.write(reinterpret_cast<const char*>(&node), sizeof(node));
    outfile.write(reinterpret_cast<const char*>(&node->isE), sizeof(node->isE));
    // �Խ����ӽ�����д��
    for(auto child:node->next){
        serialize_node(outfile, child);
    }
}
// �����л�
TrieNode* Dict::deserialize_node(ifstream& infile){
    TrieNode* node;
    infile.read(reinterpret_cast<char*>(&node), sizeof(node));
    if(!node){return NULL;}
    node = new TrieNode{};
    infile.read(reinterpret_cast<char*>(&node->isE), sizeof(node->isE));
    for(auto& child:node->next){child = deserialize_node(infile);}
    return node;
}
// �����rootΪ������Trie���д洢�����е���
void Dict::show_all_words(TrieNode* root, int &count, string word){
    // �����ǰ�ڵ�������ʽ�β������õ���
    char ch;
    if(root->isE){cout<<word<<endl;}
    // ������ǰ�ڵ�������ӽڵ㲢����ǰ�ӽڵ���ַ����뵽������
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
            // �ݹ�������ӽڵ�
            show_all_words(root->next[i], count, newWord);
        }
    }
    if(count==0){cout<<"����������~~�ʿ��л�û�е��� Orz �Ȼ��������~~����������"<<endl;}
    return ;
}
