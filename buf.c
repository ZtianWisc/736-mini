#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    FILE *rd;
    FILE *wt;
    size_t io_bufsize;
    char *io_buffer;

    rd = fopen("read", "rd");
    if (rd == NULL)
    {
        perror("fopen failed\n");
        return -1;
    }

    io_bufsize = __fbufsize(rd);
    printf("The buffer size is %zd\n", io_bufsize);
    if (strcmp(argv[1], "-d")==0){
        setvbuf(rd, NULL, _IONBF, 0);
    } else {
        io_buffer = (char*) malloc(atoi(argv[1]) * sizeof(char));
        setvbuf(rd, io_buffer, _IOFBF, atoi(argv[1]));
    }
    io_bufsize = __fbufsize(rd);
    printf("The buffer size is %zd\n", io_bufsize);
    exit(0);
}
