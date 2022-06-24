## nm

nm (name mangling) is a Unix command used to dump the symbol table and their attributes from a binary executable file (including libraries, compiled object modules, shared-object files, and standalone executables)

## Symbol table

a symbol table is a data structure used by a language translator such as a compiler or interpreter, where each identifier (or symbols), constants, procedures and functions in a program's source code is associated with information relating to its declaration or appearance in the source. In other words, the entries of a symbol table store the information related to the entry's corresponding symbol.

A symbol table may only exist in memory during the translation process, or it may be embedded in the output of the translation, such as in an ABI object file for later use. For example, it might be used during an interactive debugging session, or as a resource for formatting a diagnostic report during or after execution of a program.

## Object file

An object file is a computer file containing object code, that is, machine code output of an assembler or compiler. The object code is usually relocatable, and not usually directly executable. There are various formats for object files, and the same machine code can be packaged in different object file formats. An object file may also work like a shared library.
For example, under Linux, the GNU Compiler Collection compiler will generate files with a .o extension which use the ELF format.

## ELF

In computing, the Executable and Linkable Format (ELF, formerly named Extensible Linking Format), is a common standard file format for executable files, object code, shared libraries, and core dumps.

### ELF layout

![ELF_layout.png](./_resources/ELF_layout.jpg)
_An ELF file has two views: the program header shows the segments used at run time, whereas the section header lists the set of sections._

- elf header can be found here: `/usr/include/elf.h` \*

* File header
  The ELF header defines whether to use 32-bit or 64-bit addresses. The header contains three fields that are affected by this setting and offset other fields that follow them. The ELF header is 52 or 64 bytes long for 32-bit and 64-bit binaries respectively.

```c
typedef struct
{
  unsigned char e_ident[EI_NIDENT];     /* Magic number and other info */
  Elf64_Half    e_type;                 /* Object file type */
  Elf64_Half    e_machine;              /* Architecture */
  Elf64_Word    e_version;              /* Object file version */
  Elf64_Addr    e_entry;                /* Entry point virtual address */
  Elf64_Off     e_phoff;                /* Program header table file offset */
  Elf64_Off     e_shoff;                /* Section header table file offset */
  Elf64_Word    e_flags;                /* Processor-specific flags */
  Elf64_Half    e_ehsize;               /* ELF header size in bytes */
  Elf64_Half    e_phentsize;            /* Program header table entry size */
  Elf64_Half    e_phnum;                /* Program header table entry count */
  Elf64_Half    e_shentsize;            /* Section header table entry size */
  Elf64_Half    e_shnum;                /* Section header table entry count */
  Elf64_Half    e_shstrndx;             /* Section header string table index */
} Elf64_Ehdr;
```

- Program header
  Describing zero or more memory segments.
  The program header table tells the system how to create a process image. It is found at file offset e_phoff, and consists of e_phnum entries, each with size e_phentsize. The layout is slightly different in 32-bit ELF vs 64-bit ELF, because the p_flags are in a different structure location for alignment reasons.

- Section header
  Describing zero or more sections.
  The section header table is an array of Elf32_Shdr or Elf64_Shdr structures. It is found at file offset e_shoff, and consists of e_shnum entries, each with size e_shentsize.

```c
typedef struct
{
  Elf64_Word    sh_name;                /* Section name (string tbl index) */
  Elf64_Word    sh_type;                /* Section type */
  Elf64_Xword   sh_flags;               /* Section flags */
  Elf64_Addr    sh_addr;                /* Section virtual addr at execution */
  Elf64_Off     sh_offset;              /* Section file offset */
  Elf64_Xword   sh_size;                /* Section size in bytes */
  Elf64_Word    sh_link;                /* Link to another section */
  Elf64_Word    sh_info;                /* Additional section information */
  Elf64_Xword   sh_addralign;           /* Section alignment */
  Elf64_Xword   sh_entsize;             /* Entry size if section holds table */
} Elf64_Shdr;
```

The goal here is to roam the section header until finding the header of symbols section: `sh_type == SHT_SYMTAB`.
We also have to keep in memory the header of shstrtab section wich gives us access to the table of sections name (useful to find the printed out nm type), and the header of strtab wich gives us access to the name of symbol names.

The table of symbol headers is reached with `sh_offset` of `Elf64_Shdr[symtab].sh_offset` and give us access to the

- Symbol header table
  Describing 0 or more symbols. It is an array of ElfN_Sym structure. The size of this section is found at `Elf64_Shdr[symtab].sh_size`

```c
typedef struct
{
  Elf64_Word    st_name;                /* Symbol name (string tbl index) */
  unsigned char st_info;                /* Symbol type and binding */
  unsigned char st_other;               /* Symbol visibility */
  Elf64_Section st_shndx;               /* Section index */
  Elf64_Addr    st_value;               /* Symbol value */
  Elf64_Xword   st_size;                /* Symbol size */
} Elf64_Sym;
```

To format `nm` output I needed to use `st_name` `st_info` `st_shndx` `st_value` (and sections name)

An executable can be ran without its section part. It can be tested while striping this part with the command `strip -s file` then running the pgm: It will be loaded anyway. Its size will be however lighter.

nm displays: address, info, name
It can fetch multiple files to process them. If one of them isn't recognized it is displayed to the user and others files are still processed.
An argument -[gupa] can be put everywhere in the cmd line.
If no file is given, nm try to open 'a.out' file.
The output is sorted in alphanum order, case-insensitive and skipping non-alphanum characters. Finally if two symbols have the same name but not the same type/info, a specific choice is made (see my implementation in `strcmp_nocase()` in sorting.c)

### bonus:

- [ ] a: Display all symbols, even debugger-only symbols; normally these are not listed.
- [x] g: Display only external symbols.
- [x] u: Display only undefined symbols (those external to each object file).
- [x] r: Reverse the order of the sort (whether numeric or alphabetic); let the last come first.
- [x] p: Do not bother to sort the symbols in any order; print them in the order encountered.
