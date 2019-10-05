#include "types.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

int
main(int argc, char **argv)
{
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
    
    int max = sizeof(buf);
    int cc, i;
    char c;
    for(i=0; i+1 < max; ){
        cc = read(0, &c, 1);
        if(cc < 1)
            break;
        buf[i++] = c;
        if(c == '\n' || c == '\r')
            break;
    }
    if(write(fd, buf, i) != i)
          printf(1, "error: write %s to %s failed\n", buf, buffer);              
    close(fd);
    exit();
}
