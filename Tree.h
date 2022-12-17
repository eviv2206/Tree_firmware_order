//
// Created by zhenya on 17.12.2022.
//

#ifndef UNTITLED30_TREE_H
#define UNTITLED30_TREE_H

#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

typedef struct treeNode{
    int key;
    int number;
    struct treeNode *leftChild;
    struct treeNode *rightChild;
}treeNode;

typedef treeNode *treeNodePtr;

void node_insert(treeNodePtr *, int );
void delete_node(treeNodePtr *, int);

void my_order(treeNodePtr, int, int*);
void print_tree(treeNodePtr, int );

#endif //UNTITLED30_TREE_H
