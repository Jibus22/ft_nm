#include "ft_nm.h"

int is_special_section_indice(uint16_t section_index) {
  return (section_index == SHN_LOPROC || section_index == SHN_BEFORE ||
          section_index == SHN_AFTER || section_index == SHN_HIPROC ||
          section_index == SHN_LOOS || section_index == SHN_HIOS ||
          section_index == SHN_ABS || section_index == SHN_COMMON ||
          section_index == SHN_XINDEX || section_index == SHN_HIRESERVE);
}

unsigned int rbt_to_buf(t_rbt *node, char *output_buf, unsigned int ret) {
  unsigned int ret2;

  if (!node) return 0;

  ret2 = rbt_to_buf(node->left, output_buf, ret);
  if (ret2) ret = ret2;
  ret += ft_strlcpy(output_buf + ret, (char *)node->value, SYMBUFSIZE);
  ret2 = rbt_to_buf(node->right, output_buf, ret);
  if (ret2) ret = ret2;
  return ret;
}

bool getargs(e_arg env) {
  static int env_cache;
  static bool init;

  if (!init) {
    if (env & ARG_A) env_cache |= ARG_A;
    if (env & ARG_G) env_cache |= ARG_G;
    if (env & ARG_U) env_cache |= ARG_U;
    if (env & ARG_R) env_cache |= ARG_R;
    if (env & ARG_P) env_cache |= ARG_P;
    init = true;
  }
  return (env & env_cache);
}
