/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "cc1000",              /* Team name */

    "Chaunecy WANG",     /* First member full name */
    "chaunecy@outlook.com",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
#define block 16
void rotate(int dim, pixel *src, pixel *dst) 
{
    // naive_rotate(dim, src, dst);
    int i, j;
	int opt_dim = dim - 1; 
    for (i = 0; i < dim; i+=block) {
		for (j = 0; j < dim; j+=block) {
			for (int a=i, end_a=i+block; a<end_a;a++) {
				int tmp = a * dim;
				for (int b=j,end_b=j+block; b<end_b; b++) {
					dst[RIDX(opt_dim - b, a, dim)] = src[tmp + b];
				}
			}
		}
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

static pixel init_pixel() {
	pixel p;
	p.red = 0;
	p.green = 0;
	p.blue = 0;
	return p;
}

static pixel my_avg(int dim, int i, int j, pixel *src) {
	int ii, jj;
	int mini = max(i - 1, 0);
	int maxi = min(i+1, dim-1);
	int minj = max(j - 1, 0);
	int maxj = min(j + 1, dim-1);
	pixel_sum sum;
	pixel current_pixel;
	
	
	pixel p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;
	int left_ok, right_ok, up_ok, down_ok;
	
	left_ok = i  > 0;
	right_ok = i + 1 < dim;
	up_ok = j > 0;
	down_ok = j + 1 < dim;
	pixel avater = init_pixel();
	p1 = (left_ok && up_ok) ? src[RIDX(i-1, j-1, dim)] : avater;
	p2 = (up_ok) ? src[RIDX(i, j-1, dim)]: avater;
	p3 = (right_ok && up_ok) ? src[RIDX(i + 1, j-1, dim)] : avater;
	p4 = (left_ok) ? src[RIDX(i-1, j, dim)] : avater;
	p5 = src[RIDX(i, j, dim)];
	p6 = (right_ok) ? src[RIDX(i + 1, j, dim)]: avater;
	p7 = (left_ok && down_ok) ? src[RIDX(i-1, j+1, dim)]: avater;
	p8 = (down_ok) ? src[RIDX(i, j+1, dim)]: avater;
	p9 = (right_ok && down_ok) ? src[RIDX(i +1, j+1, dim)]: avater;
	
    sum.red = sum.green = sum.blue = 0;
    sum.num = 0;

	int red1, red2, red3, blue1, blue2, blue3, green1, green2, green3;
	red1 = ((int)p2.red + p3.red) + p1.red;
	blue1 += ((int)p2.blue + p3.blue) + p1.blue;
	green1 += ((int)p2.green + p3.green) + p1.green;
	
	red2 =((int)p5.red + p6.red) + p4.red;
	blue2 =((int)p5.blue + p6.blue) + p4.blue;
	green2 = ((int)p5.green + p6.green) + p4.green;
	
	red3 =((int)p8.red + p9.red) + p7.red;
	blue3 =((int)p8.blue + p9.blue) + p7.blue;
	green3 = ((int)p8.green + p9.green) + p7.green;
	
	sum.red = (red1 + red2) + red3;
	sum.blue = blue1 + blue2 + blue3;
	sum.green = green1 + green2 + green3;
	
	sum.num = (maxi - mini + 1) * (maxj - minj + 1);
    current_pixel.red = (unsigned short) (sum.red/sum.num);
    current_pixel.green = (unsigned short) (sum.green/sum.num);
    current_pixel.blue = (unsigned short) (sum.blue/sum.num);
	return current_pixel;
}



/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = my_avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst)
{
    int i, j;

    pixel current_pixel;
    pixel *pcurrent_pixel = &current_pixel;

    i = 0;
    j = 0;
    pcurrent_pixel->red =
        (unsigned short)(((int)(src[RIDX(0, 0, dim)].red + src[RIDX(0, 1, dim)].red +
                                src[RIDX(1, 0, dim)].red + src[RIDX(1, 1, dim)].red)) /
                         4);
    pcurrent_pixel->green =
        (unsigned short)(((int)(src[RIDX(0, 0, dim)].green + src[RIDX(0, 1, dim)].green +
                                src[RIDX(1, 0, dim)].green + src[RIDX(1, 1, dim)].green)) /
                         4);
    pcurrent_pixel->blue =
        (unsigned short)(((int)(src[RIDX(0, 0, dim)].blue + src[RIDX(0, 1, dim)].blue +
                                src[RIDX(1, 0, dim)].blue + src[RIDX(1, 1, dim)].blue)) /
                         4);
    dst[RIDX(0, 0, dim)] = current_pixel;

    i = 0;
    j = dim - 1;
    pcurrent_pixel->red =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].red + src[RIDX(i + 1, j, dim)].red +
                                src[RIDX(i, j - 1, dim)].red + src[RIDX(i + 1, j - 1, dim)].red)) /
                         4);
    pcurrent_pixel->green =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].green + src[RIDX(i + 1, j, dim)].green +
                                src[RIDX(i, j - 1, dim)].green + src[RIDX(i + 1, j - 1, dim)].green)) /
                         4);
    pcurrent_pixel->blue =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].blue + src[RIDX(i + 1, j, dim)].blue +
                                src[RIDX(i, j - 1, dim)].blue + src[RIDX(i + 1, j - 1, dim)].blue)) /
                         4);
    dst[RIDX(i, j, dim)] = current_pixel;

    i = dim - 1;
    j = 0;
    pcurrent_pixel->red =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].red + src[RIDX(i - 1, j, dim)].red +
                                src[RIDX(i, j + 1, dim)].red + src[RIDX(i - 1, j + 1, dim)].red)) /
                         4);
    pcurrent_pixel->green =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].green + src[RIDX(i - 1, j, dim)].green +
                                src[RIDX(i, j + 1, dim)].green + src[RIDX(i - 1, j + 1, dim)].green)) /
                         4);
    pcurrent_pixel->blue =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].blue + src[RIDX(i - 1, j, dim)].blue +
                                src[RIDX(i, j + 1, dim)].blue + src[RIDX(i - 1, j + 1, dim)].blue)) /
                         4);
    dst[RIDX(i, j, dim)] = current_pixel;

    i = dim - 1;
    j = dim - 1;
    pcurrent_pixel->red =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].red + src[RIDX(i - 1, j, dim)].red +
                                src[RIDX(i, j - 1, dim)].red + src[RIDX(i - 1, j - 1, dim)].red)) /
                         4);
    pcurrent_pixel->green =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].green + src[RIDX(i - 1, j, dim)].green +
                                src[RIDX(i, j - 1, dim)].green + src[RIDX(i - 1, j - 1, dim)].green)) /
                         4);
    pcurrent_pixel->blue =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].blue + src[RIDX(i - 1, j, dim)].blue +
                                src[RIDX(i, j - 1, dim)].blue + src[RIDX(i - 1, j - 1, dim)].blue)) /
                         4);
    dst[RIDX(i, j, dim)] = current_pixel;

    j = 0;
    for (i = 1; i < dim - 1; i++)
    {
        pcurrent_pixel->red =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].red + src[RIDX(i - 1, j + 1, dim)].red +
                                    src[RIDX(i, j, dim)].red + src[RIDX(i, j + 1, dim)].red +
                                    src[RIDX(i + 1, j, dim)].red + src[RIDX(i + 1, j + 1, dim)].red)) /
                             6);
        pcurrent_pixel->green =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].green + src[RIDX(i - 1, j + 1, dim)].green +
                                    src[RIDX(i, j, dim)].green + src[RIDX(i, j + 1, dim)].green +
                                    src[RIDX(i + 1, j, dim)].green + src[RIDX(i + 1, j + 1, dim)].green)) /
                             6);
        pcurrent_pixel->blue =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].blue + src[RIDX(i - 1, j + 1, dim)].blue +
                                    src[RIDX(i, j, dim)].blue + src[RIDX(i, j + 1, dim)].blue +
                                    src[RIDX(i + 1, j, dim)].blue + src[RIDX(i + 1, j + 1, dim)].blue)) /
                             6);
        dst[RIDX(i, j, dim)] = current_pixel;
    }

    i = dim - 1;
    for (j = 1; j < dim - 1; j++)
    {
        pcurrent_pixel->red =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].red + src[RIDX(i - 1, j, dim)].red +
                                    src[RIDX(i, j - 1, dim)].red + src[RIDX(i - 1, j - 1, dim)].red +
                                    src[RIDX(i, j + 1, dim)].red + src[RIDX(i - 1, j + 1, dim)].red)) /
                             6);
        pcurrent_pixel->green =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].green + src[RIDX(i - 1, j, dim)].green +
                                    src[RIDX(i, j - 1, dim)].green + src[RIDX(i - 1, j - 1, dim)].green +
                                    src[RIDX(i, j + 1, dim)].green + src[RIDX(i - 1, j + 1, dim)].green)) /
                             6);
        pcurrent_pixel->blue =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].blue + src[RIDX(i - 1, j, dim)].blue +
                                    src[RIDX(i, j - 1, dim)].blue + src[RIDX(i - 1, j - 1, dim)].blue +
                                    src[RIDX(i, j + 1, dim)].blue + src[RIDX(i - 1, j + 1, dim)].blue)) /
                             6);
        dst[RIDX(i, j, dim)] = current_pixel;
    }

    j = dim - 1;
    for (i = 1; i < dim - 1; i++)
    {
        pcurrent_pixel->red =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].red + src[RIDX(i - 1, j - 1, dim)].red +
                                    src[RIDX(i, j, dim)].red + src[RIDX(i, j - 1, dim)].red +
                                    src[RIDX(i + 1, j, dim)].red + src[RIDX(i + 1, j - 1, dim)].red)) /
                             6);
        pcurrent_pixel->green =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].green + src[RIDX(i - 1, j - 1, dim)].green +
                                    src[RIDX(i, j, dim)].green + src[RIDX(i, j - 1, dim)].green +
                                    src[RIDX(i + 1, j, dim)].green + src[RIDX(i + 1, j - 1, dim)].green)) /
                             6);
        pcurrent_pixel->blue =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].blue + src[RIDX(i - 1, j - 1, dim)].blue +
                                    src[RIDX(i, j, dim)].blue + src[RIDX(i, j - 1, dim)].blue +
                                    src[RIDX(i + 1, j, dim)].blue + src[RIDX(i + 1, j - 1, dim)].blue)) /
                             6);
        dst[RIDX(i, j, dim)] = current_pixel;
    }

    i = 0;
    for (j = 1; j < dim - 1; j++)
    {
        pcurrent_pixel->red =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].red + src[RIDX(i + 1, j, dim)].red +
                                    src[RIDX(i, j - 1, dim)].red + src[RIDX(i + 1, j - 1, dim)].red +
                                    src[RIDX(i, j + 1, dim)].red + src[RIDX(i + 1, j + 1, dim)].red)) /
                             6);
        pcurrent_pixel->green =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].green + src[RIDX(i + 1, j, dim)].green +
                                    src[RIDX(i, j - 1, dim)].green + src[RIDX(i + 1, j - 1, dim)].green +
                                    src[RIDX(i, j + 1, dim)].green + src[RIDX(i + 1, j + 1, dim)].green)) /
                             6);
        pcurrent_pixel->blue =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].blue + src[RIDX(i + 1, j, dim)].blue +
                                    src[RIDX(i, j - 1, dim)].blue + src[RIDX(i + 1, j - 1, dim)].blue +
                                    src[RIDX(i, j + 1, dim)].blue + src[RIDX(i + 1, j + 1, dim)].blue)) /
                             6);
        dst[RIDX(i, j, dim)] = current_pixel;
    }


    for (i = 1; i < dim - 1; i++)
    {
        for (j = 1; j < dim - 1; j++)
        {
            pcurrent_pixel->red =
                (unsigned short)(((int)(src[RIDX(i + 1, j, dim)].red + src[RIDX(i + 1, j - 1, dim)].red +
                                        src[RIDX(i, j, dim)].red + src[RIDX(i - 1, j, dim)].red +
                                        src[RIDX(i, j - 1, dim)].red + src[RIDX(i - 1, j - 1, dim)].red +
                                        src[RIDX(i, j + 1, dim)].red + src[RIDX(i - 1, j + 1, dim)].red +
                                        src[RIDX(i + 1, j + 1, dim)].red)) /
                                 9);
            pcurrent_pixel->green =
                (unsigned short)(((int)(src[RIDX(i + 1, j, dim)].green + src[RIDX(i + 1, j - 1, dim)].green +
                                        src[RIDX(i, j, dim)].green + src[RIDX(i - 1, j, dim)].green +
                                        src[RIDX(i, j - 1, dim)].green + src[RIDX(i - 1, j - 1, dim)].green +
                                        src[RIDX(i, j + 1, dim)].green + src[RIDX(i - 1, j + 1, dim)].green +
                                        src[RIDX(i + 1, j + 1, dim)].green)) /
                                 9);
            pcurrent_pixel->blue =
                (unsigned short)(((int)(src[RIDX(i + 1, j, dim)].blue + src[RIDX(i + 1, j - 1, dim)].blue +
                                        src[RIDX(i, j, dim)].blue + src[RIDX(i - 1, j, dim)].blue +
                                        src[RIDX(i, j - 1, dim)].blue + src[RIDX(i - 1, j - 1, dim)].blue +
                                        src[RIDX(i, j + 1, dim)].blue + src[RIDX(i - 1, j + 1, dim)].blue +
                                        src[RIDX(i + 1, j + 1, dim)].blue)) /
                                 9);
            dst[RIDX(i, j, dim)] = current_pixel;
        }
    }
}

/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

