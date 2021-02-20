#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Use: ex2 number");
		return -1;
	}
	pid_t process_id = fork();
	if (process_id < 0)
		return errno;
	else if (process_id == 0)
	{
		int collatz = atoi(argv[1]);
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
	}
	else
	{
		int child_id = wait(NULL);
		printf("Child %d finished \n", child_id);
	}

	return 0;
}
