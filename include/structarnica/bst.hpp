#ifndef BST_HPP
#define BST_HPP

#include <cmath>
#include <queue>
#include <deque>
#include <stack>
#include <iostream>

namespace DS {

template<typename T, typename Compare = std::less<T> >
class BST {

    struct Node {

        Node() = default;

        Node(T val):_data{val} {}

        T _data;
        unsigned _count{1};
        Node* _left{nullptr};
        Node* _right{nullptr};
    };

public:

    BST() {}

    ~BST() { clear(); }

    const T& max() {
        Node* it = m_root;
        for (;it->_right; it = it->_right);
        return it->_data;
    }

    const T& min() {
        Node* it = m_root;
        for (;it->_left; it = it->_left);
        return it->_data;
    }

    void clear() {
        std::deque<Node*> q;
        q.push_back(m_root);

        for (Node* t;!q.empty();){
            t = q.back();
            q.pop_back();

            if (!t)
                continue;

            if (t->_left)
                q.push_back(t->_left);
            if (t->_right)
                q.push_back(t->_right);

            delete t;
        }
    }

    template<typename F>
    void bfs(F&& func) {
        std::deque<Node*> q;
        q.push_back(m_root);

        for (Node* it;!q.empty();){
            it = q.front();
            q.pop_front();

            if (!it)
                continue;

            func(it->_data);

            if (it->_left)
                q.push_back(it->_left);

            if (it->_right)
                q.push_back(it->_right);
        }
    }

    void insert(const T& val) {
        Node* it = m_root;
        Node* prev = nullptr;

        for (;it;){
            prev = it;
            if (it->_data == val){
                it->_count++;
                return;
            } else if (comp(val, it->_data)){
                it = it->_left;
            } else it = it->_right;
        }

        if (!prev)
            m_root = new Node(val);
        else if (comp(val, prev->_data))
            prev->_left = new Node(val);
        else prev->_right = new Node(val);
    }

    template<typename F>
    void rpreorder(F&& func) {
        _preorder(m_root, func);
    }

    template<typename F>
    void preorder(F&& func) {
        if (!m_root)
            return;

        std::stack<Node*> st;
        st.push(m_root);

        for (Node* it; !st.empty();){
            it = st.top();
            st.pop();

            func(it->_data);

            if (it->_right)
                st.push(it->_right);
            if (it->_left)
                st.push(it->_left);
        }
    }

    template<typename F>
    void rpostorder(F&& func) {
        _postorder(m_root, func);
    }

    template<typename F>
    void postorder(F&& func) {
        std::stack<Node*> st;
        Node* it = m_root;
        Node* prev = it;
        for (;it;){
            for (; it->_left; it = it->_left)
                st.push(it);
            for (;!it->_right || it->_right == prev;){
                func(it->_data);
                prev = it;
                if (st.empty())
                    return;
                it = st.top();
                st.pop();
            }
            st.push(it);
            it = it->_right;
        }
    }

    template<typename F>
    void rinorder(F&& func) {
        _inorder(m_root, func);
    }

    template<typename F>
    void inorder(F&& func) {
        std::stack<Node*> st;
        Node* it = m_root;
        for (;it;){
            for (;it;){
                if (it->_right)
                    st.push(it->_right);
                st.push(it);
                it = it->_left;
            }

            it = st.top();
            st.pop();

            for (;!st.empty() && !it->_right;){
                func(it->_data);
                it = st.top();
                st.pop();
            }

            func(it->_data);
            if (st.empty())
                break;

            it = st.top();
            st.pop();
        }
    }

    void erase(const T& val) {
        Node* it = m_root;
        Node* prev = nullptr;
        for (;it;){
            if (val == it->_data)
                break;
            prev = it;
            if (comp(val, it->_data))
                it = it->_left;
            else it = it->_right;
        }

        if (it && val == it->_data){
            if (it == m_root)
                _erase_merge(m_root);
            else if (prev->_left == it)
                _erase_merge(prev->_left);
            else _erase_merge(prev->_right);
        }
    }

    std::size_t transform_to_list() {
        return _create_backbone(m_root);
    }

    void balance() {
        Node* tmp = new Node();

        tmp->_right = m_root;

        std::size_t len = _create_backbone(tmp);

        std::size_t height = std::log2(len + 1);

        std::size_t n_nodes = std::pow(2, height) - 1;

        for (n_nodes = n_nodes / 2; n_nodes > 0; n_nodes /= 2){
            _compress(tmp, n_nodes);
        }

        m_root = tmp->_right;

        delete tmp;
    }

private:

    void _compress(Node* grand, std::size_t m){
        Node* tmp = grand->_right;
        Node* prev;

        for (std::size_t i = 0; i < m; i++){
            prev = tmp;
            tmp = tmp->_right;
            grand->_right = tmp;
            prev->_right = tmp->_left;
            tmp->_left = prev;
            grand = tmp;
            tmp = tmp->_right;
        }
    }

    Node* _rotate_left(Node* grand, Node* parent, Node* child) {
        if (grand){
            grand->_right = child;
        } else m_root = child;

        parent->_right = child->_left;
        child->_left = parent;
        return grand;
    }

    Node* _rotate_right(Node* grand, Node* parent, Node* child) {
        if (grand){
            grand->_right = child;
        } else m_root = child;

        parent->_left = child->_right;
        child->_right = parent;
        return grand;
    }

    std::size_t _to_vine(Node* root) {
        std::size_t len = 0;

        Node* tmp = root->_right;

        for (;tmp;){
            if (tmp->_left){
                Node* prev = tmp;
                tmp = tmp->_left;
                prev->_left = tmp->_right;
                tmp->_right = prev;
                root->_right = tmp;
            } else {
                root = tmp;
                tmp = tmp->_right;
            }
            ++len;
        }

        return len;
    }

    //make tree a list
    std::size_t _create_backbone(Node* root) {
        Node* grand = nullptr;
        Node* parent = root;
        Node* child = nullptr;
        std::size_t len = 0;

        for (;parent;){
            child = parent->_left;
            if (child){
                grand = _rotate_right(grand, parent, child);
                parent = child;
            } else {
                ++len;
                grand = parent;
                parent = parent->_right;
            }
        }

        return len;
    }

    void _erase_copy(Node*& node) {
        if (!node)
            return;

        if (node->_count > 1){
            node->_count--;
            return;
        }

        Node* prev = node;
        Node* it = node;

        if (!node->_right)
            node = node->_left;
        else if (!node->_left)
            node = node->_right;
        else {
            it = node->_left;
            prev = node;

            for (; it->_right;){
                prev = it;
                it = it->_right;
            }

            node->_data = it->_data;
            node->_count = it->_count;
            if (prev == node)
                prev->_left = it->_left;
            else prev->_right = it->_right;
        }

        delete it;
    }

    void _erase_merge(Node*& node) {
        if (!node)
            return;

        if (node->_count > 1){
            node->_count--;
            return;
        }

        Node* prev = node;

        if (!node->_right){
            node = node->_left;
        } else if (!node->_left){
            node = node->_right;
        } else {
            prev = node->_left;
            for (; prev->_right; prev = prev->_right);

            prev->_right = node->_right;

            prev = node;
            node = node->_left;
        }

        delete prev;
    }

    Node* _find(Node* root, const T& val) const {
        for (;root;){
            if (val == root->_data)
                return root;
            else if (comp(val, root->_data))
                root = root->_left;
            else root = root->_right;
        }

        return nullptr;
    }

    template<typename F>
    void _preorder(Node* root, F&& func) {
        if (root){
            func(root->_data);
            _preorder(root->_left, func);
            _preorder(root->_right, func);
        }
    }

    template<typename F>
    void _inorder(Node* root, F&& func) {
        if (root){
            _inorder(root->_left, func);
            func(root->_data);
            _inorder(root->_right, func);
        }
    }

    template<typename F>
    void _postorder(Node* root, F&& func) {
        if (root){
            _postorder(root->_left, func);
            _postorder(root->_right, func);
            func(root->_data);
        }
    }

    Compare comp;

    Node* m_root{nullptr};

};

} //DS namespace

#endif // BST_HPP
