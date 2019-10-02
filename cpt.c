#include "types.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char **argv)
{
	// printf(1, "argc:  %d\n", argc);
	// printf(1, "argv1:  %s\n", argv[1]);
	// printf(1, "argv2:  %s\n", argv[2]);
	if(argc <= 1){
		printf(2, "error cpt: missing file operand\n");
	    exit();
  	}
	char* buffer = argv[1];
 	int fd = open(buffer, O_CREATE | O_RDWR);
    if(fd < 0) {
        printf(1, "error cpt: create file failed\n");
        exit();
    }
    // int a;
    // scanf("%d", &a);
    close(fd);
	
    return 0;
}
