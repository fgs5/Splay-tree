#include "SplayTree.h"

#include <vector>

void SplayTree::set_child(Node * child, Node * to_set)
{
    Node * parent = child->parent;
    if (parent != nullptr) {
        if (parent->value > child->value) {
            parent->left = to_set;
        }
        else {
            parent->right = to_set;
        }
    }
}

SplayTree::Node * SplayTree::get_grand_parent(const Node * x)
{
    return x->parent == nullptr ? nullptr : x->parent->parent;
}

void SplayTree::get_values(std::vector<int> & result, const Node * cur)
{
    if (cur != nullptr) {
        get_values(result, cur->left);
        result.push_back(cur->value);
        get_values(result, cur->right);
    }
}

void SplayTree::erase_all_nodes(Node * cur)
{
    if (cur != nullptr) {
        erase_all_nodes(cur->left);
        erase_all_nodes(cur->right);
        delete cur;
    }
}

void SplayTree::rotate(Node * x) const
{
    bool is_left = (x == x->parent->right);
    Node * parent = x->parent;
    Node * son = x->right;
    if (is_left) {
        son = x->left;
        parent->right = son;
        x->left = parent;
    }
    else {
        parent->left = son;
        x->right = parent;
    }
    if (son != nullptr) {
        son->parent = parent;
    }
    x->parent = get_grand_parent(x);
    set_child(x, x);
    parent->parent = x;
}

void SplayTree::splay(Node * x) const
{
    if (x == nullptr) {
        return;
    }
    root = x;
    while (x->parent != nullptr) {
        if (get_grand_parent(x) != nullptr) {
            Node * to_rotate = x;
            if ((x == x->parent->left && x->parent == get_grand_parent(x)->left) || (x == x->parent->right && x->parent == get_grand_parent(x)->right)) {
                to_rotate = x->parent;
            }
            rotate(to_rotate);
        }
        rotate(x);
    }
}

SplayTree::Node * SplayTree::find(int value, Node * cur)
{
    SplayTree::Node * next = cur;
    while (next != nullptr && cur->value != value) {
        cur = next;
        next = value < cur->value ? cur->left : cur->right;
    }
    return cur;
}

std::size_t SplayTree::size() const
{
    return tree_size;
}

bool SplayTree::contains(int value) const
{
    Node * result = find(value, root);
    splay(result);
    return result != nullptr && result->value == value;
}

bool SplayTree::insert(int value)
{
    Node * result = find(value, root);
    if (result == nullptr || result->value != value) {
        if (empty()) {
            root = new Node(value, nullptr);
        }
        else {
            Node * to_add = new Node(value, result);
            set_child(to_add, to_add);
            splay(to_add);
        }
        ++tree_size;
        return true;
    }
    splay(result);
    return false;
}

bool SplayTree::remove(int value)
{
    if (!contains(value)) {
        return false;
    }
    Node * cur = root;
    Node * temp = cur;
    if (cur->right != nullptr) {
        temp = find(cur->value, cur->right);
        cur->value = temp->value;
        cur = temp;
    }
    Node * to_set = (cur->left != nullptr) ? cur->left : cur->right;
    if (to_set != nullptr) {
        cur = to_set;
        cur->parent = temp->parent;
    }
    if (temp == root) {
        root = to_set;
    }
    set_child(cur, to_set);
    delete temp;
    --tree_size;
    return true;
}

bool SplayTree::empty() const
{
    return size() == 0;
}

std::vector<int> SplayTree::values() const
{
    std::vector<int> result;
    result.reserve(size());
    get_values(result, root);
    return result;
}

SplayTree::~SplayTree()
{
    erase_all_nodes(root);
}
