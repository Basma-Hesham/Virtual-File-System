/*
 * mainFunctions.cpp
 *
 *  Created on: Jun 28, 2017
 *      Author: rana
 */


#include "mainFunctions.h"

fileSystem VFS;

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void renameFile(char * FileName,char * Folder)
{
    fileslist obj;
    string FileNameStr=FileName;
    for (int i=1;;i++)
    {
        string addedChar="(";
        std::ostringstream ss;
        ss << i;
        addedChar+=ss.str();
        addedChar+=")";
        cout<<addedChar<<endl;
        //prepare new name
        std::size_t Lastfound;
        std::size_t found = FileNameStr.find('.');
        if(found!=std::string::npos)
        {
            while (found!=std::string::npos)
            {
                Lastfound=found;
                found=FileNameStr.find(".",found+1,1);
            }
            string NewFileNameStr=FileNameStr.substr(0,Lastfound);
            cout<<NewFileNameStr<<endl;
            NewFileNameStr+=addedChar;
            cout<<NewFileNameStr<<endl;
            NewFileNameStr+=FileNameStr.substr(Lastfound,string::npos);	//starting form the / mark till the end of the string
            cout<<NewFileNameStr<<endl;
            const char *NewName = NewFileNameStr.c_str();
            char NewNameChar[255];
            strcpy(NewNameChar,NewName);
            if(!(VFS.search_recordbynameandfolder(NewNameChar,Folder,obj)))
            {
                strcpy(FileName,NewNameChar);
                break;
            }
        }
        else
        {
            cout<<"this file name is not allowed without . "<<endl;
        }
    }
}

double CopyFileFromHDtoVFS(char* Folder,const char* path, fileslist &guiObj)
{
    //checking size
    string PathWarning=path;
    string FolderWarning=Folder;
    if(PathWarning.length()>=255 )
    {
        cout<<"Path length is too long"<<endl;
        return -2;

    }

    #ifdef _WIN32
        clock_t start=clock();
    #endif
    #ifdef __linux
        struct timeval start, end;
        gettimeofday(&start, NULL);

    #endif

    double mtime, seconds, useconds;

    fstream CopiedFile;
    string FileNameStr,FolderStr;
    char FileName[255];char Date[30];
    int noOfBlocks=0;int FirstBlock=0;

    double BytesLeft;
    CopiedFile.open(path,ios::in| ios::binary);
    double CopiedFileSize=filesize(path);
    BytesLeft=CopiedFileSize;
    stringstream ostrm;
    string s;
    ostrm << CopiedFile.rdbuf();
    s=ostrm.str();


    char *p =new char [s.size()];
    for(int i=0;i<s.size();i++)
    {
        p[i]=s[i];
    }
    int ExtensionSizeNeeded;
    if(VFS.CheckFATSize(1,ExtensionSizeNeeded))
    {
        if(VFS.CheckFileSize(CopiedFileSize,noOfBlocks,ExtensionSizeNeeded))
        {
            if(CopiedFileSize!=0)
            {
                cout<<"noOfBlocks needed"<<noOfBlocks<<endl;
                VFS.getemptyBlocks(noOfBlocks,FirstBlock);
                cout<<"used Blocks starts from : "<<FirstBlock;
                cout<<endl;
                if (CopiedFile.is_open())
                {
                    VFS.writeData(FirstBlock,p,s.size());
                    CopiedFile.close();
                }
            }
            delete [] p;

            //Send data to FAT
            //get file name
            string Path=path;
            std::size_t Lastfound;
            std::size_t found = Path.find('/');
            if(found!=std::string::npos)
            {
                while (found!=std::string::npos)
                {
                    Lastfound=found;
                    found=Path.find("/",found+1,1);
                }
                 FileNameStr=Path.substr(Lastfound+1,string::npos);	//starting form the / mark till the end of the string
                 const char *cstr = FileNameStr.c_str();
                 strcpy(FileName, cstr);
            }
            else
            {
                const char *cstr = Path.c_str();
                strcpy(FileName, cstr);
            }


            //To get Date
            // current date/time based on current system
            time_t now = time(0);
            // convert now to string form
            char* dt = ctime(&now);
            strcpy(Date,dt);

            //handle repeated name
            cout<<"FileName Before : "<<FileName<<endl;
            fileslist testObj; //no use
            if(VFS.search_recordbynameandfolder(FileName,Folder,testObj))
            {
                renameFile(FileName,Folder);
                cout<<"File name is repeated"<<endl;
                cout<<"FileName AFter : "<<FileName<<endl;
            }

            //Send Data to FAT
            fileslist obj;
            if (CopiedFileSize==0)
            {
                noOfBlocks=-1;
                FirstBlock=-1;
            }
            obj.setData(FileName, CopiedFileSize, Date,noOfBlocks,FirstBlock,Folder);
            VFS.FatWrite(obj);
            guiObj=obj;
            VFS.calculateFB();
        }
        else
        {
            cout<<"there's no space"<<endl;
            return -1;
        }

    }
    else
    {
        cout<<"there's no space"<<endl;
        return -1;
    }

    //Throughput measuring
    #ifdef _WIN32
        clock_t timeElapsed = (clock() - start);
        mtime = timeElapsed *1000/ CLOCKS_PER_SEC;
    #endif
    #ifdef __linux
        gettimeofday(&end, NULL);
        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = (((seconds)* 1000) + useconds / 1000.0) + 0.5;

    #endif
    double Throughput= CopiedFileSize/mtime;	//Throughput in bytes/sec
    cout<<"Throughput of copying this file is "<<Throughput<<"bytes/msec"<<endl;
    return Throughput;
}

double CheckFolderSize(const char * path,int & numberOfFiles,bool &PathLength)
{

    if(PathLength==false)
    {
        return -1;
    }
    double totalSize=0;
    int OldNumberOFFiles=0;
    //Get folder name
    char Folder [255];
    char ComPath[255];
    string FolderStr;
    string Path=path;
    std::size_t Lastfound;
    std::size_t found = Path.find('/');
    if(found!=std::string::npos)
    {
        while (found!=std::string::npos)
        {
            Lastfound=found;
            found=Path.find("/",found+1,1);
        }
        FolderStr=Path.substr(Lastfound+1,string::npos);	//starting form the / mark till the end of the string
        const char *cstr = FolderStr.c_str();
        strcpy(Folder, cstr);
    }
    else
    {
        const char *cstr = Path.c_str();
        strcpy(Folder, cstr);
    }
    cout<<"Current Folder is "<<Folder<<endl;
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(path);
    cout<<"Folder Content"<<endl;
    //first iteration to get the total
    if (dpdf != NULL)
    {
       while (epdf = readdir(dpdf))
       {
          if(epdf->d_name[0]!='.')
          {
                //concatente the old path to the file name to form the new path of the file or folder
                string EpdfName=epdf->d_name;
                if(EpdfName.length()+Path.length()+1>244)
                {
                    PathLength=false;
                    return -1;
                }
                struct stat s;
                strcpy(ComPath,path);
                strcat(ComPath,"/");
                strcat(ComPath,epdf->d_name);
                char *CompletePathConst=ComPath;
                if( stat(ComPath,&s) == 0 )
                {
                    if( s.st_mode & S_IFDIR )
                    {
                        cout<<"found Folder "<<CompletePathConst<<endl;
                        totalSize+=CheckFolderSize(CompletePathConst,OldNumberOFFiles,PathLength);
                        cout<<"total size till now : "<<totalSize<<endl;
                        cout<<"OldNumberOFFiles : "<<OldNumberOFFiles<<endl;

                    }
                    else if( s.st_mode & S_IFREG )
                    {
                        cout<<"found File "<<CompletePathConst<<endl;
                        cout<<"file size : "<<filesize(ComPath)<<endl;
                        totalSize+=(ceil(  (filesize(ComPath))/(1.00*( VFS.retBlockSize() ))   )  )*(VFS.retBlockSize());
                        numberOfFiles++;
                        cout<<"numberOfFiles till now in "<<Folder<<" : "<<numberOfFiles<<endl;
                        cout<<"total size till now : "<<totalSize<<endl;
                    }
                    else
                    {
                        cout<<"Something else"<<endl;
                    }
                }
                else
                {
                    cout<<"doesn't Exist"<<endl;
                }

          }
       }
        //increase numberOfFiles by 1 to save the folder record itself
//        if(numberOfFiles==0)
        {
            numberOfFiles++;
        }

    }

    cout<<"numberOfFiles in sub Folders is "<< OldNumberOFFiles <<endl;
    numberOfFiles=numberOfFiles+OldNumberOFFiles;
    cout<<"numberOfFiles in this folder is "<< numberOfFiles<<endl;
    cout<<"total Size of this path : "<<path<<  "  is : "<<totalSize<<endl;
    return totalSize;

}


void renameFolder(char *Folder)
{

    cout<<"renameFolder "<<endl;
    inorderlist objlist;
    string FolderNameStr=Folder;
    for (int i=1;;i++)
    {
        string addedChar="(";
        std::ostringstream ss;
        ss << i;
        addedChar+=ss.str();
        addedChar+=")";
        cout<<addedChar<<endl;
        //prepare new name
        string NewFolderNameStr=FolderNameStr;
        cout<<NewFolderNameStr<<endl;
        NewFolderNameStr+=addedChar;
        cout<<NewFolderNameStr<<endl;
        const char *NewName = NewFolderNameStr.c_str();
        char NewNameChar[255];
        strcpy(NewNameChar,NewName);
        if(!(VFS.search_recordbyfolder(NewNameChar,objlist)))
        {
            strcpy(Folder,NewNameChar);
            break;
        }

    }

}

double CopyFolderFromHDtoVFS(char * PreFolder,const char* path, inorderlist &list)
{
    cout<<"CopyFolderFromHDtoVFS, PreFolder : "<<PreFolder<<" path : "<<path<<endl;
    fileslist arbitobject;
    double mtime, seconds, useconds;
    double Throughput;
    #ifdef _WIN32
        clock_t start = clock();
    #endif
    #ifdef __linux
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif

    //checking size
    string PathWarning=path;
    string FolderWarning=PreFolder;
    if(PathWarning.length()>=255 )
    {
        cout<<"Path length is too long"<<endl;
        return -2;
    }

    int totalSize;
    char PreFolderCopy [255];
    char Folder [255];
    char FolderForFolder [255];
    char FolderForFiles [255];
    strcpy(PreFolderCopy,PreFolder);
    //edit Folder name if repeated
    char pre[255]="*****";


    //check Folder Size
    int TotalNumberOfItems=0;
    bool PathLength=true;
    int FolderSize=CheckFolderSize(path,TotalNumberOfItems,PathLength);
    if(FolderSize==-1 && PathLength==false)
    {
        cout<<"Path length of inner files is too long"<<endl;
        return -2;
    }

    bool SizeValid=false;
    int noOfBlocks;
    //check Fat Availability
    int ExtensionSizeNeeded;
    cout<<"TotalNumberofItems : "<<TotalNumberOfItems<<endl;
    if(VFS.CheckFATSize(TotalNumberOfItems,ExtensionSizeNeeded))
    {
        SizeValid=true;
        cout<<"Fat is OK"<<endl;
    }
    else
    {
        SizeValid=false;
        cout<<"There's no space in Fat for this file"<<endl;
        return -1;
    }

    if(FolderSize==0)
        SizeValid=true;
    //check Files Availability
    cout<<"Folder Size : "<<FolderSize<<endl;
    if(SizeValid&&VFS.CheckFileSize(FolderSize,noOfBlocks,ExtensionSizeNeeded))
    {
        cout<<"Last Logical Part is OK"<<endl;
        //Get folder name
        string FolderStr;
        string Path=path;
        std::size_t Lastfound;
        std::size_t found = Path.find('/');
        if(found!=std::string::npos)
        {
            while (found!=std::string::npos)
            {
                Lastfound=found;
                found=Path.find("/",found+1,1);
            }
            FolderStr=Path.substr(Lastfound+1,string::npos);	//starting form the / mark till the end of the string
            const char *cstr = FolderStr.c_str();
            strcpy(Folder, cstr);
        }
        else
        {
            const char *cstr = Path.c_str();
            strcpy(Folder, cstr);
        }

        cout<<"old Folder Name : "<<endl;
        cout<<PreFolderCopy<<endl;
        if((strcmp(PreFolderCopy,pre) == 0))
        {
            //handle repeated Folder name
            cout<<"FolderName Before : "<<Folder<<endl;
            inorderlist testListObj; //no use
            if(VFS.search_recordbyFirstfolder(Folder,testListObj))
            {
                cout<<"File has been found"<<endl;
                renameFolder(Folder);
                cout<<"Folder name is repeated"<<endl;
                cout<<"FolderName AFter : "<<Folder<<endl;
            }
            else
            {
                cout<<"File has never existed before"<<endl;
            }
        }



        //concatenate previous folder to the current folder
        if(!(strcmp(PreFolderCopy,pre) == 0))
        {
            strcat(PreFolderCopy, "/");
            strcat(PreFolderCopy,Folder);
            strcpy(Folder,PreFolderCopy);
        }
        //Saving Folder record in Fat
        //if(FolderSize==0)
        {
            int ExtensionSizeNeeded;
            if (VFS.CheckFATSize(1,ExtensionSizeNeeded))
            {
                fileslist obj;
                char objName[255]="*****";
                double size=0;
                int FirstBlock=-1;
                obj.setData(objName,size,objName,FirstBlock,FirstBlock,Folder);
                VFS.FatWrite(obj);
                list.insert(obj);
            }
            else
            {
                cout<<"No space in Fat"<<endl;
            }
        }

        cout<<"Folder is "<<Folder<<endl;
        strcpy(FolderForFolder,Folder);
        strcpy(FolderForFiles,Folder);

        //open folder to copy files or another folder
        DIR *dpdf;
        struct dirent *epdf;
        dpdf = opendir(path);
        if (dpdf != NULL)
        {
           while (epdf = readdir(dpdf))
           {
              if(epdf->d_name[0]!='.')
              {
                    //concatente the old path to the file name to form the new path of the file or folder
                    struct stat s;
                    char ComPath[255];
                    strcpy(ComPath,path);
                    strcat(ComPath,"/");
                    strcat(ComPath,epdf->d_name);
                    if( stat(ComPath,&s) == 0 )
                    {
                        if( s.st_mode & S_IFDIR )
                        {
                            cout<<"Copying Folder "<<FolderForFolder<<endl;
                            CopyFolderFromHDtoVFS(FolderForFolder,ComPath,list);
                        }
                        else if( s.st_mode & S_IFREG )
                        {
                            cout<<"Copying File "<<ComPath<<endl;
                            cout<<"From Folder : "<<FolderForFiles<<endl;
                            CopyFileFromHDtoVFS(FolderForFiles,ComPath,arbitobject);
                            list.insert(arbitobject);
                        }
                        else
                        {
                            cout<<"Something else"<<endl;
                        }
                    }
                    else
                    {
                        cout<<"doesn't Exist"<<endl;
                    }
              }
           }
        }
        //Throughput measuring
    #ifdef _WIN32
        clock_t timeElapsed = (clock() - start);
        mtime = timeElapsed * 1000 / CLOCKS_PER_SEC;
    #endif
    #ifdef __linux
        gettimeofday(&end, NULL);
        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = (((seconds)* 1000) + useconds / 1000.0) + 0.5;
    #endif
        Throughput= FolderSize/mtime;	//Throughput in bytes/sec
        cout<<"Throughput of copying this folder is "<<Throughput<<"bytes/msec"<<endl;
    }
    else
    {
        cout<<"There is no space for this Folder: "<<PreFolder<<endl;
        return -1;
    }
    return Throughput;
}

double CopyFileFromVFStoHD(char* FileName,char Folder [],char* path)
{
    /*
     -file name is given
     -path of the file required to copy in
     -send file name to Fat, retrieve the blocks and size
     -open the hard disk file
     -read these block from VFS and write into file
     */

    //checking size
    string PathWarning=path;
    if(PathWarning.length()>=255 )
    {
        cout<<"Path length is too long"<<endl;
        return -2;

    }

    double mtime, seconds, useconds;
    #ifdef _WIN32
        clock_t start=clock();
    #endif
    #ifdef __linux
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif

    DataStr DataObj;
    fstream outfile;
    string filename=FileName; string Path=path;
    char CompletePath[255];
    strcpy(CompletePath,path); // copy string one into the result.
    strcat(CompletePath,"/");
    strcat(CompletePath,FileName);
    outfile.open(CompletePath,ios::out|ios::binary);

    //send file name to Fat, retrieve the blocks and size
    fileslist CopiedRecord;
    if (VFS.search_recordbynameandfolder(FileName,Folder,CopiedRecord))
    {
        int addedsize=CopiedRecord.retSize();
        if(addedsize!=0)
        {
            char*Buffer=new char[addedsize];
            //reading data from VFS and writing in HD
            VFS.readData (CopiedRecord.retFirstBlock(),Buffer,addedsize);
            string y(addedsize,'a');
            for(int i=0;i<addedsize;i++)
            {
                y[i]=Buffer[i];
            }
            outfile << y;

            outfile.close();
            delete [] Buffer;
        }
    }
    else
    {
        cout<<"File Not Found"<<endl;
    }
    //Throughput measuring
    #ifdef _WIN32
        clock_t timeElapsed = (clock() - start);
        mtime=timeElapsed*1000/CLOCKS_PER_SEC;
    #endif
    #ifdef __linux
        gettimeofday(&end, NULL);
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = (((seconds)* 1000) + useconds / 1000.0) + 0.5;

    #endif


    double Throughput= CopiedRecord.retSize()/mtime;	//Throughput in bytes/sec
    cout<<"Throughput of copying this file is "<<Throughput<<"bytes/msec"<<endl;
    return Throughput;
}

double CopyFolderFromVFStoHD(char Folder [],char* path)
{
    cout<<"Copy Folder from VFS to HD, Folder : "<<Folder<<" path :  "<<path<<endl;
    //checking size
    string PathWarning=path;
    if(PathWarning.length()>=255 )
    {
        cout<<"Path length is too long"<<endl;
        return -2;
    }

    double mtime, seconds, useconds;
    #ifdef _WIN32
        clock_t start = clock();
    #endif
    #ifdef __linux
        struct timeval start, end;
        gettimeofday(&start, NULL);

    #endif

    double totalSize=0;
    inorderlist List;
    fileslist obj;
    //get the required Folder index
    string FolderIndexStr=Folder;
    string FolderStr;
    std::size_t Lastfound=0;
    std::size_t found = FolderIndexStr.find('/');
    int indexofMainFolder=0;
    if(found!=std::string::npos)
    {
        //for multiple of folders
        while (found!=std::string::npos)
        {
            found=FolderIndexStr.find("/",found+1,1);
            indexofMainFolder++;
        }
    }
    cout<<"indexofMainFolder : "<<indexofMainFolder<<endl;


    //test the new list//
    cout<<endl<<endl<<"New List"<<endl;
    if(VFS.seach_recordbyFirstChars(Folder,List))
    {
        bool flag2;
        flag2=List.retrieve_the_first_element(obj);
//		cout<<"file.retName() : "<<file.retName()<<" Folder "<<Folder<<" CompletePath "<<CompletePath<<endl;
        while(flag2)
        {
            obj.showData();
            totalSize+=CopyFolderinsideFolder(obj.retFolder(),path,indexofMainFolder);
            flag2=List.retrieve_the_next_element(obj);
        }

    }
    cout<<"List Ends"<<endl;


    //Throughput measuring
    #ifdef _WIN32
        clock_t timeElapsed = (clock() - start);
        mtime = timeElapsed *1000/ CLOCKS_PER_SEC;
    #endif
    #ifdef __linux
        gettimeofday(&end, NULL);
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = (((seconds)* 1000) + useconds / 1000.0) + 0.5;
    #endif

    double Throughput= totalSize/mtime;	//Throughput in bytes/sec
    cout<<"Throughput of copying this folder is "<<Throughput<<"bytes/msec"<<endl;
    return Throughput;
}

double CopyFolderinsideFolder(char Folder [],char* path,int indexofMainFolder)
{
    fstream outfile;
    inorderlist FilesList;
    bool flag;
    fileslist file;
    double totalSize=0;
    char CompletePath[255];
    //seperating Folder Name into Vector
    string FolderStr;
    vector<string> Folders;
    string PathOfFolders=Folder;
    std::size_t Lastfound=0;
    std::size_t found = PathOfFolders.find('/');
    if(found!=std::string::npos)
    {
        //for multiple of folders
        while (found!=std::string::npos)
        {
            FolderStr=PathOfFolders.substr(Lastfound,found-Lastfound);
            Folders.push_back(FolderStr);
            cout<<FolderStr<<endl;
            Lastfound=found+1;
            found=PathOfFolders.find("/",found+1,1);
        }
        FolderStr=PathOfFolders.substr(Lastfound,string::npos);
        Folders.push_back(FolderStr);
        cout<<FolderStr<<endl;

        strcpy(CompletePath,path); // copy string one into the result.
        //for each Folder, Append it and create the Folder, if not found
        for(int i=indexofMainFolder;i<Folders.size();i++)
        {
            cout<<"Folders[i] "<<Folders[i]<<endl;
            const char* charFolder=Folders[i].c_str();
            cout<<charFolder<<endl;
            strcat(CompletePath,"/");
            strcat(CompletePath,charFolder);
            cout<<"CompletePath : "<<CompletePath<<"---------------------------------------------"<<endl;

            #ifdef __linux
                const int dir_err = mkdir(CompletePath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                if (-1 == dir_err)
                {
                    cout << "Folder Already exist" << endl;
                }
            #endif
            #ifdef _WIN32

                const int dir_err =_mkdir(CompletePath);
                if (dir_err == 0)
                {
                    cout << "created---------------------------------------" << endl;
                }
                else if (dir_err == -1){
                    cout << "failed----------------------------------------" << endl;
                }
            #endif

        }

    }
    else
    {
        //Only one Folder
        //only needed if folder was single

        strcpy(CompletePath,path); // copy string one into the result.
        strcat(CompletePath,"/");
        strcat(CompletePath,Folder);
        cout<<"only one folder Complete path : "<<CompletePath<<"------------------------------"<<endl;
        //create Folder
        #ifdef __linux
            const int dir_err = mkdir(CompletePath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if (-1 == dir_err)
            {
                cout << "Folder Already exist" << endl;
            }
        #endif
#ifdef _WIN32

    const int dir_err =_mkdir(CompletePath);
    if (dir_err == 0)
    {
        cout << "created---------------------------------------" << endl;
    }
    else if (dir_err == -1){
        cout << "failed----------------------------------------" << endl;
    }
#endif
    }
    cout<<"search for Folder "<<Folder<<endl;
    //search Folder by the first few chars
    if(VFS.search_recordbyfolder(Folder,FilesList))
    {
        flag=FilesList.retrieve_the_first_element(file);
        cout<<"file.retName() : "<<file.retName()<<" Folder "<<Folder<<" CompletePath "<<CompletePath<<endl;
        char *falseName="*****";
        while(flag)
        {
            if(strcmp(file.retName(),falseName)!=0)
            {
                cout<<"File is Copying... "<<endl;
                CopyFileFromVFStoHD(file.retName(),Folder,CompletePath);
                totalSize+=file.retSize();
                file.showData();
            }
            flag=FilesList.retrieve_the_next_element(file);
        }
    }
    else
    {
        cout<<"Folder doesn't Exist"<<endl;
    }
    return totalSize;
}

void RemoveFile(char* FileName,char* FolderName)
{
    /*
     -Ask FAT where it exists (returns fileslist Object) and tell it to delete it so record is turned to ///
     -Tell FBL to disable these Blocks
     -Update Super Block
     */
    cout<<"RemoveFile :  "<<" FileName  : "<<FileName<<" FolderName : "<<FolderName<<endl;
    char* ExceptionName="*****";
    fileslist file;
    VFS.search_recordbynameandfolder(FileName,FolderName,file);
    cout<<"File to be deleted"<<endl;
    file.showData();
    VFS.delete_record(file.retName(),file.retFolder());
    inorderlist list;

    if(strcmp(file.retName(),ExceptionName)!=0  && (file.retSize()!=0))
    {
        cout<<"File name is not *****"<<endl;
        VFS.Deleteblocks(file.retNumberofblocks(),file.retFirstBlock());
        for(int i=0;i<file.retNumberofblocks();i++)
            VFS.decreaseFilesSize();
        VFS.calculateFB();
    }
     ListFilesDisplay(list);

}


void RemoveFolder(char Folder [])
{
/*
 -search FAT by Folder and get list of file
 -loop over them and RemoveFile
 */

    inorderlist toRemoveList;
    fileslist obj;
    //test the new list//
    cout<<endl<<"Remove Folder named : "<<endl;
    cout<<Folder<<endl;
    cout<<endl<<endl<<"To remove  List"<<endl;
    if(VFS.seach_recordbyFirstChars(Folder,toRemoveList))
    {
        bool flag2;
        flag2=toRemoveList.retrieve_the_first_element(obj);
        while(flag2)
        {
            cout<<"this file needs to be deleted"<<endl;
            obj.showData();
            inorderlist List;
            bool flag;
            fileslist file;
            cout<<"To delete File "<<obj.retName()<<endl;
            RemoveFile(obj.retName(),obj.retFolder());
            flag2=toRemoveList.retrieve_the_next_element(obj);
        }
    }
    cout<<"List Ends"<<endl;

}

long FreeBlocks()
{
    long FreeBlocks;
    VFS.calculateFB();
    VFS.readFreeBlocks(FreeBlocks);
    cout<<"from FreeBlocks, freeblocks: "<<FreeBlocks<<endl;
    return FreeBlocks;
}

void ListFilesDisplay(inorderlist &List)
{
//    inorderlist List;
    VFS.ListFiles(List);
    fileslist obj;
    bool flag ;
    flag=List.retrieve_the_first_element(obj);
    while(flag)
    {
        obj.showData();
        flag=List.retrieve_the_next_element(obj);
    }
}

double FragmentationPercentage()
{
    //due to defragmentation manager fragmentation percentage is always 100
    cout<<"0%"<<endl;
    return 0;
}
double add_txtFile(string name, string rec, fileslist &Obj)
{
    //>>>>>>>>>>>>>>>check if return ==-1 , cout << noEmpty space
    ofstream outputFile;
    //string rec,name,rec2;
    //cout << "Enter the name: ";
    //cin >> name;
    name = name + ".txt";
    outputFile.open(name.c_str());

    //cout << "Enter the record : ";
    //getline(cin,rec);
    //getline(cin,rec);
    //cout << rec << endl;//---------------------->test

    outputFile << rec;
    outputFile.close();
    //get File path path
#ifdef __linux

    char currentDir[255];
    getcwd(currentDir, sizeof(currentDir));

#endif
#ifdef _WIN32

    char currentDir[255];
    GetCurrentDir(currentDir, sizeof(currentDir ) );

#endif
    string path = string(currentDir) + "/" + name;
    cout <<"Path :         "<< path << endl;
    //call copy file from HD
    double x = CopyFileFromHDtoVFS("*****", path.c_str(), Obj);

    //delete file from hard disk
    int deleted;
    deleted = remove(name.c_str());
    if (deleted != 0){
        cout << "Falied " << endl;
    }
    else{
        cout << "Deleted Successfully" << endl;
    }


    return x;
}



