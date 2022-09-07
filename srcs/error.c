#include "ft_nm.h"

int error_wrap(unsigned int err, t_rbt *root, char *output_buf) {
  if (root) destroy_rbt(root);
  if (output_buf) free(output_buf);
  return oops_error(err);
}

int oops_error(unsigned int err) {
  static struct {
    int err_nb;
    char oops_msg[OOPS_SIZE];
  } oops[OOPS_NB] = {
      {OOPS_UND, "undefined err\n\0"},
      {OOPS_NOTELF, "file isn't ELF\n\0"},
      {OOPS_NOCAVE, "no cave found\n\0"},
      {OOPS_BAD_ARG_NB, "wrong number of arguments\n\0"},
      {OOPS_BAD_ELF, "elf corrupted\n\0"},
      {OOPS_NO_LOAD, "no LOAD segment found\n\0"},
      {OOPS_BAD_PHDR, "program header corrupted\n\0"},
      {OOPS_BAD_SHDR, "section header corrupted\n\0"},
      {OOPS_BAD_SYMTAB, "symtab unreachable or corrupted\n\0"},
      {OOPS_BAD_STRTAB, "strtab unreachable or corrupted\n\0"},
  };

  if (err > OOPS_NB) err = OOPS_UND;
  write(STDERR_FILENO, "nm error: ", 10);
  write(STDERR_FILENO, oops[err].oops_msg, ft_strlen(oops[err].oops_msg));
  return EXIT_FAILURE;
}
