#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "defs.h"
#include "x86.h"
#include "elf.h"

#define MAXNUMPATH 10

// keep all pathes in a 2d array
char pathes[MAXNUMPATH][1024];

// the file path
char file_path[2048];

int 
sys_setPath(void){  
  char* envPathInput;
  if(argstr(0, &envPathInput) < 0)  return -1;

  memset(pathes, 0, sizeof(file_path) / 2 * MAXNUMPATH);
  //cprintf("%s wq", envPathInput);
  int pathPos = 0;
  int pathNum = 0;
  int i = 0;
  if (envPathInput[0] == 'P' && envPathInput[1] == 'A' && envPathInput[2] == 'T' && envPathInput[3] == 'H' && envPathInput[4] == ' '){
    for(i = 5; i < strlen(envPathInput); i++){
      if(envPathInput[i] == ':'){
        pathes[pathNum++][pathPos] = '\0';
        pathPos = 0;
      }
      else  pathes[pathNum][pathPos++] = envPathInput[i];
    }
    for(i = 0; i<10; i++){
      cprintf("%s\n", pathes[i]);
    }
  }
  else cprintf("please enter PATH variable\n");
  return 0;
}

int
exec(char *path, char **argv)
{
  char *s, *last;
  int i, off;
  uint argc, sz, sp, ustack[3+MAXARG+1];
  struct elfhdr elf;
  struct inode *ip;
  struct proghdr ph;
  pde_t *pgdir, *oldpgdir;
  int length;
  int found = 0;
  struct proc *curproc = myproc();
  int n;
  
  if((ip = namei(path)) == 0){
    length = strlen(path);

    for(i = 0; i < MAXNUMPATH && !found; i++){
      memset(file_path, 0, sizeof(char) * 2048);

      n = 0;

      for(n=0; n < strlen(pathes[i]); n++)  file_path[n] = pathes[i][n];

      for(n=0; n < length; n++) file_path[strlen(pathes[i]) + n] = path[n];

      file_path[strlen(pathes[i]) + length] = '\0';

      if((ip = namei(file_path)) != 0){
        path = file_path;
        found = 1;
      }
    }

    if(!found)  return -1;
  }

  ilock(ip);
  pgdir = 0;

  // Check ELF header
  if(readi(ip, (char*)&elf, 0, sizeof(elf)) < sizeof(elf))
    goto bad;
  if(elf.magic != ELF_MAGIC)
    goto bad;

  if((pgdir = setupkvm()) == 0)
    goto bad;

  // Load program into memory.
  sz = 0;
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    if(readi(ip, (char*)&ph, off, sizeof(ph)) != sizeof(ph))
      goto bad;
    if(ph.type != ELF_PROG_LOAD)
      continue;
    if(ph.memsz < ph.filesz)
      goto bad;
    if((sz = allocuvm(pgdir, sz, ph.vaddr + ph.memsz)) == 0)
      goto bad;
    if(loaduvm(pgdir, (char*)ph.vaddr, ip, ph.off, ph.filesz) < 0)
      goto bad;
  }

  iunlockput(ip);
  ip = 0;

  // Allocate two pages at the next page boundary.
  // Make the first inaccessible.  Use the second as the user stack.
  sz = PGROUNDUP(sz);
  if((sz = allocuvm(pgdir, sz, sz + 2*PGSIZE)) == 0)
    goto bad;
  clearpteu(pgdir, (char*)(sz - 2*PGSIZE));
  sp = sz;

  // Push argument strings, prepare rest of stack in ustack.
  for(argc = 0; argv[argc]; argc++) {
    if(argc >= MAXARG)
      goto bad;
    sp = (sp - (strlen(argv[argc]) + 1)) & ~3;
    if(copyout(pgdir, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
      goto bad;
    ustack[3+argc] = sp;
  }
  ustack[3+argc] = 0;

  ustack[0] = 0xffffffff;  // fake return PC
  ustack[1] = argc;
  ustack[2] = sp - (argc+1)*4;  // argv pointer

  sp -= (3+argc+1) * 4;
  if(copyout(pgdir, sp, ustack, (3+argc+1)*4) < 0)
    goto bad;

  // Save program name for debugging.
  for(last=s=path; *s; s++)
    if(*s == '/')
      last = s+1;
  safestrcpy(curproc->name, last, sizeof(curproc->name));

  // Commit to the user image.
  oldpgdir = curproc->pgdir;
  curproc->pgdir = pgdir;
  curproc->sz = sz;
  curproc->tf->eip = elf.entry;  // main
  curproc->tf->esp = sp;
  switchuvm(curproc);
  freevm(oldpgdir);
  return 0;

 bad:
  if(pgdir)
    freevm(pgdir);
  if(ip)
    iunlockput(ip);
  return -1;
}
