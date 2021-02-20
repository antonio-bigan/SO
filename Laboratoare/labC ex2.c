#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_occurences(const char* text, const char* word);

int main() {
	char text[100];
	scanf("%[^\n]", text);
	char cuvant[20];
	scanf("%s", cuvant);
	int nrAparitii = count_occurences(text, cuvant);
	printf("Cuvantul %s apare de %d ori in textul dat\n", cuvant, nrAparitii);

	return 0;
}

int count_occurences(const char* text, const char* word) {
	char textCp[100];
	int nrAparitii = 0;
	strcpy(textCp, text);
	char * p = strtok(textCp, " ");
	while (p != NULL) {
		if (strcmp(p, word) == 0)
			nrAparitii++;
		p = strtok(NULL, " ");
	}
	return nrAparitii;
}
