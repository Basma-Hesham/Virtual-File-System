/*
 * fileslist.h
 *
 *  Created on: Jun 19, 2017
 *      Author: ranamagdyomran
 */

#ifndef FILESLIST_H_
#define FILESLIST_H_
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <fstream>
using namespace std;

class fileslist {
public:
    fileslist();
    void setData(char Name[] , double Size, char Date[],int Numberofblocks,int FirstBlock,char Folder [] );
    void showData();
    char * retName();
    double retSize();
    char * retDate();
    int retNumberofblocks();
    int retFirstBlock();
    char * retFolder();
    virtual ~fileslist();
private:
    char name[255];
    double size;
    char date[30];
    int numberofblocks;
    int firstBlock;
    char folder [255] ;
};


#endif /* FILESLIST_H_ */
