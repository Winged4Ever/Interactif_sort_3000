/*
 * File:   header.h
 * Author: Tomasz 'Winged' Liszkowski
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
#define LEFT        3
#define RIGHT       4
#define DATABSIZE   50
#define ESC         27
#define ENTER       13
#define BSP         8

typedef struct
{
    char column1_title[20];
    int column1[DATABSIZE];
    char column2_title[20];
    char column2[DATABSIZE][80];
    char column3_title[20];
    int column3[DATABSIZE];
    char column4_title[20];
    float column4[DATABSIZE];
    char column5_title[20];
    float column5[DATABSIZE];
} database_t;

typedef struct
{
    int row;
    int column;
} point;

void f_draw(char*);
int stringLength(char*);
void closeProg();
void runInteractif();
void updateInteractif();
int getMove(int);
void loadMap(char*);
void interpretBoard();
void pAAction(int);
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
void loadDatabase(database_t *);
void initializeData();
void f_printData(int, int, int, char*);
void scrollData(int);
void activateDatabase(database_t);
int filterData(int, char*, int, char*);
void sortData(int, int);
void copyData(int);
void cAAppearance(int, int);
int cMAttribute(int, int, int, int);
int cMAppearance(int, int, int, char);
int useDoor(int, int);
int openDoor(int, int);
int closeDoor(int, int);
int returnAttribute(int);
char returnVisible(int);
void pMAction(int);
void waitingArrows(WINDOW*, int);
int writeCenter(WINDOW*, int, char*);
void f_passValue(char*, int*);
void loadGlobalActions(char*);
int f_returnACharPos(int, char);
void mapConditions();
void cAAttribute(int, int);

#define draw(a)                 f_draw(#a)
#define update(a)               f_update(#a)
#define printData(a,b,c,d)      f_printData(a,b,c,#d)
#define returnACharPos(a,b)     f_returnACharPos(a,#b)
#define passValue(a,b)          f_passValue(#a,b)

#define XMIN                    getbegx(whatWindow)
#define XMAX                    getmaxx(whatWindow)
#define YMIN                    getbegy(whatWindow)
#define YMAX                    getmaxy(whatWindow)

extern char visibleMap[HEIGHT_INT][WIDTH_WIN+1], board[HEIGHT_INT][WIDTH_WIN+1], mapName[20];
extern int attributeMap[HEIGHT_INT][WIDTH_WIN];
extern point charPos, nextPos;
extern WINDOW *interactif_win, *sort_win;
extern database_t movies_database, current_database, filtered_database;
extern int firstVisRow, lastRow;
extern int action1, action2;
extern char v_column1[70], v_column3[70], v_column2[70], v_column4[70], v_column5[70];
extern int v_order;

/*Check what's inside numerical (D), string (S) variable and e.t.c.*/
#define DC(Var)     (mvprintw(23, 4, "vCheck: " #Var " = %d", Var));getch();clearLine(stdscr, 23);wrefresh(stdscr);
#define FC(Var)     (mvprintw(23, 4, "vCheck: " #Var " = %f", Var));getch();clearLine(stdscr, 23);wrefresh(stdscr);
#define SC(Var)     (mvprintw(23, 4, "vCheck: " #Var " = %s", Var));getch();clearLine(stdscr, 23);wrefresh(stdscr);
#define XC(Var)     (mvprintw(23, 4, "vCheck: " #Var " = %x", Var));getch();clearLine(stdscr, 23);wrefresh(stdscr);

/*Extended ASCII characters*/
#define DLUC        201
#define DRUC        187
#define DLLC        200
#define DRLC        188
#define DVERL       185
#define DVERR       204
#define DCROSS      206
#define DVER        186
#define DHOR        205
#define LUC         218
#define RUC         191
#define LLC         192
#define RLC         217
#define VERL        180
#define VERR        195
#define CROSS       197
#define VER         179
#define HOR         196

/*Define uppercase letters for broader action scripting*/
#define A           17
#define B           18
#define C           19
#define D           20
#define E           21
#define F           22
#define G           23
#define H           24
#define I           25
#define J           26
#define K           27
#define L           28
#define M           29
#define N           30
#define O           31
#define P           32
#define Q           33
#define R           34
#define S           35
#define T           36
#define U           37
#define V           38
#define W           39
#define X           40
#define Y           41
#define Z           42

#endif
