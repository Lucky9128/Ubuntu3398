#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/time.h>
#include<sys/stat.h>


       #define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


void main(int argc,char *argv[])
{
	int fd;
	if((fd = open("file2.txt",O_CREAT|O_RDWR|__O_LARGEFILE,S_IRWXU|S_IRWXG|S_IRWXO))<0)
	{
		perror("file openign error");
		exit(0);
	}   
	FILE *fp = fdopen(fd,"wb");
	struct timeval *t = (struct timeval *)malloc(sizeof(struct timeval));
	struct timeval *t2 = (struct timeval *)malloc(sizeof(struct timeval));
	struct timezone *tz = (struct timezone *)malloc(sizeof(struct timezone));
	int k=gettimeofday(t,tz);
	long int hi = t->tv_sec;

	char *text = (char *)malloc(sizeof(char)*1024*1024*1024);
	memset(text,'a',1024*1024*1024);

		write(fd,text,1024*1024*1024);
	k=gettimeofday(t2,tz);
	long int hi2 = t2->tv_sec;
	printf("Time taken by write to write 1GB: %ld\n",hi2-hi);
	fclose(fp);
	close(fd); 
	
	// char *addr;
	// fd = open("fiel3.txt", O_CREAT|O_RDWR|__O_LARGEFILE,S_IRWXU|S_IRWXG|S_IRWXO);
	// if (fd == -1)
	// 	handle_error("open");
	// struct stat sb;
	// size_t offset, pa_offset;
	// size_t length;
	// ssize_t s;
	// if (fstat(fd, &sb) == -1)           /* To obtain file size */
	// 	handle_error("fstat");
	// offset = atoi(argv[2]);
	// pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
	// 	/* offset for mmap() must be page aligned */

	// printf("%lu\n",);
	// if (offset >= sb.st_size) {
	// 	fprintf(stderr, "offset is past end of file\n");
	// 	exit(EXIT_FAILURE);
	// }

	// if (argc == 4) {
	// 	length = atoi(argv[3]);
	// 	if (offset + length > sb.st_size)
	// 		length = sb.st_size - offset;
	// 			/* Can't display bytes past end of file */

	// } else {    /* No length arg ==> display to end of file */
	// 	length = sb.st_size - offset;
	// }

	// addr = mmap(NULL, length + offset - pa_offset, PROT_READ,
	// 			MAP_PRIVATE, fd, pa_offset);
	// if (addr == MAP_FAILED)
	// 	handle_error("mmap");

	// s = write(STDOUT_FILENO, addr + offset - pa_offset, length);
	// if (s != length) {
	// 	if (s == -1)
	// 		handle_error("write");

	// 	fprintf(stderr, "partial write");
	// 	exit(EXIT_FAILURE);
	// }

	// munmap(addr, length + offset - pa_offset);
	// close(fd);
}
