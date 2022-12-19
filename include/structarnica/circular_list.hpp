#ifndef CIRCULAR_LIST_HPP
#define CIRCULAR_LIST_HPP

#include <optional>
#include <iterator>
#include <sstream>
#include <iostream>

namespace DS {

//Circular list
//and CircularIterator
template<typename T>
class CircularList {

private:

    struct Node {

        explicit Node(T val):data{val} {}

        T data;
        Node* next{nullptr};
        Node* prev{nullptr};
    };

    bool _erase(Node* ptr) {
        if (!ptr)
            return false;

        if (m_head == m_tail){
            clear();
            return true;
        }

        if (ptr == m_head){
            m_head = m_head->next;
            delete ptr;
            if (m_head){
                m_head->prev = m_tail;
                m_tail->next = m_head;
            }
        } else if (ptr == m_tail){
            m_tail = m_tail->prev;
            delete ptr;
            if (m_tail){
                m_tail->next = m_head;
                m_head->prev = m_tail;
            }
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
            
            if (it->next == m_head)
                break;
        }

        return nullptr;
    }

    Node* m_head{nullptr};
    Node* m_tail{nullptr};

public:

    struct CircularIterator {

    public:

        CircularIterator() = default;

        explicit CircularIterator(Node* ptr):m_ptr{ptr} {}

        CircularIterator(const CircularIterator& other) {
            m_ptr = other.m_ptr;
        }

        CircularIterator& operator=(const CircularIterator& other) {
            m_ptr = other.m_ptr;
            return *this;
        }

        T& operator*() {
            return m_ptr->data;
        }

        CircularIterator operator++() {
            m_ptr = m_ptr->next;
            return *this;
        }

        CircularIterator operator++(int) {
            Iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        CircularIterator operator--() {
            m_ptr = m_ptr->prev;
            return *this;
        }

        CircularIterator operator--(int) {
            Iterator tmp(*this);
            m_ptr = m_ptr->prev;
            return *this;
        }

        bool operator==(const CircularIterator& other) const {
            return m_ptr == other.m_ptr;
        }

        bool operator!=(const CircularIterator& other) const {
            return m_ptr != other.m_ptr;
        }

        friend CircularList<T>;

    private:

        Node* m_ptr{nullptr};

    };

    struct Iterator {

        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using INode = CircularList<T>::Node*;

    public:

        Iterator() = default;

        explicit Iterator(INode ptr):m_ptr{ptr},m_starting{ptr} {}

        Iterator(const Iterator& other) {
            m_ptr = other.m_ptr;
            m_starting = other.m_starting;
        }

        Iterator& operator=(const Iterator& other) {
            m_ptr = other.m_ptr;
            m_starting = other.m_starting;
            return *this;
        }

        reference operator*() {
            return m_ptr->data;
        }

        Iterator operator++() {
            m_ptr = m_ptr->next;

            if (m_ptr == m_starting)
                m_ptr = nullptr;

            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        Iterator operator--() {
            m_ptr = m_ptr->prev;

            if (m_ptr == m_starting)
                m_ptr = nullptr;

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

        friend CircularList<T>;

    private:

        INode m_ptr{nullptr};
        INode m_starting{nullptr};

    };

    using iterator = CircularList<T>::Iterator;
    using const_iterator = typename CircularList<const T>::Iterator;

    CircularList() {}

    CircularList(std::size_t sz) {
        for (;sz--;)
            push_back(T{});
    }

    CircularList(std::initializer_list<T> lst) {
        for (auto x : lst)
            push_back(x);
    }

    CircularList(const CircularList& other) {
        for (Node* it = other.m_head; it; it = it->next){
            push_back(it->data);

            if (it->next == other.m_head)
                break;
        }
    }

    CircularList& swap(CircularList&& other) {
        std::swap(m_head, other.m_head);
        std::swap(m_tail, other.m_tail);
        return *this;
    }

    CircularList& operator=(CircularList&& other) {
        return swap(std::move(other));
    }

    CircularList& operator=(const CircularList& other) {
        CircularList tmp(other);
        return swap(std::move(tmp));
    }

    ~CircularList() {
        if (empty())
            return;

        clear();
    }

    CircularList copy() {
        CircularList res;

        for (Node* it = m_head; it; it = it->next){
            res.push_back(it->data);

            if (it->next == m_head)
                break;
        }

        return res;
    }

    bool operator==(const CircularList& other) const {
        Node* it1 = m_head;
        Node* it2 = other.m_head;

        for (;;){
            if (it1 == nullptr || it2 == nullptr)
                break;

            if (it1->data != it2->data)
                return false;

            if (it1->next == m_head || it2->next == other.m_head)
                break;

            it1 = it1->next;
            it2 = it2->next;
        }

        //last value
        if (it1->next == m_head && it2->next == other.m_head)
            return it1->data == it2->data;
        else return false; //else size is different
    }

    bool operator!=(const CircularList& other) const {
        return !(*this == other);
    }

    operator bool() {
        return !empty();
    }

    CircularList operator+(CircularList&& other) {
        CircularList res = copy();

        for (auto x : other)
            res.push_back(x);

        return res;
    }

    CircularList& operator+=(CircularList&& other) {
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

            if (it->next == m_head)
                break;
        };

        return res;
    }

    void clear() {
        for (Node* prev = m_head; prev;){

            if (m_head == m_tail){
                delete prev;
                break;
            }

            m_head = m_head->next;
            delete prev;
            prev = m_head;
        }

        m_head = nullptr;
        m_tail = nullptr;
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
            n->next = m_tail;
            n->next = m_head;
            return;
        }

        m_tail->next = n;
        n->prev = m_tail;
        n->next = m_head;
        m_head->prev = n;
        m_tail = n;
    }

    void push_front(T val) {
        Node* n = new Node(val);

        //if it's first node
        if (!m_head){
            m_head = n;
            m_tail = n;
            n->next = m_tail;
            n->prev = m_head;
            return;
        }

        m_head->prev = n;
        n->next = m_head;
        n->prev = m_tail;
        m_tail->next = n;
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

            if (it->next == m_head)
                break;
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

    iterator find(T val){ return iterator(_find(val)); }

    std::string to_string() {
        std::stringstream ss;

        ss << '[';

        for (Node* it = m_head; it; it = it->next){
            ss << it->data;

            if (it->next == m_head)
                break;

            if (it->next)
                ss << ", ";
        }

        ss << ']';
        return ss.str();
    }

    iterator begin() { return iterator(m_head); }
    iterator end() { return iterator(nullptr); }
    const_iterator cbegin() const { return iterator(m_head); }
    const_iterator cend() const { return iterator(nullptr); }

#ifdef DS_DEBUG_LIST

    void print_list() {
        std::cout << to_string() << '\n';
    }

#endif

};

} //DS namespace

#endif // CIRCULAR_LIST_HPP
