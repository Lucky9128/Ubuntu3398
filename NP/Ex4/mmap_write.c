#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/time.h>
#include<sys/stat.h>

void main()
{
	int fd;
	if((fd = open("filebywrite.txt",O_CREAT|O_RDWR|__O_LARGEFILE,S_IRWXU|S_IRWXG|S_IRWXO))<0)
	{
		perror("file openign error");
		exit(0);
	}   
	size_t textsize = 1024*1024*1024+1; // + \0 null character

	struct timeval *t = (struct timeval *)malloc(sizeof(struct timeval));
	struct timeval *t2 = (struct timeval *)malloc(sizeof(struct timeval));
	struct timezone *tz = (struct timezone *)malloc(sizeof(struct timezone));
	int k=gettimeofday(t,tz);


	//time by write
	char *text = (char *)malloc(sizeof(char)*1024*1024);
	memset(text,'a',1024*1024);	
	long int tim1 = t->tv_sec;
	for(int i=0;i<1024;i++)
		write(fd,text,1024*1024);
	k=gettimeofday(t2,tz);
	long int tim2= t2->tv_sec;
	printf("Time taken by write to write 1GB: %ld\n",tim2-tim1);
	close(fd); 



	const char *filepath = "filebymmap.txt";

	fd = open(filepath, O_CREAT | O_RDWR , (mode_t)0600);

	if (fd == -1)
	{
		perror("Error opening file for writing");
		exit(EXIT_FAILURE);
	}
	if (lseek(fd, textsize-1, SEEK_SET) == -1)
	{
		close(fd);
		perror("Error calling lseek() to 'stretch' the file");
		exit(EXIT_FAILURE);
	}
	if((write(fd,"",1))<0)
	{
		close(fd);
		perror("file writing error:");
		exit(EXIT_FAILURE);
	}
	char *map = mmap(0, textsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (map == MAP_FAILED)
	{
		close(fd);
		perror("Error mmapping the file");
		exit(EXIT_FAILURE);
	}

	char *m2=map;

	//time by mmap
	k=gettimeofday(t,tz);
	tim1 = t->tv_sec;
	for(int i=0;i<1024;i++)	
	{
		memcpy(m2,text,1024*1024);
		m2+=1024*1024-1;
	}
	k=gettimeofday(t2,tz);
	tim2= t2->tv_sec;
	printf("Time taken by mmap to write 1GB: %ld\n",tim2-tim1);
	
	if (msync(map, textsize, MS_SYNC) == -1)
	{
		perror("Could not sync the file to disk");
	}

	if (munmap(map, textsize) == -1)
	{
		close(fd);
		perror("Error un-mmapping the file");
		exit(EXIT_FAILURE);
	}

	// Un-mmaping doesn't close the file, so we still need to do that.
	close(fd);   
}
