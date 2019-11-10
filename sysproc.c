#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int 
sys_count_number_of_digits(void)
{
  struct proc *curproc = myproc();
  int num = curproc->tf->edx;
  int counter = 0;
  // cprintf("number passed by register : %d \n",  num);
  if (num == 0)
  {
    counter++;
  }else{
    while(num != 0)
    {
      counter++;
      num /= 10;
    }
  }
  cprintf("number digits count : %d \n",  counter);
  return 0;
}

int 
sys_get_parent_pid(void){
  return myproc()->parent->pid;
}

int
sys_get_children_pid(void){
  int pid;
  if(argint(0, &pid) < 0)
    return -1;
  return children(pid);
}

int
sys_gettime(void)
{
  struct rtcdate r;
  cmostime(&r);
  cprintf("current time: %d:%d:%d\n", (&r)->hour, (&r)->minute, (&r)->second);
  return r.second;
}

int
sys_sleepsec(void)
{
  uint xticks = 0;
  int secs;
  uint startTime;
  uint finishTime;
  if (argint(0, &secs) < 0) {
    return -1;
  }
  startTime = sys_uptime();
  while (((int)xticks - (int)startTime) < secs*100)
  {
    acquire(&tickslock);
    xticks = ticks;
    release(&tickslock);
  }
  finishTime = sys_uptime();
  return ((int)finishTime-(int)startTime)/100;
}
