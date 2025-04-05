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
    int8_t getBalance () const;
    void setBalance (int8_t balance);
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
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
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
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
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
    virtual void insert(const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here

    void rotateRight(AVLNode<Key,Value>* curr, AVLNode<Key,Value>* parent);
    void rotateLeft(AVLNode<Key,Value>* curr, AVLNode<Key,Value>* parent);

    void insertFix(AVLNode<Key,Value>* curr, AVLNode<Key,Value>* parent);
    void removeFix(AVLNode<Key,Value>* curr, int dif);
    
    //AVLNode<Key, Value>* internalFind(const Key& key) const override; // can this be imported from regular bst
    AVLNode<Key, Value>* findAVLNode(const Key& key) const;

};





template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::findAVLNode(const Key& key) const
{
    Node<Key, Value>* curr = this->root_;
    while(curr) {
        if(key == curr->getKey()) {
            return static_cast<AVLNode<Key, Value>*>(curr);
        } 
        else if(key < curr->getKey()) {
            curr = curr->getLeft();
        }
        else {
            curr = curr->getRight();
        }
    }
    return nullptr;
}

// template<typename Key, typename Value>
// AVLNode<Key, Value>* AVLTree<Key, Value>::internalFind(const Key& key) const
// {
//     Node<Key, Value>* curr = this->root_;
//     while (curr != nullptr) {
//         if (curr->getKey() == key) {
//             return static_cast<AVLNode<Key, Value>*>(curr);
//         }
//         else if (key < curr->getKey()) {
//             curr = curr->getLeft();
//         }
//         else {
//             curr = curr->getRight();
//         }
//     }
//     return nullptr;
// }



template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* curr)
{
    AVLNode<Key, Value>* grandparent = parent->getParent();

    // reconnect gp
    if(grandparent) {
        if (grandparent->getLeft() == parent) {
            grandparent->setLeft(curr);
        } else {
            grandparent->setRight(curr);
        }
    } else {
        this->root_ = curr;
    }

    //  reset child pointers
    curr->setParent(grandparent);
    parent->setParent(curr);

    // parent->setRight(curr->getLeft());
    // if (curr->getLeft()) {
    //     curr->getLeft()->setParent(parent);
    // }
    AVLNode<Key, Value>* temp = curr->getLeft();
    parent->setRight(temp);
    if(temp) {
        temp->setParent(parent);
    }




    curr->setLeft(parent);

    // change balance rr:
    // int8_t parent_balance = parent->getBalance();
    // int8_t curr_balance = curr->getBalance();

    // if(curr_balance < 0) {
    //     parent->setBalance(parent_balance + 1 - curr_balance);
    // } 
    
    // else {
    //     parent->setBalance(parent_balance + 1);
    // }


    // if(parent_balance > 0) {
    //     curr->setBalance(curr_balance + 1 + parent_balance);
    // } 
    // else {
    //     curr->setBalance(curr_balance + 1);
    // }
}
 

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* curr)
{
    AVLNode<Key, Value>* grandparent = parent->getParent();

    // reconnect gp to curr
    if (grandparent) {
        if (grandparent->getLeft() == parent) {
            grandparent->setLeft(curr);
        } else {
            grandparent->setRight(curr);
        }
    } else {
        this->root_ = curr;
    }

    // update parent and curr pointers
    curr->setParent(grandparent);
    parent->setParent(curr);

    // parent->setLeft(curr->getRight());
    // if (curr->getRight()) {
    //     curr->getRight()->setParent(parent);
    // }
    AVLNode<Key, Value>* temp = curr->getRight();
    parent->setLeft(temp);
    if(temp) {
        temp->setParent(parent);
    }


    curr->setRight(parent);

    // rebalace for LL

    // int8_t parent_balance = parent->getBalance();
    // int8_t curr_balance = curr->getBalance();
    // if(curr_balance > 0) {
    //     parent->setBalance(parent_balance - 1 + curr_balance);
    // } 
    // else {
    //     parent->setBalance(parent_balance - 1);
    // }

    // if(parent_balance < 0) {
    //     curr->setBalance(curr_balance - 1 - parent_balance);
    // } else {
    //     curr->setBalance(curr_balance - 1);
    // }
}




template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* curr, AVLNode<Key, Value>* parent)
{
    if(!parent || !parent->getParent()) return;

    AVLNode<Key, Value>* grandparent = parent->getParent();

    // left child side case
    if(grandparent->getLeft() == parent) {
        grandparent->updateBalance(-1);

        if(grandparent->getBalance() == 0) {
            return;
        }
        else if(grandparent->getBalance() == -1) {
            insertFix(parent, grandparent);
        }
        else if (grandparent->getBalance() == -2) {
            if (parent->getLeft() == curr) {// LL case
                rotateRight(grandparent, parent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else {  // LR case
                rotateLeft(parent, curr);
                rotateRight(grandparent, curr);
                if (curr->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                }
                else if (curr->getBalance() == -1) {
                    parent->setBalance(0);
                    grandparent->setBalance(1);
                }
                else if(curr->getBalance() == 1) {
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                }
                curr->setBalance(0);
            }
        }
    }

    // right child case
    else {
        grandparent->updateBalance(1);

        if(grandparent->getBalance() == 0) {
            return;
        }
        else if(grandparent->getBalance() == 1) {
            insertFix(parent, grandparent);
        }
        else if(grandparent->getBalance() == 2) {
            if(parent->getRight() == curr) { // RR case
                rotateLeft(grandparent, parent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else { // RL case
                rotateRight(parent, curr);
                rotateLeft(grandparent, curr);
                if(curr->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                }
                else if(curr->getBalance() == 1) {
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                }
                else if(curr->getBalance() == -1) {
                    parent->setBalance(1);
                    grandparent->setBalance(0);
                }
                curr->setBalance(0);
            }
        }
    }
}




/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    newNode->setBalance(0);

    if (this->root_ == nullptr) {
        this->root_ = newNode;
        return;
    }

    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while(current) {
        parent = current;
        if(new_item.first < current->getKey()) {
            current = current->getLeft();
        } 
        else if(new_item.first > current->getKey()) {
            current = current->getRight();
        } 
        
        else{
            current->setValue(new_item.second);
            delete newNode;
            return;
        }
    }

    newNode->setParent(parent);
    if(new_item.first < parent->getKey()) {
        parent->setLeft(newNode);
    } 
    else {
        parent->setRight(newNode);
    }

    // Rebalance if needed
    if (parent->getBalance() == 0) {
        if(newNode == parent->getLeft()) {
            parent->setBalance(-1);
        }

        else {
            parent->setBalance(1);
        }
        insertFix(newNode, parent);
    } 
    else {
    if ((parent->getBalance() == -1 && newNode == parent->getRight()) ||
        (parent->getBalance() == 1 && newNode == parent->getLeft())) {
        parent->setBalance(0); // balanced out
    } else {
        insertFix(newNode, parent); // imbalance worsened
    }
    }
} // check later




template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* curr, int dif)
{
    if (!curr) return;

    AVLNode<Key, Value>* parent = curr->getParent();
    int parentDif = 0;
    if(parent) {
        if(parent->getLeft() == curr) {
            parentDif = 1;  // We came from left → left subtree is now shorter
        }
        else {
            parentDif = -1; // We came from right → right subtree is now shorter
        }
    }

    int balance = curr->getBalance();
    int newBalance = balance + dif;

    if (dif == -1) {
        // Left subtree was shortened
        if(newBalance == -1) {
            curr->setBalance(-1);
            //return;
        }
        else if (newBalance == 0) {
            curr->setBalance(0);
            removeFix(parent, parentDif);
        }
        else if (newBalance == -2) {
            AVLNode<Key, Value>* child = curr->getLeft();
            int childBalance = child->getBalance();

            if (childBalance == -1) {
                rotateRight(curr, child);
                curr->setBalance(0);
                child->setBalance(0);
                removeFix(parent, parentDif);
            }
            else if (childBalance == 0) {
                rotateRight(curr, child);
                curr->setBalance(-1);
                child->setBalance(1);
                //return;
            }
            else if (childBalance == 1) {
                AVLNode<Key, Value>* grandchild = child->getRight();
                rotateLeft(child, grandchild);
                rotateRight(curr, grandchild);

                if (!grandchild) return; // bug
                int grandchildBalance = grandchild->getBalance();
                if (grandchildBalance == 1) {
                    curr->setBalance(0);
                    child->setBalance(-1);
                } 
                else if (grandchildBalance == 0) {
                    curr->setBalance(0);
                    child->setBalance(0);
                } 
                else {
                    curr->setBalance(1);
                    child->setBalance(0);
                }
                grandchild->setBalance(0);
                removeFix(parent, parentDif);
                return;
            }
        }
    }
    else if (dif == 1) {
        // Right subtree was shortened
        if (newBalance == 1) {
            curr->setBalance(1);
            //return;
        }
        else if (newBalance == 0) {
            curr->setBalance(0);
            removeFix(parent, parentDif);
        }
        else if (newBalance == 2) {
            AVLNode<Key, Value>* child = curr->getRight();
            int childBalance = child->getBalance();

            if (childBalance == 1) {
                rotateLeft(curr, child);
                curr->setBalance(0);
                child->setBalance(0);
                removeFix(parent, parentDif);
            }
            else if (childBalance == 0) {
                rotateLeft(curr, child);
                curr->setBalance(1);
                child->setBalance(-1);
                //return;
            }
            else if (childBalance == -1) {
                AVLNode<Key, Value>* grandchild = child->getLeft();
                rotateRight(child, grandchild);
                rotateLeft(curr, grandchild);
                if (!grandchild) return;//bug
                int grandchildBalance = grandchild->getBalance();
                if (grandchildBalance == -1) {
                    curr->setBalance(0);
                    child->setBalance(1);
                } else if (grandchildBalance == 0) {
                    curr->setBalance(0);
                    child->setBalance(0);
                } else {
                    curr->setBalance(-1);
                    child->setBalance(0);
                }
                grandchild->setBalance(0);
                removeFix(parent, parentDif);
                return;
            }
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
    AVLNode<Key, Value>* nodeDelete = static_cast<AVLNode<Key, Value>*>(this->findAVLNode(key));
    if(!nodeDelete) return;

    // two children case
    if(nodeDelete->getLeft() && nodeDelete->getRight()) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeDelete));
        nodeSwap(nodeDelete, pred);
    }

    // 1 or 0 children
    AVLNode<Key, Value>* child = nullptr;
    if(nodeDelete->getLeft()) {
        child = nodeDelete->getLeft();
    }
    else if(nodeDelete->getRight()) {
        child = nodeDelete->getRight();
    }

    AVLNode<Key, Value>* parent = nodeDelete->getParent();
    int dif = 0;

    if(parent) {
        if (parent->getLeft() == nodeDelete) {
            dif = 1;
        }
        else {
            dif = -1;
        }
    }

    // if theres a child, recconect it
    if(child) {
        child->setParent(parent);
    }

    // do same for parent
    if(!parent) {
        this->root_ = child;
    } else {
        if(parent->getLeft() == nodeDelete) {
            parent->setLeft(child);
        }
        else {
            parent->setRight(child);
        }
    }

    delete nodeDelete;
    removeFix(parent, dif);
}



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif