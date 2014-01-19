/*
 * File:   sort.c
 * Author: Tomasz Liszkowski
 */

#include <curses.h>
#include <windows.h>
#include <assert.h>
#include <string.h>
#include "header.h"

WINDOW *sort_win;

/*Initialize the sort window*/
void initializeSort()
{
    sort_win = newwin(HEIGHT_SORT, WIDTH_WIN, 11, 1);
    bgchange(sort_win, COLOR_BLACK, COLOR_BLUE, 98);
}
/*End of initializeSort*/
