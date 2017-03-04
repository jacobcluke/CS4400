#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <elf.h>

/* Given the in-memory ELF header pointer as `ehdr` and a section
   header pointer as `shdr`, returns a pointer to the memory that
   contains the in-memory content of the section */
#define AT_SEC(ehdr, shdr) ((void *)(ehdr) + (shdr)->sh_offset)

static void check_for_shared_object(Elf64_Ehdr *ehdr);
static void fail(char *reason, int err_code);

static void inspect_so(Elf64_Ehdr *ehdr);
static Elf64_Shdr* section_by_name(Elf64_Ehdr *ehdr, char* name);
static void print_variables_from_function(char* address, Elf64_Ehdr *ehdr);

int main(int argc, char **argv) {
  int fd;
  size_t len;
  void *p;
  Elf64_Ehdr *ehdr;

  if (argc != 2)
    fail("expected one file on the command line", 0);

  /* Open the shared-library file */
  fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    fail("could not open file", errno);

  /* Find out how big the file is: */
  len = lseek(fd, 0, SEEK_END);

  /* Map the whole file into memory: */
  p = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
  if (p == (void*)-1)
    fail("mmap failed", errno);

  /* Since the ELF file starts with an ELF header, the in-memory image
     can be cast to a `Elf64_Ehdr *` to inspect it: */
  ehdr = (Elf64_Ehdr *)p;

  /* Check that we have the right kind of file: */
  check_for_shared_object(ehdr);

  /* Add a call to your work here */
  inspect_so(ehdr);

  return 0;
}

void inspect_so(Elf64_Ehdr *ehdr){

  // Get pointers to the various sections
  Elf64_Shdr* dynsym_shdr = section_by_name(ehdr, ".dynsym");
  Elf64_Sym* syms = AT_SEC(ehdr, dynsym_shdr);
  char* strs = AT_SEC(ehdr, section_by_name(ehdr, ".dynstr"));

  Elf64_Shdr* text_shdr = section_by_name(ehdr, ".text");
  char* code = AT_SEC(ehdr, text_shdr);

  int i, j;
  int size = dynsym_shdr->sh_size / dynsym_shdr->sh_entsize;
  char* address;

  // Output the name of each function provided by a shared library
  for(i = 0; i < size; i++){
    if(ELF64_ST_TYPE(syms[i].st_info) == STT_FUNC && syms[i].st_size != 0){
      printf("%s\n", strs + syms[i].st_name);
      // For each function, print the variables that appear in it
      //    address = code + (syms[i].st_value - text_shdr->sh_addr);
      //  print_variables_from_function(address, ehdr);
    }
  }

}

/*
void print_variables_from_function(char* address, Elf64_Ehdr* ehdr){

  int inFunction = 1;
  int next, tempInt,jump_target;
  char* plt = AT_SEC(ehdr, section_by_name(ehdr, ".plt"));
  while(inFunction){
    next = *address;
    if(next == 0xc3)inFunction = 0;
    if(next == 0xe9)inFunction = 0;
    if(next == 0xeb)inFunction = 0;
    if(next == 0x48){
      address+=3;
    }

    }
  }
  return;
}*/

Elf64_Shdr* section_by_name(Elf64_Ehdr* ehdr, char* name){

  unsigned int sh_offset = ehdr->e_shoff;
  Elf64_Shdr* shdrs = (void*)ehdr + sh_offset;
  unsigned long shstrtab_offset = shdrs[ehdr->e_shstrndx].sh_offset;
  char* strings = (void*)ehdr + shstrtab_offset;

  int i;
  for(i = 0; i < ehdr->e_shnum; i++){
    if(strcmp(strings + shdrs[i].sh_name, name) == 0){
      return &shdrs[i];
    }
  }
  return NULL;
}
static void check_for_shared_object(Elf64_Ehdr *ehdr) {
  if ((ehdr->e_ident[EI_MAG0] != ELFMAG0)
      || (ehdr->e_ident[EI_MAG1] != ELFMAG1)
      || (ehdr->e_ident[EI_MAG2] != ELFMAG2)
      || (ehdr->e_ident[EI_MAG3] != ELFMAG3))
    fail("not an ELF file", 0);

  if (ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    fail("not a 64-bit ELF file", 0);
  
  if (ehdr->e_type != ET_DYN)
    fail("not a shared-object file", 0);
}

static void fail(char *reason, int err_code) {
  fprintf(stderr, "%s (%d)\n", reason, err_code);
  exit(1);
}
