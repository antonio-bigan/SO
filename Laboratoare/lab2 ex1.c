#include <unistd.h>
#include <string.h>

int main() {
	char *content = "Hello World!\n";
	write(1, content, strlen(content));
	return 0;
}
