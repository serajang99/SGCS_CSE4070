#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>


//int fib (int n);
//int max_of_four_intt(int,int, int, int);

int
main (int argc, char **argv)
{

//	printf("%s %s %s %s %s\n",argv[0],argv[1],argv[2],argv[3],argv[4]);
//	printf("%d %d %d %d %d\n",(int)argv[0],atoi(argv[1]),(int)argv[2],(int)argv[3],(int)argv[4]);


  printf("%d %d\n",fibonacci(atoi(argv[1])),max_of_four_int(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4])));

  return EXIT_SUCCESS;
}


/*additional system call*/
/*int fib (int n){
        int f[3] = {0,1,0};
        int i;
        for(i=2;i<=n;++i)
                f[i%3] = f[(i+1)%3] + f[(i+2)%3];
        return f[n%3];
}

int max_of_four_intt (int a, int b, int c, int d){
        int max = a;
        if(b>max)
                max = b;
        if(c>max)
                max = c;
        if(d>max)
                max = d;
        return max;
}
*/
