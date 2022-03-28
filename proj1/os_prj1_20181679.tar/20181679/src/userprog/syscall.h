#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H


#include "lib/user/syscall.h"


//void syscall_init (void);
void syscall_halt(void);
void syscall_exit(int status);
pid_t syscall_exec(const char *cmd_line);
int syscall_wait(pid_t pid);
int syscall_read(int fd, void *buffer, unsigned size);
int syscall_write(int fd, const void *buffer, unsigned size);
int fibonacci(int n);
int max_of_four_int(int a, int b, int c, int d);

void syscall_init (void);

#endif /* userprog/syscall.h */
