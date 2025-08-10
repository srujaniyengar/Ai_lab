#include <iostream>
#include <string>
#include <queue>
using namespace std;

struct Node {
    string value;
    Node* left;
    Node* right;
    Node(string val) : value(val), left(nullptr), right(nullptr) {}
};

// Build DFS-style skewed tree (right-deep, original order)
Node* buildDFSTree(const string& expr) {
    string ops;
    string number;

    // Separate number and unary operators
    for (char c : expr) {
        if (isdigit(c)) number += c;
        else if (c == '!' || c == '~') ops += c;
    }

    if (number.empty()) return nullptr;

    Node* root = new Node(number);

    // Attach operators as new parents, in the order they appear
    for (char op : ops) {
        Node* newNode = new Node(string(1, op));
        newNode->left = root;
        root = newNode;
    }

    return root;
}

// Build BFS-style heap-like tree
Node* buildBFSTree(const string& expr) {
    string ops;
    string number;

    for (char c : expr) {
        if (isdigit(c)) number += c;
        else if (c == '!' || c == '~') ops += c;
    }

    if (number.empty()) return nullptr;

    Node* root = new Node(number);
    queue<Node*> q;
    q.push(root);

    int i = 0;
    while (i < ops.size()) {
        Node* current = q.front();
        q.pop();

        if (i < ops.size()) {
            current->left = new Node(string(1, ops[i++]));
            q.push(current->left);
        }
        if (i < ops.size()) {
            current->right = new Node(string(1, ops[i++]));
            q.push(current->right);
        }
    }

    return root;
}

// Print tree sideways
void printTree(Node* node, int depth = 0) {
    if (!node) return;
    printTree(node->right, depth + 1);
    for (int i = 0; i < depth; ++i) cout << "    ";
    cout << node->value << endl;
    printTree(node->left, depth + 1);
}

int main() {
    string expr;
    cout << "Enter expression (e.g., 4!!~~~~~): ";
    getline(cin, expr);

    Node* dfsRoot = buildDFSTree(expr);
    Node* bfsRoot = buildBFSTree(expr);

    cout << "\nDFS Skewed Tree:\n";
    printTree(dfsRoot);

    cout << "\nBFS Heap-Like Tree:\n";
    printTree(bfsRoot);

    return 0;
}