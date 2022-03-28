#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "userprog/process.h"
#include "userprog/exception.h"
#include "devices/input.h"
#include <stdlib.h>
#include <string.h>


static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
//	printf("tes\n");
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
//  printf("end\n");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
//  printf("syscall\n");
  int *syscall_num = (f->esp);

  if(!is_user_vaddr(syscall_num))
	  syscall_exit(-1);

//  printf("%d\n",syscall_num[0]);
  switch(syscall_num[0]){
	  case SYS_HALT : syscall_halt(); 
					  break;

	  case SYS_EXIT : if(is_user_vaddr(&syscall_num[1]))
						  syscall_exit(syscall_num[1]);
					  else
						  syscall_exit(-1);
					  break;

	  case SYS_EXEC :if(is_user_vaddr(&syscall_num[1]))
						  f->eax = (uint32_t)syscall_exec((const char*)syscall_num[1]);
					  else
						  syscall_exit(-1);
					  break;

	  case SYS_WAIT : f->eax = (uint32_t)syscall_wait((pid_t)syscall_num[1]);
					  break;

	  case SYS_READ : if(is_user_vaddr(&syscall_num[1]) && is_user_vaddr(&syscall_num[2]) && is_user_vaddr(&syscall_num[3]))
						  f->eax = (uint32_t)syscall_read(syscall_num[1],(void*)syscall_num[2],(unsigned)syscall_num[3]);
					  else
						  syscall_exit(-1);
					  break;

	  case SYS_WRITE : if(is_user_vaddr(&syscall_num[1]) && is_user_vaddr(&syscall_num[2]) && is_user_vaddr(&syscall_num[3]))
							f->eax = (uint32_t)syscall_write(syscall_num[1],(void*)syscall_num[2],(unsigned)syscall_num[3]);
						else 
							syscall_exit(-1);
						break;

	  case SYS_FIB : if(is_user_vaddr(&syscall_num[1]))
						 f->eax = (uint32_t)fibonacci(syscall_num[1]);
					 else
						 syscall_exit(-1);
					 break;

	  case SYS_MAX : if(is_user_vaddr(&syscall_num[4]))
						 f->eax = (uint32_t)max_of_four_int(syscall_num[1],syscall_num[2],syscall_num[3],syscall_num[4]);
					 else
						 syscall_exit(-1);
					 break;

	  default : thread_exit();
				break;
  }

}

void syscall_halt(void){
	shutdown_power_off();
}

void syscall_exit(int status){
	
	struct thread *t = thread_current();
	t->is_end =1;
	t->exit_status = status;
	list_remove(&(t->child_elem));
	printf("%s: exit(%d)\n",t->name,t->exit_status);

	if(t->parent)
		t->parent->exit_status = t->exit_status;

	thread_exit();
}

pid_t syscall_exec(const char *cmd_line){
	return (pid_t)process_execute(cmd_line);
}

int syscall_wait(pid_t pid){
	return process_wait(pid);
}

int syscall_read(int fd, void *buffer, unsigned size){
	unsigned i;
	char *buf = (char*)buffer;

  // stdin
	if(fd==0){
      for(i=0;i<size;i++){
		if((buf[i]=(char)input_getc())=='\0')
			break;
	  }
      return i;
    }
	return -1;
}

int syscall_write(int fd, const void *buffer, unsigned size){
	
	// stdout
	if(fd==1){
		putbuf((char*)buffer,size);
		return size;
    }
	return -1;
}

/*additional system call*/
int fibonacci (int n){
	int f[3] = {0,1,0};
	int i;
	for(i=2;i<=n;++i)
		f[i%3] = f[(i+1)%3] + f[(i+2)%3];
	return f[n%3];
}

int max_of_four_int (int a, int b, int c, int d){
	int max = a;
	if(b>max)
		max = b;
	if(c>max)
		max = c;
	if(d>max)
		max = d;
	return max;
}

