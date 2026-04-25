#include <stdio.h>
#include <stdlib.h>

typedef struct Edge {
    int v, u, w;
} Edge;

int compare_edge(const void *a, const void *b) {
    const Edge *e1 = (Edge *) a;
    const Edge *e2 = (Edge *) b;
    return e1->w - e2->w;
}

int build_structs(const int n, const int m, int **graph_matrix, int *weight, Edge *edges) {
    for (int i = 0; i < m; i++) {
        printf("Введите 2 вершины и вес ребра соединяющий их: ");
        int v, u, w;
        scanf("%d %d %d", &v, &u, &w);
        graph_matrix[v][i] = 1;
        graph_matrix[u][i] = 1;
        weight[i] = w;

        edges[i].v = v;
        edges[i].u = u;
        edges[i].w = w;
    }
    return 0;
}

int find_master(int v, int *parents) {
    if (v == parents[v]) return v;
    return parents[v] = find_master(parents[v], parents);
}

void unite_slaves(const int u, const int v, int *parents) {
    const int u_master = find_master(u, parents);
    const int v_master = find_master(v, parents);
    if (u_master != v_master) {
        parents[u_master] = v_master;
    }
}

void kruskal(const int n, const int m, int *parents, const Edge *edges) {
    Edge *result = (Edge *) malloc(sizeof(Edge) * (n - 1));
    if (result == NULL) return;
    int res_weight = 0, res_count = 0;

    for (int i = 0; i < m; i++) {
        int v = edges[i].v;
        int u = edges[i].u;
        if (find_master(v, parents) != find_master(u, parents)) {
            unite_slaves(v, u, parents);

            result[res_count++] = edges[i];
            res_weight += edges[i].w;
        }
        if (res_count == n - 1) break;
    }

    printf("\nМИНИМАЛЬНОЕ ОСТОВНОЕ ДЕРЕВО\n");
    for (int i = 0; i < res_count; i++) {
        printf("%d -- %d | вес: %d\n", result[i].v, result[i].u, result[i].w);
    }
    printf("Итого: %d\n", res_weight);

    free(result);
}

void free_matrix(int **graph_matrix, const int j) {
    for (int i = 0; i < j; i++) {
        free(graph_matrix[i]);
    }
    free(graph_matrix);
}

int main() {
    printf("Введите количество вершин и рёбер через пробел: ");
    int n, m;
    scanf("%d %d", &n, &m);

    int **graph_matrix = (int **) malloc(sizeof(int *) * n);
    int *weight = (int *) malloc(sizeof(int) * m);
    Edge *edges = (Edge *) malloc(sizeof(Edge) * m);
    int *parents = (int *) malloc(sizeof(int) * n);

    if (parents == NULL) {
        free(weight);
        free(edges);
        free(graph_matrix);
        return 1;
    }
    if (edges == NULL) {
        free(graph_matrix);
        free(weight);
        free(parents);
        return 1;
    }
    if (graph_matrix == NULL) {
        free(weight);
        free(edges);
        free(parents);
        return 1;
    }
    if (weight == NULL) {
        free(graph_matrix);
        free(edges);
        free(parents);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        graph_matrix[i] = (int *) calloc(m, sizeof(int));
        if (graph_matrix[i] == NULL) {
            free_matrix(graph_matrix, i);
            free(weight);
            free(edges);
            free(parents);
            return 1;
        }
    }

    for (int i = 0; i < n; i++) {
        parents[i] = i;
    }

    build_structs(n, m, graph_matrix, weight, edges);
    qsort(edges, m, sizeof(Edge), compare_edge);

    kruskal(n, m, parents, edges);

    free_matrix(graph_matrix, n);
    free(weight);
    free(edges);
    free(parents);
    return 0;
}
4 5


