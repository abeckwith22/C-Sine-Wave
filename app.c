#include <stdio.h>
// #include <stdlib.h> // required by atoi (don't need it right now)
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <sys/ioctl.h>
#include <unistd.h>

// used to decorate sine wave with colors.
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void sine_func(int width, int height);
int get_sine(double value, int scale, double strength);
int get_terminal_size(int *width, int *height);
int get_terminal_width(void);

int main(int argc, char** argv){
    int width;
    int height;
    get_terminal_size(&width, &height);

    printf("Width (cols): %d, Height (rows): %d", width, height);

    sine_func(width-2, height);
    // get_tables();
    return 0;
}

/* returns list of terminal width and height */
int get_terminal_size(int *p1, int *p2){
    struct winsize w;

    // use ioctl to get terminal size
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl");
        return 1;
    }

    // set width and height
    *p1 = w.ws_col;
    *p2 = w.ws_row;

    // printf("Width: %d, Height: %d\n", w.ws_col, w.ws_row);
    return 0;
}

int get_terminal_width() {
    struct winsize w;

    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl");
        return 1;
    }

    return w.ws_col;
}

int get_sine(double value, int scale, double strength){
    // strength = strength/1000;
    return ((sin(value/strength)+1)*scale)/2;
}

void sine_func(int width, int height){

    bool drawing = true;
    bool collision = false;
    bool bright_up = false;
    // bool strength_up = false;
    double wave_strength = 100;
    int slowdown = 2500;
    int i, j;
    double min = 10, max = 10;
    double line_width = 25;
    // char* line = ".";
    char* space = " ";

    char* colors[7] = {ANSI_COLOR_RED, ANSI_COLOR_YELLOW, ANSI_COLOR_GREEN, ANSI_COLOR_BLUE, ANSI_COLOR_CYAN, ANSI_COLOR_MAGENTA, ANSI_COLOR_RESET};
    char brightness[13] = {' ', '.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@'};
    int bright_idx = 0;
    int color_idx = 0;

    for(i = 0;;i++){
        printf("|");
        width = get_terminal_width()-2;

        for(j=0; j < width; j++){ // draws along the width of the screen
            int s = get_sine((double)i, width, wave_strength);
            if((j+line_width>s && j-line_width<s)){
                printf("%c%s", brightness[bright_idx], colors[color_idx]);
                // printf(""ANSI_COLOR_RESET);
            }
            else
                printf("%s", space);
        }

        // changes color of wave
        if(i%50==0){
            if(color_idx < 6){
                color_idx++;
            }else{
                color_idx=0;
            }
        }

        // changes wave strength
        // NOTE: Changing the wave strength gradually doesn't work as intended because I'm only printing characters to a terminal window and drawing a newline.
        //       There isn't any magic that printf can do to redraw the lines after they've already been drawn (at least as far as I know).
        //       So I'm just leaving it as is.

        // if(i%200==0){
        //     if(wave_strength <= 25)
        //         strength_up=true;
        //     if(wave_strength >= 200)
        //         strength_up=false;

        //     if(strength_up){
        //         wave_strength+=1;
        //     }else {
        //         wave_strength-=1;
        //     }
        // }

        // changes brightness of wave
        if(i%20==0){
            if(bright_idx > 10){
                bright_up=false;
            }
            if(bright_idx <= 7){
                bright_up=true;
            }

            if(bright_up){
                bright_idx++;
            }
            else {
                bright_idx--;
            }
        }
        usleep(slowdown);
        printf("|");
        printf("\n");
    }
}
