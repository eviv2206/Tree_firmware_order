#include "tree.h"

void printMenu(){
    printf("1 for add\n");
    printf("2 for delete\n");
    printf("3 for pre order\n");
    printf("4 for in order\n");
    printf("5 for post order\n");
    printf("6 for printing tree\n");
    printf("7 for firmware\n");
    printf("0 for exit\n");
}

int input_value(){
    int value = 0;
    printf("value: ");
    scanf(" %d", &value);
    fflush(stdin);
    return value;
}

void start(){
    treeNodePtr root = NULL;
    int amount = 0;
    printMenu();
    int choice = -1;
    while(choice != 0){
        printf("\noption:\n");
        scanf(" %d", &choice);
        switch (choice) {
            case 1:
                node_insert(&root, input_value());
                break;
            case 2:
                delete_node(&root, input_value());
                break;
            case 3:
                my_order(root, 1, 0);
                break;
            case 4:
                my_order(root, 2, 0);
                break;
            case 5:
                my_order(root, 3, 0);
                break;
            case 6:
                print_tree(root, 1);
                break;
            case 7:
                print_tree(root, 2);
                break;
        }
    }

    while (root != NULL){
        delete_node(&root, root->key);
    }

}

int main() {
    start();
    return 0;
}
