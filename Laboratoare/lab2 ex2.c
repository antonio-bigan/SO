#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {

	if(argc != 3) {
		write(1, "Use mycp [source] [destination]\n", 20);
		return 1;
	}

	int sfd = open(argv[1], O_RDONLY);
	if (sfd < 0) {
		perror("Error occurred during opening input file");
		return 1;
	}

	struct stat sb;
	if (stat(argv[2], &sb) != -1 && errno != 2) {
		printf("Error occurred during opening output file: File already exists\n");
		return 1;
	}

	int dfd = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	char *content = malloc(1000*sizeof(char));
	while (1) {

		int bytesR =  read(sfd, content, 1000);
		if (bytesR == -1) {
			perror("Error occurred during reading from input file");
			return 1;
		}
		if (bytesR == 0) {
			break;
		}
		int bytesW = write(dfd, content, bytesR);
		if (bytesW >= 0 && bytesW < bytesR) {
			bytesW = write(dfd, content + bytesW, bytesR - bytesW);
		}
		if (bytesW == -1) {
			perror("Error occurred during writing to output file");
			return 1;
		}
	}
	close(sfd);
	close(dfd);
	printf("File  %s  has been successfully copied to  %s\n", argv[1], argv[2]);

	return 0;
}