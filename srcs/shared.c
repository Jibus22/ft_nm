#include "ft_nm.h"

int is_special_section_indice(uint16_t section_index) {
  return (section_index == SHN_LOPROC || section_index == SHN_BEFORE ||
          section_index == SHN_AFTER || section_index == SHN_HIPROC ||
          section_index == SHN_LOOS || section_index == SHN_HIOS ||
          section_index == SHN_ABS || section_index == SHN_COMMON ||
          section_index == SHN_XINDEX || section_index == SHN_HIRESERVE);
}

void rbt_to_buf(t_rbt *node, char *output_buf, size_t bufsize) {
  if (node) {
    rbt_to_buf(node->left, output_buf, bufsize);
    ft_strlcat(output_buf, (char *)node->value, bufsize);
    rbt_to_buf(node->right, output_buf, bufsize);
  }
}
