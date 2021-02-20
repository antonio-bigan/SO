#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	pid_t process_id = fork();
	if (process_id < 0)
		return errno;
	else if (process_id == 0)
	{
		char *argv1[] = {"ls", NULL};
		execve("/bin/ls", argv1, NULL);
	}
	else
	{
		printf("My PID=%d, Child PID=%d \n", getpid(), process_id);
		int child_id = wait(NULL);
		printf("Child %d finished \n", child_id);
	}

	return 0;
}
