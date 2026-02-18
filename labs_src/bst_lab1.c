#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define COUNT 4

typedef struct bst_node {
    int data;
    struct bst_node *left;
    struct bst_node *right;
} bst_node;

void insert(bst_node **root, const int data) {
    if (*root == NULL) {
        bst_node *new_node = (bst_node *) malloc(sizeof(bst_node));
        if (new_node == NULL) {
            printf("Memory allocation failed\n");
            return;
        }
        new_node->data = data;
        new_node->left = NULL;
        new_node->right = NULL;
        *root = new_node;
        return;
    }
    if (data < (*root)->data) {
        insert(&(*root)->left, data);
    } else if (data > (*root)->data) {
        insert(&(*root)->right, data);
    }
}

void delete(bst_node **root, const int data) {
    if (*root == NULL) {
        printf("The tree is empty\n");
        return;
    }
    bst_node *temp = *root;
    if (data < (*root)->data) {
        delete(&(*root)->left, data);
    } else if (data > (*root)->data) {
        delete(&(*root)->right, data);
    } else {
        if ((*root)->left == NULL && (*root)->right == NULL) {
            *root = NULL;
            free(temp);
            return;
        }
        if ((*root)->left == NULL && (*root)->right != NULL) {
            *root = (*root)->right;
            free(temp);
            return;
        }
        if ((*root)->right == NULL && (*root)->left != NULL) {
            *root = (*root)->left;
            free(temp);
            return;
        }
        if ((*root)->left != NULL && (*root)->right != NULL) {
            const bst_node *curr = (*root)->right;
            while (curr->left != NULL) {
                curr = curr->left;
            }
            (*root)->data = curr->data;
            delete(&(*root)->right, curr->data);
            return;
        }
    }
}

int isMirror(const bst_node *left, const bst_node *right) {
    if (left == NULL && right == NULL) return 1;
    if (left == NULL || right == NULL) return 0;
    if (left->data != right->data) return 0;
    return isMirror(left->left, right->right) && isMirror(left->right, right->left);
}

int isSymmetric(const bst_node *root) {
    if (root == NULL) { return 1; }
    return isMirror(root->left, root->right);
}

void print(const bst_node *root, int level) {
    if (root == NULL) return;
    print(root->right, level + COUNT);
    for (int i = 0; i < level; i++) {
        printf(" ");
    }
    printf("%d", root->data);
    printf("\n");
    print(root->left, level + COUNT);
}

int main() {
    bst_node *root = NULL;
    srand(time(NULL));
    for (int i = 0; i < 50; i++) {
        int val = rand() % 11;
        insert(&root, val);
    }
    printf("Tree generated with 1024 elements.\n");
    print(root, 0);
    return 0;
}
