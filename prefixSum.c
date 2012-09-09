/*
 * Author: Chandan Kumar chandan.kumar@students.iiit.ac.in
 * Date: 2012-09-09
 *
 * Assignment 1.3 of Parallel Programming course during 
 * Monsoon 2012 semester offered by Suresh Purini.
 * 
 * Prefix Sum Problem.
 * Input list assumed to be power of 2.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define LENGTH 32

int *allocateList(int length);
int *generateList(int length);
void displaylist(int *list, int length);

void prefixSum(int *list, int length);
void prefixSumSweep(int *list, int length);
int upSweep(int *list, int begin, int end);
void downSweep(int *list, int begin, int end, int carry);

int main (int argc, char *argv[])
{
    int *list;
    int length=LENGTH;

    list = generateList(length);

    printf("Original list\n");
    displaylist(list, length);

//    prefixSum(list, length);
    prefixSumSweep(list, length);
    
    printf("Prefix sum list\n");
    displaylist(list, length);
}


void prefixSum(int *list, int n)
{
    int i;
    for (i=1; i<n; i++) {
        list[i]+=list[i-1];
    }
}

void prefixSumSweep(int *list, int n)
{
    upSweep(list, 0, n-1);
    displaylist(list, n);
    downSweep(list, 0, n-2, 0); //nth element is already downSweeped.
}

/**
 * Upward sweep of the list
 * example input list: 0 1 2 3 4 5 6 7
 *                       1   5   9   13
 *                           6       22
 *                                   28
 * result of upSweep:  0 1 2 6 4 9 6 28
 * complexity: relatively easy.
 */
int upSweep(int *list, int begin, int end)
{
    if (end-begin==1) { //i.e. if begin and end are neighbour cells.
        list[end]+=list[begin];
        return list[end];
    }
    list[end]=upSweep(list, begin, (begin+end)/2) + upSweep(list, (begin+end)/2+1, end);
    return list[end];
}

/**
 * Downward sweep of the list.
 * Propagates the summed elements to the rest of the list.
 * 
 * Logic: Break the list into two at mid-point.  Send first half for a down sweep.
 * Send second half for a down sweep, with the mid-point value as carry.
 *
 * complexity: a little complex.
 */
void downSweep(int *list, int begin, int end, int carry)
{
    if (end-begin==2) {  //this condition will be met if the list dimension is a power of 2.  Untested for the other cases.
        list[begin]+=carry;
        list[end]=list[end]+list[begin+1]+carry;
        list[begin+1]+=carry;
        return;
    }
    list[(begin+end)/2]+=carry;
    downSweep(list, begin, (begin+end)/2-1, carry);
    downSweep(list, (begin+end)/2+1, end, list[(begin+end)/2]);
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

void displaylist(int *list, int n)
{
    int i;
    for (i=0; i<n; i++) {
        printf("%3d ", list[i]); 
    }
    printf("\n");
}