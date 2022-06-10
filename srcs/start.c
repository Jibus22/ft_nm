#include "ft_nm.h"

void print_Ehdr(Elf64_Ehdr *Ehdr) {
  printf(
      "e_version: %d, e_type: %d, e_machine: %d, e_shoff: %lu, e_shentsize: "
      "%d, e_shnum: %d, e_entry: %lu, e_shstrndx: %d\n",
      Ehdr->e_version, Ehdr->e_type, Ehdr->e_machine, Ehdr->e_shoff,
      Ehdr->e_shentsize, Ehdr->e_shnum, Ehdr->e_entry, Ehdr->e_shstrndx);
}

void print_Shdr(Elf64_Shdr *Shdr, int i) {
  printf(
      " [%d] - sh_name: %d, sh_type: %d, sh_addr: %lu, sh_offset: %lu, "
      "sh_size: %lu, sh_entsize: %lu, sh_link: %d\n",
      i, Shdr->sh_name, Shdr->sh_type, Shdr->sh_addr, Shdr->sh_offset,
      Shdr->sh_size, Shdr->sh_entsize, Shdr->sh_link);
}

int exit_nm(int exit_status, int fd, void *file, size_t filesize) {
  if (fd > -1) close(fd);
  if (file) munmap(file, filesize);
  return exit_status;
}

int check_elf_ident(char *file, int *arch) {
  char buf[EI_NIDENT - EI_PAD] = {0};

  *arch = file[EI_CLASS];
  return ((ft_memcmp(file, ELFMAG, SELFMAG) != 0) +
          (!file[EI_CLASS] || file[EI_CLASS] > 2) +
          (!file[EI_DATA] || file[EI_DATA] > 2) +
          (file[EI_VERSION] != EV_CURRENT) +
          (ft_memcmp(file + EI_PAD, buf, EI_NIDENT - EI_PAD) != 0));
}

int ft_nm_x32(void *file, size_t filesize) {
  Elf32_Ehdr hdr_32;

  if (filesize < sizeof(hdr_32)) return EXIT_FAILURE;
  ft_memcpy(&hdr_32, file, sizeof(hdr_32));
  return EXIT_SUCCESS;
}

void print_sym(Elf64_Sym *Ssymtab, char *Sstrtab, int j) {
  printf("  [%2d] - Type: %d, Bind: %d, Visibility: %d -- ", j,
         ELF64_ST_TYPE(Ssymtab[j].st_info), ELF64_ST_BIND(Ssymtab[j].st_info),
         ELF64_ST_VISIBILITY(Ssymtab[j].st_other));
  printf("st_shndx: %5d, st_value: %7lu, st_size: %4lu, name: %s\n",
         Ssymtab[j].st_shndx, Ssymtab[j].st_value, Ssymtab[j].st_size,
         &(Sstrtab[Ssymtab[j].st_name]));
}

int handle_symtab(void *file, Elf64_Ehdr *Ehdr, Elf64_Shdr *Shdrt, int symtab) {
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

  if (filesize < sizeof(Ehdr)) return EXIT_FAILURE;
  ft_memcpy(&Ehdr, file, sizeof(Ehdr));
  if (!Ehdr.e_shnum || !Ehdr.e_shoff || Ehdr.e_version == EV_NONE ||
      filesize < Ehdr.e_shoff + (sizeof(*Shdrt) * Ehdr.e_shnum) ||
      Ehdr.e_shentsize != sizeof(*Shdrt))
    return EXIT_FAILURE;

  print_Ehdr(&Ehdr);
  Shdrt = (Elf64_Shdr *)(file + Ehdr.e_shoff);

  for (int i = 0; i < Ehdr.e_shnum; i++) {
    if (Shdrt[i].sh_type == SHT_SYMTAB) {
      write(1, "YEAH SYMTAB HERE\n", 17);
      handle_symtab(file, &Ehdr, Shdrt, i);
    }
    print_Shdr(&(Shdrt[i]), i);
  }
  return EXIT_SUCCESS;
}

int main(int ac, char **av) {
  int fd = -1, arch, ret;
  struct stat statbuf;
  void *file;

  if (ac == 1) return exit_nm(EXIT_FAILURE, fd, NULL, 0);
  if ((fd = open(av[1], O_RDONLY)) == -1)
    return exit_nm(EXIT_FAILURE, fd, NULL, 0);
  if (fstat(fd, &statbuf) == -1) return exit_nm(EXIT_FAILURE, fd, NULL, 0);
  if (statbuf.st_size < 16) return exit_nm(EXIT_FAILURE, fd, NULL, 0);
  if ((file = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
      MAP_FAILED)
    return exit_nm(EXIT_FAILURE, fd, NULL, 0);
  if (check_elf_ident(file, &arch) > 0)
    return exit_nm(EXIT_FAILURE, fd, file, statbuf.st_size);
  if (arch == ELFCLASS32)
    ret = ft_nm_x32(file, statbuf.st_size);
  else
    ret = ft_nm_x64(file, statbuf.st_size);
  return exit_nm(ret, fd, file, statbuf.st_size);
}
