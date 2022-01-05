#include <stdio.h>
#include <unistd.h>

int main() {

int i = 10;
char string[4] = "TEST";

  while (i < 100) {
    printf("[%s - %p]\n", string, &string);
    sleep(1);
  }

  return 0;
}