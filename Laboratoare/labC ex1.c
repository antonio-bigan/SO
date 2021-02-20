#include <stdio.h>
#include <stdlib.h>

int main() {
	int n;
	scanf("%d", &n);
	int **matrice;
	matrice = calloc(n, sizeof(int *));
	for (int i = 0; i < n; i++) {
		matrice[i] = calloc(n, sizeof(int));
		for (int j = 0; j < n; j++) {
			scanf("%d", &matrice[i][j]);
		}
	}

	printf("Elementele matricei sunt:\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", matrice[i][j]);
		}
		printf("\n");
	}

	if (n % 2 == 1) {
		printf("Elementul de la intersectia diagonalelor este: %d\n", matrice[n/2][n/2]);
	}

	printf("Prima diagonala este:\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < i; j++) {
			printf("  ");
		}
		printf("%d\n", matrice[i][i]);
	}

	printf("A doua diagonala este:\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n-i-1; j++) {
			printf("  ");
		}
		printf("%d\n", matrice[i][n-i-1]);
	}
	
	for (int i = 0; i < n; i++) {
		free(matrice[i]);
	}
	free(matrice);

	return 0;
}
