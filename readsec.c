#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define	IS_ELF(ehdr)	((ehdr).e_ident[EI_MAG0] == ELFMAG0 && \
			 (ehdr).e_ident[EI_MAG1] == ELFMAG1 && \
			 (ehdr).e_ident[EI_MAG2] == ELFMAG2 && \
			 (ehdr).e_ident[EI_MAG3] == ELFMAG3)

int main(int argc, char *argv[]) {
	int fd, i;
	struct stat sb;
	char *head;
	Elf64_Ehdr *ehdr;
	Elf64_Shdr *shdr;
	Elf64_Shdr *shstr;
	
	fd = open(argv[1], O_RDONLY);
	fstat(fd, &sb);
	head = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	
	ehdr = (Elf64_Ehdr *)head;

	if (!IS_ELF(*ehdr)) {
		fprintf(stderr, "This is not ELF file. (%s)\n", argv[1]);
		exit(1);
	}
	
	shstr = (Elf64_Shdr *) (head + ehdr->e_shoff + ehdr->e_shentsize * ehdr->e_shstrndx);

	for (i = 0; i < ehdr->e_shnum; i++) {
		shdr = (Elf64_Shdr *) (head + ehdr->e_shoff + ehdr->e_shentsize * i);
		printf("%s\n", (char *)(head + shstr->sh_offset + shdr->sh_name));
	}
	
	munmap(head, sb.st_size);
	close(fd);

	exit(0);
}
