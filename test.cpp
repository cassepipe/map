#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include "test.h"

int test(func_ptr_t func, string test_name)
{
	int wstatus = 0;
	int cpid = fork();

	if ( cpid == -1 )
		cout << "Error during test : fork() system call failed\n";
	else if ( cpid == 0 )
		func();
	else
		wait(&wstatus);
	if ( WIFSIGNALED(wstatus) )
	{
		if ( WTERMSIG(wstatus) == SIGSEGV )
			cout << test_name << "ran into a segfault\n";
		else
			cout << "Test was killed by signal n°" << WTERMSIG(wstatus) << "\n";
	}
	else if WIFEXITED(wstatus)
		return WEXITSTATUS(wstatus);
	return (-1);
}
