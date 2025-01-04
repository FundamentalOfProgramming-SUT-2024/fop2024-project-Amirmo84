#ifndef MAINMENU
#define MAINMENU

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include "createNewUser.h"
#include "userLogin.h"

void print_menu(WINDOW *menu_win, int highlight, int height, int width);
void game_menu();
void profile_menu();
void score_table(int width, WINDOW *menu_win);

void mainMenu(){
    initscr();            
    clear();
    noecho();
    cbreak();
    start_color();
    curs_set(0);
    mvprintw(8, 10, "Welcome! I hope you enjoy this game!");        
    int startx = 0, starty = 0;
    int width, height;
    int highlight = 1;
    int choice = 0;
    int c;
    getmaxyx(stdscr, height, width);
    WINDOW *menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);
    refresh();
    char *choices[] = {
        "1. Create New User",
        "2. Log In",
        "3. Game Menu",
        "4. Profile Menu",
        "5. Score Table"
    };
    int n_choices = sizeof(choices) / sizeof(char *);

    while (1){
        curs_set(0);
        noecho();
        print_menu(menu_win, highlight, height, width);
        c = wgetch(menu_win);
        switch (c){
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    highlight--;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else
                    highlight++;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                break;
        }

        if (choice != 0) {
            switch (choice) {
                case 1:
                    clear();
                    create_new_user(menu_win);
                    break;
                case 2:
                    clear();
                    user_login();
                    break;
                case 3: 
                    clear();
                    game_menu();
                    break;
                case 4:
                    clear();
                    profile_menu();
                    break;
                case 5:
                    clear();
                    score_table(width, menu_win);
                    break;
                default:
                    break;
            }
            choice = 0;
        }
    }
    endwin();
}

void print_menu(WINDOW *menu_win, int highlight, int height, int width) {
    int x = width / 2, y = height / 2 - 2;
    box(menu_win, 0, 0);

    char *choices[] ={
        "1. Create New User",
        "2. Log In",
        "3. Game Menu",
        "4. Profile Menu",
        "5. Score Table"
    };
    int n_choices = sizeof(choices) / sizeof(char *);

    for (int i = 0; i < n_choices; i++) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        y++;
    }
    wrefresh(menu_win);
}

void profile_menu(){
    getch();
}

void game_menu(){
    getch();
}

void score_table(int width, WINDOW *menu_win){
    getch();
    clear();
    FILE *file = fopen("Counter.txt", "r");
    short num = fscanf(file, "%d", &num);
    short scores_sort[num], t, index = 0;
    fclose(file);
    file = fopen("scores.txt", "r");
    while (fscanf(file, "%d", &t) == 1)
        scores_sort[index++] = t;
    for (int i = 0; i < num; i++){
        for (int j = 0; j < num - 1; j++){
            if (scores_sort[j] < scores_sort[j + 1]){
                int temp = scores_sort[j];
                scores_sort[j] = scores_sort[j + 1];
                scores_sort[j + 1] = temp;
            }
        }
    }
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    for (int i = 0; i < num; i++){
        mvwprintw(menu_win, 2 * i + 2, width/2, "%d", scores_sort[i]);
        refresh();
    }
}

#endif