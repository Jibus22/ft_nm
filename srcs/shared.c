#include "ft_nm.h"

int is_special_section_indice(uint16_t section_index) {
  return (section_index == SHN_LOPROC || section_index == SHN_BEFORE ||
          section_index == SHN_AFTER || section_index == SHN_HIPROC ||
          section_index == SHN_LOOS || section_index == SHN_HIOS ||
          section_index == SHN_ABS || section_index == SHN_COMMON ||
          section_index == SHN_XINDEX || section_index == SHN_HIRESERVE);
}

void destroy_buffer(char **symbol, char *output_buf, int symb_nb) {
  for (int i = 0; i < symb_nb; i++) free(symbol[i]);
  free(symbol);
  free(output_buf);
}
