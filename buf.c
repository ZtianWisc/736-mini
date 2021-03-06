#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char* argv[])
{
    struct timeval tv1, tv2;
    FILE *rd;
    FILE *wt;
    size_t io_bufsize;
    char *io_buffer;

    rd = fopen("read", "rd");
    wt = fopen("write", "wb");
    if (rd == NULL || wt == NULL){
        exit(1);
    }
    if (strcmp(argv[1], "-d")==0){
        setvbuf(rd, NULL, _IONBF, 0);
        setvbuf(wt, NULL, _IONBF, 0);
    } else {
        io_buffer = (char*) malloc(atoi(argv[1]) * sizeof(char));
        setvbuf(rd, io_buffer, _IOFBF, atoi(argv[1]));
        setvbuf(wt, io_buffer, _IOFBF, atoi(argv[1]));
    }
    io_bufsize = __fbufsize(rd);
    printf("The I/O buffer size is %zd\n", io_bufsize);

    // write from read to write
    size_t n_char = atoi(argv[2]);
    char* buffer = (char*) malloc(n_char * sizeof(char));
    // get start time
    gettimeofday(&tv1, NULL);
    while(fread(buffer, sizeof(char), n_char, rd) != 0){
        fwrite(buffer, sizeof(char), n_char, wt);
    }
    // get done time
    fclose(rd);
    fclose(wt);
    gettimeofday(&tv2, NULL);
    printf("Time cost:   (%ld).(%ld)\n", tv2.tv_sec - tv1.tv_sec,
           tv2.tv_usec - tv1.tv_usec);
    exit(0);
}
