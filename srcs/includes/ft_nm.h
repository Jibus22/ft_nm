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
#define NM_TYPE_NB 15

/* values to hold some data relative to .strtab and .shstrtab */
#define STRTAB 0
#define SHSTRTAB 1

#define OOPS_SIZE 48

typedef enum {
  OOPS_UND = 0,
  OOPS_NOTELF,
  OOPS_NOCAVE,
  OOPS_BAD_ARG_NB,
  OOPS_BAD_ELF,
  OOPS_NO_LOAD,
  OOPS_BAD_PHDR,
  OOPS_BAD_SHDR,
  OOPS_BAD_SYMTAB,
  OOPS_BAD_STRTAB,
  OOPS_NB,
} e_oops;

/* tuple to link section name with nm symbol type */
typedef struct {
  char nmType;
  char sectionName[31];
} s_nmTypename;

typedef enum {
  ARG_A = 1 << 0,
  ARG_G = 1 << 1,
  ARG_U = 1 << 2,
  ARG_R = 1 << 3,
  ARG_P = 1 << 4,
} e_arg;

#define NM_MAXARGS 5
typedef struct s_args {
  char arg_name[NM_MAXARGS][3];
  int arg_flag[NM_MAXARGS];
} t_args;

/* nm processing */
int ft_nm_x64(const void *file, size_t filesize);
int ft_nm_x32(const void *file, size_t filesize);

/* print_debug.c */
void print_sym(Elf64_Sym *Ssymtab, char *Sstrtab, int j);
void print_Ehdr(Elf64_Ehdr *Ehdr);
void print_Shdr(Elf64_Shdr *Shdr, int i);

/* sorting.c */
int strcmp_nocase_asc(const void *s1, const void *s2);
int strcmp_nocase_desc(const void *s1, const void *s2);

/* format_output.c */
void format_output(t_rbt **root, unsigned long addr, const char *symname,
                   unsigned char st_info, uint16_t st_shndx,
                   const char *sectionName, const int arch);

/* shared.c */
int is_special_section_indice(uint16_t section_index);
unsigned int rbt_to_buf(t_rbt *node, char *output_buf, unsigned int ret);
bool getargs(e_arg env);

/* error.c */
int oops_error(unsigned int err);
int error_wrap(unsigned int err, t_rbt *root, char *output_buf);

#endif
