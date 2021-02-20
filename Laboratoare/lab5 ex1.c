#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	printf("Starting parent %d\n", getpid());

	char *shm_collatz = "shmcollatz";
	int shm_fd = shm_open(shm_collatz, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (shm_fd == -1)
	{
		perror(NULL);
		shm_unlink(shm_collatz);
		return errno;
	}
	size_t shm_size = getpagesize() * (argc - 1);
	if (ftruncate(shm_fd, shm_size) == -1)
	{
		perror(NULL);
		shm_unlink(shm_collatz);
		return errno;
	}

	for (int i = 1; i < argc; i++)
	{
		pid_t pid = fork();
		if (pid < 0)
		{
			perror(NULL);
			shm_unlink(shm_collatz);
			return errno;
		}
		else if (pid == 0)
		{
			void *shm_ptr = mmap(0, getpagesize(), PROT_WRITE, MAP_SHARED, shm_fd, getpagesize() * (i - 1));
			if (shm_ptr == MAP_FAILED)
			{
				perror(NULL);
				shm_unlink(shm_collatz);
				return errno;
			}

			char *wr_ptr = shm_ptr;
			int collatz = atoi(argv[i]);

			wr_ptr += sprintf(wr_ptr, "%d: ", collatz);
			while (collatz > 1)
			{
				wr_ptr += sprintf(wr_ptr, "%d ", collatz);
				if (collatz % 2 == 0)
				{
					collatz = collatz / 2;
				}
				else
				{
					collatz = 3 * collatz + 1;
				}
			}
			wr_ptr += sprintf(wr_ptr, "%d. \n", collatz);

			munmap(shm_ptr, getpagesize());
			printf("Done Parent %d Me %d\n", getppid(), getpid());
			exit(EXIT_SUCCESS);
		}
	}
	for (int i = 1; i < argc; i++)
		wait(NULL);

	char *shm_ptr = mmap(0, shm_size, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (shm_ptr == MAP_FAILED)
	{
		perror(NULL);
		shm_unlink(shm_collatz);
		return errno;
	}

	for (int i = 1; i < argc; i++)
	{
		printf("%s", shm_ptr + getpagesize() * (i - 1));
	}

	munmap(shm_ptr, shm_size);
	printf("Done Parent %d Me %d\n", getppid(), getpid());
	shm_unlink(shm_collatz);
	return 0;
}
