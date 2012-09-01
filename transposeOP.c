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
 * Assignment 1.b of Parallel Programming course during 
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
void transpose(int *matrix1, int *matrix2, int size_s, int row_m, int column_n);
// matrix can be a sub-matrix (nxn) of another matrix (NxN). Use tilesize s for the algorithm. 
void transpose1Tiled(int *matrix, int dimension_N, int size_n, int tilesize_s);
// two tiled transpose of matrix (nxn) with tile size as s1 and s2.
void transpose2Tiled(int *matrix, int dimension_n, int tile1size_s1, int tile2size_s2);

void transposeCacheOblivious(int *matrix, int length_N, int tilesize_n, int row_i, int column_j);
// matrices can be sub-matrices 
void swapTiles(int *matrix1, int *matrix2, int tilesize_s, int length_n);

void printm(int *matrix, int row, int column);
void printmf(int *matrix, int row, int column, const char *filename);

int* M(int *matrix, int dimension, int i, int j);

/**
 * Sample arguments
 *
 * To transpose a randomly generated 64x64 matrix using basic algorithm 
 * (both input and output displayed on screen)
 *              ./transposeIP -basic -n64
 *
 * To transpose a randomly generated 128x128 matrix using 1tiled algorithm 
 * with tile size 16
 *              ./transposeIP -1tiled -n 128 -s1 16
 *
 * To transpose a randomly generated 128x128 matrix using 2tiled algorithm
 * with level 1 tile size 16 and level 2 tile size 4
 *              ./transposeIP -2tiled -n 128 -s1 16 -s2 4
 *
 * To transpose a 4096x4096 matrix using cache oblivious algorithm with input 
 * file infile.txt and directing output to outfile.txt
 *              ./transposeIP -cacheob -i infile.txt -n 4096 -o outfile.txt
 *
 * Future work: 
 * - main() is too big.  Move argument handling to a separate function.
 * - add details of various algorithms used.
 *
 */
int main(int argc, char *argv[])
{
    int* m;
    const char *usage = "Usage: transposeIP -(basic|1tiled|2tiled|cacheob) [[-i <infile>] -n <dimension>] [-s1 tilesize] [-s2 tilesize] [-o <outfile>] \n";
    char *infile=NULL, *outfile=NULL, mode='b';
    int dimension=0, tile1size=0, tile2size=0;
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
        } else if (!strcmp("-n", argv[i])) {
            sscanf(argv[++i], "%d", &dimension);
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
        if (dimension==0) {
            dimension = LENGTH;
        }
        m = generateMatrix(dimension);
    } else {
        if (dimension<=0) {
            printf("Please provide a valid dimension of the input matrix.\n");
            printf("%s", usage);
            exit(0);
        }
        m = loadMatrix(infile, dimension);
    }

    printf("\nBefore Transpose: \n");
    printm(m, dimension);
    
    switch (mode) {
        case 'b':
            transpose(m, dimension, dimension);
            break;
        case '1':
            if (tile1size == 0) {
                tile1size = TILE1SIZE;
            }
            transpose1Tiled(m, dimension, dimension, tile1size);
            break;
        case '2':
            if (tile2size == 0) {
                tile2size = TILE2SIZE;
            }
            transpose2Tiled(m, dimension, tile1size, tile2size);
            break;
        case 'c':
            transposeCacheOblivious(m, dimension, dimension, 0, 0);
            break;
        default:
            printf("Option not recognized or not implemented\n");
            printf("%s", usage);
            exit(0);
    }
    
    printf("\nAfter Transpose: \n");
    if (outfile==NULL) {
        printm(m, dimension);
    } else {
        printmf(m, dimension, outfile);
    }
    
    free(m);
    return 1;
}

/**
 * m is sub-matrix (length s) of another matrix (length n).
 * For basic transpose, s equals n.
 *
 */
void transpose(int *a, int *b, int s, int m, int n) 
{
    int i, j;
    
    for (i=0; i<s; i++) {
        for (j=i+1; j<s; j++) {
            *M(a,n,i,j)=*M(m,n,j,i);
        } 
    }
}


int* M(int*m, int N, int i, int j)
{
//    printf("cell number is %d ", i*N+j);
    return m+i*N+j;
}

/**
 * (i1, j1) is the position of the nxn sub-matrix in an NxN matrix.
 * s is the tilesize.
 * It is important to have the right tile size for optimal performance.
 * Find this using (?)
 */
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
//        printf("Swapping %d, %d\n", *M(m,N,i,j+1), *M(m,N,i+1,j));
//        printf("at cell number %d and %d\n", N*i+j+1, N*(i+1)+j);
        temp = *M(m,N,i,j+1);
        *M(m,N,i,j+1) = *M(m,N,i+1,j);
        *M(m,N,i+1,j) = temp;
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
            temp=*M(a,n,i,j);
            *M(a,n,i,j)=*M(b,n,i,j);
            *M(b,n,i,j)=temp;
//            temp=a[i][j];
//            a[i][j]=b[i][j];
//            b[i][j]=temp;
        }
    }
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

int* generateMatrix(int row, int column)
{
    int i, j, *m;
    m = allocateMatrix(row, column);
    printf("Generating matrix...\n");
    for (i=0; i<row; i++) {
        for (j=0; j<column; j++) {
//            m[i][j] = rand()%10;
            *M(m,row,i,j) = rand()%10;
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
            fscanf(fp, "%d", M(m,row,i,j));
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