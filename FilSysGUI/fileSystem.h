/*
 * fileSystem.h
 *
 *  Created on: May 22, 2017
 *      Author: rana
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <iostream>
#include <fstream>
#include "inorderlist.h"
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <algorithm>
#include <iterator>
#include <cstring>
#include "fileslist.h"

#ifdef __linux	                  /* __unix__ is usually defined by compilers targeting Unix systems */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <dirent.h>

#endif

#ifdef _WIN32     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */

#include <windows.h>
#include <io.h>//instead of unistd.h
#include <stdio.h>
#include <tchar.h>
#include <time.h>//instead of sys/time.h
#include "dirent_W.h"


#endif


using namespace std;


struct DataStr
{
    char Data[1025];
};

class fileSystem {
public:
    fileSystem();
    void SuperBlockInitialization();
    void setOffsets(int setBlockSize);
    void getVariablesFromSuperBlock();
    void DisplaySuperBlock();
    void setBlocksLastLogicalPart(long NewBlocksLLP);
    int retBlocksLastLogicalPart();
    void readFreeBlocks(long &FreeBlocks);
    bool isFirstTime();
    void setBlockSize(int NewBlockSize);
    int retBlockSize();
/////////////Files Functions///////////////
    void writeData (int BlockNumber,char*Data,int Addedsize);
    void readData (int BlockNumber,char*&Data,int addedsize);
    void readFilesSize(int &FilesSize);
    long TotalFilesBlocks();
    long FilesFreeBlocks();
    void increaseFilesSize (int AddedFileSize);
    void decreaseFilesSize ();
    bool CheckFileSize(double CopiedFileSize,int &noOfBlocks,int ExtensionSizeNeeded);
////////////////FAT Functions/////////////////
    void FatInitialization();
    void Fatdisplay(int pos);
    bool FATBlocksShift(long start, long end ,long shiftOffset);
    void write_record(int pos,fileslist &obj);
    void FatWrite(fileslist FileListObject);
    void read_record(int pos, fileslist &obj);
    void readFATSize(int &FilesSize);
    void increaseFATSize ();
    void decreaseFATSize ();
    bool search_recordbyname(char Name [],fileslist &searchObject);
    bool search_recordbyfolder(char Folder [],inorderlist &List);
    bool search_recordbyFirstfolder(char Folder [],inorderlist &List);
    bool search_recordbynameandfolder(char Name [],char Folder [],fileslist &searchObject);
    bool search_recordbyStart(int start,fileslist &searchObject);
    bool seach_recordbyFirstChars(char Folder [],inorderlist &List);
    bool delete_record(char Name [],char Folder []);
    void FatTransfer();
    void ListFiles(inorderlist &List);
    void MoveDown();
    ////////////////Extension Functions///////////
    void ExtensionInitialization(int noOfBlocks);
    long retExtensionStart();
    void setExtensionStart(long NewExtensionStart);
    bool CheckFATSize(int NumberofItems,int &ExtensionSizeNeeded);
    bool ReserveExtension(int noOfBlocks);
    void readExtensionSize(int &FilesSize);
    void increaseExtensionSize ();
    void decreaseExtensionSize ();
    int retReservationNeed();
    ////////////////////////-----------------FBL Function----------------//////////////////////
    void FBLInitialization();
    bool MoveUp();
    void calculate_Size(long offset1,long BlockTotalSize,long &t);//calculate size of the FBL
    void calculateFB();//calculate total freeblocks
    bool getemptyBlocks(int noOfBlocks,int &FirstBlock);//for Add and copy
    bool Deleteblocks(int noOfBlocks,int FirstBlock);//for Delete function
    long MaxSeqFreeBlocks();// for fragmentation
    virtual ~fileSystem();

private:
    fstream fileSys;
    long size;
    long BlockSize;
    long SuperBlock;
    long FAT;
    long FBL;	//starting offset of FBL
    long Files;
    long superBlockSizeOffset;
    long fatSizeOffset;
    long fblSizeOffset;	//total size index in super block
    long FBLSize;	//dynamic which is set depending on Block Size
    long filesSizeOffset;
    long Blocks_LastLogicalPart;
    long ExtensionSizeOffset;
    long ExtensionStartOffset;
    long BlocksLLPSizeOffset;
    long freeBlocksSizeOffset;
    long FirstTimeOffset;
    long BlockSizeOffset;
    long ExtensionStart;
    long FilesStartOffset;	//this where Files start is going to be saved
    int writeInFAT;
    int ReservationNeed;
    int FilesListSize;
    /////------------------------FBL variables----------------------------///////
    //long total_size;//total size of file system
    long fat_size;//FAT size
    long freeblocks;//Free blocks size in the actual block not the block containing the size
    long fbl_blocksize;//the FBL Block , the block containing the total free size
    long actual_size;//actual occupied blocks size in tha last logical part
    long descriptionSize;

};

#endif /* FILESYSTEM_H_ */
