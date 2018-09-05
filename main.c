/* ---- main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Aug 18 16:32:20 2017
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
#include <stdio.h>
#include <ncurses.h>
#include "tetromino.h"
#include "highscore.h"
#include "game.h"

int main(int argc, char*argv[]) {
  
  int status=1;
  highscore_t *highscores;
  int userTime;
  char userName[11];
  printf("Enter a username max 10 characters long\n");
  scanf("%10s",userName);
  printf("Enter how long you would like the game to last in seconds this can be anywhere from between 1(60) to 5(300) minutes\n");
  scanf("%d", &userTime);
  if(userTime > 300 || userTime < 60){ userTime=90;}
 
  highscores = game(highscores, userTime, userName);
  endwin();
  return (0);
}
/* main.c ends here */
