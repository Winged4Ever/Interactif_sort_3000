/*
 * File:   sort.c
 * Author: Tomasz Liszkowski
 */

#include <curses.h>
#include <windows.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

WINDOW *sort_win;
database_t current_database;
database_t movies_database;
database_t filtered_database;
int firstVisRow, maxVisRow;

/*Initialize the sort window*/
void initializeSort()
{
    sort_win = newwin(HEIGHT_SORT, WIDTH_WIN, 11, 1);
    bgchange(sort_win, COLOR_BLACK, COLOR_BLUE, 98);
    loadDatabase(&movies_database);
    activateDatabase(movies_database);
    initializeData();
}
/*End of initializeSort*/

/*This function will load data from the database in a proper way*/
void loadDatabase(database_t *whatDatabase)
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
                strcpy(whatDatabase->column1_title, token);
                break;
            /*Title*/
            case 2:
                strcpy(whatDatabase->column2_title, token);
                break;
            /*Year*/
            case 3:
                strcpy(whatDatabase->column3_title, token);
                break;
            /*Rating*/
            case 4:
                strcpy(whatDatabase->column4_title, token);
                break;
            /*Your rating*/
            case 5:
                strcpy(whatDatabase->column5_title, token);
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
                    sscanf (token, "%d", &whatDatabase->column1[row]);
                    break;
                /*Title*/
                case 2:
                    strcpy(whatDatabase->column2[row], token);
                    break;
                /*Year*/
                case 3:
                    sscanf (token, "%d", &whatDatabase->column3[row]);
                    break;
                /*Rating*/
                case 4:
                    sscanf (token, "%f", &whatDatabase->column4[row]);
                    break;
                /*Your rating*/
                case 5:
                    sscanf (token, "%f", &whatDatabase->column5[row]);
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

/*Switches provided database to the current database*/
void activateDatabase(database_t whatDatabase)
{
    int i;
    maxVisRow = 0, firstVisRow = 0;

    for (i = 0; i <= DATABSIZE-1; i++)
    {
        current_database.column1[i] = whatDatabase.column1[i];
        strcpy (current_database.column1_title, whatDatabase.column1_title);
        strcpy (current_database.column2[i], whatDatabase.column2[i]);
        strcpy (current_database.column2_title, whatDatabase.column2_title);
        current_database.column3[i] = whatDatabase.column3[i];
        strcpy (current_database.column3_title, whatDatabase.column3_title);
        current_database.column4[i] = whatDatabase.column4[i];
        strcpy (current_database.column4_title, whatDatabase.column4_title);
        current_database.column5[i] = whatDatabase.column5[i];
        strcpy (current_database.column5_title, whatDatabase.column5_title);

        if (whatDatabase.column1[i] != 0)
            maxVisRow++;
    }
    /*Manage how far can scroll go (if is less than 10 lines, it should not be able to scroll*/
    if (maxVisRow <= 10)
        maxVisRow = 0;
    else
        maxVisRow -= 10;
}
/*End of activateDatabase*/

/*This function will print in sort_win whole current database structure without sorting*/
void initializeData()
{
    WINDOW* whatWindow = sort_win;
    int column, row;

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
    wrefresh(sort_win);
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
            int goingToPrint = current_database.column1[rowInFile];
            mvwprintw(whatWindow, rowInWindow, x, "%d", goingToPrint);
        }
        else if (strcmp (whatToPrint, "title") == 0)
        {
            char goingToPrint[80];
            strcpy (goingToPrint, current_database.column1_title);
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
            if (stringLength(current_database.column2[rowInFile]) >= 41)
            {
                int i;

                for (i = 0; i <= 40; i++)
                {
                    goingToPrint[i] = current_database.column2[rowInFile][i];
                }
                goingToPrint[i+1] = '\0';
                goingToPrint[i] = '/';
            }
            else
                strcpy (goingToPrint, current_database.column2[rowInFile]);
        }
        else if (strcmp (whatToPrint, "title") == 0)
        {
            strcpy (goingToPrint, current_database.column2_title);
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
            mvwprintw(whatWindow, rowInWindow, x, "    ");
            int goingToPrint = current_database.column3[rowInFile];
            mvwprintw(whatWindow, rowInWindow, x, "%d", goingToPrint);
        }
        else if (strcmp (whatToPrint, "title") == 0)
        {
            char goingToPrint[80];
            strcpy (goingToPrint, current_database.column3_title);
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
            mvwprintw(whatWindow, rowInWindow, x, "  ");
            float goingToPrint = current_database.column4[rowInFile];
            mvwprintw(whatWindow, rowInWindow, x, "%1.1f", goingToPrint);
        }
        else if (strcmp (whatToPrint, "title") == 0)
        {
            char goingToPrint[80];
            strcpy (goingToPrint, current_database.column4_title);
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
            mvwprintw(whatWindow, rowInWindow, x, "  ");
            float goingToPrint = current_database.column5[rowInFile];
            mvwprintw(whatWindow, rowInWindow, x, "%1.1f", goingToPrint);
        }
        else if (strcmp (whatToPrint, "title") == 0)
        {
            char goingToPrint[80];
            strcpy (goingToPrint, current_database.column5_title);
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
        if (firstVisRow > 0)
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
        if (firstVisRow < maxVisRow)
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

/*This function will sort the data focusing on chosen column*/
void sortData(int whichCol, int order)
{
    /*Increasing*/
    if (order == UP)
    {

    }
    /*Decreasing*/
    else if (order == DOWN)
    {

    }
    else
        assert(!TRUE);
}
/*End of sortData*/

/*This function will leave only lines with chosen values*/
int f_filterData(int col1, char* value1, int col2, char* value2)
{
    /*Check if not provided same columns*/
    if (col1 == col2)
        return FALSE;

    int i = 0, line;

    if (col1 == 1)
    {
        /*Create appropriate type of variable*/
        int searchFor1 = atoi(value1);
        /*First check for value1 in col1, check every line*/
        for (line = 0; line <= DATABSIZE-1; line++)
        {
            /*If found chosen value1 in column1*/
            if (movies_database.column1[line] == searchFor1)
            {
                /*Now check the value2 in col2 in current line*/
                if (col2 == 1)
                {
                    int searchFor2 = atoi(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column1[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 2)
                {
                    /*If found chosen value2 in column2*/
                    if (strcmp (movies_database.column2[line], value2) == 0)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 3)
                {
                    int searchFor2 = atoi(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column3[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 4)
                {
                    float searchFor2 = atof(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column4[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 5)
                {
                    float searchFor2 = atof(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column5[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
            }
        }
    }
    else if (col1 == 2)
    {
        /*First check for value1 in col1, check every line*/
        for (line = 0; line <= DATABSIZE-1; line++)
        {
            /*If found chosen value1 in column1*/
            if (strcmp (movies_database.column2[line], value1) == 0)
            {
                /*Now check the value2 in col2 in current line*/
                if (col2 == 1)
                {
                    int searchFor2 = atoi(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column1[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 2)
                {
                    /*If found chosen value2 in column2*/
                    if (strcmp (movies_database.column2[line], value2) == 0)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 3)
                {
                    int searchFor2 = atoi(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column3[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 4)
                {
                    float searchFor2 = atof(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column4[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 5)
                {
                    float searchFor2 = atof(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column5[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
            }
        }
    }
    else if (col1 == 3)
    {
        /*Create appropriate type of variable*/
        int searchFor1 = atoi(value1);
        /*First check for value1 in col1, check every line*/
        for (line = 0; line <= DATABSIZE-1; line++)
        {
            /*If found chosen value1 in column1*/
            if (movies_database.column3[line] == searchFor1)
            {
                /*Now check the value2 in col2 in current line*/
                if (col2 == 1)
                {
                    int searchFor2 = atoi(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column1[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 2)
                {
                    /*If found chosen value2 in column2*/
                    if (strcmp (movies_database.column2[line], value2) == 0)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 3)
                {
                    int searchFor2 = atoi(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column3[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 4)
                {
                    float searchFor2 = atof(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column4[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 5)
                {
                    float searchFor2 = atof(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column5[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
            }
        }
    }
    else if (col1 == 4)
    {
        /*Create appropriate type of variable*/
        float searchFor1 = atof(value1);
        /*First check for value1 in col1, check every line*/
        for (line = 0; line <= DATABSIZE-1; line++)
        {
            /*If found chosen value1 in column1*/
            if (movies_database.column4[line] == searchFor1)
            {
                /*Now check the value2 in col2 in current line*/
                if (col2 == 1)
                {
                    int searchFor2 = atoi(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column1[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 2)
                {
                    /*If found chosen value2 in column2*/
                    if (strcmp (movies_database.column2[line], value2) == 0)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 3)
                {
                    int searchFor2 = atoi(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column3[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 4)
                {
                    float searchFor2 = atof(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column4[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 5)
                {
                    float searchFor2 = atof(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column5[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
            }
        }
    }
    else if (col1 == 5)
    {
        /*Create appropriate type of variable*/
        float searchFor1 = atof(value1);
        /*First check for value1 in col1, check every line*/
        for (line = 0; line <= DATABSIZE-1; line++)
        {
            /*If found chosen value1 in column1*/
            if (movies_database.column5[line] == searchFor1)
            {
                /*Now check the value2 in col2 in current line*/
                if (col2 == 1)
                {
                    int searchFor2 = atoi(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column1[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 2)
                {
                    /*If found chosen value2 in column2*/
                    if (strcmp (movies_database.column2[line], value2) == 0)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 3)
                {
                    int searchFor2 = atoi(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column3[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 4)
                {
                    float searchFor2 = atof(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column4[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
                else if (col2 == 5)
                {
                    float searchFor2 = atof(value2);
                    /*If found chosen value2 in column2*/
                    if (movies_database.column5[line] == searchFor2)
                    {
                        /*Copy those values to newly created structure*/
                        filtered_database.column1[i] = movies_database.column1[line];
                        strcpy (filtered_database.column2[i], movies_database.column2[line]);
                        filtered_database.column3[i] = movies_database.column3[line];
                        filtered_database.column4[i] = movies_database.column4[line];
                        filtered_database.column5[i] = movies_database.column5[line];
                        i++;
                    }
                }
            }
        }
    }
    else
        assert(!TRUE);

    /*If found at least one matching data*/
    if (i != 0)
        return TRUE;

    /*If not found anything*/
    return FALSE;
}
/*End of f_filterData*/
