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
#include <string.h>
#include <math.h>

#define LENGTH 8
#define ENDRECURSION 7

int *allocateList(int length);
int *generateList(int length, int noinit);
void displaylist(int *list, int length);

void prefixSum(int *list, int length);
void prefixSumSweep(int *list, int length);
void prefixSumCacheObl(int *list, int length);
int upSweep(int *list, int begin, int end);
void downSweep(int *list, int begin, int end, int carry);
int iterativeUpSweep(int *list, int begin, int end);
void simpleUpSweep(int *list, int length);
void simpleDownSweep(int *list, int length);

int main (int argc, char *argv[])
{
    int *list, i, noio=0, noinit=0;
    int length=LENGTH;
    char mode='b';
    char *usage="Usage: prefixSum -(basic|sweep|1tiled|2tiled|cacheob|parallel) [-n <dimension>] [-s1 tilesize] [-s2 tilesize] [-noIO] [-noinit]\n";

    for (i=1; i<argc; i++) {
        if (!strcmp("-n", argv[i])) {
            sscanf(argv[++i], "%d", &length);
        } else if (!strcmp("-noIO", argv[i]) || !strcmp("-noio", argv[i])) {
            noio=1;
        } else if (!strcmp("-noinit", argv[i])) {
            noinit=1;
        } else if (!strcmp("-n", argv[i])) {
            sscanf(argv[++i], "%d", &length);
        } else if (!strcmp("-basic", argv[i])   ||
                   !strcmp("-sweep", argv[i])   ||
                   !strcmp("-1tiled", argv[i])  ||
                   !strcmp("-2tiled", argv[i])  ||
                   !strcmp("-cacheob", argv[i]) ||
                   !strcmp("-parallel", argv[i])) {
            mode=argv[i][1]; //mode = 'b', 's', '1', '2', 'c' or 'p'
        }
    }
    list = generateList(length, noinit);
    

    
    if (!noio) {
        printf("Original list\n");
        displaylist(list, length);
    }
    
    switch (mode) {
        case 'b':
            prefixSum(list, length);
            break;
        case 's':
            prefixSumSweep(list, length);
            break;
        case 'c':
            prefixSumCacheObl(list, length);
            break;
        default:
            printf("Option not recognized.\n");
            printf("%s", usage);
            break;
    }
    
    if (!noio) {
        printf("Prefix sum list\n");
        displaylist(list, length);
    }
}


void prefixSum(int *list, int n)
{
    int i;
    for (i=1; i<n; i++) {
        list[i]+=list[i-1];
    }
}

void prefixSumSweep(int *list, int length)
{
    simpleUpSweep(list, length);
    //    printf("\nAfter upsweep...\n");
    //    displaylist(list, length);
    
    simpleDownSweep(list, length);
    //    printf("\nAfter downsweep...\n");
    //    displaylist(list, length);
}

void prefixSumCacheObl(int *list, int n)
{
    upSweep(list, 0, n-1);
//    displaylist(list, n);
    downSweep(list, 0, n-2, 0); //nth element is already downSweeped.
}

/**
 * Recursive upward sweep of the list
 * example input list: 0 1 2 3 4 5 6 7
 *                       1   5   9   13
 *                           6       22
 *                                   28
 * result of upSweep:  0 1 2 6 4 9 6 28
 * complexity: relatively easy.
 * begin and end are indices of the list to be upsweeped.
 */
int upSweep(int *list, int begin, int end)
{
//    if (end-begin==1) { //i.e. if begin and end are neighbour cells.
//        list[end]+=list[begin];
//        return list[end];
//    }
//    if (end-begin==ENDRECURSION) { //i.e. if begin and end are neighbour cells.
//        return simpleUpSweep(list+begin, end-begin+1);
//    }
    if (end-begin==3) {
        list[begin+1]+=list[begin];
        return (list[end]+=list[begin+1]+list[end-1]);
    }
    list[end]=upSweep(list, begin, (begin+end)/2) + upSweep(list, (begin+end)/2+1, end);
    return list[end];
}


/**
 * Recursive downward sweep of the list.
 * Propagates the summed elements to the rest of the list.
 * 
 * Logic: Break the list into two at mid-point.  Send first half for a down sweep.
 * Send second half for a down sweep, with the mid-point value as carry.
 *
 * complexity: a little complex.
 * begin and end are indices of the list to be downsweeped.
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


void simpleUpSweep(int *list, int length)
{
    //sample length 16 i.e. list (0..15)
    int i, j, stride;
    for (i=1; i<=log2(length); i++) {
        stride = pow(2, i);
        for (j=stride-1; j<length; j+=stride) {
            list[j]+=list[j-stride/2];
        }
    }
}

/*
 * Not fully implemented yet.
 */
void simpleDownSweep(int *list, int length)
{
    int index,j,k;
    int maxpower2position; // e.g. 1, 3, 7, 15, 31, 63, 127 ...
    int newposition; // e.g. 7+4, 7+2, 15+8, 15+4, ...
    
    for (index=1; index<length; index++) {
        if (index%2==0) {
            list[index]+=list[index-1];
        } else {
            // find the max power2 position.
            j=1;
            while(pow(2,j)-1<=index) {    //pow(2, j-1)  
                j++;
            }
            maxpower2position=pow(2,--j)-1; //for index 1, j=2, maxpower2position=1
            if (index==maxpower2position) { //if index is maxpower2position, it is a local maxima, hence no addition required.
                printf("index=%d, skipping...\n", index);
                continue;
            }
            for (k=0; ; k++) {
                newposition=maxpower2position+pow(2, k);
                if (newposition==index) {
                    list[index]+=list[maxpower2position]+list[newposition];
                    break;
                } else if (newposition>index) {
                    break;
                }
            }
        }
    }
}

/* Algorithm not correct
 *
 *
void simpleDownSweep(int *list, int length)
{
    // value at any position is the sum of values at positions obtained 
    // by adding largest power of 2 with next largest power of 2 and so
    // on until position exceeds the current position.
    int index, j=0, k=0, value;
    int maxpower2position; // e.g. 1, 3, 7, 15, 31, 63, 127 ...
    int newposition; // e.g. 7+4, 7+2, 15+8, 15+4, ...
    int tempnewposition;
    // value at position 9, v(9) = v(7) + v(7+2)
    // value at position 13,v(13)= v(7) + v(7+4) + v(7+4+2)
    // if position is a multiple of 2, add value to itself too
    // value at position 18,v(18)= v(15)+ v(15+2) + v(18)
    // value at position 10,v(10)= v(7) + v(7+2) + v(10)
    // value at position 6, v(6) = v(3) + v(3+2) + v(6)

    // assume length = 16 i.e. list[0..15]
    for (index=1; index<length; index++) {
        j=1;
        // find the max power2 position.
        while(pow(2,j)-1<=index) {    //pow(2, j-1)  
            j++;
        }
        maxpower2position=pow(2,--j)-1; //for index 1, j=2, maxpower2position=1
        if (index==maxpower2position) { //if index is maxpower2position, it is a local maxima, hence no addition required.
            printf("index=%d, skipping...\n", index);
            continue;
        }
        value=list[maxpower2position];
        printf("value=%d, ", value);
        newposition=maxpower2position;
        for (k=j-1; k>0 ;k--) {
            tempnewposition=newposition+(int)pow(2, k);
            if (tempnewposition<=index) {
                newposition=tempnewposition;
                value+=list[newposition];
                printf("newposition=%d, value=%d", newposition, value);
            }
            if (tempnewposition==index) {
                break;  // break out of the inner for-loop.
            }
        }
        if (index%2==0) {
            value+=list[index];
        }
        printf(", index=%d, final value=%d\n", index, value);
        list[index]=value;
    }
}
 */

/*
 * Ignore this
 *
int iterativeUpSweep(int *list, int begin, int end)
{
    // sample begin 0, end 7
    // sample begin 8, end 15
    int i, j, temp=0;
    printf("begin=%d, end=%d\n", begin, end);
    for (i=begin+1; i<end; i+=2) {
        //i is always odd.
        for (j=begin+1; j<i; j+=2) {
            //sum all the odd positions
            printf("i=%d, j=%d\n", i, j);
            temp+=list[j];
        }
        list[end]=list[end-1]+temp;
    }
    return list[end];
}
*/

int *generateList(int n, int noinit)
{
    int *list, i;
    list=allocateList(n);
    if (!noinit) {
        for (i=0; i<n; i++) {
            list[i] = i;
        }
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