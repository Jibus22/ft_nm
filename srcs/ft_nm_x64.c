#include "ft_nm.h"

static int handle_symtab(void *file, Elf64_Ehdr *Ehdr, Elf64_Shdr *Shdrt,
                         int symtab) {
  int j = 0, strtab = Shdrt[symtab].sh_link;
  char *Sstrtab;
  Elf64_Sym *Ssymtab;

  if (sizeof(*Ssymtab) != Shdrt[symtab].sh_entsize ||
      Shdrt[symtab].sh_link >= Ehdr->e_shnum)
    return EXIT_FAILURE;

  Sstrtab = (char *)(file + Shdrt[strtab].sh_offset);
  Ssymtab = file + Shdrt[symtab].sh_offset;

  for (int i = 0; i < Shdrt[symtab].sh_size; i += sizeof(*Ssymtab)) {
    print_sym(Ssymtab, Sstrtab, j);
    j++;
  }

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
  if (symtab) handle_symtab(file, &Ehdr, Shdrt, symtab);
  return EXIT_SUCCESS;
}
