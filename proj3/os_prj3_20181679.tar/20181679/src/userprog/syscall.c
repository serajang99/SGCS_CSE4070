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

#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/synch.h"

#include "userprog/pagedir.h"

#define FILE_MAX 128
//struct semaphore mutex;
//struct semaphore sema_read;
static struct lock filelock;

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
//	printf("tes\n");
	intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
	lock_init(&filelock);
//	sema_init(&mutex,1);
//  printf("end\n");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
 //  printf("syscall\n");
  int *syscall_num = (f->esp);
  // printf("syscall %d \n", syscall_num[0]);

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

	  case SYS_CREATE : if(is_user_vaddr(&syscall_num[2]))
							f->eax = sys_create((const char*)syscall_num[1],(unsigned)syscall_num[2]);
						else 
							syscall_exit(-1);
						break;
	  
	  case SYS_REMOVE : if(is_user_vaddr(&syscall_num[1]))
							f->eax = sys_remove((const char*)syscall_num[1]);
						else 
							syscall_exit(-1);
						break;
	  
	  case SYS_OPEN : if(is_user_vaddr(&syscall_num[1]))
							f->eax = (uint32_t)sys_open((const char*)syscall_num[1]);
						else 
							syscall_exit(-1);
						break;	

	  case SYS_FILESIZE : if(is_user_vaddr(&syscall_num[1]))
							f->eax = (uint32_t)sys_filesize((int)syscall_num[1]);
						else 
							syscall_exit(-1);
						break;

	  case SYS_READ : if(is_user_vaddr(&syscall_num[1]) && is_user_vaddr(&syscall_num[2]) && is_user_vaddr(&syscall_num[3])){
						  f->eax = (uint32_t)syscall_read((int)syscall_num[1],(void*)syscall_num[2],(unsigned)syscall_num[3]);

					  }
					  else
						  syscall_exit(-1);
					  break;

	  case SYS_WRITE : if(is_user_vaddr(&syscall_num[1]) && is_user_vaddr(&syscall_num[2]) && is_user_vaddr(&syscall_num[3]))
							f->eax = (uint32_t)syscall_write(syscall_num[1],(void*)syscall_num[2],(unsigned)syscall_num[3]);
						else 
							syscall_exit(-1);
						break;

	  case SYS_SEEK : if(is_user_vaddr(&syscall_num[2]))
							sys_seek((int)syscall_num[1],(unsigned)syscall_num[2]);
						else 
							syscall_exit(-1);
						break;

	  
	  case SYS_TELL : if(is_user_vaddr(&syscall_num[1]))
							f->eax = sys_tell((int)syscall_num[1]);
						else 
							syscall_exit(-1);
						break;

	  case SYS_CLOSE : if(is_user_vaddr(&syscall_num[1]))
							sys_close((int)syscall_num[1]);
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
	printf("%s: exit(%d)\n",t->name,status);
	t->exit_status = status;

	for (int i=2;i< FILE_MAX;i++){
		if(thread_current()->fd[i] != NULL && i>=0 && i<FILE_MAX)
			sys_close(i);
	}

	thread_exit();
}

pid_t syscall_exec(const char *cmd_line){
	return (pid_t)process_execute(cmd_line);
}

int syscall_wait(pid_t pid){
	return process_wait(pid);
}


/*proj2 : file system syscalls*/
bool sys_create(const char *file, unsigned initial_size){
	if(file == NULL)
		syscall_exit(-1);
	return filesys_create(file, (off_t)initial_size);
}

int sys_remove(const char *file){
	if(file == NULL)
		syscall_exit(-1);
	return filesys_remove(file);
}

//open & close
int sys_open(const char *file){
	
	struct file *fp = NULL;
	int state = -1;

	if(file == NULL){
	//	printf("test\n");
		syscall_exit(-1);
	}
	if(!is_user_vaddr(file))
		syscall_exit(-1);


//	printf("sysopen : %s\n",file);

//	lock_acquire(&filelock);

	fp = filesys_open(file);
	if(fp != NULL){
		for(int i=2; i< FILE_MAX; i++){
			if(thread_current()->fd[i]==NULL){
				state=i;
				if(!strcmp(file,thread_current()->name))
					file_deny_write(fp);
				thread_current()->fd[i] = fp;
				break;
			}
			else 
				continue;
		}
	}
	else
		return -1;
//	lock_release(&filelock);

	return state;
}

//filesize
unsigned sys_filesize(int fd){
	if(thread_current()->fd[fd] == NULL)
		syscall_exit(-1);
	return file_length(thread_current()->fd[fd]);
}


int syscall_read(int fd, void *buffer, unsigned size){
	unsigned i;
	char *buf = (char*)buffer;
	int ret = -1;
  // stdin
//	printf("test\n");

	if(!is_user_vaddr(buffer))
		syscall_exit(-1);

	if(fd==0){
      for(i=0;i<size;i++){
		((char*)buffer)[i] = input_getc();
	  }
      ret = size;
    }
	else if(fd==1);
	else{
		if(thread_current()->fd[fd] == NULL)
			syscall_exit(-1);
		if(fd <0 || fd >= FILE_MAX)
			syscall_exit(-1);
	//	sema_down(&mutex);
		lock_acquire(&filelock);
		ret = file_read(thread_current()->fd[fd],buffer,size);	
		lock_release(&filelock);
	//	sema_up(&mutex);
	}
	return ret;
}

int syscall_write(int fd, const void *buffer, unsigned size){
	
	int ret = -1;
	if(!is_user_vaddr(buffer))
		syscall_exit(-1);

	// stdout
	if(fd==1){
		putbuf((char*)buffer,size);
		ret = size;
    }
	//
	else if(fd==0);
	else{
		if(thread_current()->fd[fd] == NULL)
			syscall_exit(-1);
		if(fd <0 || fd>=FILE_MAX)
			syscall_exit(-1);
//		if(thread_current()->fd[fd]->deny_write)
//			file_deny_write(thread_current()->fd[fd]);
	//	sema_down(&mutex);
		lock_acquire(&filelock);
		ret = file_write(thread_current()->fd[fd],buffer,size);
		lock_release(&filelock);
	//	sema_up(&mutex);

	}
	return ret;
//	return -1;
}


void sys_close(int fd){
	if(thread_current()->fd[fd] == NULL)
		syscall_exit(-1);
	file_close(thread_current()->fd[fd]);
	thread_current()->fd[fd] = NULL;
}

//file position
void sys_seek(int fd, unsigned pos){
	if(thread_current()->fd[fd] == NULL)
		syscall_exit(-1);
	file_seek(thread_current()->fd[fd],(off_t)pos);
}

unsigned sys_tell(int fd){
	if(thread_current()->fd[fd] == NULL)
		syscall_exit(-1);
	return (unsigned)file_tell(thread_current()->fd[fd]);
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

