//
// Created by zhenya on 17.12.2022.
//
#include <stdlib.h>
#include "tree.h"

void update_level(treeNodePtr root, treeNodePtr node) {
    if (node != NULL) {
        if (node == root->leftChild) node->number = (root->number) * 2 + 1;
        else node->number = (root->number) * 2 + 2;
        update_level(node, node->leftChild);
        update_level(node, node->rightChild);
    }
}

void node_insert(treeNodePtr *root, int key) {
    if (*root == NULL) {
        treeNodePtr tmp = malloc(sizeof(treeNode));
        tmp->key = key;
        tmp->number = 0;
        tmp->leftChild = NULL;
        tmp->rightChild = NULL;
        *root = tmp;
    } else {
        int notToAdd = 0;
        treeNodePtr currPtr = *root, prevPtr;
        while (currPtr != NULL && notToAdd != 1) {
            prevPtr = currPtr;
            if (currPtr->key == key) notToAdd = 1;
            if (currPtr->key > key) currPtr = currPtr->leftChild;
            else currPtr = currPtr->rightChild;
        }
        if (notToAdd != 1) {
            currPtr = malloc(sizeof(treeNode));
            currPtr->key = key;
            currPtr->rightChild = NULL;
            currPtr->leftChild = NULL;
            if (prevPtr->key > key) prevPtr->leftChild = currPtr;
            else prevPtr->rightChild = currPtr;
        }
    }
    update_level(*root, (*root)->leftChild);
    update_level(*root, (*root)->rightChild);
}

treeNodePtr search(treeNodePtr *root, int key) {
    treeNodePtr tmp = *root;
    if ((tmp == NULL) || (tmp->key == key)) return tmp;
    if (key < tmp->key) return search(&(tmp->leftChild), key);
    else return search(&(tmp->rightChild), key);
}

treeNodePtr find_parent(treeNodePtr *root, int childKey) {
    treeNodePtr parent = *root;
    if (parent->key == childKey) return NULL;
    else {
        if (parent->key > childKey) {
            if (parent->leftChild->key == childKey) return parent;
            else return find_parent(&parent->leftChild, childKey);
        } else {
            if (parent->rightChild->key == childKey) return parent;
            else return find_parent(&parent->rightChild, childKey);
        }
    }
}

treeNodePtr min(treeNodePtr root) {
    treeNodePtr l = root;
    while (l->leftChild != NULL) l = l->leftChild;
    return l;
}

treeNodePtr max(treeNodePtr root) {
    treeNodePtr m = root;
    while (m->rightChild != NULL) m = m->rightChild;
    return m;
}

treeNodePtr next_node(treeNodePtr root) {
    treeNodePtr p = root, l = NULL;
    if (p->rightChild != NULL)
        return min(p->rightChild);
    l = find_parent(&p, l->key);
    while ((l != NULL) && (p == l->rightChild)) {
        p = l;
        l = find_parent(&root, l->key);
    }
    return l;
}

void delete_root(treeNodePtr *root) {
    treeNodePtr tmp = *root;

    if (tmp->leftChild == NULL && tmp->rightChild == NULL) {
        (*root)->key = 0;
        (*root)->number = 0;
        *root = NULL;
    }

    if (tmp->leftChild != NULL &&  tmp->rightChild == NULL) {
        *root = tmp->leftChild;
        free(tmp);
    }

    if (tmp->leftChild == NULL &&  tmp->rightChild != NULL){
        *root = tmp->rightChild;
        free(tmp);
    }

    if (tmp->leftChild != NULL &&  tmp->rightChild != NULL){
        treeNodePtr m  = next_node(tmp);
        if (tmp->rightChild == m){
            m->leftChild = tmp->leftChild;
        }else{
            find_parent(root, m->key)->leftChild = NULL;
            m->rightChild = tmp->rightChild;
            m->leftChild = tmp->leftChild;
        }
        *root = m;
        free(tmp);
    }

    if (*root != NULL){
        (*root)->number = 0;
    }
}

void delete_node(treeNodePtr *root, int key) {
    treeNodePtr l = NULL, m = NULL;
    l = search(root, key);
    if (l != NULL) {
        if (l != *root) {
            if ((l->leftChild == NULL) && (l->rightChild == NULL)) {
                m = find_parent(root, l->key);
                if (l == m->rightChild) m->rightChild = NULL;
                else m->leftChild = NULL;
                free(l);
            } else if ((l->leftChild == NULL) && (l->rightChild != NULL)) {
                m = find_parent(root, l->key);
                if (l == m->rightChild) m->rightChild = l->rightChild;
                else m->leftChild = l->rightChild;
                free(l);
            } else if ((l->leftChild != NULL) && (l->rightChild == NULL)) {
                m = find_parent(root, l->key);
                if (l == m->rightChild) m->rightChild = l->leftChild;
                else m->leftChild = l->leftChild;
                free(l);
            }
            if ((l->leftChild != NULL) && (l->rightChild != NULL)) {
                m = next_node(l);
                if (l->rightChild == m){
                    treeNodePtr parent = find_parent(root, l->key);
                    if (parent->leftChild == l){
                        parent->leftChild = m;
                        m->leftChild = l->leftChild;
                        free(l);
                    }else{
                        parent->rightChild = m;
                        m->leftChild = l->leftChild;
                    }
                }else{
                    find_parent(root, m->key)->leftChild = NULL;
                    l->key = m->key;
                    free(m);
                }
            }
        } else delete_root(root);
        if (*root != NULL) {
            update_level(*root, (*root)->leftChild);
            update_level(*root, (*root)->rightChild);
        }
    }
}

void my_order(treeNodePtr node, int mode, int *n) {
    //  1 for straight order
    //  2 for in order
    //  3 for post order
    //  4 for getting numbers for heap
    //  5 for getting amount of nodes
    if (node != NULL) {
        if (mode == 5) {
            (*n)++;
        }
        if (mode == 4) {
            if (node->number > *n) *n = node->number;
        }
        if (mode == 1) {
            printf("%3d -> ", node->key);
        }
        my_order(node->leftChild, mode, n);
        if (mode == 2) {
            printf("%3d -> ", node->key);
        }
        my_order(node->rightChild, mode, n);
        if (mode == 3) {
            printf("%2d ->", node->key);
        }
    }
}

static int my_pos = 0;

void fill_array(treeNodePtr node, int *arr, int mode) {
    if (node != NULL) {
        if (mode == 1) arr[node->number] = node->key;
        fill_array(node->leftChild, arr, mode);
        if (mode == 2) {
            arr[my_pos] = node->key;
            my_pos++;
        }
        fill_array(node->rightChild, arr, mode);
    }
}

int getNum(treeNodePtr root) {
    int n = 0;
    my_order(root, 4, &n);
    return n;
}

void get_degree(int *pow, int *num, int n) {
    while (1) {
        if (n >= *num - 1) {
            (*num) *= 2;
            (*pow)++;
        } else break;
    }
}

void print_char(int amount, char to_print) {
    for (int i = 0; i < amount; ++i) {
        printf("%c", to_print);
    }
}

void no_firmware(int *arr, int num, int pow, int bit_depth) {
    int leftSpaces = (num - 1) * bit_depth;
    int spaceBetween;
    for (int i = 0, lim = 1, pos = 0; i < pow; ++i, lim *= 2) {
        spaceBetween = leftSpaces;
        leftSpaces = (leftSpaces - bit_depth) / 2;
        print_char(leftSpaces, ' ');
        for (int j = 0; j < lim; ++j) {
            char str[bit_depth + 1];
            if (arr[pos] == 0) {
                print_char(bit_depth, '-');
            } else {
                for (int k = 0; k < bit_depth; ++k) {
                    str[k] = ' ';
                }
                str[bit_depth] = '\0';
                int temp = bit_depth - 1, num_to_convert = arr[pos];
                while (num_to_convert > 0) {
                    str[temp--] = (char) (num_to_convert % 10 + 48);
                    num_to_convert /= 10;
                }
                printf("%s", str);
            }
            ++pos;
            print_char(spaceBetween, ' ');
        }
        printf("\n");
    }
}

void with_firmware(int *arr, int *arr2, int num, int pow, int bit_depth, int str_len, int n, int amount) {
    int leftSpaces = (num - 1) * str_len;
    int spaceBetween;
    for (int i = 0, lim = 1, pos = 0; i < pow; ++i, lim *= 2) {
        spaceBetween = leftSpaces;
        leftSpaces = (leftSpaces - str_len) / 2;
        print_char(leftSpaces, ' ');
        for (int j = 0; j < lim; ++j) {
            char str[str_len + 1];
            if (arr[pos] == 0) {
                print_char(str_len, '-');
            } else {
                for (int k = 0; k < str_len; ++k) {
                    str[k] = ' ';
                }
                str[str_len] = '\0';
                int left = 2 * pos + 1;
                int right = left + 1;
                int num_to_convert;
                int k;
                if (left > n || arr[left] == 0) {
                    left = 0;
                    while (arr2[left] != arr[pos]) ++left;
                    if (left - 1 < 0) {
                        num_to_convert = arr[0];
                    } else {
                        num_to_convert = arr2[left - 1];
                    }
                    str[0] = '(';
                    str[1 + bit_depth] = ')';
                    k = bit_depth;
                    while (num_to_convert > 0) {
                        str[k] = (char) (num_to_convert % 10 + 48);
                        --k;
                        num_to_convert /= 10;
                    }
                }
                num_to_convert = arr[pos];
                k = 2 + 2 * bit_depth;
                while (num_to_convert > 0) {
                    str[k--] = (char) (num_to_convert % 10 + 48);
                    num_to_convert /= 10;
                }
                if (right > n) {
                    right = 0;
                    while (arr2[right] != arr[pos]) ++right;
                    if (right + 2 > amount) {
                        num_to_convert = arr[0];
                    } else {
                        num_to_convert = arr2[right + 1];
                    }
                    str[str_len - 1] = ')';
                    str[str_len - bit_depth - 2] = '(';
                    k = str_len - 2;
                    while (num_to_convert > 0) {
                        str[k] = (char) (num_to_convert % 10 + 48);
                        --k;
                        num_to_convert /= 10;
                    }
                }
                printf("%s", str);
            }
            ++pos;
            print_char(spaceBetween, ' ');
        }
        printf("\n");
    }
}

int define_bits(int max) {
    int bits = 0;
    while (max > 0) {
        ++bits;
        max /= 10;
    }
    return bits;
}

void print_tree(treeNodePtr root, int mode) {
    //1 for printing without firmware
    //2 for with firmware
    if (root != NULL) {
        int n = getNum(root);
        int pow = 0;
        int num = 1;

        get_degree(&pow, &num, n);

        int arr[num];
        for (int i = 0; i < num; ++i) {
            arr[i] = 0;
        }

        my_pos = 0;

        fill_array(root, arr, 1);

        int amount = 0;
        my_order(root, 5, &amount);
        int arr2[amount];
        my_pos = 0;
        int bit_depth = define_bits(max(root)->key);
        switch (mode) {
            case 1:
                no_firmware(arr, num, pow, bit_depth);
                break;
            case 2:
                fill_array(root, arr2, 2);
                with_firmware(arr, arr2, num, pow, bit_depth, bit_depth * 3 + 6, n, amount);
                break;
        }
    } else{
        printf("NULL\n");
    }
}

