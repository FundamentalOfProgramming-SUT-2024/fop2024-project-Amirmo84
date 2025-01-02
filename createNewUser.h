#ifndef NEWUSER
#define NEWUSER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>

int is_username_taken(const char *username);
int validate_password(const char *password);
int validate_email(const char *email);
void save_user(const char *username, const char *password, const char *email);
void create_new_user(WINDOW *menu) {
    char username[50], password[50], email[50];
    int valid = 0;
    curs_set(1);
    echo();
    while (1){
        clear();
        mvprintw(0, 75, "Create New User Menu");
        mvprintw(2, 75, "Enter Username: ");
        refresh();
        getstr(username);
        if (is_username_taken(username)) {
            mvprintw(4, 75, "Error: Username already exists.");
            mvprintw(5, 75, "Press any key to try again!");
            refresh();
            getch();
            continue;
        }

        while (!valid) {
            curs_set(0);
            noecho();
            mvprintw(4, 75, "Enter Password: "); 
            refresh();
            getstr(password);
            if (validate_password(password))
                valid = 1;
            else{
                mvprintw(5, 75, "Error: Password must be at least 7 characters and include:");
                mvprintw(6, 75, "At least one uppercase letter");
                mvprintw(7, 75, "At least one lowercase letter");
                mvprintw(8, 75, "At least one digit");
                attr_on(A_UNDERLINE, NULL);
                mvprintw(9, 75, "Press any key to try again!");
                refresh();
                curs_set(1);
                getch();
                attr_off(A_UNDERLINE, NULL);
                mvprintw(9, 75, "Press any key to try again!");
                refresh();
            }
        }
        valid = 0;
        wmove(menu, 5, 75);
        wclrtobot(menu);
        wrefresh(menu);
        while (!valid){
            wmove(menu, 6, 0);
            wclrtoeol(menu);
            wrefresh(menu);
            mvprintw(6, 75, "Enter Email: ");
            curs_set(1);
            echo();
            refresh();
            getstr(email);
            if (validate_email(email))
                valid = 1;
            else{
                mvprintw(8, 75, "Error: Invalid email format (ex: xxx@y.zzz)");
                attr_on(A_UNDERLINE, NULL);
                mvprintw(9, 75, "Press any key to try again!");
                refresh();
                getch();
                attr_off(A_UNDERLINE, NULL);
                mvprintw(9, 75, "Press any key to try again!");
                refresh();
            }
        }
        clear();
        save_user(username, password, email);
        mvprintw(7, 75, "User created successfully!");
        mvprintw(8, 63, "Press any key to return to the main menu to enjoy the game!");
        refresh();
        getch();
        break;
    }
}

int is_username_taken(const char *username) {
    FILE *file = fopen("users.txt", "r");
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char saved_username[50];
        sscanf(line, "%[^,]", saved_username);
        if (strcmp(saved_username, username) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int validate_password(const char *password) {
    int has_upper = 0, has_lower = 0, has_digit = 0;
    if (strlen(password) < 7)
        return 0;
    for (int i = 0; password[i] != '\0'; i++) {
        if (isupper(password[i]))
            has_upper = 1;
        if (islower(password[i]))
            has_lower = 1;
        if (isdigit(password[i]))
            has_digit = 1;
    }
    return has_upper && has_lower && has_digit;
}

int validate_email(const char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return (at && dot && at < dot);
}

void save_user(const char *username, const char *password, const char *email) {
    FILE *file = fopen("users.txt", "a");
    fprintf(file, "%s,%s,%s\n", username, password, email);
    fclose(file);
}

#endif
