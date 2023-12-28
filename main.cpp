#include <iostream>
#include <vector>
#include <unistd.h>
#include <curses.h>
#include <cstdlib>
#include <cmath>
#include "headers.h"

int tab[20][10][2] = {0};
int pointx[20] = {};
int pointy[10] = {};
int next;
bool game = true;
int score = 0;

int slep = 1000;
int skip = 400;

struct {
    int pos[4] = {
            3, 10, 18, 25
    };

    void n0(WINDOW * win, int pos){
        mvwprintw(win, 5, pos, " 0000 ");
        mvwprintw(win, 6, pos, "00  00");
        mvwprintw(win, 7, pos, "00  00");
        mvwprintw(win, 8, pos, "00  00");
        mvwprintw(win, 9, pos, " 0000 ");
    }

    void n1(WINDOW * win, int pos){
        mvwprintw(win, 5, pos, "1111  ");
        mvwprintw(win, 6, pos, "  11  ");
        mvwprintw(win, 7, pos, "  11  ");
        mvwprintw(win, 8, pos, "  11  ");
        mvwprintw(win, 9, pos, "111111");
    }

    void n2(WINDOW * win, int pos){
        mvwprintw(win, 5, pos, " 2222 ");
        mvwprintw(win, 6, pos, "22  22");
        mvwprintw(win, 7, pos, "   22 ");
        mvwprintw(win, 8, pos, "  22  ");
        mvwprintw(win, 9, pos, "222222");
    }

    void n3(WINDOW * win, int pos){
        mvwprintw(win, 5, pos, " 3333 ");
        mvwprintw(win, 6, pos, "33  33");
        mvwprintw(win, 7, pos, "   333");
        mvwprintw(win, 8, pos, "33  33");
        mvwprintw(win, 9, pos, " 3333 ");
    }

    void n4(WINDOW * win, int pos){
        mvwprintw(win, 5, pos, "44  44");
        mvwprintw(win, 6, pos, "44  44");
        mvwprintw(win, 7, pos, "444444");
        mvwprintw(win, 8, pos, "    44");
        mvwprintw(win, 9, pos, "    44");
    }

    void n5(WINDOW * win, int pos){
        mvwprintw(win, 5, pos, "555555");
        mvwprintw(win, 6, pos, "55    ");
        mvwprintw(win, 7, pos, "55555 ");
        mvwprintw(win, 8, pos, "    55");
        mvwprintw(win, 9, pos, "55555 ");
    }

    void n6(WINDOW * win, int pos){
        mvwprintw(win, 5, pos, " 6666 ");
        mvwprintw(win, 6, pos, "66    ");
        mvwprintw(win, 7, pos, "66666 ");
        mvwprintw(win, 8, pos, "66  66");
        mvwprintw(win, 9, pos, " 6666 ");
    }

    void n7(WINDOW * win, int pos){
        mvwprintw(win, 5, pos, "777777");
        mvwprintw(win, 6, pos, "   77 ");
        mvwprintw(win, 7, pos, "  77  ");
        mvwprintw(win, 8, pos, " 77   ");
        mvwprintw(win, 9, pos, "77    ");
    }

    void n8(WINDOW * win, int pos){
        mvwprintw(win, 5, pos, " 8888 ");
        mvwprintw(win, 6, pos, "88  88");
        mvwprintw(win, 7, pos, " 8888 ");
        mvwprintw(win, 8, pos, "88  88");
        mvwprintw(win, 9, pos, " 8888 ");
    }

    void n9(WINDOW * win, int pos){
        mvwprintw(win, 5, pos, " 9999 ");
        mvwprintw(win, 6, pos, "99  99");
        mvwprintw(win, 7, pos, " 99999");
        mvwprintw(win, 8, pos, "    99");
        mvwprintw(win, 9, pos, " 9999 ");
    }

    void print(WINDOW * win, int num, int pos){
        switch(num){
            case 1:
                n1(win, this->pos[pos]);
                break;
            case 2:
                n2(win, this->pos[pos]);
                break;
            case 3:
                n3(win, this->pos[pos]);
                break;
            case 4:
                n4(win, this->pos[pos]);
                break;
            case 5:
                n5(win, this->pos[pos]);
                break;
            case 6:
                n6(win, this->pos[pos]);
                break;
            case 7:
                n7(win, this->pos[pos]);
                break;
            case 8:
                n8(win, this->pos[pos]);
                break;
            case 9:
                n9(win, this->pos[pos]);
                break;
            default:
                n0(win, this->pos[pos]);
            break;
        }

    }
}numbers;

std::vector<Brick> v;


//----------move all down after scoring


void chkscore(){
    for(int i = 0; i < 20; i++) {
        bool score = true;
        for (int j = 0; j < 10; j++) {
            if (::tab[i][j][0] != 1)
                score = false;
        }
        if (score) {
            ::score += 100;
            deleteRow(i);
            usleep(slep);
            movedown(i);
        }
    }
}

void addBrick(){
    // 1-sprawdza i obniza reszte 2-
    chkscore();
    if(v.size()>1)
        chkfail();

    switch(next){
        case 0:
            v.push_back(L());
            break;
        case 1:
            v.push_back(I());
            break;
        case 2:
            v.push_back(O());
            break;
        case 3:
            v.push_back(S());
            break;
        case 4:
            v.push_back(Z());
            break;
        case 5:
            v.push_back(J());
            break;
        case 6:
            v.push_back(T());
            break;
    }
    next = rand()%7;


}

class MainWindow {
public:
    WINDOW * mainwin;

    virtual void print(){
        mainwin = newwin(42,42,0,0);
        box(mainwin, 0, 0);
        mvwprintw(mainwin, 0, 1, "Tetris");

        start_color();

        initColors();

        for(int i = 0; i < 20; i++)
            for(int j = 0; j < 10; j++)
                if(::tab[i][j][0]==1) {
                    wattron(mainwin, COLOR_PAIR(::tab[i][j][1]));
                    mvwprintw(mainwin, ::pointx[i], ::pointy[j], "****");
                    mvwprintw(mainwin, ::pointx[i]+1, ::pointy[j], "****");
                    wattroff(mainwin, COLOR_PAIR(::tab[i][j][1]));
                }
    }

    void printScore(WINDOW * win){
        std::string s = std::to_string(score);
        while(s.length() != 4)
            s.insert(0, "0");


        for(int i = 0; i < 4; i++)
            numbers.print(win, int(s[i])-48, i);
    }
};

class NextWindow : MainWindow {
public:
    WINDOW * nextwin;

    void square(int x, int y){
        mvwprintw(nextwin, x, y, "******");
        mvwprintw(nextwin, x+1, y, "******");
        mvwprintw(nextwin, x+2, y, "******");
    }

    void print() override{
        nextwin = newwin(12, 34, 0, 45);
        box(nextwin, 0, 0);
        mvwprintw(nextwin, 0, getmaxx(nextwin)/2-2, "Next");

        start_color();
        initColors();
        wattron(nextwin, COLOR_PAIR(6));

        switch(next){
            case 0:
                square(6, 8);
                square(6, 14);
                square(6, 20);
                square(3, 20);
                break;
            case 1:

                square(4, 5);
                square(4, 11);
                square(4, 17);
                square(4, 23);
                break;
            case 2:

                square(3, 11);
                square(3, 17);
                square(6, 11);
                square(6, 17);
                break;
            case 3:

                square(6, 8);
                square(6, 14);
                square(3, 14);
                square(3, 20);
                break;
            case 4:

                square(3, 8);
                square(3, 14);
                square(6, 14);
                square(6, 20);
                break;
            case 5:

                square(3, 8);
                square(3, 14);
                square(3, 20);
                square(6, 20);
                break;
            case 6:

                square(6, 8);
                square(6, 14);
                square(6, 20);
                square(3, 14);
                break;
        }

        wattroff(nextwin, COLOR_PAIR(6));

    }
};

class ScoreWindow : MainWindow {
public:
    WINDOW * scorewin;

    void print() override{
        scorewin = newwin(30,34,12,45);
        box(scorewin, 0, 0);
        mvwprintw(scorewin, 0, getmaxx(scorewin)/2-2, "Score");

        printScore(scorewin);

    }
};

class DeathWindow : MainWindow {
public:
    WINDOW * deathwin;

    void print() override{

        deathwin = newwin(13, 34, 14, 26);
        box(deathwin, 0, 0);

        start_color();
        initColors();
        wattron(deathwin, COLOR_PAIR(7));

        mvwprintw(deathwin, 0, getmaxx(deathwin)/2-2, "DEATH");
        mvwprintw(deathwin, 2, getmaxx(deathwin)/2-2, "SCORE:");

        wattroff(deathwin, COLOR_PAIR(7));
        printScore(deathwin);

    }
};

void eraseTab(){
    //----------------------------------------------clean everything
    for (auto & i : ::tab)
        for (auto & j : i)
            j[0] = 0;
}

void move(int x) {
    Brick &b = v.back();

    switch (x) {
        case KEY_RIGHT:
            sideMove(1, b);
            break;
        case KEY_LEFT:
            sideMove(-1, b);
            break;
        case KEY_DOWN:
            fall();
            break;
        case KEY_UP:
            rotate();
            break;
        default:
            return;
    }

    flushinp();
}

void death(){
    DeathWindow deathwin = DeathWindow();
    clear();
    refresh();
    while(1==1){
        deathwin.print();
        wrefresh(deathwin.deathwin);
        usleep(1000000);
    }
}



int main() {
    //system("firefox https://www.youtube.com/watch?v=0Aq436MrS_o");
    srand((unsigned)time(NULL));
    initPoint();
    initColors();

    noecho();
    //zmiana rozmiaru terminala
    std::cout<<"\e[8;45;79t";

    initscr();
    curs_set(0);
    timeout(0);
    keypad(stdscr, TRUE);

    MainWindow mainwin = MainWindow();

    //next window
    NextWindow nextwin = NextWindow();

    //debug window - score?
    ScoreWindow scorewin = ScoreWindow();

    ::next = rand()%7;
    addBrick();

    fillTab();


    int n = 0;
    while(game) {

        wclear(mainwin.mainwin);
        mainwin.print();

        wclear(nextwin.nextwin);
        nextwin.print();

        wclear(scorewin.scorewin);
        scorewin.print();

        wrefresh(mainwin.mainwin);
        wrefresh(nextwin.nextwin);

        //debug
//        for(int i = 0; i < 20; i++){
//            for(int j = 0; j < 10; j++){
//                mvwaddch(scorewin.scorewin, i+1, j+1, char(tab[i][j][0]+48));
//            }
//        }

        wrefresh(scorewin.scorewin);

        //usleep(1050000);
        usleep(slep);
        n++;
        if(n%skip==0)
            fall();
        move(getch());

        eraseTab();
        fillTab();
    }

    delwin(mainwin.mainwin);
    delwin(nextwin.nextwin);
    delwin(scorewin.scorewin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}