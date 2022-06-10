#ifndef FT_NM_H
#define FT_NM_H

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

#include "libft.h"

/* nm processing */
int ft_nm_x64(void *file, size_t filesize);
int ft_nm_x32(void *file, size_t filesize);


/* print_debug.c */
void print_sym(Elf64_Sym *Ssymtab, char *Sstrtab, int j);
void print_Ehdr(Elf64_Ehdr *Ehdr);
void print_Shdr(Elf64_Shdr *Shdr, int i);

#endif
