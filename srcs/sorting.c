#include "ft_nm.h"

static int strcmp_nocase(const char *s1, const char *s2) {
  char c1 = 0, c2 = 0;
  int j = 0, i = 0;

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
  if (!s1[i] && !s2[j])
    return (((ft_islower(*(s1 - 2))) * (ft_isupper(*(s2 - 2)))) ||
            (*(s2 - 2) == 'D' && *(s1 - 2) == 'W') ||
            *(s2 - 2) == 'W' && *(s1 - 2) == 'T');
  return c1 - c2;
}

void asc_sort(char **symbol, int size) {
  char *tmp;
  int i = 0, j = 0;

  if (size < 2) return;
  while (i + 1 < size) {
    while (i >= 0 && strcmp_nocase(symbol[i] + 19, symbol[i + 1] + 19) > 0) {
      tmp = (char *)symbol[i];
      symbol[i] = symbol[i + 1];
      symbol[i + 1] = tmp;
      i--;
    }
    i++;
  }
}
