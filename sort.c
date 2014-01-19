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
database_t movies_database;
int firstVisRow;

/*Initialize the sort window*/
void initializeSort()
{
    sort_win = newwin(HEIGHT_SORT, WIDTH_WIN, 11, 1);
    bgchange(sort_win, COLOR_BLACK, COLOR_BLUE, 98);
    loadDatabase();
    initializeData();
}
/*End of initializeSort*/

/*This function will load data from the database in a proper way*/
void loadDatabase()
{
    char tempArray[80];
    WINDOW* whatWindow = sort_win;

	FILE *pFile = fopen ("sortDatabase.dat" , "r");
	/*If file cannot be opened, show an error and then crash*/
	if (pFile == NULL)
	{
		perror ("Error opening file");
	}
	/*If successfully open the file*/
	else
	{
	    int i, row;
        char* token;

        /*First extract titles of columns from first line*/
        fgets (tempArray, sizeof(tempArray) ,pFile);
        /*Extract first 'token' from that line*/
        token = strtok (tempArray, ";");
        for (i = 1; i <= 5; i++)
        {
            switch (i)
            {
            /*NO*/
            case 1:
                strcpy(movies_database.column1_title, token);
                break;
            /*Title*/
            case 2:
                strcpy(movies_database.column2_title, token);
                break;
            /*Year*/
            case 3:
                strcpy(movies_database.column3_title, token);
                break;
            /*Rating*/
            case 4:
                strcpy(movies_database.column4_title, token);
                break;
            /*Your rating*/
            case 5:
                strcpy(movies_database.column5_title, token);
                break;
            }
            /*Extract next 'token' (NULL means to continue extracting where previous strtok ended)*/
            token = strtok (NULL, ";");
        }

        /*Then read other values and assign them*/
        for (row = 0;  row <= DATABSIZE - 1; row++)
        {
            if (fgets (tempArray, sizeof(tempArray) ,pFile) == !EOF)
                break;
            /*Extract first 'token' from that line*/
            token = strtok (tempArray, ";");
            for (i = 1; i <= 5; i++)
            {
                switch (i)
                {
                /*NO*/
                case 1:
                    sscanf (token, "%d", &movies_database.column1[row]);
                    break;
                /*Title*/
                case 2:
                    strcpy(movies_database.column2[row], token);
                    break;
                /*Year*/
                case 3:
                    sscanf (token, "%d", &movies_database.column3[row]);
                    break;
                /*Rating*/
                case 4:
                    sscanf (token, "%f", &movies_database.column4[row]);
                    break;
                /*Your rating*/
                case 5:
                    sscanf (token, "%f", &movies_database.column5[row]);
                    break;
                }
                /*Extract next 'token' (NULL means to continue extracting where previous strtok ended)*/
                token = strtok (NULL, ";");
            }
        }
    }
	/*When reached end of file and not found anything*/
	fclose (pFile);
	wattron(whatWindow, COLOR_PAIR(5));
	draw(table);
	initializeData();
    wattroff(whatWindow, COLOR_PAIR(5));
}
/*End of loadDatabase*/

/*This function will print in sort_win whole database structure without sorting*/
void initializeData()
{
    WINDOW* whatWindow = sort_win;
    int column, row, firstVisRow = 1;

    printData(1, 0, 0, title);
    printData(2, 0, 0, title);
    printData(3, 0, 0, title);
    printData(4, 0, 0, title);
    printData(5, 0, 0, title);
    wrefresh(whatWindow);
    Sleep (DRAWSPEED);

    for (row = 2; row <= YMAX-1; row++)
    {
        for (column = 1; column <= 5; column++)
        {
            printData(column, row-2, row, data);
            printData(column, row-2, row, data);
            printData(column, row-2, row, data);
            printData(column, row-2, row, data);
            printData(column, row-2, row, data);
        }
    }
}
/*End of initializeData*/

/*This awful function will print out what we want and where we want*/
void f_printData(int whatColumn, int rowInFile, int rowInWindow, char* whatToPrint)
{
    WINDOW* whatWindow = sort_win;
    wattron(whatWindow, COLOR_PAIR(5));
    int x = 0;

    if (whatColumn == 1)
    {
        x = 1;
        if (strcmp (whatToPrint, "data") == 0)
        {
            mvwprintw(whatWindow, rowInWindow, x, "  ");
            int goingToPrint = movies_database.column1[rowInFile];
            mvwprintw(whatWindow, rowInWindow, x, "%d", goingToPrint);
        }
        else if (strcmp (whatToPrint, "title") == 0)
        {
            char goingToPrint[80];
            strcpy (goingToPrint, movies_database.column1_title);
            mvwprintw(whatWindow, rowInWindow, x, "%s", goingToPrint);
        }
        else
            assert(!TRUE);
    }
    else if (whatColumn == 2)
    {
        x = 6;
        char goingToPrint[80];

        if (strcmp (whatToPrint, "data") == 0)
        {
            mvwprintw(whatWindow, rowInWindow, x, "                                          ");
            if (stringLength(movies_database.column2[rowInFile]) >= 41)
            {
                int i;

                for (i = 0; i <= 40; i++)
                {
                    goingToPrint[i] = movies_database.column2[rowInFile][i];
                }
                goingToPrint[i+1] = '\0';
                goingToPrint[i] = '/';
            }
            else
                strcpy (goingToPrint, movies_database.column2[rowInFile]);
        }
        else if (strcmp (whatToPrint, "title") == 0)
        {
            strcpy (goingToPrint, movies_database.column2_title);
        }
        else
            assert(!TRUE);
        mvwprintw(whatWindow, rowInWindow, x, "%s", goingToPrint);
    }
    else if (whatColumn == 3)
    {
        x = 50;
        if (strcmp (whatToPrint, "data") == 0)
        {
             int goingToPrint = movies_database.column3[rowInFile];
             mvwprintw(whatWindow, rowInWindow, x, "%d", goingToPrint);
        }
        else if (strcmp (whatToPrint, "title") == 0)
        {
            char goingToPrint[80];
            strcpy (goingToPrint, movies_database.column3_title);
            mvwprintw(whatWindow, rowInWindow, x, "%s", goingToPrint);
        }
        else
            assert(!TRUE);
    }
    else if (whatColumn == 4)
    {
        x = 57;
        if (strcmp (whatToPrint, "data") == 0)
        {
            float goingToPrint = movies_database.column4[rowInFile];
            mvwprintw(whatWindow, rowInWindow, x, "%1.1f", goingToPrint);
        }
        else if (strcmp (whatToPrint, "title") == 0)
        {
            char goingToPrint[80];
            strcpy (goingToPrint, movies_database.column4_title);
            mvwprintw(whatWindow, rowInWindow, x, "%s", goingToPrint);
        }
        else
            assert(!TRUE);
    }
    else if (whatColumn == 5)
    {
        x = 66;
        if (strcmp (whatToPrint, "data") == 0)
        {
            float goingToPrint = movies_database.column5[rowInFile];
            mvwprintw(whatWindow, rowInWindow, x, "%1.1f", goingToPrint);
        }
        else if (strcmp (whatToPrint, "title") == 0)
        {
            char goingToPrint[80];
            strcpy (goingToPrint, movies_database.column5_title);
            mvwprintw(whatWindow, rowInWindow, x, "%s", goingToPrint);
        }
        else
            assert(!TRUE);
    }
}
/*End of f_printData*/

/*This function will scroll the data*/
void scrollData(int direction)
{
    WINDOW* whatWindow = sort_win;
    int rowInWindow, column;

    if (direction == UP)
    {
        /*Check if is not scrolled all the way up already*/
        if (firstVisRow >= 1)
        {
            firstVisRow--;
            /*Print in all rows in window*/
            for (rowInWindow = 2; rowInWindow <= YMAX-1; rowInWindow++)
            {
                /*Print in all columns*/
                for (column = 1; column <= 5; column++)
                {
                    printData(column, firstVisRow + rowInWindow - 2, rowInWindow, data);
                }
            }
            wrefresh(whatWindow);
        }
    }
    else if (direction == DOWN)
    {
        /*Check if is not scrolled all the way down already*/
        if (firstVisRow <= DATABSIZE-11)
        {
            firstVisRow++;
            /*Print in all rows in window*/
            for (rowInWindow = 2; rowInWindow <= YMAX-1; rowInWindow++)
            {
                /*Print in all columns*/
                for (column = 1; column <= 5; column++)
                {

                    printData(column, firstVisRow + rowInWindow - 2, rowInWindow, data);
                }
            }
        }
        wrefresh(whatWindow);
    }
    else
        assert(!TRUE);
}
/*End of scrollData*/
