/*
 * File:   userDefined.c
 * Author: Tomasz 'Winged' Liszkowski
 */

#include <curses.h>
#include <windows.h>
#include <assert.h>
#include <string.h>
#include "header.h"

int v_order = UP;

/*Define here what the action characters on the map will do when entered on their area*/
void pAAction(int aChar)
{
    if (strcmp(mapName, "menu") == 0)
    {
        switch (aChar)
        {
        case 1:
            initializeSort();
            switchMap("filtering");
            break;
        case 2:
            initializeSort();
            switchMap("sorting");
            break;
        case 3:
            teleport(aChar);
            break;
        case 4:
            closeProg();
            break;
        }
    }
    else if (strcmp(mapName, "sorting") == 0)
    {
        switch (aChar)
        {
        case A:
            teleport(B);
            scrollData(UP);
            break;
        case C:
            teleport(B);
            scrollData(DOWN);
            break;
        }
    }
    else if (strcmp(mapName, "filtering") == 0)
    {
        switch (aChar)
        {
        case 1:
            teleport(2);
            scrollData(UP);
            break;
        case 3:
            teleport(2);
            scrollData(DOWN);
            break;
        case 7:
            closeDoor(Q, DOWN);
            break;
        case 8:
            closeDoor(W, UP);
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
/*End of pAAction*/

/*If you want to change e.g. appearance or attribute of a character automatically when loading the map, change it here*/
void cAAppearance(int y, int x)
{
    int aChar = board[y][x] - 48;
    if (strcmp(mapName, "menu") == 0)
    {
        switch (aChar)
        {
        /*Appearance don't need to be changed*/
        default:
            break;
        }
    }
    else if (strcmp(mapName, "sorting") == 0)
    {
        switch (aChar)
        {
        case A:
            visibleMap[y][x] = '^';
            break;
        case B:
            visibleMap[y][x] = ' ';
            break;
        case C:
            visibleMap[y][x] = 'v';
            break;
        case D:
            visibleMap[y][x] = 'I';
            break;
        case X:
            visibleMap[y][x] = 64-48;
            break;
        /*Appearance don't need to be changed*/
        default:
            break;
        }
    }
    else if (strcmp(mapName, "filtering") == 0)
    {
        switch (aChar)
        {
        case 6:
            visibleMap[y][x] = ' ';
            break;
        case 7:
            visibleMap[y][x] = ' ';
            break;
        case 8:
            visibleMap[y][x] = ' ';
            break;
        case U:
            visibleMap[y][x] = ' ';
            break;
        case I:
            visibleMap[y][x] = ' ';
            break;
        case Z:
            visibleMap[y][x] = '1';
            break;
        case X:
            visibleMap[y][x] = '2';
            break;
        case C:
            visibleMap[y][x] = '3';
            break;
        case V:
            visibleMap[y][x] = '4';
            break;
        case B:
            visibleMap[y][x] = '5';
            break;
        case 1:
            visibleMap[y][x] = '^';
            break;
        case 2:
            visibleMap[y][x] = ' ';
            break;
        case 3:
            visibleMap[y][x] = 'v';
            break;
        case Q:
            visibleMap[y][x] = '-';
            break;
        case W:
            visibleMap[y][x] = '-';
            break;
        case E:
            visibleMap[y][x] = '|';
            break;
        case R:
            visibleMap[y][x] = '|';
            break;
        case T:
            visibleMap[y][x] = '|';
            break;
        case Y:
            visibleMap[y][x] = '|';
            break;
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
void pMAction(int aChar)
{
    if (strcmp(mapName, "menu") == 0)
    {
        switch (aChar)
        {
        case 1:
            break;
        case 0:
            useDoor(E, UP);
            useDoor(F, DOWN);
            useDoor(H, LEFT);
            useDoor(G, RIGHT);
            break;
        }
    }
    else if (strcmp(mapName, "sorting") == 0)
    {
        switch (aChar)
        {
        case 1:
            sortData(1, v_order);
            break;
        case 2:
            sortData(2, v_order);
            break;
        case 3:
            sortData(3, v_order);
            break;
        case 4:
            sortData(4, v_order);
            break;
        case 5:
            sortData(5, v_order);
            break;
        case O:
            initializeData();
            break;
        case U:
            v_order = UP;
            break;
        case D:
            v_order = DOWN;
            break;
            /*@*/
        case (64-48):
            switchMap("menu");
            break;
        }
    }
    else if (strcmp(mapName, "filtering") == 0)
    {
        switch (aChar)
        {
        case O:
            loadGlobalActions(mapName);
            activateDatabase(movies_database);
            initializeData();
            cMAppearance(U, 0, 0, ' ');
            cMAppearance(I, 0, 0, ' ');
            break;
        case Z:
            if (action1 == 1 && action2 == FALSE)
                break;
            if (action1 != FALSE)
            {
                passValue(column1, &action2);
                teleport (U);
                openDoor(Q, DOWN);
                if (action2 != FALSE)
                    cMAppearance(U, 0, 0, '*');
            }
            else
            {
                passValue(column1, &action1);
                teleport (I);
                openDoor(W, UP);
                if (action1 != FALSE)
                    cMAppearance(I, 0, 0, '*');
            }
            break;
        case X:
            if (action1 == 2 && action2 == FALSE)
                break;
            if (action1 != FALSE)
            {
                passValue(column2, &action2);
                teleport (U);
                openDoor(Q, DOWN);
                if (action2 != FALSE)
                    cMAppearance(U, 0, 0, '*');
            }
            else
            {
                passValue(column2, &action1);
                teleport (I);
                openDoor(W, UP);
                if (action1 != FALSE)
                    cMAppearance(I, 0, 0, '*');
            }
            break;
        case C:
            if (action1 == 3 && action2 == FALSE)
                break;
            if (action1 != FALSE)
            {
                passValue(column3, &action2);
                teleport (U);
                openDoor(Q, DOWN);
                if (action2 != FALSE)
                    cMAppearance(U, 0, 0, '*');
            }
            else
            {
                passValue(column3, &action1);
                teleport (I);
                openDoor(W, UP);
                if (action1 != FALSE)
                    cMAppearance(I, 0, 0, '*');
            }
            break;
        case V:
            if (action1 == 4 && action2 == FALSE)
                break;
            if (action1 == TRUE)
            {
                passValue(column4, &action2);
                teleport (U);
                openDoor(Q, DOWN);
                if (action2 != FALSE)
                    cMAppearance(U, 0, 0, '*');
            }
            else
            {
                passValue(column4, &action1);
                teleport (I);
                openDoor(W, UP);
                if (action1 != FALSE)
                    cMAppearance(I, 0, 0, '*');
            }
            break;
        case B:
            if (action1 == 5 && action2 == FALSE)
                break;
            if (action1 != FALSE)
            {
                passValue(column5, &action2);
                teleport (U);
                openDoor(Q, DOWN);
                if (action2 != FALSE)
                    cMAppearance(U, 0, 0, '*');
            }
            else
            {
                passValue(column5, &action1);
                teleport (I);
                openDoor(W, UP);
                if (action1 != FALSE)
                    cMAppearance(I, 0, 0, '*');
            }
            break;
        case 6:
            useDoor(E, RIGHT);
            break;
        case 9:
            switch (action1)
            {
            case 1:
                switch (action2)
                {
                case 2:
                    filterData(action1, v_column1, action2, v_column2);
                    break;
                case 3:
                    filterData(action1, v_column1, action2, v_column3);
                    break;
                case 4:
                    filterData(action1, v_column1, action2, v_column4);
                    break;
                case 5:
                    filterData(action1, v_column1, action2, v_column5);
                    break;
                }
                break;
            case 2:
                switch (action2)
                {
                case 1:
                    filterData(action1, v_column2, action2, v_column1);
                    break;
                case 3:
                    filterData(action1, v_column2, action2, v_column3);
                    break;
                case 4:
                    filterData(action1, v_column2, action2, v_column4);
                    break;
                case 5:
                    filterData(action1, v_column2, action2, v_column5);
                    break;
                }
                break;
            case 3:
                switch (action2)
                {
                case 1:
                    filterData(action1, v_column3, action2, v_column1);
                    break;
                case 2:
                    filterData(action1, v_column3, action2, v_column2);
                    break;
                case 4:
                    filterData(action1, v_column3, action2, v_column4);
                    break;
                case 5:
                    filterData(action1, v_column3, action2, v_column5);
                    break;
                }
                break;
            case 4:
                switch (action2)
                {
                case 1:
                    filterData(action1, v_column4, action2, v_column1);
                    break;
                case 2:
                    filterData(action1, v_column4, action2, v_column2);
                    break;
                case 3:
                    filterData(action1, v_column4, action2, v_column3);
                    break;
                case 5:
                    filterData(action1, v_column4, action2, v_column5);
                    break;
                }
                break;
            case 5:
                switch (action2)
                {
                case 1:
                    filterData(action1, v_column5, action2, v_column1);
                    break;
                case 2:
                    filterData(action1, v_column5, action2, v_column2);
                    break;
                case 3:
                    filterData(action1, v_column5, action2, v_column3);
                    break;
                case 4:
                    filterData(action1, v_column5, action2, v_column4);
                    break;
                }
                break;
            }
            break;
            /*@*/
        case (64-48):
            switchMap("menu");
            break;
        }
    }
    else
    {
        perror ("'mapName' out of speck");
        SC(mapName);
        assert(!TRUE);
    }
    mapConditions();
}
/*End of pMAction*/

/*Change attribute of uppercases*/
void cAAttribute(int y, int x)
{
    int aChar = board[y][x] - 48;
    if (strcmp(mapName, "menu") == 0)
    {
        switch (aChar)
        {
        case 1:
            attributeMap[y][x] = 2;
            break;
        case 2:
            attributeMap[y][x] = 2;
            break;
        case 3:
            attributeMap[y][x] = 2;
            break;
        case 4:
            attributeMap[y][x] = 2;
            break;
        default:
            attributeMap[y][x] = 1;
            break;
        }
    }
    else if (strcmp(mapName, "sorting") == 0)
    {
        switch (aChar)
        {
        case A:
            attributeMap[y][x] = 2;
            break;
        case C:
            attributeMap[y][x] = 2;
            break;
        default:
            attributeMap[y][x] = 0;
            break;
        }
    }
    else if (strcmp(mapName, "filtering") == 0)
    {
        switch (aChar)
        {
            break;
        case 1:
            attributeMap[y][x] = 2;
            break;
        case 2:
            attributeMap[y][x] = 0;
            break;
        case 3:
            attributeMap[y][x] = 2;
            break;
        case 6:
            attributeMap[y][x] = 0;
            break;
        case 7:
            attributeMap[y][x] = 3;
            break;
        case 8:
            attributeMap[y][x] = 3;
            break;
        case Z:
            attributeMap[y][x] = 0;
            break;
        case X:
            attributeMap[y][x] = 0;
            break;
        case C:
            attributeMap[y][x] = 0;
            break;
        case V:
            attributeMap[y][x] = 0;
            break;
        case B:
            attributeMap[y][x] = 0;
            break;
        case 9:
            attributeMap[y][x] = 0;
            break;
        case O:
            attributeMap[y][x] = 0;
            break;
        default:
            attributeMap[y][x] = 1;
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
/*End of cAAttribute*/

/*If sufficient number of condition is met, will invoke an action*/
void mapConditions()
{
    if (strcmp(mapName, "filtering") == 0)
    {
        if (action1 != FALSE && action2 != FALSE)
            openDoor(R, RIGHT);
        else
            closeDoor(R, RIGHT);
    }
}
/*End of mapConditions*/
