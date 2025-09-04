#include <stdio.h>
int main() {
    int n, i, j, src;
    printf("Enter number of nodes: ");
    scanf("%d", &n);
    int cost[n][n];
    printf("Enter cost adjacency matrix (999 for infinity):\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &cost[i][j]);
    printf("Enter source node: ");
    scanf("%d", &src);
    int dist[n], visited[n], prev[n];
    for (i = 0; i < n; i++) {
        dist[i] = cost[src][i];
        prev[i] = src;
        visited[i] = 0;
    }
    dist[src] = 0;
    visited[src] = 1;
    for (i = 1; i < n; i++) {
        int u = -1, min = 999;
        for (j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < min) {
                min = dist[j];
                u = j;
            }
        }
        visited[u] = 1;
        for (j = 0; j < n; j++) {
            if (!visited[j] && dist[j] > dist[u] + cost[u][j]) {
                dist[j] = dist[u] + cost[u][j];
                prev[j] = u;
            }
        }
    }
    printf("\nShortest paths from source %d:\n", src);
    for (i = 0; i < n; i++) {
        printf("To %d : cost = %d via %d\n", i, dist[i], prev[i]);
    }
    return 0;
}
