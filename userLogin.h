#ifndef LOGIN
#define LOGIN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

int verify(const char *username, const char *password);
void user_login();
void guest_login();

void user_login(){
    char username[50], password[50];
    int valid = 0;
    while (!valid){
        clear();
        mvprintw(0, 75, "Login Menu");
        mvprintw(2, 75, "1. Log In");
        mvprintw(4, 75, "2. Log In as Guest");
        mvprintw(8, 75, "Enter your choice: ");
        refresh();
        char choice = getch();
        if (choice == '1'){
            while (1){
                clear();
                mvprintw(2, 75, "Enter Username: ");
                refresh();
                echo();
                curs_set(1);
                getstr(username);

                mvprintw(4, 75, "Enter Password: ");
                refresh();
                curs_set(0);
                noecho();
                getstr(password);

                if (verify(username, password)){
                    mvprintw(6, 75, "Login successful! Welcome, %s.", username);
                    valid = 1;
                    break;
                }
                else{
                    mvprintw(6, 70, "Error: Invalid username or password.");
                    mvprintw(7, 75, "Press any key to try again!");
                }
                refresh();
                getch();
            }
        }
        else if (choice == '2') {
            guest_login();
            valid = 1;
        }
        else{
            mvprintw(10, 75, "Invalid choice. Press any key to try again.");
            refresh();
            getch();
        }
    }
}

int verify(const char *username, const char *password){
    FILE *file = fopen("users.txt", "r");
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char saved_username[50], saved_password[50], saved_email[50];
        sscanf(line, "%[^,],%[^,],%s", saved_username, saved_password, saved_email);
        if (strcmp(saved_username, username) == 0 && strcmp(saved_password, password) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void guest_login(){
    clear();
    mvprintw(0, 75, "Logged in as Guest.");
    mvprintw(1, 75, "Welcome to the game!");
    mvprintw(3, 75, "Press any key to enjoy the game!");
    refresh();
    getch();
}

#endif