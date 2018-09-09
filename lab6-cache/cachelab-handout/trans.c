/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	int i, j, tmp;
	int stride = 8, a, b, tmp1, tmp2, tmp3, tmp4, rm, rn;
	// M 與 N 不相等，或者 M 不是 8 的整數倍  
	if ((M != N) || (M & 7) || (N & 7)) {
	    //int dm = M & (~7);
	    rm = M & 7;
	    
	    //int dn = N & (~7);
	    rn = N & 7;
	    
	    for (i = N - rn; i < N; i++) {
	    	for (j = 0; j < M - rm; j++) {
	    		tmp = A[i][j];
	    		B[j][i] = tmp;
	    	}
	    }
	    for (i = 0; i < N; i++) {
	    	for (j = M - rm; j < M; j++) {
	    		tmp = A[i][j];
	    		B[j][i] = tmp;
	    	}
	    }
	    
	    M = M - rm;
	    N = N -rn;
	}
	for (i = 0; i < N; i+=stride) {
		for (j = 0; j < M; j+=stride) {      
			for (a = i; a<i+stride;a++) {  
				tmp = A[a][j];
				tmp1 = A[a][j + 1];
				tmp2 = A[a][j + 2];
				rm = A[a][j + 3];
				rn = A[a][j + 4];
				b = A[a][j + 5];
				tmp3 = A[a][j + 6];
				tmp4 = A[a][j + 7];
				        
					         
				B[j][a] = tmp;
				B[j+1][a] = tmp1;
				B[j+2][a] = tmp2;
				B[j+3][a] = rm;
				B[j+4][a] = rn;
				B[j+5][a] = b;
				B[j+6][a] = tmp3;
				B[j+7][a] = tmp4;        
				
			}
		}
	}

}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

