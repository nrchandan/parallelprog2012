/*
 * Author: Chandan Kumar chandan.kumar@students.iiit.ac.in
 * Date: 2012-08-20
 *
 * Assignment 1.2 of Parallel Programming course during 
 * Monsoon 2012 semester offered by Suresh Purini.
 * 
 * Out-of-place matrix transpose of dimension m x n (m rows, n columns)
 * Input dimensions m and n assumed to be power of 2.
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROW 16384 
#define COLUMN 16384
#define TILE1SIZE 16
#define TILE2SIZE 4


int* allocateMatrix(int row, int column);
int* generateMatrix(int row, int column, int noinit);
int* loadMatrix(char *infile, int row, int column);

// matrix can be a sub-matrix (mxn) of another matrix (MxN).
void transpose(int *matrix1, int *matrix2, int row_M, int column_N, int row_m, int column_n);

void printm(int *matrix, int row, int column);
void printmf(int *matrix, int row, int column, const char *filename);

int *A(int *matrix, int dimension, int i, int j);

/**
 * Sample arguments
 *
 * To transpose a randomly generated 64x32 matrix using basic algorithm 
 * (both input and output displayed on screen)
 *              ./transposeOP -basic -m 64 -n 32
 *
 *
 */
int main(int argc, char *argv[])
{
    int *a, *b;
    const char *usage = "Usage: basictransposeOP [[-i <infile>] -m <row> -n <column>] [-o <outfile>] [-noIO] [-noinit]\n";
    char *infile=NULL, *outfile=NULL;
    int row=0, column=0, tile1size=0, tile2size=0;
    int i, noio=1, noinit=1;;
    
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
        } else if (!strcmp("-m", argv[i])) {
            sscanf(argv[++i], "%d", &row);
        } else if (!strcmp("-n", argv[i])) {
            sscanf(argv[++i], "%d", &column);
        } else if (!strcmp("-noIO", argv[i]) || !strcmp("-noio", argv[i])) {
            noio=1;
        } else if (!strcmp("-noinit", argv[i])) {
            noinit=1;
        }
    }
    
    if (infile == NULL) {
        // no input file provided
        if (row==0) {
            row=ROW;
        }
        if (column==0) {
            column=COLUMN;
        }
        a = generateMatrix(row, column, noinit);

    } else {
        if (row<=0 || column<=0) {
            printf("Please provide a valid dimension of the input matrix.\n");
            printf("%s", usage);
            exit(0);
        }
        a = loadMatrix(infile, row, column);
    }
    b = allocateMatrix(column, row);
    
    if (!noio) {
        printf("\nBefore Transpose: \n");
        printm(a, row, column);
    }
    
    printf("Starting transpose...\n");

    transpose(a, b, row, column, row, column);
    printf("Transpose complete.\n");
    if (!noio) {
        printf("\nAfter Transpose: \n");
        if (outfile==NULL) {
            printm(b, column, row);
        } else {
            printmf(b, column, row, outfile);
        }
    }
    
    free(a);
    free(b);
    return 1;
}

/**
 * Transpose matrix a and place it into b.
 * a and b can be sub-matrices (mxn, nxm) of another matrix (MxN, NxM).
 *
 */
void transpose(int *a, int *b, int M, int N, int m, int n)
{
    int i, j;

    #pragma omp parallel default (none) shared (a,b,M,N,m,n,i) private (j)
    #pragma omp for schedule (runtime)   
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            *A(b,M,j,i)=*A(a,N,i,j);
        }
    }
}


int *A(int*m, int N, int i, int j)
{
//    printf("cell number is %d ", i*N+j);
    return m+i*N+j;
}

 
/**
 * Print matrix in stdout.
 *
 */
void printm(int *a, int m, int n)
{
    int i, j;
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
//            printf("%d ", m[i][j]);
            printf("%d ", *A(a,n,i,j));
        }
        printf("\n");
    }
}

/**
 * Print matrix in the specified file.
 *
 */
void printmf(int *a, int m, int n, const char *filename)
{
    int i, j;
    
    FILE *fp = fopen(filename, "w");
    if (fp==NULL) {
        printf("Error opening %s for writing\n", filename);
    }
    
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            fprintf(fp, "%d ", *A(a,n,i,j));
        }
        fprintf(fp, "\n");
    }
}

int* generateMatrix(int row, int column, int noinit)
{
    int i, j, *m;
    m = allocateMatrix(row, column);
    if (!noinit) {
        printf("Generating matrix...\n");
        for (i=0; i<row; i++) {
            for (j=0; j<column; j++) {
                //            m[i][j] = rand()%10;
                //            *A(m,column,i,j) = rand()%10;
                *A(m,column,i,j) = j%10;
            }
        }
    }
    return m;
}

int* loadMatrix(char *filename, int row, int column)
{
    int i, j, *m;
    
    FILE *fp = fopen(filename, "r");
    if (fp==NULL) {
        printf("Error opening %s for reading\n", filename);
        exit(1);
    }
    
    m = allocateMatrix(row, column);
    printf("Loading matrix...\n");
    for (i=0; i<row; i++) {
        for (j=0; j<column; j++) {
//            fscanf(fp, "%d", &m[i][j]);
            fscanf(fp, "%d", A(m,row,i,j));
        }
    }
    return m;
}

int* allocateMatrix(int row, int column)
{
    int *matrix;
    printf("Allocating Memory...\n");
    matrix = malloc(sizeof(int)*row*column);
    if (matrix==NULL) {
        printf("\nError allocating memory.  Exiting ...\n");
        exit(1);
    }
    return matrix;
}
