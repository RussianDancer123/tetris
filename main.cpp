#include <iostream>
#include <vector>
#include <unistd.h>
#include <curses.h>
#include <cstdlib>
#include "headers.h"

int tab[20][10][2] = {0};
int pointx[20] = {};
int pointy[10] = {};
int next;
bool game = true;
int score = 0;

int slep = 1000;
int skip = 400;


std::vector<Brick> v;


//----------move all down after scoring
void movedown(int p) {
    for (auto &b: v) {
        for (auto &s: b.tab) {
            if (s.x < p && s.x>0)
                s.x += 1;
        }
    }
}

void chkscore(){
    for(int i = 0; i < 20; i++) {
        bool score = true;
        for (int j = 0; j < 10; j++) {
            if (::tab[i][j][0] != 1)
                score = false;
        }
        if (score) {
            ::score += 100;
            for(auto &b : v){
                for (auto &s: b.tab) {
                    if(s.x==i) {
                        s = Square(-1, -1);
                    }
                }
            }
            movedown(i);
        }
    }
}

void addBrick(){
    // 1-sprawdza i obniza reszte 2-
    chkscore();

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
};

class NextWindow : MainWindow {
public:
    WINDOW * nextwin;

    void print() override{
        nextwin = newwin(12, 34, 0, 45);
        box(nextwin, 0, 0);
        mvwprintw(nextwin, 0, getmaxx(nextwin)/2-2, "Next");
        mvwaddch(nextwin, 2, 2, char(::next+48));
    }
};

class ScoreWindow : MainWindow {
public:
    WINDOW * scorewin;

    void print() override{
        scorewin = newwin(30,34,12,45);
        box(scorewin, 0, 0);
        mvwprintw(scorewin, 0, getmaxx(scorewin)/2-2, "Score");
        mvwprintw(scorewin, 4, getmaxx(scorewin)/2-+5, "%s", std::to_string(score).data());
    }
};

class DeathWindow : MainWindow {
public:
    WINDOW * deathwin;

    void print() override{
        //center?
        deathwin = newwin(7, 15, 15, 36);
        box(deathwin, 0, 0);
        mvwprintw(deathwin, 0, getmaxx(deathwin)/2-2, "DEATH");
        mvwprintw(deathwin, 2, getmaxx(deathwin)/2-2, "SCORE");
        mvwprintw(deathwin, 4, getmaxx(deathwin)/2-2, "%s", std::to_string(score).data());
    }
};

void eraseTab(){
    //----------------------------------------------clean everything
    for (auto & i : ::tab)
        for (auto & j : i)
            j[0] = 0;


}



void rotate(){
    Brick &b = v.back();
    const int n=4, m=4;
    int tmpTab[n][m] = {{0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0}};


    //min x, y
    int xOff, yOff;
    Square ymin = b.tab[0];
    for(auto i: b.tab){
        if(i.y<ymin.y)
            ymin = i;
    }

    Square xmin = b.tab[0];
    for(auto i: b.tab){
        if(i.x<xmin.x)
            xmin = i;
    }

    //offset przesuwa do (0, 0) w lewym gornym rogu aby dac do tablicy 4x4 a pozniej przesowa
    //z powrotem na miejsce
    xOff = xmin.x;
    yOff = ymin.y;

    for(auto i : b.tab){
        tmpTab[(i.x)-xOff][(i.y)-yOff] = 1;
    }

    int tmpTab2[n][m]={0};
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            tmpTab2[j][i] = tmpTab[3-i][j];
        }
    }

    int x = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(tmpTab2[i][j]==1){
                b.tab[x].x = i+xOff;
                b.tab[x].y = j+yOff;
                x++;
            }
        }
    }


}

void sideMove(int n, Brick& b){
    Square ymin = b.tab[0];
    for(auto i: b.tab){
        if(i.y<ymin.y)
            ymin = i;
    }

    Square ymax = b.tab[0];
    for(auto i: b.tab){
        if(i.y>ymax.y)
            ymax = i;
    }


    if(ymin.y+n >= 0 && ymax.y+n <= 9) {

        for(auto i : b.tab){
            if(tab[i.x][i.y+n][0] == 1){
                bool sk = true;
                for(auto j : b.tab){
                    if(i.x == j.x && i.y+n == j.y)
                        sk = false;
                }
                if(sk)
                    return;
            }
        }

        for(auto &i : b.tab){
            i.y+=n;
        }
    }
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
            return;
        default:
            return;
    }

    flushinp();
}

void death(){
    DeathWindow deathwin = DeathWindow();
    clear();
    std::cout<<"death";
    ::game = false;
    while(1==1){
        deathwin.print();
        wrefresh(deathwin.deathwin);
        usleep(1000000);
    }
}

void chkfail(){
    if(v.size()==1)
        return;
    Brick &b = v.end()[-2];

    Square smin = b.tab[0];

    for(auto i: b.tab){
        if(i.x<smin.x && i.x>=0) smin = i;
    }

    if(smin.x == 0){
        death();
    }
}

int main() {
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
        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 10; j++){
                mvwaddch(scorewin.scorewin, i+1, j+1, char(tab[i][j][0]+48));
            }
        }

        wrefresh(scorewin.scorewin);

        //usleep(1050000);
        usleep(slep);
        n++;
        if(n%skip==0)
            fall();
        move(getch());

        chkfail();

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