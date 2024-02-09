#include <vector>
#include <iostream>
using namespace std;

struct BPlusTree {
    struct Node {
        int M;
        bool isLeaf;
        vector<int> keys; // max keys M, min keys M/2
        Node* next; // puntero al siguiente nodo hoja
        Node(int order): M(order), isLeaf(true), next(nullptr) {}
    };

    Node* root;
    BPlusTree(int order): root(new Node(order)) {}

    pair<int, Node*> searchKey(Node* node, int value) {
        int i = 0;
        while (i < node->keys.size() && value > node->keys[i]) {
            i++;
        }
        if (i < node->keys.size() && value == node->keys[i]) {
            return {i, node};
        }
        if (node->isLeaf) {
            return {-1, node};
        }
        return searchKey(node->children[i], value);
    }

    Node* splitRoot(Node* &node) {
        Node* temp = new Node(node->M);
        temp->isLeaf = false;
        temp->children.push_back(node);
        node = temp;
        splitChild(temp, 0);
        return temp;
    }

    void splitChild(Node* &node, int index) {
        Node* tempChild = node->children[index];
        Node* tempHalf = new Node(node->M);
        tempHalf->isLeaf = tempChild->isLeaf;
        int mid = tempChild->M / 2;

        for (int i = mid; i < tempChild->keys.size(); i++) {
            tempHalf->keys.push_back(tempChild->keys[i]);
        }

        if (!tempChild->isLeaf) {
            for (int i = mid; i < tempChild->children.size(); i++) {
                tempHalf->children.push_back(tempChild->children[i]);
            }
        }

        tempChild->keys.erase(tempChild->keys.begin() + mid, tempChild->keys.end());
        tempChild->children.erase(tempChild->children.begin() + mid + 1, tempChild->children.end());

        node->keys.insert(node->keys.begin() + index, tempChild->keys[mid]);
        node->children.insert(node->children.begin() + index + 1, tempHalf);
    }

    void insertKey(Node* node, int value) {
        if (node->keys.size() == node->M) {
            Node* temp = splitRoot(root);
            insertNonFullNode(temp, value);
        } else {
            insertNonFullNode(node, value);
        }
    }

    void insertNonFullNode(Node* &node, int value) {
        int i = node->keys.size() - 1;
        if (node->isLeaf) {
            node->keys.push_back(0);
            while (i >= 0 && value < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = value;
        } else {
            while (i >= 0 && value < node->keys[i]) {
                i--;
            }
            i++;
            if (node->children[i]->keys.size() == node->M) {
                splitChild(node, i);
                if (value > node->keys[i]) {
                    i++;
                }
            }
            insertNonFullNode(node->children[i], value);
        }
    }

    void deleteKey(Node* node, int key) {
        if (node == nullptr) {
            return;
        }
        pair<int, Node*> keyPosition = searchKey(node, key);
        int index = keyPosition.first;
        Node* foundNode = keyPosition.second;
        if (foundNode == nullptr) {
            return;
        }
        if (foundNode->isLeaf && index < foundNode->keys.size() && foundNode->keys[index] == key) {
            foundNode->keys.erase(foundNode->keys.begin() + index);
            return;
        }
        if (!foundNode->isLeaf) {
            int predecessorKey = foundNode->keys[index];
            Node* predecessorNode = foundNode->children[index];
            while (!predecessorNode->isLeaf) {
                predecessorNode = predecessorNode->children.back();
            }
            foundNode->keys[index] = predecessorNode->keys.back();
            deleteKey(predecessorNode, predecessorNode->keys.back());
            return;
        }
        if (index == foundNode->keys.size() || foundNode->keys[index] != key) {
            return;
        }
        if (index == 0 && foundNode->children[index + 1]->keys.size() > node->M / 2) {
            int borrowedKey = foundNode->children[index + 1]->keys[0];
            foundNode->keys[index] = borrowedKey;
            deleteKey(foundNode->children[index + 1], borrowedKey);
            return;
        }
        if (index > 0 && foundNode->children[index - 1]->keys.size() > node->M / 2) {
            int borrowedKey = foundNode->children[index - 1]->keys.back();
            foundNode->keys[index] = borrowedKey;
            deleteKey(foundNode->children[index - 1], borrowedKey);
            return;
        }
        if (index < foundNode->keys.size()) {
            mergeNodes(foundNode, index);
            deleteKey(foundNode, key);
        } else {
            mergeNodes(foundNode, index - 1);
            deleteKey(foundNode, key);
        }
    }

    void mergeNodes(Node* parent, int index) {
        Node* leftChild = parent->children[index];
        Node* rightChild = parent->children[index + 1];

        leftChild->keys.push_back(parent->keys[index]);

        for (int i = 0; i < rightChild->keys.size(); ++i) {
            leftChild->keys.push_back(rightChild->keys[i]);
        }

        leftChild->next = rightChild->next;
        parent->keys.erase(parent->keys.begin() + index);
        parent->children.erase(parent->children.begin() + index + 1);

        delete rightChild;
    }

    void printTree(Node* node, int level = 0) {
        if (node != nullptr) {
            cout << "Level " << level << ": ";
            for (int i = 0; i < node->keys.size(); ++i) {
                cout << node->keys[i] << " ";
            }
            cout << endl;
            if (!node->isLeaf) {
                for (int i = 0; i < node->children.size(); ++i) {
                    printTree(node->children[i], level + 1);
                }
            } else {
                printTree(node->next, level);
            }
        }
    }
};

int main() {
    BPlusTree bPlusTree(4);

    // Insertar valores
    bPlusTree.insertKey(bPlusTree.root, 5);
    bPlusTree.insertKey(bPlusTree.root, 8);
    bPlusTree.insertKey(bPlusTree.root, 1);
    bPlusTree.insertKey(bPlusTree.root, 3);
    bPlusTree.insertKey(bPlusTree.root, 9);
    bPlusTree.insertKey(bPlusTree.root, 6);
    bPlusTree.insertKey(bPlusTree.root, 7);
    bPlusTree.insertKey(bPlusTree.root, 2);
    bPlusTree.insertKey(bPlusTree.root, 4);

    // Imprimir el árbol
    bPlusTree.printTree(bPlusTree.root);

    // Eliminar un valor
    bPlusTree.deleteKey(bPlusTree.root, 6);
    cout << "Después de eliminar 6:" << endl;
    bPlusTree.printTree(bPlusTree.root);

    return 0;
}
