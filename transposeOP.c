#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROW 64
#define COLUMN 32
#define TILE1SIZE 16
#define TILE2SIZE 4

/*
 * Author: Chandan Kumar chandan.kumar@students.iiit.ac.in
 * Date: 2012-08-20
 *
 * Assignment 1.2 of Parallel Programming course during 
 * Monsoon 2012 semester offered by Suresh Purini.
 * 
 * Out-of-place matrix transpose of dimension m x n.
 * Input size assumed to be power of 2. 
 *
 */

int* allocateMatrix(int row, int column);
int* generateMatrix(int row, int column);
int* loadMatrix(char *infile, int row, int column);

// matrix can be a sub-matrix (sxs) of another matrix (nxn).
void transpose(int *matrix1, int *matrix2, int row_M, int column_N, int row_m, int column_n);
// matrix can be a sub-matrix (nxn) of another matrix (NxN). Use tilesize s for the algorithm. 
void transpose1Tiled(int *matrix, int dimension_N, int size_n, int tilesize_s);
// two tiled transpose of matrix (nxn) with tile size as s1 and s2.
void transpose2Tiled(int *matrix, int dimension_n, int tile1size_s1, int tile2size_s2);

void transposeCacheOblivious(int *matrix, int length_N, int tilesize_n, int row_i, int column_j);
// matrices can be sub-matrices 
void swapTiles(int *matrix1, int *matrix2, int tilesize_s, int length_n);

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
    int *a, *b;
    const char *usage = "Usage: transposeOP -(basic|1tiled|2tiled|cacheob) [[-i <infile>] -m <row> -n <column>] [-s1 tilesize] [-s2 tilesize] [-o <outfile>] \n";
    char *infile=NULL, *outfile=NULL, mode='b';
    int row=0, column=0, tile1size=0, tile2size=0;
    int i;
    
    
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
                   !strcmp("-cacheob", argv[i])) {
            mode=argv[i][1]; //mode = 'b', '1', '2' or 'c'
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
        a = generateMatrix(row, column);

    } else {
        if (row<=0 || column<=0) {
            printf("Please provide a valid dimension of the input matrix.\n");
            printf("%s", usage);
            exit(0);
        }
        a = loadMatrix(infile, row, column);
    }
    b = allocateMatrix(column, row);
    
    printf("\nBefore Transpose: \n");
    printm(a, row, column);
    
    switch (mode) {
        case 'b':
            transpose(a, b, row, column, row, column);
            break;
        case '1':
            if (tile1size == 0) {
                tile1size = TILE1SIZE;
            }
//            transpose1Tiled(m, dimension, dimension, tile1size);
            break;
        case '2':
            if (tile2size == 0) {
                tile2size = TILE2SIZE;
            }
//            transpose2Tiled(m, dimension, tile1size, tile2size);
            break;
        case 'c':
//            transposeCacheOblivious(m, dimension, dimension, 0, 0);
            break;
        default:
            printf("Option not recognized or not implemented\n");
            printf("%s", usage);
            exit(0);
    }
    
    printf("\nAfter Transpose: \n");
    if (outfile==NULL) {
        printm(b, column, row);
    } else {
        printmf(b, column, row, outfile);
    }
    
    free(a);
    free(b);
    return 1;
}

/**
 * Transpose matrix a and place it into b.
 * a and b can be sub-matrices (mxn, nxm) of another matrix (MxN, NxM).
 * For basic transpose, s equals n.
 *
 */
void transpose(int *a, int *b, int M, int N, int m, int n) 
{
    int i, j;
    
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
 * (i1, j1) is the position of the nxn sub-matrix in an NxN matrix.
 * s is the tilesize.
 * It is important to have the right tile size for optimal performance.
 * Find this using (?)
 *
void transpose1Tiled(int *m, int N, int n, int s)
{
    int i, j;
    
    if (n<=s) {
        transpose(m, n, n);
    } else {
        for (i=0; i<n; i+=s) {
            for (j=i; j<n; j+=s) {
                //transpose 1st tile
                //A(i, j)(i+s, j+s) is the 1st tile.
                transpose(M(m,N,i,j), s, N);
                if (i==j) {
                    //only the above transpose needed since this is a diagonal tile.
                    //only tiles above/below the diagonal need swapping and transposing.
                    continue;
                }
                
                //transpose 2nd tile
                //A(j, i)(j+s, i+s) is the 2nd tile.
                transpose(M(m,N,j,i), s, N);               

                //swap the tiles
                swapTiles(M(m,N,i,j), M(m,N,j,i), s, N);
            }
        }    
    }
}


void transpose2Tiled(int *m, int n, int s1, int s2)
{
    int i, j;
    
    if (s1<=s2) {
        printf("Tilesize s1 must be greater than tilesize s2\n");
        exit(0);
    }
    if (n<=s1) {
        transpose(m, n, n);
    } else {
        for (i=0; i<n; i+=s1) {
            for (j=i; j<n; j+=s1) {
                //transpose 1st tile using sub-tiling.
                transpose1Tiled(M(m,n,i,j), n, s1, s2);
                if (i==j) {
                    continue;
                }
                
                //transpose 2nd tile using sub-tiling.
                transpose1Tiled(M(m,n,j,i), n, s1, s2);
                
                //swap the tiles.
                swapTiles(M(m,n,i,j), M(m,n,j,i), s1, n);
            }
        }
    }
}

void transposeCacheOblivious(int *m, int N, int n, int i, int j)
{   
    int temp;
//    printf("N= %d, n=%d, i=%d, j=%d\n", N, n, i, j);
    if (n==2) {
        //base case: if matrix size is 2x2, swap m(0,1) with m(1,0)
//        printf("Swapping %d, %d\n", *A(m,N,i,j+1), *A(m,N,i+1,j));
//        printf("at cell number %d and %d\n", N*i+j+1, N*(i+1)+j);
        temp = *A(m,N,i,j+1);
        *A(m,N,i,j+1) = *A(m,N,i+1,j);
        *A(m,N,i+1,j) = temp;
        return;        
    }
    transposeCacheOblivious(m, N, n/2, i, j);
    transposeCacheOblivious(m, N, n/2, i, j+n/2);
    transposeCacheOblivious(m, N, n/2, i+n/2, j);
    transposeCacheOblivious(m, N, n/2, i+n/2, j+n/2);
    swapTiles(M(m,N,i,j+n/2), M(m,N,i+n/2,j), n/2, N);
}

void swapTiles(int *a, int *b, int s, int n)
{
    int i, j, temp;
    for (i=0; i<s; i++) {
        for (j=0; j<s; j++) {
            temp=*A(a,n,i,j);
            *A(a,n,i,j)=*A(b,n,i,j);
            *A(b,n,i,j)=temp;
//            temp=a[i][j];
//            a[i][j]=b[i][j];
//            b[i][j]=temp;
        }
    }
}
*/
 
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

int* generateMatrix(int row, int column)
{
    int i, j, *m;
    m = allocateMatrix(row, column);
    printf("Generating matrix...\n");
    for (i=0; i<row; i++) {
        for (j=0; j<column; j++) {
//            m[i][j] = rand()%10;
            *A(m,column,i,j) = rand()%10;
//            *A(m,column,i,j) = j%10;
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