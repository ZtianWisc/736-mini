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
    wt = fopen("write", "wb");
    if (rd == NULL || wt == NULL){
        exit(1);
    }
    if (strcmp(argv[1], "-d")==0){
        setvbuf(rd, NULL, _IONBF, 0);
    } else {
        io_buffer = (char*) malloc(atoi(argv[1]) * sizeof(char));
        setvbuf(rd, io_buffer, _IOFBF, atoi(argv[1]));
    }
    io_bufsize = __fbufsize(rd);
    printf("The buffer size is %zd\n", io_bufsize);

    // write from read to write
    size_t n_char = atoi(argv[2]);
    char* buffer = (char*) malloc(n_char * sizeof(char));
    while(fread(buffer, sizeof(char), n_char, rd) != NULL){
        fwrite(buffer, sizeof(char), n_char, wt);
    }
    fclose(rd);
    fclose(wt);
    exit(0);
}
