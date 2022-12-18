#ifndef DLIST_HPP
#define DLIST_HPP

#include <memory>
#include <iterator>
#include <optional>
#include <iostream>
#include <sstream>

namespace DS {

//doubly linked list
//provides same set of operations as singly linked list
//TODO: optimize with usage of holding both pointers
template<typename T>
class DList {

private:

    struct Node {

        explicit Node(T val):data{val} {}

        Node(T val, Node* next_node, Node* prev_node):
            data{val}, next{next_node}, prev{prev_node} {}

        T data;
        Node* next{nullptr};
        Node* prev{nullptr};
    };

    Node* m_head{nullptr};
    Node* m_tail{nullptr};

    bool _erase(Node* ptr) {
        if (!ptr)
            return false;

        if (ptr == m_head){
            m_head = m_head->next;
            delete ptr;
            if (m_head)
                m_head->prev = nullptr;
        } else if (ptr == m_tail){
            m_tail = m_tail->prev;
            delete ptr;
            if (m_tail)
                m_tail->next = nullptr;
        } else {
            Node* prev = ptr->prev;
            Node* next = ptr->next;
            prev->next = next;
            next->prev = prev;
            delete ptr;
        }

        return true;
    }

    Node* _find(T val) {
        for (Node* it = m_head; it; it = it->next){
            if (it->data == val)
                return it;
        }

        return nullptr;
    }

public:

    struct Iterator {

        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using INode = DList<T>::Node*;

    public:

        Iterator() = default;

        explicit Iterator(INode ptr):m_ptr{ptr} {}

        Iterator(const Iterator& other) {
            m_ptr = other.m_ptr;
        }

        Iterator& operator=(const Iterator& other) {
            m_ptr = other.m_ptr;
            return *this;
        }

        reference operator*() {
            return m_ptr->data;
        }

        Iterator operator++() {
            m_ptr = m_ptr->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        Iterator operator--() {
            m_ptr = m_ptr->prev;
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp(*this);
            m_ptr = m_ptr->prev;
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return m_ptr == other.m_ptr;
        }

        bool operator!=(const Iterator& other) const {
            return m_ptr != other.m_ptr;
        }

        friend DList<T>;

    private:

        INode m_ptr{nullptr};

    };

    using iterator = DList<T>::Iterator;
    using const_iterator = typename DList<const T>::Iterator;

    DList() {}

    DList(std::size_t sz) {
        for (;sz--;)
            push_back(T{});
    }

    DList(std::initializer_list<T> lst) {
        for (auto x : lst)
            push_back(x);
    }

    DList(const DList& other) {
        for (Node* it = other.m_head; it; it = it->next)
            push_back(it->data);
    }

    DList& swap(DList&& other) {
        std::swap(m_head, other.m_head);
        std::swap(m_tail, other.m_tail);
        return *this;
    }

    DList& operator=(DList&& other) {
        return swap(std::move(other));
    }

    DList& operator=(const DList& other) {
        DList tmp(other);
        return swap(std::move(tmp));
    }

    ~DList() {
        if (empty())
            return;

        clear();
    }

    DList copy() {
        DList res;

        for (Node* it = m_head; it; it = it->next)
            res.push_back(it->data);

        return res;
    }

    bool operator==(const DList& other) const {
        Node* it1 = m_head;
        Node* it2 = other.m_head;

        for (;;){
            if (it1 == nullptr || it2 == nullptr)
                break;

            if (it1->data != it2->data)
                return false;

            it1 = it1->next;
            it2 = it2->next;
        }

        return it1 == it2 ? true : false;
    }

    bool operator!=(const DList& other) const {
        return !(*this == other);
    }

    operator bool() {
        return !empty();
    }

    DList operator+(DList&& other) {
        DList res = copy();

        for (auto x : other)
            res.push_back(x);

        return res;
    }

    DList& operator+=(DList&& other) {
        for (auto x : other)
            push_back(x);

        return *this;
    }

    std::size_t size() const {
        if (!m_head)
            return 0;

        std::size_t res{0};

        for (Node* it = m_head; it; it = it->next){
            res++;
        };

        return res;
    }

    DList& clear() {
        for (Node* prev = m_head; prev; ){
            m_head = m_head->next;
            delete prev;
            prev = m_head;
        }

        m_head = nullptr;
        m_tail = nullptr;
        return *this;
    }

    bool empty() const { return !m_head; }

    std::optional<T> first() const {
        return m_head ? std::optional<T>{m_head->data} : std::nullopt;
    }

    std::optional<T> last() const {
        return m_tail ? std::optional<T>{m_tail->data} : std::nullopt;
    }

    const T& cfront() const { return front(); }

    const T& cback() const { return back(); }

    T& front() const { return m_head->data; }

    T& back() const { return m_tail->data; }

    void push_back(T val) {
        Node* n = new Node(val);

        //if it's first node
        if (!m_head){
            m_head = n;
            m_tail = n;
            return;
        }

        m_tail->next = n;
        n->prev = m_tail;
        m_tail = n;
    }

    void push_front(T val) {
        Node* n = new Node(val);

        //if it's first node
        if (!m_head){
            m_head = n;
            m_tail = n;
            return;
        }

        n->next = m_head;
        m_head->prev = n;
        m_head = n;
    }

    std::optional<T> pop_front() {
        if (!m_head)
            return std::nullopt;

        T ret = m_head->data;
        _erase(m_head);
        return ret;
    }

    std::optional<T> pop_back() {
        if (!m_tail)
            return std::nullopt;
        T ret = m_tail->data;
        _erase(m_tail);
        return ret;
    }

    std::size_t count(T val) {
        std::size_t ret{0};

        for (Node* it = m_head; it; it = it->next){
            if (it->data == val)
                ++ret;
        }

        return ret;
    }

    void erase(iterator from, iterator until) {
        if (!from.m_ptr)
            return;

        Node* prev = from.m_ptr->prev;
        Node* tmp;
        while(from != until){
            tmp = from.m_ptr;
            ++from;
            delete tmp;
        }

        prev->next = until.m_ptr;
        if (until.m_ptr)
            until.m_ptr->prev = prev;
    }

    bool erase(iterator it) { return _erase(it.m_ptr); }

    bool erase(T val) { return _erase(_find(val)); }

    Iterator find(T val){ return Iterator(_find(val)); }

    std::string to_string() {
        std::stringstream ss;

        ss << '[';

        for (Node* it = m_head; it; it = it->next){
            ss << it->data;

            if (it->next)
                ss << ", ";
        }

        ss << ']';
        return ss.str();
    }

    iterator begin() { return iterator(m_head); }
    iterator end() { return iterator{nullptr}; }
    const_iterator cbegin() const { return iterator(m_head); }
    const_iterator cend() const { return iterator(nullptr); }

#ifdef DS_DEBUG_LIST

    void print_list() {
        std::cout << to_string() << '\n';
    }

#endif

};

} //DS namespace

#endif // DLIST_HPP
