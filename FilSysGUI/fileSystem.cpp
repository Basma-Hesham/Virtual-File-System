/*
 * fileSystem.cpp
 *
 *  Created on: May 22, 2017
 *      Author: rana
 */

#include "fileSystem.h"
#include "fileslist.h"
#include "inorderlist.h"

//Offsets
#define Size 134217728
#define	superBlock 0
#define	fat 1024
#define fbl 656384

//super block Size offset
#define superBlockSizeOff 0
#define fatSizeOff 4
#define fblSizeOff 8
#define filesSizeOff 16
#define extensionSizeoff 24
#define extensionStarteoff 32
#define BlocksLLPSizeOff	40
#define freeBlocksSizeOff 48
#define FirstTimeOff	56
#define BlockSizeOff	64
#define	FilesStartOff 72



fileSystem::fileSystem()
{
    SuperBlock=superBlock;
    FAT=fat;
    FBL=fbl;
    superBlockSizeOffset=superBlockSizeOff;
    fatSizeOffset=fatSizeOff;
    fblSizeOffset=fblSizeOff;
    filesSizeOffset=filesSizeOff;
    ExtensionSizeOffset=extensionSizeoff;
    ExtensionStartOffset=extensionStarteoff;
    BlocksLLPSizeOffset=BlocksLLPSizeOff;
    freeBlocksSizeOffset=freeBlocksSizeOff;
    FirstTimeOffset=FirstTimeOff;
    BlockSizeOffset=BlockSizeOff;
    FilesStartOffset=FilesStartOff;
    size=Size;
    //fileSys.open ("data.bin",ios::out | ios::binary);
    fileSys.open ("data.bin",ios::in | ios::out | ios::binary);

    char buffer[2]="0";
    fileSys.seekp(size);
    fileSys.write (buffer, 1);
    fileSys.close();
    //FatInitialization();
    //SuperBlockInitialization();
    getVariablesFromSuperBlock();
}

void fileSystem::SuperBlockInitialization()
{
    ExtensionStart=size;
    setBlockSize(1024);//by default Block size in 1024
    writeInFAT=1;
    int initsuperBlockSize=48;
    int initSize=0;
    int initBlockSize=1024;
    int FBLSize=sizeof(long)+Blocks_LastLogicalPart;
    cout<<"From SuperBlock Blocks_LastLogicalPart : "<<Blocks_LastLogicalPart;
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp((superBlockSizeOffset));
    outFile.write((char*)&initsuperBlockSize, sizeof(initsuperBlockSize));
    outFile.seekp((fatSizeOffset));
    outFile.write((char*)&initSize, sizeof(initSize));
    outFile.seekp((fblSizeOffset));
    outFile.write((char*)&FBLSize, sizeof(FBLSize));
    outFile.seekp((filesSizeOffset));
    outFile.write((char*)&initSize, sizeof(initSize));
    outFile.seekp((ExtensionSizeOffset));
    outFile.write((char*)&initSize, sizeof(initSize));
    outFile.seekp((ExtensionStartOffset));
    outFile.write((char*)&ExtensionStart, sizeof(ExtensionStart));
    outFile.seekp((BlocksLLPSizeOffset));
    outFile.write((char*)&Blocks_LastLogicalPart, sizeof(Blocks_LastLogicalPart));
    outFile.seekp((freeBlocksSizeOffset));
    outFile.write((char*)&initSize, sizeof(Size));
    outFile.seekp((BlockSizeOffset));
    outFile.write((char*)&initBlockSize, sizeof(initBlockSize));
    outFile.seekp((FilesStartOffset));
    outFile.write((char*)&Files, sizeof(Files));
    outFile.close();
}

//this functions sets the offset of all the dynamic logical part
void  fileSystem::setOffsets(int setBlockSize)
{
    cout<<"from setOffset"<<endl;
    cout<<"setBlockSize : "<<setBlockSize<<endl;
    if(setBlockSize==256)
    {
        Files=1188864;
//		FBLSize=320*1024;
    }
    else if(setBlockSize==512)
    {
        Files=932864;
//		FBLSize=270*1024;
    }
    else if(setBlockSize==1024)
    {
        Files=799744;
//		FBLSize=140*1024;
    }
    else if(setBlockSize==2048)
    {
        Files=727040;
//		FBLSize=69*1024;
    }

    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp((FilesStartOffset));
    outFile.write((char*)&Files, sizeof(Files));
    outFile.close();
    cout<<"Files Start : "<<Files<<endl;
}

void fileSystem::getVariablesFromSuperBlock()
{
    cout<<endl<<"From  getVariablesFromSuperBlock "<<endl;
    ExtensionStart=retExtensionStart();
    cout<<"ExtensionStart: "<<ExtensionStart<<endl;
    //get FBLSize and Files start
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg((fblSizeOffset));
    inFile.read((char*)&FBLSize, sizeof(FBLSize));
    inFile.seekg((FilesStartOffset));
    inFile.read((char*)&Files, sizeof(Files));
    inFile.close();
    Blocks_LastLogicalPart=retBlocksLastLogicalPart();
    cout<<"Blocks_LastLogicalPart "<<Blocks_LastLogicalPart<<endl;
    BlockSize=retBlockSize();
    cout<<"Block Size : "<<BlockSize<<" Files start : "<<Files<<" FBLSize : "<<FBLSize<<endl;
    calculateFB();
}

void fileSystem::setBlocksLastLogicalPart(long NewBlocksLLP)
{
    Blocks_LastLogicalPart=NewBlocksLLP;
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp((BlocksLLPSizeOffset));
    outFile.write((char*)&Blocks_LastLogicalPart, sizeof(Blocks_LastLogicalPart));
    outFile.close();
    cout<<"from setBlocksLastLogicalPart, Blocks_LastLogicalPart : "<<Blocks_LastLogicalPart<<endl;

}
int fileSystem::retBlocksLastLogicalPart()
{
    int returnValue;
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg((BlocksLLPSizeOffset));
    inFile.read((char*)&Blocks_LastLogicalPart, sizeof(Blocks_LastLogicalPart));
    inFile.close();
    returnValue=Blocks_LastLogicalPart;
    cout<<"retBlocksLastLogicalPart, Blocks_LastLogicalPart : "<<Blocks_LastLogicalPart<<endl;
    return returnValue;
}

void fileSystem::DisplaySuperBlock()
{
    cout<<"ExtensionStart: "<<ExtensionStart<<endl;
    int DataObj;
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg(superBlockSizeOffset);
    inFile.read((char*)&DataObj, sizeof(DataObj));
    cout<<"Super Block Size : "<<DataObj;
    inFile.seekg(fatSizeOffset);
    inFile.read((char*)&DataObj, sizeof(DataObj));
    cout<<" FAT Size : "<<DataObj;
    inFile.seekg(fblSizeOffset);
    inFile.read((char*)&DataObj, sizeof(DataObj));
    cout<<" FBL Size : "<<DataObj;
    inFile.seekg(filesSizeOffset);
    inFile.read((char*)&DataObj, sizeof(DataObj));
    cout<<" files Size : "<<DataObj;
    inFile.seekg(ExtensionSizeOffset);
    inFile.read((char*)&DataObj, sizeof(DataObj));
    cout<<" Extension Size : "<<DataObj;
    inFile.seekg(ExtensionStartOffset);
    inFile.read((char*)&DataObj, sizeof(DataObj));
    cout<<" Extension start : "<<DataObj;
    inFile.seekg((BlocksLLPSizeOffset));
    inFile.read((char*)&Blocks_LastLogicalPart, sizeof(Blocks_LastLogicalPart));
    cout<<"Blocks_LastLogicalPart : "<<Blocks_LastLogicalPart;
    inFile.seekg(freeBlocksSizeOffset);
    inFile.read((char*)&DataObj, sizeof(DataObj));
    cout<<" freeBlocks Size : "<<DataObj;
    inFile.seekg(BlockSizeOffset);
    inFile.read((char*)&DataObj, sizeof(DataObj));
    cout<<" BlockSize Size : "<<DataObj<<endl;
    inFile.seekg(FilesStartOffset);
    inFile.read((char*)&DataObj, sizeof(DataObj));
    cout<<" FilesStartOffset : "<<DataObj<<endl;
    inFile.close();
}


void fileSystem::readFreeBlocks(long &FreeBlocks)
{
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg(freeBlocksSizeOffset);
    inFile.read((char*)&FreeBlocks, sizeof(FreeBlocks));
    inFile.close();
}

bool fileSystem:: isFirstTime()
{
    int FirstTimeLaunch; //0 for never been set, 1 for it has been opened
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg(FirstTimeOff);
    inFile.read((char*)&FirstTimeLaunch, sizeof(FirstTimeLaunch));
    inFile.close();
    if (FirstTimeLaunch ==1)
    {
        return false;

    }
    else
    {
        FirstTimeLaunch=1;
        fstream outFile;
        outFile.open("data.bin",ios::in|ios::out| ios::binary);
        outFile.seekp(FirstTimeOff);
        outFile.write((char*)&FirstTimeLaunch, sizeof(FirstTimeLaunch));
        outFile.close();
        return true;
    }

}
void fileSystem::setBlockSize(int NewBlockSize)
{
    BlockSize=NewBlockSize;
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp((BlockSizeOffset));
    outFile.write((char*)&BlockSize, sizeof(BlockSize));
    outFile.close();
    setOffsets(NewBlockSize);
    Blocks_LastLogicalPart=TotalFilesBlocks();
    setBlocksLastLogicalPart(Blocks_LastLogicalPart);
    FBLSize=sizeof(long)+Blocks_LastLogicalPart;
    fstream outFile1;
    outFile1.open("data.bin",ios::in|ios::out| ios::binary);
    outFile1.seekp((fblSizeOffset));
    outFile1.write((char*)&FBLSize, sizeof(FBLSize));
    outFile1.close();
    FBLInitialization();
}

int fileSystem::retBlockSize()
{
    int returnValue;
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg((BlockSizeOffset));
    inFile.read((char*)&BlockSize, sizeof(BlockSize));
    inFile.close();
    return BlockSize;
}
void fileSystem::writeData (int BlockNumber,char*Data,int Addedsize)
{
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp((Files+BlockNumber*BlockSize));
    outFile.write(Data, Addedsize);
    outFile.close();
    //DataStr temp;
    //DataObj=temp;
    //update super Block
    increaseFilesSize (Addedsize);
}
void fileSystem::readData (int BlockNumber,char*&Data,int addedsize)
{
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg(Files+BlockNumber*BlockSize);
    inFile.read(Data, addedsize);
    inFile.close();
}
long fileSystem::TotalFilesBlocks()
{	cout<<"size: "<<size<<" Files : "<<Files<<" ExtensionStart : "<<ExtensionStart<<" (size-ExtensionStart) : "<<(size-ExtensionStart)<<" BlockSize: "<<BlockSize<<endl;
    return floor((size-Files-(size-ExtensionStart))/BlockSize) ;
}
long fileSystem::FilesFreeBlocks()
{
    long FilesFreeBlocks;
    int FilesSize;
    readFilesSize(FilesSize);
    FilesFreeBlocks=ceil( ((size-Files)/(BlockSize*1.00)) )-ceil( FilesSize/(BlockSize*1.00) );
    return FilesFreeBlocks;
}
void fileSystem::readFilesSize(int &FilesSize)
{
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg(filesSizeOffset);
    inFile.read((char*)&FilesSize, sizeof(FilesSize));
    inFile.close();

}
void fileSystem::increaseFilesSize (int AddedFileSize)
{
    int filesSize;
    readFilesSize(filesSize);
    cout<<"from increaseFilesSize files size Before : "<<filesSize<<endl;
    filesSize=filesSize+(ceil(AddedFileSize/(BlockSize*1.00)) ) * BlockSize ;
    cout<<"from increaseFilesSize files size After : "<<filesSize<<endl;
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp((filesSizeOffset));
    outFile.write((char*)&filesSize, sizeof(filesSize));
    outFile.close();
}


void fileSystem::decreaseFilesSize ()
{
    int filesSize;
    readFilesSize(filesSize);
    filesSize-=BlockSize;
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp((filesSizeOffset));
    outFile.write((char*)&filesSize, sizeof(filesSize));
    outFile.close();
}
bool fileSystem::CheckFileSize(double CopiedFileSize,int &noOfBlocks,int ExtensionSizeNeeded)
{
    int filesSize;
    int FilesTotalSize=size-Files-(size-ExtensionStart);//edit in case of FBL extension
    cout<<"ExtensionStart : "<<ExtensionStart<<endl;
    readFilesSize(filesSize);
    cout<<"FilesTotalSize : "<<FilesTotalSize<<" filesSize:  "<<filesSize<<" CopiedFileSize: "<<CopiedFileSize<<endl;
    int RequiredBlocks=ceil( (  (ceil(  (CopiedFileSize)/(1.00*( retBlockSize() ))   )  )*(retBlockSize())
            +(ceil(  (ExtensionSizeNeeded)/(1.00*( retBlockSize() ))   )  )*(retBlockSize())  )   / (1.00*retBlockSize())  );

    cout<<"ExtensionSizeNeeded : "<<(ceil(  (ExtensionSizeNeeded)/(1.00*( retBlockSize() ))   )  )*(retBlockSize())<<endl;
    cout<<"FilesFreeBlocks : "<<FilesFreeBlocks()<<"RequiredBlocks : "<<RequiredBlocks<<endl;
    if((FilesFreeBlocks())<(RequiredBlocks))
    {
        cout<<"No space in Last Logical Part, try MoveDown"<<endl;
        cout<<"ExtensionStart : "<<ExtensionStart<<endl;
        cout<<"size : "<<size<<endl;
        if(ExtensionStart!=size)
        {
            MoveDown();
            FilesTotalSize=size-Files-(size-ExtensionStart);
            if((FilesTotalSize-filesSize)<(CopiedFileSize+ExtensionSizeNeeded))
            {
                cout<<"No space in Files"<<endl;
                return false;
            }
            else
            {
                if(ReservationNeed==1)
                {
                    ReserveExtension(ceil(ExtensionSizeNeeded/(BlockSize*1.00)));
                }
                noOfBlocks=ceil(CopiedFileSize/BlockSize);
                return true;
            }
        }
        else
        {
            cout<<"No space in Files"<<endl;
            return false;
        }

    }
    else
    {
        if(ReservationNeed==1)
        {
            ReserveExtension(ceil(ExtensionSizeNeeded/(BlockSize*1.00)));
        }
        noOfBlocks=ceil(CopiedFileSize/BlockSize);
        return true;

    }
}
////////////////////FAT////////////

void fileSystem:: FatInitialization()
{
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    char buffer[2]="/";
    for(long int i =fat;i<fbl;i++)
    {
        outFile.seekp(i);
        outFile.write(buffer,1);
    }
    outFile.close();
}

bool fileSystem::CheckFATSize(int NumberofItems,int &ExtensionSizeNeeded)
{
    int FatSize;
    int FatOccupied=0;int ExtensionOccupied=0; //total occupied size
    readFATSize(FatSize);
    FatOccupied=FatSize;
    readExtensionSize(FatSize);
    ExtensionOccupied=FatSize;
    cout<<" FATOccupied "<<FatOccupied << " ExtensionOccupied : "<<ExtensionOccupied;
    int FatFreeSpace=(FBL-FAT)-FatOccupied;
    int ExtensionFreeSpace=(size-ExtensionStart)-ExtensionOccupied;
    int noOfBlocks;
    if (FatFreeSpace<(sizeof(fileslist)*NumberofItems))
    {
        cout<<"Fat is full. Use extension"<<endl;
        int NewNumberofItems= NumberofItems-  (floor(  FatFreeSpace/(1.00*sizeof(fileslist))) ); //needed from Extension
        if (ExtensionFreeSpace<(sizeof(fileslist)*NewNumberofItems))
        {
            cout<<"no space in Extension. we need to reserve Extension"<<endl;
            ReservationNeed=1;
            ExtensionSizeNeeded=sizeof(fileslist)*NewNumberofItems;
            cout<<"ExtensionSizeNeeded : "<<ExtensionSizeNeeded<<endl;
            return true;
        }
        else
        {
            writeInFAT=0;
            ReservationNeed=0;
            ExtensionSizeNeeded=0;
            cout<<"There's space in Extension"<<endl;
            return true;
        }

    }
    else
    {
        writeInFAT=1;
        ReservationNeed=0;
        ExtensionSizeNeeded=0;
        cout<<"There's space in Fat"<<endl;
        return true;
    }

}

bool fileSystem::ReserveExtension(int noOfBlocks)
{
    long NewBlocksLLP, NewExtensionStart;
    cout<<"Files gave me totalBlocks: "<<noOfBlocks<<endl;
    NewBlocksLLP=Blocks_LastLogicalPart-noOfBlocks;
    setBlocksLastLogicalPart(NewBlocksLLP);
    cout<<" Blocks_LastLogicalPart : "<<Blocks_LastLogicalPart;
    NewExtensionStart = ExtensionStart- (noOfBlocks*BlockSize);
    setExtensionStart(NewExtensionStart);
    cout<<" ExtensionStart: "<<ExtensionStart<<endl;
    ExtensionInitialization(noOfBlocks);
    writeInFAT=0;
    cout<<"Reservation Done"<<endl;
    return true;
}

bool fileSystem::FATBlocksShift(long start, long end ,long shiftOffset)
{
    cout<<"start : "<<start<<" end : "<<end<<" shiftOffset : "<<shiftOffset<<endl;
    fileslist OldRecord, NewRecord;
    long StartRange=start;
    while(StartRange<=end)
    {
        search_recordbyStart(StartRange,OldRecord);
        cout<<"search start "<<StartRange<<endl;
        cout<<"old Record : ";
        OldRecord.showData();
        int NewStartRange=OldRecord.retFirstBlock()-shiftOffset;
        NewRecord.setData(OldRecord.retName(),OldRecord.retSize(),OldRecord.retDate(),OldRecord.retNumberofblocks(),NewStartRange,OldRecord.retFolder());
        cout<<"NewRecord: ";
        NewRecord.showData();
        delete_record(OldRecord.retName(),OldRecord.retFolder());
        FatWrite(NewRecord);
        cout<<"StartRange before : "<<StartRange<<endl;
        StartRange=StartRange+OldRecord.retNumberofblocks();
        cout<<"StartRange after : "<<StartRange<<endl;
    }
    return true;
}
void fileSystem::write_record(int pos,fileslist &obj)
{
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp(pos);
    outFile.write((char*)&obj, sizeof(obj));
    outFile.close();
}

void fileSystem::read_record(int pos, fileslist &obj)
{
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg(pos);
    inFile.read((char*)&obj, sizeof(obj));
    inFile.close();
}
void fileSystem::readFATSize(int &FilesSize)
{
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg(fatSizeOffset);
    inFile.read((char*)&FilesSize, sizeof(FilesSize));
    inFile.close();

}
void fileSystem::increaseFATSize ()
{
    int filesSize;
    readFATSize(filesSize);
    filesSize+=sizeof(fileslist);
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp((fatSizeOffset));
    outFile.write((char*)&filesSize, sizeof(filesSize));
    outFile.close();
}
void fileSystem::decreaseFATSize ()
{
    int filesSize;
    readFATSize(filesSize);
    filesSize-=sizeof(fileslist);
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp((fatSizeOffset));
    outFile.write((char*)&filesSize, sizeof(filesSize));
    outFile.close();
}
void fileSystem::Fatdisplay(int pos)
{
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg(pos);
    fileslist obj;
    inFile.read((char*)&obj, sizeof(obj));
    obj.showData();
    inFile.close();
}

void fileSystem::MoveDown()
{
    int start;
    bool First_Slash=false,First_non_Slash=false;
    int First_Slash_index=size;int First_non_Slash_index=size;int Second_Slash_index=size;
    fileslist record; //this will save first letter to check either it's slash or not
    char *T="/";
    start = retExtensionStart();
    int File_end=size;
    fileslist object;
    int object_size=sizeof(object);
    bool done=true;
    int i;
    while(done)
    {
        //looping over objects in extension to find first slash, first non slash, second slash
        for(i=File_end-object_size;i>start;i-=object_size)
        {
            read_record(i,record);
            //if char is a slash then empty record
            if(record.retName()[0]=='/')
            {
                //first empty record
                if (!First_Slash)
                {
                    First_Slash=true;
                    First_Slash_index=i;
                }
                else if(First_Slash && First_non_Slash)//second empty slash after the first non slash
                {
                    Second_Slash_index=i;
                    break;//no need for more looping over objects
                }
            }
            else //if anything other than slash - real letters
            {
                if(First_Slash && !First_non_Slash)
                {
                    First_non_Slash=true;
                    First_non_Slash_index=i;
                }

            }
        }
        //if we reached the end of extension and second slash not found
        if(First_Slash &&First_non_Slash&&i==start)
        {
            Second_Slash_index=i;
        }
        //this condition means extension is empty, then no need for looping - probably won't apply here
        if( !((First_Slash_index>First_non_Slash_index )&&(First_non_Slash_index>Second_Slash_index)) || (Second_Slash_index==File_end) || (First_non_Slash_index==File_end) )
        {
            done=false;
            break;
        }

        for(int i=First_non_Slash_index;i>Second_Slash_index;i=i-object_size)
        {
            read_record(i,record);
            delete_record(record.retName(),record.retFolder());
            FatWrite(record);
        }
        First_Slash_index=File_end;First_non_Slash_index=File_end;Second_Slash_index=File_end;//re-initializing
        First_Slash=false;
        First_non_Slash=false;
    }
    //after this there must be no empty space in Extension So decrease Extension start to be exactly as Extension size
    //to change BlocksLLP, Extension Start
    cout<<endl<<endl<<"After Move Down"<<endl;
    int ExtenSize;
    readExtensionSize(ExtenSize);
    long NewExtensionStart = floor( (size-ExtenSize)/(BlockSize*1.00) ) *BlockSize ;
    setExtensionStart(NewExtensionStart);
    cout<<" ExtensionStart: "<<ExtensionStart<<endl;
    long NewBlocksLLP=TotalFilesBlocks();
    setBlocksLastLogicalPart(NewBlocksLLP);
    cout<<" Blocks_LastLogicalPart : "<<Blocks_LastLogicalPart;
}

void fileSystem:: FatWrite(fileslist FileListObject)
{
    fstream File;
    File.open ("data.bin",ios::in |ios::out | ios::binary);
    fileslist LoopObject;
    cout<<"FAT write "<<writeInFAT<<endl;
    if(writeInFAT==1)
    {
        for(int i=FAT;i<FBL;i=i+sizeof(LoopObject))
        {
            read_record(i,LoopObject);
            if(LoopObject.retName()[0]=='/')
            {
                write_record(i,FileListObject);
                Fatdisplay(i);
                increaseFATSize();
                File.close();
                return;
            }
        }
    }
    else
    {
        cout<<"Fat write,no space in FAt, use extension"<<endl;
        for(int i=size-sizeof(LoopObject);i>ExtensionStart;i=i-sizeof(LoopObject))
        {
            read_record(i,LoopObject);
            if(LoopObject.retName()[0]=='/')
            {
                write_record(i,FileListObject);
                Fatdisplay(i);
                increaseExtensionSize();
                File.close();
                return;
            }
        }
    }
    File.close();
}

bool fileSystem::search_recordbyname(char Name [],fileslist &searchObject)
{
    fstream File;
    File.open ("data.bin",ios::in |ios::out | ios::binary);
    fileslist LoopObject;
    for(int i=fat;i<fbl;i=i+sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if (strcmp(LoopObject.retName(),Name) == 0)
        {
            searchObject=LoopObject;
            File.close();
            return true;
        }
    }
    cout<<"search by name, not found in fat, use extension"<<endl;
    for(int i=size-sizeof(LoopObject);i>ExtensionStart;i=i-sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if (strcmp(LoopObject.retName(),Name) == 0)
        {
            searchObject=LoopObject;
            File.close();
            return true;
        }
    }

    File.close();
    return false;
}
bool fileSystem::search_recordbyfolder(char Folder [],inorderlist &List)
{
    cout<<"Search by folder"<<endl;
    fstream File;
    int exist=0;
    File.open ("data.bin",ios::in |ios::out | ios::binary);
    fileslist LoopObject;
    for(int i=fat;i<fbl;i=i+sizeof(LoopObject))
    {
        read_record(i,LoopObject);
//		cout<<"LoopObject.retFolder() : "<<LoopObject.retFolder()<<" Folder : "<<Folder<<endl;
        if ((strcmp(LoopObject.retFolder(),Folder) == 0))
        {
    //		cout<<"matched"<<endl;
            if(LoopObject.retName()[0]!='/')
            {
        //		cout<<"File name is not /"<<endl;
                exist=1;
                List.insert(LoopObject);
            }
        }
    }
    cout<<"search by Folder, not found in fat, use extension"<<endl;
    for(int i=size-sizeof(LoopObject);i>ExtensionStart;i=i-sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if ((strcmp(LoopObject.retFolder(),Folder) == 0))
        {
            if(LoopObject.retName()[0]!='/')
            {
                exist=1;
                List.insert(LoopObject);
            }
        }
    }
    File.close();
    if(exist==1)
    {
        return true;
    }
    else
        return false;
}

bool fileSystem::search_recordbyFirstfolder(char Folder [],inorderlist &List)
{
    cout<<"Search by first folder"<<endl;
    int exist=0;
    fileslist LoopObject;
    for(int i=FAT;i<FBL;i=i+sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if(LoopObject.retName()[0]!='/')
        {
            //obtaining first folder name
            const char* FirstFolder1;
            char FirstFolderChar1[255];
            string FolderStr1=LoopObject.retFolder();
            string FirstFolderStr1;
            //finding slash to find first name
            std::size_t Lastfound1;
            std::size_t found1 = FolderStr1.find('/');
            if(found1!=std::string::npos)
            {
                FirstFolderStr1=FolderStr1.substr(0,found1);	//starting form the / mark till the end of the string
                const char *cstr1 = FirstFolderStr1.c_str();
                strcpy(FirstFolderChar1, cstr1);
            }
            else
            {
                FirstFolder1=FolderStr1.c_str();
                strcpy(FirstFolderChar1,FirstFolder1);
            }

            cout<<"First Folder Char "<<FirstFolderChar1<<endl;
            if ((strcmp(FirstFolderChar1,Folder) == 0))
            {
                exist=1;
                List.insert(LoopObject);
            }
        }
    }
    cout<<"search by Folder, use extension"<<endl;
    cout<<"ExtensionStart : "<<ExtensionStart<<endl;
    cout<<"sizeof(LoopObject) "<<sizeof(LoopObject)<<endl;
    for(int i=size-sizeof(LoopObject);i>ExtensionStart;i=i-sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if(LoopObject.retName()[0]!='/')
        {
            //obtaining first folder name
            const char* FirstFolder;
            char FirstFolderChar[255];
            string FolderStr=LoopObject.retFolder();
            string FirstFolderStr;
    //		cout<<"LoopObject.retFolder() : "<<LoopObject.retFolder()<<" Folder : "<<Folder<<endl;
            //finding slash to find first name
            std::size_t Lastfound;
            std::size_t found = FolderStr.find('/');
            if(found!=std::string::npos)
            {
                FirstFolderStr=FolderStr.substr(0,found);	//starting form the / mark till the end of the string
                const char *cstr = FirstFolderStr.c_str();
                strcpy(FirstFolderChar, cstr);
            }
            else
            {
                FirstFolder=FolderStr.c_str();
                strcpy(FirstFolderChar,FirstFolder);
            }
            cout<<"First Folder Char "<<FirstFolderChar<<endl;

            if ((strcmp(FirstFolderChar,Folder) == 0))
            {
                exist=1;
                List.insert(LoopObject);
            }
        }
    }

    if(exist==1)
    {
        return true;
    }
    else
        return false;
}

bool fileSystem::search_recordbyStart(int start,fileslist &searchObject)
{
    fstream File;
    File.open ("data.bin",ios::in |ios::out | ios::binary);
    fileslist LoopObject;
    int i;
    for(i=fat;i<fbl;i=i+sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if(LoopObject.retName()[0]!='/')
        {
            if (LoopObject.retFirstBlock() ==start)
            {
                searchObject=LoopObject;
                File.close();
                return true;
            }
        }
    }
    cout<<"search by block, not found in fat, use extension"<<endl;
    //same loop for extension
    for(int i=size-sizeof(LoopObject);i>ExtensionStart;i=i-sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if(LoopObject.retName()[0]!='/')
        {
            if (LoopObject.retFirstBlock() ==start)
            {
                searchObject=LoopObject;
                File.close();
                return true;
            }
        }
    }
    File.close();
    return false;
}

bool fileSystem::seach_recordbyFirstChars(char Folder [],inorderlist &List)
{
    string FolderNameStr=Folder;
    int FolderLength=FolderNameStr.length();
    cout<<"Search by FirstChars in folder"<<endl;
    fstream File;
    int exist=0;
    File.open ("data.bin",ios::in |ios::out | ios::binary);
    fileslist LoopObject;
    for(int i=fat;i<fbl;i=i+sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if(LoopObject.retName()[0]!='/')
        {
            string LoopFolder=LoopObject.retFolder();
            if(LoopFolder.length()>=FolderLength)
            {
                string SubFolderName= LoopFolder.substr(0,FolderLength);
                if(FolderNameStr==SubFolderName)
                {
                    if( (LoopFolder.length()>FolderLength))
                    {
                        if((LoopFolder[FolderLength]=='/'))
                        {
                            exist=1;
                            cout<<endl<<endl<<"Object is added";
                            LoopObject.showData();
                            List.insert(LoopObject);
                        }

                    }
                    else
                    {
                        exist=1;
                        cout<<endl<<endl<<"Object is added";
                        LoopObject.showData();
                        List.insert(LoopObject);
                    }

                }
            }
        }
    }
    cout<<"search by Folder, not found in fat, use extension"<<endl;
    for(int i=size-sizeof(LoopObject);i>ExtensionStart;i=i-sizeof(LoopObject))
    {
        read_record(i,LoopObject);
            if(LoopObject.retName()[0]!='/')
            {
                string LoopFolder=LoopObject.retFolder();
                    if(LoopFolder.length()>=FolderLength)
                    {
                        string SubFolderName= LoopFolder.substr(0,FolderLength);
                        if(FolderNameStr==SubFolderName)
                        {
                            if( (LoopFolder.length()>FolderLength))
                            {
                                if((LoopFolder[FolderLength]=='/'))
                                {
                                    exist=1;
                                    cout<<endl<<endl<<"Object is added";
                                    LoopObject.showData();
                                    List.insert(LoopObject);
                                }

                            }
                            else
                            {
                                exist=1;
                                cout<<endl<<endl<<"Object is added";
                                LoopObject.showData();
                                List.insert(LoopObject);
                            }

                        }
                    }
            }
    }
    File.close();
    if(exist==1)
    {
        return true;
    }
    else
        return false;

}

bool fileSystem::search_recordbynameandfolder(char Name [],char Folder[],fileslist &searchObject)
{
    fstream File;
    File.open ("data.bin",ios::in |ios::out | ios::binary);
    fileslist LoopObject;
    for(int i=fat;i<fbl;i=i+sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if(LoopObject.retName()[0]!='/')
        {
            LoopObject.showData();
            if ( ( strcmp(LoopObject.retName(),Name) == 0 )&&(strcmp(LoopObject.retFolder(),Folder) == 0))
            {
                searchObject=LoopObject;
                File.close();
                return true;
            }
        }
    }
    cout<<"search by name and folder, not found in fat, use extension"<<endl;
    for(int i=size-sizeof(LoopObject);i>ExtensionStart;i=i-sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if ( ( strcmp(LoopObject.retName(),Name) == 0 )&& (strcmp(LoopObject.retFolder(),Folder) == 0))
        {
            searchObject=LoopObject;
            File.close();
            return true;
        }
    }

    File.close();
    return false;
}

bool fileSystem::delete_record(char Name [],char Folder [])
{
    cout<<"delete_record"<<endl;
    cout<<"ExtensionStart: "<<ExtensionStart<<endl;
    fstream File;
    File.open ("data.bin",ios::in |ios::out | ios::binary);
    fileslist LoopObject;
    for(int i=fat;i<fbl;i=i+sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if ( (strcmp(LoopObject.retName(),Name) == 0 ) && ( strcmp(LoopObject.retFolder(),Folder) == 0  ) )
        {
            cout<<LoopObject.retName() <<"  deleted"<<endl;
            fileslist deletedObj;
            write_record(i,deletedObj);
            decreaseFATSize ();
            DisplaySuperBlock();
            File.close();
            FatTransfer();
            return true;
        }
    }
    cout<<"delete, not found in fat, use extension"<<endl;
    for(int i=size-sizeof(LoopObject);i>ExtensionStart;i=i-sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if ( (strcmp(LoopObject.retName(),Name) == 0 ) && ( strcmp(LoopObject.retFolder(),Folder) == 0  ) )
        {
            cout<<LoopObject.retName() <<"  deleted"<<endl;
            fileslist deletedObj;
            write_record(i,deletedObj);
            decreaseExtensionSize ();
            File.close();
            return true;
        }
    }
    File.close();
    return false;
}

void fileSystem::FatTransfer()
{
    cout<<"in FatTransfer"<<endl;
    fileslist LoopObject;
    int start=size-(floor((size-ExtensionStart)/(1.00*sizeof(fileslist))) * sizeof(fileslist));
    cout<<"start is "<<size-(floor((size-ExtensionStart)/(1.00*sizeof(fileslist))) * sizeof(fileslist))<<endl;
    for(int i= start;i<size;i=i+sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if(LoopObject.retName()[0]!='/')
        {
            cout<<"found "<<LoopObject.retName()<< " deleted from Extension and written in FAT"<<endl;
            delete_record(LoopObject.retName(),LoopObject.retFolder());
            writeInFAT=1;
            FatWrite(LoopObject);
            break;
        }
    }
}

void fileSystem::ListFiles(inorderlist &List)
{

    fstream File;
    File.open ("data.bin",ios::in |ios::out | ios::binary);
    fileslist LoopObject;
    for(int i=fat;i<fbl;i=i+sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if(LoopObject.retName()[0]!='/')
        {
         List.insert(LoopObject);
        }

    }
    cout<<"List files,  use extension"<<endl;
    for(int i=size-sizeof(LoopObject);i>ExtensionStart;i=i-sizeof(LoopObject))
    {
        read_record(i,LoopObject);
        if(LoopObject.retName()[0]!='/')
        {
         List.insert(LoopObject);
        }

    }
        File.close();
}

///////////////////Extension//////////////////
void fileSystem:: ExtensionInitialization(int noOfBlocks)
{
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    char buffer[2]="/";
    for(long int i =ExtensionStart;i<ExtensionStart+(noOfBlocks*BlockSize);i++)
    {
        outFile.seekp(i);
        outFile.write(buffer,1);
    }
    outFile.close();
}
long fileSystem::retExtensionStart()
{
    fstream inFile;
    int Extension;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg(ExtensionStartOffset);
    inFile.read((char*)&Extension, sizeof(Extension));
    inFile.close();
    return Extension;
}

void  fileSystem::setExtensionStart(long NewExtensionStart)
{
    ExtensionStart=NewExtensionStart;
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp(ExtensionStartOffset);
    outFile.write((char*)&ExtensionStart, sizeof(ExtensionStart));
    outFile.close();
}


void fileSystem::readExtensionSize(int &FilesSize)
{
    fstream inFile;
    inFile.open("data.bin",ios::in|ios::out| ios::binary);
    inFile.seekg(ExtensionSizeOffset);
    inFile.read((char*)&FilesSize, sizeof(FilesSize));
    inFile.close();
}

void fileSystem::increaseExtensionSize ()
{
    int ExtensionSize;
    readExtensionSize(ExtensionSize);
    ExtensionSize+=sizeof(fileslist);
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp((ExtensionSizeOffset));
    outFile.write((char*)&ExtensionSize, sizeof(ExtensionSize));
    outFile.close();
}


void fileSystem::decreaseExtensionSize ()
{
    int ExtensionSize;
    readExtensionSize(ExtensionSize);
    ExtensionSize-=sizeof(fileslist);
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    outFile.seekp((ExtensionSizeOffset));
    outFile.write((char*)&ExtensionSize, sizeof(ExtensionSize));
    outFile.close();
}

int fileSystem::retReservationNeed()
{
    return ReservationNeed;
}

/////////////////FBL//////////////////
void fileSystem::FBLInitialization()
{
    fstream outFile;
    outFile.open("data.bin",ios::in|ios::out| ios::binary);
    char T='T';
    char F='F';
    for(int i=0;i<Blocks_LastLogicalPart;i++)
    {//Blocks_lastlogicalPart
        outFile.seekp(FBL+sizeof(long)+i,outFile.beg);//4 is the size of long datatype (for freeblocks number)
        outFile.write((char * ) &T,1);
    }
    outFile.close();
}


bool fileSystem::MoveUp()
{

    char T='T';
    char F='F';
    long Blockindex=Blocks_LastLogicalPart;//number of block in last logical part=(totalsize-superblocksize-fatsize-FBLsize)/sizeofBlock
    long FirstT=0,FirstF=0,secondT=0;
    bool flag=false,flagF=false;
    bool done=true;//keep looping and moving up groups of consequent consumed blocks
    char memblock;
    cout<<"Blockindex "<<Blockindex<<endl;
    while(done)
    {
        ///get first true then first false then second true
        long i;
        for(i=0;i<Blockindex;i++)
        {//loop around blocks of files
            fileSys.open ("data.bin",ios::in |ios::out | ios::binary);
            fileSys.seekg(FBL+sizeof(long)+i,fileSys.beg);
            fileSys.read((char * ) &memblock,sizeof(char));
            fileSys.close();

            if(memblock==T )
            {//True
                if(!flag )
                {//first true not found
                    FirstT=i;
                    flag=true;
                }
                else if(flag && flagF)//second true
                {
                        cout<<"i = "<<i<<endl;
                        secondT=i;
                        break;
                }

            }//end of if(finding True Block)
            else
            {//False
                if(flag && !flagF)
                {
                    FirstF=i;
                    flagF=true;
                }

            }
        }//end of looping over all blocks
        if(flag && flagF && i==Blockindex)//second true
        {
            cout<<"Here"<<endl;
            secondT=Blockindex;

        }

        if( !( (FirstT<FirstF) && (FirstF<secondT) ) || secondT==0 || FirstF==0 )
        {
            done=false;
            break;
        }
        //move up in ------------------FBL----------------
        cout<<"FirstT : "<<FirstT<<" secondT : "<<secondT<<" FirstF: "<<FirstF<<endl;
        DataStr DataObj;

        for(int i=FirstF;i<FirstF+(secondT-FirstF);i++)
        {

            //---------------------Free Old Blocks in FBL-----------------------
            fileSys.open ("data.bin",ios::in |ios::out | ios::binary);
            fileSys.seekp(FBL+sizeof(long)+i, fileSys.beg);
            fileSys.write((char * ) &T,1);
            fileSys.close();


            //move up in -----------last logical part-------------
            char *Buffer =new char[retBlockSize()];
            readData(i,Buffer,retBlockSize());
            cout<<"read from "<<i<<endl;
            writeData(i-(FirstF-FirstT),Buffer,retBlockSize());
            cout<<"write in "<<i-(FirstF-FirstT)<<endl;
            delete [] Buffer;
        }
        //move up in ------------------FAT----------------
        FATBlocksShift(FirstF ,(FirstF+(secondT-FirstF)-1),FirstF-FirstT);

        //------------------------reserve new blocks in FBL--------------------

        for(long i=FirstT;i<FirstT+(secondT-FirstF);i++)
        {
            fileSys.open ("data.bin",ios::in |ios::out | ios::binary);
            fileSys.seekp(fbl+sizeof(long)+i,fileSys.beg);
            fileSys.write((char * )&F,1);
            fileSys.close();
        }
        cout<<endl;
        FirstT=0,FirstF=0,secondT=0;//re-initializing
        flag=false;
        flagF=false;
    }//end of while loop
    cout<<"Move Up is done"<<endl;
    return done;

}

/*FreeBlocks in all file system */

void fileSystem::calculateFB()
{
        int filesSize;
        int FilesTotalSize=size-Files-(size-ExtensionStart);//edit in case of FBL extension
        cout<<"ExtensionStart : "<<ExtensionStart<<endl;
        readFilesSize(filesSize);
        cout<<"Files Start : "<<Files<<" FilesTotalSize : "<<FilesTotalSize<<" filesSize:  "<<filesSize<<endl;
        freeblocks=floor((FilesTotalSize-filesSize)/(BlockSize*1.00));
        fileSys.open ("data.bin",ios::in |ios::out | ios::binary);
        //write freeblocks in superblock
        fileSys.seekp(freeBlocksSizeOffset,fileSys.beg);
        fileSys.write((char *)&freeblocks,sizeof(freeblocks));
        fileSys.close();
        cout<<"from calculate FB, freeblocks: "<<freeblocks<<endl;
}

bool fileSystem::getemptyBlocks(int noOfBlocks,int &FirstBlock)
{
    /*
     *
     */
    int *Required_Blocks=new int [noOfBlocks];
    char T='T';
    char F='F';
    long Blockindex=Blocks_LastLogicalPart;//number of block in last logical part=(totalsize-superblocksize-fatsize-FBLsize)/sizeofBlock
    long flag=0,counter=0,index=0,tempindex=0;
    char memblock;
    findagain:
    for(long i=0;i<Blockindex;i++)
    {//loop around blocks of files
        fileSys.open ("data.bin",ios::in |ios::out | ios::binary);
        fileSys.seekg(FBL+sizeof(long)+i,fileSys.beg);
        fileSys.read((char * ) &memblock,sizeof(char));
        fileSys.close();
        if(memblock==T ){//True
            if(counter==0){
                tempindex=i;
            }
            counter++;
        }//end of if(finding True Block)
        else{//False
            if(counter==noOfBlocks){
                flag=counter;
                index=tempindex;
                counter=0;//re-initialize
                break;

            }
            else if(counter>noOfBlocks){
                flag=counter;
                index=tempindex;

            }//end
            counter=0;


        }//false

    }//end of looping on the blocks of the files
    if(counter>=noOfBlocks )
    {
        flag=counter;
        index=tempindex;
        counter=0;//re-initialize
    }//end of (counter>flag)

    ///check condition
    if(flag<noOfBlocks)
    {
        MoveUp();
        goto findagain;
    }

    for(long i=0;i<noOfBlocks;i++)
    {
        Required_Blocks[i]=i+index;//blocks start from zero
        fileSys.open ("data.bin",ios::in |ios::out | ios::binary);
        fileSys.seekp(FBL+sizeof(freeblocks)+i+index,fileSys.beg);
        fileSys.write((char * )&F,1);
        fileSys.close();
    }
    FirstBlock=Required_Blocks[0];
    delete [] Required_Blocks;
    cout<<"getEmptyBlocks is done"<<endl;
    return true;
}
bool fileSystem:: Deleteblocks(int noOfBlocks,int FirstBlock)
{
    char T='T';
    //int Blockindex=Blocks_lastlogicalPart;//number of block in last logical part=(totalsize-superblocksize-fatsize-FBLsize)/sizeofBlock
    //long offset;

    for(long j=0;j<noOfBlocks;j++)
    {
        fileSys.open ("data.bin",ios::in |ios::out | ios::binary);
        fileSys.seekp(FBL+sizeof(long)+FirstBlock+j, fileSys.beg);
        fileSys.write((char * ) &T,1);
        fileSys.close();
    }//end of Required deleted Blocks
    return true;
}

long fileSystem::MaxSeqFreeBlocks(){//// for fragementation calculation
    char T='T';
    long Blockindex=Blocks_LastLogicalPart;//number of block in last logical part=(totalsize-superblocksize-fatsize-FBLsize)/sizeofBlock
    long flag=0,counter=0;
    char memblock ;

    for(long i=0;i<Blockindex;i++){
        fileSys.open ("data.bin",ios::in |ios::out | ios::binary);
        fileSys.seekg (FBL+sizeof(long)+i, fileSys.beg);
        fileSys.read ((char * )&memblock, sizeof(char));
        fileSys.close();
        if(memblock==T ){
            counter++;
        }//end of if(finding True Block)
        else{
            if(counter>flag){
                flag=counter;
            }
            counter=0;
        }//False

    }//end of looping on the blocks
    if(counter>flag){
        flag=counter;
        counter=0;
    }
    return flag;
}

fileSystem::~fileSystem()
{

    // TODO Auto-generated destructor stub
}

