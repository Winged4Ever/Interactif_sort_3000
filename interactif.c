/*
 * File:   interactif.c
 * Author: Tomasz Liszkowski
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
char mapName[20];

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
    printFrom(stdscr, HEIGHT-1, 16, "to move your character.");
    wattroff(stdscr, COLOR_PAIR(1));

    int input;
    while (1 == 1)
    {
        /*Receive input*/
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
        mvwprintw(interactif_win, charPos.row, charPos.column, "%c", board[charPos.row][charPos.column]);
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
                board[row][column] = tempArray[column];
            }
            row++;
        }
    }
	/*When reached end of file and not found anything*/
	fclose (pFile);

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
            /*If is digit (from ASCII table)*/
            else if (board[y][x] >= 48 && board[y][x] <= 57)
            {
                /*Attribute 2 will redirect to performAction function*/
                attributeMap[y][x] = 2;
            }
            /*If is letter (from ASCII table)*/
            else if ((board[y][x] >= 65 && board[y][x] <= 90) || (board[y][x] >= 97 && board[y][x] <= 122))
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
                attributeMap[y][x] = 1;
            }
        }
    }
}
/*End of interpretBoard*/

/*Here will be defined what will perform 'digit' areas. Need to be manually
 *specified after creating the map*/
int performAction(int digit)
{
    if (strcmp(mapName, "menu") == 0)
    {
        if (digit == 1)
        {
            initializeSort();
            switchMap("database");
        }
        else if (digit == 2)
        {
            teleport(digit);
        }
        else if (digit == 3)
        {
            closeProg();
        }
        else
            assert(!TRUE);
    }
    else if (strcmp(mapName, "database") == 0)
    {
        if (digit == 1)
        {
            teleport(2);
            scrollData(UP);
        }
        else if (digit == 2)
        {
            return TRUE;
        }
        else if (digit == 3)
        {
            teleport(2);
            scrollData(DOWN);
        }
        else if (digit == 4)
        {
            if (filterData(3, 2002, 4, 8.7) == TRUE)
            {
                activateDatabase(filtered_database);
                initializeData();
            }
            return TRUE;
        }
        else if (digit == 5)
        {
            activateDatabase(movies_database);
            initializeData();
            return TRUE;
        }
        else if (digit == 6)
        {
            sortData(2, DOWN);
            initializeData();
            return TRUE;
        }
        else
            assert(!TRUE);
    }
    else
    {
        perror ("'mapName' out of speck");
        SC(mapName);
        assert(!TRUE);
    }
    return FALSE;
}
/*End of performAction*/

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
            /*In case if used two same digits, skip the entrance*/
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
        break;
    /*Wall attribute, never possible to move, so always FALSE*/
    case 1:
        return FALSE;
        break;
    /*Action attribute, so need to call performAction*/
    case 2:
        /*Pass what number is on the next position*/
        if (performAction(board[nextPos.row][nextPos.column] - 48) == TRUE)
            return TRUE;
        break;
    }
    return 2;
}
/*End of checkAction*/
