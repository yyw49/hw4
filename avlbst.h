#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance() const;
    void setBalance(int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char

};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    //AVLTree();
    //~AVLTree();
    virtual void insert(const std::pair<const Key, Value>& new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2);

    // Add helper functions here
    void insert_fix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void remove_fix(AVLNode<Key, Value>* n, int8_t diff);
    void rightRotate(AVLNode<Key, Value>* p);
    void leftRotate(AVLNode<Key, Value>* p);
    void adjustBalancesAfterDoubleRotation(AVLNode<Key, Value>* g, AVLNode<Key, Value>* n, AVLNode<Key, Value>* c);

};

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* p) 
{
    
    if (static_cast<AVLNode<Key, Value>*>(this->root_) == p)
    {
      this->root_ = p->getLeft();

    if (this->root_)
    {
      p->setLeft(this->root_->getRight());
    }
    if (this->root_->getRight())
    {
      this->root_->getRight()->setParent(p);
    }
    this->root_->setRight(p);
    this->root_->setParent(nullptr);

    p->setParent(static_cast<AVLNode<Key, Value>*>(this->root_));
    return;
    }

    AVLNode<Key, Value>* q = p->getParent();

    if (q->getLeft() == p)
    {
      p->setParent(p->getLeft());
      q->setLeft(p->getLeft());
      p->setParent(p->getLeft());
      p->setLeft(q->getLeft()->getRight());

      if (q->getLeft()->getRight())
      {
        q->getLeft()->getRight()->setParent(p);
      }
      if (q->getLeft())
      {
      q->getLeft()->setRight(p);
      q->getLeft()->setParent(q);
      }
    }
    else
    {
      AVLNode<Key, Value>* pLeft = p->getLeft();
     //AVLNode<Key, Value>* qRight = q->getRight();

      q->setRight(pLeft);
      p->setParent(pLeft);
      p->setLeft(q->getRight()->getRight());

      if (q->getRight()->getRight())
      {
        q->getRight()->getRight()->setParent(p);
      }

      q->getRight()->setParent(q);
      q->getRight()->setRight(p);
    }
    return;

}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* p) 
{
    if (static_cast<AVLNode<Key, Value>*>(this->root_) == p) 
    {
    AVLNode<Key, Value>* newRoot = p->getRight();  

    p->setRight(newRoot->getLeft());
    if (newRoot->getLeft()) {
        newRoot->getLeft()->setParent(p);  
    }

    this->root_ = newRoot;
    newRoot->setParent(nullptr);  

    newRoot->setLeft(p);
    p->setParent(newRoot);  
    return;
}
    AVLNode<Key, Value>* g = p->getParent();

if (p == g->getRight()) 
{
    AVLNode<Key, Value>* pRightChild = p->getRight();

    g->setRight(pRightChild);
    if (pRightChild) pRightChild->setParent(g);

    p->setRight(pRightChild ? pRightChild->getLeft() : nullptr);
    if (p->getRight()) p->getRight()->setParent(p);

    if (pRightChild) pRightChild->setLeft(p);
    p->setParent(pRightChild);
} else 
{
    AVLNode<Key, Value>* pRightChild = p->getRight();

    g->setLeft(pRightChild);
    if (pRightChild) pRightChild->setParent(g);

    p->setRight(pRightChild ? pRightChild->getLeft() : nullptr);
    if (p->getRight()) p->getRight()->setParent(p);

    if (pRightChild) pRightChild->setLeft(p);
    p->setParent(pRightChild);
}

return;
}


/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item)
{
    // TODO
   if (!this->root_) {
    this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    return;
}

AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
bool inserted_at_left = true;

while (temp) {
    if (temp->getKey() == new_item.first) 
    {
        temp->setValue(new_item.second);
        return;
    }

    if (new_item.first < temp->getKey()) 
    {
        if (!temp->getLeft()) {
            temp->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, temp));
            inserted_at_left = true;
            break;
        }
        temp = temp->getLeft();
    } else {
        if (!temp->getRight()) {
            temp->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, temp));
            inserted_at_left = false;
            break;
        }
        temp = temp->getRight();
    }
}
    int8_t pbalance = temp->getBalance();

// If the balance is not zero, set it to zero and return (no further action needed)
if (pbalance != 0) 
{
    temp->setBalance(0);
} 
else 
{
    
    temp->setBalance(inserted_at_left ? -1 : 1);

    
    insert_fix(temp, inserted_at_left ? temp->getLeft() : temp->getRight());
}
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
   if (!p || !p->getParent()) return;
AVLNode<Key, Value>* g = p->getParent();

if (p == g->getLeft()) {
    g->setBalance(g->getBalance() - 1);
    int8_t gbalance = g->getBalance();

    switch (gbalance) {
        case 0:
            
            break;
        case -1:
            
            insert_fix(g, p);
            break;
        case -2:
            
            if (n == p->getLeft()) {
                // Zig-zig case
                rightRotate(g);
                g->setBalance(0);
                p->setBalance(0);
            } else {
                // Zig-zag case
                leftRotate(p);
                rightRotate(g);
                int8_t nbalance = n->getBalance();
                if (nbalance == 0) {
                    g->setBalance(0);
                    p->setBalance(0);
                } else {
                    g->setBalance(nbalance == 1 ? 0 : 1);
                    p->setBalance(nbalance == -1 ? 0 : -1);
                    n->setBalance(0);
                }
            }
            break;
        default:
            break;
    }
}
    else {
    g->setBalance(g->getBalance() + 1);
    int8_t gbalance = g->getBalance();

    switch (gbalance) {
        case 0:
            // Tree height hasn't changed, so no further action is required.
            break;
        case 1:
            // Tree is unbalanced but within limits, continue up the tree.
            insert_fix(g, p);
            break;
        case 2:
            // Tree is unbalanced and requires rotations.
            if (n == p->getRight()) {
                // Zig-zig scenario
                leftRotate(g);
                g->setBalance(0);
                p->setBalance(0);
            } else {
                // Zig-zag scenario
                rightRotate(p);
                leftRotate(g);
                
                int8_t nbalance = n->getBalance();
                // Reset balances based on 'n's balance before the rotations
                g->setBalance(nbalance == 1 ? -1 : 0);
                p->setBalance(nbalance == -1 ? 1 : 0);
                n->setBalance(0);
            }
            break;
        default:
            // This case should not occur as balance can only be -2, -1, 0, 1, or 2
            break;
    }
}
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
if (!n) {
    
    return;
}


if (n->getLeft() && n->getRight()) 
{
    
    AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(n));
    
    nodeSwap(n, pred);
}

AVLNode<Key, Value>* p = n->getParent();
int diff = 0; 

    if (p) {
    diff = (n == p->getLeft()) ? 1 : -1;

    AVLNode<Key, Value>* child = n->getLeft() ? n->getLeft() : n->getRight();

    if (n == p->getLeft()) 
    {
        p->setLeft(child);
    } else 
    {
        p->setRight(child);
    }

    if (child) 
    {
        child->setParent(p);
    }
}
    else 
    {
        if (!n->getLeft() && !n->getRight()) 
        {
    // `n` is the root and has no children, so the tree becomes empty
    this->root_ = nullptr;
} else 
{
    // Set the root to `n`'s non-null child and clear its parent
    this->root_ = n->getLeft() ? n->getLeft() : n->getRight();
    this->root_->setParent(nullptr);
}
    }

    delete n;
    remove_fix(p, diff);

}

//error here wtf
template <class Key, class Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key, Value>* n, int8_t diff) {
    while (n) 
    {
        AVLNode<Key, Value>* p = n->getParent();
        int8_t ndiff = 0;

        n->setBalance(n->getBalance() + diff);

        if (n->getBalance() == -2 || n->getBalance() == 2) {
            AVLNode<Key, Value>* c = (n->getBalance() == -2) ? n->getLeft() : n->getRight();
            if (!c) break; // Prevent segmentation fault

            int8_t cbalance = c->getBalance();
            if (n->getBalance() == -2) 
            {
                if (cbalance <= 0) 
                {
                    rightRotate(n);
                    if (cbalance == -1) 
                    {
                        n->setBalance(0);
                        c->setBalance(0);
                    } else 
                    { // cbalance == 0
                        n->setBalance(-1);
                        c->setBalance(1);
                    }
                } 
                else 
                { // cbalance == 1
                    AVLNode<Key, Value>* g = c->getRight();
                    if (!g) break; 

                    leftRotate(c);
                    rightRotate(n);
                    adjustBalancesAfterDoubleRotation(g, n, c);
                }
            } 
            else 
            { // n->getBalance() == 2
                if (cbalance >= 0) {
                    leftRotate(n);
                    if (cbalance == 1) {
                        n->setBalance(0);
                        c->setBalance(0);
                    } else { // cbalance == 0
                        n->setBalance(1);
                        c->setBalance(-1);
                    }
                } 
                else 
                { // cbalance == -1
                    AVLNode<Key, Value>* g = c->getLeft();
                    if (!g) break; // Prevent segmentation fault

                    rightRotate(c);
                    leftRotate(n);
                    adjustBalancesAfterDoubleRotation(g, n, c);
                }
            }
        } else if (n->getBalance() == 0) 
        {
            if (p) {
                ndiff = (n == p->getLeft()) ? -1 : 1;
            }
        } else 
        {
            break; // The tree is balanced
        }

        n = p;
        diff = ndiff;
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::adjustBalancesAfterDoubleRotation(AVLNode<Key, Value>* g, AVLNode<Key, Value>* n, AVLNode<Key, Value>* c) {
    int8_t gbalance = g->getBalance();

    if (gbalance == 1) 
    {
        n->setBalance(-1);
    } else if (gbalance == -1) 
    {
        n->setBalance(1);
    } else {
        n->setBalance(0);
    }

    if (gbalance == 1 || gbalance == -1) 
    {
        c->setBalance(0);
    } else {
        c->setBalance(1);
    }

    g->setBalance(0);
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
