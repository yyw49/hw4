#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkLeafDepth(Node* node, int depth, int& leafDepth);

bool equalPaths(Node * root)
{
    int leafDepth = -1;  
    return checkLeafDepth(root, 0, leafDepth);
}

bool checkLeafDepth(Node* node, int depth, int& leafDepth)
{
    if (node == nullptr)
    {
        return true;
    }

    if (node->left == nullptr and node->right == nullptr)
    {
        if (leafDepth == -1)
        {
            leafDepth = depth;
        }
        return depth == leafDepth;
    }

    return checkLeafDepth(node->left, depth+1, leafDepth) && 
            checkLeafDepth(node->right, depth+1, leafDepth);
}




