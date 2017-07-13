/*
 * fileslist.cpp
 *
 *  Created on: Jun 19, 2017
 *      Author: ranamagdyomran
 */

#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <string>
#include "fileslist.h"

using namespace std ;

fileslist::fileslist()
{
    for(int i=0;i<255;i++)
    {
        name[i]='/';
        folder[i]='/';
    }
    for(int i=0;i<30;i++)
    {
        date[i]='/';
    }
    size=0;
    numberofblocks=0;
    firstBlock=0;
}


void fileslist::setData(char Name[] , double Size, char Date[],int Numberofblocks,int FirstBlock,char Folder [] )
{
    strcpy(name, Name);
    size=Size;
    strcpy(date, Date);
    numberofblocks=Numberofblocks;
    firstBlock= FirstBlock;
    strcpy(folder, Folder);
}

void fileslist::showData()
{
    cout << "\n Name : " << name;
    cout << " Size : " << size;
    cout<<" Date : "<<date;
    cout << " NumberofBlocks : " << numberofblocks;
    cout << " FirstBlock : " << firstBlock;
    cout << " Folder : " << folder<<endl;
}

char * fileslist::retName()
{
    return name;
}

double fileslist::retSize()
{
    return size;
}
char * fileslist::retDate()
{
    return date;
}

int fileslist::retNumberofblocks()
{
    return numberofblocks;
}

int fileslist::retFirstBlock()
{
    return firstBlock;
}

char * fileslist::retFolder()
{
    return folder;
}


fileslist::~fileslist()
{

    // TODO Auto-generated destructor stub
}

