#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	printf("Starting parent %d\n", getpid());
	for (int i = 1; i < argc; i++)
	{
		pid_t pid = fork();
		if (pid < 0)
			return errno;
		else if (pid == 0)
		{
			int collatz = atoi(argv[i]);
			printf("%d: ", collatz);
			while (collatz > 1)
			{
				printf("%d ", collatz);
				if (collatz % 2 == 0)
				{
					collatz = collatz / 2;
				}
				else
				{
					collatz = 3 * collatz + 1;
				}
			}
			printf("%d. \n", collatz);
			goto exit;
		}
	}
	for (int i = 1; i < argc; i++)
		wait(NULL);
exit:
	printf("Done Parent %d Me %d\n", getppid(), getpid());
	return 0;
}
