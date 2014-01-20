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
#define DATABSIZE   99

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
int performAction(int);
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
int f_filterData(int, char*, int, char*);
void sortData(int, int);
void copyData(int);

#define draw(a)                 f_draw(#a)
#define update(a)               f_update(#a)
#define printData(a,b,c,d)      f_printData(a,b,c,#d)
#define filterData(a,b,c,d)     f_filterData(a,#b,c,#d)

#define XMIN                    getbegx(whatWindow)
#define XMAX                    getmaxx(whatWindow)
#define YMIN                    getbegy(whatWindow)
#define YMAX                    getmaxy(whatWindow)

extern char board[HEIGHT_INT][WIDTH_WIN+1];
extern char mapName[20];
extern int attributeMap[HEIGHT_INT][WIDTH_WIN];
extern point charPos;
extern point nextPos;
extern WINDOW *interactif_win;
extern WINDOW *sort_win;
extern database_t movies_database;
extern database_t current_database;
extern database_t filtered_database;
extern int firstVisRow, lastRow;

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

#endif
