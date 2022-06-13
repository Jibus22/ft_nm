#include "ft_nm.h"

void concat_address(char *dst, unsigned long n) {
  unsigned long e;
  short int res;
  int i = 1, shift;

  for (e = n / 16; e; i++) e /= 16;
  shift = 16 - i;
  while (i--) {
    res = ((n / ft_pow(16, e++)) % 16);
    res = ((res + 48) * (res < 10)) + ((res + 87) * (res >= 10));
    dst[i + shift] = res;
  }
}

static unsigned char getSymType(const char *Sname, int symbind,
                                unsigned long addr) {
  int c = '.';
  s_nmTypename symtype[NM_TYPE_NB] = {
      {'b', ".bss\0"},          {'t', ".text\0"},    {'t', ".init\0"},
      {'t', ".fini\0"},         {'d', ".data\0"},    {'d', ".got.plt\0"},
      {'d', ".init_array\0"},   {'r', ".rodata\0"},  {'r', ".eh_frame\0"},
      {'r', ".eh_frame_hdr\0"}, {'d', ".dynamic\0"}, {'d', ".fini_array\0"},

  };

  if (symbind == STB_WEAK) return 'W' * (addr != 0) + 'w' * (!addr);
  if (!addr)
    c = 'u';
  else
    for (int i = 0; i < NM_TYPE_NB; i++)
      if (!ft_strncmp(Sname, symtype[i].sectionName, ft_strlen(Sname)))
        c = symtype[i].nmType;
  return ((c * (symbind == STB_LOCAL)) + ((c & '_') * (symbind == STB_GLOBAL)));
}

void format_buffer(char *buf, unsigned long addr, const char *symname,
                   int symbind, const char *sectionName) {
  char symtype = getSymType(sectionName, symbind, addr);

  if (addr) {
    ft_strlcpy(buf, "0000000000000000   ", SYMBUFSIZE);
    concat_address(buf, addr);
  } else
    ft_strlcpy(buf, "                   ", SYMBUFSIZE);
  buf[17] = symtype;
  ft_strlcat(buf, symname, SYMBUFSIZE);
  ft_strlcat(buf, "\n", SYMBUFSIZE);
}
