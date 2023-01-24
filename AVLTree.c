
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"
#include "Record.h"
#include "AVLTree.h"

typedef struct node *Node;
struct node {
    Record rec;
    Node   left;
    Node   right;
    int    height;
};

struct tree {
    Node    root;
    int     (*compare)(Record, Record);
};

////////////////////////////////////////////////////////////////////////
// Auxiliary functions

static void doTreeFree(Node n, bool freeRecords);
static Node newNode(Record rec);
static Record doTreeSearch(Tree t, Node n, Record rec);
Node doTreeInsert(Tree t, Node n, Record rec, bool *res);
Node rotateLeft(Node n);
Node rotateRight(Node n);
int height(Node n);
void doTreeSearchBetween(Tree t, Node n, Record lower, Record upper, List l);
Record doTreeNext(Tree t, Node n, Record r, Record searched);

////////////////////////////////////////////////////////////////////////

static Node newNode(Record rec) {
    Node n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    n->rec = rec;
    n->left = NULL;
    n->right = NULL;
    n->height = 0;
    return n;
}

////////////////////////////////////////////////////////////////////////

Tree TreeNew(int (*compare)(Record, Record)) {
    Tree t = malloc(sizeof(*t));
    if (t == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    t->root = NULL;
    t->compare = compare;
    return t;
}

////////////////////////////////////////////////////////////////////////

void TreeFree(Tree t, bool freeRecords) {
    doTreeFree(t->root, freeRecords);
    free(t);
}

static void doTreeFree(Node n, bool freeRecords) {
    if (n != NULL) {
        doTreeFree(n->left, freeRecords);
        doTreeFree(n->right, freeRecords);
        if (freeRecords) {
            RecordFree(n->rec);
        }
        free(n);
    }
}

////////////////////////////////////////////////////////////////////////

Record TreeSearch(Tree t, Record rec) {
    return doTreeSearch(t, t->root, rec);
}

static Record doTreeSearch(Tree t, Node n, Record rec) {
    if (n == NULL) {
        return NULL;
    }

    int cmp = t->compare(rec, n->rec);
    if (cmp < 0) {
        return doTreeSearch(t, n->left, rec);
    } else if (cmp > 0) {
        return doTreeSearch(t, n->right, rec);
    } else {
        return n->rec;
    }
}


////////////////////////////////////////////////////////////////////////
/* IMPORTANT: 
   Do NOT modify the code above this line. 
   You must not modify the 'node' and 'tree' structures defined above.
   You must not modify the functions defined above.
*/
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////

bool TreeInsert(Tree t, Record rec) {
    // TODO: Complete this function
    bool res = false; // if the record was inserted
    t->root = doTreeInsert(t, t->root, rec, &res);
    return res;
}

Node doTreeInsert(Tree t, Node n, Record rec, bool *res) {
    // termination/zero case
    if (n == NULL) {
        *res = true;
        return newNode(rec);
    }

    // compare new rec with the rec we are currently looking at
    int cmp = t->compare(rec, n->rec);
    // go to the left subtree
    if (cmp < 0) {
        n->left = doTreeInsert(t, n->left, rec, res);
    } // go to the right subtree
    else if (cmp > 0) {
        n->right = doTreeInsert(t, n->right, rec, res);
    } // records are the same
    else { 
        *res = false;
    }
    // update height of tree
    n->height = height(n);
    int lheight = 0;
    int rheight = 0;
    // measure height of left and right subtree
    if (n->left != NULL) {
        // lheight = n->left->height;
        lheight = height(n->left);
    }
    if (n->right != NULL) {
        // rheight = n->right->height;
        rheight = height(n->right);
    }
    // Left Left rotation
    if (n != NULL && (lheight - rheight) > 1 &&
        t->compare(rec, n->left->rec) < 0) {
        return rotateRight(n);
    } // Right Right rotation
    if (n != NULL && (lheight - rheight) < -1 &&
        t->compare(rec, n->right->rec) > 0) {
        return rotateLeft(n);
    } // Left Right rotation
    if (n != NULL && (lheight - rheight) > 1 &&
        t->compare(rec, n->left->rec) > 0) {
        n->left = rotateLeft(n->left);
        return rotateRight(n);
    } // Right Left rotation
    if (n != NULL && (lheight - rheight) < -1 &&
        t->compare(rec, n->right->rec) < 0) {
        n->right = rotateRight(n->right);
        return rotateLeft(n);
    }
    return n;
}

int height(Node n) {
    if (n == NULL) {
        return 0;
    }
    else {
        // calculate the height of the left and right subtree
        int lheight = height(n->left);
        int rheight = height(n->right);
        // update height of current node and return height
        if (lheight > rheight) {
            // n->height = lheight + 1;
            return lheight + 1;
        }
        else {
            // n->height = rheight + 1;
            return rheight + 1;
        }
    }
}

Node rotateLeft(Node n) {
    Node y = n->right;
    Node z = y->left;

    y->left = n;
    n->right = z;
    n->height = height(n);
    y->height = height(y);
    return y;
}

Node rotateRight(Node n) {
    Node y = n->left;
    Node z = y->right;

    y->right = n;
    n->left = z;
    n->height = height(n);
    y->height = height(y);
    return y;
}


////////////////////////////////////////////////////////////////////////

List TreeSearchBetween(Tree t, Record lower, Record upper) {
    // TODO: Complete this function
    List l = ListNew();
    doTreeSearchBetween(t, t->root, lower, upper, l);
    return l;
}

// n - the current node
// l - a list to accumulate results
void doTreeSearchBetween(Tree t, Node n, Record lower,
                                Record upper, List l) {
    if (n == NULL) return;
    if (t->compare(lower, n->rec) < 0) {
        doTreeSearchBetween(t, n->left, lower, upper, l);
    }
    if(t->compare(lower, n->rec) <= 0 && t->compare(upper, n->rec) >= 0) {
        ListAppend(l, n->rec);
    }
    if (t->compare(upper, n->rec) > 0) {
        doTreeSearchBetween(t, n->right, lower, upper, l);
    }
}
 
////////////////////////////////////////////////////////////////////////

Record TreeNext(Tree t, Record r) {
    // TODO: Complete this function
    // tree is empty
    if (t->root == NULL) return NULL;
    Record searched = doTreeNext(t, t->root, r, t->root->rec);
    
    if (t->compare(searched, r) < 0) {
        searched = NULL;
    }
    return searched;
}

// loop trough tree from largest to lowest value 
Record doTreeNext(Tree t, Node n, Record r, Record searched) {
    // zero condition; return previous searched node
    if (n == NULL) return searched;
    
    // update searched node 
    if  ((n != NULL &&
        // current rec must be greater
        t->compare(r, n->rec) < 0 &&
        // and current rec must be smaller than previously saved rec
        t->compare(n->rec, searched) < 0) ||
        // or saved rec is smaller than the one we look for
        // (only in case of default/ root node record)
        t->compare(searched, r) < 0) {
        
        searched = n->rec;
    }
    searched = doTreeNext(t, n->right, r, searched);
    searched = doTreeNext(t, n->left, r, searched);
    return searched;
}
