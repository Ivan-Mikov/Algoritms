#include <iostream>

int max(int a, int b) {
    return a > b ? a : b;
}

class AVLtree {
private:
    struct Node {
        int value;
        Node* left;
        Node* right;
        int height = 0;
        int count; // Количество элементов в поддереве, включая текущий узел
        Node(int value) { 
            value = value; 
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    int getHeight(Node* node) {
        if (!node) return -1;
        return node->height;
    }

    void updateHeight(Node* node) {
        auto leftHeight = getHeight(node->left);
        auto rightHeight = getHeight(node->right);
        node->height = max(leftHeight, rightHeight) + 1;
    }

    int diffHeights(Node* node) {
        if (!node) return 0;
        return getHeight(node->right) - getHeight(node->left);
    }

    void swap(Node* node1, Node* node2) {
        auto tmpHeight = node1->height;
        auto tmpValue = node1->value;
        auto tmpCount = node1->count;
        node1->value = node2->value;
        node1->height = node2->height;
        node1->count = node2->count;
        node2->value = tmpValue;
        node2->height = tmpHeight;
        node2->count = tmpCount;
    }

    void rightTurn(Node* node) {
        swap(node, node->left);
        auto tmp = node->right;
        node->right = node->left;
        node->left = node->right->left;
        node->right->left = node->right->right;
        node->right->right = tmp;
        updateHeight(node->right);
        updateHeight(node);
    }

    void leftTurn(Node* node) {
        swap(node, node->right);
        auto tmp = node->left;
        node->left = node->right;
        node->right = node->left->right;
        node->left->right = node->left->left;
        node->left->left = tmp;
        updateHeight(node->left);
        updateHeight(node);
    }

    void stabilizeTree(Node* node) {
        int diff = diffHeights(node);
        if (diff == -2) {
            if (diffHeights(node->left) == 1) leftTurn(node->left);
            rightTurn(node);
        } else if (diff == 2) {
            if (diffHeights(node->right) == -1) rightTurn(node->right);
            leftTurn(node);
        }
    }

    void pushRool(int val, Node* node) {
        if (val < node->value) {
            if (!node->left) {
                auto* newNode = new Node(val);
                node->left = newNode;
                return;
            } else {
                pushRool(val, node->left);
            }

        } else {
            if (!node->right) {
                auto* newNode = new Node(val);
                node->right = newNode;
                return;
            } else {
                pushRool(val, node->right);
            }
        }
        updateHeight(node);
        stabilizeTree(node);
        node->count++;
    }

    int getMax(Node* node) {
        while (node->right) {
            node = node->right;
        }
        return node->value;
    }

    Node* popRool(int val, Node* node) {
        if (!node) return nullptr;
        if (val < node->value) node->left = popRool(val, node->left);
        else if (val > node->value) node->right = popRool(val, node->right);
        else {
            if (!node->left) node = node->right;
            else if (!node->right) node = node->left;
            else {
                node->value = getMax(node->left);
                node->right = popRool(node->value, node->right);
            }
        }

        updateHeight(node);
        stabilizeTree(node);
        node->count--;

        return node;
    }

    int kRool(int k, Node* node) {
        auto countOfLeft = (node->left) ? node->left->count : -2; // Тогда при любом k гарантированно и
        if (k == countOfLeft + 1) return node->value; // пойдем вправо в случае отсутствия левой ветки
        else if (k < countOfLeft + 1) {
            return kRool(k, node->left);
        } else {
            return kRool(k - countOfLeft - 1, node->right);
        }
    }

public:

    void push(int val) {
        pushRool(val, root);
    }

    void pop(int val) {
        auto* buff = popRool(val, root);
    }

    int getKStatistic(int k) { // Предпологаем, что 0 <= k <= root->count
        return kRool(k, root); // Нормальную проверку для k не успеваю дописать
    }
};
