#ifndef STACK_SIZE_H_
#define STACK_SIZE_H_

#include <cstdlib>

void RunWithStackSize(void (*func)(void), size_t stsize = 1024 * 1024 * 1024) {
  char *stack, *send;
  stack = (char *)malloc(stsize);
  send = stack + stsize - 16;
  send = (char *)((uintptr_t)send / 16 * 16);
  asm volatile(
      "mov %%rsp, (%0)\n"
      "mov %0, %%rsp\n"
      :
      : "r"(send));
  func();
  asm volatile("mov (%0), %%rsp\n" : : "r"(send));
  free(stack);
}

#endif  // STACK_SIZE_H_
