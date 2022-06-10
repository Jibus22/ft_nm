#include "ft_nm.h"

int ft_nm_x32(void *file, size_t filesize) {
  Elf32_Ehdr hdr_32;

  if (filesize < sizeof(hdr_32)) return EXIT_FAILURE;
  ft_memcpy(&hdr_32, file, sizeof(hdr_32));
  return EXIT_SUCCESS;
}

