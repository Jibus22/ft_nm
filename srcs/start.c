#include "ft_nm.h"

static int exit_nm(int exit_status, int fd, void *file, size_t filesize,
                   const char *filename, const char *err_msg) {
  char err_buf[320];

  if (fd > -1) close(fd);
  if (file) munmap(file, filesize);
  if (err_msg) {
    ft_strlcpy(err_buf, "nm: ", sizeof(err_buf));
    if (filename) {
      ft_strlcat(err_buf, "'", sizeof(err_buf));
      ft_strlcat(err_buf, filename, sizeof(err_buf));
      ft_strlcat(err_buf, "': ", sizeof(err_buf));
    }
    ft_strlcat(err_buf, err_msg, sizeof(err_buf));
    write(STDERR_FILENO, err_buf, ft_strlen(err_buf));
  }
  return exit_status;
}

static void output_filename(const char *filename) {
  char buf[320];
  int ret = 0;

  ret += ft_strlcpy(buf, "\n", sizeof(buf));
  ret += ft_strlcpy(buf + ret, filename, sizeof(buf));
  ret += ft_strlcpy(buf + ret, ":\n", sizeof(buf));
  write(STDOUT_FILENO, buf, ret);
}

static int check_elf_ident(char *file, int *arch) {
  *arch = file[EI_CLASS];
  return ((ft_memcmp(file, ELFMAG, SELFMAG) != 0) +
          (file[EI_CLASS] <= 0 || file[EI_CLASS] > 2) +
          (file[EI_DATA] <= 0 || file[EI_DATA] > 2) +
          (file[EI_VERSION] != EV_CURRENT));
}

static int is_arg(const char *s) {
  return ((!ft_strncmp("-a\0", s, 3)) || (!ft_strncmp("-g\0", s, 3)) ||
          (!ft_strncmp("-u\0", s, 3)) || (!ft_strncmp("-r\0", s, 3)) ||
          (!ft_strncmp("-p\0", s, 3)));
}

static int ft_nm(int ac, char *av, int j) {
  int ret, arch, fd = -1;
  struct stat statbuf;
  void *file;

  if (is_arg(av)) return EXIT_SUCCESS;
  if ((fd = open(av, O_RDONLY)) == -1)
    return exit_nm(EXIT_FAILURE, fd, NULL, 0, av, "No such file\n");
  if (fstat(fd, &statbuf) == -1)
    return exit_nm(EXIT_FAILURE, fd, NULL, 0, NULL, NULL);
  if (statbuf.st_size < 16)
    return exit_nm(EXIT_FAILURE, fd, NULL, 0, NULL, NULL);
  if ((file = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
      MAP_FAILED)
    return exit_nm(EXIT_FAILURE, fd, NULL, 0, NULL, NULL);
  if (check_elf_ident(file, &arch) > 0)
    return exit_nm(EXIT_FAILURE, fd, file, statbuf.st_size, av,
                   "file format not recognized\n");
  if (ac - j > 2) output_filename(av);
  if (arch == ELFCLASS32)
    ret = ft_nm_x32(file, statbuf.st_size);
  else
    ret = ft_nm_x64(file, statbuf.st_size);
  return exit_nm(ret, fd, file, statbuf.st_size, NULL, NULL);
}

static int init_args(int ac, char **av) {
  int j = 0;
  e_arg nm_args = 0;
  t_args arg_tuple = {{"-a\0", "-g\0", "-u\0", "-r\0", "-p\0"},
                      {ARG_A, ARG_G, ARG_U, ARG_R, ARG_P}};

  for (int i = 1; i < ac; i++) {
    if (ft_strlen(av[i]) > 2) continue;
    for (int k = 0; k < 5; k++) {
      if (!ft_strncmp(arg_tuple.arg_name[k], av[i], 3)) {
        nm_args |= arg_tuple.arg_flag[k];
        j++;
      }
    }
  }
  getargs(nm_args);
  return j;
}

int main(int ac, char **av) {
  int ret = 0, j;

  j = init_args(ac, av);
  if (ac - j == 1)
    ret += ft_nm(ac, "a.out", j);
  else
    for (int i = 1; i < ac; i++) ret += ft_nm(ac, av[i], j);

  return ret;
}
