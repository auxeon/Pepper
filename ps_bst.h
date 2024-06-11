#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#ifndef PS_BST_H
#define PS_BST_H

#define COUNT(x) sizeof(x)/sizeof(x[0])

typedef struct node{
    int data;
    struct node* parent;
    struct node* left;
    struct node* right;
}node;

static node* bst_create_node(int d){
    node* n = (node*)malloc(sizeof(node));
    n->data = d;
    n->parent = n;
    n->left = NULL;
    n->right = NULL;
    return n;
}

static node** bst_insert(node** n, node** pn, int d){
    if(!*n){
        *n = bst_create_node(d);
        (*n)->parent = pn?*pn:NULL;
        return n;
    }
    if((*n)->data > d){
        bst_insert(&((*n)->left),n,d);
    }
    else if((*n)->data < d){
        bst_insert(&((*n)->right),n,d);
    }
}

static int bst_exists(node** rn, int d){
    int res = false;
    if(!*rn){
        res = false;
    }
    else if((*rn)->data == d){
        res = true;
    }
    else if((*rn)->data > d){
        res = bst_exists(&((*rn)->left),d);
    }
    else if((*rn)->data < d){
        res = bst_exists(&((*rn)->right),d);
    }
    return res;
}

static node** bst_find(node** n, int d){
    // base case
    if(!*n){
        return NULL;
    }
    if((*n)->data == d){
        return n;
    }
    else if((*n)->data > d){
        return bst_find(&((*n)->left),d);
    }
    else{
        return bst_find(&((*n)->right),d);
    }

}

static node* bst_from_postorder(int* data, int l, int r){
    if(l>r){
        return NULL;
    }
    node* n = bst_create_node(data[r]);

    int i=r-1;
    while(data[i] > n->data){
        i -=1;
    }
    n->right = bst_from_postorder(data,i+1,r-1);
    n->left = bst_from_postorder(data, l, i);
    return n;
}

static void bst_log_node_details(node**n, node** k){
    printf("parent address : %p\n",(*n)?(*n):NULL);
    printf("parent left : %p\n",(*n)->left?(*n)->left:NULL);
    printf("parent right : %p\n",(*n)->right?(*n)->right:NULL);
    printf("kid address : %p\n",(*k)?(*k):NULL);
    printf("kid left : %p\n",(*k)?(*k)->left:NULL);
    printf("kid right : %p\n",(*k)?(*k)->right:NULL);
}

/*
 * 0 left
 * 1 right
 * -1 not a kid
 */
static int bst_is_k_left_or_right(node** n, node** k){
    //bst_log_node_details(n,k);
    if((*n)->left == *k){
        return 0;
    }
    else if((*n)->right == *k){
        return 1;
    }
    return -1;
}
/*
 *  number of kids
 */
static int bst_count_kids(node** n){
    int res = 0;
    res += (*n)->left?1:0;
    res += (*n)->right?1:0;
    return res;
}
/*
 *  finds inorder predecessor of key
 */
static node** bst_inorder_pred(node** n, int key){
    node** found = NULL;
    found = bst_find(n,key);
    if(!*found){
        return found;   
    }
    if((*found)->left){
        node** poda = &((*found)->left);
        while((*poda)->right){
            poda = &((*poda)->right);
        }
        found = poda;
        return found;
    }
}
/*    

            12
       6       16
   3     8    14
  1 5        13


    leaf    : just delete and set parent left/right to NULL
    one kid : par(node)->(right/left) = node->(kid(left/right)))
    two kid : find inorder pred : pred->par->(left/right) <swap> node

*/
static void bst_copy_node(node** d, node** s){
    (*d)->data = (*s)->data;
}

/*
 * 1 success
 * 0 fail
 */


static int bst_delete(node** n, int key){
    if(!(*n)){
      return false;  
    }
    node** found = bst_find(n,key);
    if(!found){
        return false;
    }
    int nkids = bst_count_kids(found);
    node** par = &((*found)->parent);
    node** pred = NULL;
    node* t = *found;
    switch(nkids){
        case 0:
            switch(bst_is_k_left_or_right(par,found)){
                case 0:
                    (*par)->left = NULL;
                    free(*found);
                    break;
                case 1:
                    (*par)->right = NULL;
                    free(*found);
                    break;
                default:
                    break;
            }
            break;
        case 1:
            switch(bst_is_k_left_or_right(par,found)){
                case 0:
                    (*par)->left = (*found)->left?(*found)->left:(*found)->right;
                    free(t);
                    break;
                case 1:
                    (*par)->right = (*found)->left?(*found)->left:(*found)->right;
                    free(t);
                    break;
                default:
                    break;
            }
            break;
        case 2:
            // find inorder predecessor
            // copy data and then delete pred
            pred = bst_inorder_pred(found,key);
            if(!pred){
                t = *n;
                (*n) = (*n)->right;
                free(t);
            }
            bst_copy_node(found,pred);
            par = &((*pred)->parent);
            switch(bst_is_k_left_or_right(par,pred)){
                case 0:
                    (*par)->left = NULL;
                    free(*pred);
                    break;
                case 1:
                    (*par)->right = NULL;
                    free(*pred);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
        
    }
    return true;
}

/*
 *  print post order traversal of bst
 */
static void bst_print_postorder(node** n){
    if(!(*n)){
        return;
    }
    bst_print_postorder(&((*n)->left));
    bst_print_postorder(&((*n)->right));
    printf("%d ", (*n)->data);
}
        
static int bst_tests(){
   /*
             12
       6            16
   3     8        14
  1 5           13

 * */
    node* root = NULL;
    bst_insert(&root,NULL,12);
    bst_insert(&root,&root,6);
    bst_insert(&root,&root,16);
    bst_insert(&root,&root,3);
    bst_insert(&root,&root,8);
    bst_insert(&root,&root,14);
    bst_insert(&root,&root,1);
    bst_insert(&root,&root,5);
    bst_insert(&root,&root,13);
   
    printf("tree 1 : manual insertion\n"); 
    bst_print_postorder(&root);
    

    // creating new tree from old post order
    int post_data[] = {1,5,3,8,6,13,14,16,12};
    int index = COUNT(post_data)-1;
    
    node* postbst = bst_from_postorder(
        post_data,
        0,
        COUNT(post_data)-1
    );
    
    printf("\ntree 2 : bst from postorder\n");
    bst_print_postorder(&postbst);
    printf("\n");

    

    //bst_delete(&root,13);
    printf("delete 8\n");
    bst_delete(&root,8);
    bst_print_postorder(&root);
    printf("\n");
    printf("delete 14\n");
    bst_delete(&root,14);
    bst_print_postorder(&root);
    printf("\n");
    printf("delete 6\n");
    bst_delete(&root,6);
    bst_print_postorder(&root);
    printf("\n");
    printf("find\n");
    printf("23 : %d\n",bst_exists(&root,23));
    printf("7 : %d\n",bst_exists(&root,7));
    printf("\n");
    // finding the predecessor
    int key = 12;
    printf("pred(%d) : %d\n",key,(*bst_inorder_pred(&root,key))->data);
    return 0;
}




#endif
