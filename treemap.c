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
if (x == NULL)return NULL; //para alcanzar el m??nimo, s??lo recorremos hacia el punto m??s izquierdo
while(x->left != NULL){
  x = x->left;
}
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode *parent = node->parent;
  
    int cont = (node->left != NULL) + (node->right != NULL);
    if (cont == 0)
    { 
        if(parent->left == node) 
        {
            parent->left = NULL;
        } else if(parent->right == node){
            parent->right = NULL;
        }
    }if(cont == 1) {
        TreeNode * hijo;
        if(node->left){
          hijo = node->left;
        } 
        else{
          hijo = node->right;
        } 
        if(parent->left == node) {
            parent->left = hijo;
        }if(parent->right == node) {
            parent->right = hijo;
        }
        hijo->parent = parent;
        free(node);
    }if(cont == 2) {
        TreeNode *myor = minimum(node->right);
        node->pair = myor->pair;
        removeNode(tree, myor);
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
    Pair *buscarPair = searchTreeMap(tree, key);
    TreeNode *aux = tree->root;
    TreeNode *Ub = aux;
    int aux1, aux2;

  
    if (buscarPair != NULL){
      return buscarPair;
    }

    while (aux != NULL)
    {
        aux1 = tree->lower_than(aux->pair->key, key);
        if (aux1 == 1)
        {
            aux1 = tree->lower_than(Ub->pair->key, aux->pair->key);
            if (aux1 == 1){
              Ub = aux;
            } 
            aux = aux->right;
        }
        else 
        {
            aux1 = tree->lower_than(Ub->pair->key, aux->pair->key);
            if (aux1 == 1){
              Ub = aux;
            } 

            aux1 = tree->lower_than(key, aux->pair->key);
            aux2 = tree->lower_than(aux->pair->key, Ub->pair->key);
            if (aux1 == 1 && aux2 == 1) Ub = aux;
            aux = aux->left;
        }
    }

    aux1 = tree->lower_than(Ub->pair->key, key);
    if (aux1 == 1) {
      return NULL;
    }

    return Ub->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode *first = minimum(tree->root);
    return first->pair;
    
}

Pair * nextTreeMap(TreeMap * tree) {
  TreeNode *aux = tree->current;
  if(tree->current->right != NULL){
    tree->current = minimum(tree->current->right);
    return tree->current->pair;
  }
  else{
    while (aux->parent != NULL){
      aux = aux->parent;
      if(tree->lower_than(tree->current->pair->key,aux->pair->key)==1){
        tree->current = aux;
        return tree->current->pair;
      }
    }
  }
  return NULL;
}
