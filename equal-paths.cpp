#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool equalPaths_helper_funct(Node* node, int currentDepth, int& depth) {

    if (node == nullptr) return true;
    if (node->left == nullptr && node->right == nullptr) { // if leaf
        if (depth == -1) {
            depth = currentDepth;  
            return true;
        } else {
            return depth == currentDepth;
        }
    }

    // recurse left and right subtrees
    return equalPaths_helper_funct(node->left, currentDepth + 1, depth) && equalPaths_helper_funct(node->right, currentDepth + 1, depth);
}


bool equalPaths(Node* root) {
    if (root == nullptr)
        return true;

    int depth = -1;
    return equalPaths_helper_funct(root, 0, depth);
}