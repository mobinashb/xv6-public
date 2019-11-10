#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"


int main(int argc, char *argv[])
{
   char buffer[1024];
   printf(1, "Enter number : \n");
   read(1, buffer, 1024);
   int num = atoi(buffer);
   int reg_value;

   __asm__("movl %%edx, %0" : "=r" (reg_value));
   // printf(1, "register old value: %s\n", reg_value);
   __asm__("movl %0, %%edx" :  : "r"(num) );

   count_number_of_digits();

   __asm__("movl %0, %%edx" :  : "r"(reg_value));
   // __asm__("movl %%edx, %0" : "=r" (reg_value));
   // printf(1, "register current value: %s\n", reg_value); 
   exit();
}
