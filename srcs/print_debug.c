#include "ft_nm.h"

void print_Ehdr(Elf64_Ehdr *Ehdr) {
    printf(
        "e_version: %d, e_type: %d, e_machine: %d, e_shoff: %lu, e_shentsize: "
        "%d, e_shnum: %d, e_entry: %lu, e_shstrndx: %d\n",
        Ehdr->e_version, Ehdr->e_type, Ehdr->e_machine, Ehdr->e_shoff,
        Ehdr->e_shentsize, Ehdr->e_shnum, Ehdr->e_entry, Ehdr->e_shstrndx);
}

void print_Shdr(Elf64_Shdr *Shdr, int i) {
    printf(
        " [%d] - sh_name: %d, sh_type: %d, sh_addr: %lu, sh_offset: %lu, "
        "sh_size: %lu, sh_entsize: %lu, sh_link: %d\n",
        i, Shdr->sh_name, Shdr->sh_type, Shdr->sh_addr, Shdr->sh_offset,
        Shdr->sh_size, Shdr->sh_entsize, Shdr->sh_link);
}

void print_sym(Elf64_Sym *Ssymtab, char *Sstrtab, int j) {
    printf("  [%2d] - Type: %d, Bind: %d, Visibility: %d -- ", j,
           ELF64_ST_TYPE(Ssymtab[j].st_info), ELF64_ST_BIND(Ssymtab[j].st_info),
           ELF64_ST_VISIBILITY(Ssymtab[j].st_other));
    printf("st_shndx: %5d, st_value: %7lu, st_size: %4lu, name: %s\n",
           Ssymtab[j].st_shndx, Ssymtab[j].st_value, Ssymtab[j].st_size,
           &(Sstrtab[Ssymtab[j].st_name]));
}
