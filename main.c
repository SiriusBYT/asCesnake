#include <stdbool.h>	// Bool
#include <stdlib.h>	    // Files
#include <stdio.h>  	// STDLib
#include <ncurses.h>	// Input
#include <time.h>       // Time based stuff
#include <pthread.h>    // Multithreading
#include <unistd.h>     // Sleep

const unsigned int screen_width = 32;
const unsigned int screen_height = 9;

const unsigned char g_Air = 32; // " "
const unsigned char g_Head = 35; // "#"
// const unsigned char g_Head = 254; // "■"

// Variables
int x=screen_width/2, y=screen_height/2;
int score=0;

struct game_pointers {
    char* p_key;
    int* p_gamematrix;
    int* p_x;
    int* p_y;
    int* p_score;
};

void tf_video();
void* tf_input();

int main() {
    // Allow raw input
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // set up threads
    pthread_t thread_input;

    // Generate the Game Matrix's array
    int gamematrix[screen_height][screen_width] = {};
    for (int cycle=0; cycle < screen_height; cycle++) {
        for (int subcycle=0; subcycle < screen_width; subcycle++) {
            gamematrix[cycle][subcycle] = 0;
        }
    }

    char key; // redundant? since the inputs are entirely inside the input thread now
    int (*p_gamematrix)[screen_width] = gamematrix; // what the fuck am i looking at

    struct game_pointers p_struct = {&key, *p_gamematrix, &x, &y, &score};

    pthread_create(&thread_input, NULL, tf_input, &p_struct);

    int x_center = screen_width/2;
    int y_center = screen_height/2;
    gamematrix[y_center][x_center] = 1; // Set the middle of the screen to be the snake head

    // Game loop
    tf_video(0, gamematrix);
    while (true) {
        switch (key) {
            case 27: // ESC
            printw("Exiting...\n");
            refresh();
            endwin();
            return 0;
        default:
            tf_video(key, gamematrix);
            break;
        }
    }
}

void* tf_input(void* v_struct) {
    // recover the variables needed and transfer them to local pointers to avoid code looking retarded
    struct game_pointers* p_struct = (struct game_pointers*) v_struct;
    char* p_key = p_struct -> p_key;
    int* p_gamematrix = p_struct -> p_gamematrix;
    int* p_x = p_struct -> p_x;
    int* p_y = p_struct -> p_y;

    int memory_loc; // retardness incoming

    // Input System
    while (true) {
        *p_key = getch();

        // I AM FUCKING LOOSING BRAINCELLS
        memory_loc = (*p_y*screen_width)+*p_x;
        p_gamematrix[memory_loc] = 0;

        switch (*p_key) {
            case 3: // Up Arrow
                (y-1 < 0) ? y=screen_height-1 : y--;
                break;
            case 4: // Left Arrow
                (x-1 < 0) ? x=screen_width-1 : x--;
                break;

            case 2: // Down Arrow
                (y+1 == screen_height) ? y=0 : y++;
                break;
            case 5: // Right Arrow
                (x+1 == screen_width) ? x=0 : x++;
                break;
        }

        // let's hope this never causes a segfault lol
        memory_loc = (*p_y*screen_width)+*p_x;
        p_gamematrix[memory_loc] = 1; 
    }
}

void tf_video(int key, int gamematrix[][screen_width]) {
    clear();
    printw("X: %d Y: %d \n", x, y);
    
    printw("Key pressed: %d\n", key);
    time_t timer = time(NULL); // gotta find a better function than this pile of garbage for this
    printw("Time: %ds\n", timer);

    // This is fucking atrocious.
    char Border_Horizontal[screen_width+3];
    for (int cycle = 0; cycle < screen_width+2; cycle++) {
        Border_Horizontal[cycle] = '=';
    }
    Border_Horizontal[screen_width+2] = '\0';
    

    printw("%s\n", Border_Horizontal);
    for (int cycle=0; cycle < screen_height; cycle++) {
        printw("|");
        for (int subcycle=0; subcycle < screen_width; subcycle++) {
            switch (gamematrix[cycle][subcycle]) {
                case 0:
                    printw("%c", g_Air);
                    break;
                case 1:
                    printw("%c", g_Head);
                    break;
            }
        }
        printw("|\n\0");
    }
    printw("%s\n", Border_Horizontal);
    refresh();
}
