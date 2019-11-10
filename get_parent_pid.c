#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"


int main(int argc, char *argv[])
{
    int child_pid = fork();
	if (child_pid == 0)
	{
		int pid = getpid();
        int ppid = get_parent_pid();
		printf(1, " child proc -> with pid: %d and parent pid: %d\n", pid, ppid);	
	}else{
		int pid = getpid();
		printf(1, " parent proc -> with pid: %d and child pid: %d\n", pid, child_pid);	
		wait();
	}
   exit();
}
