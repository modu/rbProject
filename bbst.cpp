#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "RedBlackTree.h"

using namespace std;

/*Function to build RB tree in O(lg n) */
NODEPTR cbst(vector<inputPairPTR> &num, int st, int ed, NODEPTR p ){
    if (st>ed){
        return NULL;
    }else{
        int mid = st+(ed-st)/2;
        NODEPTR bst = new Node(num[mid]->ID  , num[mid]->count);
        bst->left = cbst(num,st,mid-1, bst);
        bst->right = cbst(num,mid+1,ed , bst);
        bst->p = p ;
        return bst;
    }
}

NODEPTR sortedArrayToBST(vector<inputPairPTR> &num) {
    if (num.size()==0){return NULL;}

    /*Setting root's parent as NULL*/
    NODEPTR root = NULL;
    return cbst(num,0,num.size()-1, root );
}

int main(int argc, char * argv[])
{
    /*Check if a file is given as a command-line parameter
     if not provided show error and exit*/
    if (argc == 0) {
        cout<<"Error print program usuage\n";
        return -1;
    }
    
    /*Read from the file-name and build a Red-Black Tree*/
    ifstream file(argv[1]); /*fstream*/
    vector<inputPairPTR> tempVector;
    int idTemp , countTemp ,ignoreFirstLine = 1;
    string temp;
    
    if(file.is_open()){
        while(file.good()){
            if (ignoreFirstLine == 1) { /*Ignore the first number in file which is anyway lenght of input*/
                getline(file,temp);
                ignoreFirstLine = 0;
                continue;
            }
            getline(file,temp);
            stringstream ss(temp);
            ss >> idTemp >> countTemp;
            //cout<< idTemp << " " << countTemp << "\n\n";
            struct inputPair * newNode = (struct inputPair *) malloc(sizeof(inputPair));
            newNode->ID =  idTemp;
            newNode->count = countTemp;
            tempVector.push_back(newNode);
        }
    } else{
        cout << "Error: Problem opening input file or some other problem with file\n\n";
        exit(0);
    }
    /*Build the RedBlackTree*/
    NODEPTR root = sortedArrayToBST(tempVector);
    
    /*Freeing the memory of vector , input file may be 1GB so need to free it! */
    vector<inputPairPTR>::iterator i;
    for (i = tempVector.begin(); i != tempVector.end(); i++) {
        free(*i);
    }
    tempVector.clear();
    
    RBTree rbtree = RBTree(root);
    rbtree.makeLeafRed(root);
    /*Red Black Tree ready!*/

    
    /*Loop infiniately unless input is exit command */
    while (1) {
        //Input a command
        string command;
        getline(cin,command);
        stringstream ss(command);
        //cout<<command<<"\n";
        int theID , count = 0 ;
        string leaveIt;
        if (command.find("increase") != string::npos) {
            ss >> leaveIt >> theID >> count;
            //            cout<<"Increase Case\n";
            rbtree.Increase(theID,count,root);
            cout<<"\n";
        }
        else if (command.find("reduce") != string::npos) {
            ss >> leaveIt >> theID >> count;
            //            //cout<<"reduce Case\n";
            rbtree.Reduce(root,theID,count);
            cout<<"\n";
        }
        else if (command.find("count") != string::npos) {
            ss >> leaveIt >> theID;
            //            cout<<"count Case\n";
            rbtree.Count(root,theID);
            cout<<"\n";
        }
        else if (command.find("inrange") != string::npos) {
            int ID1 = 0 , ID2 = 0;
            ss >> leaveIt >> ID1 >> ID2;
            //Reduce(root,theID,count);
            cout << rbtree.InRange(root, ID1 , ID2);
            cout<<"\n";
        }
        else if (command.find("next") != string::npos) {
            ss >> leaveIt >> theID;
            //            cout<<"next Case\n";
            NODEPTR temp = rbtree.successor(root,theID);
            if (temp == NULL) {
                cout<<"0 0";
            }
            else {
                cout<<temp->key << " "<<temp->count;
            }
            cout<<"\n";
        }
        else if (command.find("previous") != string::npos) {
            ss >> leaveIt >> theID;
            //            cout<<"previous Case\n";
            NODEPTR temp = rbtree.predecessor(root,theID);
            if (temp == NULL) {
                cout<<"0 0";
            }
            else {
                cout<<temp->key << " "<<temp->count;
            }
            cout<<"\n";
        }
        else if (command.find("quit") != string::npos) {
            exit(0);
        }
        else if (command.find("inorder") != string::npos){
            cout<<"\n";
            rbtree.inorder(root);
            cout<<"\n";
        }
    }
    return 0;
}
