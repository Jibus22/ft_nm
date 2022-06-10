#include "ft_nm.h"

static int exit_nm(int exit_status, int fd, void *file, size_t filesize) {
  if (fd > -1) close(fd);
  if (file) munmap(file, filesize);
  return exit_status;
}

static int check_elf_ident(char *file, int *arch) {
  char buf[EI_NIDENT - EI_PAD] = {0};

  *arch = file[EI_CLASS];
  return ((ft_memcmp(file, ELFMAG, SELFMAG) != 0) +
          (!file[EI_CLASS] || file[EI_CLASS] > 2) +
          (!file[EI_DATA] || file[EI_DATA] > 2) +
          (file[EI_VERSION] != EV_CURRENT) +
          (ft_memcmp(file + EI_PAD, buf, EI_NIDENT - EI_PAD) != 0));
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
