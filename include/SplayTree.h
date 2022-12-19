#pragma once

#include <vector>

class SplayTree
{
private:
    struct Node
    {
        Node * left = nullptr;
        Node * right = nullptr;
        int value;
        Node * parent = nullptr;

        Node(int value, Node * parent)
            : value(value)
            , parent(parent)
        {
        }
    };

    std::size_t tree_size = 0;
    mutable Node * root = nullptr;

    static void set_child(Node * child, Node * to_set);

    static Node * get_grand_parent(const Node * x);

    void rotate(Node * x) const;

    void splay(Node * x) const;

    static void get_values(std::vector<int> & result, const Node * cur);

    static void erase_all_nodes(Node * cur);

    static Node * find(int value, Node * cur);

public:
    bool contains(int value) const;
    bool insert(int value);
    bool remove(int value);

    std::size_t size() const;
    bool empty() const;

    std::vector<int> values() const;

    ~SplayTree();
};
