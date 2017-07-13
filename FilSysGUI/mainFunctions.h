/*
 * mainFunctions.h
 *
 *  Created on: Jun 28, 2017
 *      Author: rana
 */

#ifndef MAINFUNCTIONS_H_
#define MAINFUNCTIONS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
//#include <unistd.h>
#include <vector>
#include <sstream>
#include <chrono>  // for high_resolution_clock
#include "fileSystem.h"

#ifdef __linux	                    /* __unix__ is usually defined by compilers targeting Unix systems */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#endif

#ifdef _WIN32     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */

#include <windows.h>
#include <io.h>//instead of unistd.h
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <direct.h>
#define GetCurrentDir _getcwd

#endif


using namespace std;
extern fileSystem VFS;

std::ifstream::pos_type filesize(const char* filename);
double CopyFileFromHDtoVFS(char* Folder,const char* path, fileslist &guiObj);
double CopyFolderFromHDtoVFS(char * PreFolder,const char* path, inorderlist &list);
double CopyFileFromVFStoHD(char* FileName,char* Folder,char* path);
double CopyFolderFromVFStoHD(char* Folder,char* path);
double CopyFolderinsideFolder(char Folder [],char* path, int indexofMainFolder);
void RemoveFile(char* FileName,char* FolderName);
void RemoveFolder(char Folder []);
long FreeBlocks();
void ListFilesDisplay(inorderlist &list);
double FragmentationPercentage();
double CheckFolderSize(const char * path,int & numberOfFiles,bool &PathLength);
double add_txtFile(string name,string rec,fileslist &Obj);

#endif /* MAINFUNCTIONS_H_ */
