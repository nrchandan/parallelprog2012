#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LENGTH 64
#define TILESIZE 16

/*
 * Input size assumed to be power of 2. Matrix assumed to be square i.e n x n.
 *
 */

int* allocateMatrix(int length);
int* generateMatrix(int length);
int* loadMatrix(char *infile, int length);

void transpose(int *matrix, int tilesize, int length);
void transpose1Tiled(int *matrix, int length, int tilesize);
void transposeCacheOblivious(int *matrix, int length, int tilesize, int row, int column);

void swapTiles(int *matrix1, int *matrix2, int tilesize, int length);

void printm(int *matrix, int length);
void printmf(int *matrix, int length, const char *filename);

int* M(int *matrix, int dimension, int i, int j);


int main(int argc, char *argv[])
{
    int* m;
    char *usage = "Usage: transpose -(basic|1tiled|2tiled|cacheob) [[-i <infile>] -n <dimension>] [-s tilesize] [-o <outfile>] \n";
    char *infile=NULL, *outfile=NULL, mode;
    int dimension=0, tilesize=0;
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
        } else if (!strcmp("-s", argv[i])) {
            sscanf(argv[++i], "%d", &tilesize);
        } else if (!strcmp("-basic", argv[i])   ||
                   !strcmp("-1tiled", argv[i])  ||
                   !strcmp("-2tiled", argv[i])  ||
                   !strcmp("-cacheob", argv[i])) {
            mode=argv[i][1]; //mode = 'b', '1', '2' or 'c'
        }
    }
//    printf("infile=%s outfile=%s dimension=%d mode=%c\n", infile, outfile, dimension, mode);
    
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
    if (tilesize == 0) {
        tilesize = TILESIZE;
    }
//    printf("infile=%s outfile=%s dimension=%d mode=%c\n", infile, outfile, dimension, mode);
    printf("\nBefore Transpose: \n");
    printm(m, dimension);
    
    switch (mode) {
        case 'b':
            transpose(m, dimension, dimension);
            break;
        case '1':
            transpose1Tiled(m, dimension, tilesize);
            break;
        case 'c':
            transposeCacheOblivious(m, dimension, dimension, 0, 0);
            break;
        default:
            printf("%s option not recognized or not implemented\n", argv[1]);
            printf("Usage: transpose -(basic|1tiled|2tiled) <inputfile>\n");
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
 * m is sub-matrix/ tile (length s) of another matrix (length n).
 * For basic transpose, s equals n.
 *
 */
void transpose(int *m, int s, int n) 
{
    int i, j, temp;
    
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
 *
 * It is important to have the right tile size for optimal performance.
 * Find this using (?)
 */
void transpose1Tiled(int *m, int n, int s)
{
//    int i, j, temp;
    int i1, j1;
    
    if (n<=s) {
        transpose(m, n, n);
    } else {
        for (i1=0; i1<n; i1+=s) {
            for (j1=i1; j1<n; j1+=s) {
                //printf("i1=%d, j1=%d\n", i1, j1);
                
                //transpose 1st tile
                //A(i1, j1)(i1+s, j1+s) is the 1st tile.
                
                transpose(M(m,n,i1,j1),s,n);
                if (i1==j1) {
                    //only the above transpose needed since this is a diagonal tile.
                    //only tiles above/below the diagonal need swapping and transposing.
                    continue;
                }
                
                //transpose 2nd tile
                //A(j1, i1)(j1+s, i1+s) is the 2nd tile.
                
                transpose(M(m,n,j1,i1), s, n);

                //swap the tiles

                swapTiles(M(m,n,i1,j1), M(m,n,j1,i1), s, n);
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

int* generateMatrix(int n)
{
    int i, j, *m;
    m = allocateMatrix(n);
    printf("Generating matrix...\n");
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
//            m[i][j] = rand()%10;
            *M(m,n,i,j) = rand()%10;
//            *(m+i*N+j)= rand()%10;
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