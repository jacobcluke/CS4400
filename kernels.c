/*******************************************
 * Solutions for the CS:APP Performance Lab
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Jacob C. Luke",     /* Full name */
  "jacobcluke@gmail.com",  /* Email address */
};

/***************
 * COMPLEX KERNEL
 ***************/

/******************************************************
 * Your different versions of the complex kernel go here
 ******************************************************/

/* 
 * naive_complex - The naive baseline version of complex 
 */
char naive_complex_descr[] = "naive_complex: Naive baseline implementation";
void naive_complex(int dim, pixel *src, pixel *dest)
{
  int i, j;

  for(i = 0; i < dim; i++)
    for(j = 0; j < dim; j++)
    {

      dest[RIDX(dim - j - 1, dim - i - 1, dim)].red = ((int)src[RIDX(i, j, dim)].red +
						      (int)src[RIDX(i, j, dim)].green +
						      (int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].green = ((int)src[RIDX(i, j, dim)].red +
							(int)src[RIDX(i, j, dim)].green +
							(int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].blue = ((int)src[RIDX(i, j, dim)].red +
						       (int)src[RIDX(i, j, dim)].green +
						       (int)src[RIDX(i, j, dim)].blue) / 3;

    }
}

/*
 * smartCache_complex - improves upon the naive implementation by making less calls to memory
 */
char smartCache_complex_descr[] = "smartCache_complex: regular";
void smartCache_complex(int dim, pixel *src, pixel *dest)
{
  int i, j, ii, jj;
  int color;
  pixel p;
  int itemp1, itemp2, jtemp1, jtemp2;
  itemp1 = jtemp1 = 0;
  int area = dim* dim;

  for(i = 0; i < dim; i += 8){
    itemp1 = i * dim;
    for(j = 0; j < dim; j += 8){
      itemp2 = itemp1;
      jtemp1 = j * dim;
      for(ii = i; ii < i + 8; ii++){
        jtemp2 = jtemp1;
        for(jj = j; jj < j + 8; jj++){
          p = src[itemp2 + jj];

          color = (p.red + p.green + p.blue)/3;
          p.red = p.blue = p.green = color;

          dest[area -jtemp2 - ii - 1] = p;
          jtemp2+=dim;
        }
        itemp2 += dim;
      }
    }
  }
}
/*
 * complex - Your current working version of complex
 * IMPORTANT: This is the version you will be graded on
 */
char complex_descr[] = "complex: Current working version";
void complex(int dim, pixel *src, pixel *dest)
{
  smartCache_complex(dim, src, dest);
}

/*********************************************************************
 * register_complex_functions - Register all of your different versions
 *     of the complex kernel with the driver by calling the
 *     add_complex_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_complex_functions() {
  add_complex_function(&smartCache_complex, smartCache_complex_descr);
  add_complex_function(&naive_complex, naive_complex_descr);
}


/***************
 * MOTION KERNEL
 **************/

/***************************************************************
 * Various helper functions for the motion kernel
 * You may modify these or add new ones any way you like.
 **************************************************************/
typedef struct{
  int red;
  int green;
  int blue;
}block;

/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int num_neighbors = 0;
  for(ii=0; ii < 3; ii++)
    for(jj=0; jj < 3; jj++) 
      if ((i + ii < dim) && (j + jj < dim)) 
      {
	num_neighbors++;
	red += (int) src[RIDX(i+ii,j+jj,dim)].red;
	green += (int) src[RIDX(i+ii,j+jj,dim)].green;
	blue += (int) src[RIDX(i+ii,j+jj,dim)].blue;
      }
  
  current_pixel.red = (unsigned short) (red / num_neighbors);
  current_pixel.green = (unsigned short) (green / num_neighbors);
  current_pixel.blue = (unsigned short) (blue / num_neighbors);
  
  return current_pixel;
}

/*
 * Does last two rows for block motion
 */
static void set_last_two_rows(int dim, pixel *src, pixel *dst){
  int j;
  block blocks[3];
  block tempBlock;
  pixel tempPixel;
  pixel p1, p2;
  int size = dim*dim;

  // 2nd to last row
  for(j = 1; j < 3; j++){
    p1 = src[size - dim - dim + j - 1];
    p2 = src[size - dim + j - 1];

    tempBlock.red = p1.red + p2.red;
    tempBlock.green = p1.green + p2.green;
    tempBlock.blue = p1.blue + p2.blue;
    blocks[j] = tempBlock;
  }

  for(j = 0; j < dim - 2; j++){
    blocks[0] = blocks[1];
    blocks[1] = blocks[2];

    p1 = src[size - dim - dim + j + 2];
    p2 = src[size - dim + j + 2];

    tempBlock.red = p1.red + p2.red;
    tempBlock.green = p1.green + p2.green;
    tempBlock.blue = p1.blue + p2.blue;
    blocks[2] = tempBlock;

    tempPixel.red = (blocks[0].red + blocks[1].red + blocks[2].red)/6;
    tempPixel.green = (blocks[0].green + blocks[1].green + blocks[2].green)/6;
    tempPixel.blue = (blocks[0].blue + blocks[1].blue + blocks[2].blue)/6;
    dst[size - dim - dim + j] = tempPixel;
  }

  tempPixel.red = (blocks[1].red + blocks[2].red)/4;
  tempPixel.green = (blocks[1].green + blocks[2].green)/4;
  tempPixel.blue = (blocks[1].blue + blocks[2].blue)/4;
  dst[size - dim - 2] = tempPixel;

  tempPixel.red = blocks[2].red/2;
  tempPixel.green = blocks[2].green/2;
  tempPixel.blue = blocks[2].blue/2;
  dst[size - dim - 1] = tempPixel;

  // Last row
  for(j = 1; j < 3; j++){
    p1 = src[size - dim + j - 1];
    tempBlock.red = p1.red;
    tempBlock.blue = p1.blue;
    tempBlock.green = p1.green;
    blocks[j] = tempBlock;
  }
  for(j = 0; j < dim - 2; j++){
    blocks[0] = blocks[1];
    blocks[1] = blocks[2];

    p2 = src[size - dim + j + 2];

    tempBlock.red = p2.red;
    tempBlock.green = p2.green;
    tempBlock.blue = p2.blue;
    blocks[2] = tempBlock;

    tempPixel.red = (blocks[0].red + blocks[1].red + blocks[2].red)/3;
    tempPixel.green = (blocks[0].green + blocks[1].green + blocks[2].green)/3;
    tempPixel.blue = (blocks[0].blue + blocks[1].blue + blocks[2].blue)/3;
    dst[size - dim + j] = tempPixel;
  }

  tempPixel.red = (blocks[1].red + blocks[2].red)/2;
  tempPixel.green = (blocks[1].green + blocks[2].green)/2;
  tempPixel.blue = (blocks[1].blue + blocks[2].blue)/2;
  dst[size - 2] = tempPixel;

  tempPixel.red = blocks[2].red;
  tempPixel.green = blocks[2].green;
  tempPixel.blue = blocks[2].blue;
  dst[size - 1] = tempPixel;
}

/******************************************************
 * Your different versions of the motion kernel go here
 ******************************************************/


/*
 * block_motion - Sets the correct pixel values for row i of *dst
 */
char block_motion_descr[] = "block_motion: Less redundancy with adding up sums from src";
void block_motion(int dim, pixel *src, pixel *dst){

  int i,j;
  block blocks[3];
  block tempBlock;
  pixel tempPixel;
  pixel p1, p2, p3;
  int index = 0;

  for(i = 0; i < dim - 2; i++){

    // Initialize first 3 blocks
    for(j = 1; j < 3; j++){
      p1 = src[index + j  - 1];
      p2 = src[index + dim + j - 1];
      p3 = src[index + dim + dim + j - 1];

      tempBlock.red = p1.red + p2.red + p3.red;
      tempBlock.green = p1.green + p2.green + p3.green;
      tempBlock.blue = p1.blue + p2.blue + p3.blue;

      blocks[j] = tempBlock;
    }

    // Go through the rest of the row, cycling through different blocks
    for(j = 0; j < dim - 2; j++){
      blocks[0] = blocks[1];
      blocks[1] = blocks[2];

      p1 = src[index + j + 2];
      p2 = src[index + dim + j + 2];
      p3 = src[index + dim + dim + j + 2];

      tempBlock.red = p1.red + p2.red + p3.red;
      tempBlock.green = p1.green + p2.green + p3.green;
      tempBlock.blue = p1.blue + p2.blue + p3.blue;
      blocks[2] = tempBlock;

      tempPixel.red = (blocks[0].red + blocks[1].red + blocks[2].red)/9;
      tempPixel.green = (blocks[0].green + blocks[1].green + blocks[2].green)/9;
      tempPixel.blue = (blocks[0].blue + blocks[1].blue + blocks[2].blue)/9;

      dst[index + j] = tempPixel;
    }

    // Set the last two columns
    tempPixel.red = (blocks[1].red + blocks[2].red)/6;
    tempPixel.green = (blocks[1].green + blocks[2].green)/6;
    tempPixel.blue = (blocks[1].blue + blocks[2].blue)/6;
    dst[index + dim - 2] = tempPixel;

    tempPixel.red = blocks[2].red/3;
    tempPixel.green = blocks[2].green/3;
    tempPixel.blue = blocks[2].blue/3;
    dst[index + dim - 1] = tempPixel;

    index += dim;
  }

  set_last_two_rows(dim, src, dst);
}

/*
 * naive_motion - The naive baseline version of motion
 */
char naive_motion_descr[] = "naive_motion: Naive baseline implementation";
void naive_motion(int dim, pixel *src, pixel *dst)
{
  int i, j;

  for (i = 0; i < dim; i++)
    for(j = 0; j < dim; j++)
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
}

/*
 * motion - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion: Current working version";
void motion(int dim, pixel *src, pixel *dst) 
{
  block_motion(dim, src, dst);
}

/********************************************************************* 
 * register_motion_functions - Register all of your different versions
 *     of the motion kernel with the driver by calling the
 *     add_motion_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_motion_functions() {
  add_motion_function(&block_motion, block_motion_descr);
  add_motion_function(&naive_motion, naive_motion_descr);
}
