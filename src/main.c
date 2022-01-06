#include <mach/mach.h>
#include <mach/mach_error.h>
#include <mach/mach_init.h>
#include <mach/mach_traps.h>
#include <mach/mach_vm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// usage: ./executable (pid) (virtual mem addr) [r/w] [string to write]

int main(int argc, char **argv) {
  /*
   * The access control flags are bitwise OR'd together to create a value
   * which will later be passed to our vm_protect calls.
   * If you need R/W on your memory region,
   * VM_PROT_DEFAULT is equivalent to VM_PROT_READ and VM_PROT_WRITE.
   */

  char *string_to_ow = malloc(2 * (sizeof(char)));
  if (string_to_ow == NULL) {
    printf("The allocation of string_to_ow has failed");
    return 1;
  }

  struct vm_region_t {
    mach_vm_address_t addr;
    size_t size; // R/W Bytes size on target vm
    vm_prot_t prot; 
    pointer_t buf;  // buffer for R option
    uint32_t sz;
  } vm_region_p;

  if (argv[1] == NULL) {
    printf("You have forgot to enter the pid\n");
    return 1;
  } else if (argv[2] == NULL) {
    printf("You have forgot the address\n");
    return 1;

  } else if (argv[3] == NULL) {
    printf("You have forgot to set what you want to do (read or write)\n");
    return 1;
  } else if (argv[4] == NULL && strcmp(argv[3], "w") == 0) {
    printf("you have forgot the string for the overwriting\n");
    return 1;
  }

  // storing I/O
  int action = 0;

  if (strcmp(argv[3], "r") == 0) {
    action = 1;
  } else if (strcmp(argv[3], "w") == 0) {
    action = 2;
  } else {
    printf("action not valid.\n");
    return 1;
  }

  // this is literally an int
  pid_t pid = atoi(argv[1]);
  printf("looking for process with id -> %d\n", pid);

  vm_region_p.addr = strtoull(argv[2], NULL, 0);

  if (action == 2) {
    /* Basically realloc(prt, size) returns a pointer to a new memory block
    that has the size specified by size and DEALLOCATES the block pointed to by
    ptr. If it fails, the original memory block is not DEALLOCATED
    */

    char *tmp_str = realloc(string_to_ow, sizeof(argv[4]) * sizeof(char));

    if (tmp_str == NULL) {
      printf("The re-allocation of array a has failed\n");
      free(string_to_ow);
      return 1;
    } else {
      string_to_ow = tmp_str;
    }

    strcpy(string_to_ow, argv[4]);

    vm_region_p.size = sizeof(string_to_ow);

    printf("Print sizeof(string_to_ow) %zu\n", vm_region_p.size);
  }

  // here we write down the permissions we need to set
  vm_region_p.prot = VM_PROT_READ | VM_PROT_WRITE;

  mach_port_t task;

  kern_return_t kret;

  kret = task_for_pid(mach_task_self(), atoi(argv[1]), &task);

  if (kret != KERN_SUCCESS) {
    printf("task_for_pid() failed with the message: %s !\n",
           mach_error_string(kret));
    return 1;
  } else {
    printf("task_for_pid() successfully called !\n");
  }

  // R/W Bytes size | tmp
  vm_region_p.size = 128;

  /*printf(
      "Allocating new vm region on the same address to execute the "
      "overwriting\n");*/

  // here we allocate a new vm region on the same address of the previous
  /*kret = mach_vm_allocate(task, &vm_region_p.addr, vm_region_p.size,
                          VM_FLAGS_ANYWHERE);
  if (kret != KERN_SUCCESS) {
    printf("mach_vm_allocate() failed with the message: %s !\n",
           mach_error_string(kret));
    return 1;
  }*/

  /* If we don't set access control for our virtual memory region, we won't be
   able to execute it. */

  kret = mach_vm_protect(task, vm_region_p.addr, vm_region_p.size, FALSE,
                         vm_region_p.prot);

  if (kret != KERN_SUCCESS) {
    printf("mach_vm_protect() failed with the message: %s !\n",
           mach_error_string(kret));
    printf("vm_region_p.addr: %llu | %s", vm_region_p.addr, argv[2]);
    return 1;
  }

  if (action == 1) {
    
    /* kern_return_t mach_vm_read
          vm_map_read_t target_task,
          mach_vm_address_t address,
          mach_vm_size_t size, // size to read
          vm_offset_t *data,
          mach_msg_type_number_t *dataCnt
    */

    printf("Reading the string on the memory region\n");

    // http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/vm_read.html
    mach_vm_read(task, vm_region_p.addr, vm_region_p.size, &vm_region_p.buf,
                 &vm_region_p.sz);

    if (kret != KERN_SUCCESS) {
      printf("mach_vm_read() failed with the message: %s !\n",
             mach_error_string(kret));
      return 1;
    } else {
      char r_out[vm_region_p.size];
      memcpy(r_out, (const void *)vm_region_p.buf, vm_region_p.size);
      printf("%s\nbuffer:%s\nsize:%d\n", r_out, vm_region_p.buf,
             vm_region_p.sz);
    }
  } else if (action == 2) {
    printf("Writing the string on the memory region\n");
    
    /* here we write the string
      test.c print a string stored in a char with a size of 4 bytes
      so changing the string with a 5 (or more) bytes one, will cause a segfault 
      */
    kret =
        mach_vm_write(task, vm_region_p.addr, string_to_ow, vm_region_p.size);

    if (kret != KERN_SUCCESS) {
      printf("mach_vm_write() failed with the message: %s !\n",
             mach_error_string(kret));
      return 1;
    }
  }
  return 0;
}
