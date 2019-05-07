/*
*   Operating System Lab
*       Lab2 (Synchronization)
*       Student id : 32141444     32144777        32130182
*       Student name : Kim HeeJu  Choi YoungJae   Yoon ChanYoung
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"



int lab2_node_print_inorder(lab2_tree *tree) {
//driver function
    inorder(tree->root);

    return 0;
}



void inorder(lab2_node *ptr){  
//For one node in BST is inorder the subtree which has ptr as root.
    if(ptr == NULL) return;

    inorder(ptr->left);
    inorder(ptr->right);
}



lab2_tree *lab2_tree_search(lab2_tree *tree, int key) {
//search function
	if(tree->root == NULL) 	return NULL;
	if(tree->root->key>key) 
		return lab2_tree_search(tree->root->left, key);
	else 
		return lab2_tree_search(tree->root->right, key);
}



lab2_tree *lab2_tree_create() {
//tree create funtion
    lab2_tree *tree = (lab2_tree *)malloc(sizeof(lab2_tree)); //allocate trees
    
    pthread_mutex_init(&tree->mutex, NULL); //thread initiation
    
    tree->root = NULL; //tree's root is NULL
    node_count = 0; // the count of node(the number of nodes) is 0 at create.

    return tree;
}




lab2_node * lab2_node_create(int key) {
//node create funtion
    lab2_node *node = (lab2_node *)malloc(sizeof(lab2_node)); //allocate nodes
    pthread_mutex_init(&node->mutex, NULL); //thread initiation
    
    ///////////////////////initializing///////////////////////

    node->left = NULL;
    node->right = NULL;
    node->key = key; 

    ///////////////////////initializing///////////////////////

    return node; // return node
}




int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){
//insert funtion 
    lab2_node *parent = NULL,
              *node,
              *t_root = tree->root;

    int key = new_node->key;
    node = new_node;

    while(t_root != NULL){
        if(key == t_root->key){
            node_count++;            
	    return 0;}
        parent = t_root;
	
	if(key < t_root->key) t_root = t_root->left;
	else t_root = t_root->right;
    }

    if(parent == NULL){
        tree->root = node;
        node_count++;
        return 0;}

    if(key < parent->key) parent->left = node;
    else parent->right = node;

    node_count++;

    return 0;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert function.

 lab2_node *parent = NULL,
              *node,
              *t_root = tree->root;

    int key = new_node->key;
    node = new_node;

    while(t_root != NULL){
        if(key == t_root->key){
	pthread_mutex_lock(&tree->mutex);
            node_count++;            
	pthread_mutex_unlock(&tree->mutex);
	    return 0;}
        parent = t_root;
	pthread_mutex_lock(&tree->mutex);
	if(key < t_root->key) t_root = t_root->left;
	else t_root = t_root->right;
	pthread_mutex_unlock(&tree->mutex);
    }

    if(parent == NULL){
        pthread_mutex_lock(&tree->mutex);
	tree->root = node;
        node_count++;
	pthread_mutex_unlock(&tree->mutex);
        return 0;}

	pthread_mutex_lock(&tree->mutex);
    if(key < parent->key) parent->left = node;
    else parent->right = node;

    node_count++;
	pthread_mutex_unlock(&tree->mutex);
    return 0;


}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert_cg function.
	
	  lab2_node *parent = NULL,
              *node,
              *t_root = tree->root;

	pthread_mutex_lock(&tree->mutex);

    int key = new_node->key;
    node = new_node;

    while(t_root != NULL){
        if(key == t_root->key){
            node_count++;        
		pthread_mutex_unlock(&tree->mutex);    
	    return 0;}
        parent = t_root;
	
	if(key < t_root->key) t_root = t_root->left;
	else t_root = t_root->right;
    }

    if(parent == NULL){
        tree->root = node;
        node_count++;
		pthread_mutex_unlock(&tree->mutex);    
        return 0;}

    if(key < parent->key) parent->left = node;
    else parent->right = node;

    node_count++;
		pthread_mutex_unlock(&tree->mutex);    
    return 0;
   }



int lab2_node_remove(lab2_tree *tree, int key) {
//erase node function
    lab2_node *parent = NULL, *child, *success, *success_p, 
	      *t_root = tree->root;
    
    node_count--;
    
        while(t_root != NULL && t_root->key != key){
        	parent = t_root;
		{ if(key < t_root->key) t_root = t_root->left;
	  	  else t_root = t_root->right;}     	
	}
    	
	if(t_root == NULL) {
        return 0;}

    	if((t_root->left == NULL) && (t_root->right == NULL)){
        	if(parent != NULL){
            		if(parent->left == t_root) parent->left = NULL;
	    		else parent->right = NULL;} 
		else tree->root = NULL;} 

    	else if((t_root->left == NULL)||(t_root->right == NULL)){
        	{ if(t_root->left != NULL) child = t_root->left;
	  	   else child =  t_root -> right;}
	        if( parent != NULL ){
            		if(parent->left == t_root) parent->left = child;
            		else parent->right = child;} 
        	else tree->root = child;} 

    	else {  success_p = t_root;
        	success = t_root->right;
       		while(success->left != NULL){
            		success_p = success;
            		success = success->left;}

        	if(success_p->left == success) success_p->left = success->right;
        	else success_p->right = success->right;
        	     t_root->key = success->key;
        	     t_root = success;}

    return 0;
}




/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove function.
    lab2_node *parent = NULL, *child, *success, *success_p, 
	      *t_root = tree->root;
    pthread_mutex_lock(&tree->mutex);
    node_count--;
    pthread_mutex_unlock(&tree->mutex);

        while(t_root != NULL && t_root->key != key){
		pthread_mutex_lock(&tree->mutex);        	
		parent = t_root;
		pthread_mutex_unlock(&tree->mutex);
		pthread_mutex_lock(&tree->mutex);
		{ if(key < t_root->key) t_root = t_root->left;
	  	  else t_root = t_root->right;}     	
		pthread_mutex_unlock(&tree->mutex);
	}

	if(t_root == NULL) {
        return 0;}

    	if((t_root->left == NULL) && (t_root->right == NULL)){
		pthread_mutex_lock(&tree->mutex);
        	if(parent != NULL){
            		if(parent->left == t_root) {
			parent->left = NULL;
			}
	    		else parent->right = NULL;} 
		else tree->root = NULL;
		pthread_mutex_unlock(&tree->mutex);} 

    	else if((t_root->left == NULL)||(t_root->right == NULL)){
		pthread_mutex_lock(&tree->mutex);
        	{ if(t_root->left != NULL) child = t_root->left;
	  	   else child =  t_root -> right;}
	        if( parent != NULL ){
            		if(parent->left == t_root) parent->left = child;
            		else parent->right = child;} 
        	else tree->root = child;
		pthread_mutex_unlock(&tree->mutex);} 

    	else {  pthread_mutex_lock(&tree->mutex);
		success_p = t_root;
        	success = t_root->right;
       		while(success->left != NULL){
            		success_p = success;
            		success = success->left;}

        	if(success_p->left == success) success_p->left = success->right;
        	else success_p->right = success->right;
        	     t_root->key = success->key;
        	     t_root = success;
		pthread_mutex_unlock(&tree->mutex);}

    return 0;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_cg function.
    lab2_node *parent = NULL, *child, *success, *success_p, 
	      *t_root = tree->root;
    pthread_mutex_lock(&tree->mutex);
    node_count--;
    
        while(t_root != NULL && t_root->key != key){
        	parent = t_root;
		{ if(key < t_root->key) t_root = t_root->left;
	  	  else t_root = t_root->right;}     	
	}
    pthread_mutex_unlock(&tree->mutex);

	if(t_root == NULL) {
        return 0;}
	
    pthread_mutex_lock(&tree->mutex);	
    	if((t_root->left == NULL) && (t_root->right == NULL)){
        	if(parent != NULL){
            		if(parent->left == t_root) parent->left = NULL;
	    		else parent->right = NULL;} 
		else tree->root = NULL;} 

    	else if((t_root->left == NULL)||(t_root->right == NULL)){
        	{ if(t_root->left != NULL) child = t_root->left;
	  	   else child =  t_root -> right;}
	        if( parent != NULL ){
            		if(parent->left == t_root) parent->left = child;
            		else parent->right = child;} 
        	else tree->root = child;} 

    	else {  success_p = t_root;
        	success = t_root->right;
       		while(success->left != NULL){
            		success_p = success;
            		success = success->left;}

        	if(success_p->left == success) success_p->left = success->right;
        	else success_p->right = success->right;
        	     t_root->key = success->key;
        	     t_root = success;}
    pthread_mutex_unlock(&tree->mutex);
    return 0;
 
}



void lab2_tree_delete(lab2_tree *tree) {
//erase tree function
    int value;
    lab2_node *t_root = tree->root; 
    if(t_root == NULL) return;
    while(t_root != NULL){
        value = t_root->key;
        lab2_node_remove(tree, value);
        t_root = tree->root;
    }
}


