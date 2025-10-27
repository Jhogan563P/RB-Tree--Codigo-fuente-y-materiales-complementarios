#ifndef AVL_H
#define AVL_H
#include <iostream>
#include <queue>
using namespace std;

template <typename T>
class AVL {
    class node {
    public:
        T data;
        node* left;
        node* right;
        int height;
    };
    node* root = NULL;

    // devuelve máximo (inorder predecessor) de un subárbol dado
    T maxUtility(node* current)
    {
        while (current->right != NULL) current = current->right;
        return current->data;
    }

    int height(node* current) {
        if (current == NULL) return 0;
        return current->height;
    }

    node* rightRotation(node* current) {
        node* new_node = current->left;
        current->left = new_node->right;
        new_node->right = current;
        current->height = 1 + max(height(current->left), height(current->right));
        new_node->height = 1 + max(height(new_node->left), height(new_node->right));
        return new_node;
    }

    node* leftRotation(node* current) {
        node* new_node = current->right;
        current->right = new_node->left;
        new_node->left = current;
        current->height = 1 + max(height(current->left), height(current->right));
        new_node->height = 1 + max(height(new_node->left), height(new_node->right));
        return new_node;
    }

    void inOrderUtility(node* current) {
        if (current == NULL) return;
        inOrderUtility(current->left);
        cout << current->data << " ";
        inOrderUtility(current->right);
    }

    void preOrderUtility(node* current) {
        if (current == NULL) return;
        cout << current->data << " ";
        preOrderUtility(current->left);
        preOrderUtility(current->right);
    }

    void postOrderUtility(node* current) {
        if (current == NULL) return;
        postOrderUtility(current->left);
        postOrderUtility(current->right);
        cout << current->data << " ";
    }

    node* insertUtility(node* current, T value) {
        if (current == NULL) {
            node* new_node = new node;
            new_node->data = value;
            new_node->right = NULL;
            new_node->left = NULL;
            new_node->height = 1;
            return new_node;
        }
        if (value < current->data) current->left = insertUtility(current->left, value);
        else if (value > current->data) current->right = insertUtility(current->right, value);
        // si value == current->data no insertamos (evitar duplicados)
        current->height = 1 + max(height(current->left), height(current->right));
        int balance = height(current->left) - height(current->right);

        if (balance > 1) {
            if (value > current->left->data) {
                current->left = leftRotation(current->left);
                return rightRotation(current);
            } else {
                return rightRotation(current);
            }
        } else if (balance < -1) {
            if (value < current->right->data) {
                current->right = rightRotation(current->right);
                return leftRotation(current);
            } else {
                return leftRotation(current);
            }
        }
        return current;
    }

    node* removeUtility(node* current, T value) {
        if (current == NULL) {
            // nothing to do
            return NULL;
        }

        if (value < current->data) {
            current->left = removeUtility(current->left, value);
        } else if (value > current->data) {
            current->right = removeUtility(current->right, value);
        } else {
            // found node to remove
            if (current->left == NULL && current->right == NULL) {
                delete current;
                return NULL; // important: return immediately
            } else if (current->left != NULL && current->right != NULL) {
                T mx = maxUtility(current->left);
                current->data = mx;
                // IMPORTANT: reasignar el subárbol izquierdo con el resultado de la eliminación
                current->left = removeUtility(current->left, mx);
            } else {
                // uno solo hijo
                node* child = (current->right != NULL) ? current->right : current->left;
                delete current;
                return child; // el hijo toma el lugar del actual
            }
        }

        // si llegamos aquí current no es NULL
        current->height = 1 + max(height(current->left), height(current->right));
        int balance = height(current->left) - height(current->right);

        // Rebalanceo
        if (balance > 1) {
            if (height(current->left) >= height(current->right)) {
                return rightRotation(current);
            } else {
                current->left = leftRotation(current->left);
                return rightRotation(current);
            }
        } else if (balance < -1) {
            if (height(current->right) >= height(current->left)) {
                return leftRotation(current);
            } else {
                current->right = rightRotation(current->right);
                return leftRotation(current);
            }
        }
        return current;
    }

    void display_BFS() {
        if (root == NULL) cout << "Tree is empty" << endl;
        else {
            queue<node*> q;
            q.push(root);
            node* current;
            while (!q.empty()) {
                current = q.front();
                q.pop();
                cout << current->data << " ";
                if (current->left != NULL) q.push(current->left);
                if (current->right != NULL) q.push(current->right);
            }
            cout << endl;
        }
    }
public:
    void insert(T value) {
        root = insertUtility(root, value);
    }
    void remove(T value) {
        root = removeUtility(root, value);
    }
    bool find(T value) {
        node* current = root;
        while (current != NULL) {
            if (value == current->data) return true;
            else if (value < current->data) current = current->left;
            else current = current->right;
        }
        return false;
    }
    void inorder() {
        inOrderUtility(root);
        cout << endl;
    }
    void preOrder() {
        preOrderUtility(root);
        cout << endl;
    }
    void postOrder() {
        postOrderUtility(root);
        cout << endl;
    }
    void levelOrder() {
        display_BFS();
    }
};
#endif //AVL_H
