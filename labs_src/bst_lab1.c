#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    if (root == NULL) {
        return;
    }
    print(root->right, level + COUNT);
    for (int i = 0; i < level; i++) {
        printf(" ");
    }
    printf("%d", root->data);
    printf("\n");
    print(root->left, level + COUNT);
}

void clean(bst_node **root) {
    if (*root == NULL) {
        return;
    }
    clean(&(*root)->left);
    clean(&(*root)->right);
    free(*root);
    *root = NULL;
}

void cli_cute(bst_node **root, char *tokens[], int token_cnt) {
    char *command = tokens[0];
    if (strcmp(command, "insert") == 0) {
        if (token_cnt == 2) {
            insert(root, atoi(tokens[1]));
        } else {
            printf("Ошибка: команда \"insert\" требует 1 аргумент\n");
            printf("Пример: insert 3\n");
        }
    } else if (strcmp(command, "delete") == 0) {
        if (token_cnt == 2) {
            delete(root, atoi(tokens[1]));
        } else {
            printf("Ошибка: команда delete требует 1 аргумент\n");
            printf("Пример: delete 3\n");
        }
    } else if (strcmp(command, "isSymmetric") == 0) {
        if (token_cnt == 1) {
            int res = isSymmetric(*root);
            printf("%d\n", res);
        } else {
            printf("Ошибка: команда isSymmetric не требует аргументов\n");
            printf("Пример: isSymmetric\n");
        }
    } else if (strcmp(command, "print") == 0) {
        if (token_cnt == 1) {
            print(*root, 0);
        }
    } else if (strcmp(command, "clean") == 0) {
        if (token_cnt == 1) {
            clean(root);
        } else {
            printf("Ошибка: команда clean не требует аргументов\n");
            printf("Пример: clean\n");
        }
    } else {
        printf("Неизвестная команда\n");
    }
}


int main() {
    bst_node *root = NULL;
    char user_cmd[256];
    while (fgets(user_cmd, sizeof(user_cmd), stdin) != NULL) {
        user_cmd[strcspn(user_cmd, "\n")] = 0;
        if (strlen(user_cmd) == 0) {
            continue;
        }
        char *saveptr = NULL;
        char *tokens[5];
        int token_cnt = 0;
        char *token = strtok_r(user_cmd, " ", &saveptr);
        while (token != NULL && token_cnt < 5) {
            tokens[token_cnt++] = token;
            token = strtok_r(NULL, " ", &saveptr);
        }
        if (token_cnt == 0) {
            continue;
        }
        cli_cute(&root, tokens, token_cnt);
    }
    printf("Выход из программы...\n");
    return 0;
}
