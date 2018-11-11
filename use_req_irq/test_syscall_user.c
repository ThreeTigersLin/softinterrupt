#include <stdio.h>
//#inlcude <linux/kernel.h>
#include <unistd.h>   
#include <sys/syscall.h>
#define __NR_sys_mycall 333

//extern long int _syscall(long int_sysno,...)__THROW;

//and then a simple call is done as such

int main()
{
	int err;
	err = syscall(__NR_sys_mycall, 0);

	printf("The value is %d\n",err);
}
