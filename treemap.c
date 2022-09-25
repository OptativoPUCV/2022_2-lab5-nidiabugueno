#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap * new =(TreeMap*)malloc(sizeof(TreeMap));
  new -> root = NULL;
  new->lower_than = lower_than;
  
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  if (tree->root == NULL){
    //creamos un nuevo nodo
    tree->root = createTreeNode(key,value);
  }else{
    tree->current = tree->root;
    while(tree->current != NULL){ //empezamos a buscar
      if(is_equal(tree, tree->current->pair->key,key)){
        return; //solo porque es de tipo void
      }
      if(tree->lower_than(key,tree->current->pair->key)){
        if(tree->current->left == NULL){
          tree->current->left = createTreeNode(key,value);
          tree->current->left->parent = tree->current;
          tree->current = tree->current->left;
          return;
        }
        tree->current = tree->current->left;
      }
      else{
        if(tree->current->right == NULL){
          tree->current->right = createTreeNode(key,value);
          tree->current->right->parent = tree->current;
          tree->current = tree->current->right;
          return;
        }
        tree->current = tree->current->right;
      }
    }
  }

}

TreeNode * minimum(TreeNode * x){
  if (x == NULL)return NULL; //para alcanzar el mínimo, sólo recorremos hacia el punto más izquierdo
  while(x->left != NULL){
    x = x->left;
  }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if(tree->root == NULL){
    return;
  }
  if(node->left == NULL || node->right == NULL)
  {
    if(node->left!= NULL)
    {
      node->left->parent = node->parent;
      node->parent->left = node->left;
      node = NULL;
    }
    if(node->right!=NULL)
    {
      node->right->parent = node->parent;
      node->parent->right = node->right;
      node = NULL;
    }else
    {
      node->parent->left = node->left;
      node = NULL;
      
    }
  }else{
    TreeNode* x = node->rigth;
    x = minimun(x);
    node->parent->left = x;
    x->parent = node->parent;
    x->right = node->right;
  }

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  tree->current = tree->root;
  while(tree->current != NULL){
    if(is_equal(tree,tree->current->pair->key,key)){
      return tree->current->pair;
    }
    if(tree->lower_than(tree->current->pair->key,key)){
      tree->current = tree->current->right;
    }
    else{
      tree->current = tree->current->left;
    }
  }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
