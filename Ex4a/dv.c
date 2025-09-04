#include <stdio.h>
int main() {
    int n, i, j, k;
    printf("Enter number of nodes: ");
    scanf("%d", &n);
    int cost[n][n], dist[n][n], via[n][n];
    printf("Enter cost matrix (use 999 for infinity):\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
            dist[i][j] = cost[i][j];
            via[i][j] = j;
        }
    }
    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    via[i][j] = via[i][k];
                }
            }
        }
    }
    printf("\nFinal Distance Vector Table:\n");
    for (i = 0; i < n; i++) {
        printf("From node %d:\n", i);
        for (j = 0; j < n; j++) {
            printf("To %d via %d : %d\n", j, via[i][j], dist[i][j]);
        }
        printf("\n");
    }
    return 0;
}
