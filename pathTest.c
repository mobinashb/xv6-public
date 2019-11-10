#include "types.h"
#include "user.h"


int main(int argc, char *argv[])
{
    char input[1024];
    char data[1024];
    read(0, input, 1024);
    if(input[0] == 's' && input[1] == 'e' && input[2] == 't'){
        int i = 0;
        while (i < strlen(input) - 4){
            data[i] = input[i + 4];
            i += 1;
        }
        setPath(data);
    }
    exit();
}
