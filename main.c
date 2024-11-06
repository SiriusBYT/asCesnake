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

void screen();

void* tf_input(void* v_key) {
    char* p_key = (char*) v_key;
    while (true) {
        *p_key = getch();
    }
}

int main() {
    // Allow raw input
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // set up threads
    pthread_t thread_input;

    // Create input thread
    char key; char *p_key = &key;
    pthread_create(&thread_input, NULL, tf_input, p_key);



    int display[screen_height][screen_width] = {};
    for (int cycle=0; cycle < screen_height; cycle++) {
        for (int subcycle=0; subcycle < screen_width; subcycle++) {
            display[cycle][subcycle] = 0;
        }
    }

    int x_center = screen_width/2;
    int y_center = screen_height/2;
    display[y_center][x_center] = 1; // Set the middle of the screen to be the snake head

    // Game loop
    screen(0, display);
    while (true) {
        switch (key) {
            case 27: // ESC
            printw("Exiting...\n");
            refresh();
            endwin();
            return 0;
        default:
            // Input System
            display[y][x] = 0;
            switch (key) {
                case 4: // Left Arrow
                    (x-1 < 0) ? x=screen_width-1 : x--;
                    break;
                case 3: // Up Arrow
                    (y-1 < 0) ? y=screen_height-1 : y--;
                    break;
                case 5: // Right Arrow
                    (x+1 == screen_width) ? x=0 : x++;
                    break;
                case 2: // Down Arrow
                    (y+1 == screen_height) ? y=0 : y++;
                    break;

            }
            display[y][x] = 1;
            screen(key, display);
            break;
        }
    }
}

void screen(int key, int display[][screen_width]) {
    clear();
    printw("X: %d Y: %d \n", x, y);
    
    printw("Key pressed: %d\n", key);
    time_t timer = time(NULL);
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
            switch (display[cycle][subcycle]) {
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
