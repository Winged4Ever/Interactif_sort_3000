/*
 * File:   header.h
 * Author: Tomasz Liszkowski
 */

#ifndef MYHEAD_H_
#define MYHEAD_H_

#define HEIGHT      25
#define WIDTH       80
#define WIDTH_WIN   78
#define HEIGHT_INT  9
#define HEIGHT_SORT 12
#define EXIT        2
#define DRAWSPEED   20
#define UP          1
#define DOWN        2

void f_draw(char*);
int stringLength(char*);
void closeProg();
void runInteractif();
void updateInteractif();
int getMove(int);
void loadMap(char*);
void interpretBoard();
void performAction(int);
void switchMap(char*);
int teleport(int);
int checkAction(int);
void initializeSort();
void silenceOn();
void silenceOff();
void clearLine(WINDOW*, int);
void printCenter(WINDOW*, int, char*, int);
void ejectAnimation(WINDOW*, int, char[][], int);
void moveAndPrint(WINDOW*, int, int, char[][], int);
void wipeAnimation(WINDOW*, int);
void reprintLine(WINDOW*, int);
void bgchange(WINDOW*, int, int, int);
void update(char*);
void printFrom(WINDOW*, int, int, char*);

#define draw(a)                 f_draw(#a)
#define update(a)               f_update(#a)

#define XMIN                    getbegx(whatWindow)
#define XMAX                    getmaxx(whatWindow)
#define YMIN                    getbegy(whatWindow)
#define YMAX                    getmaxy(whatWindow)

typedef struct
{
    int row;
    int column;
} point;

typedef struct
{
    int height;
    int width;
    int y;
    int x;
} vWINDOW_t;

extern char board[HEIGHT_INT][WIDTH_WIN+1];
extern char mapName[20];
extern int attributeMap[HEIGHT_INT][WIDTH_WIN];
extern point charPos;
extern point nextPos;
extern WINDOW *interactif_win;
extern WINDOW *sort_win;

/*Check what's inside numerical (D) or string (S) variable*/
#define DC(Var)     (mvprintw(23, 4, "vCheck: " #Var " = %d", Var));getch();clearLine(stdscr, 23);wrefresh(stdscr);
#define SC(Var)     (mvprintw(23, 4, "vCheck: " #Var " = %s", Var));getch();clearLine(stdscr, 23);wrefresh(stdscr);

/*Extended ASCII characters*/
#define DLUC        201
#define DRUC        187
#define DLLC        200
#define DRLC        188
#define DVERL       185
#define DVERR       204
#define DVER        186
#define DHOR        205
#define LUC         218
#define RUC         191
#define LLC         192
#define RLC         217
#define VERL        180
#define VERR        195
#define VER         179
#define HOR         196

#endif
