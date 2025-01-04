#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

typedef struct{
    int x; // top left
    int y; //top left
    int height;
    int width;
} Rooms;

int collision(Rooms room1, Rooms room2){
    return !(room1.x + room1.width < room2.x || room2.x + room2.width < room1.x || room1.y + room1.height < room2.y || room2.y + room2.height < room1.y);
}

void create_rooms(Rooms rooms[], int rooms_count, int terminal_x, int terminal_y){
    for (int i = 0; i < rooms_count; i++){
        Rooms new;
        while (1){
            int flag = 1;
            new.width = rand() % 4 + 6;
            new.height = rand() % 4 + 6;
            new.x = rand() % (terminal_x - new.width);
            new.y = rand() % (terminal_y - new.height);
            for (int j = 0; j < i; j++){
                if (collision(new, rooms[j])){
                    flag = 0;
                    break;
                }
            }
            if (flag)
                break;
        }
        rooms[i] = new;
    }
}

void door(Rooms room){
    int door1 = rand() % 3;
    int door2 = rand() % 3;
    if(door1 == door2)
        door2 = 3;
    int pos;
    switch (door1) {
        case 0:
            pos = rand() % room.width;
            mvprintw(room.y, room.x + pos, "+");
            break;
        case 1:
            pos = rand() % room.height;
            mvprintw(room.y + pos, room.x + room.width, "+");
            break;
        case 2:
            pos = rand() % room.width;
            mvprintw(room.y + room.height, room.x + pos, "+");
            break;
        default:
            break;
    }
    switch (door2){
        case 0:
            pos = rand() % room.width;
            mvprintw(room.y, room.x + pos, "+");
            break;
        case 1:
            pos = rand() % room.height;
            mvprintw(room.y + pos, room.x + room.width, "+");
            break;
        case 2:
            pos = rand() % room.width;
            mvprintw(room.y + room.height, room.x + pos, "+");
            break;
        default:
            break;
    }
}
void obsticle(Rooms room){
    int x ,y;
    x = rand() % room.width;
    y = rand() % room.height;
    while (x == 0)
        x = rand() % room.width;
    while (y==0)
        y = rand() % room.height;
    x = room.x + x;
    y = room.y + y;
    mvprintw(y, x, "O");
}

void printt(Rooms room){
    for (int i = 0; i < room.width; i++){
        mvprintw(room.y, room.x + i, "_");
        mvprintw(room.y + room.height, room.x + i, "_");
    }
    for (int i = 0; i < room.height; i++){
        mvprintw(room.y + i + 1, room.x, "|");
        mvprintw(room.y + i + 1, room.x + room.width, "|");
    }
    for (int i = 1; i < room.width; i++){
        for(int j = 1 ; j < room.height ; j++)
            mvprintw(room.y + j,room.x + i,".");
    }
    obsticle(room);
    door(room);
}

int main(){
    srand(time(NULL));
    initscr();
    curs_set(0);
    noecho();
    int rooms_count = 6 + (rand() % 3);
    int terminal_x, terminal_y;
    getmaxyx(stdscr, terminal_y, terminal_x);
    Rooms rooms[rooms_count];
    create_rooms(rooms, rooms_count, terminal_x - 5, terminal_y - 5);
    for(int i = 0 ; i < rooms_count ; i++){
        printt(rooms[i]);
        refresh();
    }
    getch();
    endwin();
    return 0;
}