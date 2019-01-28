#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char* argv[])
{
    struct timeval tv1, tv2;
    FILE *wt;
    size_t n_bytes_per_file;
    size_t n_files_write;
    size_t n_buffer;
    int direct_io;

    if (argc != 5){
        printf("Usage: ./direct #_of_bytes_per_file #_of_files_to_write -b/-d fwrite_buffer_size");
        exit(1);
    }
    // parse number of bytes per file
    n_bytes_per_file = atoi(argv[1]);
    // parse number of files to write
    n_files_write = atoi(argv[2]);
    // parse direct io / bufferred io
    if (strcmp(argv[3], "-d")==0){
        direct_io = 1;
    } else if (strcmp(argv[3], "-b")==0){
        direct_io = 0;
    } else {
        printf("Usage: ./direct #_of_bytes_per_file #_of_files_to_write -b/-d fwrite_buffer_size");
    }
    // parse fwrite buffer size
    n_buffer = atoi(argv[4]);
    char* fwrite_buffer = (char*) malloc(n_buffer * sizeof(char));
    // start writting files
    gettimeofday(&tv1, NULL);
    size_t files_written = 0;
    while (files_written < n_files_write){
        files_written++;
        wt = fopen("dummy.txt", "w");
        if (wt == NULL){
            exit(1);
        }
        if (direct_io){
            setvbuf(wt, NULL, _IONBF, 0);
        } else {
            char *io_buffer = (char*) malloc(4096 * sizeof(char));
            setvbuf(wt, io_buffer, _IOFBF, 4096);
        }
        size_t bytes_written = 0;
        while (bytes_written < n_bytes_per_file){
            fwrite_buffer[0] = 'a';
            fwrite_buffer[1] = '\0';
            bytes_written += fwrite(fwrite_buffer, sizeof(char), strlen(fwrite_buffer) + 1, wt);
        }
        fclose(wt);
    }
    gettimeofday(&tv2, NULL);
    if (tv1.tv_usec > tv2.tv_usec){
      tv2.tv_sec--;
      tv2.tv_usec += 1000000;
    }
    printf("Time cost:   %ld.%lds\n", tv2.tv_sec - tv1.tv_sec, 
            tv2.tv_usec - tv1.tv_usec);
    exit(0);
}

