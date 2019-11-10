#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"


int main(int argc, char *argv[])
{
	int pid = getpid();
	printf(1, "pid %d\n", pid);
	int child_pid = fork();
	if (child_pid != 0)
	{
		int second_child_pid = fork();
		if (second_child_pid != 0)
		{
			int children_pids = get_children_pid(pid);
			printf(1, "children pids: %d\n", children_pids);	
		}
	}
	exit();
}
