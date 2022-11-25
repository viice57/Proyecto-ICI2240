#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../header/treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode
{
	TreePair * pair;
  	TreeNode * left;
  	TreeNode * right;
  	TreeNode * parent;
};

struct TreeMap
{
  	TreeNode * root;
  	TreeNode * current;
  	int (*lower_than) (void * key1, void * key2);
};

int is_equal_tree(TreeMap * tree, void * key1, void * key2)
{
	if(tree->lower_than(key1, key2) == 0 && tree->lower_than(key2, key1) == 0) return 1;
 	else return 0;
}

TreeNode * minimum(TreeNode * x)
{
  	if(!x) return NULL;
    
  	while(x->left) x = x->left;
  	return x;
}

TreeNode * createTreeNode(void * key, void * value)
{
  	TreeNode * new = (TreeNode *) malloc(sizeof(TreeNode));
  	if(!new) return NULL;
  
  	new->pair = (TreePair *) malloc(sizeof(TreePair));
  	new->pair->key = key;
  	new->pair->value = value;
  	new->parent = new->left = new->right = NULL;
  
  	return new;
}

TreeMap * createTreeMap(int (*lower_than) (void * key1, void * key2))
{
  	TreeMap * new = (TreeMap *) malloc(sizeof(TreeMap));
  	if(!new) return NULL;
  
  	new->root = new->current = NULL;
  	new->lower_than = lower_than;
  
 	return new;
}

void insertTreeMap(TreeMap * tree, void* key, void * value)
{
	if(!tree) return;
    
	TreeNode * aux = tree->root;
	TreeNode * parent = NULL;
    
	while (aux)
	{
		parent = aux;
        
		if(tree->lower_than(key, aux->pair->key)) aux = aux->left;
    	else if(tree->lower_than(aux->pair->key, key)) aux = aux->right;
   		else return;
  	}
    
  	TreeNode * new = createTreeNode(key, value);
  	new->parent = parent;
    
  	if(!parent) tree->root = new;
  	else if(tree->lower_than(key, parent->pair->key)) parent->left = new;
  	else parent->right = new;
  
  	tree->current = new;
}

void removeNode(TreeMap * tree, TreeNode* node)
{
  	//nodo hoja, 0 hijos
  	if(!node->right && !node->left)
	{
    	TreeNode * parent = node->parent;
    	if(!parent) tree->root = NULL;
    	else if(parent->left == node) parent->left = NULL;
		else if(parent->right == node) parent->right = NULL;
  	}
	//2 hijos
	else if(node->right && node->left)
	{
    	TreeNode * aux = node->right;
    	aux = minimum(aux);
    
    	node->pair->key = aux->pair->key;
    	node->pair->value = aux->pair->value;
    	removeNode(tree, aux);
    
    	return;
  	}
	// 1 hijo
	else
	{
    	TreeNode * parent = node->parent;
    	TreeNode * child = node->right;
    
    	if(!child) child = node->left;
    	child->parent = parent;
    	if(!parent) tree->root = child;
    	else if(parent->left == node) parent->left = child;
    	else if(parent->right == node) parent->right = child;
  	}
  	free(node->pair);
  	free(node); 
  
  	return;
}

void eraseTreeMap(TreeMap * tree, void* key)
{
  	if(!tree || !tree->root) return;
  	if(!searchTreeMap(tree, key)) return;
  
  	TreeNode * node = tree->current;
  	removeNode(tree, node);
}

TreePair * searchTreeMap(TreeMap * tree, void * key)
{
  	if(!tree || !tree->root) return NULL;
    
  	TreeNode * aux = tree->root;
    
  	while(aux)
	{
    	if(tree->lower_than(key, aux->pair->key)) aux = aux->left;
    	else if(tree->lower_than(aux->pair->key, key)) aux = aux->right;
    	else break;
  	}
  	tree->current = aux; 
  	if(!aux) return NULL;
	
  	return (void *) aux->pair;
}

TreePair * upperBound(TreeMap * tree, void * key)
{
  	if (!tree || !tree->root) return NULL;
    
  	TreeNode * aux = tree->root;
  	TreeNode * ub_node = NULL;
    
  	while(aux)
	{
    	if((!ub_node || tree->lower_than(aux->pair->key, ub_node->pair->key)) && !tree->lower_than(aux->pair->key, key))
      	ub_node = aux;

    	if(tree->lower_than(key, aux->pair->key)) aux = aux->left;
    	else if(tree->lower_than(aux->pair->key, key)) aux = aux->right;
    	else break;
  	}  
  	tree->current = ub_node;
  	if(!ub_node) return NULL;
	
  	return ub_node->pair;
}

TreePair * firstTreeMap(TreeMap * tree)
{
  	if (!tree || !tree->root) return NULL;
    
  	TreeNode * aux = tree->root;
  	tree->current = minimum(aux);
  	if(!tree->current) return NULL;
  
  	return tree->current->pair;
}

TreePair * nextTreeMap(TreeMap * tree)
{
  	if(!tree || !tree->root || !tree->current) return NULL;
    
  	if(tree->current->right)
  	{
   		TreeNode * aux = tree->current->right;
    	tree->current = minimum(aux);
    	printf(minimum(aux));
    
    	return tree->current->pair;
  	}
    
  	TreeNode * aux = tree->current->parent;
    
	while(aux && tree->current == aux->right)
	{
    	tree->current = aux;
    	aux = aux->parent;
  	} 
  	tree->current = aux;
  	if(!aux) return NULL;
    
  	return aux->pair;
}