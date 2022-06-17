#include "ft_nm.h"

static int buffer_nm(const Elf32_Ehdr *Ehdr, const Elf32_Shdr *Shdrt,
                     const char *shstrtab, const Elf32_Sym *Ssymtab,
                     const char *Sstrtab, int symb_nb,
                     const unsigned int *smax) {
  char **symbol = (char **)malloc(symb_nb * sizeof(char *));
  char *output_buf = (char *)malloc(symb_nb * SYMBUFSIZE * sizeof(char));
  unsigned int ret = 0, j = 0, shstrtabndx;

  if (!symbol || !output_buf) return EXIT_FAILURE;
  ft_bzero(output_buf, symb_nb * SYMBUFSIZE);
  for (int i = 0; i < symb_nb; i++) {
    symbol[i] = (char *)malloc(SYMBUFSIZE);
    if (!symbol[i]) return EXIT_FAILURE;
    ft_bzero(symbol[i], SYMBUFSIZE);
  }

  /* starts from 1 bc 1st symbol is null */
  for (int i = 1; i < symb_nb; i++) {
    if (Ssymtab[i].st_shndx > Ehdr->e_shnum &&
        !is_special_section_indice(Ssymtab[i].st_shndx))
      return EXIT_FAILURE;
    if (is_special_section_indice(Ssymtab[i].st_shndx) ||
        ELF32_ST_TYPE(Ssymtab[i].st_info) == STT_SECTION)
      continue;
    shstrtabndx = Shdrt[Ssymtab[i].st_shndx].sh_name;
    if (shstrtabndx > smax[SHSTRTAB] || Ssymtab[i].st_name > smax[STRTAB])
      return EXIT_FAILURE;
    format_output(symbol[j], Ssymtab[i].st_value, &Sstrtab[Ssymtab[i].st_name],
                  Ssymtab[i].st_info, Ssymtab[i].st_shndx,
                  &shstrtab[shstrtabndx], 32);
    j++;
  }

  /* here do some sorting */
  asc_sort(symbol, j, 32);
  for (unsigned int i = 0; i < j; i++)
    ret += ft_strlcpy(output_buf + ret, symbol[i], SYMBUFSIZE);
  write(STDOUT_FILENO, output_buf, ret);

  destroy_buffer(symbol, output_buf, symb_nb);
  return EXIT_SUCCESS;
}

static int handle_symtab(const void *file, const Elf32_Ehdr *Ehdr,
                         const Elf32_Shdr *Shdrt, int symtab, size_t filesize) {
  unsigned int symb_nb = 0, strtab = Shdrt[symtab].sh_link, smax[2];
  char *Sstrtab, *Sshstrtab;
  Elf32_Sym *Ssymtab;

  if (sizeof(*Ssymtab) != Shdrt[symtab].sh_entsize ||
      Shdrt[symtab].sh_link >= Ehdr->e_shnum ||
      Ehdr->e_shstrndx >= Ehdr->e_shnum ||
      filesize < Shdrt[strtab].sh_offset + Shdrt[strtab].sh_size ||
      filesize < Shdrt[symtab].sh_offset + Shdrt[symtab].sh_size ||
      filesize <
          Shdrt[Ehdr->e_shstrndx].sh_offset + Shdrt[Ehdr->e_shstrndx].sh_size)
    return EXIT_FAILURE;

  Sstrtab = (char *)(file + Shdrt[strtab].sh_offset);
  Sshstrtab = (char *)(file + Shdrt[Ehdr->e_shstrndx].sh_offset);
  Ssymtab = (Elf32_Sym *)(file + Shdrt[symtab].sh_offset);
  smax[STRTAB] = Shdrt[strtab].sh_size;
  smax[SHSTRTAB] = Shdrt[Ehdr->e_shstrndx].sh_size;

  for (unsigned long i = 0; i < Shdrt[symtab].sh_size; i += sizeof(*Ssymtab)) {
    /* print_sym(Ssymtab, Sstrtab, symb_nb); */
    symb_nb++;
  }

  return buffer_nm(Ehdr, Shdrt, Sshstrtab, Ssymtab, Sstrtab, symb_nb, smax);
  return EXIT_SUCCESS;
}

int ft_nm_x32(const void *file, size_t filesize) {
  Elf32_Ehdr Ehdr;
  Elf32_Shdr *Shdrt;
  int symtab = -1;

  if (filesize < sizeof(Ehdr)) return EXIT_FAILURE;
  ft_memcpy(&Ehdr, file, sizeof(Ehdr));
  if (!Ehdr.e_shnum || !Ehdr.e_shoff || Ehdr.e_version == EV_NONE ||
      filesize < Ehdr.e_shoff + (sizeof(*Shdrt) * Ehdr.e_shnum) ||
      Ehdr.e_shentsize != sizeof(*Shdrt))
    return EXIT_FAILURE;

  /* print_Ehdr(&Ehdr); */
  Shdrt = (Elf32_Shdr *)(file + Ehdr.e_shoff);

  for (int i = 0; i < Ehdr.e_shnum && symtab == -1; i++) {
    if (Shdrt[i].sh_type == SHT_SYMTAB) symtab = i;
    /* print_Shdr(&(Shdrt[i]), i); */
  }
  if (symtab > -1)
    handle_symtab(file, &Ehdr, Shdrt, symtab, filesize);
  else
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
