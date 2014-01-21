/*
 * File:   userDefined.c
 * Author: Tomasz 'Winged' Liszkowski
 */

#include <curses.h>
#include <windows.h>
#include <assert.h>
#include <string.h>
#include "header.h"

/*Define here what the action characters on the map will do when entered on their area*/
int pAAction(int aChar)
{
    if (strcmp(mapName, "menu") == 0)
    {
        switch (aChar)
        {
        case 1:
            initializeSort();
            switchMap("database");
            break;
        case 2:
            teleport(aChar);
            break;
        case 3:
            closeProg();
            break;
        default:
            return TRUE;
        }
    }
    else if (strcmp(mapName, "database") == 0)
    {
        switch (aChar)
        {
        case 1:
            teleport(2);
            scrollData(UP);
            break;
        case 2:
            return TRUE;
        case 3:
            teleport(2);
            scrollData(DOWN);
            break;
        case 4:
            if (filterData(3, 2002, 4, 8.7) == TRUE)
            {
                activateDatabase(filtered_database);
                initializeData();
            }
            return TRUE;
        case D:
            return FALSE;
        default:
            return TRUE;
        }
    }
    else
    {
        perror ("'mapName' out of speck");
        SC(mapName);
        assert(!TRUE);
    }
    return 2;
}
/*End of pAAction*/

/*If you want to change e.g. appearance or attribute of a character automatically when loading the map, change it here*/
void cAAppearance(int x, int y)
{
    int aChar = visibleMap[x][y] - 48;
    if (strcmp(mapName, "menu") == 0)
    {
        switch (aChar)
        {
        case 0:
            visibleMap[x][y] = '2';
            break;
        case 1:
            visibleMap[x][y] = 'O';
            break;
        case A:
            visibleMap[x][y] = '1';
            break;
        case B:
            visibleMap[x][y] = '2';
            break;
        case C:
            visibleMap[x][y] = '3';
            break;
        /*Appearance don't need to be changed*/
        default:
            break;
        }
    }
    else if (strcmp(mapName, "database") == 0)
    {
        switch (aChar)
        {
        case A:
            visibleMap[x][y] = 'Z';
            break;
        case D:
            visibleMap[x][y] = '-';
            break;
        case 5:
            visibleMap[x][y] = ' ';
            break;
        /*Appearance don't need to be changed*/
        default:
            break;
        }
    }
    else
    {
        perror ("'mapName' out of speck");
        SC(mapName);
        assert(!TRUE);
    }
}
/*End of cAAppearance*/

/*What will happen when pressed ENTER while standing on chosen area*/
int pMAction(int aChar)
{
    if (strcmp(mapName, "menu") == 0)
    {
        switch (aChar)
        {
        case 1:
            break;
        default:
            return TRUE;
        }
    }
    else if (strcmp(mapName, "database") == 0)
    {
        switch (aChar)
        {
        case 5:
            useDoor(D, UP);
            return TRUE;
        default:
            return FALSE;
        }
    }
    else
    {
        perror ("'mapName' out of speck");
        SC(mapName);
        assert(!TRUE);
    }
    return 2;
}
/*End of pMAction*/
