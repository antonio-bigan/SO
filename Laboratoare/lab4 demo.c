#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
//	printf("Hello, ");
	pid_t process_id = fork();
	if (process_id < 0)
		return errno;
	else if (process_id == 0)
	{
		printf("Child, id meu este %d \n", getpid());
		printf("Child, parentul meu are id %d \n", \
			getppid());
	}
	else
	{
		wait(NULL);
		printf("Parent, id meu este %d \n", getpid());
	}

	return 0;
}