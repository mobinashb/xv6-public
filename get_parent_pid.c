#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"


int main(int argc, char *argv[])
{
    // int pid = fork();
	// if (pid == 0)
	// {
	// 	printf(1, "child proc gets parent id: %s\n", get_parent_pid());
	// }else{
	// 	printf(1, "parent proc gets parent id: %s\n", get_parent_pid());
	// }
   int ppid = get_parent_pid();
   printf(1, "parent pid: %d\n", ppid);
   exit();
}
