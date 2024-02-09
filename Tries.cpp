#include <string>
#include <iostream>
using namespace std;

struct TriesTree {
    struct TrieNode {
        TrieNode* next;
        TrieNode* child;
        char key;
        bool isLeaf;
        
        TrieNode() = default;
        TrieNode(char _key): next(nullptr), child(nullptr), isLeaf(false), key(_key) {}

        void DeleteNode(TrieNode* &node){
            if(!node) return;
            DeleteNode(node->child);
            DeleteNode(node->next);
            delete node;
        }

        TrieNode* search(TrieNode* root, string &word){
            TrieNode* temp = root;
            int indx = 0;
            if(!temp){
                return temp;
            }
            while(temp != nullptr && indx < word.length()){
                if(temp->key != word[indx]){
                    temp = temp->next;
                }
                if(indx == word.length() - 1){
                    return temp;
                }
                else{
                    temp = temp->child;
                    indx++;
                }
            }
            return temp;
        }

        TrieNode* insertion(TrieNode* root, string &word){
            TrieNode* temp = root;
            int indx = 0;
            if(!root){
                while(indx < word.length()){
                    temp = new TrieNode(word[indx]);
                    if(indx < word.length() - 1)temp = temp->child;
                    indx++;
                }
                temp->isLeaf= true;
                return temp;
            }
            while(indx < word.length()){
                if(temp){
                    temp->isLeaf = false;
                    if(temp->key != word[indx]){
                        if(!temp->next) {
                            temp->next = new TrieNode(word[indx]);
                        }
                        temp = temp->next;
                    } else {
                        if(!temp->child) {
                            temp->child = new TrieNode(word[indx]);
                        }
                        temp = temp->child;
                        indx++;
                    }
                } else {
                    temp = new TrieNode(word[indx]);
                    if(indx == word.length() - 1){
                        temp->isLeaf = true;
                    }
                    if(!root) {
                        root = temp;
                    }
                    indx++;
                }
            }

            return temp;
        }


        TrieNode* deletion(TrieNode* &root, string &word) {
            TrieNode* temp = root;
            if(!temp){
                return temp;
            }
            
            bool eliminado = false;
            while(!eliminado){
                if(temp->key != word[0]){
                    temp = temp->next;
                }
                else{
                    DeleteNode(temp);
                    eliminado = true;
                    return nullptr;
                }

            }
            
            return temp;
        }
    };
    TrieNode *root;
    TriesTree(){
        root = new TrieNode();
    }
    void search(string word){
        if(root->search(root, word) != nullptr) cout << "encontrado " << word << endl;
        else cout << "no existe en el arbol " << word << endl;
    }
    void insertion(string word){
        TrieNode* temp = root->insertion(root, word);
        if(temp != nullptr) cout << "insertado " << word << endl;
        else cout << "no se pudo insertar " << word << endl;
    }
    void deletation(string word){
        TrieNode* temp = root->deletion(root, word);
        if(temp == nullptr) cout << "eliminado " << word << endl;
        else cout << "no existe en el arbol " << word << endl;
    }
};

int main() {
    TriesTree trie;

    // Prueba de inserción
    trie.insertion("apple");
    trie.insertion("banana");
    trie.insertion("apricot");
    trie.insertion("orange");

    // Prueba de búsqueda
    trie.search("apple");   // Debería imprimir "encontrado apple"
    trie.search("banana");  // Debería imprimir "encontrado banana"
    trie.search("apricot"); // Debería imprimir "encontrado apricot"
    trie.search("orange");  // Debería imprimir "encontrado orange"
    trie.search("grape");   // Debería imprimir "no existe en el arbol"

    // Prueba de eliminación
    trie.deletation("banana");  // Debería imprimir "eliminado banana"
    trie.search("banana");      // Debería imprimir "no existe en el arbol"

    return 0;
}
