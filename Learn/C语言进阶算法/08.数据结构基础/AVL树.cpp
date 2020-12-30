/* By Vamei */
/* binary search tree */
#include <stdio.h>
#include <stdlib.h>

typedef struct node *position;
typedef int ElementTP;

struct node {
    int depth; 
    position parent;
    ElementTP element;
    position lchild;
    position rchild;
};

/* pointer => root node of the tree */
typedef struct node *TREE;

position insert_value(TREE, ElementTP);
int depth(TREE);

static void insert_node_to_nonempty_tree(TREE, position);
static void update_root_depth(TREE); 
static TREE recover_avl(TREE, position);
static int depth_diff(TREE); 
static position insert_leaf(TREE, ElementTP);
static void insert_node_to_nonempty_tree(TREE, position);
static TREE left_single_rotate(TREE);
static TREE left_double_rotate(TREE);
static TREE right_single_rotate(TREE);
static TREE right_double_rotate(TREE);

int main(void) 
{
    TREE tr;
    position np;
    ElementTP element;
    tr = NULL;
    tr = insert_value(tr, 18);
    tr = insert_value(tr, 5);
    printf("root: %d\n", tr->element);
    printf("depth: %d\n", depth(tr));
    tr = insert_value(tr, 2); 
    printf("root: %d\n", tr->element);
    printf("depth: %d\n", depth(tr));
    tr = insert_value(tr, 4);
    printf("root: %d\n", tr->element);
    printf("depth: %d\n", depth(tr));
    printf("root->lchild: %d\n", tr->lchild->element);
    tr = insert_value(tr, 3);
    printf("root: %d\n", tr->element);
    printf("depth: %d\n", depth(tr));
    printf("root->lchild: %d\n", tr->lchild->element);
    printf("root->lchild->lchild: %d\n", tr->lchild->lchild->element);
}


/*
 * insert value
 *
 */
position insert_value(TREE tr, ElementTP value) 
{
    position newtree;

    /* insert a value to a binary search tree */
    newtree = insert_leaf(tr, value);
    update_root_depth(newtree);
    if (tr == NULL) {
        tr = newtree;
    }
    else {
        tr = recover_avl(tr, newtree);
    }
    return tr;
}

/*
 * get the depth of the tree
 * use this function to access depth
 */
int depth(TREE tr) {
    if (tr == NULL) {
        return 0;
    }
    else {
        return tr->depth;
    }
}

//========================================
// static functions: for internal use

/* 
 * traverse the path from newtree node to root node
 * make one rotation, recover AVL and stop
 */
static TREE recover_avl(TREE tr, position np) 
{
    int myDiff;
    while (np != NULL) {
        update_root_depth(np);
        myDiff = depth_diff(np);
    if (myDiff > 1 || myDiff < -1) {
            if (myDiff > 1) {
                /* left rotate needed */
                if(depth_diff(np->rchild) > 0) {
                    np = left_single_rotate(np);
                }
                else {
                    np = left_double_rotate(np);
                }
            }
            if (myDiff < -1) {
                if(depth_diff(np->lchild) < 0) {
                    np = right_single_rotate(np);
                }
                else {
                    np = right_double_rotate(np);
                }
            }
            /* if rotation changes root node */
            if (np->parent == NULL) tr = np;
            break;
        }
        np = np->parent;
    }
    
    return tr;
}

/*
 * difference of rchild->depth and lchild->depth
 */
static int depth_diff(TREE tr) 
{
    if (tr == NULL) {
        return 0;
    }
    else {
        return depth(tr->rchild) - depth(tr->lchild);
    }
}


/* 
 * left single rotation 
 * return the newtree root
 */
static TREE left_single_rotate(TREE tr) 
{
    TREE newtreeRoot, parent;
    parent  = tr->parent;
    newtreeRoot = tr->rchild;
    /* detach & attach */ 
    if (newtreeRoot->lchild != NULL) newtreeRoot->lchild->parent = tr;
    tr->rchild = newtreeRoot->lchild;
    update_root_depth(tr);
   
    /* raise newtree root node */
    newtreeRoot->lchild = tr;
    newtreeRoot->parent = parent;
    if (parent != NULL) {
        if (parent->lchild == tr) {
        parent->lchild = newtreeRoot;
    }
    else {
        parent->rchild = newtreeRoot;
    }
    }
    tr->parent = newtreeRoot;
    update_root_depth(newtreeRoot);
    return newtreeRoot;
}

/* 
 * right single rotation 
 * return the newtree root
 */
static TREE right_single_rotate(TREE tr) 
{
    TREE newtreeRoot, parent;
    parent  = tr->parent;
    newtreeRoot = tr->lchild;

    /* detach & attach */
    if (newtreeRoot->rchild != NULL) newtreeRoot->rchild->parent = tr;
    tr->lchild = newtreeRoot->rchild;
    update_root_depth(tr);
  
    /* raise newtree root node */
    newtreeRoot->rchild = tr;
    newtreeRoot->parent = parent;
    if (parent != NULL) {
        if (parent->lchild == tr) {
        parent->lchild = newtreeRoot;
    }
    else {
        parent->rchild = newtreeRoot;
    }
    }
    tr->parent = newtreeRoot;
    update_root_depth(newtreeRoot);
    return newtreeRoot;
}

/*
 * left double rotation
 * return
 */
static TREE left_double_rotate(TREE tr) 
{
    right_single_rotate(tr->rchild);
    return left_single_rotate(tr);
}

/*
 * right double rotation
 * return
 */
static TREE right_double_rotate(TREE tr) 
{
    left_single_rotate(tr->lchild);
    return right_single_rotate(tr);
}

/*
 * update tr->depth
 * assume lchild->depth and rchild->depth are correct
 */
static void update_root_depth(TREE tr) 
{
    int maxChildDepth; 
    int depLChild, depRChild;
    if (tr==NULL) return;
    else {
        depLChild = depth(tr->lchild);
        depRChild = depth(tr->rchild);
        maxChildDepth = depLChild > depRChild ? depLChild : depRChild;
        tr->depth = maxChildDepth + 1;
    }
}

/* 
 * insert a newtree value into the tree as a leaf
 * return address of the newtree node
 */
static position insert_leaf(TREE tr, ElementTP value) 
{
    position np;
    /* prepare the node */
    np = (position) malloc(sizeof(struct node));
    np->element = value;
    np->parent  = NULL;
    np->lchild  = NULL;
    np->rchild  = NULL;
 
    if (tr != NULL) {
        insert_node_to_nonempty_tree(tr, np);
    }
    return np;
}

/*
 * insert a node to a non-empty tree
 * called by insert_value()
 */
static void insert_node_to_nonempty_tree(TREE tr, position np)
{
    /* insert the node */
    if(np->element <= tr->element) {
        if (tr->lchild == NULL) {
            /* then tr->lchild is the proper place */
            tr->lchild = np;
            np->parent = tr;
            return;
        }
        else {
            insert_node_to_nonempty_tree(tr->lchild, np);
        }
    }
    else if(np->element > tr->element) {
        if (tr->rchild == NULL) {
            tr->rchild = np;
            np->parent = tr;
            return;
        }
        else {
            insert_node_to_nonempty_tree(tr->rchild, np);
        }
    }
}