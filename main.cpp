#include <iostream>
#include <vector>
#include <unistd.h>
#include <curses.h>
#include <cstdlib>
#include <limits>
#include "headers.h"

//tab - plansza z kolorem w ostatnim, pointx/pointy - pozycje w terminalu (tab[1][1] == 1 drukuje kwadrat w pozycji pointx[1] pointy[1] w terminalu)
//next - nastepny klocek
int tab[20][10][2] = {0};
int pointx[20] = {};
int pointy[10] = {};
int next;
bool game = true;
int score = 0;

//slep - predkosc gry, skip - predkosc spadania klockow
int slep = 1000;
int skip = 400;

std::vector<Brick> v;


//sprawdza zapelnienie rzedow - jesli jest to dodaje pkt i usuwa oraz obniza jeszte
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
            movedown(i);
        }
    }
}


void addBrick(){
    chkscore();
    //sprawdza przegrana od postawienia pierwszego klocka
    if(v.size()>1)
        chkfail();

    //dodaje dany klocek wylosowany poprzednio do zmiennej next i losuje nastepny
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

//okienka i drukowanie
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

    void printBrick(int n){
        switch(n){
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
    }

    void print() override{
        nextwin = newwin(12, 34, 0, 45);
        box(nextwin, 0, 0);
        mvwprintw(nextwin, 0, getmaxx(nextwin)/2-2, "Next");

        start_color();
        initColors();
        wattron(nextwin, COLOR_PAIR(6));

        printBrick(next);

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

//ruch i spadanie
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

//smierc z oknem smierci
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

    //init
    initscr();
    curs_set(0);
    timeout(0);
    keypad(stdscr, TRUE);

    //okna i pierwszy klocek
    MainWindow mainwin = MainWindow();
    NextWindow nextwin = NextWindow();
    ScoreWindow scorewin = ScoreWindow();

    ::next = rand()%7;
    addBrick();

    fillTab();

    //czas gry
    int n = 0;
    while(game) {

        //okienka
        wclear(mainwin.mainwin);
        mainwin.print();

        wclear(nextwin.nextwin);
        nextwin.print();

        wclear(scorewin.scorewin);
        scorewin.print();

        wrefresh(mainwin.mainwin);
        wrefresh(nextwin.nextwin);
        wrefresh(scorewin.scorewin);

        //klatka gry, inkrementacja czasu i ruch klockow/spadanie
        usleep(slep);
        n++;
        if(n%skip==0)
            fall();
        move(getch());

        //przekroczenie max wartosci int
        if(n == std::numeric_limits<int>::max())
            n = 0;

        //czyszczenie planszy i zapelnienie na podstawie klockow
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