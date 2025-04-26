#include <vector>
#include <random>
#include <iostream>
using std::vector, std::pair;

std::random_device rd;
std::mt19937 gen(rd());

int randInt(int a, int b) {
    std::uniform_int_distribution<> dist(a, b);
    return dist(gen);
}

int getIndexMax(std::vector<int>& vec, int l, int r) {
    if (r < l) return -1;
    auto indexMax = l;
    for (int i = l; i <= r; ++i) {
        if (vec[i] > vec[indexMax])
            indexMax = i;
    }
    return indexMax;
}

template <class T>
class Treap {

    struct Node {
        T value;
        int priority;

        Node* left;
        Node* right;

        Node(T value, int pr) {
            value = value;
            priority = pr;

            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;
    int size; // size не успел сделать 

    Node* step(std::vector<T>& vec, std::vector<int>& pr, int l, int r) {
        if (r < l) return nullptr;
        auto mInd = getIndexMax(pr, l, r);
        auto* head = new Node(vec[mInd], pr[mInd]);
        head->left = step(vec, pr, l, mInd - 1);
        head->right = step(vec, pr, mInd + 1, r);

        return head;
    }

    Node* merge(Node* a, Node* b) { // Все значения а должны быть меньше всех значений в b
        if (!a && !b) return nullptr;
        if (!a && b) return b;
        if (!b && a) return a; 

        if (a->priority > b->priority) {
            a->right = merge(a->right, b);
            return a;

        } else {
            b->left = merge(a, b->left);
            return b;
        }
    }

    pair<Node*, Node*> split(Node* head, T spliter) {
        if (!head) return {nullptr, nullptr};
        if (head->value < spliter) {
            auto answer = split(head->right, spliter);
            head->right = answer.first;
            return {head, answer.second};
        } else {
            auto answer = split(head->left, spliter);
            head->left = answer.second;
            return {answer.first, head};
        }
    } 

public:
    Treap(std::vector<T>& vec) {
        size = vec.size();
        std::vector<int> pr;
        for (int i = 0; i < vec.size(); ++i) {
            pr.push_back(randInt(0, 100));
        }
        root = step(vec, pr, 0, vec.size() - 1);
    }

    void push(T elem) {
        auto trees = split(root, elem);
        auto treeSmall = trees.first;
        auto treeBig = trees.second;
        
        auto* newNode = new Node(elem, randInt(0, 100));
        treeSmall = merge(treeSmall, newNode);
        root = merge(treeSmall, treeBig);
        size++;
    }

    void pop(T elem) {
        auto trees = split(root, elem);
        auto treeSmall = trees.first;

        trees = split(trees.second, elem + 1);
        auto treeElem = trees.first;
        auto treeBig = trees.second;

        treeElem = merge(treeElem->left, treeElem->right); // Удаляю корень
        treeSmall = merge(treeSmall, treeElem);
        root = merge(treeSmall, treeBig);

        size--;
    }

    int size() {
        return size;
    }
};

int main() {
    std::vector<int> v = {2, 3, 6, 4, 2, 0, -1, 2};
    Treap<int> tr {v};
    tr.push(9);
    std::cout << tr.size();
}