//
// Created by doomsram on 20.12.23.
//

#ifndef TETRISV4_HEADERS_H
#define TETRISV4_HEADERS_H

extern int tab[20][10][2];
extern int pointx[20];
extern int pointy[10];
extern void addBrick();
extern void death();

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

void initPoint(){
    for(int i = 0; i < 20; i++){
        ::pointx[i] = (2*i)+1;
    }
    for(int j = 0; j < 10; j++){
        ::pointy[j] = (4*j)+1;
    }
}

void initColors(){
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(5, COLOR_RED, COLOR_RED);
    init_pair(6, COLOR_WHITE, COLOR_WHITE);
    init_pair(7, COLOR_RED, COLOR_BLACK);
}

class Square{
public:
    int x, y;

    Square(int x = -1, int y = -1){
        this->x = x;
        this->y = y;
    }

    void deleteSquare(){
        x = -1;
        y = -1;
    }

};

class Brick{
private:
    void setColor(){
        color = 1+(rand()%5);
    }

protected:
    int color;

    Square tab[4];

public:

    int blockType;
    int centerSquare;

    bool chkCol(){
        bool res = false;
        for(auto i : tab){
            if(::tab[i.x][i.y][0]==1)
                res = true;
        }
        return res;
    }

    Brick(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4, int type = 0, int center = 0){

        int n = 0;
        do {
            int y = Brick::chkY(ran(), 4);
            tab[0] = Square(0 + x1, y + y1);
            tab[1] = Square(0 + x2, y + y2);
            tab[2] = Square(0 + x3, y + y3);
            tab[3] = Square(0 + x4, y + y4);
            n++;
            if(n>=50)
                death();
        }while(chkCol());

        setColor();
        this->blockType = type;
        this->centerSquare = center;
    }

    static int ran(){
        return 1+(rand()%10);
    }

    static int chkY(int y, int n){
        return y-n<0? y: y-n;
    }

    void fillTab(){
            for(int i = 0; i < 4; i++){
                if(tab[i].x>=0)
                    ::tab[tab[i].x][tab[i].y][0] = 1;
                ::tab[tab[i].x][tab[i].y][1] = color;
            }

    }

    void fall(){
        Square smax = tab[0];

        for(auto & i : tab) {
            if(i.x>smax.x) smax = i;
        }

        if(smax.x == 19) {
            addBrick();
            return;
        }

        for(auto & i : tab){
            if(::tab[i.x+1][i.y][0]==1){
                bool stop = true;
                for(auto & j : tab){
                    if(i.x+1 == j.x && i.y == j.y)
                        stop = false;
                }
                if(stop){
                    addBrick();
                    return;
                }
            }
        }

        for(auto & i : tab) {
            i.x++;
        }

    }

    void movedown(int p) {
        for (auto & i : tab) {
            if(i.x < p && i.x > 0)
                i.x += 1;
        }
    }

    void deleteRow(int p){
        for(auto & i : tab){
            if(i.x == p)
                i.deleteSquare();
        }
    }


    void sideMove(int n){
        Square ymin = tab[0];
        for(auto & i : tab){
            if(i.y<ymin.y)
                ymin = i;
        }

        Square ymax = tab[0];
        for(auto & i : tab){
            if(i.y>ymax.y)
                ymax = i;
        }


        if(ymin.y+n >= 0 && ymax.y+n <= 9) {

            for(auto & i : tab){
                if(::tab[i.x][i.y+n][0] == 1){
                    bool sk = true;
                    for(auto & j : tab){
                        if(i.x == j.x && i.y+n == j.y)
                            sk = false;
                    }
                    if(sk)
                        return;
                }
            }

            for(auto & i : tab){
                i.y+=n;
            }
        }
    }

    void chkfail(){
        Square smin = tab[0];

        for(auto & i : tab){
            if(i.x<smin.x && i.x>=0) smin = i;
        }

        if(smin.x == 0){
            death();
        }
    }

    void rotateBig(){
        if(tab[0].x - tab[1].x == 0){
            tab[0].x = tab[0].x-2;
            tab[0].y = tab[0].y+2;
            tab[1].x = tab[1].x-1;
            tab[1].y = tab[1].y+1;
            tab[3].x = tab[3].x+1;
            tab[3].y = tab[3].y-1;
        }else{
            tab[0].x = tab[0].x+2;
            tab[0].y = tab[0].y-2;
            tab[1].x = tab[1].x+1;
            tab[1].y = tab[1].y-1;
            tab[3].x = tab[3].x-1;
            tab[3].y = tab[3].y+1;
        }
    }

    bool chkSelf(int x, int y){
        bool res = false;
        for(auto i : tab){
            if(i.x == x && i.y == y)
                res = true;
        }
        return !res;
    }

    //border chk - poza zakresem wraca na 0 ??????/AWD;asefokdrgsok
    static bool chkBord(int y){
        return (y >= 0 && y < 10);
    }


    void rotate(){
        if(blockType == 2)
            return;
        else if(blockType == 3) {
            rotateBig();
            return;
        }
        int tab1[3][3] = {0};
        int tab2[3][3] = {0};

        int xOff = tab[centerSquare].x-1;
        int yOff = tab[centerSquare].y-1;

        for(auto s : tab) {
            tab1[s.x-xOff][s.y-yOff] = 1;
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                    tab2[i][j] = tab1[2 - j][i];
            }
        }

        //-----------------------------------------------------athsjtkgsoritgmboi
        bool collision = false;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(::tab[i+xOff][j+yOff][0]==1 && chkSelf(i+xOff, j+yOff))
                    collision = true;
            }
        }
        if(collision)
            return;
        //////////////////-----------------return do ifa

        int s = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if(tab2[i][j]==1){
                    tab[s].x = i+xOff;
                    tab[s].y = j+yOff;
                    if(i==1 && j==1)
                        centerSquare = s;
                    s++;
                }
            }
        }
    }

};

extern std::vector<Brick> v;


void fall(){
    v.back().fall();
}

void fillTab(){
    for(auto i : v){
        i.fillTab();
    }
}

void movedown(int p) {
    for (auto &b: v) {
        b.movedown(p);
    }
}

void deleteRow(int p){
    for(auto &b : v){
        b.deleteRow(p);
    }
}

void sideMove(int n, Brick& b){
    b.sideMove(n);
}

void chkfail(){
    v.back().chkfail();
}

void rotate(){
    v.back().rotate();
}

class I : public Brick {
public:

    explicit I() :Brick(0, 0, 0, 0, 0, 1, 2, 3, 3){

    }
};

class L : public Brick{
public:

    explicit L() :Brick(0, 0, 1, 2, 0, 1, 1, 1, 1, 2){

    }
};

class J : public Brick{
public:
    explicit J() :Brick(0, 0, 1, 2, 0, 1, 0, 0, 1, 2){

    }
};

class O : public Brick{
public:
    explicit O() :Brick(0, 0, 1, 1, 0, 1, 0, 1, 2){

    }
};

class S : public Brick{
public:
    explicit S() :Brick(0, 0, 1, 1, 2, 1, 1, 0, 1, 1){

    }
};

class Z : public Brick{
public:
    explicit Z() :Brick(0, 0, 1, 1, 0, 1, 1, 2, 1, 1){

    }
};

class T : public Brick{
public:
    explicit T() :Brick(0, 1, 1, 1, 1, 0, 1, 2, 1, 2){

    }
};


#endif //TETRISV4_HEADERS_H
