#include <vector>
#include <functional>
#include <iostream>

struct ListNode {
    int value;
    ListNode* next;
};

std::ostream& operator<<(std::ostream& stream, ListNode head) {
    stream << head.value;
    auto cur = head.next;
    while (cur) {
        stream << " -> " << cur->value;
        cur = cur->next;
    }
    return stream;
}

template<class T>
class Heap {

    std::vector<T> v_;
    const std::function<bool (T, T)> heap_rule_;

    size_t getParentIndex(size_t child_index) { return (child_index) >> 1; }

    size_t getLeftChildIndex(size_t parent_index) { return 2 * parent_index + 1; }

    size_t getRightChildIndex(size_t parent_index) { return 2 * (parent_index + 1); }

    void up(size_t index) {
        while (index != 0 && heap_rule_(v_[index], v_[getParentIndex(index)])) {
            std::cout << "up" << std::endl;
            std::swap(v_[index], v_[getParentIndex(index)]);
            index = getParentIndex(index);
        }
    }
public:
    void down(size_t i) {
        size_t index = i;
        size_t left = getLeftChildIndex(index);
        size_t right = getRightChildIndex(index);
        while ((left < v_.size() || right < v_.size())) {
            if (left < v_.size()) {
                if (right < v_.size() && heap_rule_(v_[right], v_[left]) && !heap_rule_(v_[index], v_[right])) {
                    std::swap(v_[index], v_[right]);
                    index = right;
                } else if (!heap_rule_(v_[index], v_[left])) {
                    std::swap(v_[index], v_[left]);
                    index = left;
                } else return;
            } else if (!heap_rule_(v_[index], v_[right])) {
                std::swap(v_[index], v_[right]);
                index = right;
            } else return;
            left = getLeftChildIndex(index);
            right = getRightChildIndex(index);
        }
    }

    Heap() { ; }

    Heap(std::vector<T>& v, std::function<bool (T, T)> rule):v_(std::move(v)), heap_rule_(std::move(rule)) {
        for (size_t i = v_.size() / 2; i + 1 > 0; --i) {
            down(i);
        }
    }
    void push(T value) {
        v_.push_back(value);
        up(v_.size() - 1);
    }
    void pop() {
        auto i = v_.size() - 1;
        std::swap(v_[0], v_[i]);
        v_.pop_back();
        down(0);
    }
    T& top() {
        return v_[0];
    }
    T& back() {
        return v_[v_.size() - 1];
    } 
    bool empty() {
        return v_.empty();
    }
    size_t size() {
        return v_.size();
    }
};

ListNode* GetSortList(std::vector<ListNode*>& v) {
    size_t index = 0;
    while(index < v.size()) {
        if (v[index]) index++;
        else {
            std::swap(v[index], v[v.size() - 1]); // Удаляем все nullptr
            v.pop_back();
        }
    }
    if (v.empty()) return nullptr;
    Heap<ListNode*> heap {v, [](ListNode* a, ListNode* b) {return a->value < b->value;}};
    auto head = heap.top();
    auto cur = heap.top();
    while (cur && heap.size() > 0) {
        heap.top() = heap.top()->next;
        if (!heap.top() && heap.size() > 1) {
            heap.pop();
            cur->next = heap.top();
            cur = cur->next;
        } else {
            heap.down(0);
            cur->next = heap.top();
            cur = cur->next;
        }
    }
    return head;
}

int main() {
    ListNode a3 = {9, nullptr};
    ListNode a2 = {8, &a3};
    ListNode a1 = {7, &a2};

    ListNode b3 = {6, nullptr};
    ListNode b2 = {5, &b3};
    ListNode b1 = {4, &b2};

    ListNode c4 = {3, nullptr};
    ListNode c3 = {2, &c4};
    ListNode c2 = {1, &c3};
    ListNode c1 = {0, &c2};

    std::vector<ListNode*> v {&a1, &b1, &c1, nullptr};
    std::cout << "a1: " << a1 << std::endl;
    std::cout << "b1: " << b1 << std::endl;
    std::cout << "c1: " << c1 << std::endl;
    ListNode* head = GetSortList(v);
    std::cout << "head: " << *head;
}
