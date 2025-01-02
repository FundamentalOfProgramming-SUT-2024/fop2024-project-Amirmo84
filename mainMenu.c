#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include "createNewUser.h"
#include "userLogin.h"

void print_menu(WINDOW *menu_win, int highlight);
void game_menu();
void profile_menu();
void score_table();

int main(){
    initscr();            
    clear();
    noecho();
    cbreak();
    start_color();
    curs_set(0);        

    int startx = 0, starty = 0;
    int width = 40, height = 10;
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
        print_menu(menu_win, highlight);
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
                    score_table();
                    break;
                default:
                    break;
            }
            choice = 0;
        }
    }
    endwin();
    return 0;
}

void print_menu(WINDOW *menu_win, int highlight) {
    int x = 75, y = 8;
    box(menu_win, 0, 0);

    char *choices[] = {
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
        } else {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
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

void score_table(){
    getch();
}