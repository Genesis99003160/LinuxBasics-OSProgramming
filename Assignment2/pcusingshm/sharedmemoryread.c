#include<unistd.h>
#include<sys/mman.h>
#include<sys/shm.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<semaphore.h>


int main()
{
	int filesize=4096; 
	int fd,offsetval=0;
	fd=shm_open("/shm1",O_CREAT|O_RDWR,0666);
	if(fd<0)
	{
		perror("open");
		exit(1);
	}
        ftruncate(fd, filesize);
	void *pbase;
	pbase=mmap(0,filesize, PROT_READ, MAP_SHARED,fd,offsetval);
	if(pbase==MAP_FAILED)
	{
		fprintf(stderr,"mapping failed\n");
		exit(1);
	}
	sem_t *ps,*qs;
	ps=sem_open("/s1",O_CREAT, 0777, 1);
	qs=sem_open("/s2",O_CREAT, 0777, 0);
	char bufferval[64];
	sem_wait(qs);
	sem_wait(ps);
        strncpy(bufferval,pbase,14);
	printf("buf=%s\n",bufferval);
	//strcpy(bufferval,pbase+10);
	//printf("buf=%s\n",bufferval);
	sem_post(ps);
	sem_unlink("s1");
	sem_unlink("s2");
	munmap(pbase,filesize);
	return 0;
}
