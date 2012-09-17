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
 * Compile command:
 * icc transposeIP.c -lmkl_core -lmkl_intel_lp64 -lmkl_sequential
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mkl_trans.h>

#define ROW 64
#define COLUMN 32
#define TILE1SIZE 16
#define TILE2SIZE 4


float* allocateMatrix(int row, int column);
float* generateMatrix(int row, int column, int noinit);
float* loadMatrix(char *infile, int row, int column);

// matrix can be a sub-matrix (mxn) of another matrix (MxN).
void transpose(float *matrix1, float *matrix2, int row_M, int column_N, int row_m, int column_n);

void transpose1Tiled(float *matrix1, float *matrix2, int row_M, int column_N, int row_m, int column_n, int tilesize_s);

void transpose2Tiled(float *matrix1, float *matrix2, int row_m, int column_n, int tile1size_s1, int tile2size_s2);

// row_M and column N are needed in order to determine correct i and j.
void transposeCacheOblivious(float *matrix1, float *matrix2, int row_M, int column_N, int row_m, int column_n, int position_i, int position_j, int tilesize_s);

// Intel MKL BLAS optimized transpose.
void transposeBLAS(float *matrix1, float *matrix2, int row, int column);

void printm(float *matrix, int row, int column);
void printmf(float *matrix, int row, int column, const char *filename);

float *A(float *matrix, int dimension, int i, int j);

/**
 * Sample arguments
 *
 * To transpose a randomly generated 64x32 matrix using basic algorithm 
 * (both input and output displayed on screen)
 *              ./transposeOP -basic -m 64 -n 32
 *
 * To transpose a randomly generated 128x64 matrix using 1tiled algorithm 
 * with tile size 16
 *              ./transposeOP -1tiled -m 128 -n 64 -s1 16
 *
 * To transpose a randomly generated 128x64 matrix using 2tiled algorithm
 * with level 1 tile size 16 and level 2 tile size 4
 *              ./transposeOP -2tiled -m 128 -n 64 -s1 16 -s2 4
 *
 * To transpose a 4096x1024 matrix using cache oblivious algorithm with input 
 * file infile.txt and directing output to outfile.txt
 *              ./transposeOP -cacheob -i infile.txt -m 4096 -n 1024 -o outfile.txt
 *
 * Future work: 
 * - main() is too big.  Move argument handling to a separate function.
 * - add details of various algorithms used.
 *
 */
int main(int argc, char *argv[])
{
    float *a, *b;
    const char *usage = "Usage: transposeOP -(basic|1tiled|2tiled|cacheob|mkl) [[-i <infile>] -m <row> -n <column>] [-s1 tilesize] [-s2 tilesize] [-o <outfile>] [-noIO] [-noinit]\n";
    char *infile=NULL, *outfile=NULL;
    char mode='b'; //default mode is "-basic".
    int row=0, column=0, tile1size=0, tile2size=0;
    int i, noio=0, noinit=0;;
    
    
    if (argc <=1 ) {
        printf("%s", usage);
        exit(0);
    }
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
        } else if (!strcmp("-s1", argv[i])) {
            sscanf(argv[++i], "%d", &tile1size);
        } else if (!strcmp("-s2", argv[i])) {
            sscanf(argv[++i], "%d", &tile2size);
        } else if (!strcmp("-basic", argv[i])   ||
                   !strcmp("-1tiled", argv[i])  ||
                   !strcmp("-2tiled", argv[i])  ||
                   !strcmp("-cacheob", argv[i]) ||
                   !strcmp("-mkl", argv[i])) {
            mode=argv[i][1]; //mode = 'b', '1', '2', 'c' or 'm'
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
    
    switch (mode) {
        case 'b':
            transpose(a, b, row, column, row, column);
            break;
        case '1':
            if (tile1size == 0) {
                tile1size = TILE1SIZE;
            }
            transpose1Tiled(a, b, row, column, row, column, tile1size);
            break;
        case '2':
            if (tile1size == 0) {
                tile1size = TILE1SIZE;
            }
            if (tile2size == 0) {
                tile2size = TILE2SIZE;
            }
            transpose2Tiled(a, b, row, column, tile1size, tile2size);
            break;
        case 'c':
            if (tile1size == 0) {
                tile1size = TILE1SIZE;
            }
            transposeCacheOblivious(a, b, row, column, row, column, 0, 0, tile1size);
            break;
        case 'm':
            transposeBLAS(a, b, row, column);
            break;
        default:
            printf("Option not recognized or not implemented\n");
            printf("%s", usage);
            exit(0);
    }
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
void transpose(float *a, float *b, int M, int N, int m, int n)
{
    int i, j;
    
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            *A(b,M,j,i)=*A(a,N,i,j);
        }
    }
}


float *A(float*m, int N, int i, int j)
{
//    printf("cell number is %d ", i*N+j);
    return m+i*N+j;
}

void transposeBLAS(float *a, float *b, int m, int n)
{
    /*
     From mkl_trans.h:
     void MKL_Somatcopy(
     char ordering, char trans,
     size_t rows, size_t cols,
     const float alpha,
     const float * A, size_t lda,
     float * B, size_t ldb);
     
     Description of parameters at http://software.intel.com/sites/products/documentation/hpc/composerxe/en-us/2011Update/mklxe/mkl_manual_win_mac/index.htm#bla/functn_mkl_imatcopy.htm
     */

    MKL_Somatcopy ('R', 'T', m, n, 1, a, n, b, m);
}

/**
 *
 */
void transpose1Tiled(float *a, float *b, int M, int N, int m, int n, int s)
{
    int i, j;
    
    if (n<=s) {
        transpose(a, b, M, N, m, n);
    } else {
        for (i=0; i<m; i+=s) {
            for (j=0; j<n; j+=s) {
                transpose(A(a,N,i,j), A(b,M,j,i), M, N, s, s);
            }
        }    
    }
}


void transpose2Tiled(float *a, float *b, int m, int n, int s1, int s2)
{
    int i, j;
    
    if (s1<=s2) {
        printf("Tilesize s1 must be greater than tilesize s2\n");
        exit(0);
    }
    if (n<=s1) {
        transpose(a, b, m, n, m, n);
    } else {
        for (i=0; i<m; i+=s1) {
            for (j=0; j<n; j+=s1) {
                //transpose 1st tile using sub-tiling.
                transpose1Tiled(A(a,n,i,j), A(b,m,j,i), m, n, s1, s1, s2);
            }
        }
    }
}


void transposeCacheOblivious(float *a, float *b, int M, int N, int m, int n, int i, int j, int s)
{   
    printf("m=%d, n=%d, i=%d, j=%d", m, n, i, j);
    if (m>s && n>s) {
        printf(", m>s && n>s\n");
        // if both row and column dimensions are bigger than the tilesize, break up the matrix 
        // into four sub-matrices of size m/2 x n/2
        transposeCacheOblivious(A(a,N,i,j), A(b,M,j,i), M, N, m/2, n/2, i, j, s);
        transposeCacheOblivious(A(a,N,i,j+n/2), A(b,M,j+n/2,i), M, N, m/2, n/2, i, j+n/2, s);
        transposeCacheOblivious(A(a,N,i+m/2,j), A(b,M,j,i+m/2), M, N, m/2, n/2, i+m/2, j, s);
        transposeCacheOblivious(A(a,N,i+m/2,j+n/2), A(b,M,j+n/2,i+m/2), M, N, m/2, n/2, i+m/2, j+n/2, s);
    } else if (m>s) {
        printf(", m>s\n");
        // if row dimension is bigger than the tilesize, break up the matrix into two sub-matrices.
        transposeCacheOblivious(A(a,N,i,j), A(b,M,j,i), M, N, m/2, n, i, j, s);
        transposeCacheOblivious(A(a,N,i+m/2,j), A(b,M,j,i+m/2), M, N, m/2, n, i+m/2, j, s);
    } else if (n>s) {
        printf(", n>s\n");
        // if column dimension is bigger than the tilesize, break up the matrix into two sub-matrices.
        transposeCacheOblivious(A(a,N,i,j), A(b,M,j,i), M, N, m, n/2, i, j, s);
        transposeCacheOblivious(A(a,N,i,j+n/2), A(b,M,j+n/2,i), M, N, m, n/2, i, j+n/2, s);
    } else {
//        printf(" m=s && n=s\n");
        printf(", Transposing %d x %d matrix at position %d, %d\n", m, n, i, j);
        printf("First element of matrix a is %.1f\n", *a);
        transpose(a, b, M, N, m, n);
        return;
    }
}

 
/**
 * Print matrix in stdout.
 *
 */
void printm(float *a, int m, int n)
{
    int i, j;
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
//            printf("%d ", m[i][j]);
            printf("%.1f ", *A(a,n,i,j));
        }
        printf("\n");
    }
}

/**
 * Print matrix in the specified file.
 *
 */
void printmf(float *a, int m, int n, const char *filename)
{
    int i, j;
    
    FILE *fp = fopen(filename, "w");
    if (fp==NULL) {
        printf("Error opening %s for writing\n", filename);
    }
    
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            fprintf(fp, "%f ", *A(a,n,i,j));
        }
        fprintf(fp, "\n");
    }
}

float* generateMatrix(int row, int column, int noinit)
{
    int i, j;
    float *m;
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

float* loadMatrix(char *filename, int row, int column)
{
    int i, j;
    float *m;
    
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
            fscanf(fp, "%f", A(m,row,i,j));
        }
    }
    return m;
}

float* allocateMatrix(int row, int column)
{
    float *matrix;
    printf("Allocating Memory...\n");
    matrix = malloc(sizeof(int)*row*column);
    if (matrix==NULL) {
        printf("\nError allocating memory.  Exiting ...\n");
        exit(1);
    }
    return matrix;
}