#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <iterator>
#include <random>
#include <vector>
#include <optional>
#include <functional>
#include <string>
#include <sstream>
#include <iostream>

namespace DS {

//coin flip must be anything with operator() that returns 0 or 1 // true or false
static auto coin_flip = std::bind(std::uniform_int_distribution<int>(0, 1), std::mt19937());

template<typename T>
class SkipList {

private:

    struct Node {

        explicit Node(T val):_data{val} {}

        const T& data() const { return _data; }

        Node const* next() const { return _next; }

        Node const* prev() const { return _prev; }

        friend SkipList;

    private:

        T _data;
        Node* _next{nullptr};
        Node* _prev{nullptr};
        Node* _bottom{nullptr};
    };

    //0 level contains all values
    std::vector<Node*> m_levels;

    Node* _insert_at(size_t lvl, T val, Node* attach_to = nullptr) {
        Node* head = m_levels[lvl];
        Node* n = new Node(val);
        n->_bottom = attach_to;

        if (!head)
            return m_levels[lvl] = n;

        if (val < head->_data){
            n->_next = head;
            n->_prev = head->_prev;
            head->_prev = n;
            if (head->_next)
                head->_next->_prev = n;
            m_levels[lvl] = n;
        } else {
            Node* prev;
            for (;head && (val > head->_data);){
                prev = head;
                head = head->_next;
            }
            if (prev->_next)
                prev->_next->_prev = n;
            n->_next = prev->_next;
            prev->_next = n;
            n->_prev = prev;
        }

        return n;
    }

    void _clear_lvl(size_t lvl) {
        Node* head = m_levels[lvl];

        if (!head)
            return;

        Node* prev;
        for (;head;){
            prev = head;
            head = head->_next;
            delete prev;
        }
    }

    //search from left to right
    //top to bottom
    Node* _search(T val) {
        size_t lvl = m_levels.size() - 1;
        Node* res = m_levels[lvl];

        for (;!res && lvl; res = m_levels[--lvl]);

        for (;res;){

            if (val > res->_data){

                res = res->_next ? res->_next : res->_bottom;

            } else if (val < res->_data){

                if (!lvl) return nullptr;

                res = res->_prev ? res->_prev->_bottom : m_levels[--lvl];

            } else break;
        }

        return res;
    }

    void _remove(Node* ptr) {
        if (!ptr)
            return;

        Node* next_bot = ptr->_bottom;
        Node* prev = ptr->_prev;
        Node* next = ptr->_next;

        delete ptr;
        if (prev)
            prev->_next = next;
        if (next)
            next->_prev = prev;
        if (next_bot)
            _remove(next_bot);
    }

public:

    //to support STL Iterator works on level 0
    struct Iterator {

        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

    public:

        Iterator() = default;

        explicit Iterator(Node* ptr):m_ptr{ptr} {}

        Iterator(const Iterator& other) { m_ptr = other.m_ptr; }

        Iterator& operator=(const Iterator& other) {
            m_ptr = other.m_ptr;
            return *this;
        }

        reference operator*() { return m_ptr->_data; }

        Iterator operator++() {
            m_ptr = m_ptr->_next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        Iterator operator--() {
            m_ptr = m_ptr->_prev;
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp(*this);
            m_ptr = m_ptr->_prev;
            return *this;
        }

        bool operator==(const Iterator& other) const { return m_ptr == other.m_ptr; }

        bool operator!=(const Iterator& other) const { return m_ptr != other.m_ptr; }

        operator bool() const { return m_ptr; }

        friend SkipList<T>;

    private:

        Node* m_ptr{nullptr};

    };

    using iterator = SkipList<T>::Iterator;
    using const_iterator = typename SkipList<const T>::Iterator;

    SkipList(size_t max_levels = 4):m_levels(max_levels, nullptr) {}

    ~SkipList() { clear(); }

    void clear() {
        for (size_t i = 0; i < m_levels.size(); i++)
            _clear_lvl(i);
    }

    bool empty() { return m_levels[0]; }

    void remove(T val) {
        Node* res = _search(val);
        _remove(res);
    }

    iterator insert(T val) {
        auto head = _insert_at(0, val, nullptr);
        Node* n = head;

        for (int lvl = 1;coin_flip() && (lvl < static_cast<int>(m_levels.size())) ;lvl++){
            n = _insert_at(lvl, val, n);
        }

        return iterator(head);
    }

    iterator find(T val) {
        Node* res = _search(val);

        for (;res->_bottom; res = res->_bottom);

        return iterator(res);
    }

    std::string to_string(int lvl = 0) {
        std::stringstream ss;

        ss << "[";
        for (Node* it = m_levels[lvl]; it; it = it->_next){
            ss << it->_data;

            if (it->_next)
                ss << ", ";
        }

        ss << "]";
        return ss.str();
    }

    std::string dump() {
        std::stringstream ss;

        for (int lvl = m_levels.size() - 1; lvl >= 0; lvl--){
            ss << 'L' << lvl << ": ";
            ss << to_string(lvl) << '\n';
        }

        return ss.str();
    }

    iterator begin() { return iterator(m_levels[0]); }
    iterator end() { return iterator(nullptr); }
    const_iterator cbegin() { return const_iterator(m_levels[0]); };
    const_iterator cend() { return const_iterator(nullptr); }

};

};

#endif // SKIP_LIST_HPP
