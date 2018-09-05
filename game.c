/* ---- game.c --- 
 * 
 * Filename: game.c
 * Description: This is the game state machine 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Tue Sep  6 11:08:59 2017
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
#include <unistd.h> 
#include <ncurses.h>
#include <time.h>
#include "highscore.h"
#include "game.h"
#include "well.h"
#include "tetris.h"
#include "tetromino.h"
#include "key.h"

void init_game(void) {
  int x,y;
}

highscore_t *game(highscore_t *highscores, int userTime, char *userName) {
  static int state = INIT;
  tetromino_t *next = NULL;
  tetromino_t *next2 = NULL;
  tetromino_t *current = NULL;
  tetromino_t *current2 = NULL;
  well_t *w;
  int x,y;
  int c;
  int status;
  int status2;
  int arrow;
  struct timespec tim = {0,1000000};  // Each execution of while(1) is approximately 1mS
  struct timespec tim_ret;
  int move_counter = 0;
  int move_timeout = BASE_FALL_RATE;
  int maximumWellWidth = WELL_WIDTH * 2;            
  int currentWellWidth = WELL_WIDTH;
  int originalWellWidth = WELL_WIDTH;
  int score = 0;
  int timer = 0;
  int count = 0;
  int countCeiling = 1000;
  int maxTime = userTime;
  int life = 2;
  int nextX;  int next2X; int nextY;  int next2Y;
  while(1) {
    switch(state) {
    case INIT:               // Initialize the game, only run one time 
      initscr();
//    start_color();
      nodelay(stdscr,TRUE);  // Do not wait for characters using getch.  
      noecho();              // Do not echo input characters 
      getmaxyx(stdscr,y,x);  // Get the screen dimensions 
      w = init_well(((x/2)-(WELL_WIDTH/2)),3,WELL_WIDTH,WELL_HEIGHT);
      draw_well(w);
      srand(time(NULL));     // Seed the random number generator with the time. Used in create tet. 
      display_score(score, w->upper_left_x-15,w->upper_left_y);
      state = ADD_PIECE;
	refresh();
      break;
    case ADD_PIECE:          // Add a new piece to the game
        
     if (next) {
	undisplay_tetromino(next);
        undisplay_tetromino(next2);
	current = next;
	current2 = next2;
	next = create_tetromino ((w->upper_left_x+(w->width/2)), w->upper_left_y);
	next2 = create_tetromino ((w->upper_left_x+(w->width/2)+10), w->upper_left_y);
	}
      else {
	current = create_tetromino ((w->upper_left_x+(w->width/2)), w->upper_left_y);
	current2 = create_tetromino ((w->upper_left_x+(w->width/2)+10), w->upper_left_y);
	next = create_tetromino ((w->upper_left_x+(w->width/2)), w->upper_left_y);
	next2 = create_tetromino ((w->upper_left_x+(w->width/2)+ 10), w->upper_left_y);
	nextX = next->upper_left_x;  nextY  = next->upper_left_y;
	next2X = next2->upper_left_x;  next2Y  = next2->upper_left_y;
      }
      status = move_tet(current,nextX,nextY+1);
      status2 = move_tet(current2,next2X,next2Y+1);
      nextX =  next->upper_left_x; nextY = next->upper_left_y; next2X = next2->upper_left_x; next2Y  = next2->upper_left_y;	
     state = MOVE_PIECE;
     display_tetromino(current);
     refresh();
       if(status == MOVE_FAILED || status2 == MOVE_FAILED){
             state = GAME_OVER;
       }

      break;
    case MOVE_PIECE:         // Move the current piece 
      if ((arrow = read_escape(&c)) != NOCHAR) {
	switch (arrow) {
	case UP:
if(status != MOVE_FAILED){
    undisplay_tetromino(current);
    rotate_ccw(current);
    display_tetromino(current);}
if(status2 != MOVE_FAILED){
    undisplay_tetromino(current2);
    rotate_ccw(current2);
    display_tetromino(current2);
}
	  //mvprintw(10,10,"UP            ");
	  break;
	case DOWN:
if(status != MOVE_FAILED){ 
   undisplay_tetromino(current);
    rotate_cw(current);
    display_tetromino(current);
}
if(status2 != MOVE_FAILED){
    undisplay_tetromino(current2);
    rotate_cw(current2);
    display_tetromino(current2);
}
	  //mvprintw(10,10,"DOWN          ");
	  break;
	case LEFT:
if(status != MOVE_FAILED){
	  undisplay_tetromino(current);
    move_tet(current,current->upper_left_x-1,current->upper_left_y);
    display_tetromino(current);    //mvprintw(10,10,"LEFT          ");
       }
if(status2 != MOVE_FAILED){
   undisplay_tetromino(current2);
    move_tet(current2,current2->upper_left_x-1,current2->upper_left_y);
    display_tetromino(current2);}  
	  break;
	case RIGHT:
if(status != MOVE_FAILED){
    undisplay_tetromino(current);
    move_tet(current,current->upper_left_x+1,current->upper_left_y);
    display_tetromino(current);}
	  //mvprintw(10,10,"RIGHT         ");
if(status2 != MOVE_FAILED){ 
   undisplay_tetromino(current2);
    move_tet(current2,current2->upper_left_x+1,current2->upper_left_y);
    display_tetromino(current2);}

	  break;
  case PLUS:
      //getmaxyx(stdscr,y,x);  // Get the screen dimensions
    currentWellWidth = 2  * w->width;
    if(!(currentWellWidth > maximumWellWidth)) {
      undisplay_score(score, w->upper_left_x-15,w->upper_left_y);
      undraw_well(w);
      w->width = currentWellWidth;
      w = changeWellSize(((x/2)-(w->width/2)),1,w->width,WELL_HEIGHT,w);     
      display_score(score, w->upper_left_x-15,w->upper_left_y); 
      draw_well(w);
    }
    break;

    case MINUS:
    //getmaxyx(stdscr,y,x);  // Get the screen dimensions
    currentWellWidth = w->width - (w->width * 0.07);
    if(currentWellWidth > originalWellWidth) {  
      undisplay_score(score, w->upper_left_x-15,w->upper_left_y);
      undraw_well(w);
      w->width = currentWellWidth;
      w = changeWellSize(((x/2)-(w->width/2)),1,w->width,WELL_HEIGHT,w);   
      display_score(score, w->upper_left_x-15,w->upper_left_y);
      draw_well(w);
          }
    break;
	
    case REGCHAR: 
	  mvprintw(10,10,"REGCHAR 0x%02x",c);
	  if (c == ' ') {
      move_timeout = DROP_RATE;
    }
      /* Display unknown user name*/
    if(c == 'u'){
 
	mvprintw(1,60,"%s", userName);
    }
    else if (c == 'q') {
	    state = GAME_OVER;
 	}
    /*Reset the well to original size*/
    else if (c == 'r'){
        undisplay_score(score, w->upper_left_x-15,w->upper_left_y);
        getmaxyx(stdscr,y,x);  // Get the screen dimensions
        currentWellWidth = originalWellWidth;
        undraw_well(w);
        w->width = originalWellWidth;
        w = init_well(((x/2)-(WELL_WIDTH/2)),3,WELL_WIDTH,WELL_HEIGHT);
        draw_well(w);
        undisplay_score(score, w->upper_left_x-15,w->upper_left_y);
        display_score(score, w->upper_left_x-15,w->upper_left_y);

	 }
           if(timer >= maxTime && life != 0){
		if (c == 'Y' || c == 'y' && timer >= maxTime && life != 0){
                timer = 0;
                life--;
                count = 0;
                }else{state = GAME_OVER ;}
}
    }	
    }

 if(timer >= maxTime && life != 0){
                mvprintw(10,30,"If you would like to use your life press 'Y'");
}
             mvprintw(1,20,"timer: %d",timer);
	  mvprintw(1,5,"lives: %d",life);
	if(timer !=  maxTime){
        if (count++ >= countCeiling){
        timer += 1;
        count = 0;
        }}
        
       

	if (move_counter++ >= move_timeout && timer < maxTime) {
       if(status != MOVE_FAILED){
	undisplay_tetromino(current);
        status = move_tet(current,current->upper_left_x,current->upper_left_y+1);
        display_tetromino(current);}
       if(status2 != MOVE_FAILED){
	undisplay_tetromino(current2);
        status2 = move_tet(current2,current2->upper_left_x,current2->upper_left_y+1);
        display_tetromino(current2);}
        if(status == MOVE_FAILED && status2 == MOVE_FAILED){
          state = ADD_PIECE;
          move_timeout = BASE_FALL_RATE - (rand() % 301);
	
        }
	move_counter = 0;
      }
          
	
	undisplay_tetromino(next);
        move_tet(next ,50,40);
        display_tetromino(next);

        undisplay_tetromino(next2);
        move_tet(next2 ,70,40);
        display_tetromino(next2);
      
	undisplay_tetromino(current);
	
	display_tetromino(current);
	refresh();
	
	

       

 
      break;
    case GAME_OVER:
        nodelay(stdscr,FALSE);
        clear();
        getmaxyx(stdscr,y,x);
        mvprintw(1,x/2-5,"  GAME_OVER  ");
        mvprintw(2,x/2-5,"Score:  %d", score);
	highscores = load_scores("highscores.txt" );
	highscores = insert_score(highscores, userName, score);
	store_scores("highscores.txt" , highscores);
	print_score_list (highscores, 120, 20, 10);
        mvprintw(16,x/2-5,"Hit q to exit");
        getch(); // Wait for a key to be pressed.
        state = EXIT;
        break;

    case EXIT:
      return(highscores);  // Return the highscore structure back to main to be stored on disk. 
      //endwin();
      //return(0);
      break;
    }
    undraw_well(w);
    undisplay_score(score, w->upper_left_x-15,w->upper_left_y);
    score = compute_score(score , prune_well(w));
    draw_well(w);
    display_score(score, w->upper_left_x-15,w->upper_left_y); 
    refresh();
    nanosleep(&tim,&tim_ret);
  }
}

/* game.c ends here */
