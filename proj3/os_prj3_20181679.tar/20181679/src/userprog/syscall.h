#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H


#include "lib/user/syscall.h"
#include "filesys/off_t.h"
#include "filesys/file.h"
#include "filesys/filesys.h"



void syscall_init (void);
void syscall_halt(void);
void syscall_exit(int status);
pid_t syscall_exec(const char *cmd_line);
int syscall_wait(pid_t pid);
int syscall_read(int fd, void *buffer, unsigned size);
int syscall_write(int fd, const void *buffer, unsigned size);
int fibonacci(int n);
int max_of_four_int(int a, int b, int c, int d);

/*proj2*/
bool sys_create(const char *, unsigned);
int sys_remove(const char *);
int sys_open(const char *);
unsigned sys_filesize(int);
void sys_close(int);
void sys_seek(int, unsigned);
unsigned sys_tell(int);



//void syscall_init (void);

#endif /* userprog/syscall.h */
