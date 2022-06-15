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

static unsigned char getSymType(const char *Sname, int symbind, int symtype,
                                unsigned long addr) {
  int c = '.';
  s_nmTypename nmtype[NM_TYPE_NB] = {
      {'b', ".bss\0"},
      {'t', ".text\0"},
      {'t', ".init\0"},
      {'t', ".fini\0"},
      {'d', ".data\0"},
      {'d', ".got.plt\0"},
      {'d', ".init_array\0"},
      {'d', ".dynamic\0"},
      {'d', ".fini_array\0"},
      {'r', ".rodata\0"},
      {'r', ".eh_frame\0"},
      {'r', ".eh_frame_hdr\0"},
      {'r', ".gcc_except_table\0"},
  };

  if (symbind == STB_WEAK)
    return (('W' * (addr != 0) + 'w' * (!addr)) *
            (symtype == STT_FUNC || symtype == STT_NOTYPE)) +
           (('V' * (addr != 0) + 'v' * (!addr)) * (symtype == STT_OBJECT));
  if (!addr || !*Sname)
    c = 'u';
  else
    for (int i = 0; i < NM_TYPE_NB; i++) {
      if (!ft_strncmp(Sname, nmtype[i].sectionName,
                      ft_strlen(nmtype[i].sectionName) + 1)) {
        c = nmtype[i].nmType;
        break;
      }
    }
  return ((c * (symbind == STB_LOCAL)) + ((c & '_') * (symbind == STB_GLOBAL)));
}

void format_output(char *buf, unsigned long addr, const char *symname,
                   unsigned char st_info, uint16_t st_shndx,
                   const char *sectionName) {
  char symtype = getSymType(sectionName, ELF64_ST_BIND(st_info),
                            ELF64_ST_TYPE(st_info), addr);

  if (addr && st_shndx != SHN_UNDEF) {
    ft_strlcpy(buf, "0000000000000000   ", SYMBUFSIZE);
    concat_address(buf, addr);
  } else
    ft_strlcpy(buf, "                   ", SYMBUFSIZE);
  buf[17] = symtype;
  ft_strlcat(buf, symname, SYMBUFSIZE);
  ft_strlcat(buf, "\n", SYMBUFSIZE);
}
