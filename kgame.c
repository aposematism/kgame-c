/**
 * NWEN 241 Programming Assignment 3
 * kgame.c C Source File
 *
 * Name: Jordan Milburn
 * Student ID: 300261689
 * This code can be compiled with the following command: gcc main.c kgame.c -lcurses -lm -o kgame
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include "kgame.h"

void kgame_init(kgame_t *game)
{
    if(!game) return;
    
    for(int i=0; i<KGAME_SIDES; i++) 
        for(int j=0; j<KGAME_SIDES; j++)
            game->board[i][j] = ' ';
    game->score = 0;
}

void kgame_add_random_tile(kgame_t *game)
{
	int row, col;
	
	if(!game) return;
    
    int space = 0;
    for(int i = 0; i < KGAME_SIDES; i++){
        for(int j = 0; j < KGAME_SIDES; j++){
            if(game->board[i][j] == ' '){
                space = 1;
            }
        }
    }
    
    if(space = 0){return;}
    
    for(int p = 0; p < 50; p++){
        row = rand() % 4;
        col = rand() % 4;
        if(game->board[row][col] == ' '){
            break;
        }
    }
    game->board[row][col] = 'A' + (rand() % 2);
}

/** 
/** Task 1 for rendering appropriately.
/** */
void kgame_render(char *output_buffer, const kgame_t *game)
{
    char temp[18*40] = "";
    char intermediary[18] = "+---+---+---+---+\n";
    strcat(temp, intermediary);
    for(int i=0; i<KGAME_SIDES; i++){
        for(int j=0; j<KGAME_SIDES; j++){
            char built[5];
            sprintf(built, "| %c ", game->board[i][j]);
            strcat(temp, built);
        }
    strcat(temp, "|\n");
    strcat(temp, intermediary);
    }
    char score[12];
    sprintf(score, "Score: %d", game->score);
    strcat(temp, score);
    sprintf(output_buffer, "%s", temp);
}

/** 
 Pretty clean cut. Checks for K, if it is there, then return true. Else return false.
 */
bool kgame_is_won(const kgame_t *game)
{
    //: Implement correctly (task 2)
    for(int i=0; i<KGAME_SIDES; i++){
        for(int j=0; j<KGAME_SIDES; j++){
            if(game->board[i][j] == 'K'){
                return true;
            }
        }
    }
    return false;
}

/** 
    This function checks for possible moves with the game. It returns false if there is no match possible.
 */
bool kgame_is_move_possible(const kgame_t *game)
{
    // Implement correctly (task 3)
     for(int i=0; i<KGAME_SIDES; i++){
        for(int j=0; j<KGAME_SIDES; j++){
            if(game->board[i][j] == game->board[i+1][j] && i+1 < KGAME_SIDES){
                return true;
            }
            if(game->board[i][j] == game->board[i][j+1] && j+1 < KGAME_SIDES){
                return true;
            }
            if(game->board[i][j] == game->board[i-1][j] && -1 < i-1 ){
                return true;
            }
            if(game->board[i][j] == game->board[i][j-1] && -1 < j-1){
                return true;
            }
        }
     }
    return false;
}

/** 
    This uses a bit of cunning trickery to make the game work appropriately.
    It firsts merges characters that can merge then it moves the characters.
    It then moves the items in its respective direction from there.
 */
bool kgame_update(kgame_t *game, dir_t direction)
{
    char tempArray[4][4];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            tempArray[i][j] = game->board[i][j];
        }
    }
    //To keep it consistent, I will use I and J to refer to row and column respectively in all loops.
    switch(direction){
        case UP://Top row should merge first, then each subsequent row down.
            for(int z = 0; z < 4; z++){
                for(int i = 0; i < KGAME_SIDES; i++){
                    for(int j=0; j < KGAME_SIDES; j++){
                        if(i+1 < KGAME_SIDES){//Do not go outside the boundaries of the array
                            if(game->board[i][j] == game->board[i+1][j] && game->board[i][j] != ' '){
                                double t = game->board[i][j] - 'A';
                                double s = pow(2, t);//this requires the use of -lm in GCC to compile appropriately.
                                game->score = game->score + (int) s*2;//score added
                                game->board[i][j]++;
                                game->board[i+1][j] = ' ';
                            }
                        }
                    }
                }
                //continue moving objects over empty space.
                for(int i = 0; i < KGAME_SIDES; i++){
                    for(int j=0; j< KGAME_SIDES; j++){
                        if(i+1 < KGAME_SIDES){//Do not go outside the boundaries of the array
                            if(game->board[i][j] != game->board[i+1][j] && game->board[i][j] == ' '){
                                game->board[i][j] = game->board[i+1][j];
                                game->board[i+1][j] = ' ';
                            }
                        }
                    }
                }
            }
            break;
            
        case DOWN://bottom row should merge first, then eachs subsequent row up.
            for(int z = 0; z < 4; z++){
                for(int i = KGAME_SIDES-1; -1 < i ; i--){
                    for(int j=0; j<KGAME_SIDES; j++){
                        if(-1 < i-1){//Do not go outside the boundaries of the array
                            if(game->board[i][j] == game->board[i-1][j] && game->board[i][j] != ' '){
                                double t = game->board[i][j] - 'A';
                                double s = pow(2, t);//this requires the use of -lm in GCC to compile appropriately.
                                game->score = game->score + (int) s*2;//score added
                                game->board[i][j]++;
                                game->board[i-1][j] = ' ';
                            }
                        }
                    }
                }
                //continue moving objects over empty space.
                for(int i = KGAME_SIDES-1; -1 < i ; i--){
                    for(int j=0; j < KGAME_SIDES; j++){
                        if(-1 < i-1){//Do not go outside the boundaries of the array
                            if(game->board[i][j] != game->board[i-1][j] && game->board[i][j] == ' '){
                                game->board[i][j] = game->board[i-1][j];
                                game->board[i-1][j] = ' ';
                            }
                        }
                    }
                }
            }
            break;
            
        case LEFT://most leftward row should go first, followed by each row to the right
            for(int z = 0; z < 4; z++){
                for(int i=0; i<KGAME_SIDES; i++){
                    for(int j=0; j<KGAME_SIDES; j++){
                        if(j+1 < KGAME_SIDES){//Do not go outside the boundaries of the array
                            if(game->board[i][j] == game->board[i][j+1] && game->board[i][j] != ' '){
                                double t = game->board[i][j] - 'A';
                                double s = pow(2, t);//this requires the use of -lm in GCC to compile appropriately.
                                game->score = game->score + (int) s*2;//score added
                                game->board[i][j]++;
                                game->board[i][j+1] = ' ';
                            }
                        }
                    }
                }
                //continue moving objects over empty space.
                for(int i=0; i<KGAME_SIDES; i++){
                    for(int j=0; j<KGAME_SIDES; j++){
                        if(j+1 < KGAME_SIDES){//Do not go outside the boundaries of the array
                            if(game->board[i][j] != game->board[i][j+1] && game->board[i][j] == ' '){
                                game->board[i][j] = game->board[i][j+1];
                                game->board[i][j+1] = ' ';
                            }
                        }
                    }
                }
            }
            break;
        
        case RIGHT://most rightward row should go first, followed by each row to the left.
            for(int z = 0; z < 4; z++){
                for(int i=0; i <KGAME_SIDES; i++){
                    for(int j = KGAME_SIDES-1; -1 < j ; j--){
                        if(-1 < j-1){//Do not go outside the boundaries of the array
                            if(game->board[i][j] == game->board[i][j-1] && game->board[i][j] != ' '){
                                double t = game->board[i][j] - 'A';
                                double s = pow(2, t);//this requires the use of -lm in GCC to compile appropriately.
                                game->score = game->score + (int) s*2;//score added
                                game->board[i][j]++;
                                game->board[i][j-1] = ' ';
                            }
                        }
                    }
                }
                //continue moving objects over empty space.
                for(int i=0; i<KGAME_SIDES; i++){
                    for(int j = KGAME_SIDES-1; -1 < j ; j--){
                        if(-1 < j-1){//Do not go outside the boundaries of the array
                            if(game->board[i][j] != game->board[i][j-1] && game->board[i][j] == ' '){
                                game->board[i][j] = game->board[i][j-1];
                                game->board[i][j-1] = ' ';
                            }
                        }
                    }
                }
            }
            break;
    }
    kgame_add_random_tile(game);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(game->board[i][j] != tempArray[i][j]){
                return true;
            }
        }
    }
    return false;
}

/** 
    This is the file saving method developed for this game.
 */
void kgame_save(const kgame_t *game)
{
    FILE *fp = fopen("KGAME_SAVE_FILE.txt", "w");
    char saveString[50];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            char selected = game->board[i][j];
            if(selected == ' '){
                    selected = '-';
            }
            int len = strlen(saveString);
            saveString[len] = selected;
            saveString[len+1] = '\0';//cheap trick to chuck a char into a string.
        }
    }
    char scoreString[30];
    sprintf(scoreString, " %d", game->score);
    strcat(saveString, scoreString);
    fprintf(fp, "%s", saveString);
    fclose(fp);
}

/** 
    This is the file loading method developed for this game.
 */
bool kgame_load(kgame_t *game)
{
    FILE *ft = fopen("KGAME_SAVE_FILE.txt", "r");
    if(ft ==  NULL){
        return false;
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            char selected;
            fscanf(ft, "%c", &selected);
            if(selected > 'K' && selected != '-' ){
                return false;
            }
            if(selected < 'A' && selected != '-'){
                return false;
            }
        }
    }
    int checkscore;
    fscanf(ft, " ");
    fscanf(ft, "%d", &checkscore);
    if(checkscore < 0 && isalpha(checkscore)){//checking for alphabet
        return false;
    }
    
    fclose(ft);
    FILE *fp = fopen("KGAME_SAVE_FILE.txt", "r");
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            char selected;
            fscanf(ft, "%c", &selected);
            if(selected == '-'){
                selected = ' ';
            }
            game->board[i][j] = selected;
        }
    }
    int loadedScore;
    fscanf(fp, " ");
    fscanf(fp, "%d", &loadedScore);
    game->score = loadedScore;
    return true;
}

/** 
    This code can be compiled with the following command: gcc main.c kgame.c -lcurses -lm -o kgame
 */


