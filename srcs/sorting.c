#include "ft_nm.h"

static int skip_address(const char *s) {
  int i = 0;

  while (ft_isalnum(s[i]) && s[i] != '\n') i++;
  while (s[i] == ' ' && s[i] != '\n') i++;
  i += 2;
  return i;
}

static int strcmp_nocase(const char *s1, const char *s2) {
  char c1 = 0, c2 = 0;
  int a = skip_address(s1), b = skip_address(s2), i = a, j = b, ret;

  while (s1[i] && s2[j] && (c1 == c2)) {
    while (!ft_isalnum(s1[i]) && s1[i] != '\n') i++;
    while (!ft_isalnum(s2[j]) && s2[j] != '\n') j++;
    c1 = s1[i];
    c2 = s2[j];
    if (ft_isalpha(c1) && ft_isalpha(c2)) {
      c1 = (c1 * (c1 < 97)) + ((c1 & 0x5f) * (c1 >= 97));
      c2 = (c2 * (c2 < 97)) + ((c2 & 0x5f) * (c2 >= 97));
    }
    i++;
    j++;
  }
  if (!s1[i] && !s2[j]) {
    ret = (((ft_islower(s1[a - 2])) * (ft_isupper(s2[b - 2]))) ||
           (s2[b - 2] == 'D' && s1[a - 2] == 'W') ||
           (s2[b - 2] == 'W' && s1[a - 2] == 'T'));
    return (ret > 0 ? ret : -1);
  }
  return c1 - c2;
}

int strcmp_nocase_asc(const void *s1, const void *s2) {
  return strcmp_nocase((char *)s1, (char *)s2);
}

int strcmp_nocase_desc(const void *s1, const void *s2) {
  return strcmp_nocase((char *)s2, (char *)s1);
}
