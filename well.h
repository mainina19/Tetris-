/* ---- well.h --- 
 * 
 * Filename: well.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Tue Sep  6 14:10:53 2017
 * Last-Updated: Mon Feb 12 11:28:59 2018
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* Copyright (c) 2016-18 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

typedef struct well {
  int upper_left_x;
  int upper_left_y;
  int width;
  int height;
  char draw_char;
  char color[3];
} well_t;

well_t *init_well(int, int, int, int);
void draw_well(well_t *);
int prune_well(well_t * well);
well_t *changeWellSize(int upper_left_x, int upper_left_y, int width, int height, well_t *);

void undraw_well(well_t *);

/* well.h ends here */