#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>
//reverse

//global
#define HEIGHT 30
#define WIDTH 120
#define MAX_ROOM 10
#define ROOM_COUNTER 6
#define MAX_WEAPONS 5


typedef struct{
    char username[50];
    int score;
    int gold;
    int games_played;
    time_t experience_time;
} User;

typedef struct{
    int x; // top left
    int y; //top left
    int height;
    int width;
    int type;
} Rooms;
Rooms rooms[MAX_ROOM][ROOM_COUNTER];
int rooms_counter[MAX_ROOM];

// typedef struct {
//     //char map[max_x][max_y];
//     int player_row;
//     int player_col;
// } Floor;

// typedef struct {
//     int row;
//     int col;
//     int health;
//     int ancientkeys;
//     int brokenkeys;
// } Player;

typedef struct {
    int x, y;
    char * name;
    int state;
} Stuff;
Stuff elems[100];
int elem_counter = 0;

typedef struct{
    int x, y;
    int state;
} Trap;
Trap traps[50];
int trap_counter = 0;

typedef struct {
    int x, y;
    int health;
    char type;
    int num;
    int state;
    int movement_num;
    int movement_state;
} Monsters;
Monsters monsters[30];
int monster_counter = 0;

typedef struct{
    int x, y;
    char * name;
    int color;
    int state;
    int fresh;
} Food;
Food foods[100];
int foods_counter = 0;
int hunger = 100;
bool magicalFood = FALSE;
int timeFood = 50;

typedef struct {
    int x, y;
    int type;
    int state;
} Gold;
Gold golds[100];
int gold = 100;
int gold_counter = 0;

typedef struct {
    int x, y;
    int type;
    int state;
    char * name;
} Potion;
Potion potions[50];
int potions_counter = 0;
int potion_time_track = 0;
int Time_out = 100;

typedef struct {
    int x, y;
    int state;
} lockedDoor;
lockedDoor locked[50];
int locked_counter = 0;

typedef struct{
    int x, y;
    int state;
} secret_door;
secret_door secrets [50];
int secrets_counter = 0;

typedef struct {
    int x, y;
    char symbol;
    int state;
    int num_collect;
    int thrown;
} Weapon;
int weapon_counter = 0;
Weapon weapons[MAX_WEAPONS];
Weapon *weapInUse; //Dynamic
int wield_choice = 1;
int weapSpeed = 1;

//playerstuff
char main_username[50];
int characterColor = 10;
int score = 100;
bool master_key [5] = {};
bool first_key [5] = {TRUE,TRUE,TRUE,TRUE,TRUE};
bool master_keys_broken [5] = {};
int hits = 0;
int health = 100;
time_t last_health_update = 0;
time_t last_hunger_update = 0;
int current_floor = 1;
int g_state = 0;
int speed = 1;
char last_direction [3];
bool long_range_weapon = FALSE;

char password[5];
time_t password_show_time = 0;

int max_x, max_y;
void getmaxes(){
    getmaxyx(stdscr, max_y, max_x);
}

char map[HEIGHT][WIDTH];
char real_map[HEIGHT][WIDTH];
bool is_visible[HEIGHT][WIDTH];

void mainMenu();
void gameMenu();
void settingsMenu();
void Login();
void userLogin();
void star();

//all the menu options now will be chosen with this function
void choosing(char *choices[], int highlight, const char *menu_name, int num_choices){
    attron(COLOR_PAIR(6));
    attron(A_BOLD);
    mvprintw(0, max_x / 2 - strlen(menu_name) / 2, "%s", menu_name);
    attroff(A_BOLD);
    attroff(COLOR_PAIR(6));
    for (int i = 0; i < num_choices; i++){
        if (highlight == i){
            attron(A_REVERSE);
            mvprintw(max_y / 2 - num_choices + i * 2, max_x / 2 - strlen(choices[i]) / 2, "%d. %s\n", i + 1, choices[i]);
            attroff(A_REVERSE);
        }
        else
            mvprintw(max_y / 2 - num_choices + i * 2, max_x / 2 - strlen(choices[i]) / 2, "%d. %s\n", i + 1, choices[i]);
    }
    refresh();
}

//handling echo or noecho for user information
void inputt(char *var, int len, int flag){
    echo();
    int c, i = 0;
    if (flag == 0)
        noecho();
    c = getch();
    while (c != 10 && i < len){
        var[i] = (char) c;
        i++;
        if (flag == 0)
            printw("*");
        refresh();
        c = getch();
    }
    var[i] = '\0';
    noecho();
}
//Game
void generateMap();
void win_game();
void lose_game();
void printWhole();
bool collision(Rooms room1, Rooms room2);
void init_map ();
void addRooms (Rooms room);
void doorrrr(Rooms room);
void pillar (Rooms room);
void add_monster (Rooms room);
void add_trap (Rooms room);
void corridor (int x1, int y1, int x2, int y2);
void add_stairs (Rooms room);
void show_door (int ny, int nx);
void health_bar (int health);
void lose_health (int count);
void hunger_bar (int hunger);
void show_trap (int ny, int nx);
void add_hidden_door (Rooms room );
void add_master_key (Rooms room);
void generate_door_pass (char *password);
void show_password(int px, int py);
int LockedPasswordGet(int px, int py);
void locked_door (Rooms room);
void pick_up (int y, int x);
void show_corridor (int wx, int wy);
void show_room(Rooms room);
Rooms roomFinder (int wx, int wy);
void player_in_room (int px, int py, Rooms rooms[], int room_count) ;
void monster_attack (Monsters monster);
void while_inside_room (int px, int py, Rooms room);
Monsters monster_in_room (int px, int py);
bool monster_check (int x, int y, Monsters monster);
void monster_move(int px, int py, Monsters *m);
char *food_menu (int color);
void drop_weapon (int x, int y, Weapon * weapon);
void a_command (int px, int py);
void dagger_wand_arrow_attack (int px, int py, char * direction, int type);
void monster_health_check (Monsters * monster);
void player_attack (int mx, int my, char type);
void food_choice (char * name);
void food_window ();
void weapon_in_bag ();
void weapon_choice (char symbol);
void weapon_window();
void potion_choice (int type);
void potion_window ();
void inventory_window();
void p_command ();
void i_command ();
void E_command ();
void add_weapon (Rooms room);
void picked_tresure ();
void add_treasure (Rooms room);
void hunger_update ();
void health_update (int maybe); //TODO: make it better
void add_gold (Rooms room);
void elixir_of_everlife ();
void dragon_blood ();
void storm_kiss ();
void messages(char *what_happened, int maybe);
void add_potion (Rooms room);
void spoil_normal ();
void add_food (Rooms room);
void generateMap ();
void win_game();
void lose_game();
int choose_color(char name, int x, int y);

int choose_color (char tile, int x, int y) {
    if (tile == 'f') {
        for ( int i = 0; i < foods_counter; i ++) {
            if (foods[i].x == x && foods[i].y == y)
                return foods[i].color;
        }
    }
    else if (tile == 'D' || tile == 'F' || tile == 'G' || tile == 'S' || tile == 'U')
        return 2;
    else if (tile == '@') {
        for (int j = 0; j < locked_counter; j ++) {
            if (locked[j].x == x && locked[j].y == y) {
                if (locked[j].state == 0)
                    return 2;
                else
                    return 9;
            }
        }
    }
    else if (tile == '?') {
        for ( int k = 0; k < secrets_counter; k ++) {
            if (secrets[k].x == x && secrets[k].y == y) {
                if (secrets[k].state == 0)
                    return 10;
                else
                    return 5;
            }
        }
    }
    else if (tile == '<')
        return 9;
    else if (tile == '^') {
        for (int n = 0; n < trap_counter; n ++) {
            if (traps[n].x == x && traps[n].y == y) {
                if (traps[n].state == 0)
                    return 10;
                else
                    return 6;
            }
        }
    }
    else if (tile == '%') {
        for ( int p = 0; p < foods_counter; p ++) {
            if (foods[p].x == x && foods[p].y == y)
                return foods[p].color;
        }
    }
    else if (tile == 'p')
        return 6;
    else if (tile == '|' || tile == '-') {
        for ( int i = 0; i < rooms_counter[current_floor]; i ++) {
            if ((x >= rooms[current_floor][i].x && x <= rooms[current_floor][i].x + rooms[current_floor][i].width ) && (y >= rooms[current_floor][i].y && y <= rooms[current_floor][i].y + rooms[current_floor][i].height ) ) {
                if (rooms[current_floor][i].type == 2)
                    return 5;
                else if (rooms[current_floor][i].type == 1)
                    return 6;
                else
                    return 10;
            }
        }
    }
    
    else if (tile == '$'){
        for ( int m = 0; m < gold_counter; m ++) {
            if (golds[m].x == x && golds[m].y == y)
                return golds[m].type;
        }
    }
    else if (tile == 'T')
        return 5;
    else if (map[y][x] == 'm' || map[y][x] == 'd' || map[y][x] == '~' || map[y][x] == 'a' || map[y][x] == '!')
        return 4;
    else if (tile == '&')
        return 5;
    else if (tile == '*')
        return 5;
    return 10;
}

void next_floor (){
    current_floor++;
    init_map();
    generateMap();
}

void hoora (char stair) {
    if ( stair == '>')
        next_floor();
}

void print_floor () {
    attron(COLOR_PAIR(3));
    mvprintw(LINES - 1, 0, "Floor: %d", current_floor);
    attroff(COLOR_PAIR(3));
    refresh();
}

void display_hits () {
    attron(COLOR_PAIR(3));
    mvprintw(LINES -1, COLS/2 - 40, "Hits: %d", hits);
    attroff(COLOR_PAIR(3));
    refresh();
}

void desplay_gold () {
    attron(COLOR_PAIR(5));
    mvprintw(LINES -1, COLS/2 + 15, "Gold: %d", gold);
    attroff(COLOR_PAIR(5));
    refresh();
}

void printWhole() {
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            if(is_visible[j][i]) {
                int color = choose_color(map[j][i], i, j);
                attron(COLOR_PAIR(color));
                mvaddch(j, i, map[j][i]);
                attroff(COLOR_PAIR(color));
            }
            else {
                mvaddch(j, i, ' ');
            }
        }
    }
    refresh();
}

bool collision(Rooms room1, Rooms room2){
    return !(room1.x + room1.width < room2.x || room2.x + room2.width < room1.x || room1.y + room1.height < room2.y || room2.y + room2.height < room1.y);
}

void init_map () {
    for ( int i = 0; i < HEIGHT; i ++) {
        for (int j = 0; j < WIDTH; j ++) {
            map[i][j] = ' ';
            is_visible[i][j] = FALSE;
        }
    }
}

void cheat_code_M () {
    for ( int j = 0; j < HEIGHT; j ++) {
        for ( int i = 0; i < WIDTH; i ++) {
            int color = choose_color(map[j][i], i, j);
            attron(COLOR_PAIR(color));
            mvaddch(j, i, map[j][i]);
            attroff(COLOR_PAIR(color));
        }
    }
    refresh();
    messages("cheat code M", 0);
    getch();
}

void cheat_g () {
    g_state = 1;
}

void cheat_code_s (int ny, int nx) {
    bool trap = false;
    int trap_num = 0;
    bool secret_door = false;
    int door_num = 0;
    for ( int j = ny -1; j <= ny + 1; j ++) {
        for ( int i = nx -1; i <= nx + 1; i ++ ) {
            if (i!= nx || j!= ny) {
                if (map[j][i] == '?') {
                    secret_door = true;
                    door_num ++;
                }
                else if (map[j][i] == '^') {
                    trap = true;
                    trap_num++;
                }
            }
        }
    }
    
    if (trap) messages("trap around", trap_num);
    if (secret_door) messages("secret door around", door_num);
}

void addRooms (Rooms room) {
    for (int y = room.y; y < room.y + room.height; y++) {
        for (int x = room.x; x < room.x + room.width; x++) {
            if (x == room.x || x == room.x + room.width - 1)
                map[y][x] = '|';
            else if (y == room.y || y == room.y + room.height - 1)
                map[y][x] = '-';
            else
                map[y][x] = '.';
        }
    }
}

void doorrrr(Rooms room) {
    for (int y = room.y; y < room.y + room.height; y++) {
        if (map[y][room.x] == '+' && map[y][room.x - 1] != '#')
            map[y][room.x] = '|';
        else if (map[y][room.x - 1] == '#')
            map[y][room.x] = '+';
    }

    for (int y = room.y; y < room.y + room.height; y++) {
        if (map[y][room.x + room.width - 1] == '+' && map[y][room.x + room.width] != '#')
            map[y][room.x + room.width - 1] = '|';
        else if (map[y][room.x + room.width] == '#')
            map[y][room.x + room.width - 1] = '+';
    }

    for (int x = room.x; x < room.x + room.width; x++) {
        if (map[room.y][x] == '+' && map[room.y - 1][x] != '#')
            map[room.y][x] = '-';
        else if (map[room.y - 1][x] == '#')
            map[room.y][x] = '+';
    }

    for (int x = room.x; x < room.x + room.width; x++) {
        if (map[room.y + room.height - 1][x] == '+' && map[room.y + room.height][x] != '#')
            map[room.y + room.height - 1][x] = '-';
        else if (map[room.y + room.height][x] == '#')
            map[room.y + room.height - 1][x] = '+';
    }

    for (int y = room.y + 1; y < room.y + room.height - 1; y++) {
        for (int x = room.x + 1 ; x < room.x + room.width - 1; x++){
            if (map[y][x] == '+')
                map[y][x] = '.';
        }
    }
}

void pillar (Rooms room) {
    int counter = 0;
    for (int y = room.y + 2; y < room.y + room.height - 2; y++) {
        // max pillar in a single room is 3
        if (counter >= 3)
            break;
        for (int x = room.x + 2; x < room.x + room.width - 2; x++) {
            if (counter >= 3)
                break;
            if (!(rand () % 25) && map[y][x] == '.' && (map[y][x+ 1] != '+' && map[y][x-1] != '+' && map[y+1][x] != '+' && map[y-1][x] != '+')){
                //pillar won't be next to the door
                map[y][x] = 'O';
                counter++;
            }
        }
    }
}

void add_monster (Rooms room) {
    //manual probbability. can be changed based on difficulty
    int prob, counter = 0;
    if (current_floor == 1)
        prob = 50;
    else if (current_floor == 2)
        prob = 30;
    else if (current_floor == 3)
        prob = 25;
    else
        prob = 20;
    for (int y = room.y; y < room.y + room.height; y++) {
        //if (traps_count >= max) break;
        if (counter > 3)
            break;
        for (int x = room.x; x < room.x + room.width; x++) {
            int type = rand () % 10;
            char symbol = 'D';
            if (counter > 3)
                break;
            if (type == 0 || type == 2 || type == 3) {
                symbol = 'D';//D
                monsters[monster_counter].health = 5;
                monsters[monster_counter].num = 0;

            }
            else if (type == 4 || type == 5) {
                symbol = 'F';//F
                monsters[monster_counter].health = 10;
                monsters[monster_counter].num = 1;

            }
            else if (type == 6 || type == 7) {
                symbol = 'G';//G
                monsters[monster_counter].health = 15;
                monsters[monster_counter].num = 2;

            }
            else if (type == 8) {
                symbol = 'S'; //S
                monsters[monster_counter].health = 20;
                monsters[monster_counter].num = 3;
            }
            else if (type == 9) {
                symbol = 'U'; //U
                monsters[monster_counter].health = 30;
                monsters[monster_counter].num = 4;
            }
            if (rand () % prob == 0 && map[y][x] == '.') {
                monsters[monster_counter].x = x;
                monsters[monster_counter].y = y;
                monsters[monster_counter].type = symbol;
                map[y][x] = symbol;
                monsters[monster_counter].state = 0;
                monsters[monster_counter].movement_num = 0;
                monsters[monster_counter].movement_state = 0;
                monster_counter++;
                counter++;
            }
        }
    }
}

void add_trap (Rooms room) {
    int prob, max;
    //setting logical manual probability
    if (room.type == 2){
        prob = 5;
        max = 12;
    }
    else {
        prob = 50;
        max = 5;
    }
    for (int y = room.y; y < room.y + room.height; y++) {
        //if (traps_count >= max) break;
        for (int x = room.x; x < room.x + room.width; x++) {
           // if (traps_count >= max) break;
            if (!(rand () % prob) && map[y][x] == '.') {
                traps[trap_counter].x = x;
                traps[trap_counter].y = y;
                traps[trap_counter].state = 0;
                trap_counter++;
            }
        }
    }
}

void corridor (int x1, int y1, int x2, int y2) {
    bool flag = FALSE;
    if (x1 < 2) x1 = 2;
    if (y1 < 2) y1 = 2;
    if (x2 < 2) x2 = 2;
    if (y2 < 2) y2 = 2;
    if (rand() % 2){
        while (x1 != x2) {
            if (map[y1][x1] == '|' || map[y1][x1] == '-')
                map[y1][x1] = '+';
            else if ( map[y1][x1] == ' ')
                map[y1][x1] = '#';
            x1 += (x2 > x1) ? 1 : -1;
        }
        while (y1 != y2) {
            if ( map[y1][x1] == '|' || map[y1][x1] == '-')
                map[y1][x1] = '+';
            else if ( map[y1][x1] == ' ')
                map[y1][x1] = '#';
            y1 += (y2 > y1) ? 1 : -1;
        }
    }
    else{
        while (y1 != y2) {
            if (!flag ||  map[y1][x1] == '|' || map[y1][x1] == '-'){
                map[y1][x1] = '+';
                flag = TRUE;
            } 
            else if ( map[y1][x1] == ' ')
                map[y1][x1] = '#';
            y1 += (y2 > y1) ? 1 : -1;
        }
        while (x1 != x2) {
            if (!flag ||  map[y1][x1] == '|' || map[y1][x1] == '-')  {
                map[y1][x1] = '+';
                flag = TRUE;
            } 
            else if ( map[y1][x1] == ' ')
                map[y1][x1] = '#';
            x1 += (x2 > x1) ? 1 : -1;
        }
    }
}

void add_stairs (Rooms room) {
    bool flag = FALSE;
    for (int y = room.y; y < room.y + room.height; y++) {
        if (flag)
            break;
        for (int x = room.x; x < room.x + room.width; x++) {
            if (flag)
                break;
            if (!(rand () % 20) && map[y][x] == '.') {
                attron(COLOR_PAIR(9));
                map[y][x] = '<';
                attroff(COLOR_PAIR(9));
                flag = TRUE;
                refresh();
            }
        }
    }
    //making sure
    if (!flag){
        int new_x = room.x + room.width / 2 + 1;
        int new_y = room.y + room.height / 2 - 1;
        attron(COLOR_PAIR(9));
        map[new_y][new_x] = '<';
        attroff(COLOR_PAIR(9));
        refresh();
    }
}

void show_door (int ny, int nx) {
    int which_door = -1;
    for ( int i = 0; i < secrets_counter; i ++) {
        if(secrets[i].x == nx && secrets[i].y == ny) {
            which_door = i;
            secrets[i].state = 1;
        }
    }
    if (which_door != -1 && secrets[which_door].state) {
        attron(COLOR_PAIR(2));
        map[ny][nx] = '?';
        attroff(COLOR_PAIR(2));
        refresh();
    }
    printWhole();
}

void health_bar (int health) {
    if (health > 100) health = 100;
    if (health <= 0)
        lose_game();
    int filled = (health * 20) / 100;
    move(LINES - 1, COLS - 20 - 10);
    addch('[');
    attron(COLOR_PAIR(3));
    for (int i = 0; i < 20; i++) {
        if (i < filled)
            addch('#');    
        else
            addch('-');
        refresh();
    }
    attroff(COLOR_PAIR(3));
    addch(']');
    mvprintw(LINES -1, COLS - 40, "Jooon: ");
    mvprintw(LINES - 1, COLS -8, "%d%%", (health * 100) / 100);
    refresh();
}

void lose_health (int count) {
    health -= count;
    health_bar(health);
}

void hunger_bar (int hunger) {
    if (hunger > 100) hunger = 100;
    if (hunger <= 0)
        lose_game();
    //if (health > 0 && health <= 50) messages("low health", 0);
    int filled = (hunger * 20) / 100;
    move(LINES - 1, COLS - 20 - 10 - 56);
    addch('[');
    for (int i = 0; i < 20; i++) {
        if (i < filled) {
            attron(COLOR_PAIR(2));
            addch('#');
            attroff(COLOR_PAIR(2));
            //refresh();
            
        } else {
            attron(COLOR_PAIR(2));
            addch('-');
            attroff(COLOR_PAIR(2));
            //refresh();
        }
    }
    addch(']');
    char state [10];
    mvprintw(LINES -1, COLS - 39 - 56 , "Hunger: ");
    if (hunger <= 100 && hunger > 80) strcpy(state, "Peak");
    else if (hunger <= 80 && hunger > 60) strcpy(state, "Cool");
    else if (hunger <= 60 && hunger > 40) strcpy(state, "Hungry");
    else if (hunger <= 40 && hunger > 20) strcpy(state, "Starving");
    else if (hunger <= 20 && hunger > 0) strcpy(state, "Dying");

    mvprintw(LINES - 1,COLS -8 - 56, "%s", state);
    refresh();
}

void show_trap (int ny, int nx) {
    //just like door
    int which_trap = 0;
    for ( int i = 0; i < trap_counter; i ++) {
        if(traps[i].x == nx && traps[i].y == ny) {
            which_trap = i;
            traps[i].state = 1;
        }
    }

    if (traps[which_trap].state) {
        attron(COLOR_PAIR(2));
        map[ny][nx] = '^';
        attroff(COLOR_PAIR(2));
        refresh();
    }
    lose_health(5);
}

void add_hidden_door (Rooms room ) {

    for (int y = room.y; y < room.y + room.height; y++) {
        if (map[y][room.x] == '+') {
            secrets[secrets_counter].x = room.x;
            secrets[secrets_counter].y = y;
            secrets[secrets_counter].state = 0;
            secrets_counter++;
            attron(COLOR_PAIR(5));
            map[y][room.x] = '|';
            attroff(COLOR_PAIR(5));
            refresh();
        }
    }
    
    for (int y = room.y; y < room.y + room.height; y++) {
        if (map[y][room.x + room.width - 1] == '+') {
            secrets[secrets_counter].x= room.x + room.width - 1;
            secrets[secrets_counter].y = y;
            secrets[secrets_counter].state = 0;
            secrets_counter++;
            attron(COLOR_PAIR(5));
            map[y][room.x + room.width - 1] = '|';
            attroff(COLOR_PAIR(5));
            refresh();
        }
    }
    
    for (int x = room.x; x < room.x + room.width; x++) {
        if (map[room.y][x] == '+') {
            secrets[secrets_counter].x= x;
            secrets[secrets_counter].y= room.y;
            secrets[secrets_counter].state = 0;
            secrets_counter++;
            attron(COLOR_PAIR(5));
            map[room.y][x] = '-';
            attroff(COLOR_PAIR(5));
            refresh();
        }
    }
        
    for (int x = room.x; x < room.x + room.width; x++) {
        if (map[room.y + room.height - 1][x] == '+') {
            secrets[secrets_counter].x= x;
            secrets[secrets_counter].y= room.y + room.height - 1;
            secrets[secrets_counter].state = 0;
            secrets_counter++;
            attron(COLOR_PAIR(5));
            map[room.y + room.height - 1][x] = '-';
            attroff(COLOR_PAIR(5));
            refresh();
        }
    }
}

void add_master_key (Rooms room) {
    //Like before
    bool flag = FALSE;
    for (int y = room.y; y < room.y + room.height; y++) {
        if (flag)
            break;
        for (int x = room.x; x < room.x + room.width; x++) {
            if (flag)
                break;
            if (rand () % 25 == 0 && map[y][x] == '.') {
                map[y][x] = '*';
                flag = TRUE;
            }
        }
    }
    //making sure
    if (!flag){
        int finalx = room.x + room.width * 2 / 3;
        int finaly = room.y + room.height / 2;
        map[finaly][finalx] = '*';
    }
}

void generate_door_pass (char *password){
    for (int i = 0; i < 4; i++) {
        password[i] = '0' + rand() % 10;
    }
    password[4] = '\0';
}

void show_password(int px, int py) {
    if (!password_show_time) {
        generate_door_pass(password);
        password_show_time = time(NULL);
    }
    int win_width = 30;
    int win_height = 8;
    int start_x = (max_x - win_width) / 2;
    int start_y = (max_y - win_height) / 2;
    WINDOW *password_win = newwin(win_height, win_width, start_y, start_x);
    
    wbkgd(password_win, COLOR_PAIR(7));
    box(password_win, 0, 0);

    int time_passed = (int)difftime(time(NULL), password_show_time);

    char msg1[] = "SHH! Don't tell anyone!";
    int msg1_len = strlen(msg1);
    int start_col1 = (win_width - msg1_len) / 2;

    char msg2[] = "Password: ";
    int msg2_len = strlen(msg2);
    int start_col2 = (win_width - msg2_len - strlen(password)) / 2;

    while (time_passed < 5) {
        mvwprintw(password_win, 3, start_col1, "%s", msg1);
        mvwprintw(password_win, 4, start_col2, "%s%s", msg2, password);
        time_passed = (int) difftime(time(NULL), password_show_time);
        napms(1000);
        wrefresh(password_win);
    }
    password_show_time = 0;
    werase(password_win);
    delwin(password_win);
}

int LockedPasswordGet(int px, int py) {
    int which_door = 0;
    for (int c = 0; c < locked_counter; c ++ ) {
        if (locked[c].x == px && locked[c].y == py) {
            which_door = c;
        }
    }
    int win_width = 30;
    int win_height = 8;
    int start_x = (max_x - win_width) / 2;
    int start_y = (max_y - win_height) / 2;

    char is_pass[5];
    WINDOW *password_win = newwin(win_height, win_width, start_y, start_x);
    
    wbkgd(password_win, COLOR_PAIR(7));
    box(password_win, 0, 0);
    
    echo();
    char msg1 [] = "A Stranger????";
    int msg1_len = strlen(msg1);
    int start_col1 = (win_width - msg1_len) / 2;

    char msg2 [] = "What is the Password?  ";
    int msg2_len = strlen(msg2);
    int start_col2 = (win_width - msg2_len) / 2;
    mvwprintw(password_win, 3, start_col1, "%s", msg1);
    mvwprintw(password_win, 4, start_col2, "%s", msg2);
    mvwgetstr(password_win, 5, start_col2, is_pass);
    wrefresh(password_win);
    noecho();

    size_t len = strlen(is_pass);
    if (len > 0 && is_pass[len - 1] == '\n')
        is_pass[len - 1] = '\0';
    if (strcmp(password, is_pass) == 0 ) {
        locked[which_door].state = 1;
        messages("door opened", 0);
        attron(COLOR_PAIR(2));
        mvaddch(py, px, '@');
        map[py][px] = '@';
        attroff(COLOR_PAIR(2));
        refresh();
    } else {
        WINDOW *warning_1 = newwin(win_height, win_width, start_y, start_x);
        wbkgd(warning_1, COLOR_PAIR(11));
        box(warning_1, 0, 0);
        char msg4 [] = "WARNING! Try again!";
        int msg4_len = strlen(msg4);
        int start_col4 = (win_width - msg4_len) / 2;
        
        mvwprintw(warning_1, 3, start_col4, "%s", msg4);
        echo();
        mvwgetstr(warning_1, 5, start_col4, is_pass);
        noecho();

        size_t len = strlen(is_pass);
        if (len > 0 && is_pass[len - 1] == '\n') {
            is_pass[len - 1] = '\0';
        }
        wrefresh(warning_1);
        delwin(warning_1);
        
        if (strcmp(password, is_pass) == 0 ) {
            locked[which_door].state = 1;
            messages("door opened", 0);
        } else {
            WINDOW *warning_2 = newwin(win_height, win_width, start_y, start_x);
            wbkgd(warning_2, COLOR_PAIR(8));
            box(warning_2, 0, 0);
            char msg5 [] = "WARNING! Last Try!";
            int msg5_len = strlen(msg5);
            int start_col5 = (win_width - msg5_len) / 2;
            
            mvwprintw(warning_2, 3, start_col5, "%s", msg5);
            echo();
            mvwgetstr(warning_2, 5, start_col5, is_pass);
            noecho();

            size_t len = strlen(is_pass);
            if (len > 0 && is_pass[len - 1] == '\n') {
                is_pass[len - 1] = '\0';
            }
            wrefresh(warning_2);
            delwin(warning_2);
            
            if (strcmp(password, is_pass) == 0 ) {
                locked[which_door].state = 1;
                messages("door opened", 0);
            }
            else {
                WINDOW *warning_3 = newwin(win_height, win_width, start_y, start_x);
                wbkgd(warning_3, COLOR_PAIR(8));
                box(warning_3, 0, 0);
                char msg6 [] = "WARNING! Security Lockdown!";
                int msg6_len = strlen(msg6);
                int start_col6 = (win_width - msg6_len) / 2;
                mvwprintw(warning_3, 3, start_col6, "%s", msg6);
                wrefresh(warning_3);
                getch();
                delwin(warning_3);
            }
            
        }
    }
    return which_door;
}

void locked_door (Rooms room) {
    int door_x [100], door_y [100];
    for (int y = room.y; y < room.y + room.height; y++) {
        if (map[y][room.x] == '+' || map[y][room.x] == '?') {
            locked[locked_counter].x = room.x;
            locked[locked_counter].y = y;
            locked[locked_counter++].state = 0;
            attron(COLOR_PAIR(2));
            map[y][room.x] = '@';
            attroff(COLOR_PAIR(2));
        }
    }
    
    for (int y = room.y; y < room.y + room.height; y++) {
        if (map[y][room.x + room.width - 1] == '+' || map[y][room.x + room.width - 1] == '?') {
            locked[locked_counter].x  = room.x + room.width - 1;
            locked[locked_counter].y = y;
            locked[locked_counter++].state = 0;
            attron(COLOR_PAIR(2));
            map[y][room.x + room.width - 1] = '@';
            attroff(COLOR_PAIR(2));
        }
    }
    
    for (int x = room.x; x < room.x + room.width; x++) {
        if (map[room.y][x] == '+' || map[room.y][x] == '?') {
            locked[locked_counter].x = x;
            locked[locked_counter].y = room.y;
            locked[locked_counter++].state = 0;
            attron(COLOR_PAIR(2));
            map[room.y][x] = '@';
            attroff(COLOR_PAIR(2));
        }
    }
    
    for (int x = room.x; x < room.x + room.width; x++) {
        if (map[room.y + room.height - 1][x] == '+' || map[room.y + room.height - 1][x] == '?') {
            locked[locked_counter].x  = x;
            locked[locked_counter].y = room.y + room.height - 1;
            locked[locked_counter++].state = 0;
            attron(COLOR_PAIR(2));
            map[room.y + room.height - 1][x] = '@';
            attroff(COLOR_PAIR(2));
        }
    }

    int new_x, new_y;
    do {
        new_x = room.x + 1 + rand() % (room.width - 2);
        new_y = room.y + 1 + rand() % (room.height - 2);
    } while (map[new_y][new_x] != '.');
    
    attron(COLOR_PAIR(5));
    map[new_y][new_x] = '&';
    attroff(COLOR_PAIR(5));
    refresh();
}

void pick_up (int y, int x) {
    if (map[y][x] == '*') {
        map[y][x] = '.';
        elems[elem_counter].x = x;
        elems[elem_counter].y = y;
        elems[elem_counter].name = "master key";
        messages("picked up key", 0);
        elem_counter++;
    } else if (map[y][x] == '%') {
        map[y][x] = '.';
        elems[elem_counter].x = x;
        elems[elem_counter].y = y;
        elems[elem_counter].name = "food";
        int food_index = 0;
        for ( int i = 0; i < foods_counter; i ++) {
            if (foods[i].x == x && foods[i].y == y && foods[i].state == -1) {
                food_index = i;
                foods[i].state = 0;
            }
        }
        messages("picked up food", food_index);
        elem_counter++;
    } else if (map[y][x] == '$') {
        map[y][x] = '.';
        elems[elem_counter].x = x;
        elems[elem_counter].y = y;
        elems[elem_counter].name = "gold";
        int gold_type = 0;
        for ( int i = 0; i < gold_counter; i++) {
            if (golds[i].x == x && golds[i].y == y && golds[i].state == -1) {
                gold_type = golds[i].type;
                golds[i].state = 0;
            }
        }
        messages("picked up gold", gold_type);
        elem_counter++;
    } else if (map[y][x] == 'm' || map[y][x] == 'd' || map[y][x] == '~' || map[y][x] == 'a' || map[y][x] == '!') {
        map[y][x] = '.';
        elems[elem_counter].x = x;
        elems[elem_counter].y = y;
        elems[elem_counter].name = "weapon";
        char symbol = 'm';
        int type, n = 1;
        for ( int i = 0; i < weapon_counter; i++) {
            if (weapons[i].x == x && weapons[i].y == y && (weapons[i].state == -1 || weapons[i].thrown == 1)) {
                symbol = weapons[i].symbol;
                if (symbol == 'm')
                    type = 1;
                else if (symbol == 'd') {
                    type = 2;
                    n = 10;
                }
                else if (symbol == '~') {
                    type = 3;
                    n = 8;
                }
                else if (symbol == 'a') {
                    type = 4;
                    n = 20;
                }
                else
                    type = 5;
                if (weapons[i].num_collect < n) weapons[i].num_collect++;
                weapons[i].state = 0;
            }
        }

        messages("picked up weapon", type);
        elem_counter++;
    } else if (map[y][x] == 'p') {
        map[y][x] = '.';
        elems[elem_counter].x = x;
        elems[elem_counter].y = y;
        elems[elem_counter].name = "potions";
        int type = 0;
        for ( int i = 0; i < potions_counter; i ++) {
            if (potions[i].x == x && potions[i].y == y) {
                type = potions[i].type;
                potions[i].state = 0;
            }
        }
        messages("picked up potion", type);
        elem_counter++;
    }
}

void show_corridor (int wx, int wy) {
    //for now you can only see 1 corrider. will be changed based on difficulty
    for (int y = -2; y < 3; y++) {
        for (int x = -2; x < 3; x++) {
            if ((wx + x >= 0 && wx + x < WIDTH && wy + y >= 0 && wy + y < HEIGHT) && (map[wy + y][wx + x] == '#' || map[wy + y][wx + x] == '+'))
                is_visible[wy + y][wx + x] = TRUE;
        }
    }
}

void show_room(Rooms room) {
    for (int y = room.y; y < room.y + room.height; y++) {
        for (int x = room.x; x < room.x + room.width; x++)
            is_visible[y][x] = TRUE;
    }
    if (room.type != 0)
        messages("enter room", room.type);
}

Rooms roomFinder (int wx, int wy) {
    Rooms room;
    room.x = -1;
    room.y = -1;
    for (int i = 0; i < rooms_counter[current_floor]; i++) {
        if (wx >= rooms[current_floor][i].x && wx <= rooms[current_floor][i].x + rooms[current_floor][i].width && wy >= rooms[current_floor][i].y && wy <= rooms[current_floor][i].y + rooms[current_floor][i].height)
            room = rooms[current_floor][i];
    }
    return room;
}

void player_in_room (int px, int py, Rooms rooms[], int room_count) {
    Rooms room;
    for (int i = 0; i < room_count; i++) {
        room = rooms[i];
        //inside room
        if (px >= room.x && px < room.x + room.width && py >= room.y && py < room.y + room.height)
            show_room(room);
    }
    show_corridor(px, py); 
}

void monster_attack (Monsters monster) {
    char type = monster.type;
    if (type == 'D') {
        messages("attack", 0);
        lose_health(5);
    }
    else if (type == 'F') {
        messages("attack", 1);
        lose_health(5);
    }
    else if (type == 'G') {
        messages("attack", 2);
        lose_health(5);
    }
    else if (type == 'S') {
        messages("attack", 3);
        lose_health(5);
    }
    else if (type == 'U') {
        messages("attack", 4);
        lose_health(5);
    }
}

void while_inside_room (int px, int py, Rooms room) {
    for ( int i = 0; i < monster_counter; i++) {
        if (monsters[i].state == 0 && (monsters[i].x >= room.x && monsters[i].x <= room.x + room.width) && (monsters[i].y >= room.y && monsters[i].y <= room.y + room.height) && room.type != 1) {
            if (monsters[i].type == 'G' || monsters[i].type == 'S' || monsters[i].type == 'U')
                monster_move(px, py, &monsters[i]);
            if (monster_check(px, py, monsters[i]))
                monster_attack(monsters[i]);
        }
    }
    //printWhole();
}

Monsters monster_in_room (int px, int py ) {
    Monsters monster;
    monster.x = -1;
    monster.y = -1;

    for ( int i = 0; i < monster_counter; i ++) {
        if ((monsters[i].x == px || monsters[i].x == px + 1 || monsters[i].x == px - 1) && ( monsters[i].y == py || monsters[i].y == py + 1 || monsters[i].y == py - 1))
            return monsters[i];
    }
    return monster;
}

bool monster_check (int x, int y, Monsters monster) {
    return (monster.state == 0 && (monster.x == x || monster.x == x + 1 || monster.x == x - 1) && ( monster.y == y || monster.y == y + 1 || monster.y == y - 1));
}

void monster_move(int px, int py, Monsters *m) {
    if (m->type == 'G' || m->type == 'U'){
        if (m->movement_num > 5)
            return;
    }
    if (m->movement_state == 1)
        return;
    int my = m->y;
    int mx = m->x;

    int nx = mx;
    int ny = my;

    if (mx < px && mx + 1 < WIDTH && map[my][mx + 1] == '.') {
        nx++;
    } else if (mx > px && mx - 1 >= 0 && map[my][mx - 1] == '.') {
        nx--;
    } else if (my < py && my + 1 < HEIGHT && map[my + 1][mx] == '.') {
        ny++;
    } else if (my > py && my - 1 >= 0 && map[my - 1][mx] == '.') {
        ny--;
    }

    if (nx == px && ny == py) {
        monster_attack(*m);
        return;
    }

    if (nx != mx || ny != my) {
        map[my][mx] = '.';
        m->x = nx;
        m->y = ny;
        m->movement_num ++;
        map[ny][nx] = m->type;
    }
    printWhole();
}

char *food_menu (int color) {
    char *names [] = {"Ambrosia", "Slightly Moldy Cheese", "Rock-hard Biscuit", "Questionable Apple", "Mystery Meat", "Infernal Steak", "Ethereal Berries", "Exploding Berries", "Potionberry Pie"};
    if (color == 5) {
        int which = rand () % 3;
        return names[which];
    }else if (color == 2) {
        int which = 3 + rand () % 3;
        return names[which];
    } else {
        int which = 6 + rand () % 3;
        return names[which];
    }
}

void drop_weapon (int x, int y, Weapon * weapon) {
    weapon->x = x;
    weapon->y = y;
    weapon->state = 0;
    map[y][x] = weapon->symbol;
    weapon->num_collect--;
    weapon->thrown = 1;
    if (weapon->num_collect == 0)
        weapon->state = -1;
}

void a_command (int px, int py) {
    int type;
    if( weapInUse->symbol == 'd')
        type = 0;
    else if( weapInUse->symbol == '~')
        type = 1;
    else if( weapInUse->symbol == 'a')
        type = 2;
    dagger_wand_arrow_attack(px, py, last_direction, type);
}

void dagger_wand_arrow_attack (int px, int py, char * direction, int type) {
    int distance;
    char symbol;
    if (type == 0) {
        distance = 5;
        symbol = 'd';
    } else if (type == 1) {
        distance = 10;
        symbol = '~';
    } else {
        distance = 5;
        symbol = 'a';
    }
    int dx = 0, dy = 0;
    
    if (strcmp(direction, "u") == 0) dy = -1;
    else if (strcmp(direction, "d") == 0) dy = +1;
    else if (strcmp(direction, "l") == 0) dx = -1;
    else if (strcmp(direction, "ur") == 0) dx = +1;
    else if (strcmp(direction, "ur") == 0) {
        dx = +1;
        dy = -1;
    } else if (strcmp(direction, "ul") == 0) {
        dx = -1;
        dy = -1;
    } else if (strcmp(direction, "dl") == 0) {
        dx = -1;
        dy = +1;
    } else if (strcmp(direction, "dr") == 0) {
        dx = +1;
        dy = +1;
    }
    bool weapon_used = false;
     for (int i = 1; i <= distance; i++) {
        int new_x = px + i * dx;
        int new_y = py + i * dy;

        if (new_x < 0 || new_x >= WIDTH || new_y < 0 || new_y >= HEIGHT) {
            drop_weapon(px + (i - 1) * dx, py + (i - 1) * dy, weapInUse);
            return;
        }

        if (map[new_y][new_x] == '|' || map[new_y][new_x] == '-' || map[new_y][new_x] == '#') {
            drop_weapon(px + (i - 1) * dx, py + (i - 1) * dy, weapInUse);
            return;
        }

        for (int j = 0; j < monster_counter; j++) {
            if (monsters[j].x == new_x && monsters[j].y == new_y) {
                player_attack(monsters[j].x, monsters[j].y, symbol);
                weapon_used = true;
                return;
            }
        }
    }

    if (!weapon_used) {
        drop_weapon(px + distance * dx, py + distance * dy, weapInUse);
    }

    strncpy(last_direction, direction, sizeof(last_direction) - 1);
    last_direction[sizeof(last_direction) - 1] = '\0';

    long_range_weapon = true;
}

void monster_health_check (Monsters * monster) {
    if (monster->health <= 0)
        monster->state = 1;
    else
        monster->state = 0;
}

void player_attack (int mx, int my, char type) {
    if (potion_time_track == 0)
        weapSpeed = 1;
    int damage = weapSpeed * 5;
    if (type == 'd')
        damage = weapSpeed * 12;
    else if (type == '~')
        damage = weapSpeed * 15;
    else if (type == 'a')
        damage = weapSpeed * 5;
    else if (type == '!')
        damage = weapSpeed * 10;

    for (int i = 0; i < monster_counter; i ++) {
        if (monsters[i].x == mx && monsters[i].y == my) {
            if (type == '~') {
                monsters[i].movement_state = 1;
                messages("monster frozen", i);
            }
            monsters[i].health -= damage;
            monster_health_check(&monsters[i]);
            messages("player attack", i);
            if (monsters[i].state) {
                map[my][mx] = '.';
                messages("monster dead", i);
                if (weapInUse->symbol != 'm' && weapInUse->symbol != '!')
                    weapInUse->num_collect--;
                if (weapInUse->num_collect == 0)
                    weapInUse->state = -1;
                hits ++;
                display_hits();
            }
        }
    }
}

void food_choice (char * name) {
    for ( int i = 0; i < foods_counter; i ++) {
        if (strcmp(foods[i].name, name) == 0 && foods[i].state == 0) {
            if (foods[i].fresh == 0 || foods[i].fresh == 1)  {
                int type = 0;
                if (foods[i].fresh == 1 && foods[i].color == 5) {
                    dragon_blood();
                    type = 1;
                }
                else if (foods[i].fresh == 1 && foods[i].color == 6) {
                    storm_kiss();
                    type = 2;
                    magicalFood = TRUE;
                }
                foods[i].state = 1;
                health += 5;
                hunger += 5;
                health_bar(health);
                hunger_bar(hunger);
                messages("ate food", type);
                break;
            }
            else {
                foods[i].state = 1;
                health -= 5;
                hunger -= 5;
                health_bar(health);
                hunger_bar(hunger);
                messages("ate spoiled food", 0);
                break;
            }
        }
    }
    timeFood = 50;
}

void food_window () {

    int win_h = 10, win_w = 50;
    int start_y = (max_y - win_h) / 2;
    int start_x = (max_x - win_w) / 2;

    WINDOW * food = newwin(win_h, win_w, start_y, start_x);
    wclear(food);
    box(food, 0, 0);

    int ex_berry = 0, eth_berry = 0, pie = 0, amb = 0, cheese = 0, biscuit = 0, steak = 0, apple = 0, meat = 0;
    for ( int i = 0; i < foods_counter; i ++) {
        if (strcmp(foods[i].name, "Exploding Berries") == 0 && foods[i].state == 0) ex_berry++;
        else if (strcmp(foods[i].name, "Ethereal Berries") == 0 && foods[i].state == 0) eth_berry++;
        else if (strcmp(foods[i].name, "Potionberry Pie") == 0 && foods[i].state == 0) pie++;
        else if (strcmp(foods[i].name, "Ambrosia") == 0 && foods[i].state == 0) amb++;
        else if (strcmp(foods[i].name, "Slightly Moldy Cheese") == 0 && foods[i].state == 0) cheese++;
        else if (strcmp(foods[i].name, "Rock-hard Biscuit") == 0 && foods[i].state == 0) biscuit++;
        else if (strcmp(foods[i].name, "Infernal Steak") == 0 && foods[i].state == 0) steak++;
        else if (strcmp(foods[i].name, "Mystery Meat") == 0 && foods[i].state == 0) meat++;
        else if (strcmp(foods[i].name, "Questionable Apple") == 0 && foods[i].state == 0) apple++;
    }
    wattron(food,COLOR_PAIR(6));
    int len = strlen("** FOOD **");
    mvwprintw(food, 1, (win_w - len)/2, "** FOOD **");
    wattroff(food,COLOR_PAIR(6));
    
    int ex_berry_id = 0, eth_berry_id = 0, pie_id = 0, amb_id = 0, cheese_id = 0, biscuit_id = 0, steak_id = 0, apple_id = 0, meat_id = 0;
    int identifier = 1;
    if (ex_berry) {
        mvwprintw(food, identifier + 1, 1, "%d. %d Exploding Berries", identifier, ex_berry);
        ex_berry_id = identifier;
        identifier++;
    }
    if (eth_berry) {
        mvwprintw(food, identifier + 1, 1, "%d. %d Ethereal Berries", identifier, eth_berry);
        eth_berry_id = identifier;
        identifier ++;
    }
    if (pie) {
        mvwprintw(food, identifier + 1, 1, "%d. %d Potionberry Pie",identifier, pie);
        pie_id = identifier;
        identifier ++;
    }
    if (amb) {
        mvwprintw(food, identifier + 1, 1, "%d. %d Ambrosia",identifier, amb);
        amb_id = identifier;
        identifier ++;
    }
    if (cheese) {
        mvwprintw(food, identifier + 1, 1, "%d. %d Slightly Moldy Cheese",identifier, cheese);
        cheese_id = identifier;
        identifier ++;
    }
    if (biscuit) {
        mvwprintw(food, identifier + 1, 1, "%d. %d Rock-hard Biscuit ",identifier, biscuit);
        biscuit_id = identifier;
        identifier ++;
    }
    if (steak) {
        mvwprintw(food, identifier + 1, 1, "%d. %d Infernal Steak",identifier, steak);
        steak_id = identifier;
        identifier ++;
    }
    if (apple) {
        mvwprintw(food, identifier + 1, 1, "%d. %d Questionable Apple",identifier, apple);
        apple_id = identifier;
        identifier ++;
    }
    if (meat) {
        mvwprintw(food, identifier + 1, 1, "%d. %d Mystery Meat",identifier, meat);
        meat_id = identifier;
        identifier ++;
    }
    
    if (!ex_berry && !eth_berry && !pie && !amb && !cheese && !biscuit && !steak && !apple && !meat ) {
        char text [50] = "You don't have any food to consume!";
        int x = (win_w - strlen(text)) / 2;
        mvwprintw(food, 4, x, "%s", text);
    }
    
    wrefresh(food);

    int choice = getch();
    choice = choice - '0';
    if (choice == ex_berry_id) food_choice("Exploding Berries");
    else if (choice == eth_berry_id) food_choice("Ethereal Berries");
    else if (choice == pie_id) food_choice("Potionberry Pie");
    else if (choice == amb_id) food_choice("Ambrosia");
    else if (choice == cheese_id) food_choice("Slightly Moldy Cheese");
    else if (choice == biscuit_id) food_choice("Rock-hard Biscuit");
    else if (choice == steak_id) food_choice("Infernal Steak");
    else if (choice == meat_id) food_choice("Mystery Meat");
    else if (choice == apple_id) food_choice("Questionable Apple");
    delwin(food);
}

void weapon_in_bag () {
    weapInUse->state = 0;
    wield_choice = '0';
}

void weapon_choice (char symbol) {
    if (weapInUse->symbol == symbol)
        return;
    weapon_in_bag();
    for ( int i = 0; i < weapon_counter; i++){
        if (weapons[i].symbol == symbol && weapons[i].state == 0) {
            weapons[i].state =1;
            weapInUse = &weapons[i];
            wield_choice = symbol;
            break;
        }
    }
}

void weapon_window() {

    int win_height = 12, win_width = 80;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *arsenal = newwin(win_height, win_width, start_y, start_x);
    wclear(arsenal);
    box(arsenal, 0, 0);

    int len = strlen("** ARSENAL **");
    mvwprintw(arsenal, 1, (win_width - len)/2, "** ARSENAL **");

    for (int i = 2; i < win_height - 1; i++) {
          mvwaddch(arsenal, i, 39, '|');
      }
    
    int dag = 0, wand = 0, arrow = 0, sword = 0;
    for (int i = 0; i < weapon_counter; i++) {
        if (weapons[i].symbol == 'd' && weapons[i].state != -1) dag += weapons[i].num_collect;
        if (weapons[i].symbol == '~' && weapons[i].state != -1) wand += weapons[i].num_collect;
        if (weapons[i].symbol == 'a' && weapons[i].state != -1) arrow += weapons[i].num_collect;
        if (weapons[i].symbol == '!' && weapons[i].state != -1) sword += weapons[i].num_collect;
    }

    int identifier = 1;
    int short_y = 3, long_y = 3;

    mvwprintw(arsenal, short_y, 5, "Short-Range Weapons:");
    mvwprintw(arsenal, long_y, 45, "Long-Range Weapons:");

    short_y += 2;
    long_y += 2;
    
    int mac_id = 0, dag_id = 0, wand_id = 0, arrow_id = 0, sword_id = 0;
    int color = 10;
    if ('m' == wield_choice) color = 6;
    wattron(arsenal, COLOR_PAIR(color));
    mvwprintw(arsenal, short_y++, 5, "[m] Mace (%d)", 1);
    wattroff(arsenal, COLOR_PAIR(color));
    wattron(arsenal, COLOR_PAIR(2));
    mvwprintw(arsenal, short_y++, 5, "damage = 5");
    wattroff(arsenal, COLOR_PAIR(2));
    if (dag > 0) {
        int color = 10;
        if ('d' == wield_choice) color = 6;
        wattron(arsenal, COLOR_PAIR(color));
        mvwprintw(arsenal, long_y++, 45, "[d] Dagger (%d)",  dag);
        wattroff(arsenal, COLOR_PAIR(color));
        wattron(arsenal, COLOR_PAIR(2));
        mvwprintw(arsenal, long_y++, 45, "distance = 5, damage = 5");
        wattroff(arsenal, COLOR_PAIR(2));
    }
    if (sword > 0) {
        int color = 10;
        if ('!' == wield_choice) color = 6;
        wattron(arsenal, COLOR_PAIR(color));
        mvwprintw(arsenal, short_y++, 5, "[!] Sword (%d)", sword);
        wattroff(arsenal, COLOR_PAIR(color));
        wattron(arsenal, COLOR_PAIR(2));
        mvwprintw(arsenal, short_y++, 5, "damage = 10");
        wattroff(arsenal, COLOR_PAIR(2));

    }
    if (wand > 0) {
        int color = 10;
        if ('~' == wield_choice) color = 6;
        wattron(arsenal, COLOR_PAIR(color));
        mvwprintw(arsenal, long_y++, 45, "[~] Magic Wand (%d)", wand);
        wattroff(arsenal ,COLOR_PAIR(color));
        wattron(arsenal, COLOR_PAIR(2));
        mvwprintw(arsenal, long_y++, 45, "distance = 10, damage = 12");
        wattroff(arsenal, COLOR_PAIR(2));

    }
    if (arrow > 0) {
        int color = 10;
        if ('a' == wield_choice) color = 6;
        wattron(arsenal, COLOR_PAIR(color));
        mvwprintw(arsenal, long_y++, 45, "[a] Normal Arrow (%d)", arrow);
        wattroff(arsenal, COLOR_PAIR(color));
        wattron(arsenal, COLOR_PAIR(2));
        mvwprintw(arsenal, long_y++, 45, "distance = 5, damage = 5");
        wattroff(arsenal, COLOR_PAIR(2));
    }
    wrefresh(arsenal);

    int choice = wgetch(arsenal);
    if (wield_choice != '0') {
        messages("put in bag", 0);
        delwin(arsenal);
        return;
    }
    if (choice == 'm' || choice == 'd' || choice == '!' || choice == '~' || choice == 'a') {
        weapon_choice(choice);
        wield_choice = choice;
        messages("took weapon", 0);
    }
    delwin(arsenal);
    refresh();
}

bool drank_potion = FALSE;

void potion_choice (int type) {
    for ( int i = 0; i < potions_counter; i ++) {
        if (potions[i].type == type && potions[i].state == 0) {
            potions[i].state =1;
            messages("took potion", type);
            drank_potion = TRUE;
            break;
        }
    }
    if (type == 0)
        elixir_of_everlife();
    else if (type == 1)
        storm_kiss();
    else
        dragon_blood();
    potion_time_track = 10;
}

void potion_window () {
    int height = 10, width = 40;
    int start_y = (max_y - height) / 2;
    int start_x = (max_x - width) / 2;
    
    WINDOW * potion = newwin(height, width, start_y, start_x);
    wclear(potion);
    box(potion, 0, 0);
    
    int len = strlen("** POTIONS **");
    mvwprintw(potion, 1, (width - len) / 2, "** POTIONS **");

    int elix = 0, drag = 0, kiss = 0;
    for (int i = 0; i < potions_counter; i++) {
        if (potions[i].type == 0 && potions[i].state == 0) elix++;
        if (potions[i].type == 1 && potions[i].state == 0) drag++;
        if (potions[i].type == 2 && potions[i].state == 0) kiss++;
    }
    
    int identifier = 1;
    if (elix > 0)
        mvwprintw(potion, identifier++ , 1, "[e] %d Elixir of Everlife", elix);
    if (drag > 0)
        mvwprintw(potion, identifier++ + 1, 1, "[d] %d Dragon's Blood", drag);
    if (kiss > 0)
        mvwprintw(potion, identifier++ + 1, 1, "[s] %d Stormrider's Kiss", kiss);
    if (elix == 0 && drag == 0 && kiss == 0) {
        char text[] = "You don't have any potions to drink!";
        int x = (width - strlen(text)) / 2;
        mvwprintw(potion, 4, x, "%s", text);
    }

    wrefresh(potion);
    int choice = wgetch(potion);

    if (choice == 'e') potion_choice(0);
    else if (choice == 'd') potion_choice(2);
    else if (choice == 's') potion_choice(1);
    delwin(potion);
}

void inventory_window() {
    int height = 10, width = 50;
    int start_y = (max_y - height) / 2;
    int start_x = (max_x - width) / 2;

    WINDOW *inventory = newwin(height, width, start_y, start_x);
    keypad(inventory, TRUE);
    wclear(inventory);
    box(inventory, 0, 0);

    const char *choices[] = {"WEAPONS", "FOOD", "POTIONS"};
    int num_choices = sizeof(choices) / sizeof(choices[0]);
    int selected = 0;
    int key;

    while (1) {
        wclear(inventory);
        box(inventory, 0, 0);
        mvwprintw(inventory, 1, (width - strlen("** INVENTORY **")) / 2, "** INVENTORY **");

        for (int i = 0, x = 5; i < num_choices; i++, x += 15) {
            if (i == selected) {
                wattron(inventory, COLOR_PAIR(4));
                mvwprintw(inventory, 6, x, "[ %s ]", choices[i]);
                wattroff(inventory, COLOR_PAIR(4));
            } else {
                wattron(inventory, COLOR_PAIR(10));
                mvwprintw(inventory, 6, x, " %s ", choices[i]);
                wattroff(inventory, COLOR_PAIR(10));
            }
        }

        wrefresh(inventory);
        key = wgetch(inventory);

        if (key == KEY_RIGHT) {
            if (selected == 2)
                selected = 0;
            else
                selected++;
        } else if (key == KEY_LEFT) {
            if (selected == 0)
                selected = 2;
            else
                selected--;
        } else if (key == '\n') {
            break;
        }
    }

    delwin(inventory);

    switch (selected) {
        case 0: {
            weapon_window();
            break;
        }
        case 1: {
            food_window();
            break;
        }
        case 2: {
            potion_window();
            break;
        }
    }
}

void resume_save_window (int px, int py) {
    int height = 12, width = 40;
    int start_y = (max_y - height) / 2;
    int start_x = (max_x - width) / 2;

    WINDOW *resume = newwin(height, width, start_y, start_x);
    keypad(resume, TRUE);
    wclear(resume);
    box(resume, 0, 0);

    const char *choices[] = {"Quit", "Save", "Exit"};
    int num_choices = sizeof(choices) / sizeof(choices[0]);
    int selected = 0;
    int key;

    while (1) {
        wclear(resume);
        box(resume, 0, 0);
        mvwprintw(resume, 1, (width - strlen("** RESUME MENU **")) / 2 + 1, "** RESUME MENU **");
        wattron(resume, COLOR_PAIR(6));
        mvwprintw(resume, 2, (width - 32) / 2, " \\\\\\\\\\_____________________\\\"-._");
        mvwprintw(resume, 3, (width - 32) / 2, " /////~~~~~~~~~~~~~~~~~~~~~/.-'");
        wattron(resume, COLOR_PAIR(6));

        for (int i = 0, x = 3; i < num_choices; i++, x += 12) {
            if (i == selected) {
                wattron(resume, COLOR_PAIR(6));
                mvwprintw(resume, 6, x, "[ %s ]", choices[i]);
                wattroff(resume, COLOR_PAIR(6));
            } else {
                wattron(resume, COLOR_PAIR(10));
                mvwprintw(resume, 6, x, " %s ", choices[i]);
                wattroff(resume, COLOR_PAIR(10));

            }
        }

        wrefresh(resume);
        key = wgetch(resume);

        if (key == KEY_RIGHT) {
            if (selected == 2)
                selected = 0;
            else
                selected++;
        } else if (key == KEY_LEFT) {
            if (selected == 0)
                selected = 2;
            else
                selected = 0;
        } else if (key == '\n')
            break;
    }
    delwin(resume);
    switch (selected) {
        case 0:
            clear();
            star();
            gameMenu();
            break;
        case 1:
            //save_game(main_username, px, py);
            clear();
            star();
            gameMenu();
            break;
        case 2:
            break;
    }
}

void p_command () {
    mvprintw(0, 0,"What potion would you like to drink?");
    getch();
    potion_window();
}

void i_command () {
    inventory_window();
}

void E_command () {
    mvprintw(0, 0,"What food would you like to eat?");
    getch();
    food_window();
}

void add_weapon (Rooms room) {
    for (int y = room.y; y < room.y + room.height; y++) {
        for (int x = room.x; x < room.x + room.width; x++) {
            if (rand () % 50 == 0 && map[y][x] == '.') {
                int type = rand() % 4;
                char symbol;
                int num;
               if (type == 0) {
                    symbol = 'd';
                   num = 10;
                } else if (type == 1) {
                    symbol = '~';
                    num = 8;
                } else if (type == 2) {
                    symbol = 'a';
                    num = 20;
                } else {
                    symbol = '!';
                    num = 1;
                }
                if (weapon_counter == 0) {
                    symbol = 'm';
                    num = 1;
                }
                weapons[weapon_counter].symbol = symbol;
                if (weapon_counter)
                    map[y][x] = symbol;
                weapons[weapon_counter].x = x;
                weapons[weapon_counter].y = y;
                weapons[weapon_counter].state = -1;
                weapons[weapon_counter].num_collect = num;
                weapon_counter++;
            }
        }
    }
    if (current_floor == 1)
        weapInUse = &weapons[0];
}

void picked_tresure () {
    messages("picked up treasure", 0);
    win_game();
}

void add_treasure (Rooms room) {
    bool flag = FALSE;
    for (int y = room.y; y < room.y + room.height; y++) {
        if (flag)
            break;;
        for (int x = room.x; x < room.x + room.width; x++) {
            if (flag)
                break;;
            if (rand () % 20 == 0 && map[y][x] == '.') {
                attron(COLOR_PAIR(9));
                map[y][x] = 'T';
                attroff(COLOR_PAIR(9));
                flag = TRUE;
                refresh();
            }
        }
    }

    if (!flag) {
        int randx = room.x + room.width / 2;
        int randy = room.y + room.height / 3;
        map[randy][randx] = 'T';
    }
}

void add_gold (Rooms room) {
    for (int y = room.y; y < room.y + room.height; y++) {
        for (int x = room.x; x < room.x + room.width; x++) {
            if (rand () % 50 == 0 && map[y][x] == '.') {
                int color = rand() % 10;
                if ((color % 3) == 0)
                    color = 3;
                else
                    color = 5;
                golds[gold_counter].type = color;
                map[y][x] = '$';
                golds[gold_counter].x = x;
                golds[gold_counter].y = y;
                golds[gold_counter].state = -1;
                gold_counter++;
            }
        }
    }
}

void elixir_of_everlife () {
    health += 10;
    health_update(health);
}

void dragon_blood () {
    weapSpeed = 2;
    potion_time_track = 10;
}

void storm_kiss () {
    speed = 2;
    potion_time_track = 10;
}

void add_potion (Rooms room) {
    int prob;
    int type = room.type;
    if (type == 1)
        prob = 10;
    else
        prob = 50;
    for (int y = room.y; y < room.y + room.height; y++) {
        for (int x = room.x; x < room.x + room.width; x++) {
            if (rand () % prob == 0 && map[y][x] == '.') {
                int type = rand() % 3;
                map[y][x] = 'p';
                potions[potions_counter].x = x;
                potions[potions_counter].y = y;
                potions[potions_counter].type = type;
                potions[potions_counter].state = -1;
                potions_counter++;
            }
        }
    }
}

void spoil_normal () {
    for (int j = 0; j < foods_counter; j ++) {
        if (foods[j].fresh  == 1 && foods[j].state == 0)
            foods[j].fresh  = 0;
        else if (foods[j].fresh  == 0 && foods[j].state == 0)
            foods[j].fresh = -1;
    }
    timeFood = 50;
}

void add_food (Rooms room) {
    //manual probabilyty
    for (int y = room.y; y < room.y + room.height; y++) {
        for (int x = room.x; x < room.x + room.width; x++) {
            if (rand () % 50 == 0 && map[y][x] == '.') {
                int color = rand() % 3;
                if (color == 0) {
                    color = 5;
                    foods[foods_counter].fresh = 1;
                } else if (color == 1) {
                    color = 2;
                    foods[foods_counter].fresh  = 0;
                } else {
                    color = 6;
                    foods[foods_counter].fresh  = 1;
                }
                foods[foods_counter].color = color;
                map[y][x] = '%';
                foods[foods_counter].x = x;
                foods[foods_counter].y = y;
                foods[foods_counter].name = food_menu(color);
                foods[foods_counter].state = -1;
                foods_counter++;
            }
        }
    }
}

void hunger_update () {
    time_t current_time;
    time(&current_time);
    if (difftime(current_time, last_hunger_update) >= 20 ) {
        hunger -= 5;
        
        if (hunger < 0)
            hunger = 0;
        last_hunger_update = current_time;
    }
}

void health_update (int maybe) {
    time_t current_time;
    time(&current_time);

    if (difftime(current_time, last_health_update) >= 20 && hunger >= 70 ) {
        int speed = 1;
        if (maybe == 1 && potion_time_track > 0)
            speed = 2;
        health += speed;
        
        if (health < 0)
            health = 0;
        last_health_update = current_time;
    } else if (difftime(current_time, last_health_update) >= 20 && hunger < 70) {
        health--;
        if (health < 0)
            health = 0;
        last_health_update = current_time;
    }
}

void generateMap (){
    clear();
    curs_set(0);
    bool treasure_room_place = FALSE;
    init_map();
    while (rooms_counter[current_floor] < ROOM_COUNTER) {
        int type = 0;
        Rooms room;
        room.width = 4 + rand() % 4;
        room.height = 4 + rand() % 4;
        room.x = rand() % (WIDTH - room.width - 1);
        room.y = rand() % (HEIGHT - room.height - 1) + 2;
        
        bool coll = FALSE;
        
        for ( int i = 0; i < rooms_counter[current_floor] ; i++ ){
            if (collision(room, rooms[current_floor][i])) {
                coll = TRUE;
                break;
            }
        }
        if (!coll) {
           if (current_floor < 4) {
                if (!(rand() % 6))
                    room.type = 1;
                else
                    room.type = 0;
            }
            else
                room.type = 0;
            doorrrr(room);
            addRooms(room);
            add_trap(room);
            add_food(room);
            add_gold(room);
            add_weapon(room);
            add_potion(room);
            if (room.type != 1)
                add_monster(room);
            if (rooms_counter[current_floor] > 0) {
                corridor(
                         rooms[current_floor][rooms_counter[current_floor] - 1].x + rooms[current_floor][rooms_counter[current_floor] - 1].width/ 2,
                    rooms[current_floor][rooms_counter[current_floor] - 1].y + rooms[current_floor][rooms_counter[current_floor] - 1].height/ 2,
                    room.x + (room.width - 2) / 2,
                    room.y + (room.height - 2) / 2
                );
                if ( rand () % 6 == 0)
                    add_hidden_door(room);
            }
            pillar(room);
            rooms[current_floor][rooms_counter[current_floor]++] = room;
        }
    }
    
    if (current_floor == 4 && !treasure_room_place) {
        int roomn = rand () % 5 + 1;
        rooms[current_floor][roomn].type = 2;
        treasure_room_place = true;
        add_treasure (rooms[current_floor][roomn]);
    }

    int room_with_stairs = rand () %6;
    int room_with_key = rand () %6;
    add_stairs(rooms[current_floor][room_with_stairs]);
    add_master_key(rooms[current_floor][room_with_key]);
    if (!(rand() % 4))
        locked_door(rooms[current_floor][0]);
    int px = rooms[current_floor][0].x + 1, py = rooms[current_floor][0].y + 1;
    player_in_room(px, py, rooms[current_floor], rooms_counter[current_floor]);
    
    int ch;
    
    while (1) {
        //g_state = 0;
        curs_set(0);
        refresh();
        cbreak();
        noecho();
        health_bar(health);
        hunger_bar(hunger);
        print_floor();
        desplay_gold();
        display_hits();
        printWhole();
        attron(COLOR_PAIR(characterColor));
        mvaddch(py, px, '@');
        attroff(COLOR_PAIR(characterColor));
        refresh();
        
        ch = getch();
        if (ch == 'q') break;
        int nx = px, ny = py;
        if (potion_time_track > 0)
            potion_time_track--;
        if (timeFood > 0)
            timeFood--;
        if (timeFood == 0)
            spoil_normal();
        if (potion_time_track <= 0 && (drank_potion || magicalFood)) {
            speed = 1;
            if (drank_potion)
                messages("potion time over", 0);
            drank_potion = FALSE;
        }
        if (ch == KEY_UP || ch == 'j')
            ny -= speed;
        else if (ch == KEY_DOWN || ch == 'k')
            ny += speed;
        else if (ch == KEY_LEFT || ch == 'h')
            nx -= speed;
        else if (ch == KEY_RIGHT || ch == 'l')
            nx += speed;
        else if (ch =='u') {
            ny -= speed;
            nx += speed;
        } 
        else if (ch == 'y') {
            nx -= speed;
            ny -= speed;
        }
        else if (ch =='n') {
            nx += speed;
            ny += speed;
        }
        else if (ch =='b') {
            ny += speed;
            nx -= speed;
        }
        else if (ch == 'm') {
            cheat_code_M();
        }
        else if (ch == 's') {
            cheat_code_s(ny, nx);
        } else if (ch == 'e') {
            E_command();
        } else if (ch == 'i') {
            i_command();
        } else if (ch == 'p') {
            p_command();
        } else if (ch == 'g') {
            cheat_g();
        } else if (ch == 'w') {
            messages("weapon in bag", weapInUse->symbol);
            weapon_in_bag();
        } else if (ch == ' ') {
            if (weapInUse) {
                if ((weapInUse->symbol == 'd' || weapInUse->symbol == '~' || weapInUse->symbol == 'a')) {
                    mvprintw(0, 0, "Choose a direction to throw the weapon!");
                    int direction = getch();
                    char dir[3];
                    int type = 0;
                    if (direction == KEY_UP) strcpy(dir, "u");
                    else if (direction == KEY_DOWN) strcpy(dir, "d");
                    else if (direction == KEY_LEFT) strcpy(dir, "l");
                    else if (direction == KEY_RIGHT) strcpy(dir, "r");
                    else if (direction == 'u') strcpy(dir, "ur");
                    else if (direction == 'b') strcpy(dir, "dl");
                    else if (direction == 'n') strcpy(dir, "dr");
                    else if (direction == 'y') strcpy(dir, "ul");
                    
                    if (weapInUse->symbol == 'd') type = 0;
                    else if (weapInUse->symbol == '~') type = 1;
                    else if (weapInUse->symbol == 'a') type = 2;
                    dagger_wand_arrow_attack(px, py, dir, type);
                } else {
                    Monsters mon = monster_in_room(px, py);
                    if (mon.x != -1 && mon.y != -1)
                        player_attack(mon.x, mon.y, weapInUse->symbol);
                    else
                        messages("no monster", 0);
                }
            }
            else
                messages("no weapon", 0);
    } else if (ch == 'f') {
            int condition [2] = {};
            int direction = getch();
            if (direction == KEY_UP || direction == 'j') condition[0] =-1;
            else if (direction == KEY_DOWN || direction == 'k') condition[0]=1;
            else if (direction == KEY_LEFT || direction == 'h') condition[1]=-1;
            else if (direction == KEY_RIGHT || direction == 'l') condition[1]=1;
            else if (direction =='y') {
                condition[0] = -1;
                condition[1] = -1;
            } else if (direction =='u') {
                condition[0] = -1;
                condition[1] = 1;
            }  else if (direction =='b') {
                condition[0] = 1;
                condition[1] = -1;
            }  else if (direction =='n') {
                condition[0] = 1;
                condition[1] = 1;
            }
            
            while (map[ny][nx] == '.') {
                nx +=condition[1];
                ny += condition[0];
                if (map[ny][nx] == '.') {
                    px = nx;
                    py = ny;
                }
            }
        }
        else if (ch == 'r')
            resume_save_window(px, py);
        
        if ( map[ny][nx] == '#' || map[ny][nx] == '+' || map[ny][nx] == '^' || map[ny][nx] == '?') {
            px = nx;
            py = ny;
            player_in_room(px, py, rooms[current_floor], rooms_counter[current_floor]);
        } else if (map[ny][nx] == '<') {
            char enter = getch();
            hoora(enter);
            px = nx;
            py = ny;
        }
        else if (map[ny][nx] == '@') {
            bool master = FALSE;
            if (master_key[current_floor] && master_keys_broken[current_floor] == FALSE) {
                px = nx;
                py = ny;
                master_key[current_floor] = FALSE;
                master = TRUE;
            } else {
                if (master_keys_broken[current_floor] == TRUE) {
                    messages("key broke", 0);
                    for ( int i = 0; i < current_floor; i ++) {
                        if (master_keys_broken[i]) {
                            messages("fix key", 0);
                            char forge = getch();
                            if (forge == 'y') {
                                master_keys_broken[current_floor] = FALSE;
                                messages("key fixed", 0);
                                master = TRUE;
                            }
                        }
                    }
                    
                }
            }
            if (master) {
                px = nx;
                py = ny;
            } else {
                int which_door = LockedPasswordGet(nx, ny);
                if (locked[which_door].state) {
                    px = nx;
                    py = ny;
                }
            }
        } else if (map[ny][nx] == '&') {
            show_password(nx, ny);
        } else if (map[ny][nx] == '*') {
            if (!g_state) {
                int break_prob = rand() % 10;
                if (!break_prob) {
                    master_keys_broken [current_floor] = TRUE;
                }
                px = nx;
                py = ny;
                pick_up(ny, nx);
                if (first_key[current_floor])
                    messages("master key found", 0);
                if (first_key[current_floor]) {
                    master_key[current_floor] = TRUE;
                    first_key[current_floor] = FALSE;
                    g_state = 0;
                }
            } else {
                px = nx;
                py = ny;
            }
        } else if (map[ny][nx] == '%') {
            px = nx;
            py = ny;
            if (!g_state) pick_up(ny, nx);
            g_state = 0;
        } else if (map[ny][nx] == 'T') {
            px = nx;
            py = ny;
            if (!g_state) picked_tresure();
            g_state = 0;
        }
        else if (map[ny][nx] == '$') {
            px = nx;
            py = ny;
            if (!g_state) pick_up(ny, nx);
            g_state = 0;
        } else if (map[ny][nx] == 'm' || map[ny][nx] == 'd' || map[ny][nx] == '~' || map[ny][nx] == 'a' || map[ny][nx] == '!') {
            px = nx;
            py = ny;
            if (!g_state) pick_up(ny, nx);
            g_state = 0;
        } else if (map[ny][nx] == 'p') {
            px = nx;
            py = ny;
            if (!g_state) pick_up(ny, nx);
            g_state = 0;
            
        }
        else if(map[ny][nx] == '|' || map[ny][nx] == '-')
            show_door(ny, nx);
        else if (map[ny][nx] == '.') {
            for ( int i = 0; i < trap_counter; i++) {
                if(traps[i].x == nx && traps[i].y == ny)
                    show_trap(ny, nx);
            }
            px = nx;
            py = ny;
            Rooms currentRoom = roomFinder(px, py);
            if (currentRoom.x != -1 && currentRoom.y != -1)
                while_inside_room(px, py, currentRoom);
        }
        health_update(0);
        hunger_update();
        refresh();
    }
}

void difficulty() {
    int c, choice = 0;
    char menu_name[20] = "Choose difficulty";
    char *choices[] = {"Easy", "Medium", "Hard", "Exit"};
    int num_choices = sizeof(choices) / sizeof(char *);
    while (1) {
        clear();
        choosing(choices, choice, menu_name, num_choices);
        c = getch();
        if (c == KEY_UP){
            if (choice == 0)
                choice = num_choices - 1;
            else
                choice--;
        }
        else if (c == KEY_DOWN){
            if (choice == num_choices - 1)
                choice = 0;
            else
                choice++;
        }
        else if (c == 10)
            break;
    }
    settingsMenu();
}

void customization(){
    int c, choice = 0;
    char menu_name[20] = "Customize Menu";
    char *choices[] = {"Red", "Green", "Blue", "White", "Yellow", "Back"};
    int num_choices = sizeof(choices) / sizeof(char *);
    while (1){
        clear();
        choosing(choices, choice, menu_name, num_choices);
        attron(COLOR_PAIR(6));
        mvprintw(0, 40, "Here you can pick the color of your character! By default the color is the same as this menu's name color!");
        attroff(COLOR_PAIR(6));
        refresh();
        c = getch();
        if (c == KEY_UP){
            if (choice == 0)
                choice = num_choices - 1;
            else
                choice--;
        }
        else if (c == KEY_DOWN){
            if (choice == num_choices - 1)
                choice = 0;
            else
                choice++;
        }
        else if (c == 10){
            if (choice == 0)
                characterColor = 2;
            else if (choice == 1)
                characterColor = 9;
            else if (choice == 2)
                characterColor = 4;
            else if (choice == 3)
                characterColor = 10;
            else if (choice == 4)
                characterColor = 5;
            else
                break;
        }
    }
    settingsMenu();
}

void settingsMenu(){
    int c, choice = 0;
    char menu_name[20] = "Settings Menu";
    char *choices[] = {
        "Difficulty",
        "Customize your character",
        "Back"
    };
    int num_choices = sizeof(choices) / sizeof(char *);
    while (1){
        clear();
        choosing(choices, choice, menu_name, num_choices);
        c = getch();
        if (c == KEY_UP){
            if (choice == 0)
                choice = num_choices - 1;
            else
                choice--;
        }
        else if (c == KEY_DOWN){
            if (choice == num_choices - 1)
                choice = 0;
            else
                choice++;
        }
        else if (c == 10){
            if (choice == 0)
                difficulty();
            else if (choice == 1)
                customization();
            else if (choice == 2)
                gameMenu();
        }
    }
}

int compare(const void *a, const void *b) {
    return ((User *)b)->score - ((User *)a)->score;
}

void scoresTable(const char *logged_in_user, bool ok) {
    curs_set(0);
    FILE *file = fopen("scores.txt", "r");
    User users[100];
    int count = 0;
    while (fscanf(file, "%s %d %d %d %d",
                  users[count].username,
                  &users[count].score,
                  &users[count].gold,
                  &users[count].games_played,
                  &users[count].experience_time) == 5){
        count++;
    }
    fclose(file);

    qsort(users, count, sizeof(User), compare);
    int max_display = 10;
    int current_page = 0;
    int total_pages = (count + max_display - 1) / max_display;
    int c;
    time_t current_time = time(NULL);

    do{
        clear();
        mvprintw(0, 10, "Leaderboard: Top Players (Page %d of %d)", current_page + 1, total_pages);
        mvprintw(2, 0, "Rank \tUsername\tScore\tGold\tGames\tExperience");

        int start_index = current_page * max_display;
        int end_index = start_index + max_display;
        if (end_index > count)
            end_index = count;
        for (int i = start_index; i < end_index; i++) {
            double seconds_passed = difftime(current_time, users[i].experience_time);
            int days_passed = seconds_passed / (60 * 60 * 24);
            if (i == 0){
                attron(COLOR_PAIR(6) | A_BOLD);
                mvprintw(4 + i - start_index, 0, "GOAT \t%s\t\t%d\t%d\t%d\t%d\t🥇",
                         users[i].username, users[i].score, users[i].gold,
                         users[i].games_played, days_passed);
                attroff(COLOR_PAIR(6) | A_BOLD);
            }
            else if (i == 1){
                attron(COLOR_PAIR(2) | A_BOLD);
                mvprintw(4 + i - start_index, 0, "Legend \t%s\t\t%d\t%d\t%d\t%d\t🥈",
                         users[i].username, users[i].score, users[i].gold,
                         users[i].games_played, days_passed);
                attroff(COLOR_PAIR(2) | A_BOLD);
            }
            else if (i == 2) {
                attron(COLOR_PAIR(3) | A_BOLD);
                mvprintw(4 + i - start_index, 0, "Champ \t%s\t\t%d\t%d\t%d\t%d\t🥉",
                         users[i].username, users[i].score, users[i].gold,
                         users[i].games_played, days_passed);
                attroff(COLOR_PAIR(3) | A_BOLD);
            }
            else if (strcmp(users[i].username, logged_in_user) == 0) {
                attron(COLOR_PAIR(4) | A_BOLD);
                mvprintw(4 + i - start_index, 0, ">%d\t%s\t\t%d\t%d\t%d\t%d\t😉",
                         i + 1, users[i].username, users[i].score, users[i].gold,
                         users[i].games_played, days_passed);
                attroff(COLOR_PAIR(4) | A_BOLD);
            }
            else {
                attron(COLOR_PAIR(5));
                mvprintw(4 + i - start_index, 0, " %d\t%s\t\t%d\t%d\t%d\t%d",
                         i + 1, users[i].username, users[i].score, users[i].gold,
                         users[i].games_played, days_passed);
                attroff(COLOR_PAIR(5));
            }
            refresh();
        }

        mvprintw(18, 0, "Press UP/DOWN to navigate through the scores! Press q in order to go back!");
        refresh();

        c = getch();
        if (c == KEY_UP){
            if (current_page == 0)
                current_page = total_pages - 1;
            else
                current_page--;
        }
        else if (c == KEY_DOWN){
            if (current_page == total_pages - 1)
                current_page = 0;
            else
                current_page++;
        }
    } while (c != 'q');
    if (ok)
        gameMenu();
    else
        mainMenu();
}

void profile(char *mainUsername){
    clear();
    curs_set(0);
    FILE *file = fopen("scores.txt", "r");
    char line[100];
    char username[50];
    int score, gold, counter, time;
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(0, max_x / 2 - strlen("Profile Menu"), "Profile Menu");
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    time_t current_time = time(NULL);
    while (fgets(line, sizeof(line), file)){
        fscanf(file, "%s %d %d %d %d", username, &score, &gold, &counter, &time);
        if (strcmp(mainUsername, username) == 0){
            attron(COLOR_PAIR(7));
            mvprintw(max_y / 2 - 4, max_x / 2 - 10, "Username: %s   🫵", username);
            attron(A_BOLD);
            mvprintw(max_y / 2 - 2, max_x / 2 - 10, "Total score: %d   😳", score);
            attroff(A_BOLD);
            mvprintw(max_y / 2, max_x / 2 - 10, "Total gold: %d   🏆", gold);
            mvprintw(max_y / 2 + 2, max_x / 2 - 15, "Total games Played: %d   😈", counter);
            double secs = difftime(current_time, time);
            int days = secs / (60 * 24 * 24);
            mvprintw(max_y / 2 + 4, max_x / 2 - 15, "How many seconds? %d   👍", days);
            attroff(COLOR_PAIR(7));
            break;
        }
    }
    mvprintw(0, 0, "Press any key to go back!");
    refresh();
    getch();
    gameMenu();
}

void gameMenu(){
    curs_set(0);
    int c, choice = 0;
    char menu_name[20] = "Game Menu";
    char *choices[] = {
        "Create new game",
        "Load previous game",
        "Scores table",
        "Settings",
        "Profile"
    };
    int num_choices = sizeof(choices) / sizeof(char *);
    while (1){
        clear();
        choosing(choices, choice, menu_name, num_choices);
        c = getch();
        if (c == KEY_UP){
            if (choice == 0)
                choice = num_choices - 1;
            else
                choice--;
        }
        else if (c == KEY_DOWN){
            if (choice == num_choices - 1)
                choice = 0;
            else
                choice++;
        }
        else if (c == 10){
            if (choice == 0){
                init_map();
                generateMap();
            }
            else if (choice == 1){
                getch();
                refresh();
            }
            else if (choice == 2)
                scoresTable(main_username, TRUE);
            else if (choice == 3)
                settingsMenu();
            else
                profile(main_username);
        }
    }
}

int is_username_taken(const char *username) {
    FILE *file = fopen("users.txt", "r");
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char saved_username[50];
        sscanf(line, "%[^,]", saved_username);
        if (strcmp(saved_username, username) == 0){
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
    file = fopen("scores.txt", "a");
    fprintf(file, "%s %d %d %d %d\n", username, 0, 0, 0, 0);
    fclose(file);
}

void save_guest(){
    FILE *file = fopen("scores.txt", "a");
    fprintf(file, "Guest %d %d %d %d", 0, 0, 0, 0);
    fclose(file);
    strcpy(main_username, "Guest");
}

void generatePassword(char *password){
    curs_set(0);
    int lenPass = (rand() % 4) + 7;
    char genPass[lenPass + 1];
    const char lower[] = "abcdefghijklmnopqrstuvwxyz";
    const char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char digits[] = "0123456789";
    const char all[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int i;
    genPass[0] = lower[rand() % (sizeof(lower) - 1)];
    genPass[1] = upper[rand() % (sizeof(upper) - 1)];
    genPass[2] = digits[rand() % (sizeof(digits) - 1)];
    for (i = 3; i < lenPass; i++)
        genPass[i] = all[rand() % (sizeof(all) - 1)];
    for (i = 0; i < lenPass; i++){
        int randindex = rand() % lenPass;
        char temp = genPass[i];
        genPass[i] = genPass[randindex];
        genPass[randindex] = temp;
    }
    genPass[lenPass] = '\0';
    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(5, max_x / 2 - 40, "Your generated password is: %s  Please save it so you don't lose it! Press any key to continiue", genPass);
    attroff(COLOR_PAIR(4) | A_BOLD);
    refresh();
    stpcpy(password, genPass);
    getch();
}

void create_new_user(){
    char username[50], password[50], email[50];
    int valid = 0;
    curs_set(1);
    echo();
    while (1){
        clear();
        mvprintw(0, 0, "Create New User Menu");
        mvprintw(2, 0, "Enter Username: ");
        refresh();
        inputt(username, 50, 1);
        if (is_username_taken(username)){
            mvprintw(4, 0, "Error: Username already exists.");
            mvprintw(5, 0, "Press any key to try again!");
            refresh();
            getch();
            continue;
        }
        bool check = TRUE;
        while (!valid){
            move(4, 0);
            clrtoeol();
            mvprintw(4, 0, "Enter Password: "); 
            mvprintw(5, 0, "Press enter if you want a password to be generated for you! Press any other key and you will be able to enter your own password!");
            int k;
            k = getch();
            if (k == 10){
                move(5, 0);
                clrtoeol();
                check = FALSE;
                generatePassword(password);
                break;
            }
            refresh();
            move(4, 16);
            inputt(password, 50, 0);
            if (validate_password(password))
                valid = 1;
            else{
                mvprintw(6, 0, "Error: Password must be at least 7 characters and include:");
                mvprintw(7, 0, "At least one uppercase letter");
                mvprintw(8, 0, "At least one lowercase letter");
                mvprintw(9, 0, "At least one digit");
                attr_on(A_UNDERLINE, NULL);
                mvprintw(10, 0, "Press any key to try again!");
                refresh();
                getch();
                attr_off(A_UNDERLINE, NULL);
                mvprintw(10, 0, "Press any key to try again!");
                refresh();
            }
        }
        valid = 0;
        if (check){
            move(5, 0);
            clrtoeol();
        }
        refresh();
        while (!valid){
            curs_set(1);
            move(6, 0);
            clrtoeol();
            refresh();
            mvprintw(6, 0, "Enter Email: ");
            refresh();
            inputt(email, 50, 1);
            if (validate_email(email))
                valid = 1;
            else{
                mvprintw(8, 0, "Error: Invalid email format (ex: xxx@y.zzz)");
                attr_on(A_UNDERLINE, NULL);
                mvprintw(9, 0, "Press any key to try again!");
                refresh();
                getch();
                attr_off(A_UNDERLINE, NULL);
                mvprintw(9, 0, "Press any key to try again!");
                refresh();
            }
        }
        clear();
        save_user(username, password, email);
        attron(COLOR_PAIR(7) | A_BLINK);
        mvprintw(max_y / 2 - 1, max_x / 2 - strlen("User created successfully!") / 2, "User created successfully!");
        mvprintw(max_y / 2 + 1, max_x / 2 - strlen("Press any key to return to the main menu to enjoy the game!") / 2,
         "Press any key to return to the main menu to enjoy the game!");
        attroff(COLOR_PAIR(7) | A_BLINK);
        refresh();
        curs_set(0);
        getch();
        break;
    }
    curs_set(0);
    strcpy(main_username, username);
    gameMenu();
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

void userLogin(){
    char username[50], password[50];
    while (1){
        clear();
        mvprintw(2, 0, "Enter Username: ");
        refresh();
        curs_set(1);
        inputt(username, 50, 1);
        mvprintw(4, 0, "If you want to see your password while you type it press 1, Press any other key and you will see *: ");
        refresh();
        char flag = getch();
        mvprintw(6, 0, "Enter Password: ");
        refresh();
        if (flag == '1')
            inputt(password, 50, 1);
        else
            inputt(password, 50, 0);
        if (verify(username, password)){
            attron(COLOR_PAIR(3) | A_BOLD);
            mvprintw(8, 0, "Login successful! Welcome, %s.", username);
            attroff(COLOR_PAIR(3) | A_BOLD);
            getch();
            refresh();
            break;
        }
        else{
            mvprintw(8, 0, "Error: Invalid username or password.");
            attron(COLOR_PAIR(2));
            mvprintw(9, 0, "Press any key to try again!");
            attroff(COLOR_PAIR(2));
        }
        curs_set(0);
        refresh();
        getch();
    }
    stpcpy(main_username, username);
    gameMenu();
}

void forgotPassword(){
    while (1){
        clear();
        curs_set(1);
        char username[50], email[50];
        mvprintw(0, 0, "So you forgot your username.");
        mvprintw(4, 0, "Please enter your user name in order to find your password: ");
        refresh();
        inputt(username, 50, 1);
        bool check = FALSE;
        FILE *file = fopen("users.txt", "r");
        char line[100], saved_username[50], saved_password[50], saved_email[50];;
        while (fgets(line, sizeof(line), file)){
            sscanf(line, "%[^,],%[^,],%s", saved_username, saved_password, saved_email);
            if (strcmp(saved_username, username) == 0){
                check = TRUE;
                break;
            }
        }
        fclose(file);
        if (!check){
            curs_set(0);
            mvprintw(6, 0, "Such Username wasn't found! Press Enter if you want to try again! Press any other key to go back to the Login Menu!");
            int a = getch();
            if (a != 10)
                Login();
            continue;
        }
        check = FALSE;
        for (int i = 4; i > 0; i--){
            curs_set(1);
            move(6, 0);
            clrtoeol();    
            mvprintw(6, 0, "Enter the email that you used to regester: ");
            refresh();
            inputt(email, 50, 1);
            if (strcmp(saved_email, email) != 0){
                curs_set(0);
                mvprintw(8, 0, "Not the right email! You have %d tries left! Press enter to retry! Press any other key to go back to the Login Menu!", i - 1);
                refresh();
                int a = getch();
                if (a != 10)
                    Login();
                else
                    continue;;
            }
            else{
                check = TRUE;
                break;
            }
        }
        clear();
        curs_set(0);
        if (check){
            attron(COLOR_PAIR(3));
            mvprintw(max_y / 2, max_x / 2 - 10, "Your forgotten Password is: %s", saved_password);
            attroff(COLOR_PAIR(3));
            mvprintw(max_y / 2 + 2, max_x / 2 - 25, "Press any key to return to the Login menu! This time don't lose your password!");
        }
        else{
            attron(COLOR_PAIR(2));
            mvprintw(max_y / 2, max_x / 2 - 45, "You have reached the limit of your mistakes! Press any key to go back to the Login menu! Then you can try again");
            refresh();
            attroff(COLOR_PAIR(2));
        }
        getch();
        Login();
    }
    Login();
}

void Login(){
    clear();
    curs_set(0);
    int c, choice = 0;
    char menu_name[20] = "Login Menu";
    char *choices[] = {
        "Login",
        "Login as a guest",
        "Forgot password",
        "Back"
    };
    int num_choices = sizeof(choices) / sizeof(char *);
    while (1){
        clear();
        choosing(choices, choice, menu_name, num_choices);
        c = getch();
        if (c == KEY_UP){
            if (choice == 0)
                choice = num_choices - 1;
            else
                choice--;
        }
        else if (c == KEY_DOWN){
            if (choice == num_choices - 1)
                choice = 0;
            else
                choice++;
        }
        else if (c == 10){
            if (choice == 0)
                userLogin();
            else if (choice == 1){
                clear();
                mvprintw(max_y / 2, max_x / 2 - strlen("You are now playing as a guest! Press any key to enter the game menu!") / 2, 
                "You are now playing as a guest! Press any key to enter the game menu!");
                getch();
                save_guest();
                gameMenu();
            }
            else if (choice == 2)
                forgotPassword();
            else
                mainMenu();
        }
    }
}

void mainMenu(){
    int c, choice = 0;
    char menu_name[20] = "Main Menu";
    char *choices[] = {
        "Create New User",
        "LogIn",
        "Scores table"
    };
    int num_choices = sizeof(choices) / sizeof(char *);

    while (1){
        clear();
        choosing(choices, choice, menu_name, num_choices);
        c = getch();
        if (c == KEY_UP){
            if (choice == 0)
                choice = num_choices - 1;
            else
                choice--;
        }
        else if (c == KEY_DOWN){
            if (choice == num_choices - 1)
                choice = 0;
            else
                choice++;
        }
        else if (c == 10){
            if (choice == 0)
                create_new_user();
            else if (choice == 1)
                Login();
            else
                scoresTable(main_username, FALSE);
        }
    }
}

void draw_star(int x, int y, int phase) {
    char *star[4] = {
        "+",
        "x",
        "*",
        "o"
    };
    mvprintw(y, x, "%s", star[phase % 4]);
}

void star(){
    attron(COLOR_PAIR(6));
    int max_x, max_y;
    int center_x, center_y;
    int radius = 5;
    double angle = 0;
    getmaxyx(stdscr, max_y, max_x);
    center_x = max_x / 2;
    center_y = max_y / 2;
    time_t start_time = time(NULL), current_time;
    while (1){
        current_time = time(NULL);
        if (current_time - start_time >= 5)
            break;
        clear();
        for (int i = 0; i < 8; i++) {
            double theta = angle + i * M_PI / 4;
            int x = center_x + radius * cos(theta);
            int y = center_y + radius * sin(theta);
            draw_star(x, y, i);
        }

        mvprintw(center_y, center_x, "O");

        refresh();       
        usleep(50000);    
        angle += 0.1;     
    }
    attroff(COLOR_PAIR(6));
}

void draw(){
    clear();
    const char *dungeon[] = {
        "##########",
        "#....#...#",
        "#.##.#.#.#",
        "#.#...#..#",
        "#.#.####.#",
        "#...#....#",
        "###.#.##.#",
        "#....@...#",
        "##########"
    };
    int height = sizeof(dungeon) / sizeof(dungeon[0]);
    attron(COLOR_PAIR(3));
    mvprintw(0, 0, "Welcome! I hope you enjoy this game");
    mvprintw(2, 0, "Here, Press any key to start the game!");
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(2));
    mvprintw(5, max_x - 20, "Good luck");
    mvprintw(max_y - 8, max_x - 20, "You will need it!");
    attroff(COLOR_PAIR(2));
    for (int i = 0; i < height; i++)
        mvprintw((max_y - 9) / 2 + i, (max_x - 10) / 2, "%s", dungeon[i]);
    refresh();
    getch();
}

void win_game(){
    clear();
    refresh();
    int start_y = (LINES - 10) / 2;
    int start_x = (COLS - 50) / 2;
    WINDOW *win = newwin(10, 50, start_y, start_x);
    wbkgd(win, COLOR_PAIR(12));
    box(win, 0, 0);

    
    char msg [20] = "YOU WIN!";
    mvwprintw(win, 2, (50 - strlen(msg)) / 2, "%s", msg);
    mvwprintw(win, 4, 2, "%s", "With the treasure in your hands and the dungeon");
    mvwprintw(win, 5, 2, "%s", "fading behind, you emerge victorious at last!");
    mvprintw(7, 2, "Press any key to enter Hall of Fame!");
    wrefresh(win);
    getch();
    delwin(win);
    score = hits*50*0.5 + gold*0.5;
    //save_score();
}

void lose_game(){
    clear();
    refresh();
    int start_y = (LINES - 10) / 2;
    int start_x = (COLS - 50) / 2;
    WINDOW *lose = newwin(10, 50, start_y, start_x);
    wbkgd(lose, COLOR_PAIR(8));
    box(lose, 0, 0);

    
    char msg [20] = "YOU LOSE!";
    mvwprintw(lose, 2, (50 - strlen(msg)) / 2, "%s", msg);
    mvwprintw(lose, 4, 2, "%s", "Darkness consumes you as you fall...");
    mvwprintw(lose, 5, 2, "%s", "The dungeon claims another soul.");
    mvprintw(7, 2, "Press any key to enter Hall of Fame!");
    
    wrefresh(lose);
    getch();
    delwin(lose);
    score = hits*50*0.5 + gold*0.5;
    //save_score();
}

void messages(char *what_happened, int maybe){
    move(0, 0);
    if (strcmp(what_happened, "key broke") == 0) {
        printw("The Master Key breaks.\n");
    } else if (strcmp(what_happened, "fix key") == 0) {
        printw("Would you like to mend two broken Master Keys to enter? (y/n)\n");
    } else if (strcmp(what_happened, "key fixed") == 0) {
        printw("Two Master Keys have been mended. You can enter.\n");
    } else if (strcmp(what_happened, "picked up key") == 0) {
        attron(COLOR_PAIR(5));
        printw("You picked up a Master Key!\n");
        attroff(COLOR_PAIR(5));
    } else if (strcmp(what_happened, "cheat code M") == 0) {
        attron(COLOR_PAIR(5));
        printw("You have entered full map mode.");
        attroff(COLOR_PAIR(5));
    } else if (strcmp(what_happened, "trap around") == 0) {
        attron(COLOR_PAIR(2));
        printw("There are %d traps around you.", maybe);
        attroff(COLOR_PAIR(2));
    } else if (strcmp(what_happened, "secret door around") == 0) {
        attron(COLOR_PAIR(2));
        printw("There are %d secret doors around you.", maybe);
        attroff(COLOR_PAIR(2));
    } else if (strcmp(what_happened, "picked up food") == 0) {
        attron(COLOR_PAIR(9));
        printw("You picked up some %s!", foods[maybe].name);
        attroff(COLOR_PAIR(9));
    } else if (strcmp(what_happened, "picked up gold") == 0) {
        char type [10];
        int added_gold;
        if (maybe == 3) {
            strcpy(type, "Skygold");
            added_gold = 100;
        } else {
            strcpy(type, "Stargold");
            added_gold = 50;
        }
        gold += added_gold;
        desplay_gold();
        attron(COLOR_PAIR(5));
        mvprintw(0, 0, "You picked up a bag of %s", type);
        mvprintw(1, 0, "earned %d more gold!",added_gold);
        attroff(COLOR_PAIR(5));
    } else if (strcmp(what_happened, "picked up weapon") == 0) {
        char weapon_name [20];
        
        if(maybe == 1) strcpy(weapon_name, "Mace");
        else if (maybe == 2) strcpy(weapon_name, "Daggger");
        else if (maybe == 3) strcpy(weapon_name, "Magic Wand");
        else if (maybe == 4) strcpy(weapon_name, "Normal Arrow");
        else strcpy(weapon_name, "Sword");
   
        printw("You picked up a %s!", weapon_name);
    } else if (strcmp(what_happened, "picked up potion") == 0) {
        char name [20];
        if (maybe == 0) strcpy(name, "Elixir of Everlife");
        else if (maybe == 2) strcpy(name, "Dragon's Blood");
        else strcpy(name, "Stormrider's Kiss");

        printw("You picked up The %s!", name);
    } else if (strcmp(what_happened, "ate food") == 0) {
        attron(COLOR_PAIR(9));
        if (maybe == 0) printw("You successfully consumed the food!");
        else if (maybe == 1) printw("The Legendry food doubles your damaging skills!");
        else if (maybe == 2) printw("The Magical food doubles your speed!");
        attroff(COLOR_PAIR(9));
            

    } else if (strcmp(what_happened, "took weapon") == 0) {
        attron(COLOR_PAIR(9));
        printw("You are now weilding the weapon!");
        attroff(COLOR_PAIR(9));
    } else if (strcmp(what_happened, "enter room") == 0) {
        if (maybe == 1) printw("You have entered an Enchant Room!");
        else if (maybe == 2) printw("You have entered The Treasure Room!");

    } else if (strcmp(what_happened, "picked up treasure") == 0) {
        attron(COLOR_PAIR(5));
        mvprintw(0, 0, "You open the ancient chest, and golden light floods the room.");
        mvprintw(1, 0, "The legendary treasure is yours!");
        attroff(COLOR_PAIR(5));
    } else if (strcmp(what_happened, "attack") == 0 ) {
        if (maybe == 0) {
            attron(COLOR_PAIR(2));
            printw("The Deamon attacks!");
            attroff(COLOR_PAIR(2));
        } else if (maybe == 1) {
            attron(COLOR_PAIR(2));
            printw("The Fire Breathing Monster scorches you with a blast of flames!");
            attroff(COLOR_PAIR(2));
        } else if (maybe == 2) {
            attron(COLOR_PAIR(2));
            printw("The Giant swings it's fist at you!");
            attroff(COLOR_PAIR(2));
        } else if (maybe == 3) {
            attron(COLOR_PAIR(2));
            printw("The Snake hisses and attacks!");
            attroff(COLOR_PAIR(2));
        } else if (maybe == 4) {
            attron(COLOR_PAIR(2));
            printw("The Undeed damages you greatly!");
            attroff(COLOR_PAIR(2));
        }
    } else if (strcmp(what_happened, "monster dead") == 0) {
        int type = monsters[maybe].num;
        if (type == 0) {
            attron(COLOR_PAIR(9));
            printw("You beat The Deamon!");
            attroff(COLOR_PAIR(9));
        } else if (type == 1) {
            attron(COLOR_PAIR(9));
            printw("The Fire Breathing Monster hisses as it's flames die out!");
            attroff(COLOR_PAIR(9));
        } else if (type == 2) {
            attron(COLOR_PAIR(9));
            printw("You defeat The Giant!");
            attroff(COLOR_PAIR(9));
        } else if (type == 3) {
            attron(COLOR_PAIR(9));
            printw("The Snake falls by you weapon!");
            attroff(COLOR_PAIR(9));
        } else if (type == 4) {
            attron(COLOR_PAIR(9));
            printw("The Undeed collapses into a pile of bones!");
            attroff(COLOR_PAIR(9));
        }
    } else if (strcmp(what_happened, "player attack") == 0) {
        int type = monsters[maybe].num;
        if (type == 0) {
            attron(COLOR_PAIR(9));
            printw("You strike The Deamon!");
            attroff(COLOR_PAIR(9));
        } else if (type == 1) {
            attron(COLOR_PAIR(9));
            printw("You hit the Fire Breathing Monster!");
            attroff(COLOR_PAIR(9));
        } else if (type == 2) {
            attron(COLOR_PAIR(9));
            printw("You do a great number on The Giant!");
            attroff(COLOR_PAIR(9));
        } else if (type == 3) {
            attron(COLOR_PAIR(9));
            printw("You attack the snake!");
            attroff(COLOR_PAIR(9));
        } else if (type == 4) {
            attron(COLOR_PAIR(9));
            printw("You attack the Undeed!");
            attroff(COLOR_PAIR(9));
        }
    } else if (strcmp(what_happened, "low health") == 0) {
        attron(COLOR_PAIR(9));
        printw("Your vision's blurry. Eat something!");
        attroff(COLOR_PAIR(9));
    } else if (strcmp(what_happened, "no monster") == 0) {
        printw("There are no monsters around you.");
    } else if (strcmp(what_happened, "no weapon") == 0) {
        printw("You're not wielding any weapon!");
    } else if (strcmp(what_happened, "weapon drop") == 0) {
        char weapon [20];
        if (maybe == 0) strcpy(weapon, "Dagger");
        else if (maybe == 1) strcpy(weapon, "Magic Wand");
        else strcpy(weapon, "Normal Arrow");
        attron(COLOR_PAIR(2));
        printw("You dropped the %s!", weapon);
        attroff(COLOR_PAIR(2));

    } else if (strcmp(what_happened, "monster frozen") == 0) {
        char name [30];
        if (monsters[maybe].type == 'D') strcpy(name, "Deamon");
        else if (monsters[maybe].type == 'F') strcpy(name, "Fire Breathing Monster");
        else if (monsters[maybe].type == 'G') strcpy(name, "Giant");
        else if (monsters[maybe].type == 'S') strcpy(name, "Snake");
        else if (monsters[maybe].type == 'U') strcpy(name, "Undeed");
        attron(COLOR_PAIR(2));
        printw("The spell paralyzes The %s!", name);
        attroff(COLOR_PAIR(2));
    } else if (strcmp(what_happened, "weapon in bag") == 0) {
        char weapon [20];
        if (maybe == 'd') strcpy(weapon, "Dagger");
        else if (maybe == '~') strcpy(weapon, "Magic Wand");
        else if (maybe == '!') strcpy(weapon, "Sword");
        else if (maybe == 'm') strcpy(weapon, "Mace");
        else strcpy(weapon, "Normal Arrow");
        
        printw("You put the %s back into it's sheath!", weapon);
    } else if (strcmp(what_happened, "put in bag") == 0) {
        attron(COLOR_PAIR(2));
        mvprintw(0, 0, "You're already wielding a weapon!");
        mvprintw(1, 0, "Put the weapon in your bag!");
        attroff(COLOR_PAIR(2));
    } else if (strcmp(what_happened, "took potion") == 0) {
        char name [30];
        if (maybe == 0) strcpy(name, "The Elixir of Everlife");
        else if (maybe == 1) strcpy(name, "The Stormrider's Kiss");
        else if (maybe == 2) strcpy(name, "The Dragon's Blood");
        attron(COLOR_PAIR(9));
        mvprintw(0, 0, "You drank %s!", name);
        if (maybe == 0) mvprintw(1, 0, "Your health begins to recover!");
        else if (maybe == 1) mvprintw(1, 0, "You move like lightning!");
        else mvprintw(1, 0, "Your weapons are now twice as deadly!");
        attroff(COLOR_PAIR(9));
    } else if (strcmp(what_happened, "potion time over") == 0) {
        printw("The potion's effects begin to wear off!");
    } else if (strcmp(what_happened, "ate spoiled food") == 0) {
        attron(COLOR_PAIR(2));
        printw("The food was spoiled. You feel sick...");
        attroff(COLOR_PAIR(2));
    } else if (strcmp(what_happened, "no long range weapon") == 0) {
        attron(COLOR_PAIR(2));
        printw("Select a direction first!");
        attroff(COLOR_PAIR(2));
    } else if (strcmp(what_happened, "new level") == 0) {
        attron(COLOR_PAIR(9));
        printw("Welcome to level %d!", current_floor);
        attroff(COLOR_PAIR(9));
    }
    move(0,0);
    getch();
    clrtoeol();
    refresh();
}

int main(){
    srand(time(NULL));
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    // if (has_colors){
    //     start_color();
    //     init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    //     init_pair(2, COLOR_RED, COLOR_BLACK);
    //     init_pair(3, COLOR_GREEN, COLOR_BLACK);
    //     init_pair(4, COLOR_BLUE, COLOR_BLACK);
    //     init_pair(5, COLOR_WHITE, COLOR_BLACK);
    //     init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    //     init_pair(7, COLOR_CYAN, COLOR_BLACK);
    // }
    start_color();
    init_pair(9, COLOR_GREEN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_RED);
    init_pair(10, COLOR_WHITE, COLOR_BLACK);
    init_pair(11, COLOR_BLACK, COLOR_YELLOW);
    init_pair (12, COLOR_BLACK, COLOR_GREEN);
    star();
    refresh();
    getmaxes();
    draw();
    mainMenu();
    getch();
    endwin();
    return 0;
}