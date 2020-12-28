#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, const char *argv[])
{ 

    struct timeval *t1 = (struct timeval *)malloc(sizeof(struct timeval));
    struct timeval *t2 = (struct timeval *)malloc(sizeof(struct timeval));
    struct timezone *tz = (struct timezone *)malloc(sizeof(struct timezone));
int k=gettimeofday(t1,tz);
	long int hi = t1->tv_sec;

    size_t textsize = 1024*1024*1024+1; // + \0 null character

    char *text =(char *)malloc(textsize-1);
    memset(text,'a',textsize-1);
    
    const char *filepath = "h5.txt";

    int fd = open(filepath, O_CREAT | O_RDWR , (mode_t)0600);
    
    if (fd == -1)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    // Stretch the file size to the size of the (mmapped) array of char

    
    if (lseek(fd, textsize-1, SEEK_SET) == -1)
    {
        close(fd);
        perror("Error calling lseek() to 'stretch' the file");
        exit(EXIT_FAILURE);
    }
	write(fd,"",1);
    
    // Now the file is ready to be mmapped.
    char *map = mmap(0, textsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
memcpy(map,text,textsize);


	k=gettimeofday(t2,tz);
	long int hi2 = t2->tv_sec;
	printf("Time taken by write to write 1GB: %ld\n",hi2-hi);
    // Write it now to disk
    if (msync(map, textsize, MS_SYNC) == -1)
    {
        perror("Could not sync the file to disk");
    }
    
    // Don't forget to free the mmapped memory
    if (munmap(map, textsize) == -1)
    {
        close(fd);
        perror("Error un-mmapping the file");
        exit(EXIT_FAILURE);
    }

    // Un-mmaping doesn't close the file, so we still need to do that.
    close(fd);
    
    return 0;
}
