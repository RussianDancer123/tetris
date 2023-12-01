#include <iostream>
#include <vector>
#include <unistd.h>
#include <curses.h>
#include <cstdlib>

int tab[20][10] = {0};
int pointx[20] = {};
int pointy[10] = {};
int next;
bool game = true;
int score = 0;

int slep = 1000;
int skip = 400;

void initPoint(){
    for(int i = 0; i < 20; i++){
        ::pointx[i] = (2*i)+1;
    }
    for(int j = 0; j < 10; j++){
        ::pointy[j] = (4*j)+1;
    }
}

class Square{
public:
    int x, y;
    bool alive = true;

    Square(int x = 0, int y = 0){
        this->x = x;
        this->y = y;
    }

};

//simplify class - less repeated code in kids

class Brick{

public:

    Square tab[4] = {Square(), Square(), Square(), Square()};

    static int ran(){
        return 1+(rand()%10);
    }

    static int chkY(int y, int n){
        return y-n<0? y: y-n;
    }

};

class I : public Brick {
public:
    int x = 0, y, n = 4;

    I(int y = ran()) {
        this->y = chkY(y, n);
        for (int i = 0; i < 4; i++) {
            tab[i].x = x;
            tab[i].y = i + this->y;
        }
    }
};

class L : public Brick{
public:
    int x = 0, y, n = 2;
    L(int y = ran()){
        this->y = chkY(y, n);
        for(int i = 0; i < 4; ++i) {
            tab[i].x = x + i - (i/3);
            tab[i].y = this->y + (i/3);
        }
    }
};

class J : public Brick{
public:
    int x = 0, y, n = 2;
    J(int y = ran()){
        this->y = chkY(y, n);
        for(int i = 0; i < 4; ++i) {
            tab[i].x = x + i - (i/3);
            tab[i].y = this->y - (i/3) + 1;
        }
    }
};

class O : public Brick{
public:
    int x = 0, y, n = 2;
    O(int y = ran()){
        this->y = chkY(y, n);
        for(int i = 0; i < 4; ++i) {
            tab[i].x = x+i/2;
            tab[i].y = this->y+i%2;
        }
    }
};

class S : public Brick{
public:
    int x = 0, y, n = 3;
    S(int y = ran()){
        this->y = chkY(y, n);
        for(int i = 0; i < 4; ++i) {
            tab[i].x = x + ((4-i)/3);
            tab[i].y = this->y + ((i*2+1)/3);
        }
    }
};

class Z : public Brick{
public:
    int x = 0, y, n = 3;
    Z(int y = ran()){
        this->y = chkY(y, n);
        for(int i = 0; i < 4; ++i) {
            tab[i].x = x + ((1+i)/3);
            tab[i].y = this->y + ((i*2+1)/3);
        }
    }
};

class T : public Brick{
public:
    int x = 0, y, n = 3;
    T(int y = ran()){
        this->y = chkY(y, n);
        for(int i = 0; i < 4; ++i) {
            tab[i].x = x + ((1+i)/3 - (i/3));
            tab[i].y = this->y + ((1+i)/2);
        }
    }
};

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
            if (::tab[i][j] != 1)
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

    void print(){
        mainwin = newwin(42,42,0,0);
        box(mainwin, 0, 0);
        mvwprintw(mainwin, 0, 1, "Tetris");


        for(int i = 0; i < 20; i++)
            for(int j = 0; j < 10; j++)
                if(::tab[i][j]==1) {
                    mvwprintw(mainwin, ::pointx[i], ::pointy[j], "****");
                    mvwprintw(mainwin, ::pointx[i]+1, ::pointy[j], "****");
                }
    }
};

class NextWindow : MainWindow {
public:
    WINDOW * nextwin;

    void print(){
        nextwin = newwin(12, 34, 0, 45);
        box(nextwin, 0, 0);
        mvwprintw(nextwin, 0, getmaxx(nextwin)/2-2, "Next");
        mvwaddch(nextwin, 2, 2, char(::next+48));
    }
};

class ScoreWindow : MainWindow {
public:
    WINDOW * scorewin;

    void print(){
        scorewin = newwin(30,34,12,45);
        box(scorewin, 0, 0);
        mvwprintw(scorewin, 0, getmaxx(scorewin)/2-2, "Score");
        mvwprintw(scorewin, 4, getmaxx(scorewin)/2-+5, "%s", std::to_string(score).data());
    }
};

class DeathWindow : MainWindow {
public:
    WINDOW * deathwin;

    void print(){
        //center?
        deathwin = newwin(7, 15, 15, 36);
        box(deathwin, 0, 0);
        mvwprintw(deathwin, 0, getmaxx(deathwin)/2-2, "DEATH");
        mvwprintw(deathwin, 2, getmaxx(deathwin)/2-2, "SCORE");
        mvwprintw(deathwin, 4, getmaxx(deathwin)/2-2, "%s", std::to_string(score).data());
    }
};

void fillTab(){
    for (auto& i : v) {
        for(auto& j : i.tab){
            if(j.x>=0)
            ::tab[j.x][j.y] = 1;
        }
    }

}

void eraseTab(){
    for (auto & i : ::tab)
        for (int & j : i)
            j = 0;
}

void fall(){
    Brick & b = v.back();

    Square smax = b.tab[0];

    for(auto &i : b.tab) {
        if(i.x>smax.x) smax = i;
    }

    if(smax.x == 19) {
        addBrick();
        return;
    }

    for(auto i : b.tab){
        if(tab[i.x+1][i.y]==1){
            bool stop = true;
            for(auto j : b.tab){
                if(i.x+1 == j.x && i.y == j.y)
                    stop = false;
            }
            if(stop){
                addBrick();
                return;
            }
        }
    }

    for(auto &i : b.tab) {
        i.x++;
    }

}

void rotate(){
    Brick b = v.back();


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
            if(tab[i.x][i.y+n] == 1){
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

    noecho();
    //zmiana rozmiaru terminala
    std::cout<<"\e[8;45;80t";

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
                mvwaddch(scorewin.scorewin, i+1, j+1, char(tab[i][j]+48));
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