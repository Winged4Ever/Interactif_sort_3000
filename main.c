/*
 * File:   main.c
 * Author: Tomasz 'Winged' Liszkowski
 */

#include <curses.h>
#include <windows.h>
#include <assert.h>
#include <string.h>
#include "header.h"

WINDOW *logo_win;

int main()
{
    initscr();
	wclear(stdscr);
    keypad(stdscr, TRUE);
    start_color();
    raw();

    silenceOn();
    draw(border);
    draw(logo);
    draw(line);
    runInteractif();

    endwin();
    return 0;
}
/*End of main*/

/*Defined as draw !*/
void f_draw(char* whatToDraw)
{
    int x = 0, y = 0;
    if (strcmp (whatToDraw, "border") == 0)
    {
        /*Create info panel*/
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        attron(COLOR_PAIR(1));
        for (x = 0; x <= WIDTH; x++)
            mvprintw(HEIGHT-1, x, " ");
        wrefresh(stdscr);
        attroff(COLOR_PAIR(1));

        /*Draw main border*/
        mvwprintw (stdscr, 0, 0, "%c", DLUC);
        mvwprintw (stdscr, HEIGHT - 2, WIDTH - 1, "%c", DRLC);
        wrefresh (stdscr);
        Sleep (DRAWSPEED);
        for (y = 1; y <= HEIGHT - 3; y++)
        {
            mvwprintw (stdscr, y, 0, "%c", DVER);
            mvwprintw (stdscr, HEIGHT - 2 - y, WIDTH - 1, "%c", DVER);
            wrefresh (stdscr);
            Sleep (DRAWSPEED);
        }
        mvwprintw (stdscr, HEIGHT - 2, 0, "%c", DLLC);
        mvwprintw (stdscr, 0, WIDTH - 1, "%c", DRUC);

        wrefresh (stdscr);
        Sleep (DRAWSPEED);
        for (x = 1; x <= WIDTH - 2; x++)
        {
            mvwprintw (stdscr, HEIGHT - 2, x, "%c", DHOR);
            mvwprintw (stdscr, 0, WIDTH - 1 - x, "%c", DHOR);
            wrefresh(stdscr);
            Sleep (DRAWSPEED);
        }
    }
    else if (strcmp (whatToDraw, "logo") == 0)
    {
        logo_win = newwin(HEIGHT-3, WIDTH-2, 1, 1);
        char logoArray[HEIGHT][WIDTH + 1];

        FILE *pFile = fopen ("logo.dat" , "r");
        /*If file cannot be opened, show an error and then crash*/
        if (pFile == NULL)
        {
            perror ("Error opening file");
            assert(!TRUE);
        }
        /*If successfully open the file*/
        else
        {
            /*+1 cause of '\n' char*/
            char tempArray[WIDTH + 3];
            int x = 0, y = 0;
            while(fgets (tempArray, sizeof(tempArray) ,pFile) != !EOF)
            {
                for (x = 0; x <= WIDTH; x++)
                {
                    logoArray[y][x] = tempArray[x];
                }
                y++;
            }
        }
        fclose (pFile);

        ejectAnimation(logo_win, DOWN, logoArray, 0);
        printCenter(logo_win, 17, "Press Enter to begin! ", 1);
        silenceOff();
        flushinp();
        getch();
        silenceOn();
        wipeAnimation(logo_win, DOWN);
        delwin(logo_win);
    }
    else if (strcmp (whatToDraw, "line") == 0)
    {
        mvwprintw (stdscr, 10, 0, "%c", DVERR);
        mvwprintw (stdscr, 10, WIDTH - 1, "%c", DVERL);
        wrefresh (stdscr);
        Sleep (DRAWSPEED);
        for (x = 1; x <= WIDTH/2; x++)
        {
            mvwprintw (stdscr, 10, x, "%c", DHOR);
            mvwprintw (stdscr, 10, WIDTH - x - 1, "%c", DHOR);
            wrefresh (stdscr);
            Sleep (DRAWSPEED);
        }
    }
    else if (strcmp (whatToDraw, "board") == 0)
    {
        for (y = 0; y <= HEIGHT_INT; y++)
        {
            for (x = 0; x <= WIDTH_WIN; x++)
            {
                mvwprintw (interactif_win, y, x, "%c", visibleMap[y][x]);
            }
        }
        /*Draw beginning character position*/
        mvwprintw(interactif_win, charPos.row, charPos.column, "X");
        wrefresh(interactif_win);
    }
    else if (strcmp (whatToDraw, "table") == 0)
    {
        int x, y;
        WINDOW* whatWindow = sort_win;
        init_pair(5, COLOR_WHITE, COLOR_BLUE);

        /*Print separators*/
        for (y = 0; y <= 1; y++)
        {
            mvwprintw (whatWindow, y, 4, "%c", VER) ;
            mvwprintw (whatWindow, y, 45, "%c", VER);
            mvwprintw (whatWindow, y, 52, "%c", VER);
            mvwprintw (whatWindow, y, 61, "%c", VER);
            wrefresh(whatWindow);
            Sleep (DRAWSPEED);
        }

        /*Print lower line*/
        for (x = 0; x < XMAX; x++)
        {
            mvwprintw (whatWindow, 1 ,x ,"%c", HOR);
        }
        mvwprintw (whatWindow, 1, 4, "%c", CROSS);
        mvwprintw (whatWindow, 1, 45, "%c", CROSS);
        mvwprintw (whatWindow, 1, 52, "%c", CROSS);
        mvwprintw (whatWindow, 1, 61, "%c", CROSS);
        wrefresh(whatWindow);
        Sleep (DRAWSPEED);

        /*Print data body*/
        for (y = 2; y < YMAX; y++)
        {
            mvwprintw (whatWindow, y, 4, "%c", VER);
            mvwprintw (whatWindow, y, 45, "%c", VER);
            mvwprintw (whatWindow, y, 52, "%c", VER);
            mvwprintw (whatWindow, y, 61, "%c", VER);
            wrefresh(whatWindow);
            Sleep (DRAWSPEED);
        }
    }
}
/*End of f_draw*/

/*Same as mvprintw(); but with animation*/
void printFrom(WINDOW* whatWindow, int row, int column, char* text)
{
	int i = 0, length = 0;
	int curY, curX;
    getyx(curscr, curY, curX);

	length = stringLength(text);

	/*Let's first clear what has been written here*/
	for (i = 0; i <= WIDTH-1; i++)
	{
		mvwprintw(whatWindow, row, 0, " ");
	}

	/*And now write what we wan't to write there*/
	for (i = 0; i <= length; i++)
	{
		mvwprintw(whatWindow, row, column + i, "%c", text[i]);
		wrefresh(whatWindow);
		Sleep (DRAWSPEED/2);
	}
	/*Move the cursor back to the place where it was*/
	move (curY, curX);
}
/*End of printFrom*/

/*Wipe animation of a window*/
void wipeAnimation(WINDOW* whatWindow, int direction)
{
	int y, x;

	if (direction == DOWN)
	{
		/*Do it till whole area will be covered with lines with white chars*/
		for (y = YMIN; y <= YMAX; y++)
		{
			for (x = XMIN; x <= XMAX; x++)
            {
                mvwprintw(whatWindow, y, x, " ");
            }
			/*Reload one animation step*/
			wrefresh(whatWindow);
			/*Wait some time before printing next animation's step*/
			Sleep (DRAWSPEED);
		}
	}
	else if (direction == UP)
	{
		/*Do it till whole area will be covered with lines with white chars*/
		for (y = YMAX; y >= YMIN; y--)
		{
			for (x = XMIN; x <= XMAX; x++)
            {
                mvwprintw(whatWindow, y, x, " ");
            }
			/*Reload one animation step*/
			wrefresh(whatWindow);
			/*Wait some time before printing next animation's step*/
			Sleep (DRAWSPEED);
		}
	}
	/*If provided an unknown direction*/
	else
	{
		assert(!TRUE);
	}
}
/*End of wipeAnimation*/

/*Slowly change background animation of chosen window*/
void bgchange(WINDOW* whatWindow, int colorNow, int colorAfter, int uniqueID)
{
    int y, x;
    init_pair(uniqueID, colorAfter, colorNow);
    wattron(whatWindow, COLOR_PAIR(uniqueID));
    for (y = 0; y <= YMAX; y++)
    {
        for (x = 0; x <= XMAX; x++)
        {
            mvwprintw(whatWindow, y, x, ".");
        }
    }
    wrefresh(whatWindow);
    Sleep (DRAWSPEED*3);
    for (y = 0; y <= YMAX; y++)
    {
        for (x = 0; x <= XMAX; x++)
        {
            mvwprintw(whatWindow, y, x, "%c", 176);
        }
    }
    wrefresh(whatWindow);
    Sleep (DRAWSPEED*4);
    for (y = 0; y <= YMAX; y++)
    {
        for (x = 0; x <= XMAX; x++)
        {
            mvwprintw(whatWindow, y, x, "%c", 177);
        }
    }
    wrefresh(whatWindow);
    Sleep (DRAWSPEED*4);
    for (y = 0; y <= YMAX; y++)
    {
        for (x = 0; x <= XMAX; x++)
        {
            mvwprintw(whatWindow, y, x, "%c", 178);
        }
    }
    wrefresh(whatWindow);
    Sleep (DRAWSPEED*4);
    for (y = 0; y <= YMAX; y++)
    {
        for (x = 0; x <= XMAX; x++)
        {
            mvwprintw(whatWindow, y, x, " ");
        }
    }
    wattroff(whatWindow, COLOR_PAIR(uniqueID));
    init_pair(uniqueID, colorNow, colorAfter);
    wbkgd(whatWindow, COLOR_PAIR(uniqueID));
    wrefresh(whatWindow);
}
/*End of bgchange*/

/*Ejecting animation from chosen line, pointed upward or downward*/
void ejectAnimation(WINDOW* whatWindow, int direction, char whatArray[HEIGHT][WIDTH + 1], int isRelative)
{
	int z = 0, j = 0, lineCounter = 0, linesLeft, from;

	if (direction == DOWN)
	{
	    if (isRelative == 1)
        {
            from = YMIN;
            linesLeft = (YMAX-YMIN);
        }
        else if (isRelative == 0)
        {
            from = 0;
            linesLeft = HEIGHT-1;
        }
        else
            assert (!TRUE);

		/*E.g. if 'from' = 17, the first printing step will print out in 17th
		 row the content of 24th line, and on the next step - 17|23 and 18|24,
		 and on the third step - 17|22, 18|23. 19|24, and so on, till it will
		 reach the last terminal's row*/
		for (lineCounter = 0; lineCounter < linesLeft; lineCounter++)
		{
		    /*Wait some time before printing next animation's step*/
            Sleep (DRAWSPEED);
			for (z = 0; z <= j; z++)
			{
				moveAndPrint(whatWindow, (from + z), (linesLeft - j + z), whatArray, isRelative);
			}
			z = 0;
			j++;
            wrefresh(whatWindow);
		}
	}
	else if (direction == UP)
	{
	    if (isRelative == 1)
        {
            from = YMAX;
            linesLeft = (YMAX-YMIN);
        }
        else if (isRelative == 0)
        {
            from = HEIGHT;
            linesLeft = HEIGHT;
        }
        else
            assert (!TRUE);

		/*E.g. if 'from' = 17, the first printing step will print out in 17th
		 row the content of 1st line, and on the next step - 17|2 and 16|1,
		 and on the third step - 17|3, 16|2. 15|3, and so on, till it will
		 reach the first terminal's row*/
		for (lineCounter = 0; lineCounter < linesLeft; lineCounter++)
		{
			for (z = 0; z <= j; z++)
			{
				moveAndPrint(whatWindow, (from - z), (1 + j - z), whatArray, isRelative);
			}
			z = 0;
			j++;
			wrefresh(whatWindow);
			/*Wait some time before printing next animation's step*/
			Sleep (DRAWSPEED);
		}
	}
	/*If provided an unknown direction*/
	else
	{
		assert(!TRUE);
	}
}
/*End of ejectAnimation*/

/*Prints exactly the same what was in chosen line*/
void reprintLine(WINDOW* whatWindow, int whatLine)
{
    int x, delta = XMAX-XMIN;
    char toPrint[delta+1];

    for (x = 0; x <= delta; x++)
    {
        toPrint[x] = mvwinch(whatWindow, whatLine, x) & A_CHARTEXT;
    }
    for (x = 0; x <= delta; x++)
    {
        mvwprintw(whatWindow, whatLine, x, "%c", toPrint[x]);
    }
    wrefresh(whatWindow);
}
/*End of reprintLine*/

/*This will print chosen line from preloaded array into chosen
 row in the terminal (but it will not refresh the window by itself)*/
void moveAndPrint(WINDOW* whatWindow, int where, int whatLine, char whatArray[HEIGHT][WIDTH + 1], int isRelative)
{
	int x = 0, line;
    if (isRelative == 1)
    {
        line = 0;
    }
    else if (isRelative == 0)
    {
        line = XMIN;
    }
    else
        assert (!TRUE);

	/*Loads to the buffer one chosen row from the array*/
	while (x <= XMAX)
	{
		mvwprintw(whatWindow, where, x, "%c", whatArray[whatLine][line]);
		x++;
		line++;
	}
}
/*End of moveAndPrint*/

/*Syntax: printCenter(20, "Menu", 0);*/
/*if last number is 1, it will appear with animation*/
void printCenter(WINDOW* whatWindow, int line, char* whatToPrint, int wantToAnim)
{
	int i, origin, length;

	length = stringLength(whatToPrint);
	origin = (XMAX-XMIN)/2 - (length/2);
	/*Print it on the center of chosen line*/
	if (wantToAnim == 1)
    {
        for (i = 0; i <= length; i++)
        {
            mvwprintw (whatWindow, line, origin + i, "%c", whatToPrint[i]);
            wrefresh (whatWindow);
            Sleep (DRAWSPEED);
        }
    }
    else
    {
        for (i = 0; i <= length; i++)
        {
            mvwprintw (whatWindow, line, origin + i, "%c", whatToPrint[i]);
        }
        wrefresh(whatWindow);
    }
    move(line+1, origin+i);
}
/*End of printCenter*/

/*Clears line by printing white chars*/
void clearLine(WINDOW* whatWindow, int whichLine)
{
	int i = 0;

	wmove(whatWindow, whichLine, XMIN);
	for (i = 0; i <= XMAX-XMIN; i++)
	{
		wprintw(whatWindow, " ");
	}
}
/*End of clearLine*/

/*Measures provided string's length*/
int stringLength(char* text)
{
	int length = 0;
	while(text[length] != '\0')
	{
		length++;
	}
	return length;
}
/*End of stringLength*/

/*Turns off echo and cursor visibility*/
void silenceOn()
{
	noecho();
	curs_set(0);
}
/*End of silenceOn*/

/*Turns on echo and cursor visibility*/
void silenceOff()
{
	echo();
	curs_set(1);
}
/*End of silenceOff*/

/*Entered text will slide apart in two directions from the center of a window*/
/*Specifying variableType is needed in C language, unfortunately*/
int writeCenter(WINDOW* whatWindow, int line, char* saveToThis)
{
    if (line > YMAX)
    {
        perror("'line' out of window boundaries");
        DC(line);
        assert(!TRUE);
    }

    silenceOn();

    char input, tempArray[WIDTH_WIN];
    memset(tempArray, '\0', sizeof(tempArray));
    int i = 0;

    while (i < WIDTH_WIN)
    {
        flushinp();
        input = getch();
        if (input == ESC)
        {
            /*Clear variable*/
            memset (tempArray, '\0', sizeof(tempArray));
            wipeAnimation(whatWindow, DOWN);
            printCenter(whatWindow, 4, "Passing value has been aborted", 1);
            waitingArrows(whatWindow, 7);
            wipeAnimation(whatWindow, DOWN);
            return FALSE;
        }
        else if (input == ENTER)
        {
            break;
        }
        /*If BCKSPC has been pressed*/
        else if (input == BSP)
        {
            /*Check if not reached zero-pos char yet*/
            if (i > 0)
            {
                /*Delete last star and back one step*/
                i--;
                tempArray[i] = '\0';
                clearLine(whatWindow, line);
                mvwprintw(whatWindow, line, XMAX/2 - (i/2), tempArray);
            }
        }
        else
        {
            tempArray[i] = input;
            i++;
            clearLine(whatWindow, line);
            mvwprintw(whatWindow, line, XMAX/2 - (i/2), tempArray);
        }
        wrefresh(whatWindow);
    }
    strcpy (saveToThis, tempArray);

    return TRUE;
}
/*End of writeCenter*/

/*This will display arrows animation while waiting for input*/
void waitingArrows(WINDOW* whatWindow, int line)
{
    silenceOn();
    int t = 0;
    flushinp();
    /*Don't wait on getch()*/
    nodelay(stdscr, TRUE);
    while (1 == 1)
    {
        if (getch() == 13)
            break;
        Sleep(DRAWSPEED*3);
        if (t > 7)
            t = 0;
        switch(t)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            {
                mvwprintw(whatWindow, line, (XMAX/2 + 10) + t * 2, ">");
                wrefresh(whatWindow);
                break;
            }
            case 4:
            case 5:
            case 6:
            case 7:
            {
                mvwprintw(whatWindow, line, (XMAX/2 + 10) + (t - 4) * 2, " ");
                wrefresh(whatWindow);
            }
        }
        t++;
    }
    nodelay(stdscr, FALSE);
    silenceOff();
    clearLine(whatWindow, line);
	wrefresh(whatWindow);
}
/*End of waitingArrows*/

/*In order to get rid of EXIT path from deepest functions to the main*/
void closeProg()
{
    wipeAnimation(stdscr, DOWN);
    printCenter(stdscr, HEIGHT/2, "Thanks for playing! Have a nice day!", 1);
    flushinp();
    getch();
    exit (0);
}
/*End of closeProg*/
