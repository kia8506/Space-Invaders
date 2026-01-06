#include "Assets.h"
#include "Console.h"

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

void change_faze(string);
void rand_color();
void reset_color();
void create_file();
void show_score(int, int, int);
int print_on_screen(string, int, int, bool, int, int);
void place_number(int, int, int);
void place_letter(int, int, int, bool, int, int);
void place_tank(int, int);
void place_backuptank(int ,int);
void place_invader(int, int, int, bool);
void place_invader_rocket(int, int, int, int);
void place_barrier(int, int, int, int, int);
void place_hundred(int, int);
void update();
void show_destruction(int, int, int);
void initialize();
void button_initializer();
void invader_initializer();
void barrier_initializer();
void initialize_Classic_barrier(int, int);
void initialize_Heart_barrier(int, int);
void initialize_DevilEye_barrier(int, int);
void initialize_Blanket_barrier(int, int);
void show_logo(int, int);
void manage_rocket();
void manage_invader_rockets();
bool manage_invaders();
void manage_invaderS();
void show_destroyed_animation();
void instructions(int &);
void score_board();
void enter_your_name();
void select_tank();
void adjust_audio(int &);
void choose_color(int &);
void easter_egg();
void save_game();
void load_game();
void update_scoreboard(const string &, int, int);

struct Player{
    string name;
    int score;
    int time;
};

struct block{
    int fg;
    int bg;
};

struct Invader{
    int x;
    int y;
    int kind;
    bool status;
    bool alive;
    int exploded;
};

struct Barrier{
    int x;
    int y;
    int kind;
    int lives;
    int rotation;
};

struct InvaderRocket{
    bool alive;
    int kind;
    int status;
};

struct Backup{
    bool is_there;
    int x;
    int y;
};

struct Button{
    string txt;
    int x;
    int y;
    int len;
    int pos;
    bool is_there;
};

const int SCREEN_WIDTH = 341;
const int SCREEN_HEIGHT = 96;

int faze;
block** screen = new block*[SCREEN_HEIGHT];
block** updated_screen = new block*[SCREEN_HEIGHT];
Invader** invaders = new Invader*[5];
Button* buttons = new Button[10];
InvaderRocket** invader_rockets = new InvaderRocket*[SCREEN_HEIGHT];
Barrier** barriers = new Barrier*[4];
bool live_increased;
Backup backup_tanks[6];
int move_num ;
bool dir;
int count_f;
int t_x, t_y;
int button_f;
int invader_rocket_speed;
int invader_speed;
float invader_rocket_fr;
int rocket_speed;
int rocket_x, rocket_y;
bool rocket_in;
int invaderS_speed;
int invaderS_x, invaderS_y;
bool invaderS_in;
bool invaderS_dir;
int invaderS_fcounter;
string name;
bool game_over;
bool destroyed;
bool game_in_progress;
bool faze_changed;
int score;
int lives;
int play_time = 0;
bool menumusic = true;
bool gamemusic = true;
bool soundeffects = true;
int tank_num = 0;

bool rand_c = false;
bool game_loaded;

int t_c = 10; // Tank color
int rocket_c = 15; // Tank rocket color
int in_a = 15; // Invader 1 color
int in_b = 15; // Invader 2 color
int in_c = 15; // Invader 3 color
int in_s = 4; // Invader S color
int ro_a = 15; // Rocket 1 color
int ro_b = 15; // Rocket 2 color
int in_E = 15;  // Explosion color
int S_E = 15;  // Invader S explosion color
int br_a = 2; // Barrier 1 color
int br_b = 2; // Barrier 2 color
int br_c = 2; // Barrier 3 color
int br_d = 2; // Barrier 4 color
int num_c[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10}; // Color of numbers
int let_c[26] = {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15}; // Color of letters

int main()
{
    set_fontsize(6, 6);
    set_size(SCREEN_WIDTH, SCREEN_HEIGHT);

    create_file();

    mciSendString("open \"menumusic.wav\" type mpegvideo alias menumusic", NULL, 0, NULL);
    mciSendString("open \"gamemusic.wav\" type mpegvideo alias gamemusic", NULL, 0, NULL);
    mciSendString("open \"shoot.wav\" type mpegvideo alias shootmusic", NULL, 0, NULL);
    mciSendString("open \"invaderkilled.wav\" type mpegvideo alias invaderkilledmusic", NULL, 0, NULL);
    mciSendString("open \"explosion.wav\" type mpegvideo alias explosionmusic", NULL, 0, NULL);
    mciSendString("open \"invaderSin.wav\" type mpegvideo alias invaderSinmusic", NULL, 0, NULL);
    mciSendString("open \"button.wav\" type mpegvideo alias buttonmusic", NULL, 0, NULL);
    mciSendString("open \"invaderSdeath.wav\" type mpegvideo alias invaderSdeathmusic", NULL, 0, NULL);

    mciSendString("setaudio menumusic volume to 700", NULL, 0, NULL);
    mciSendString("setaudio gamemusic volume to 800", NULL, 0, NULL);
    mciSendString("setaudio shootmusic volume to 80", NULL, 0, NULL);
    mciSendString("setaudio invaderkilledmusic volume to 80", NULL, 0, NULL);
    mciSendString("setaudio explosionmusic volume to 500", NULL, 0, NULL);
    mciSendString("setaudio invaderSinmusic volume to 80", NULL, 0, NULL);
    mciSendString("setaudio buttonmusic volume to 1000", NULL, 0, NULL);
    mciSendString("setaudio invaderSdeathmusic volume to 1000", NULL, 0, NULL);

    initialize();

    int color_itr = 0;
    int sound_itr = 0;
    int ins_itr = 0;
    time_t start_time, end_time;

    const int fps = 20;
    const chrono::milliseconds frame_duration(1000/fps);

    srand(time(0));

    while(true){
        chrono::steady_clock::time_point frame_start = chrono::steady_clock::now();

        for(int i = 0; i < SCREEN_HEIGHT; i++){
            for(int j = 0; j < SCREEN_WIDTH; j++){
                if(updated_screen[i][j].bg != 0)
                    updated_screen[i][j].bg = 0;
                if(updated_screen[i][j].fg != 0)
                    updated_screen[i][j].fg = 0;
            }
            if(i == t_y+10){
                for(int j = 42; j < 299; j++){
                    updated_screen[i][j].fg = 10;
                }
            }
        }
        if(game_over){
            end_time = clock();
            play_time += (end_time - start_time)/CLOCKS_PER_SEC;
            faze = -1;
            lives = 0;
            live_increased = true;
            faze_changed = true;
            game_over = false;
            for(int i = 0; i < SCREEN_HEIGHT; i++){
                for(int j = 340; j >= 0; j--){
                    if(invader_rockets[i][j].alive){
                        invader_rockets[i][j].alive = false;
                    }
                }
            }
            update_scoreboard(name, score, play_time);
            mciSendString("stop gamemusic", NULL, 0, NULL);
        }
        if(faze_changed && !game_loaded){
            button_initializer();
            invader_initializer();
            barrier_initializer();
            invader_rocket_fr = 600*pow(0.95,faze/2);
            invader_speed = 10-(faze/2);
            move_num = 0;
            count_f = 0;
            button_f = 0;
            if(faze == -20){
                play_time = 0;
                name = "";
            }
            if(faze == -48)
                color_itr = 0;
            if(faze == -47)
                sound_itr = 0;
            if(faze == -18)
                ins_itr = 0;

            invaderS_y = 1;
            invaderS_in = false;
            invaderS_fcounter = 0;

            if(faze < -2){
                for(int i = 0; i < 6; i++){
                    backup_tanks[i].is_there = false;
                    show_destruction(backup_tanks[i].x, backup_tanks[i].y, -1);
                }
                lives = 0;
            }

            if(faze >= 0){
                if(faze == 1 || faze == 0){
                    start_time = clock();
                    score = 0;
                    dir = true;
                    live_increased = true;
                    if(gamemusic)
                        mciSendString("play gamemusic repeat from 0", NULL, 0, NULL);
                }

                invader_rocket_speed = 2 + (faze+2)/4;

                mciSendString("stop menumusic", NULL, 0, NULL);
                game_in_progress = true;
            }
            else{
                if(faze < -2)
                    if(menumusic)
                        mciSendString("play menumusic repeat", NULL, 0, NULL);
                game_in_progress = false;
            }
            faze_changed = false;
        }
        if(game_loaded){
            faze_changed = false;
            game_loaded = false;
            start_time = clock();
        }

        if(invaderS_in && (faze >= 0 || faze == -20)){
            if(soundeffects)
                mciSendString("play invaderSinmusic repeat", NULL, 0, NULL);
        }
        else{
            mciSendString("stop invaderSinmusic", NULL, 0, NULL);
        }

        if(faze == -18)
            instructions(ins_itr);

        if(faze == -19)
            score_board();

        if(faze == -20){
            show_logo(80, 40);
            //print_on_screen("SPACE", 34, 36, 1, 14, 4);
            //print_on_screen("INVADERS",120, 56, 1, 14, 4);
        }

        if(faze == -50)
            enter_your_name();

        if(faze == -49)
            select_tank();

        if(faze == -20){
            if(move_num == 27)
                move_num = 0;
            else if(button_f == 0)
                move_num++;
        }
        else if(faze == -10){
            if(move_num == 54)
                move_num = 0;
            else if(button_f == 0)
                move_num++;
        }
        else if(faze == -1){
            print_on_screen("GAME OVER", 95, 20, 1, 4, 3);
            print_on_screen("YOUR TIME", 136, 32, 1, 8, 1);
            show_score(play_time, 192, 30);
        }
        else if(faze == -2){
            print_on_screen("YOUR GAME IS SAVED", 120, 20, 1, 7, 1);
        }

        // Move buttons
        for(int i = 0; i < 10; i++){
            if(buttons[i].is_there){
                if(buttons[i].pos == 1){
                    if(button_f == 0)
                        buttons[i].x++;
                }
                if(buttons[i].pos == 0){
                    if(button_f == 0)
                        buttons[i].x--;
                }
                buttons[i].len = print_on_screen(buttons[i].txt, buttons[i].x, buttons[i].y, 1, 7, 1);
                if(move_num == 0){
                    if(buttons[i].pos == 1)
                        buttons[i].pos = 0;
                    else if(buttons[i].pos == 0)
                        buttons[i].pos = 1;
                }
            }
        }
        if(button_f < 5)
            button_f++;
        else
            button_f = 0;

        if(faze == -47)
            adjust_audio(sound_itr);

        if(faze == -48){
            choose_color(color_itr);
        }

        if(faze == -99)
            easter_egg();

        if(faze >= -2){
            print_on_screen("SCORE", 2, 26, 1, 15, 1);
            show_score(score, 3, 30);
        }

        // Show backup tanks
        int temp = 0;
        for(int i = 0; i < 6; i++)
            if(backup_tanks[i].is_there){
                temp++;
                place_backuptank(backup_tanks[i].x, backup_tanks[i].y);
            }

        for(int i = 0; i < 6; i++){
            if(temp < lives && !backup_tanks[i].is_there){
                //show_destruction(backup_tanks[i].x, backup_tanks[i].y, tank_num);
                backup_tanks[i].is_there = true;
                temp++;
            }
        }
        if(faze >= 0)
            live_increased = false;


        manage_invader_rockets();

        if(!destroyed)
            place_tank(t_x, t_y);

        // Implant barriers
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 37; j++){
                if(barriers[i][j].lives > 0){
                    place_barrier(barriers[i][j].kind, barriers[i][j].x, barriers[i][j].y, barriers[i][j].lives, barriers[i][j].rotation);
                }
            }
        }

        if(rocket_in)
            manage_rocket();

        manage_invaderS();

        bool wiped_out = manage_invaders();

        if(wiped_out && game_in_progress){
            faze += 2;
            faze_changed = true;
            if(lives < 6)
                lives++;
            live_increased = true;
            dir = true;
        }

        if(count_f < invader_speed)
            count_f++;
        else
            count_f = 0;


        update();

        if(destroyed)
            show_destroyed_animation();

        // InvaderS music
        if(!invaderS_in && (faze == -20 || faze >= 0)){
            if(rand() % 500 == 0){
                if(soundeffects)
                    mciSendString("play invaderSinmusic repeat", NULL, 0, NULL);
                if(rand() % 2 == 0){
                    invaderS_x = -15;
                    invaderS_in = true;
                    invaderS_dir = true;
                }
                else{
                    invaderS_x = SCREEN_WIDTH;
                    invaderS_in = true;
                    invaderS_dir = false;
                }
            }
        }

        // Rockets and invaderS movements
        if(rocket_in){
            rocket_y -= rocket_speed;
        }
        if(invaderS_in){
            if(invaderS_dir)
                invaderS_x += invaderS_speed;
            else
                invaderS_x -= invaderS_speed;
        }
        for(int i = SCREEN_HEIGHT - 1; i >= 0; i--){
            for(int j = SCREEN_WIDTH - 1; j >= 0; j--){
                if(invader_rockets[i][j].alive){
                    invader_rockets[i][j].alive = false;
                    if(i+invader_rocket_speed < SCREEN_HEIGHT){
                        invader_rockets[i+invader_rocket_speed][j].alive = true;
                        invader_rockets[i+invader_rocket_speed][j].kind = invader_rockets[i][j].kind;
                        invader_rockets[i+invader_rocket_speed][j].status = invader_rockets[i][j].status;
                    }
                }
            }
        }


        if(_kbhit()  && (faze > -47 || faze < -60)){
            cin.sync();
            char c = _getch();
            switch(c){
                case 'a':
                case 'A':
                    if(t_x < 20)
                        t_x += 257;
                    t_x -= 7;
                    break;

                case 'd':
                case 'D':
                    if(t_x > 320)
                        t_x -= 257;
                    t_x += 7;
                    break;
                case 's':
                    if(faze < 0)
                        break;
                    if(soundeffects)
                        mciSendString("play invaderSinmusic repeat", NULL, 0, NULL);
                    srand(time(0));
                    if(rand() % 2 == 0){
                        invaderS_x = -15;
                        invaderS_in = true;
                        invaderS_dir = true;
                    }
                    else{
                        invaderS_x = SCREEN_WIDTH;
                        invaderS_in = true;
                        invaderS_dir = false;
                    }
                    break;
                case 'l':
                    if(lives < 6)
                        lives++;
                    live_increased = true;
                    break;

                case 'x':
                case 'X':
                    if(faze < 0)
                        break;
                    end_time = clock();
                    play_time += (end_time - start_time)/CLOCKS_PER_SEC;
                    save_game();
                    for(int i = 0; i < SCREEN_HEIGHT; i++)
                        for(int j = 340; j >= 0; j--)
                            if(invader_rockets[i][j].alive)
                                invader_rockets[i][j].alive = false;
                    faze = -2;
                    mciSendString("stop gamemusic", NULL, 0, NULL);
                    faze_changed = true;
                    break;

                case '\r':
                    if(faze == -19 || faze == -18 || faze == -99){
                        faze = -20;
                        faze_changed = true;
                    }
                    break;
                case ' ':
                    if(!rocket_in){
                        rocket_x = t_x+7;
                        if(rocket_x < 42)
                            rocket_x += 257;
                        if(rocket_x > 298)
                            rocket_x -= 257;
                        rocket_y = t_y-1;
                        rocket_in = true;
                        if(soundeffects)
                            mciSendString("play shootmusic from 0", NULL, 0, NULL);
                }
            }
        }

        chrono::steady_clock::time_point frame_end = chrono::steady_clock::now();
        chrono::milliseconds frame_time = chrono::duration_cast<chrono::milliseconds>(frame_end - frame_start);

        if(frame_time < frame_duration){
            this_thread::sleep_for(frame_duration - frame_time);
        }
        else{
        }
    }
    return 0;
}

void change_faze(string text)
{
    if(text == "NEW GAME"){
        faze = -50;
    }
    else if(text == "LOAD GAME" || text == "CONTINUE"){
        load_game();
        for(int i = 0; i < 10; i++)
            buttons[i].is_there = false;
        game_loaded = true;
        live_increased = true;
        for(int i = 0; i < 6; i++)
            backup_tanks[i].is_there = false;
        if(gamemusic)
            mciSendString("play gamemusic repeat", NULL, 0, NULL);
        mciSendString("stop menumusic", NULL, 0, NULL);
    }
    else if(text == "LEADER BOARD"){
        faze = -19;
    }
    else if(text == "INSTRUCTIONS"){
        faze = -18;
    }
    else if(text == "NORMAL"){
        faze = 0;
        lives = 2;
    }
    else if(text == "CHALLENGING"){
        faze = 1;
        lives = 2;
    }
    else if(text == "MAIN MENU"){
        faze = -20;
        if(menumusic)
            mciSendString("play menumusic repeat from 0", NULL, 0, NULL);
        mciSendString("stop gamemusic", NULL, 0, NULL);
    }
    else if(text == "BACK"){
        faze = -20;
    }
    else if(text == "SETTINGS"){
        faze = -17;
    }
    else if(text == "SELECT TANK"){
        faze = -49;
    }
    else if(text == "CHOOSE COLOR"){
        faze = -48;
    }
    else if(text == "ADJUST AUDIO"){
        faze = -47;
    }
    else if(text == "EXIT"){
        for(int i = 0; i < SCREEN_HEIGHT; i++)
            delete [] screen[i];
        delete [] screen;

        for(int i = 0; i < SCREEN_HEIGHT; i++)
            delete [] updated_screen[i];
        delete [] updated_screen;

        for(int i = 0; i < 5; i++)
            delete [] invaders[i];
        delete [] invaders;

        for(int i = 0; i < SCREEN_HEIGHT; i++)
            delete [] invader_rockets[i];
        delete [] invader_rockets;

        for(int i = 0; i < 4; i++)
            delete [] barriers[i];
        delete [] barriers;

        delete [] buttons;

        mciSendString("close menumusic", NULL, 0, NULL);
        mciSendString("close gamemusic", NULL, 0, NULL);
        mciSendString("close shootmusic", NULL, 0, NULL);
        mciSendString("close invaderkilledmusic", NULL, 0, NULL);
        mciSendString("close explosionmusic", NULL, 0, NULL);
        mciSendString("close invaderSinmusic", NULL, 0, NULL);
        mciSendString("close buttonmusic", NULL, 0, NULL);
        mciSendString("close invaderSdeathmusic", NULL, 0, NULL);

        exit(0);
    }
}

void rand_color()
{
    srand(time(0));
    t_c = (rand() % 15)+1;
    rocket_c = (rand() % 15)+1;
    in_a = (rand() % 15)+1;
    in_b = (rand() % 15)+1;
    in_c = (rand() % 15)+1;
    in_s = (rand() % 15)+1;
    ro_a = (rand() % 15)+1;
    ro_b = (rand() % 15)+1;
    in_E = (rand() % 15)+1;
    S_E = (rand() % 15)+1;
    br_a = (rand() % 15)+1;
    br_b = (rand() % 15)+1;
    br_c = (rand() % 15)+1;
    br_d = (rand() % 15)+1;
    for(int i = 0; i < 10; i++)
        num_c[i] = (rand() % 15)+1;
    for(int i = 0; i < 26; i++)
        let_c[i] = (rand() % 15)+1;
    rand_c = true;
}

void reset_color()
{
    t_c = 10;
    rocket_c = 15;
    in_a = 15;
    in_b = 15;
    in_c = 15;
    in_s = 4;
    ro_a = 15;
    ro_b = 15;
    in_E = 15;
    S_E = 15;
    br_a = 2;
    br_b = 2;
    br_c = 2;
    br_d = 2;
    for(int i = 0; i < 10; i++)
        num_c[i] = 10;
    for(int i = 0; i < 26; i++)
        let_c[i] = 15;
    rand_c = false;
}

void create_file()
{
    fstream data_file;
    data_file.open("ScoreBoard.txt", ios::in);
    if (!data_file){
        data_file.open("ScoreBoard.txt", ios::out);
        data_file << 0 << "\n";
    }
    data_file.close();
}

void show_score(int num, int x, int y)
{
    int digits[6] = {0};
    int counter = 0;
    while(num > 0 && counter < 6){
        digits[counter] = num % 10;
        num /= 10;
        counter++;
    }
    if(counter > 0)
        counter--;
    for(int i = counter; i >= 0; i--){
        place_number(digits[i], (counter - i)*7+x, y);
    }
}

int print_on_screen(string txt, int x, int y, bool c, int color, int fontsize)
{
    int size = txt.length();
    int count = 0;
    for(int i = 0; i < size; i++){
        if(txt[i] == ' '){
            count += 3;
            continue;
        }
        else if(txt[i] == '\n'){
            y += 5;
            count = 0;
            continue;
        }
        place_letter(txt[i] - 'A', x + fontsize*count, y, c, color, fontsize);
        if(txt[i] == 'L'){
            count += 5;
        }
        else if(txt[i] == 'I'){
            count += 4;
        }
        else if(txt[i] == 'M' || txt[i] == 'W'){
            count += 8;
        }
        else{
            count += 6;
        }
    }

    return fontsize*count;
}

void place_number(int digit, int x, int y)
{
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 6; j++){
            if(y+i < 91 && x+j < SCREEN_WIDTH){
                if(Numbers[digit][i][j][0])
                    updated_screen[y+i][x+j].fg = num_c[digit];
                if(Numbers[digit][i][j][1])
                    updated_screen[y+i][x+j].bg = num_c[digit];
            }
        }
    }
}

void place_letter(int let, int x, int y, bool c, int color, int fontsize)
{
    bool h = 1;
    for(int i = 0; i < 6*fontsize; i++){
        if(i % fontsize == 0)
            h = !h;
        for(int j = 0; j < 7*fontsize; j++){
            if(y+i < 93 && x+j < SCREEN_WIDTH){
                if(i%2 == 0){
                    if(Letters[let][i/(2*fontsize)][j/fontsize][h]){
                        if(c)
                            updated_screen[y+(i/2)][x+j].fg = color;
                        else
                            updated_screen[y+(i/2)][x+j].fg = let_c[let];
                    }
                }
                else{
                    if(Letters[let][i/(2*fontsize)][j/fontsize][h]){
                        if(c)
                            updated_screen[y+(i/2)][x+j].bg = color;
                        else
                            updated_screen[y+(i/2)][x+j].bg = let_c[let];
                    }
                }
            }
        }
    }
}

void place_tank(int x, int y)
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 15; j++){
            if(y+i < SCREEN_HEIGHT && x+j < SCREEN_WIDTH){
                if(x+j < 42){
                    if(tank[tank_num][i][j][0])
                        updated_screen[y+i][x+j+257].fg = t_c;
                    if(tank[tank_num][i][j][1])
                        updated_screen[y+i][x+j+257].bg = t_c;
                }
                else if(x+j > 298){
                    if(tank[tank_num][i][j][0])
                        updated_screen[y+i][x+j-257].fg = t_c;
                    if(tank[tank_num][i][j][1])
                        updated_screen[y+i][x+j-257].bg = t_c;
                }
                else{
                    if(tank[tank_num][i][j][0])
                        updated_screen[y+i][x+j].fg = t_c;
                    if(tank[tank_num][i][j][1])
                        updated_screen[y+i][x+j].bg = t_c;
                }
            }
        }
    }
}

void place_backuptank(int x, int y)
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 15; j++){
            if(y+i < SCREEN_HEIGHT && x+j < SCREEN_WIDTH){
                if(tank[tank_num][i][j][0])
                    updated_screen[y+i][x+j].fg = t_c;
                if(tank[tank_num][i][j][1])
                    updated_screen[y+i][x+j].bg = t_c;
            }
        }
    }
}

void place_invader(int kind, int x, int y, bool status)
{
    if(kind == 0){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 16; j++){
                if(y+i < SCREEN_HEIGHT && y+i >= 0 && x+j < SCREEN_WIDTH && x+j >= 0){
                    if(invader_S[i][j][0])
                        updated_screen[y+i][x+j].fg = in_s;
                    if(invader_S[i][j][1])
                        updated_screen[y+i][x+j].bg = in_s;
                }
            }
        }
        return;
    }
    if(kind == -1){
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 12; j++){
                if(y+i < SCREEN_HEIGHT && x+j < SCREEN_WIDTH){
                    if(invader_explosion[i][j][0])
                        updated_screen[y+i][x+j].fg = in_E;
                    if(invader_explosion[i][j][1])
                        updated_screen[y+i][x+j].bg = in_E;
                }
            }
        }
        return;
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 12; j++){
            if(y+i < SCREEN_HEIGHT && x+j < SCREEN_WIDTH){
                if(kind == 1){
                    if(invader1[status][i][j][0])
                        updated_screen[y+i][x+j].fg = in_a;
                    if(invader1[status][i][j][1])
                        updated_screen[y+i][x+j].bg = in_a;
                }
                else if(kind == 2){
                    if(invader2[status][i][j][0])
                        updated_screen[y+i][x+j].fg = in_b;
                    if(invader2[status][i][j][1])
                        updated_screen[y+i][x+j].bg = in_b;
                }
                else if(kind == 3){
                    if(invader3[status][i][j][0])
                        updated_screen[y+i][x+j].fg = in_c;
                    if(invader3[status][i][j][1])
                        updated_screen[y+i][x+j].bg = in_c;
                }
            }
        }
    }
}

void place_invader_rocket(int kind, int x, int y, int status)
{
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(y+i < 91 && x+j < SCREEN_WIDTH){
                if(kind == 1){
                    if(Rocket1[status][i][j][0])
                        updated_screen[y+i][x+j].fg = ro_a;
                    if(Rocket1[status][i][j][1])
                        updated_screen[y+i][x+j].bg = ro_a;
                }
                else if(kind == 2){
                    if(Rocket2[status][i][j][0])
                        updated_screen[y+i][x+j].fg = ro_b;
                    if(Rocket2[status][i][j][1])
                        updated_screen[y+i][x+j].bg = ro_b;
                }
            }
        }
    }
}

void place_barrier(int kind, int x, int y, int lives, int rotation)
{
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 6; j++){
            if(y+i < 85 && x+j < SCREEN_WIDTH){
                if(kind == 1){
                    switch(rotation){
                        case 0:
                            if(Barrier1[4-lives][i][j][0])
                                updated_screen[y+i][x+j].fg = br_a;
                            if(Barrier1[4-lives][i][j][1])
                                updated_screen[y+i][x+j].bg = br_a;
                            break;
                        case 1:
                            if(Barrier1[4-lives][i][j][0])
                                updated_screen[y+i][x+5-j].fg = br_a;
                            if(Barrier1[4-lives][i][j][1])
                                updated_screen[y+i][x+5-j].bg = br_a;
                            break;
                        case 2:
                            if(Barrier1[4-lives][i][j][0])
                                updated_screen[y+2-i][x+5-j].bg = br_a;
                            if(Barrier1[4-lives][i][j][1])
                                updated_screen[y+2-i][x+5-j].fg = br_a;
                            break;
                        default:
                            if(Barrier1[4-lives][i][j][0])
                                updated_screen[y+2-i][x+j].bg = br_a;
                            if(Barrier1[4-lives][i][j][1])
                                updated_screen[y+2-i][x+j].fg = br_a;
                    }
                }
                else if(kind == 2){
                    switch(rotation){
                        case 0:
                            if(Barrier2[3-lives][i][j][0])
                                updated_screen[y+i][x+j].fg = br_b;
                            if(Barrier2[3-lives][i][j][1])
                                updated_screen[y+i][x+j].bg = br_b;
                            break;
                        case 1:
                            if(Barrier2[3-lives][i][j][0])
                                updated_screen[y+i][x+5-j].fg = br_b;
                            if(Barrier2[3-lives][i][j][1])
                                updated_screen[y+i][x+5-j].bg = br_b;
                            break;
                        case 2:
                            if(Barrier2[3-lives][i][j][0])
                                updated_screen[y+2-i][x+5-j].bg = br_b;
                            if(Barrier2[3-lives][i][j][1])
                                updated_screen[y+2-i][x+5-j].fg = br_b;
                            break;
                        default:
                            if(Barrier2[3-lives][i][j][0])
                                updated_screen[y+2-i][x+j].bg = br_b;
                            if(Barrier2[3-lives][i][j][1])
                                updated_screen[y+2-i][x+j].fg = br_b;
                    }
                }
                else if(kind == 3){
                    switch(rotation){
                        case 0:
                            if(Barrier3[4-lives][i][j][0])
                                updated_screen[y+i][x+j].fg = br_c;
                            if(Barrier3[4-lives][i][j][1])
                                updated_screen[y+i][x+j].bg = br_c;
                            break;
                        case 1:
                            if(Barrier3[4-lives][i][j][0])
                                updated_screen[y+i][x+5-j].fg = br_c;
                            if(Barrier3[4-lives][i][j][1])
                                updated_screen[y+i][x+5-j].bg = br_c;
                            break;
                        case 2:
                            if(Barrier3[4-lives][i][j][0])
                                updated_screen[y+2-i][x+5-j].bg = br_c;
                            if(Barrier3[4-lives][i][j][1])
                                updated_screen[y+2-i][x+5-j].fg = br_c;
                            break;
                        default:
                            if(Barrier3[4-lives][i][j][0])
                                updated_screen[y+2-i][x+j].bg = br_c;
                            if(Barrier3[4-lives][i][j][1])
                                updated_screen[y+2-i][x+j].fg = br_c;
                    }
                }
                else if(kind == 4){
                    switch(rotation){
                        case 0:
                            if(Barrier4[2-lives][i][j][0])
                                updated_screen[y+i][x+j].fg = br_d;
                            if(Barrier4[2-lives][i][j][1])
                                updated_screen[y+i][x+j].bg = br_d;
                            break;
                        case 1:
                            if(Barrier4[2-lives][i][j][0])
                                updated_screen[y+i][x+5-j].fg = br_d;
                            if(Barrier4[2-lives][i][j][1])
                                updated_screen[y+i][x+5-j].bg = br_d;
                            break;
                        case 2:
                            if(Barrier4[2-lives][i][j][0])
                                updated_screen[y+2-i][x+5-j].bg = br_d;
                            if(Barrier4[2-lives][i][j][1])
                                updated_screen[y+2-i][x+5-j].fg = br_d;
                            break;
                        default:
                            if(Barrier4[2-lives][i][j][0])
                                updated_screen[y+2-i][x+j].bg = br_d;
                            if(Barrier4[2-lives][i][j][1])
                                updated_screen[y+2-i][x+j].fg = br_d;
                    }
                }
            }
        }
    }
}

void place_hundred(int x, int y)
{
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 14; j++){
            if(y+i < SCREEN_HEIGHT && y+i >= 0 && x+j < SCREEN_WIDTH && x+j >= 0){
                if(Hundred[i][j][0])
                    updated_screen[y+i][x+j].fg = S_E;
                if(Hundred[i][j][1])
                    updated_screen[y+i][x+j].bg = S_E;
            }
        }
    }
}

void place_object(int x, int y, int hei, int wid, int* arr)
{
    for(int i = 0; i < hei; i++){
        for(int j = 0; j < wid; j++){
            if(y+i < SCREEN_HEIGHT && x+j < SCREEN_WIDTH){
                if(arr[i*wid*2 + j*2] != updated_screen[y+i][x+j].fg)
                    updated_screen[y+i][x+j].fg = arr[i*wid*2 + j*2];
                if(arr[i*wid*2 + j*2 + 1] != updated_screen[y+i][x+j].bg)
                    updated_screen[y+i][x+j].bg = arr[i*wid*2 + j*2 + 1];
            }
        }
    }
}

void update()
{
    set_color(15, 0);
    int last_fg = 15, last_bg = 0;
    for(int i = 0; i < SCREEN_HEIGHT; i++){
        for(int j = 0; j < SCREEN_WIDTH; j++){
            if(screen[i][j].bg != updated_screen[i][j].bg || screen[i][j].fg != updated_screen[i][j].fg){
                gotoxy(j, i);

                while(j < SCREEN_WIDTH && (screen[i][j].bg != updated_screen[i][j].bg || screen[i][j].fg != updated_screen[i][j].fg)){
                    if(updated_screen[i][j].fg == 0){
                        if(last_fg != updated_screen[i][j].bg || last_bg != updated_screen[i][j].fg){
                            last_fg = updated_screen[i][j].bg;
                            last_bg = updated_screen[i][j].fg;
                            set_color(last_fg, last_bg);
                        }
                        cout << "\u2584";
                    }
                    else{
                        if(last_fg != updated_screen[i][j].fg || last_bg != updated_screen[i][j].bg){
                            last_fg = updated_screen[i][j].fg;
                            last_bg = updated_screen[i][j].bg;
                            set_color(last_fg, last_bg);
                        }
                        cout << "\u2580";
                    }
                    screen[i][j].bg = updated_screen[i][j].bg;
                    screen[i][j].fg = updated_screen[i][j].fg;

                    j++;
                }
            }
        }
    }

    set_color(0, 0);
}

void show_destruction(int x, int y, int status)
{
    set_color(0, 0);
    int last_fg = 0, last_bg = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 15; j++){
            if(y+i < SCREEN_HEIGHT && x+j < SCREEN_WIDTH){
                if(x+j < 42 && status > 9){
                    gotoxy(x+j+257, y+i);
                    if(status == 12){
                        cout << " ";
                        continue;
                    }

                }
                else if(x+j > 298 && status > 9){
                    gotoxy(x+j-257, y+i);
                    if(status == 12){
                        cout << " ";
                        continue;
                    }
                }
                else{
                    gotoxy(x+j, y+i);
                    if(status == 12 || status == -1){
                        cout << " ";
                        continue;
                    }

                }
                if(tank[status][i][j][0] == 0){
                    if(tank[status][i][j][1]){
                        if(last_fg != t_c || last_bg != 0){
                            last_fg = t_c;
                            last_bg = 0;
                            set_color(last_fg, last_bg);
                        }
                        cout << "\u2584";
                    }
                    else{
                        if(last_fg != 0 || last_bg != 0){
                            last_fg = 0;
                            last_bg = 0;
                            set_color(last_fg, last_bg);
                        }
                        cout << "\u2584";
                    }
                }
                else{
                    if(tank[status][i][j][1]){
                        if(last_fg != t_c || last_bg != t_c){
                            last_fg = t_c;
                            last_bg = t_c;
                            set_color(last_fg, last_bg);
                        }
                        cout << "\u2584";
                    }
                    else{
                        if(last_fg != 0 || last_bg != t_c){
                            last_fg = 0;
                            last_bg = t_c;
                            set_color(last_fg, last_bg);
                        }
                        cout << "\u2584";
                    }
                }
            }
        }
    }
    set_color(0, 0);
    if(status < 10){
        for(int i = 0; i < 4; i++){
            gotoxy(x-1, y+i);
            cout << " ";
            gotoxy(x+15, y+i);
            cout << " ";
        }
        for(int j = 0; j < 15; j++){
            gotoxy(x+j, y-1);
            cout << " ";
            gotoxy(x+j, y+4);
            cout << " ";
        }
    }
}

void initialize()
{
    faze = -20;

    for(int i = 0; i < SCREEN_HEIGHT; i++){
        screen[i] = new block[SCREEN_WIDTH];
        for(int j = 0; j < SCREEN_WIDTH; j++){
            screen[i][j].bg = 0;
            screen[i][j].fg = 0;
        }
    }
    for(int i = 0; i < SCREEN_HEIGHT; i++){
        updated_screen[i] = new block[SCREEN_WIDTH];
        for(int j = 0; j < SCREEN_WIDTH; j++){
            updated_screen[i][j].bg = 0;
            updated_screen[i][j].fg = 0;
        }
    }

    for(int i = 0; i < 5; i++){
        invaders[i] = new Invader[11];
    }
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 11; j++){
            invaders[i][j].alive = 0;
            invaders[i][j].exploded = 0;
            invaders[i][j].kind = 0;
            invaders[i][j].status = 0;
            invaders[i][j].x = 0;
            invaders[i][j].y = 0;
        }
    }


    for(int i = 0; i < SCREEN_HEIGHT; i++){
        invader_rockets[i] = new InvaderRocket[SCREEN_WIDTH];
    }

    for(int i = 0; i < SCREEN_HEIGHT; i++){
        for(int j = 0; j < SCREEN_WIDTH; j++){
            invader_rockets[i][j].alive = 0;
            invader_rockets[i][j].kind = 0;
            invader_rockets[i][j].status = 0;
        }
    }

    for(int i = 0; i < 4; i++){
        barriers[i] = new Barrier[37];
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 37; j++){
            barriers[i][j].lives = 0;
            barriers[i][j].kind = 0;
            barriers[i][j].rotation = 0;
            barriers[i][j].x = 0;
            barriers[i][j].y = 0;
        }
    }


    lives = 0;
    live_increased = true;
    for(int i = 0; i < 6; i++){
        backup_tanks[i].is_there = false;

        if(i == 2 || i == 5)
            backup_tanks[i].y = 75;
        else
            backup_tanks[i].y = 81;

        if(i == 2 || i == 1)
            backup_tanks[i].x = 3;
        else if(i == 0)
            backup_tanks[i].x = 24;
        else if(i == 3)
            backup_tanks[i].x = 302;
        else if(i == 4 || i == 5)
            backup_tanks[i].x = 323;
    }

    button_initializer();
    move_num = 0;

    dir = true;
    count_f = 0;

    t_x = 150;
    t_y = 81;

    hide_cursor();

    set_color(0, 0);

    button_f = 0;

    invader_rocket_speed = 1;
    invader_speed = 10;
    invader_rocket_fr = 100;

    rocket_speed = 5;
    rocket_in = false;

    invaderS_speed = 3;
    invaderS_y = 1;
    invaderS_in = false;
    invaderS_dir = true;
    invaderS_fcounter = 0;

    name = "";

    game_over = false;

    destroyed = false;

    game_in_progress = false;

    faze_changed = true;

    score = 0;

    game_loaded = false;
}

void button_initializer()
{
    for(int i = 0; i < 10; i++)
        buttons[i].is_there = false;

    if(faze == -20){
        for(int i = 0; i < 6; i++)
            buttons[i].is_there = true;
        buttons[0].txt = "NEW GAME";
        buttons[0].pos = true;
        buttons[0].x = 45;
        buttons[0].y = 20;

        buttons[1].txt = "LOAD GAME";
        buttons[1].pos = false;
        buttons[1].x = 72;
        buttons[1].y = 12;

        buttons[2].txt = "LEADER BOARD";
        buttons[2].pos = true;
        buttons[2].x = 120;
        buttons[2].y = 20;

        buttons[3].txt = "INSTRUCTIONS";
        buttons[3].pos = false;
        buttons[3].x = 165;
        buttons[3].y = 12;

        buttons[4].txt = "SETTINGS";
        buttons[4].pos = true;
        buttons[4].x = 215;
        buttons[4].y = 20;

        buttons[5].txt = "EXIT";
        buttons[5].pos = false;
        buttons[5].x = 267;
        buttons[5].y = 12;
    }
    else if(faze == -10){
        buttons[0].txt = "NORMAL";
        buttons[0].pos = true;
        buttons[0].x = 120;
        buttons[0].y = 20;
        buttons[0].is_there = true;

        buttons[1].txt = "CHALLENGING";
        buttons[1].pos = false;
        buttons[1].x = 162;
        buttons[1].y = 12;
        buttons[1].is_there = true;

        buttons[2].txt = "BACK";
        buttons[2].pos = -1;
        buttons[2].x = 270;
        buttons[2].y = 20;
        buttons[2].is_there = true;
    }
    else if(faze == -1){
        buttons[0].txt = "MAIN MENU";
        buttons[0].pos = -1;
        buttons[0].x = 45;
        buttons[0].y = 40;
        buttons[0].is_there = true;

        buttons[1].txt = "EXIT";
        buttons[1].pos = -1;
        buttons[1].x = 270;
        buttons[1].y = 40;
        buttons[1].is_there = true;
    }
    else if(faze == -2){
        buttons[0].txt = "CONTINUE";
        buttons[0].pos = -1;
        buttons[0].x = 45;
        buttons[0].y = 35;
        buttons[0].is_there = true;

        buttons[1].txt = "MAIN MENU";
        buttons[1].pos = -1;
        buttons[1].x = 145;
        buttons[1].y = 35;
        buttons[1].is_there = true;

        buttons[2].txt = "EXIT";
        buttons[2].pos = -1;
        buttons[2].x = 270;
        buttons[2].y = 35;
        buttons[2].is_there = true;
    }
    else if(faze == -17){
        buttons[0].txt = "SELECT TANK";
        buttons[0].pos = -1;
        buttons[0].x = 45;
        buttons[0].y = 20;
        buttons[0].is_there = true;

        buttons[1].txt = "CHOOSE COLOR";
        buttons[1].pos = -1;
        buttons[1].x = 85;
        buttons[1].y = 30;
        buttons[1].is_there = true;

        buttons[2].txt = "ADJUST AUDIO";
        buttons[2].pos = -1;
        buttons[2].x = 187;
        buttons[2].y = 30;
        buttons[2].is_there = true;

        buttons[3].txt = "BACK";
        buttons[3].pos = -1;
        buttons[3].x = 270;
        buttons[3].y = 20;
        buttons[3].is_there = true;
    }
    else if(faze == -48){
        buttons[0].txt = "TANK";
        buttons[0].pos = -1;
        buttons[0].x = 45;
        buttons[0].y = 10;
        buttons[0].is_there = true;

        buttons[1].txt = "TANK ROCKET";
        buttons[1].pos = -1;
        buttons[1].x = 100;
        buttons[1].y = 10;
        buttons[1].is_there = true;

        buttons[2].txt = "INVADER ROCKET";
        buttons[2].pos = -1;
        buttons[2].x = 195;
        buttons[2].y = 10;
        buttons[2].is_there = true;

        buttons[3].txt = "INVADER A";
        buttons[3].pos = -1;
        buttons[3].x = 45;
        buttons[3].y = 20;
        buttons[3].is_there = true;

        buttons[4].txt = "INVADER B";
        buttons[4].pos = -1;
        buttons[4].x = 130;
        buttons[4].y = 20;
        buttons[4].is_there = true;

        buttons[5].txt = "INVADER C";
        buttons[5].pos = -1;
        buttons[5].x = 215;
        buttons[5].y = 20;
        buttons[5].is_there = true;

        buttons[6].txt = "BARRIER";
        buttons[6].pos = -1;
        buttons[6].x = 45;
        buttons[6].y = 30;
        buttons[6].is_there = true;

        buttons[7].txt = "RANDOMIZE";
        buttons[7].pos = -1;
        buttons[7].x = 130;
        buttons[7].y = 30;
        buttons[7].is_there = true;

        buttons[8].txt = "INVADER S";
        buttons[8].pos = -1;
        buttons[8].x = 215;
        buttons[8].y = 30;
        buttons[8].is_there = true;

        buttons[9].txt = "RESET";
        buttons[9].pos = -1;
        buttons[9].x = 150;
        buttons[9].y = 40;
        buttons[9].is_there = true;
    }
    else if(faze == -47){
        buttons[0].txt = "MENU MUSIC";
        buttons[0].pos = -1;
        buttons[0].x = 45;
        buttons[0].y = 25;
        buttons[0].is_there = true;

        buttons[1].txt = "GAME MUSIC";
        buttons[1].pos = -1;
        buttons[1].x = 133;
        buttons[1].y = 25;
        buttons[1].is_there = true;

        buttons[2].txt = "SOUND EFFECTS";
        buttons[2].pos = -1;
        buttons[2].x = 221;
        buttons[2].y = 25;
        buttons[2].is_there = true;
    }
}

void invader_initializer()
{
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 11; j++)
            invaders[i][j].alive = 0;
    if(faze < 0)
        return;
    for(int i = 0; i < 11; i++){
        if(faze % 2 == 0 && i % 2 == 1)
            continue;
        invaders[0][i].alive = 1;
        invaders[0][i].status = 0;
        invaders[0][i].x = 15*(i+3);
        invaders[0][i].y = 7;
        invaders[0][i].kind = 3;
        invaders[0][i].exploded = 0;
    }
    for(int i = 0; i < 11; i++){
        if(faze % 2 == 0 && i % 2 == 0)
            continue;
        invaders[1][i].alive = 1;
        invaders[1][i].status = 0;
        invaders[1][i].x = 15*(i+3);
        invaders[1][i].y = 15;
        invaders[1][i].kind = 2;
        invaders[1][i].exploded = 0;
    }
    for(int i = 0; i < 11; i++){
        if(faze % 2 == 0 && i % 2 == 1)
            continue;
        invaders[2][i].alive = 1;
        invaders[2][i].status = 0;
        invaders[2][i].x = 15*(i+3);
        invaders[2][i].y = 23;
        invaders[2][i].kind = 2;
        invaders[2][i].exploded = 0;
    }
    for(int i = 0; i < 11; i++){
        if(faze % 2 == 0 && i % 2 == 0)
            continue;
        invaders[3][i].alive = 1;
        invaders[3][i].status = 0;
        invaders[3][i].x = 15*(i+3);
        invaders[3][i].y = 31;
        invaders[3][i].kind = 1;
        invaders[3][i].exploded = 0;
    }
    for(int i = 0; i < 11; i++){
        if(faze % 2 == 0 && i % 2 == 1)
            continue;
        invaders[4][i].alive = 1;
        invaders[4][i].status = 0;
        invaders[4][i].x = 15*(i+3);
        invaders[4][i].y = 39;
        invaders[4][i].kind = 1;
        invaders[4][i].exploded = 0;
    }
}

void barrier_initializer()
{
    if(faze < -1){
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 37; j++)
                barriers[i][j].lives = 0;
    }


    if(faze != 0 && faze != 1)
        return;
    srand(time(0));
    int a = rand() % 4;
    if(a == 0){
        initialize_Classic_barrier(1, 3);
    }
    else if(a == 1){
        initialize_Heart_barrier(0, 3);
    }
    else if(a == 2){
        initialize_DevilEye_barrier(0, 3);
    }
    else if(a == 3){
        initialize_Blanket_barrier(0, 3);
    }

    int b = rand() % 4;
    if(b == 0){
        initialize_Classic_barrier(1, 13);
    }
    else if(b == 1){
        initialize_Heart_barrier(0, 13);
    }
    else if(b == 2){
        initialize_DevilEye_barrier(0, 13);
    }
    else if(b == 3){
        initialize_Blanket_barrier(0, 13);
    }


    int c = rand() % 4;
    if(c == 0){
        initialize_Classic_barrier(1, 23);
    }
    else if(c == 1){
        initialize_Heart_barrier(0, 23);
    }
    else if(c == 2){
        initialize_DevilEye_barrier(0, 23);
    }
    else if(c == 3){
        initialize_Blanket_barrier(0, 23);
    }


    int d = rand() % 4;
    if(d == 0){
        initialize_Classic_barrier(1, 33);
    }
    else if(d == 1){
        initialize_Heart_barrier(0, 33);
    }
    else if(d == 2){
        initialize_DevilEye_barrier(0, 33);
    }
    else if(d == 3){
        initialize_Blanket_barrier(0, 33);
    }
}

void initialize_Classic_barrier(int i, int j)
{
    barriers[i][j].kind = 3;
    barriers[i][j].lives = 4;
    barriers[i][j].x = 51 + (j*6);
    barriers[i][j].y = 62 + (i*3);
    barriers[i][j].rotation = 0;

    barriers[i+1][j].kind = 1;
    barriers[i+1][j].lives = 4;
    barriers[i+1][j].x = 51 + (j*6);
    barriers[i+1][j].y = 65 + (i*3);
    barriers[i+1][j].rotation = 1;

    barriers[i+2][j].kind = 1;
    barriers[i+2][j].lives = 4;
    barriers[i+2][j].x = 51 + (j*6);
    barriers[i+2][j].y = 68 + (i*3);
    barriers[i+2][j].rotation = 0;


    barriers[i][j+1].kind = 1;
    barriers[i][j+1].lives = 4;
    barriers[i][j+1].x = 57 + (j*6);
    barriers[i][j+1].y = 62 + (i*3);
    barriers[i][j+1].rotation = 2;

    barriers[i+1][j+1].kind = 2;
    barriers[i+1][j+1].lives = 3;
    barriers[i+1][j+1].x = 57 + (j*6);
    barriers[i+1][j+1].y = 65 + (i*3);
    barriers[i+1][j+1].rotation = 0;


    barriers[i][j+2].kind = 1;
    barriers[i][j+2].lives = 4;
    barriers[i][j+2].x = 63 + (j*6);
    barriers[i][j+2].y = 62 + (i*3);
    barriers[i][j+2].rotation = 3;

    barriers[i+1][j+2].kind = 2;
    barriers[i+1][j+2].lives = 3;
    barriers[i+1][j+2].x = 63 + (j*6);
    barriers[i+1][j+2].y = 65 + (i*3);
    barriers[i+1][j+2].rotation = 1;


    barriers[i][j+3].kind = 3;
    barriers[i][j+3].lives = 4;
    barriers[i][j+3].x = 69 + (j*6);
    barriers[i][j+3].y = 62 + (i*3);
    barriers[i][j+3].rotation = 1;

    barriers[i+1][j+3].kind = 1;
    barriers[i+1][j+3].lives = 4;
    barriers[i+1][j+3].x = 69 + (j*6);
    barriers[i+1][j+3].y = 65 + (i*3);
    barriers[i+1][j+3].rotation = 0;

    barriers[i+2][j+3].kind = 1;
    barriers[i+2][j+3].lives = 4;
    barriers[i+2][j+3].x = 69 + (j*6);
    barriers[i+2][j+3].y = 68 + (i*3);
    barriers[i+2][j+3].rotation = 1;
}

void initialize_Heart_barrier(int i, int j)
{
    barriers[i][j].kind = 2;
    barriers[i][j].lives = 3;
    barriers[i][j].x = 51 + (j*6);
    barriers[i][j].y = 62 + (i*3);
    barriers[i][j].rotation = 2;

    barriers[i+1][j].kind = 1;
    barriers[i+1][j].lives = 4;
    barriers[i+1][j].x = 51 + (j*6);
    barriers[i+1][j].y = 65 + (i*3);
    barriers[i+1][j].rotation = 0;

    barriers[i+2][j].kind = 2;
    barriers[i+2][j].lives = 3;
    barriers[i+2][j].x = 51 + (j*6);
    barriers[i+2][j].y = 68 + (i*3);
    barriers[i+2][j].rotation = 1;


    barriers[i][j+1].kind = 2;
    barriers[i][j+1].lives = 3;
    barriers[i][j+1].x = 57 + (j*6);
    barriers[i][j+1].y = 62 + (i*3);
    barriers[i][j+1].rotation = 3;

    barriers[i+1][j+1].kind = 1;
    barriers[i+1][j+1].lives = 4;
    barriers[i+1][j+1].x = 57 + (j*6);
    barriers[i+1][j+1].y = 65 + (i*3);
    barriers[i+1][j+1].rotation = 3;

    barriers[i+2][j+1].kind = 1;
    barriers[i+2][j+1].lives = 4;
    barriers[i+2][j+1].x = 57 + (j*6);
    barriers[i+2][j+1].y = 68 + (i*3);
    barriers[i+2][j+1].rotation = 0;

    barriers[i+3][j+1].kind = 2;
    barriers[i+3][j+1].lives = 3;
    barriers[i+3][j+1].x = 57 + (j*6);
    barriers[i+3][j+1].y = 71 + (i*3);
    barriers[i+3][j+1].rotation = 1;


    barriers[i][j+2].kind = 2;
    barriers[i][j+2].lives = 3;
    barriers[i][j+2].x = 63 + (j*6);
    barriers[i][j+2].y = 62 + (i*3);
    barriers[i][j+2].rotation = 2;

    barriers[i+1][j+2].kind = 1;
    barriers[i+1][j+2].lives = 4;
    barriers[i+1][j+2].x = 63 + (j*6);
    barriers[i+1][j+2].y = 65 + (i*3);
    barriers[i+1][j+2].rotation = 2;

    barriers[i+2][j+2].kind = 1;
    barriers[i+2][j+2].lives = 4;
    barriers[i+2][j+2].x = 63 + (j*6);
    barriers[i+2][j+2].y = 68 + (i*3);
    barriers[i+2][j+2].rotation = 1;

    barriers[i+3][j+2].kind = 2;
    barriers[i+3][j+2].lives = 3;
    barriers[i+3][j+2].x = 63 + (j*6);
    barriers[i+3][j+2].y = 71 + (i*3);
    barriers[i+3][j+2].rotation = 0;


    barriers[i][j+3].kind = 2;
    barriers[i][j+3].lives = 3;
    barriers[i][j+3].x = 69 + (j*6);
    barriers[i][j+3].y = 62 + (i*3);
    barriers[i][j+3].rotation = 3;

    barriers[i+1][j+3].kind = 1;
    barriers[i+1][j+3].lives = 4;
    barriers[i+1][j+3].x = 69 + (j*6);
    barriers[i+1][j+3].y = 65 + (i*3);
    barriers[i+1][j+3].rotation = 1;

    barriers[i+2][j+3].kind = 2;
    barriers[i+2][j+3].lives = 3;
    barriers[i+2][j+3].x = 69 + (j*6);
    barriers[i+2][j+3].y = 68 + (i*3);
    barriers[i+2][j+3].rotation = 0;
}

void initialize_DevilEye_barrier(int i, int j)
{
    barriers[i][j].kind = 2;
    barriers[i][j].lives = 3;
    barriers[i][j].x = 51 + (j*6);
    barriers[i][j].y = 62 + (i*3);
    barriers[i][j].rotation = 3;

    barriers[i+1][j].kind = 2;
    barriers[i+1][j].lives = 3;
    barriers[i+1][j].x = 51 + (j*6);
    barriers[i+1][j].y = 65 + (i*3);
    barriers[i+1][j].rotation = 1;

    barriers[i+2][j].kind = 2;
    barriers[i+2][j].lives = 3;
    barriers[i+2][j].x = 51 + (j*6);
    barriers[i+2][j].y = 68 + (i*3);
    barriers[i+2][j].rotation = 2;

    barriers[i+3][j].kind = 2;
    barriers[i+3][j].lives = 3;
    barriers[i+3][j].x = 51 + (j*6);
    barriers[i+3][j].y = 71 + (i*3);
    barriers[i+3][j].rotation = 0;


    barriers[i][j+1].kind = 2;
    barriers[i][j+1].lives = 3;
    barriers[i][j+1].x = 57 + (j*6);
    barriers[i][j+1].y = 62 + (i*3);
    barriers[i][j+1].rotation = 2;

    barriers[i+1][j+1].kind = 2;
    barriers[i+1][j+1].lives = 3;
    barriers[i+1][j+1].x = 57 + (j*6);
    barriers[i+1][j+1].y = 65 + (i*3);
    barriers[i+1][j+1].rotation = 0;

    barriers[i+2][j+1].kind = 2;
    barriers[i+2][j+1].lives = 3;
    barriers[i+2][j+1].x = 57 + (j*6);
    barriers[i+2][j+1].y = 68 + (i*3);
    barriers[i+2][j+1].rotation = 3;

    barriers[i+3][j+1].kind = 2;
    barriers[i+3][j+1].lives = 3;
    barriers[i+3][j+1].x = 57 + (j*6);
    barriers[i+3][j+1].y = 71 + (i*3);
    barriers[i+3][j+1].rotation = 1;


    barriers[i][j+2].kind = 2;
    barriers[i][j+2].lives = 3;
    barriers[i][j+2].x = 63 + (j*6);
    barriers[i][j+2].y = 62 + (i*3);
    barriers[i][j+2].rotation = 3;

    barriers[i+1][j+2].kind = 2;
    barriers[i+1][j+2].lives = 3;
    barriers[i+1][j+2].x = 63 + (j*6);
    barriers[i+1][j+2].y = 65 + (i*3);
    barriers[i+1][j+2].rotation = 1;

    barriers[i+2][j+2].kind = 2;
    barriers[i+2][j+2].lives = 3;
    barriers[i+2][j+2].x = 63 + (j*6);
    barriers[i+2][j+2].y = 68 + (i*3);
    barriers[i+2][j+2].rotation = 2;

    barriers[i+3][j+2].kind = 2;
    barriers[i+3][j+2].lives = 3;
    barriers[i+3][j+2].x = 63 + (j*6);
    barriers[i+3][j+2].y = 71 + (i*3);
    barriers[i+3][j+2].rotation = 0;


    barriers[i][j+3].kind = 2;
    barriers[i][j+3].lives = 3;
    barriers[i][j+3].x = 69 + (j*6);
    barriers[i][j+3].y = 62 + (i*3);
    barriers[i][j+3].rotation = 2;

    barriers[i+1][j+3].kind = 2;
    barriers[i+1][j+3].lives = 3;
    barriers[i+1][j+3].x = 69 + (j*6);
    barriers[i+1][j+3].y = 65 + (i*3);
    barriers[i+1][j+3].rotation = 0;

    barriers[i+2][j+3].kind = 2;
    barriers[i+2][j+3].lives = 3;
    barriers[i+2][j+3].x = 69 + (j*6);
    barriers[i+2][j+3].y = 68 + (i*3);
    barriers[i+2][j+3].rotation = 3;

    barriers[i+3][j+3].kind = 2;
    barriers[i+3][j+3].lives = 3;
    barriers[i+3][j+3].x = 69 + (j*6);
    barriers[i+3][j+3].y = 71 + (i*3);
    barriers[i+3][j+3].rotation = 1;
}

void initialize_Blanket_barrier(int i, int j)
{
    barriers[i][j].kind = 4;
    barriers[i][j].lives = 2;
    barriers[i][j].x = 51 + (j*6);
    barriers[i][j].y = 62 + (i*3);
    barriers[i][j].rotation = 0;

    barriers[i+1][j].kind = 4;
    barriers[i+1][j].lives = 2;
    barriers[i+1][j].x = 51 + (j*6);
    barriers[i+1][j].y = 65 + (i*3);
    barriers[i+1][j].rotation = 0;

    barriers[i+2][j].kind = 4;
    barriers[i+2][j].lives = 2;
    barriers[i+2][j].x = 51 + (j*6);
    barriers[i+2][j].y = 68 + (i*3);
    barriers[i+2][j].rotation = 0;

    barriers[i+3][j].kind = 4;
    barriers[i+3][j].lives = 2;
    barriers[i+3][j].x = 51 + (j*6);
    barriers[i+3][j].y = 71 + (i*3);
    barriers[i+3][j].rotation = 0;


    barriers[i][j+1].kind = 4;
    barriers[i][j+1].lives = 2;
    barriers[i][j+1].x = 57 + (j*6);
    barriers[i][j+1].y = 62 + (i*3);
    barriers[i][j+1].rotation = 0;

    barriers[i+1][j+1].kind = 3;
    barriers[i+1][j+1].lives = 4;
    barriers[i+1][j+1].x = 57 + (j*6);
    barriers[i+1][j+1].y = 65 + (i*3);
    barriers[i+1][j+1].rotation = 0;

    barriers[i+2][j+1].kind = 3;
    barriers[i+2][j+1].lives = 4;
    barriers[i+2][j+1].x = 57 + (j*6);
    barriers[i+2][j+1].y = 68 + (i*3);
    barriers[i+2][j+1].rotation = 3;

    barriers[i+3][j+1].kind = 4;
    barriers[i+3][j+1].lives = 2;
    barriers[i+3][j+1].x = 57 + (j*6);
    barriers[i+3][j+1].y = 71 + (i*3);
    barriers[i+3][j+1].rotation = 0;


    barriers[i][j+2].kind = 4;
    barriers[i][j+2].lives = 2;
    barriers[i][j+2].x = 63 + (j*6);
    barriers[i][j+2].y = 62 + (i*3);
    barriers[i][j+2].rotation = 0;

    barriers[i+1][j+2].kind = 3;
    barriers[i+1][j+2].lives = 4;
    barriers[i+1][j+2].x = 63 + (j*6);
    barriers[i+1][j+2].y = 65 + (i*3);
    barriers[i+1][j+2].rotation = 1;

    barriers[i+2][j+2].kind = 3;
    barriers[i+2][j+2].lives = 4;
    barriers[i+2][j+2].x = 63 + (j*6);
    barriers[i+2][j+2].y = 68 + (i*3);
    barriers[i+2][j+2].rotation = 2;

    barriers[i+3][j+2].kind = 4;
    barriers[i+3][j+2].lives = 2;
    barriers[i+3][j+2].x = 63 + (j*6);
    barriers[i+3][j+2].y = 71 + (i*3);
    barriers[i+3][j+2].rotation = 0;


    barriers[i][j+3].kind = 4;
    barriers[i][j+3].lives = 2;
    barriers[i][j+3].x = 69 + (j*6);
    barriers[i][j+3].y = 62 + (i*3);
    barriers[i][j+3].rotation = 0;

    barriers[i+1][j+3].kind = 4;
    barriers[i+1][j+3].lives = 2;
    barriers[i+1][j+3].x = 69 + (j*6);
    barriers[i+1][j+3].y = 65 + (i*3);
    barriers[i+1][j+3].rotation = 0;

    barriers[i+2][j+3].kind = 4;
    barriers[i+2][j+3].lives = 2;
    barriers[i+2][j+3].x = 69 + (j*6);
    barriers[i+2][j+3].y = 68 + (i*3);
    barriers[i+2][j+3].rotation = 0;

    barriers[i+3][j+3].kind = 4;
    barriers[i+3][j+3].lives = 2;
    barriers[i+3][j+3].x = 69 + (j*6);
    barriers[i+3][j+3].y = 71 + (i*3);
    barriers[i+3][j+3].rotation = 0;
}

void show_logo(int x, int y)
{
    place_object(x, y, 7, 16, &LOGO_spaceship[0][0][0]);
    place_object(x+22, y-2, 11, 17, &LOGO_s1[0][0][0]);
    place_object(x+39, y-2, 11, 17, &LOGO_p[0][0][0]);
    place_object(x+57, y-3, 12, 18, &LOGO_a1[0][0][0]);
    place_object(x+76, y-2, 11, 16, &LOGO_c[0][0][0]);
    place_object(x+94, y-2, 11, 16, &LOGO_e1[0][0][0]);
    place_object(x+120, y-5, 16, 17, &LOGO_nokia[0][0][0]);
    place_object(x+145, y-4, 15, 31, &LOGO_star[0][0][0]);
    place_object(x-3, y+12, 10, 17, &LOGO_astronaut[0][0][0]);
    place_object(x+18, y+13, 9, 27, &LOGO_planet[0][0][0]);
    place_object(x+49, y+13, 10, 8, &LOGO_i[0][0][0]);
    place_object(x+58, y+13, 10, 12, &LOGO_n[0][0][0]);
    place_object(x+71, y+13, 10, 13, &LOGO_v[0][0][0]);
    place_object(x+84, y+13, 10, 12, &LOGO_a2[0][0][0]);
    place_object(x+97, y+13, 10, 11, &LOGO_d[0][0][0]);
    place_object(x+109, y+13, 10, 11, &LOGO_e2[0][0][0]);
    place_object(x+121, y+13, 10, 12, &LOGO_r[0][0][0]);
    place_object(x+133, y+13, 10, 11, &LOGO_s2[0][0][0]);
    place_object(x+150, y+13, 11, 21, &LOGO_earth[0][0][0]);
    place_object(x+35, y+24, 7, 11, &LOGO_blink[0][0][0]);
}

void manage_rocket()
{
    if(rocket_y >= 1){
        bool flag = true;
        for(int i = 3; i > 3-rocket_speed && flag; i--){
            for(int j = -2; j < 1 && flag; j++){
                if(invader_rockets[rocket_y+i][rocket_x+j].alive){
                    invader_rockets[rocket_y+i][rocket_x+j].alive = false;
                    rocket_in = false;
                    flag = false;
                }
            }
        }

        for(int i = 3; i >= 0 && flag; i--){
            for(int j = 0; j < 37 && flag; j++){
                if(barriers[i][j].lives > 0){
                    if((rocket_y <= barriers[i][j].y+3 && rocket_y > barriers[i][j].y+3-rocket_speed) && (rocket_x >= barriers[i][j].x && rocket_x < barriers[i][j].x+6)){
                        flag = false;
                        rocket_in = false;
                        barriers[i][j].lives--;
                    }
                }
            }
        }

        for(int i = 0; i < 5 && flag; i++){
            for(int j = 0; j < 11 && flag; j++){
                if(invaders[i][j].alive){
                    switch(invaders[i][j].kind){
                        case 1:
                            if((rocket_y <= invaders[i][j].y+4 && rocket_y > invaders[i][j].y+4-rocket_speed) && (rocket_x >= invaders[i][j].x && rocket_x <= invaders[i][j].x+12)){
                                flag = false;
                                rocket_in = false;
                                invaders[i][j].exploded = 4;
                                invader_rocket_fr *= 0.98;
                                score += 10;
                                if(soundeffects)
                                    mciSendString("play invaderkilledmusic from 0", NULL, 0, NULL);
                            }
                            break;
                        case 2:
                            if((rocket_y <= invaders[i][j].y+4 && rocket_y > invaders[i][j].y+4-rocket_speed) && (rocket_x >= invaders[i][j].x && rocket_x <= invaders[i][j].x+11)){
                                flag = false;
                                rocket_in = false;
                                invaders[i][j].exploded = 4;
                                invader_rocket_fr *= 0.98;
                                score += 20;
                                if(soundeffects)
                                    mciSendString("play invaderkilledmusic from 0", NULL, 0, NULL);
                            }
                            break;
                        case 3:
                            if((rocket_y <= invaders[i][j].y+4 && rocket_y > invaders[i][j].y+4-rocket_speed) && (rocket_x >= invaders[i][j].x+2 && rocket_x <= invaders[i][j].x+10)){
                                flag = false;
                                rocket_in = false;
                                invaders[i][j].exploded = 4;
                                invader_rocket_fr *= 0.98;
                                score += 40;
                                if(soundeffects)
                                    mciSendString("play invaderkilledmusic from 0", NULL, 0, NULL);
                            }
                            break;
                    }
                }
            }
        }

        if(flag && invaderS_in && (rocket_y <= invaderS_y+4 && rocket_y > invaderS_y+4-rocket_speed && rocket_x >= invaderS_x && rocket_x <= invaderS_x+16)){
            flag = false;
            rocket_in = false;
            invaderS_in = false;
            mciSendString("stop invaderSinmusic", NULL, 0, NULL);
            if(soundeffects)
                mciSendString("play invaderSdeathmusic from 1000", NULL, 0, NULL);

            if(faze == -20){
                faze = -99;
                faze_changed = true;
            }
            else{
                score += 100;
                invaderS_fcounter = 1;
            }
        }

        for(int i = 0; i < 10; i++){
            if(buttons[i].is_there){
                if((rocket_y <= buttons[i].y+3 && rocket_y > buttons[i].y+3-rocket_speed) && (rocket_x >= buttons[i].x && rocket_x <= buttons[i].x+buttons[i].len)){
                    flag = false;
                    rocket_in = false;
                    invaderS_in = false;
                    mciSendString("stop invaderSinmusic", NULL, 0, NULL);
                    if(soundeffects)
                        mciSendString("play buttonmusic from 0", NULL, 0, NULL);
                    print_on_screen(buttons[i].txt, buttons[i].x, buttons[i].y, 1, 4, 1);
                    update();
                    Sleep(300);
                    print_on_screen(buttons[i].txt, buttons[i].x, buttons[i].y, 1, 7, 1);
                    update();
                    Sleep(300);
                    print_on_screen(buttons[i].txt, buttons[i].x, buttons[i].y, 1, 4, 1);
                    update();
                    Sleep(300);
                    print_on_screen(buttons[i].txt, buttons[i].x, buttons[i].y, 1, 7, 1);
                    update();
                    Sleep(300);
                    print_on_screen(buttons[i].txt, buttons[i].x, buttons[i].y, 1, 4, 1);
                    update();
                    Sleep(2500);

                    while(_kbhit())
                        _getch();

                    faze_changed = true;
                    change_faze(buttons[i].txt);
                }
            }
        }

        if(flag){
            updated_screen[rocket_y][rocket_x].fg = rocket_c;
            updated_screen[rocket_y][rocket_x].bg = rocket_c;
            updated_screen[rocket_y-1][rocket_x].fg = rocket_c;
            updated_screen[rocket_y-1][rocket_x].bg = rocket_c;
        }
    }
    else{
        rocket_in = false;
    }
}

void manage_invader_rockets()
{
    for(int i = 0; i < SCREEN_HEIGHT; i++){
        for(int j = SCREEN_WIDTH-1; j >= 0; j--){
            if(invader_rockets[i][j].alive){
                bool flag = true;

                int b_i = (i - 62)/3;
                int b_j = (j - 51)/6;
                if(b_i >= 0 && b_i < 4 && b_j >= 0 && b_j < 37){
                    if(barriers[b_i][b_j].lives > 0){
                        if((i <= barriers[b_i][b_j].y+3 && i >= barriers[b_i][b_j].y+1-invader_rocket_speed) && (j >= barriers[b_i][b_j].x && j < barriers[b_i][b_j].x+6)){
                            invader_rockets[i][j].alive = false;
                            barriers[b_i][b_j].lives--;
                            flag = false;
                        }
                    }
                }

                if((i <= t_y+3 && i >= t_y+3-invader_rocket_speed) && (j >= t_x && j < t_x+15)){
                    destroyed = true;
                    invader_rockets[i][j].alive = false;
                    flag = false;
                    if(soundeffects)
                        mciSendString("play explosionmusic from 0", NULL, 0, NULL);
                }

                if(flag){
                    place_invader_rocket(invader_rockets[i][j].kind, j, i, invader_rockets[i][j].status);
                    if(invader_rockets[i][j].status < 2)
                        invader_rockets[i][j].status++;
                    else
                        invader_rockets[i][j].status = 0;
                    }
            }
        }
    }
}

bool manage_invaders()
{
    bool flag = true;
    if(dir){
        for(int i = 0; i < 5; i++){
            for(int j = 10; j >= 0; j--){
                if(invaders[i][j].alive){
                    flag = false;
                    if(invaders[i][j].exploded > 0 ){
                        place_invader(-1, invaders[i][j].x, invaders[i][j].y-1, invaders[i][j].status);
                        invaders[i][j].exploded--;
                        if(invaders[i][j].exploded == 0)
                            invaders[i][j].alive = false;
                    }
                    else{
                        place_invader(invaders[i][j].kind, invaders[i][j].x, invaders[i][j].y, invaders[i][j].status);
                        int fr = (int)(invader_rocket_fr*(8.0/(7.0+(float)(invaders[i][j].y/5))));
                        if(invaders[i][j].kind == 3){
                            if(rand() % fr == 0){
                                invader_rockets[invaders[i][j].y][invaders[i][j].x+6].alive = true;
                                if(rand() % 2 == 0)
                                    invader_rockets[invaders[i][j].y][invaders[i][j].x+6].kind = 1;
                                else
                                    invader_rockets[invaders[i][j].y][invaders[i][j].x+6].kind = 2;
                                invader_rockets[invaders[i][j].y][invaders[i][j].x+6].status = 0;
                            }
                        }
                        else if(invaders[i][j].kind == 2){
                            if(rand() % (2*fr) == 0){
                                invader_rockets[invaders[i][j].y][invaders[i][j].x+6].alive = true;
                                invader_rockets[invaders[i][j].y][invaders[i][j].x+6].kind = 1;
                                invader_rockets[invaders[i][j].y][invaders[i][j].x+6].status = 0;
                            }
                        }

                        if(count_f == 0){
                            invaders[i][j].status = !invaders[i][j].status;
                            if(dir && invaders[i][j].x >= 288){
                                dir = false;
                                for(int k = 0; k < i; k++){
                                    for(int z = 0; z < 11; z++){
                                        invaders[k][z].y += 5;
                                        invaders[k][z].x -= 3;
                                    }
                                }
                            }
                            if(!dir){
                                invaders[i][j].y += 5;
                                if(invaders[i][j].y > 70)
                                    game_over = true;
                            }
                            else
                                invaders[i][j].x += 3;
                        }
                    }
                }
            }
        }
    }
    else{
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 11; j++){
                if(invaders[i][j].alive){
                    flag = false;
                    if(invaders[i][j].exploded > 0){
                        place_invader(-1, invaders[i][j].x, invaders[i][j].y-1, invaders[i][j].status);
                        invaders[i][j].exploded--;
                        if(invaders[i][j].exploded == 0)
                            invaders[i][j].alive = false;
                    }
                    else{
                        place_invader(invaders[i][j].kind, invaders[i][j].x, invaders[i][j].y, invaders[i][j].status);
                        int fr = (int)(invader_rocket_fr*(8.0/(7.0+(float)(invaders[i][j].y/5))));
                        if(invaders[i][j].kind == 3){
                            if(rand() % fr == 0){
                                invader_rockets[invaders[i][j].y][invaders[i][j].x+6].alive = true;
                                if(rand() % 2 == 0)
                                    invader_rockets[invaders[i][j].y][invaders[i][j].x+6].kind = 1;
                                else
                                    invader_rockets[invaders[i][j].y][invaders[i][j].x+6].kind = 2;
                                invader_rockets[invaders[i][j].y][invaders[i][j].x+6].status = 0;
                            }
                        }
                        else if(invaders[i][j].kind == 2){
                            if(rand() % (2*fr) == 0){
                                invader_rockets[invaders[i][j].y][invaders[i][j].x+6].alive = true;
                                invader_rockets[invaders[i][j].y][invaders[i][j].x+6].kind = 1;
                                invader_rockets[invaders[i][j].y][invaders[i][j].x+6].status = 0;
                            }
                        }

                        if(count_f == 0){
                            invaders[i][j].status = !invaders[i][j].status;
                            if(!dir && invaders[i][j].x <= 45){
                                dir = true;
                                invader_speed--;
                                for(int k = 0; k < i; k++){\
                                    for(int z = 0; z < 11; z++){
                                        invaders[k][z].y += 5;
                                        invaders[k][z].x += 3;
                                    }
                                }
                            }
                            if(dir){
                                invaders[i][j].y += 5;
                                if(invaders[i][j].y > 70)
                                    game_over = true;
                            }
                            else
                                invaders[i][j].x -= 3;
                        }
                    }
                }
            }
        }
    }
    return flag;
}

void manage_invaderS()
{
    if(invaderS_in){
        if(invaderS_dir){
            if(invaderS_x < SCREEN_WIDTH){
                place_invader(0, invaderS_x, invaderS_y, 0);
            }
            else{
                mciSendString("stop invaderSinmusic", NULL, 0, NULL);
                invaderS_in = false;
            }
        }else{
            if(invaderS_x > -15){
                place_invader(0, invaderS_x, invaderS_y, 0);
            }
            else{
                mciSendString("stop invaderSinmusic", NULL, 0, NULL);
                invaderS_in = false;
            }
        }
    }
    if(invaderS_fcounter > 0){
        if(invaderS_fcounter > 20){
            invaderS_fcounter = 0;
        }
        else{
            if(invaderS_fcounter < 3)
                place_hundred(invaderS_x, invaderS_y);
            if(invaderS_fcounter >= 5 && invaderS_fcounter < 7)
                place_hundred(invaderS_x, invaderS_y);
            if(invaderS_fcounter >= 9)
                place_hundred(invaderS_x, invaderS_y);
            invaderS_fcounter++;
        }
    }
}

void show_destroyed_animation()
{
    for(int i = 0; i < 12; i++){
        show_destruction(t_x, t_y, 10+(i%2));
        Sleep(50);
    }
    Sleep(2000);
    show_destruction(t_x, t_y, 12);
    if(lives > 0){
        lives--;
        for(int i = 0; i < 6; i++){
            if(backup_tanks[i].is_there){
                int n_x = backup_tanks[i].x;
                int n_y = backup_tanks[i].y;
                while(n_y < 81){
                    n_y++;
                    show_destruction(n_x, n_y, tank_num);
                    //Sleep(10);
                }

                if(i < 3){
                    while(n_x < 73){
                        n_x++;
                        show_destruction(n_x, n_y, tank_num);
                        //Sleep(10);
                    }
                }
                else{
                    while(n_x > 254){;
                        n_x--;
                        show_destruction(n_x, n_y, tank_num);
                        //Sleep(10);
                    }
                }
                t_x = n_x;
                backup_tanks[i].is_there = false;
                break;
            }
        }
    }
    else{
        game_over = true;
    }
    destroyed = false;
    while(_kbhit())
        _getch();
}

void instructions(int &ins_itr)
{
    string ins_text = "SPACE INVADERS IS AN OLD ARCADE GAME WHERE PLAYERS TRY TO\nSHOOT DOWN WAVES OF DECENDING ALIENS\n";
    ins_text += "THIS IS A IMPLIMENTATION OF THAT GAME IN WINDOWS TERMINAL\nREPRESENTED TO YOU BY KIA SHEIKHI AND SOHEIL VOJDANI\nAKA THE NOKIA TEAM\nPLEASE ENJOY\n\n";
    ins_text += "CONTROLS ARE A AND D FOR MOVING LEFT AND RIGHT AND SPACE FOR\nSHOOTING\n";
    ins_text += "IF YOU CHOOSE CHALLENGING MORE ENEMIES COME AT YOU EACH ROUND\nYOU ALSO GET A ADDITIONAL LIVE AFTER BEATING EACH ROUND\n";
    ins_text += "AND THE MAXIMUM AMOUNT OF BACKUP LIVES IS SIX\nYOU CAN PAUSE THE GAME BY PRESSING X IN ORDER TO SAVE YOUR GAME\n";
    ins_text += "IN THE SETTINGS YOU CAN CHANGE THE TANK ICON AND GAME COLORS\nYOU CAN ALSO TURN OFF AND ON MUSIC AND SOUND EFFECTS FROM\nSETTINGS\n\n";
    ins_text += "GO BACK TO MAIN MENU BY PRESSING ENTER";

    print_on_screen(ins_text.substr(0, ins_itr), 0, 3, 1, 7, 1);
    if(ins_itr < ins_text.length())
        ins_itr++;
}

void score_board()
{
    print_on_screen("TOP TEN PLAYERS", 130, 3, 1, 7, 1);
    print_on_screen("RANK             PLAYER             BEST SCORE             TIME", 40, 10, 1, 7, 1);
    fstream data_file;
    data_file.open("ScoreBoard.txt", ios::in);
    string player_name;
    int player_score;
    int player_time;
    int count = 1;
    string garbage;
    getline(data_file, garbage);
    while(data_file >> player_name >> player_score >> player_time){
        show_score(count, 40, 9+6*count);
        print_on_screen(player_name, 103,10+6*count, 1, 12, 1);
        show_score(player_score, 177, 9+6*count);
        show_score(player_time, 273, 9+6*count);
        count++;
    }
    print_on_screen("PRESS ENTER TO RETURN", 5, 87, 1, 7, 1);
    data_file.close();
}

void enter_your_name()
{
    print_on_screen("ENTER YOUR NAME", 125, 20, 1, 7, 1);
    if(_kbhit()){
        char c = _getch();
        if((c >= 'A' && c <= 'Z') || c == ' '){
            name = name + c;
        }
        else if(c >= 'a' && c <= 'z'){
            name = name + (char)toupper(c);
        }
        else if(c == '\b'){
            if(name != "")
                name.pop_back();
        }
        else if(c == '\r'){
            if(name != ""){
                faze = -10;
                faze_changed = true;
            }
        }
    }
    int distance_from_center = 0;
    for(int i = 0; i < name.length(); i++){
        if(name[i] == 'L'){
            distance_from_center += 5;
        }
        else if(name[i] == 'I'){
            distance_from_center += 4;
        }
        else if(name[i] == 'M' || name[i] == 'W'){
            distance_from_center += 8;
        }
        else if(name[i] == ' '){
            distance_from_center += 3;
        }
        else{
            distance_from_center += 6;
        }
    }
    distance_from_center /= 2;
    print_on_screen(name, 168-distance_from_center, 25, 1, 12, 1);
}

void select_tank()
{
    print_on_screen("PRESS W OR A TO CHANGE TANK", 100, 20, 1, 7, 1);
    print_on_screen("PRESS ENTER TO CONFIRM AND GO BACK", 80, 30, 1, 7, 1);
    if(_kbhit()){
        char c = _getch();
        switch(c){
            case 'a':
            case 'A':
                if(tank_num == 0)
                    tank_num = 9;
                else
                    tank_num--;
                break;

            case 'd':
            case 'D':
                if(tank_num == 9)
                    tank_num = 0;
                else
                    tank_num++;
                break;

            case '\r':
                faze = -17;
                faze_changed = true;
                break;
        }
    }
}

void adjust_audio(int &sound_itr)
{
    print_on_screen("USE WASD TO MOVE", 45, 50, 1, 7, 1);
    print_on_screen("SPACE TO TURN AUDIO ON AND OFF", 45, 57, 1, 7, 1);
    print_on_screen("AND ENTER TO CONFIRM AND RETURN", 45, 64, 1, 7, 1);

    if(menumusic)
        print_on_screen("ON", 45, 30, 1, 2, 1);
    else
        print_on_screen("OFF", 45, 30, 1, 4, 1);

    if(gamemusic)
        print_on_screen("ON", 133, 30, 1, 2, 1);
    else
        print_on_screen("OFF", 133, 30, 1, 4, 1);

    if(soundeffects)
        print_on_screen("ON", 221, 30, 1, 2, 1);
    else
        print_on_screen("OFF", 221, 30, 1, 4, 1);

    print_on_screen(buttons[sound_itr].txt, buttons[sound_itr].x, buttons[sound_itr].y, 1, 12, 1);


    if(_kbhit()){
        char c = _getch();
        switch(c){
            case 'a':
            case 'A':
                if(sound_itr > 0)
                    sound_itr--;
                break;

            case 'd':
            case 'D':
                if(sound_itr < 2)
                    sound_itr++;
                break;

            case '\r':
                faze = -17;
                faze_changed = true;
                break;

            case ' ':
                if(sound_itr == 0){
                    menumusic = !menumusic;
                    if(!menumusic)
                        mciSendString("stop menumusic", NULL, 0, NULL);
                    else
                        mciSendString("play menumusic repeat from 0", NULL, 0, NULL);
                }
                else if(sound_itr == 1){
                    gamemusic = !gamemusic;
                }
                else if(sound_itr == 2){
                    soundeffects = !soundeffects;
                }
                break;
        }
    }
}

void choose_color(int &color_itr)
{
    print_on_screen("USE WASD TO MOVE", 45, 50, 1, 7, 1);
    print_on_screen("SPACE TO CHANGE COLOR", 45, 57, 1, 7, 1);
    print_on_screen("AND ENTER TO CONFIRM AND RETURN", 45, 64, 1, 7, 1);

    print_on_screen(buttons[color_itr].txt, buttons[color_itr].x, buttons[color_itr].y, 1, 12, 1);

    place_tank(72, 9);
    updated_screen[10][170].fg = rocket_c;
    updated_screen[10][170].bg = rocket_c;
    updated_screen[11][170].fg = rocket_c;
    updated_screen[11][170].bg = rocket_c;

    place_invader_rocket(1, 278, 10, 1);
    place_invader_rocket(2, 286, 10, 1);

    place_invader(1, 101, 19, 0);
    place_invader(2, 185, 19, 1);
    place_invader(3, 268, 19, 1);

    place_barrier(3, 94, 28, 4, 0);
    place_barrier(3, 100, 28, 4, 1);
    place_barrier(3, 94, 31, 4, 3);
    place_barrier(3, 100, 31, 4, 2);

    place_invader(0, 270, 29, 0);
    if(_kbhit()){
        char c = _getch();
        switch(c){
            case 'a':
            case 'A':
                if(color_itr >= 9)
                    break;
                if(color_itr % 3 != 0)
                    color_itr--;
                break;

            case 'd':
            case 'D':
                if(color_itr >= 9)
                    break;
                if(color_itr % 3 != 2)
                    color_itr++;
                break;

            case 's':
            case 'S':
                if(color_itr >= 9)
                    break;
                if(color_itr == 7)
                    color_itr = 9;
                if(color_itr < 6)
                    color_itr += 3;
                break;

            case 'w':
            case 'W':
                if(color_itr > 9)
                    break;
                if(color_itr == 9){
                    color_itr = 7;
                    break;
                }
                if(color_itr > 2)
                    color_itr -= 3;
                break;

            case '\r':
                    faze = -17;
                    faze_changed = true;
                break;

            case ' ':
                if(color_itr == 0){
                    if(t_c < 15)
                        t_c++;
                    else
                        t_c = 0;
                }
                else if(color_itr == 1){
                    if(rocket_c < 15)
                        rocket_c++;
                    else
                        rocket_c = 0;
                }
                else if(color_itr == 2){
                    if(ro_a < 15 && ro_b < 15){
                        ro_a++;
                        ro_b++;
                    }
                    else{
                        ro_a = 0;
                        ro_b = 0;
                    }
                }
                else if(color_itr == 3){
                    if(in_a < 15)
                        in_a++;
                    else
                        in_a = 0;
                }
                else if(color_itr == 4){
                    if(in_b < 15)
                        in_b++;
                    else
                        in_b = 0;
                }
                else if(color_itr == 5){
                    if(in_c < 15)
                        in_c++;
                    else
                        in_c = 0;
                }
                else if(color_itr == 6){
                    if(br_a < 15 && br_b < 15 && br_c < 15 && br_d < 15){
                        br_a++;
                        br_b++;
                        br_c++;
                        br_d++;
                    }
                    else{
                        br_a = 0;
                        br_b = 0;
                        br_c = 0;
                        br_d = 0;
                    }
                }
                else if(color_itr == 7){
                    rand_color();
                }
                else if(color_itr == 8){
                    if(in_s < 15)
                        in_s++;
                    else
                        in_s = 0;
                }
                else if(color_itr == 9){
                    reset_color();
                }
                break;
        }
    }
}

void easter_egg()
{
    print_on_screen("ESTER EGG", 20, 30, 1, 9, 6);
}

void save_game()
{
    ofstream out_file("Data.txt");
    out_file << faze << endl;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 11; j++){
            out_file << invaders[i][j].alive << " " << invaders[i][j].exploded << " " << invaders[i][j].kind << " " << invaders[i][j].status << " " << invaders[i][j].x << " " << invaders[i][j].y << "  ";
        }
        out_file << endl;
    }
    for(int i = 0; i < SCREEN_HEIGHT; i++){
        for(int j = 0; j < SCREEN_WIDTH; j++){
            out_file << invader_rockets[i][j].alive << " " << invader_rockets[i][j].kind << " " << invader_rockets[i][j].status << "  ";
        }
        out_file << endl;
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 37; j++){
            out_file << barriers[i][j].kind << " " << barriers[i][j].lives << " " << barriers[i][j].rotation << " " << barriers[i][j].x << " " << barriers[i][j].y << "  ";
        }
        out_file << endl;
    }
    out_file << live_increased << endl;
    for(int i = 0; i < 6; i++){
        out_file << backup_tanks[i].is_there << " " << backup_tanks[i].x << " " << backup_tanks[i].y << "  ";
    }
    out_file << endl;
    out_file << move_num << endl;
    out_file << dir << endl;
    out_file << count_f << endl;
    out_file << t_x << " " << t_y << endl;
    out_file << button_f << endl;
    out_file << invader_rocket_speed << endl;
    out_file << invader_rocket_fr << endl;
    out_file << rocket_speed << endl;
    out_file << rocket_x << " " << rocket_y << endl;
    out_file << rocket_in << endl;
    out_file << invaderS_speed << endl;
    out_file << invaderS_x << " " << invaderS_y << endl;
    out_file << invaderS_in << endl;
    out_file << invaderS_dir << endl;
    out_file << invaderS_fcounter << endl;
    out_file << game_over << endl;
    out_file << destroyed << endl;
    out_file << game_in_progress << endl;
    out_file << faze_changed << endl;
    out_file << score << endl;
    out_file << lives << endl;
    out_file << tank_num << endl;
    out_file << rand_c << endl;
    out_file << t_c << endl;
    out_file << rocket_c << endl;
    out_file << in_a << endl;
    out_file << in_b << endl;
    out_file << in_c << endl;
    out_file << in_s << endl;
    out_file << ro_a << endl;
    out_file << ro_b << endl;
    out_file << in_E << endl;
    out_file << S_E << endl;
    out_file << br_a << endl;
    out_file << br_b << endl;
    out_file << br_c << endl;
    out_file << br_d << endl;
    out_file << play_time << endl;
    out_file << menumusic << endl;
    out_file << gamemusic << endl;
    out_file << soundeffects << endl;
    for(int i = 0; i < 10; i++){
        out_file << num_c[i] << " ";
    }
    out_file << endl;
    for(int i = 0; i < 26; i++){
        out_file << let_c[i] << " ";
    }
    out_file << endl;
    out_file << name << endl;


    out_file.close();
}

void load_game()
{
    ifstream in_file("Data.txt");
    in_file >> faze;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 11; j++){
            in_file >> invaders[i][j].alive >> invaders[i][j].exploded >> invaders[i][j].kind >> invaders[i][j].status >> invaders[i][j].x >> invaders[i][j].y;
        }
    }
    for(int i = 0; i < SCREEN_HEIGHT; i++){
        for(int j = 0; j < SCREEN_WIDTH; j++){
            in_file >> invader_rockets[i][j].alive >> invader_rockets[i][j].kind >> invader_rockets[i][j].status;
        }
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 37; j++){
            in_file >> barriers[i][j].kind >> barriers[i][j].lives >> barriers[i][j].rotation >> barriers[i][j].x >> barriers[i][j].y;
        }
    }
    in_file >> live_increased;
    for(int i = 0; i < 6; i++){
        in_file >> backup_tanks[i].is_there >> backup_tanks[i].x >> backup_tanks[i].y;
    }
    in_file >> move_num;
    in_file >> dir;
    in_file >> count_f;
    in_file >> t_x >> t_y;
    in_file >> button_f;
    in_file >> invader_rocket_speed;
    in_file >> invader_rocket_fr;
    in_file >> rocket_speed;
    in_file >> rocket_x >> rocket_y;
    in_file >> rocket_in;
    in_file >> invaderS_speed;
    in_file >> invaderS_x >> invaderS_y;
    in_file >> invaderS_in;
    in_file >> invaderS_dir;
    in_file >> invaderS_fcounter;
    in_file >> game_over;
    in_file >> destroyed;
    in_file >> game_in_progress;
    in_file >> faze_changed;
    in_file >> score;
    in_file >> lives;
    in_file >> tank_num;
    in_file >> rand_c;
    in_file >> t_c;
    in_file >> rocket_c;
    in_file >> in_a;
    in_file >> in_b;
    in_file >> in_c;
    in_file >> in_s;
    in_file >> ro_a;
    in_file >> ro_b;
    in_file >> in_E;
    in_file >> S_E;
    in_file >> br_a;
    in_file >> br_b;
    in_file >> br_c;
    in_file >> br_d;
    in_file >> play_time;
    in_file >> menumusic;
    in_file >> gamemusic;
    in_file >> soundeffects;
    for(int i = 0; i < 10; i++){
        in_file >> num_c[i];
    }
    for(int i = 0; i < 26; i++){
        in_file >> let_c[i];
    }
    getline(in_file, name);

    in_file.close();
}

void update_scoreboard(const string &player_name, int player_score, int player_time)
{
    ifstream data_file("ScoreBoard.txt");
    int num = 0;
    if(data_file.is_open())
        data_file >> num;

    Player* players = new Player[num];

    for(int i = 0; i < num; i++)
        data_file >> players[i].name >> players[i].score >> players[i].time;

    bool found = false;
    for(int i = 0; i < num; i++){
        if(players[i].name == player_name){
            if(player_score > players[i].score){
                players[i].score = player_score;
                players[i].time = player_time;
            }
            found = true;
            break;
        }
    }

    if(!found){
        Player* new_players = new Player[num + 1];
        for(int i = 0; i < num; i++)
            new_players[i] = players[i];
        new_players[num].name = player_name;
        new_players[num].score = player_score;
        new_players[num].time = player_time;
        delete [] players;
        players = new_players;
        num++;
    }

    for(int i = 0; i < num - 1; i++){
        for(int j = 0; j < num - i - 1; j++){
            if(players[j].score < players[j+1].score){
                Player temp = players[j];
                players[j] = players[j+1];
                players[j+1] = temp;
            }
            else if(players[j].score == players[j+1].score && players[j].time > players[j+1].time){
                Player temp = players[j];
                players[j] = players[j+1];
                players[j+1] = temp;
            }
        }
    }

    ofstream out_file("ScoreBoard.txt");
    out_file << min(num, 10) << "\n";
    for(int i = 0; i < num && i < 10; i++)
        out_file << players[i].name << " " << players[i].score << " " << players[i].time << "\n";

    out_file.close();

    delete [] players;
}

