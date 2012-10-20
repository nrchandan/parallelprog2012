/**
 * Basic program to understand the syntax of atomic CAS method.
 * CAS = Compare and Swap.
 *
 * Author: chandan.kumar@students.iiit.ac.in
 * Date: 2012-10-08
 *
 */


#include <stdio.h>

int main (int argc, char *argv[])
{
  int *ptr;
  int a=1, b=2;
  ptr = &a;

  printf("ptr's value is %d\n", *ptr);
  //__sync_bool_compare_and_swap (&ptr, &b, &a);
  __sync_bool_compare_and_swap (&ptr, &a, &b);
  printf("ptr's value is now %d\n", *ptr);
}
