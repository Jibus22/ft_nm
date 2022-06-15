#include "ft_nm.h"

static int buffer_nm(const Elf64_Shdr *Shdrt, const char *shstrtab,
                     const Elf64_Sym *Ssymtab, const char *Sstrtab,
                     int symb_nb) {
  char **symbol = (char **)malloc(symb_nb * sizeof(char *));
  char *output_buf = (char *)malloc(symb_nb * SYMBUFSIZE * sizeof(char));
  int ret = 0, j = 0, shstrtabndx;

  if (!symbol || !output_buf) return EXIT_FAILURE;
  ft_bzero(output_buf, symb_nb * SYMBUFSIZE);
  for (int i = 0; i < symb_nb; i++) {
    symbol[i] = (char *)malloc(SYMBUFSIZE);
    if (!symbol[i]) return EXIT_FAILURE;
    ft_bzero(symbol[i], SYMBUFSIZE);
  }

  /* starts from 1 bc 1st symbol is null */
  for (int i = 1; i < symb_nb; i++) {
    if (Ssymtab[i].st_shndx == SHN_ABS ||
        ELF64_ST_TYPE(Ssymtab[i].st_info) == STT_SECTION)
      continue;
    shstrtabndx = Shdrt[Ssymtab[i].st_shndx].sh_name;
    format_buffer(symbol[j], Ssymtab[i].st_value, &Sstrtab[Ssymtab[i].st_name],
                  Ssymtab[i].st_info, Ssymtab[i].st_shndx,
                  &shstrtab[shstrtabndx]);
    j++;
  }

  /* here do some sorting */
  asc_sort(symbol, j);

  for (int i = 0; i < j; i++)
    ret += ft_strlcpy(output_buf + ret, symbol[i], SYMBUFSIZE);

  write(1, output_buf, ret);

  for (int i = 0; i < symb_nb; i++) free(symbol[i]);
  free(symbol);
  free(output_buf);

  return EXIT_SUCCESS;
}

static int handle_symtab(const void *file, const Elf64_Ehdr *Ehdr,
                         const Elf64_Shdr *Shdrt, int symtab, int filesize) {
  int symb_nb = 0, strtab = Shdrt[symtab].sh_link;
  char *Sstrtab, *Sshstrtab;
  Elf64_Sym *Ssymtab;

  if (sizeof(*Ssymtab) != Shdrt[symtab].sh_entsize ||
      Shdrt[symtab].sh_link >= Ehdr->e_shnum ||
      filesize < Shdrt[strtab].sh_offset + Shdrt[strtab].sh_size ||
      filesize < Shdrt[symtab].sh_offset + Shdrt[symtab].sh_size)
    return EXIT_FAILURE;

  Sstrtab = (char *)(file + Shdrt[strtab].sh_offset);
  Sshstrtab = (char *)(file + Shdrt[Ehdr->e_shstrndx].sh_offset);
  Ssymtab = (Elf64_Sym *)(file + Shdrt[symtab].sh_offset);

  for (int i = 0; i < Shdrt[symtab].sh_size; i += sizeof(*Ssymtab)) {
    /* print_sym(Ssymtab, Sstrtab, symb_nb); */
    symb_nb++;
  }

  buffer_nm(Shdrt, Sshstrtab, Ssymtab, Sstrtab, symb_nb);

  return EXIT_SUCCESS;
}

int ft_nm_x64(const void *file, size_t filesize) {
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
