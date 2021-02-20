#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

void *reverseString(void *);
char *result;

int main(int argc, char *argv[]) {
    pthread_t thr;

    if (argc != 2) {
        fprintf(stderr, "usage: ex1 <string>\n");
        return -1;
    }

    if (pthread_create(&thr, NULL, reverseString, argv[1])) {
        perror(NULL);
        return errno;
    }

    if (pthread_join(thr, NULL)) {
        perror(NULL);
        return errno;
    }

    printf("%s scris invers este %s\n", argv[1], result);
    free(result);
    return 0;
}

void *reverseString(void *param) {
    char *word = param;
    result = malloc(strlen(word));
    for (int i = 0; i < strlen(word); i++) {
        result[i] = word[strlen(word) - i - 1];
    }

    return NULL;
}
