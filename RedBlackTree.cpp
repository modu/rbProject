#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "RedBlackTree.h"
using namespace std;


void RBTree::inorder(NODEPTR x) {
    if (x != NULL) {
        inorder(x->left);
        printf("%d ", x->key);
        inorder(x->right);
    }
}

NODEPTR RBTree::search(NODEPTR root, int k) {
    if (root == NULL || root->key == k)
        return root;
    if (k < root->key)
        return search(root->left, k);
    else
        return search(root->right, k);
}

NODEPTR RBTree::minimum(NODEPTR root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}
/*Searches right most in BST manner*/
NODEPTR RBTree::maximum(NODEPTR root) {
    while (root->right != NULL)
        root = root->right;
    return root;
}


NODEPTR RBTree::getClosestNode(NODEPTR pRoot, int value){
    NODEPTR pClosest = NULL;
    int minDistance = 0x7FFFFFFF;
    NODEPTR pNode = pRoot;
    while(pNode != NULL)
    {
        int distance = abs(pNode->key - value);
        if(distance < minDistance)
        {
            minDistance = distance;
            pClosest = pNode;
        }
        
        if(distance == 0)
            break;
        
        if(pNode->key > value)
            pNode = pNode->left;
        else if(pNode->key < value)
            pNode = pNode->right;
    }
    
    return pClosest;
}

/*Print the ID and the count of the event with the lowest ID that is greater that theID. Print “0 0”, if there is no next ID.
 Internally uses getCloasestNode and predecessor recursively to find nearest predecessor
 
 */
NODEPTR RBTree::predecessor(NODEPTR root, int x) {
    
    NODEPTR temp = search(root, x);
    if (temp == NULL) {
        //        printf("%d not in tree\n", x);
        NODEPTR temp1 = getClosestNode(root , x );
        if (temp1->key < x ) {
            return temp1;
        }
        else {
            return predecessor(root , temp1->key);
        }
    }
    NODEPTR currentPtr = temp;
    if (currentPtr->left != NULL)
        return maximum(currentPtr->left);
    
    NODEPTR y = currentPtr->p;
    while (y != NULL && currentPtr == y->left) {
        currentPtr = y;
        y = y->p;
    }
    return y;
}

/*Print the ID and the count of the event with the greatest key that is less that theID. Print “0 0”, if there is no previous ID.
 Internally uses getCloasestNode and predecessor recursively to find nearest successor

 */
NODEPTR RBTree::successor(NODEPTR root, int x) {
    NODEPTR temp = search(root, x);
    if (temp == NULL) {
        //        printf("%d not in tree\n", x);
        NODEPTR temp1 = getClosestNode(root, x);
        if (temp1->key < x ) {
            return successor(root , temp1->key);
        }
        else {
            return temp1;
        }
    }
    NODEPTR currentPtr = temp;
    if (currentPtr->right != NULL)
        return minimum(currentPtr->right);
    NODEPTR y = currentPtr->p;
    while (y != NULL && currentPtr == y->right) {
        currentPtr = y;
        y = y->p;
    }
    return y;
}

void RBTree::leftrotate(NODEPTR treeroot, NODEPTR x) {
    NODEPTR y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->p = x;
    y->p = x->p;
    if (x->p == NULL)
        treeroot = y;
    else if (x->p->left == x)
        x->p->left = y;
    else
        x->p->right = y;
    y->left = x;
    x->p = y;
}

void RBTree::rightrotate(NODEPTR treeroot, NODEPTR y) {
    NODEPTR x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->p = y;
    x->p = y->p;
    if (y->p == NULL)
        treeroot = x;
    else if (y->p->left == y)
        y->p->left = x;
    else
        y->p->right = x;
    x->right = y;
    y->p = x;
}


void RBTree::rbinsertfixup(NODEPTR treeroot, NODEPTR z) {
    while (z->p->color == RED) {
        if (z->p == z->p->p->left) {
            NODEPTR y = z->p->p->right;
            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else {
                if (z == z->p->right) {
                    z = z->p;
                    leftrotate(treeroot,z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                rightrotate(treeroot,z->p->p);
            }
        }
        else {
            NODEPTR y = z->p->p->left;
            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else {
                if (z == z->p->left) {
                    z = z->p;
                    rightrotate(treeroot,z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                leftrotate(treeroot,z->p->p);
            }
        }
    }
    (treeroot)->color = BLACK;
}


void RBTree::rbinsert(NODEPTR treeroot, int z, int _count) {
    NODEPTR Z = (NODEPTR) malloc(sizeof(struct Node));
    Z->key = z;
    Z->count = _count;
    NODEPTR y = NULL;
    NODEPTR x = treeroot;
    while (x != NULL) {
        y = x;
        if (Z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    Z->p = y;
    if (y == NULL)
        treeroot = Z;
    else if (Z->key < y->key)
        y->left = Z;
    else
        y->right = Z;
    Z->left = NULL;
    Z->right = NULL;
    Z->color = RED;
    cout<<Z->count;
    rbinsertfixup(treeroot,Z);
}


void RBTree::rbtransplant(NODEPTR treeroot, NODEPTR u, NODEPTR v) {
    if (u->p == NULL)
        treeroot = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}

/*Internal function to do rotations and fix of color to maintaint RB tree property
 */
void RBTree::rbdeletefixup(NODEPTR treeroot, NODEPTR x) {
    //cout<<"rbdeletefixup \n";
    while (x != treeroot && x->color == BLACK) {
        if (x == x->p->left) {
            NODEPTR w = x->p->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                leftrotate(treeroot,x->p);
                w = x->p->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->p;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightrotate(treeroot,w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                leftrotate(treeroot,x->p);
                x = treeroot;
            }
        }
        else {
            NODEPTR w = x->p->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                rightrotate(treeroot,x->p);
                w = x->p->left;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->p;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftrotate(treeroot,w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                rightrotate(treeroot,x->p);
                x = treeroot;
            }
        }
    }
   // cout<<"rbdeletefixup end \n";

    x->color = BLACK;
}

/*
 Red Black tree delete function
 */
void RBTree::rbdelete(NODEPTR treeroot, int z) {
    NODEPTR Z = search(treeroot, z);

    if (Z == NULL) {
        printf("Node to be deleted not found\n");
        return;
    }
    
    NODEPTR y = Z;
    int yoc = y->color;
    NODEPTR x;
    if (Z->left == NULL) {
        x = Z->right;
        //cout<<"rbdelete \n";
        rbtransplant(treeroot,Z,Z->right);
    }
    else if (Z->right == NULL) {
        x = Z->left;
        rbtransplant(treeroot,Z,Z->left);
    }
    else {
        y = minimum(Z->right);
        yoc = y->color;
        x = y->right;
        if (y->p == Z){
            //cout<<y->key; //<< " " << x->key;
            if (x != NULL ) x->p = y;
        }
        else {
            rbtransplant(treeroot,y,y->right);
            y->right = Z->right;
            y->right->p = y;
        }
        rbtransplant(treeroot,Z,y);
        y->left = Z->left;
        y->left->p = y;
        y->color = Z->color;
    }
    if (yoc == BLACK)
        rbdeletefixup(treeroot,x);
}

/*Internal function to make every leaf red after tree is recursively created*/

void RBTree::makeLeafRed(NODEPTR root){
    
    if (root == NULL) {
        return ;
    }
    if (root->left == NULL && root->right == NULL) {
        root->color = RED;
        return;
    }
    if (root->left !=NULL ) {
        makeLeafRed(root->left);
    }
    if (root->right != NULL) {
        makeLeafRed(root->right);
    }
}


/*Increase the count of the event theID by m. 
 If theID is not present, insert it. 
 Print the count of theID after the addition.
 Internally uses rbinsert to add the new element
 */
void RBTree::Increase(int theID , int m , NODEPTR root ){
    //    cout<<"Inside the increase\n";
    if (root == NULL) {
        rbinsert(root, theID , m);
        return ;
    }
    NODEPTR currentPtr = root;
    //    cout<<"Inside the increase\n";
    while (currentPtr!=NULL) {
        /*If ID is found then increment the counter and return */
        if (currentPtr->key == theID) {
            currentPtr->count = currentPtr->count + m;
            cout<<currentPtr->count;
            return;
        }
        /*if theID is greater then currentPtr key then search in right else search in left*/
        if (currentPtr->key < theID) {
            currentPtr = currentPtr->right;
        }
        else {
            currentPtr = currentPtr->left;
        }
        
    }
    if (currentPtr == NULL) {
        rbinsert(root, theID , m );
    }
}


/*Decrease the count of theID by m. If theID’s count becomes less than or equal to 0, remove theID from the counter. Print the count of theID after the deletion, or 0 if theID is removed or not present.
 Internally uses rbdelete once the count decrease less then or equal to zero
 */
void RBTree::Reduce( NODEPTR root,int theID , int m  ){
    if (root == NULL) {
        return ;
    }
    NODEPTR currentPtr = root;

    while (currentPtr!=NULL) {
        /*If ID is found then increment the counter and return */
        if (currentPtr->key == theID) {
            currentPtr->count = currentPtr->count - m;
            if (currentPtr->count <=0) {
                rbdelete(root, currentPtr->key);
                //cout<<"Deleted successfully\n   ";
                cout <<"0";
                return ;
            }
            cout<<currentPtr->count;
            return;
        }
        
        /*if theID is greater then currentPtr key then search in right else search in left*/
        if (currentPtr->key < theID) {
            currentPtr = currentPtr->right;
        }
        else {
            currentPtr = currentPtr->left;
        }
        
    }
    /*Couldn't find the element*/
    if (currentPtr == NULL) {
        cout<<"0";
    }
    
}

/*Print the count of theID iteratively. If not present, prints 0*/
void RBTree::Count(NODEPTR root , int theID ){
    NODEPTR currentPtr = root;
    
    while (currentPtr!=NULL) {
        if (currentPtr->key == theID) {
            cout<<currentPtr->count;
            return ;
        }
        if (currentPtr->key < theID) {
            currentPtr = currentPtr->right;
        }
        else currentPtr = currentPtr->left;
    }
    /*Couldn't find the element*/
    if (currentPtr == NULL) {
        cout<<"0\n";
    }
    
}


/*Print the total count for IDs between ID1 and ID2 inclusively
 this does it recursively 
 */

int RBTree::InRange(NODEPTR root , int low , int high ){
    
    // Base case
    if (!root) return 0;
    
    // Special Optional case for improving efficiency
    if (root->key == high && root->key == low)
        return root->count;
    
    // If current node is in range, then include it in count and
    // recur for left and right children of it
    if (root->key <= high && root->key >= low)
        return root->count + InRange(root->left, low, high) +
        InRange(root->right, low, high);
    
    // If current node is smaller than low, then recur for right
    // child
    else if (root->key < low)
        return InRange(root->right, low, high);
    
    // Else recur for left child
    else return InRange(root->left, low, high);
    
}
