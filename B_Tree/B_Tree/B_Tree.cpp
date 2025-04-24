#include <iostream>
#include <queue>
#include <string>  
using namespace std;

const int T = 3; 

class BTreeNode {
public:
    char keys[2 * T - 1];
    BTreeNode* children[2 * T];
    int numKeys;
    bool isLeaf;

    BTreeNode(bool leaf) {
        isLeaf = leaf;
        numKeys = 0;
        for (int i = 0; i < 2 * T; i++)
            children[i] = nullptr;
    }

    void insertNonFull(char k);
    void splitChild(int i, BTreeNode* y);
};

class BTree {
public:
    BTreeNode* root;

    BTree() {
        root = new BTreeNode(true);
    }

    void insert(char k);
    void printByLevels(); 
};

void BTreeNode::insertNonFull(char k) {
    int i = numKeys - 1;

    if (isLeaf) {
        while (i >= 0 && k < keys[i]) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        numKeys++;
    }
    else {
        while (i >= 0 && k < keys[i]) i--;

        if (children[i + 1]->numKeys == 2 * T - 1) {
            splitChild(i + 1, children[i + 1]);

            if (k > keys[i + 1]) i++;
        }
        children[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->isLeaf);
    z->numKeys = T - 1;

    for (int j = 0; j < T - 1; j++)
        z->keys[j] = y->keys[j + T];

    if (!y->isLeaf) {
        for (int j = 0; j < T; j++)
            z->children[j] = y->children[j + T];
    }

    y->numKeys = T - 1;

    for (int j = numKeys; j >= i + 1; j--)
        children[j + 1] = children[j];
    children[i + 1] = z;

    for (int j = numKeys - 1; j >= i; j--)
        keys[j + 1] = keys[j];
    keys[i] = y->keys[T - 1];

    numKeys++;
}

void BTree::insert(char k) {
    if (root->numKeys == 2 * T - 1) {
        BTreeNode* s = new BTreeNode(false);
        s->children[0] = root;
        s->splitChild(0, root);

        int i = 0;
        if (s->keys[0] < k)
            i++;
        s->children[i]->insertNonFull(k);

        root = s;
    }
    else {
        root->insertNonFull(k);
    }
}

void BTree::printByLevels() {
    if (!root) return;

    queue<pair<BTreeNode*, int>> q;
    q.push({ root, 0 });

    int currentLevel = 0;
    cout << "B-Tree (by levels):\n";

    while (!q.empty()) {
        auto [node, level] = q.front(); q.pop();

        if (level != currentLevel) {
            cout << endl;
            currentLevel = level;
        }

        cout << "[";
        for (int i = 0; i < node->numKeys; i++) {
            cout << node->keys[i];
            if (i < node->numKeys - 1) cout << " ";
        }
        cout << "] ";

        if (!node->isLeaf) {
            for (int i = 0; i <= node->numKeys; i++) {
                if (node->children[i]) {
                    q.push({ node->children[i], level + 1 });
                }
            }
        }
    }
    cout << endl;
}

int main() {
    BTree tree;
    string input;


    cout << "Enter characters (separated by spaces): ";
    getline(cin, input);


    for (char c : input) {
        if (c != ' ') {
            tree.insert(c);
        }
    }


    tree.printByLevels();

    return 0;
}
