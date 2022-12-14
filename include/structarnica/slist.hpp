#include <memory>
#include <iterator>
#include <exception>
#include <optional>
#include <iostream>
#include <sstream>

namespace DS {

//Single linked list class
//holds pointers to head and tail for faster insert operations
template<typename T>
struct SList {

    struct Iterator {

        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using INode = SList<T>::Node*;

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

        bool operator==(const Iterator& other) {
            return m_ptr == other.m_ptr;
        }

        bool operator!=(const Iterator& other) {
            return m_ptr != other.m_ptr;
        }

        friend SList<T>;

    private:
        
        INode m_ptr{nullptr};

    };

    using iterator = SList<T>::Iterator;
    using const_iterator = SList<const T>::Iterator;

public:

    SList() {}

    SList(std::size_t sz) {
        for (;sz--;){
            push_front(T{});
        }
    }

    SList(std::initializer_list<T> lst) {
        for (auto x : lst)
            push_back(x);
    }

    SList(const SList& other) {
        for (Node* it = other.m_head; it != nullptr; it = it->next){
            push_front(it->data);
        }

        reverse();
    }

    SList& swap(SList&& other) {
        std::swap(m_head, other.m_head);
        std::swap(m_tail, other.m_tail);
        return *this;
    }

    SList& operator=(const SList& other) {
        SList tmp(other);
        return swap(std::move(tmp));
    }

    SList& operator=(SList&& other) {
        return swap(std::move(other));
    }

    SList(SList&& other) {
        swap(std::move(other));
    }

    ~SList() {
        if (!this->size())
            return;

        clear();
    }

    SList copy() {
        SList res;

        for (Node* it = m_head; it; it = it->next)
            res.push_back(it->data);

        return res;
    }

    bool operator==(const SList& other) {
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

    bool operator!=(const SList& other) {
        return !(*this == other);
    }

    operator bool() {
        return !empty();
    }

    SList operator+(SList&& other) {
        SList res = copy();

        for (auto x : other)
            res.push_back(x);

        return res;
    }

    SList& operator+=(SList&& other) {
        for (auto x : other)
            push_back(x);
            
        return *this;
    }

    //stl support
    iterator begin() { return iterator(m_head); }
    iterator end() { return iterator(nullptr); }
    const_iterator cbegin() const { return iterator(m_head); }
    const_iterator cend() const { return iterator(nullptr); }

    //list api to use without stl
    std::size_t size() const {
        if (!m_head)
            return 0;

        std::size_t res{0};

        for (Node* it = m_head; it; it = it->next){ res++; };
        return res;
    }

    SList& clear() {
        for (Node* prev = m_head; prev; ){
            m_head = m_head->next;
            delete prev;
            prev = m_head;
        }

        m_head = nullptr;
        m_tail = nullptr;
        return *this;
    }

    bool empty() const { return m_head == nullptr; }

    SList& reverse() {
        if (empty() || (size() == 1) )
            return *this;

        Node* tmp{nullptr};
        Node* curr{m_head};
        Node* prev{nullptr};

        m_tail = m_head;

        for (;;){
            tmp = curr->next;
            curr->next = prev;
            if (!tmp)
                break;
            prev = curr;
            curr = tmp;
        }

        m_head = curr;
        m_tail->next = nullptr;
        return *this;
    }

    bool contains(T val) {
        return this->_find(val).first;
    }

    std::optional<std::size_t> find(T val) {
        auto p = this->_find(val);

        if (!p.first)
            return std::nullopt;


        return std::optional{p.second};
    }

    std::optional<T> get(std::size_t index) {
        Node* it = m_head;
        for (;index && it; it = it->next, index--);

        return it ? std::optional{it->data} : std::nullopt;
    }

    std::optional<T> first() const {
        return m_head ? std::optional<T>{m_head->data} : std::nullopt;
    }

    std::optional<T> last() const {
        return m_tail ? std::optional<T>{m_tail->data} : std::nullopt;
    }

    const T& cfront() const {
        return front();
    }

    const T& cback() const {
        return back();
    }

    T& front() const {
        if (m_head)
            return m_head->data;

        throw std::exception("accessing element of empty list");
    }

    T& back() const {
        if (m_tail)
            return m_tail->data;

        throw std::exception("accessing element of empty list");
    }

    void push_back(T val){
        if (empty())
            return push_front(val);

        if (m_tail){
            m_tail->next = new Node(std::forward<T>(val));
            m_tail = m_tail->next;
        } else {
            m_tail = new Node(std::forward<T>(val));
            m_head->next = m_tail;
        }
    }

    void push_front(T val) {
        //if head = nullptr this is first node added
        Node* n = new Node(std::forward<T>(val), m_head);

        if (!m_tail)
            m_tail = m_head;

        m_head = n;
    }

    std::optional<T> pop_front() {
        if (!m_head)
            return std::nullopt;

        T ret = m_head->data;
        Node* t = m_head;
        m_head = m_head->next;
        delete t;
        return ret;
    }

    std::optional<T> pop_back() {
        switch (size()){
            case 0: return std::nullopt; break;
            case 1: return pop_front(); break;
            default:
            {
                T val = m_tail->data;
                Node* prev = nullptr;

                for (prev = m_head; prev->next != m_tail; prev = prev->next);

                if (m_tail)
                    delete m_tail;

                if (prev)
                    prev->next = nullptr;

                m_tail = prev;

                return val;
            }
        }
    }

    std::size_t count(T val) {
        std::size_t ret{};

        for (Node* it = m_head; it; it = it->next){
            if (it->data == val)
                ++ret;
        }

        return ret;
    }

    void erase(iterator from, iterator until) {
        Iterator prev;
        for (auto it = begin(); it != from;){
            prev = it++;
        }

        if (!prev.m_ptr)
            return;

        for(;;){
            if (from == until)
                break;
            auto t = from.m_ptr;
            ++from;
            delete t;
        }

        prev.m_ptr->next = until.m_ptr;
        m_tail = until.m_ptr;
    }

    bool erase(iterator it) {
        if (!it.m_ptr)
            return false;

        if (it == begin()){
            pop_front();
            return true;
        }

        Iterator prev(m_head);
        Iterator curr(m_head->next);

        for (;;){
            if (!curr.m_ptr)
                break;
            if (curr == it)
                break;
            prev = curr++;
        }

        prev.m_ptr->next = curr.m_ptr->next;
        if (it.m_ptr == m_tail)
            m_tail = prev.m_ptr;
        delete curr.m_ptr;
        return true;
    }

    bool erase(T val) {
        if (!m_head)
            return false;

        if (m_head == m_tail){
            if (m_head->data == val){
                clear();
                return true;
            }

            return false;
        }

        if (m_head->data == val){
            Node* t = m_head;
            m_head = m_head->next;
            delete t;
            return true;
        }

        Node* prev{m_head};
        Node* it{m_head->next};

        for (;;){
            if (!it)
                return false;
            if (it->data == val)
                break;
            prev = it;
            it = it->next;
        }

        prev->next = it->next;
        if (it == m_tail)
            m_tail = prev;
        delete it;
        return true;
    }

    bool erase_at(std::size_t index) {
        Node* prev{nullptr};
        Node* it{m_head};

        for (;index--;){
            prev = it;
            it = it->next;
        }
    }

    std::string to_string() {
        std::stringstream ss;

        ss << '[';

        for (Node* it = m_head; it != nullptr; it = it->next){
            ss << it->data;

            if (it->next)
                ss << ", ";
        }

        ss << ']';
        return ss.str();
    }

#ifdef DS_DEBUG_LIST
    
    void print_list() {
        std::cout << to_string() << '\n';   
    }

#endif

private:

    struct Node {

        explicit Node(T val):data{val} {}

        Node(T val, Node* next_node):data{val}, next{next_node} {}

        T data;
        Node* next{nullptr};
    };

    std::pair<Node*, int> _find(T val) {
        int pos = 0;
        for (Node* it = m_head; it != nullptr; it = it->next, pos++)
            if (it->data == val)
                return {it, pos};

        return {nullptr, pos};
    }

    Node* m_head{nullptr};
    Node* m_tail{nullptr};
    
};

}//DS namespace