#ifndef FT_NM_H
#define FT_NM_H

#include <elf.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "libft.h"

#define SYMBUFSIZE 128
#define NM_TYPE_NB 12

typedef struct {
  char nmType;
  char sectionName[15];
} s_nmTypename;

/* nm processing */
int ft_nm_x64(const void *file, size_t filesize);
int ft_nm_x32(void *file, size_t filesize);

/* print_debug.c */
void print_sym(Elf64_Sym *Ssymtab, char *Sstrtab, int j);
void print_Ehdr(Elf64_Ehdr *Ehdr);
void print_Shdr(Elf64_Shdr *Shdr, int i);

/* shared.c */
void concat_address(char *dst, unsigned long n);
void format_buffer(char *buf, unsigned long addr, const char *symname,
		   int symbind, const char *sectionName);

#endif
