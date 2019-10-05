#include "types.h"
#include "user.h"
#include "fcntl.h"

char buf[2048];

int
main(int argc, char **argv)
{
	if(argc <= 1){
		printf(2, "error cpt: missing file operand\n");
	    exit();
  	}else if (argc == 2){
        char* buffer = argv[1];
        int fd = open(buffer, O_CREATE | O_RDWR);
        if(fd < 0) {
            printf(2, "error cpt: failed to create file %s\n", buffer);
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
              printf(2, "error: write %s to %s failed\n", buf, buffer);              
        close(fd);   
    }else if (argc == 3){
        char* in_buf = argv[1];
        int in_fd = open(in_buf, O_RDWR);
        if(in_fd < 0) {
            printf(2, "error cpt: failed to open %s no such file\n", in_buf);
            exit();
        }

        char* out_buf = argv[2];
        int out_fd = open(out_buf, O_CREATE | O_RDWR);
        if(out_fd < 0) {
            printf(2, "error cpt: failed to create file %s\n", out_buf);
            exit();
        }

        int max = sizeof(buf);
        int cc, i;
        char c;
        for(i=0; i+1 < max; ){
            cc = read(in_fd, &c, 1);
            if(cc < 1)
                break;
            buf[i++] = c;
            if(c == '\n' || c == '\r')
                break;
        }
        close(in_fd);
        
        if(write(out_fd, buf, i) != i)
              printf(2, "error: write %s to %s failed\n", buf, out_buf);              
        close(out_fd); 
    }
    exit();
}
