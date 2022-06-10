#include "ft_nm.h"

static int format_buffer(Elf64_Sym *Ssymtab, char *symbol, char *Sstrtab) {
  if (Ssymtab->st_shndx == SHN_ABS) return EXIT_SUCCESS;
  if (Ssymtab->st_value)
    sprintf(symbol, "%016lx %s\n", Ssymtab->st_value,
        &Sstrtab[Ssymtab->st_name]);
  else
    sprintf(symbol, "%16c %s\n", ' ', &Sstrtab[Ssymtab->st_name]);
  return EXIT_SUCCESS;
}

static int buffer_nm(Elf64_Sym *Ssymtab, char *Sstrtab, int j) {
  char **symbol = (char **)malloc(j * sizeof(char **));
  char *output_buf = (char *)malloc(j * 128 * sizeof(char));
  int ret = 0;

  if (!symbol || !output_buf) return EXIT_FAILURE;
  ft_bzero(output_buf, j * 128);
  for (int i = 0; i < j; i++) {
    symbol[i] = (char *)malloc(128);
    if (!symbol[i]) return EXIT_FAILURE;
    ft_bzero(symbol[i], 128);
  }

  for (int i = 0; i < j; i++) format_buffer(&Ssymtab[i], symbol[i], Sstrtab);

  /* here some sorting */

  for (int i = 0; i < j; i++)
    ret += ft_strlcpy(output_buf + ret, symbol[i], 128);

  write(1, output_buf, ret);

  for (int i = 0; i < j; i++) free(symbol[i]);
  free(symbol);
  free(output_buf);

  return EXIT_SUCCESS;
}

static int handle_symtab(void *file, Elf64_Ehdr *Ehdr, Elf64_Shdr *Shdrt,
                         int symtab, int filesize) {
  int j = 0, strtab = Shdrt[symtab].sh_link;
  char *Sstrtab;
  Elf64_Sym *Ssymtab;

  if (sizeof(*Ssymtab) != Shdrt[symtab].sh_entsize ||
      Shdrt[symtab].sh_link >= Ehdr->e_shnum ||
      filesize < Shdrt[strtab].sh_offset + Shdrt[strtab].sh_size ||
      filesize < Shdrt[symtab].sh_offset + Shdrt[symtab].sh_size)
    return EXIT_FAILURE;

  Sstrtab = (char *)(file + Shdrt[strtab].sh_offset);
  Ssymtab = file + Shdrt[symtab].sh_offset;

  for (int i = 0; i < Shdrt[symtab].sh_size; i += sizeof(*Ssymtab)) {
    /* print_sym(Ssymtab, Sstrtab, j); */
    j++;
  }

  buffer_nm(Ssymtab, Sstrtab, j);

  return EXIT_SUCCESS;
}

int ft_nm_x64(void *file, size_t filesize) {
  Elf64_Ehdr Ehdr;
  Elf64_Shdr *Shdrt;
  int symtab = 0;

  if (filesize < sizeof(Ehdr)) return EXIT_FAILURE;
  ft_memcpy(&Ehdr, file, sizeof(Ehdr));
  if (!Ehdr.e_shnum || !Ehdr.e_shoff || Ehdr.e_version == EV_NONE ||
      filesize < Ehdr.e_shoff + (sizeof(*Shdrt) * Ehdr.e_shnum) ||
      Ehdr.e_shentsize != sizeof(*Shdrt))
    return EXIT_FAILURE;

  /* print_Ehdr(&Ehdr); */
  Shdrt = (Elf64_Shdr *)(file + Ehdr.e_shoff);

  for (int i = 0; i < Ehdr.e_shnum; i++) {
    if (Shdrt[i].sh_type == SHT_SYMTAB) symtab = i;
    /* print_Shdr(&(Shdrt[i]), i); */
  }
  if (symtab)
    handle_symtab(file, &Ehdr, Shdrt, symtab, filesize);
  else
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
