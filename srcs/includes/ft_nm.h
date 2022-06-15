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

/* values for output buffer and display */
#define SYMBUFSIZE 320
#define NM_TYPE_NB 14

/* values to hold some data relative to .strtab and .shstrtab */
#define STRTAB 0
#define SHSTRTAB 1

/* tuple to link section name with nm symbol type */
typedef struct {
  char nmType;
  char sectionName[31];
} s_nmTypename;

/* nm processing */
int ft_nm_x64(const void *file, size_t filesize);
int ft_nm_x32(void *file, size_t filesize);

/* print_debug.c */
void print_sym(Elf64_Sym *Ssymtab, char *Sstrtab, int j);
void print_Ehdr(Elf64_Ehdr *Ehdr);
void print_Shdr(Elf64_Shdr *Shdr, int i);

/* sorting.c */
void asc_sort(char **symbol, int size);

/* format_output.c */
void concat_address(char *dst, unsigned long n);
void format_output(char *buf, unsigned long addr, const char *symname,
                   unsigned char st_info, uint16_t st_shndx,
                   const char *sectionName);

/* shared.c */
int is_special_section_indice(uint16_t section_index);
void destroy_buffer(char **symbol, char *output_buf, int symb_nb);

#endif
