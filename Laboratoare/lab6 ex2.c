#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

void *calculeazaCij(void *);
int **A, **B, **C;

struct Cij
{
    int row;
    int col;
    int length;
};

int main(int argc, char *argv[])
{
    int m, p, n;
    pthread_t **thr;

    // verific datele de intrare
    if (argc != 4)
    {
        fprintf(stderr, "usage: ex2 <m> <p> <n>\n");
        return -1;
    }

    m = atoi(argv[1]);
    p = atoi(argv[2]);
    n = atoi(argv[3]);
    if (m <= 0 || p <= 0 || n <= 0)
    {
        fprintf(stderr, "%d, %d and %d must be greater than 0\n", m, p, n);
        return -1;
    }

    // generez si afisez matricea A(m,p)
    A = calloc(m, sizeof(int *));
    printf("Elementele matricei A sunt:\n");
    for (int i = 0; i < m; i++)
    {
        A[i] = calloc(p, sizeof(int));
        for (int j = 0; j < p; j++)
        {
            A[i][j] = rand() % 10;
            printf("%d\t", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // generez si afisez matricea B(p,n)
    B = calloc(p, sizeof(int *));
    printf("Elementele matricei B sunt:\n");
    for (int i = 0; i < p; i++)
    {
        B[i] = calloc(n, sizeof(int));
        for (int j = 0; j < n; j++)
        {
            B[i][j] = rand() % 10;
            printf("%d\t", B[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // generez matricele C(m,n) si a firelor corespondente pe care le lansez
    C = calloc(m, sizeof(int *));
    thr = calloc(m, sizeof(pthread_t *));
    for (int i = 0; i < m; i++)
    {
        C[i] = calloc(n, sizeof(int));
        thr[i] = calloc(n, sizeof(pthread_t));
        for (int j = 0; j < n; j++)
        {
            struct Cij *coords = malloc(sizeof(struct Cij));
            coords->row = i;
            coords->col = j;
            coords->length = p;

            if (pthread_create(&thr[i][j], NULL, calculeazaCij, coords))
            {
                perror(NULL);
                return errno;
            }
        }
    }

    // astept finalizarea firelor de executie si afisez matricea C(m,n)
    printf("Elementele matricei C sunt:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (pthread_join(thr[i][j], NULL))
            {
                perror(NULL);
                return errno;
            }
            printf("%d\t", C[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //eliberez memoria matricelor A(m,p), B(p,n), C(m,n) si a firelor corespondente
    for (int i = 0; i < m; i++)
    {
        free(A[i]);
    }
    free(A);
    for (int i = 0; i < p; i++)
    {
        free(B[i]);
    }
    free(B);
    for (int i = 0; i < m; i++)
    {
        free(C[i]);
        free(thr[i]);
    }
    free(C);
    free(thr);

    return 0;
}

void *calculeazaCij(void *v)
{
    struct Cij *param = v;
    int sum = 0;

    for (int i = 0; i < param->length; i++)
    {
        sum += A[param->row][i] * B[i][param->col];
    }
    C[param->row][param->col] = sum;

    free(param);
    return NULL;
}
