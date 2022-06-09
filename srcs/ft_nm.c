#include "ft_nm.h"

void print_Ehdr(Elf64_Ehdr *Ehdr) {
  printf(
      "e_version: %d, e_type: %d, e_machine: %d, e_shoff: %lu, e_shentsize: "
      "%d, e_shnum: %d\n",
      Ehdr->e_version, Ehdr->e_type, Ehdr->e_machine, Ehdr->e_shoff,
      Ehdr->e_shentsize, Ehdr->e_shnum);
}

void print_Shdr(Elf64_Shdr *Shdr) {
  printf("sh_type: %d, sh_addr: %lu, sh_offset: %lu, sh_size: %lu\n",
         Shdr->sh_type, Shdr->sh_addr, Shdr->sh_offset, Shdr->sh_size);
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

int ft_nm_x64(void *file, size_t filesize) {
  Elf64_Ehdr Ehdr;
  Elf64_Shdr *Shdr;

  if (filesize < sizeof(Ehdr)) return EXIT_FAILURE;
  ft_memcpy(&Ehdr, file, sizeof(Ehdr));
  if (filesize < Ehdr.e_shoff + sizeof(Shdr)) return EXIT_FAILURE;
  print_Ehdr(&Ehdr);
  /* ft_memcpy(&Shdr, file + Ehdr.e_shoff, sizeof(Shdr)); */
  for (int i = 0; i < Ehdr.e_shnum; i++) {
    Shdr = (Elf64_Shdr *)(file + Ehdr.e_shoff + (Ehdr.e_shentsize * i));
    print_Shdr(Shdr);
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
