#ifndef AVL_TREE
#define AVL_TREE

#include <list>

template<class Key, class Val>
struct Node {

    Key key_;
    Val val_;
    Node * left_child_;
    Node * right_child_;
    int height_;

    Node(Key key, Val val) {
        key_ = key;
        val_ = val;
        left_child_ = nullptr;
        right_child_ = nullptr;
        height_ = 1;
    }

    Node(Key key, Val val, Node<Key, Val> * left_child, Node<Key, Val> * right_child, int height) {
        key_ = key;
        val_ = val;
        left_child_ = left_child;
        right_child_ = right_child;
        height_ = height;
    }
};

template<class Key, class Val>
class AVLTree {

    Node<Key, Val> * root_;
    int size_;

    Node<Key, Val> * InOrderSuccessor(Node<Key, Val> * root) {
        Node<Key, Val> * itr = root->right_child_;
        while (itr->left_child_ != nullptr)
        {
            itr = itr->left_child_;
        }
        return itr;
    }

    int CountChildren(Node<Key, Val> * node) {
        int count = 0;
        if (node->left_child_) count++;
        if (node->right_child_) count++;
        return count;
    }

    void UpdateHeight(Node<Key, Val> * root) {

        int left_height = 1;
        int right_height = 1;

        if (root->right_child_)
        {
            right_height = root->right_child_->height_ + 1;
        }

        if (root->left_child_)
        {
            left_height = root->left_child_->height_ + 1;
        }

        root->height_ = std::max(left_height, right_height);
    }

    Node<Key, Val> * RotateRight(Node<Key, Val> * root) {
        Node<Key, Val> * to_be_replaced = root->left_child_->right_child_;
        root->left_child_->right_child_ = root;
        root = root->left_child_;
        root->right_child_->left_child_ = to_be_replaced;

        UpdateHeight(root->right_child_);
        UpdateHeight(root);

        return root;
    }

    Node<Key, Val> * BalanceLeft(Node<Key, Val> * root) {
        bool single_rotation = BalanceFactor(root->left_child_) >= 0;

        if (single_rotation)
        {
            return RotateRight(root);
        }
        else
        {
            root->left_child_ = RotateLeft(root->left_child_);
            return RotateRight(root);
        }
    }

    Node<Key, Val> * RotateLeft(Node<Key, Val> * root) {
        Node<Key, Val> * to_be_replaced = root->right_child_->left_child_;
        root->right_child_->left_child_ = root;
        root = root->right_child_;
        root->left_child_->right_child_ = to_be_replaced;

        UpdateHeight(root->left_child_);
        UpdateHeight(root);

        return root;
    }

    Node<Key, Val> * BalanceRight(Node<Key, Val> * root) {
        bool single_rotation = BalanceFactor(root->right_child_) <= 0;

        if (single_rotation)
        {
            return RotateLeft(root);
        }
        else
        {
            root->right_child_ = RotateRight(root->right_child_);
            return RotateLeft(root);
        }
    }

    int BalanceFactor(Node<Key, Val> * root) {

        int left_height = 0;
        int right_height = 0;

        if (root->right_child_)
        {
            right_height = root->right_child_->height_;
        }

        if (root->left_child_)
        {
            left_height = root->left_child_->height_;
        }

        return left_height - right_height;
    }

    Node<Key, Val> * BalanceTree(Node<Key, Val> * root) {

        int balance_factor = BalanceFactor(root);
        bool is_balanced = balance_factor >= -1 && balance_factor <= 1;
        bool left_heavy = balance_factor == 2;

        if (is_balanced)
        {
            return root;
        }
        else if (left_heavy)
        {
            return BalanceLeft(root);
        }
        else
        {
            return BalanceRight(root);
        }
    }

    Node<Key, Val> * InsertRecursively(Node<Key, Val> * root, Key key, Val val) {

        if (root == nullptr)
        {
            return new Node<Key, Val>(key, val);
        }

        if (key > root->key_)
        {
            root->right_child_ = InsertRecursively(root->right_child_, key, val);
        }
        else if (key < root->key_)
        {
            root->left_child_ = InsertRecursively(root->left_child_, key, val);
        }
        else
        {
            root->val_ = val;
            return root;
        }

        UpdateHeight(root);
        return BalanceTree(root);
    }

    Node<Key, Val> * RemoveNode(Node<Key, Val> * root) {
        int num_children = CountChildren(root);
        if (num_children == 1)
        {
            Node<Key, Val> * to_delete = root;
            if (root->right_child_)
            {
                root = root->right_child_;
            }
            else
            {
                root = root->left_child_;
            }
            delete to_delete;
        }
        else
        {
            delete root;
            root = nullptr;
        }
        return root;
    }

    Node<Key, Val> * RemoveRecursively(Node<Key, Val> * root, Key key) {

        if (key > root->key_)
        {
            root->right_child_ = RemoveRecursively(root->right_child_, key);
        }
        else if (key < root->key_)
        {
            root->left_child_ = RemoveRecursively(root->left_child_, key);
        }
        else
        {
            return RemoveNode(root);
        }

        UpdateHeight(root);
        return BalanceTree(root);
    }


    void RecursivelyDeleteNodes(Node<Key, Val> * root) {

        if (root == nullptr)
        {
            return;
        }

        RecursivelyDeleteNodes(root->left_child_);
        RecursivelyDeleteNodes(root->right_child_);

        delete root;
    }

    void RecursiveInOrder(Node<Key, Val> * root, std::list<Node<Key, Val> *> & traversal) {

        if (root == nullptr)
        {
            return;
        }

        RecursiveInOrder(root->left_child_, traversal);
        traversal.push_back(root);
        RecursiveInOrder(root->right_child_, traversal);
    }

    void RecursivePostOrder(Node<Key, Val> * root, std::list<Node<Key, Val> *> & traversal) {

        if (root == nullptr)
        {
            return;
        }

        RecursivePostOrder(root->left_child_, traversal);
        RecursivePostOrder(root->right_child_, traversal);
        traversal.push_back(root);
    }

    void RecursivePreOrder(Node<Key, Val> * root, std::list<Node<Key, Val> *> & traversal) {

        if (root == nullptr)
        {
            return;
        }

        traversal.push_back(root);
        RecursivePreOrder(root->left_child_, traversal);
        RecursivePreOrder(root->right_child_, traversal);
    }

public:

    AVLTree() {
        root_ = nullptr;
        size_ = 0;
    }

    ~AVLTree() {
        Clear();
    }

    Node<Key, Val> * Find(Key key) {

        Node<Key, Val> * itr = root_;
        while (itr != nullptr && itr->key_ != key)
        {
            if (key < itr->key_)
            {
                itr = itr->left_child_;
            }
            else if (key > itr->key_)
            {
                itr = itr->right_child_;
            }
        }
        return itr;
    }

    void Insert(Key key, Val val) {
        root_ = InsertRecursively(root_, key, val);
        size_++;
    }

    void Remove(Key key) {

        Node<Key, Val> * to_remove = Find(key);

        if (!to_remove)
        {
            return;
        }

        int num_children = CountChildren(to_remove);
        if (num_children == 2)
        {
            Node<Key, Val> * successor = InOrderSuccessor(to_remove);
            Key key_to_swap = successor->key_;
            Val val_to_swap = successor->val_;

            root_ = RemoveRecursively(root_, successor->key_);

            to_remove->key_ = key_to_swap;
            to_remove->val_ = val_to_swap;
        }
        else
        {
            root_ = RemoveRecursively(root_, key);
        }
        size_--;
    }

    void Clear() {
        RecursivelyDeleteNodes(root_);
        root_ = nullptr;
        size_ = 0;
    }

    int Size() {
        return size_;
    }

    bool Empty() {
        return size_ == 0;
    }

    int Height() {
        return root_ ? root_->height_ : 0;
    }

    std::list<Node<Key, Val> *> InOrderTraversal() {
        std::list<Node<Key, Val> *> traversal;
        RecursiveInOrder(root_, traversal);
        return traversal;
    }

    std::list<Node<Key, Val> *> PostOrderTraversal() {
        std::list<Node<Key, Val> *> traversal;
        RecursivePostOrder(root_, traversal);
        return traversal;
    }

    std::list<Node<Key, Val> *> PreOrderTraversal() {
        std::list<Node<Key, Val> *> traversal;
        RecursivePreOrder(root_, traversal);
        return traversal;
    }
};

#endif