#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROW 64
#define COLUMN 32
#define TILESIZE 32


/**
 * Input size assumed to be power of 2.
 *
 */

int** allocateMatrix(int row, int column);
int** generateMatrix(int row, int column);
int** loadMatrix(char *infile, int row, int column);

void transpose(int **matrix, int row, int column);
void transpose1Tiled(int **matrix, int row, int column);
void transposeCacheOblivious(int **matrix, int row, int column);

void transposeOPBasic(int **source, int **target, int row, int column);

void printm(int **matrix, int row, int column);
void printmf(int **matrix, int row, int column, const char *filename);

void swapTiles(int **matrix1, int **matrix2, int row, int column);

int main(int argc, char *argv[])
{
    int **a, **b;
    char *usage = "Usage: transposeOP -(basic|1tiled|2tiled|cacheob) [[-i <infile>] -r <row> -c <column>] [-o <outfile>] \n";
    char *infile=NULL, *outfile=NULL, mode;
    int row=0, column=0;
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
        } else if (!strcmp("-r", argv[i])) {
            sscanf(argv[++i], "%d", &row);
        } else if (!strcmp("-c", argv[i])) {
            sscanf(argv[++i], "%d", &column);            
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
        if (row==0) {
            row = ROW;
        }
        if (column==0) {
            column = COLUMN;
        }
        m = generateMatrix(row, column);
    } else {
        if (row<=0 || column<=0) {
            printf("Please provide a valid dimension of the input matrix.\n");
            printf("%s", usage);
            exit(0);
        }
        m = loadMatrix(infile, row, column);
    }
    
    //    m = generateMatrix(LENGTH);
    
    //    printf("\nBefore Transpose: \n");
    //    printmf(m, LENGTH, outfile);
    
    switch (mode) {
        case 'b':
            transpose(m, row, column);
            break;
        case '1':
            transpose1Tiled(m, row, column);
            break;
        default:
            printf("%s option not recognized or not implemented\n", argv[1]);
            printf("Usage: transpose -(basic|1tiled|2tiled) <inputfile>\n");
            exit(0);
    }
    
    
    //    printf("\nAfter Transpose: \n");
    //    printm(m, LENGTH);
    if (outfile==NULL) {
        printm(m, row, column);
    } else {
        printmf(m, row, column, outfile);
    }
    
    free(m);
    return 1;
}

void transpose(int **a, int **b, int r, int c)
{
    int i, j;
    
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            b[j][i]=a[i][j];
        }
    }
}

void transpose1Tiled(int **m, int n)
{
    int i, j, temp;
    int i1, j1, s=TILESIZE;
    
    if (n<=s) {
        transpose(m, n);
    } else {
        for (i1=0; i1<n; i1+=s) {
            for (j1=i1; j1<n; j1+=s) {
                //printf("i1=%d, j1=%d\n", i1, j1);
                
                //transpose 1st tile
                //A(i1, j1)(i1+s, j1+s) is the 1st tile.
                //printf("transposing tile[%d][%d]\n", i1, j1);
                //transpose(&m[i1,j1], s);
                for (i=0; i<s; i++) {
                    for (j=i; j<s; j++) {
                        temp = m[i1+i][j1+j];
                        m[i1+i][j1+j]=m[i1+j][j1+i];
                        m[i1+j][j1+i]=temp;
                    }
                }
                if (i1==j1) {
                    //only the above transpose needed.
                    continue;
                }
                //transpose 2nd tile
                //A(j1, i1)(j1+s, i1+s) is the 2nd tile.
                //printf("transposing tile[%d][%d]\n", j1, i1);
                //transpose(&m[j1,i1], s);
                for (i=0; i<s; i++) {
                    for (j=i; j<s; j++) {
                        temp = m[j1+i][i1+j];
                        m[j1+i][i1+j]=m[j1+j][i1+i];
                        m[j1+j][i1+i]=temp;
                    }
                }
                //swap the tiles
                //printf("swapping tiles [%d][%d] and [%d][%d]\n", i1, j1, j1, i1);
                //swapTiles(&m[i1,j1], &m[j1,i1], s);
                for (i=0; i<s; i++) {
                    for (j=0; j<s; j++) {
                        temp = m[i1+i][j1+j];
                        m[i1+i][j1+j]=m[j1+i][i1+j];
                        m[j1+i][i1+j]=temp;
                    }
                }
            }
        }        
    }
}

void transposeCacheOblivious(int **m, int n)
{
    int temp;
    if (n==2) {
        temp=m[0][1];
        m[0][1]=m[1][0];
        m[1][0]=temp;
    }
    transposeCacheOblivious(&m[0,0], n/2);
    transposeCacheOblivious(&m[0,n/2], n/2);
    transposeCacheOblivious(&m[n/2,0], n/2);
    transposeCacheOblivious(&m[n/2,n/2], n/2);
    //swap m[0][n/2] with m[n/2][0]
}

void swapTiles(int **a, int **b, int n)
{
    int i, j, temp;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            temp=a[i][j];
            a[i][j]=b[i][j];
            b[i][j]=temp;
        }
    }
}

void printm(int **m, int n)
{
    int i, j;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}

void printmf(int **m, int n, const char *filename)
{
    int i, j;
    
    FILE *fp = fopen(filename, "w");
    if (fp==NULL) {
        printf("Error opening %s for writing\n", filename);
    }
    
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            fprintf(fp, "%d ", m[i][j]);
        }
        fprintf(fp, "\n");
    }
}

int** generateMatrix(int length)
{
    int i, j, **m;
    m = allocateMatrix(length);
    for (i=0; i<length; i++) {
        for (j=0; j<length; j++) {
            m[i][j] = rand()%10;
        }
    }
    return m;
}

int** loadMatrix(char *filename, int n)
{
    int i, j, **m;
    
    FILE *fp = fopen(filename, "r");
    if (fp==NULL) {
        printf("Error opening %s for reading\n", filename);
        exit(1);
    }
    
    m = allocateMatrix(n);
    
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            fscanf(fp, "%d", &m[i][j]);
        }
    }
    return m;
}

int** allocateMatrix(int length)
{
    int i, **m;
    // m = malloc(sizeof(int)*length*length);
    m = calloc(length, sizeof(int *));
    for (i=0; i<length; i++) {
        m[i] = calloc(length, sizeof(int));
        if (m[i] == NULL) {
            printf("\nError allocating memory.  Exiting ...\n");
            exit(1);            
        }
    }
    if (m==NULL) {
        printf("\nError allocating memory.  Exiting ...\n");
        exit(1);
    }
    return m;
}