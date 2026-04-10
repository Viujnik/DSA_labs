#include <stdio.h>
#include <stdlib.h>
#include <_ctype.h>
#include <string.h>
#define COUNT 6

typedef struct TreeNode {
    char *value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;



TreeNode *create_tree_node(const char *val) {
    TreeNode *node = malloc(sizeof(TreeNode));
    if (!node) return NULL;
    node->value = strdup(val);
    if (node->value == NULL) {
        return NULL;
    }
    node->left = node->right = NULL;
    return node;
}

void free_tree_node(TreeNode *node) {
    if (node == NULL) return;
    free_tree_node(node->left);
    free_tree_node(node->right);
    free(node->value);
    free(node);
}

void print_tree(const TreeNode *root, int level) {
    if (root == NULL) {
        return;
    }

    print_tree(root->right, level + COUNT);

    for (int i = 0; i < level; i++) {
        printf(" ");
    }
    printf("[%s]\n", root->value);

    print_tree(root->left, level + COUNT);
}


typedef struct Stack {
    char *data;
    struct Stack *next;
} Stack;

int is_empty(Stack **top) {
    return *top == NULL;
}

int push(Stack **top, char *value) {
    Stack *new_node = malloc(sizeof(Stack));
    if (new_node == NULL) {
        return -1;
    }
    new_node->data = value;
    new_node->next = *top;
    *top = new_node;
    return 0;
}

char *pop(Stack **top) {
    if (is_empty(top)) {
        return NULL;
    }
    Stack *temp = *top;
    *top = (*top)->next;
    char *result = temp->data;
    free(temp);
    return result;
}

char *peek(Stack **top) {
    if (is_empty(top)) {
        return NULL;
    }
    return (*top)->data;
}


typedef struct NodeStack {
    TreeNode *node;
    struct NodeStack *next;
} NodeStack;

int is_empty_stack(NodeStack **top) {
    return *top == NULL;
}

int push_node(NodeStack **top, TreeNode *tree_node) {
    NodeStack *new_stack_node = malloc(sizeof(NodeStack));
    if (new_stack_node == NULL) return -1;
    new_stack_node->node = tree_node;
    new_stack_node->next = *top;
    *top = new_stack_node;
    return 0;
}

TreeNode *pop_node(NodeStack **top) {
    if (is_empty_stack(top)) return NULL;
    NodeStack *temp = *top;
    *top = (*top)->next;
    TreeNode *res = temp->node;
    free(temp);
    return res;
}


char *get_string(const char *src, size_t *start, size_t size) {
    while (*start < size && isspace((unsigned char) src[*start])) {
        (*start)++;
    }
    if (*start >= size) return NULL;
    int capacity = 8;
    char *result = (char *) malloc(capacity * sizeof(char));
    if (result == NULL) return NULL;
    size_t res_size = 0;
    if (isdigit(src[*start])) {
        while (*start < size && isdigit(src[*start])) {
            if (res_size + 2 > capacity) {
                capacity *= 2;
                char *buf = (char *) realloc(result, capacity * sizeof(char));
                if (buf == NULL) {
                    free(result);
                    return NULL;
                }
                result = buf;
            }
            result[res_size++] = src[(*start)++];
        }
    } else {
        result[res_size++] = src[(*start)++];
    }
    result[res_size] = '\0';
    return result;
}


int check_is_digit(const char *src) {
    if (src == NULL || *src == '\0') return 0;
    size_t len = strlen(src);
    for (size_t i = 0; i < len; i++) {
        unsigned char cur = src[i];
        if (!isdigit(cur)) {
            return 0;
        }
    }
    return 1;
}

int get_priority(const char *op) {
    if (strcmp(op, "(") == 0) return 0;
    if (strcmp(op, ")") == 0) return 1;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 2;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) return 3;
    return -1;
}

int poliz(char **dst, const char *src, size_t n) {
    if (src == NULL || n == 0) return -1;
    Stack *stack = NULL;
    size_t dst_size = 0;
    size_t i = 0;

    char *cur;
    while ((cur = get_string(src, &i, n)) != NULL) {
        if (check_is_digit(cur)) {
            dst[dst_size++] = cur;
        } else if (strcmp(cur, "(") == 0) {
            push(&stack, cur);
        } else if (strcmp(cur, ")") == 0) {
            while (!is_empty(&stack) && strcmp(peek(&stack), "(") != 0) {
                dst[dst_size++] = pop(&stack);
            }
            if (!is_empty(&stack)) {
                char *open_bracket = pop(&stack);
                free(open_bracket);
            }
            free(cur);
        } else {
            int p_cur = get_priority(cur);
            while (!is_empty(&stack) && get_priority(peek(&stack)) >= p_cur) {
                dst[dst_size++] = pop(&stack);
            }
            push(&stack, cur);
        }
    }

    while (!is_empty(&stack)) {
        char *current = pop(&stack);
        if (strcmp(current, "(") == 0) {
            free(current);
        } else {
            dst[dst_size++] = current;
        }
    }

    return (int) dst_size;
}


int build_tree(TreeNode **root, char **src, size_t size) {
    if (src == NULL || *src == NULL) return -1;
    NodeStack *stack = NULL;
    for (int i = 0; i < size; i++) {
        TreeNode *new_node = create_tree_node(src[i]);
        if (new_node == NULL) return -1;
        if (!check_is_digit(src[i])) {
            new_node->right = pop_node(&stack);
            new_node->left = pop_node(&stack);
            if (new_node->right == NULL || new_node->left == NULL) {
                return -1;
            }
        }
        push_node(&stack, new_node);
    }
    *root = pop_node(&stack);
    return 1;
}

int zero_filter(TreeNode **root) {
    if (root == NULL || *root == NULL) return -1;
    zero_filter(&((*root)->left));
    zero_filter(&((*root)->right));

    TreeNode *node = *root;
    if ((*root)->left == NULL && (*root)->right == NULL) return -1;

    int left_is_zero = (strcmp(node->left->value, "0") == 0);
    int right_is_zero = (strcmp(node->right->value, "0") == 0);
    if (strcmp(node->value, "*") == 0 && (left_is_zero || right_is_zero)) {
        free_tree_node(node->left);
        free_tree_node(node->right);
        node->left = NULL;
        node->right = NULL;
        free(node->value);
        node->value = strdup("0");
    } else if (strcmp(node->value, "+") == 0) {
        if (left_is_zero) {
            TreeNode *temp = node->right;
            free(node->left->value);
            free(node->left);
            free(node->value);
            free(node);
            *root = temp;
        } else if (right_is_zero) {
            TreeNode *temp = node->left;
            free(node->right->value);
            free(node->right);
            free(node->value);
            free(node);
            *root = temp;
        }
    }
    return 0;
}

int main() {
    TreeNode *root = NULL;
    char *buf = (char *) malloc(1024);
    if (buf == NULL) {
        return -1;
    }
    fgets(buf, 1024, stdin);
    buf[strcspn(buf, "\n")] = 0;
    char **expr = (char **) malloc(1024 * sizeof(char *));
    if (expr == NULL) {
        free(buf);
        return -1;
    }
    int dst_size = poliz(expr, buf, strlen(buf));

    build_tree(&root, expr, dst_size);

    printf("\n");

    zero_filter(&root);

    print_tree(root, 0);

    for (int i = 0; i < dst_size; i++) {
        free(expr[i]);
    }
    free(buf);
    free(expr);
    return 0;
}

// 2 * (2 + 3) + (0 * (4 + 5)) + (6 + 0)
// пошитые деревья