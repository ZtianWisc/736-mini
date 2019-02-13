#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define LENGTH (1024UL*1024*1024)
#define PROTECTION (PROT_READ | PROT_WRITE)
#define ADDR (void *)(0x0UL)
#define FLAGS (MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB)

void check_bytes(char *addr)
{
	printf("First hex is %x\n", *((unsigned int *)addr));
}

void write_bytes(char *addr)
{
	unsigned long i;

	for (i = 0; i < LENGTH; i++)
		*(addr + i) = (char)i;
}

void read_bytes(char *addr)
{
	unsigned long i;

	check_bytes(addr);
	for (i = 0; i < LENGTH; i++)
		if (*(addr + i) != (char)i) {
			printf("Mismatch at %lu\n", i);
			break;
		}
}

int main(void)
{
	void *addr;

	addr = mmap(ADDR, LENGTH, PROTECTION, FLAGS, 0, 0);
	if (addr == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}

	printf("Returned address is %p\n", addr);
	check_bytes(addr);
	write_bytes(addr);
	read_bytes(addr);

	munmap(addr, LENGTH);

	exit(0);
}

