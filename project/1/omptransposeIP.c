/*
 * Author: Chandan Kumar chandan.kumar@students.iiit.ac.in
 * Date: 2012-09-14
 *
 * Assignment 1.1 of Parallel Programming course during 
 * Monsoon 2012 semester offered by Suresh Purini.
 * 
 * In-place matrix transpose.
 * Input size assumed to be power of 2. 
 * Matrix assumed to be square i.e n x n.
 *
 * Compile command: gcc -fopenmp omptransposeIP.c
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>

#define LENGTH 16384 

int* allocateMatrix(int length);
int* generateMatrix(int length, int noinit);
int* loadMatrix(char *infile, int length);

// matrix can be a sub-matrix (sxs) of another matrix (nxn).
void transpose(int *matrix, int size_s, int dimension_n);

void printm(int *matrix, int length);
void printmf(int *matrix, int length, const char *filename);

int* M(int *matrix, int dimension, int i, int j);

/**
 * Sample arguments
 *
 * To transpose a randomly generated 64x64 matrix using basic algorithm 
 * (both input and output displayed on screen)
 *              ./transposeIP -basic -n64
 *
 */
int main(int argc, char *argv[])
{
    int* m;
    const char *usage = "Usage: basictransposeIP [[-i <infile>] -n <dimension>] [-o <outfile>] [-noIO] [-noinit]\n";
    char *infile=NULL, *outfile=NULL;
    int dimension=0, tile1size=0, tile2size=0;
    int i, noio=1, noinit=1;
    
/*    
    if (argc <=1 ) {
        printf("%s", usage);
        exit(0);
    }
*/
    //    printf("%d\n", argc);
    for (i=1; i<argc; i++) {
        //        printf("%s", argv[i]);
        if (!strcmp("-i", argv[i])) {
            infile = argv[++i];
        } else if (!strcmp("-o", argv[i])) {
            outfile = argv[++i];
        } else if (!strcmp("-n", argv[i])) {
            sscanf(argv[++i], "%d", &dimension);
        } else if (!strcmp("-noIO", argv[i]) || !strcmp("-noio", argv[i])) {
            noio=1;
        } else if (!strcmp("-noinit", argv[i])) {
            noinit=1;
        }
    }
    
    if (infile == NULL) {
        // no input file provided
        if (dimension==0) {
            dimension = LENGTH;
        }
        m = generateMatrix(dimension, noinit);
    } else {
        if (dimension<=0) {
            printf("Please provide a valid dimension of the input matrix.\n");
            printf("%s", usage);
            exit(0);
        }
        m = loadMatrix(infile, dimension);
    }

    if (!noio) {
        printf("\nBefore Transpose: \n");
        printm(m, dimension);
    }
    
    printf("Starting transpose...\n");
    transpose(m, dimension, dimension);
    printf("Transpose complete.\n");
    if (!noio) {
        printf("\nAfter Transpose: \n");
        if (outfile==NULL) {
            printm(m, dimension);
        } else {
            printmf(m, dimension, outfile);
        }
    }
    free(m);
    return 1;
}

/**
 * m is sub-matrix/ tile (length s) of another matrix (length n).
 * For basic transpose, s equals n.
 *
 */
void transpose(int *m, int s, int n) 
{
    int i, j, temp;

    #pragma omp parallel default (none) shared (m,n,s,i) private (j,temp)  
    #pragma omp for schedule (runtime) 
    for (i=0; i<s; i++) {
        for (j=i+1; j<s; j++) {
            temp = *M(m,n,i,j);
            *M(m,n,i,j)=*M(m,n,j,i);
            *M(m,n,j,i)=temp;
        } 
    }
}


int* M(int*m, int N, int i, int j)
{
//    printf("cell number is %d ", i*N+j);
    return m+i*N+j;
}


/**
 * Print matrix in stdout.
 *
 */
void printm(int *m, int n)
{
    int i, j;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
//            printf("%d ", m[i][j]);
            printf("%d ", *M(m,n,i,j));
        }
        printf("\n");
    }
}

/**
 * Print matrix in the specified file.
 *
 */
void printmf(int *m, int n, const char *filename)
{
    int i, j;
    
    FILE *fp = fopen(filename, "w");
    if (fp==NULL) {
        printf("Error opening %s for writing\n", filename);
    }
    
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
//            fprintf(fp, "%d ", m[i][j]);
            fprintf(fp, "%d ", *M(m,n,i,j));
        }
        fprintf(fp, "\n");
    }
}

int* generateMatrix(int n, int noinit)
{
    int i, j, *m;
    m = allocateMatrix(n);

    if (!noinit) {
        printf("Generating matrix...\n");
        for (i=0; i<n; i++) {
            for (j=0; j<n; j++) {
                //            m[i][j] = rand()%10;
                //            *M(m,n,i,j) = rand()%10;
                // Modulo 10 so that each number is only 1 digit -
                // easy for displaying larger matrices on screen.
                *M(m,n,i,j) = j%10;
                //            *(m+i*N+j)= rand()%10;
            }
        }
    }
    return m;
}

int* loadMatrix(char *filename, int n)
{
    int i, j, *m;
    
    FILE *fp = fopen(filename, "r");
    if (fp==NULL) {
        printf("Error opening %s for reading\n", filename);
        exit(1);
    }
    
    m = allocateMatrix(n);
    printf("Loading matrix...\n");
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
//            fscanf(fp, "%d", &m[i][j]);
            fscanf(fp, "%d", M(m,n,i,j));
        }
    }
    return m;
}

int* allocateMatrix(int length)
{
    int *matrix;
    printf("Allocating Memory...\n");
    matrix = malloc(sizeof(int)*length*length);
    if (matrix==NULL) {
        printf("\nError allocating memory.  Exiting ...\n");
        exit(1);
    }
    return matrix;
}
