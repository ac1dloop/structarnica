#include <memory>
#include <iterator>
#include <exception>
#include <optional>
#include <iostream>
#include <sstream>

namespace DS {

template<typename T>
struct SList {

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

    bool erase(T val) {
        if (!m_head)
            return false;

        if (size() == 1){
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