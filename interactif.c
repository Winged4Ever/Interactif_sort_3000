/*
 * File:   interactif.c
 * Author: Tomasz 'Winged' Liszkowski
 */

#include <curses.h>
#include <windows.h>
#include <assert.h>
#include <string.h>
#include "header.h"

point nextPos;
point charPos;
WINDOW *interactif_win;
char board[HEIGHT_INT][WIDTH_WIN+1];
int attributeMap[HEIGHT_INT][WIDTH_WIN];
char visibleMap[HEIGHT_INT][WIDTH_WIN+1];
char mapName[20];
int action1, action2;
char v_column1[70], v_column3[70], v_column2[70], v_column4[70], v_column5[70];

/*Main function of interactif window*/
void runInteractif()
{
    interactif_win = newwin(HEIGHT_INT, WIDTH_WIN, 1, 1);
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    bgchange(interactif_win, COLOR_BLACK, COLOR_WHITE, 99);

    /*Load board from array from the file or from somewhere else*/
    loadMap("menu");

    /*Print bottom info*/
    wattron(stdscr, COLOR_PAIR(1));
    printFrom(stdscr, HEIGHT-1, 1, "Use");
    wattroff(stdscr, COLOR_PAIR(1));
    init_pair(4, COLOR_RED, COLOR_WHITE);
    wattron(stdscr, COLOR_PAIR(4));
    printFrom(stdscr, HEIGHT-1, 5, "ARROW KEYS");
    wattroff(stdscr, COLOR_PAIR(4));
    wattron(stdscr, COLOR_PAIR(1));
    printFrom(stdscr, HEIGHT-1, 16, "to move your character. Use");
    wattroff(stdscr, COLOR_PAIR(1));
    wattron(stdscr, COLOR_PAIR(4));
    printFrom(stdscr, HEIGHT-1, 44, "ENTER");
    wattroff(stdscr, COLOR_PAIR(4));
    wattron(stdscr, COLOR_PAIR(1));
    printFrom(stdscr, HEIGHT-1, 50, "to 'use' objects");
    wattroff(stdscr, COLOR_PAIR(1));

    flushinp();
    int input;
    while (1 == 1)
    {
        /*Receive input*/
        flushinp();
        input = getch();
        getMove(input);
    }
}
/*End of runInteractif*/

/*Will update things like position, and other's agents positions (if invented)*/
void f_update(char* what)
{
    if (strcmp (what, "pos") == 0)
    {
        mvwprintw(interactif_win, charPos.row, charPos.column, "%c", visibleMap[charPos.row][charPos.column]);
        mvwprintw(interactif_win, nextPos.row, nextPos.column, "X");
    }
    wrefresh(interactif_win);
}
/*End of f_update*/

/*This function will check if move from the input is possible and then if
 *it is, it will move the character and call the area function if needed*/
int getMove(int input)
{
    if (input == KEY_UP)
    {
        nextPos.row--;
        switch (checkAction(attributeMap[nextPos.row][nextPos.column]))
        {
        /*If is possible and not exit*/
        case TRUE:
            update(pos);
            charPos.row--;
            break;
        /*If is not possible and not exit*/
        case FALSE:
            nextPos.row++;
            break;
        default:
            break;
        }
    }
    else if (input == KEY_DOWN)
    {
        nextPos.row++;
        switch (checkAction(attributeMap[nextPos.row][nextPos.column]))
        {
        /*If is possible and not exit*/
        case TRUE:
            update(pos);
            charPos.row++;
            break;
        /*If is not possible and not exit*/
        case FALSE:
            nextPos.row--;
            break;
        default:
            break;
        }
    }
    else if (input == KEY_LEFT)
    {
        nextPos.column--;
        switch (checkAction(attributeMap[nextPos.row][nextPos.column]))
        {
        /*If is possible and not exit*/
        case TRUE:
            update(pos);
            charPos.column--;
            break;
        /*If is not possible and not exit*/
        case FALSE:
            nextPos.column++;
            break;
        default:
            break;
        }
    }
    else if (input == KEY_RIGHT)
    {
        nextPos.column++;
        switch (checkAction(attributeMap[nextPos.row][nextPos.column]))
        {
        /*If is possible and not exit*/
        case TRUE:
            update(pos);
            charPos.column++;
            break;
        /*If is not possible and not exit*/
        case FALSE:
            nextPos.column--;
            break;
        default:
            break;
        }
    }
    else if (input == ENTER)
    {
        pMAction(board[charPos.row][charPos.column] - 48);
    }
    return 0;
}
/*End of getMove*/

/*Search for the map in the file, then interpret it*/
void loadMap(char* whatMap)
{
    /*mapName for the future*/
    strcpy (mapName, whatMap);

	FILE *pFile = fopen ("mapsDatabase.dat" , "r");
	/*If file cannot be opened, show an error and then crash*/
	if (pFile == NULL)
	{
		perror ("Error opening file");
	}
	/*If successfully open the file*/
	else
	{
        char tempArray[WIDTH_WIN + 1];
        int length = stringLength(whatMap) + 1;

        /*Find this map first*/
        while(fgets (tempArray, length ,pFile) != !EOF)
        {
            if (strcmp (whatMap, tempArray) == 0)
                break;
        }
        /*Skip unchecked white chars*/
        fgets (tempArray, sizeof(WIDTH_WIN + 1) ,pFile);

        /*Then copy it's content to the array*/
        int linesLeft, column, row = 0;
        for (linesLeft = 0; linesLeft <= 8; linesLeft++)
        {
            fgets (tempArray, WIDTH_WIN + 2, pFile);
            for (column = 0; column <= 80; column++)
            {
                /*'board' will be untouched, in order to reference to the original map in some procedures*/
                board[row][column] = tempArray[column];
                /*'visibleMap' may be overwritten, modified, and all*/
                visibleMap[row][column] = tempArray[column];
            }
            row++;
        }
    }
	/*When reached end of file and not found anything*/
	fclose (pFile);

    loadGlobalActions(whatMap);
    interpretBoard();
    draw(board);
}
/*End of loadMap*/

/*Check what characters are on the board, some of them will act as walls,
 *some as a spawn place and e.t.c.*/
void interpretBoard()
{
    int x, y;

    /*Check every position and assign an attribute which will be checked in
     *real-time when calculating the move*/
    for (y = 0; y <= HEIGHT_INT-1; y++)
    {
        for (x = 0; x <= WIDTH_WIN-1; x++)
        {
            /*Floor*/
            if (board[y][x] == ' ')
            {
                attributeMap[y][x] = 0;
            }
            /*Wall*/
            else if (board[y][x] == '#')
            {
                attributeMap[y][x] = 1;
            }
            /*Another wall*/
            else if (board[y][x] == '$')
            {
                attributeMap[y][x] = 1;
            }
            /*If is DIGIT or UPPERCASE letter (from ASCII table)*/
            else if ((board[y][x] >= 48 && board[y][x] <= 57) || (board[y][x] >= 65 && board[y][x] <= 90))
            {
                cAAppearance(y, x);
                cAAttribute(y, x);
            }
            /*If is LOWERCASE letter (from ASCII table)*/
            else if ((board[y][x] >= 97 && board[y][x] <= 122))
            {
                attributeMap[y][x] = 0;
            }
            /*Spawn point*/
            else if (board[y][x] == '@')
            {
                attributeMap[y][x] = 0;
                /*I can assign those coordinates this way because of when the
                 *map change is triggered, the next thing before drawing it, is
                 *calling this function which will set a spawn position*/
                charPos.row = y;
                charPos.column = x;
                nextPos.row = charPos.row;
                nextPos.column = charPos.column;
            }
            else
            {
                attributeMap[y][x] = 0;
            }
        }
    }
}
/*End of interpretBoard*/

/*Will call some functions and clear clear some things*/
void switchMap(char* whatMap)
{
    loadMap(whatMap);
}
/*End of switchMap*/

/*This function will 'teleport' or I should rather type, assign the very next
 *position of character*/
int teleport(int searchFor)
{
    int x, y;
    /*Search for chosen digit*/
    for (y = 0; y <= HEIGHT_INT - 1; y++)
    {
        for (x = 0; x <= WIDTH_WIN - 1; x++)
        {
            /*In case if used two same digits, skip the entrance position*/
            if (y == nextPos.row && x == nextPos.column)
            {
                continue;
            }
            /*If found right exit, convert int to char and assign nextPos*/
            else if (board[y][x] == searchFor + 48)
            {
                nextPos.row = y;
                nextPos.column = x;
                update(pos);
                charPos.row = y;
                charPos.column = x;
                return TRUE;
            }
        }
    }
    return FALSE;
}
/*End of teleport*/

/*This function will get the digit from attributeMap array and return is it
 *possible or perform by itself functions like teleport*/
int checkAction(int digit)
{
    switch (digit)
    {
    /*Floor attribute, always possible to move, so always TRUE*/
    case 0:
        return TRUE;
    /*Wall attribute, never possible to move, so always FALSE*/
    case 1:
        return FALSE;
    /*Just run some action*/
    case 2:
        /*Pass what number is on the next position*/
        pAAction(board[nextPos.row][nextPos.column] - 48);
        break;
    /*Allow to walk on that area + run some action*/
    case 3:
        /*Pass what number is on the next position*/
        pAAction(board[nextPos.row][nextPos.column] - 48);
        return TRUE;
    }
    return -1;
}
/*End of checkAction*/

/*Change the attribute of chosen area*/
int cMAttribute(int searchFor, int dY, int dX, int attribute)
{
    int x, y;
    /*Search for chosen aChar*/
    for (y = 0; y <= HEIGHT_INT - 1; y++)
    {
        for (x = 0; x <= WIDTH_WIN - 1; x++)
        {
            if (board[y][x] == searchFor + 48)
            {
                attributeMap[y-dY][x+dX] = attribute;
                return 0;
            }
        }
    }
    return 0;
}
/*End of changeAttribute*/

/*Change appearance using action scripting*/
int cMAppearance(int searchFor, int dY, int dX, char character)
{
    int x, y;
    /*Search for chosen aChar*/
    for (y = 0; y <= HEIGHT_INT - 1; y++)
    {
        for (x = 0; x <= WIDTH_WIN - 1; x++)
        {
            if (board[y][x] == searchFor + 48)
            {
                visibleMap[y-dY][x+dX] = character;
                /*If our character is standing on that point, don't refresh it now*/
                if (charPos.row == y-dY && charPos.column == x+dX)
                    return 0;

                mvwprintw (interactif_win, y-dY, x+dX, "%c", visibleMap[y-dY][x+dX]);
                wrefresh(interactif_win);
                return 0;
            }
        }
    }
    return -1;
}
/*End of changeAppearance*/

/*This will return attribute of chosen aChar's area*/
int returnAttribute(int searchFor)
{
    int x, y;
    for (y = 0; y <= HEIGHT_INT - 1; y++)
    {
        for (x = 0; x <= WIDTH_WIN - 1; x++)
        {
            if (board[y][x] == searchFor + 48)
            {
                return attributeMap[y][x];
            }
        }
    }
    return 0;
}
/*End of returnAttribute*/

/*It will return what is in the visibleMap over the aChar*/
char returnVisible(int searchFor)
{
    int x, y;
    for (y = 0; y <= HEIGHT_INT - 1; y++)
    {
        for (x = 0; x <= WIDTH_WIN - 1; x++)
        {
            if (board[y][x] == searchFor + 48)
            {
                return visibleMap[y][x];
            }
        }
    }
    return 0;
}
/*End of returnVisible*/

/*Return one of the coords (only one cause it will be later helpful in dividing map)*/
/*Defined as returnACharPos !*/
int f_returnACharPos(int searchFor, char xOrY)
{
    int x, y;
    for (y = 0; y <= HEIGHT_INT - 1; y++)
    {
        for (x = 0; x <= WIDTH_WIN - 1; x++)
        {
            if (board[y][x] == searchFor + 48)
            {
                if (xOrY == 'y')
                    return y;
                else if (xOrY == 'x')
                    return x;
                else
                    perror ("'xOrY' out of speck");
                    SC(xOrY);
                    assert(!TRUE);
            }
        }
    }
    /*If not found, return -1*/
    return -1;
}
/*End of f_returnACharPos*/

/*Will open/close the doors*/
int useDoor(int searchFor, int direction)
{
    if (openDoor(searchFor, direction) == FALSE)
        closeDoor(searchFor, direction);
    return 0;
}
/*End of useDoor*/

/*Will only open the door*/
int openDoor(int searchFor, int direction)
{
    if (returnAttribute(searchFor) != 0)
    {
        if (direction == UP)
        {
            cMAttribute(searchFor, 0, 0, 0);
            cMAppearance(searchFor, 0, 0, ' ');
            cMAppearance(searchFor, 1, 0, '/');
            Sleep (DRAWSPEED*2);
            cMAppearance(searchFor, 1, 0, ' ');
            cMAppearance(searchFor, 1, -1, '|');
            cMAttribute(searchFor, 1, -1, 1);
        }
        else if (direction == DOWN)
        {
            cMAttribute(searchFor, 0, 0, 0);
            cMAppearance(searchFor, 0, 0, ' ');
            cMAppearance(searchFor, -1, 0, '/');
            Sleep (DRAWSPEED*2);
            cMAppearance(searchFor, -1, 0, ' ');
            cMAppearance(searchFor, -1, 1, '|');
            cMAttribute(searchFor, -1, 1, 1);
        }
        else if (direction == LEFT)
        {
            cMAttribute(searchFor, 0, 0, 0);
            cMAppearance(searchFor, 0, 0, ' ');
            cMAppearance(searchFor, 0, -1, 92);
            Sleep (DRAWSPEED*2);
            cMAppearance(searchFor, 0, -1, ' ');
            cMAppearance(searchFor, -1, -1, '-');
            cMAttribute(searchFor, -1, -1, 1);
        }
        else if (direction == RIGHT)
        {
            cMAttribute(searchFor, 0, 0, 0);
            cMAppearance(searchFor, 0, 0, ' ');
            cMAppearance(searchFor, 0, 1, 92);
            Sleep (DRAWSPEED*2);
            cMAppearance(searchFor, 0, 1, ' ');
            cMAppearance(searchFor, 1, 1, '-');
            cMAttribute(searchFor, 1, 1, 1);
        }
        return TRUE;
    }
    return FALSE;
}
/*End of openDoor*/

/*Will only close the door*/
int closeDoor(int searchFor, int direction)
{
    if (returnAttribute(searchFor) == 0)
    {
        if (direction == UP)
        {
            cMAttribute(searchFor, 1, -1, 0);
            cMAppearance(searchFor, 1, -1, ' ');
            cMAppearance(searchFor, 1, 0, '/');
            Sleep (DRAWSPEED*2);
            cMAttribute(searchFor, 0, 0, 1);
            cMAppearance(searchFor, 1, 0, ' ');
            cMAppearance(searchFor, 0, 0, '-');
        }
        else if (direction == DOWN)
        {
            cMAttribute(searchFor, -1, 1, 0);
            cMAppearance(searchFor, -1, 1, ' ');
            cMAppearance(searchFor, -1, 0, '/');
            Sleep (DRAWSPEED*2);
            cMAttribute(searchFor, 0, 0, 1);
            cMAppearance(searchFor, -1, 0, ' ');
            cMAppearance(searchFor, 0, 0, '-');
        }
        else if (direction == LEFT)
        {
            cMAttribute(searchFor, -1, -1, 0);
            cMAppearance(searchFor, -1, -1, ' ');
            cMAppearance(searchFor, 0, -1, 92);
            Sleep (DRAWSPEED*2);
            cMAttribute(searchFor, 0, 0, 1);
            cMAppearance(searchFor, 0, -1, ' ');
            cMAppearance(searchFor, 0, 0, '|');
        }
        else if (direction == RIGHT)
        {
            cMAttribute(searchFor, 1, 1, 0);
            cMAppearance(searchFor, 1, 1, ' ');
            cMAppearance(searchFor, 0, 1, 92);
            Sleep (DRAWSPEED*2);
            cMAttribute(searchFor, 0, 0, 1);
            cMAppearance(searchFor, 0, 1, ' ');
            cMAppearance(searchFor, 0, 0, '|');
        }
        return TRUE;
    }
    return FALSE;
}
/*End of closeDoor*/

/*Will create new window, providing an elegant way of passing input*/
/*Defined as passValue !*/
void f_passValue(char* whatColumn, int* returnSuccess)
{
    WINDOW *pass_win;
    pass_win = newwin(HEIGHT_INT, WIDTH_WIN, 1, 1);
    bgchange(pass_win, COLOR_WHITE, COLOR_CYAN, 97);
    wattron(pass_win, COLOR_PAIR(97));
    char value[70];

    if (strcmp (whatColumn, "column1") == 0)
    {
        printCenter (pass_win, 4, "Enter the value for column 1", 1);
        if (writeCenter(pass_win, 6, value) == TRUE)
        {
            strcpy (v_column1, value);
            *returnSuccess = 1;
        }
    }
    else if (strcmp (whatColumn, "column2") == 0)
    {
        printCenter(pass_win, 4, "Enter the value for column 2", 1);
        if (writeCenter(pass_win, 6, value) == TRUE)
        {
            strcpy (v_column2, value);
            *returnSuccess = 2;
        }
    }
    else if (strcmp (whatColumn, "column3") == 0)
    {
        printCenter(pass_win, 4, "Enter the value for column 3", 1);
        if (writeCenter(pass_win, 6, value) == TRUE)
        {
            strcpy (v_column3, value);
            *returnSuccess = 3;
        }
    }
    else if (strcmp (whatColumn, "column4") == 0)
    {
        printCenter(pass_win, 4, "Enter the value for column 4", 1);
        if (writeCenter(pass_win, 6, value) == TRUE)
        {
            strcpy (v_column4, value);
            *returnSuccess = 4;
        }
    }
    else if (strcmp (whatColumn, "column5") == 0)
    {
        printCenter(pass_win, 4, "Enter the value for column 5", 1);
        if (writeCenter(pass_win, 6, value) == TRUE)
        {
            strcpy (v_column5, value);
            *returnSuccess = 5;
        }
    }
    else
        assert (!TRUE);
    /*Revert to previous window*/
    silenceOn();
    wattroff(pass_win, COLOR_PAIR(97));
    bgchange(pass_win, COLOR_CYAN, COLOR_WHITE, 96);
    delwin(pass_win);
    interactif_win = newwin(HEIGHT_INT, WIDTH_WIN, 1, 1);
    wattron(interactif_win, COLOR_PAIR(1));
    draw(board);
}
/*End of f_passValue*/

/*Reset some kind of quest conditions*/
void loadGlobalActions(char* whatMap)
{
    if (strcmp (whatMap, "filtering") == 0)
    {
        action1 = FALSE;
        action2 = FALSE;
    }
}
/*End of loadGlobalActions*/
