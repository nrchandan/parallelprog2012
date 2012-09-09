/*
 * Author: Chandan Kumar chandan.kumar@students.iiit.ac.in
 * Date: 2012-09-09
 *
 * Assignment 1.3 of Parallel Programming course during 
 * Monsoon 2012 semester offered by Suresh Purini.
 * 
 * Prefix Sum Problem.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define LENGTH 32

int *allocateList(int length);
int *generateList(int length);
void prefixSum(int *list, int length);
void displaylist(int *list, int length);

int main (int argc, char *argv[])
{
    int *list;
    int length=LENGTH;

    list = generateList(length);

    printf("Original list\n");
    displaylist(list, length);

    prefixSum(list, length);
    
    printf("Prefix sum list\n");
    displaylist(list, length);
}

int *generateList(int n)
{
    int *list, i;
    list=allocateList(n);
    for (i=0; i<n; i++) {
        list[i] = i;
    }
    return list;
}

int *allocateList(int n)
{
    int *list;
    list = (int *)malloc(sizeof(int)*n);
    if (list==NULL) {
        printf("Error allocating list.  Exiting...\n");
        exit(1);
    }
    return list;
}

void prefixSum(int *list, int n)
{
    int i;
    for (i=1; i<n; i++) {
        list[i]+=list[i-1];
    }
}

void displaylist(int *list, int n)
{
    int i;
    for (i=0; i<n; i++) {
        printf("%3d ", list[i]); 
    }
    printf("\n");
}