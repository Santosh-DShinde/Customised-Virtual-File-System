#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<io.h>

/////////////////////////////////////////////////////////////////////////////////////////       
//   Author   	 : Sanosh Dnyanoba Shinde
//   Date     	 : 19/01/2021
//	 Purpose 	 : Main purpose of this project is to create our own costomise file system
//				   Like Linux Operating System 
//	 Description : This project is build for create our own file system.     
//	
/////////////////////////////////////////////////////////////////////////////////////////       


/////////////////////////////////////////////////////////////////////////////////////////       
//		Define all Macroes
/////////////////////////////////////////////////////////////////////////////////////////

#define MAXINODE 50

#define READ 1      //it is used in project  permition
#define WRITE 2     //it is used in project  permition

#define MAXFILESIZE 1024      // Maximum file size is 1024 

#define REGULAR 1       
#define SPECIAL 2

#define START 0      
#define CURRENT 1
#define END 2

typedef struct superblock      // structure superblock
{
	int TotalInodes;
	int FreeInode;
}SUPERBLOCK, *PSUPERBLOCK;
////////////////////////////////////////////////////////////////////////////////////////
typedef struct inode          //structure  Inode. it contain information about file
{
	char FileName[MAXINODE];
	int InodeNumber;
	int FileSize;
	int FileActualSize;
	int FileType;
	char *Buffer;
	int LinkCount;
	int ReferenceCount;
	int permission;  // 1  2  3
	struct inode *next;
}INODE, *PINODE, **PPINODE;
///////////////////////////////////////////////////////////////////////////////////////
typedef struct filetable
{
	int readoffset;
	int writeoffset;
	int Count;
	int mode;
	PINODE ptrinode;
}FILETABLE, *PFILETABLE;
///////////////////////////////////////////////////////////////////////////////////////
typedef struct ufdt         //it is a structure used for create the ufdt Array
{
	PFILETABLE ptrfiletable;
}UFDT;
///////////////////////////////////////////////////////////////////////////////////////
UFDT UFDTArr[MAXINODE];       //   Global Variables
SUPERBLOCK SUPERBLOCKobj;       // which we can access globaly
PINODE head = NULL;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		1]	Name Of Function   :  man . This function is use to Display Manual , When we type man as a input. it helps us how must be a propper command is.
//		2]	Input Parameters   :  char *name .  We take a character pointer as a parameter to take string from main function, where main is our Entry point function.
//		3]	The Return Value Of Parameter : void .  The return value of the parameter is void, Because this function is not going to return anyathing.. Therefore The return value of our function is void.
//		
//		4]	Description : The main pourpose of the man function is to guide the user how to give propper commands. It helps the user to type commands or create file or other any operations.
//			That function takes string from user and pass to the man function, Man function takes that string into the pointer, Whose name is name
//    	    That name compares with each and every command. If any command is compares successfully. Then control goes inside if else condition and what is print in that condition it will be print on screen.
//			
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void man(char *name)             // it is a manual funtion 
{
	if (name == NULL)return;

	if (stricmp(name,"create")==0)
	{
		printf("Description : Used to Create new regular file \n");
		printf("Usage : create file_name Permition\n");
	}
	else if (stricmp(name,"read")==0)
	{
		printf("Deacription : Used To Read The data From The Regular File\n");
		printf("Usage : read File_name No_Of_Bytes_To_Read\n");
	}
	else if (stricmp(name,"write")==0)
	{
		printf("Description : Used to write into Regilar File\n");
		printf("Usage : write File_Name\n After this Enter The Data That We Want To Write\n");
	}
	else if (stricmp(name,"ls")==0)
	{
		printf("Description : Used to List All Information of File\n");
		printf("Usage : ls\n");
	}
	else if ((name,"stat")==0)
	{
		printf("Description : Used to display information of file\n");
		printf("Usage : stat file_name\n");
	}
	else if (stricmp(name,"fstat")==0)
	{
		printf("Description :Used To Display information of file\n");
		printf("Usage : stat File_Descriptor\n");
	}
	else if (stricmp(name,"truncate")==0)
	{
		printf("Description :Used To remove data from file\n");
		printf("Usage : truncate File_Name\n");
	}
	else if (stricmp(name,"open")==0)
	{
		printf("Description :Used To Open Existing file\n");
		printf("Usage : open File_Name mode\n");
	}
	else if (stricmp(name,"close")==0)
	{
		printf("Description :Used To Close Opened file\n");
		printf("Usage : Close File_Name\n");
	}
	else if (stricmp(name,"closeall")==0)
	{
		printf("Description :Used To Close All Opened file\n");
		printf("Usage : closeall\n");
	}
	else if (stricmp(name, "lseek")==0)
	{
		printf("Description :Used To Change File Offset \n");
		printf("Usage : lseek File_Name_ChangeInOffset StartPoint\n");
	}
	else if (stricmp(name,"rm")==0)
	{
		printf("Description :Used To Delete the FIle \n");
		printf("Usage : rm File_Name\n");
	}
	else 
	{
		printf("Error : NO Manual Entry Available.\n");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	1]	Name Of Function : DisplayHelp
//	2]	Input Parameters : -
//	3]	The Return Value Of Parameter : void
//	
//	4]	Description : DisplayHelp is the function which is use to help the user about its workin.
//					  This Function isn't take any parameter and returns nothing .  
//					 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DisplayHelp()
{
	printf(" ls 	  : TO List Out All Files\n");
	printf(" clear    : TO CLear Console \n");
	printf(" open     : TO Open The Files\n");
	printf(" close    : TO Close The Files\n");
	printf(" closeall : TO Close The All Opened Files\n");
	printf(" read     : TO Read The Contents From Files\n");
	printf(" write    : TO Write Contents Into The Files\n");
	printf(" exit     : TO Terminate File System\n");
	printf(" stat     : TO Display The Information Of File Using Name\n");
	printf(" fstat    : TO Display Information Of File Using File Descriptor \n");
	printf(" truncate : TO Remove All Data From File\n");
	printf(" rm       : TO Delete The File\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	1]	Name Of Function   :  GetFDFromName
//	2]	Input Parameters   :  *name
//	3]	The Return Value Of Parameter : int 
//	4]	Description : The main perpose of the function is to get the File Discrip From Name
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GetFDFromName(char *name)
{
	int i = 0;

	while(i < MAXINODE)
	{
		if (UFDTArr[i].ptrfiletable != NULL)
			if (stricmp((UFDTArr[i].ptrfiletable->ptrinode->FileName),name) ==0)
			{
				break;
			i++;
		}

	if (i == MAXINODE)   
		 return -1;    
		else 
		  return i;      
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	1]	Name Of Function   :  Get_Inode
//	2]	Input Parameters   :  *name
//	3]	The Return Value Of Parameter : PINODE 
//	4]	Description : The main perpose of the function is to get the Inode Number
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PINODE Get_Inode(char *name)
{
	PINODE temp = head;
	int i = 0 ;

	if (name == NULL)
		return NULL;

while(temp != NULL)
{
	if (stricmp(name,temp->FileName)==0)
		break;	
	temp = temp->next;
}
return temp;
}

void CreateDILB()        ////////////////////////
{
	int i = 1;
	PINODE newn = NULL;
	PINODE temp = head;

	while(i <= MAXINODE)
	{
		newn = (PINODE)malloc(sizeof(INODE));

		newn->LinkCount = 0;
		newn->ReferenceCount = 0;
		newn->FileType = 0;
		newn->FileSize = 0;

		newn->Buffer = NULL;
		newn->next = NULL;

		newn->InodeNumber = i;          

		if (temp == NULL)
		{
			head = newn;
			temp = head;
		}
		else
		{
			temp->next = newn;
			temp = temp->next; 
		}
		i++;
	}
 // printf("DILB created successfully\n");
}

void InitialiseSuperBlock()   /////////////////////////////////////
{
	int i = 0;

	while(i < MAXINODE) 	// MAXINODE = 50
	{
		UFDTArr[i].ptrfiletable = NULL;
		i++;
	}

SUPERBLOCKobj.TotalInodes = MAXINODE;
SUPERBLOCKobj.FreeInode = MAXINODE;
}

int CreateFile(char *name, int permission)
{
	int i = 0;
	PINODE temp = head;

	if ((name == NULL) || (permission == 0) || (permission > 3))
		return -1;
	
	if (SUPERBLOCKobj.FreeInode == 0)
	return -2;

(SUPERBLOCKobj.FreeInode)--;

if (Get_Inode(name)!= NULL)
	return -3;

while(temp != NULL)
{
	if (temp->FileType == 0)
	break;
temp = temp->next;
}
	
while(i < MAXINODE)
{
	if (UFDTArr[i].ptrfiletable == NULL)
		break;
	i++;
}

UFDTArr[i].ptrfiletable = (PFILETABLE) malloc (sizeof(FILETABLE));

UFDTArr[i].ptrfiletable->Count = 1;
UFDTArr[i].ptrfiletable->mode = permission;
UFDTArr[i].ptrfiletable->readoffset = 0;
UFDTArr[i].ptrfiletable->writeoffset = 0;

UFDTArr[i].ptrfiletable->ptrinode = temp;

strcpy(UFDTArr[i].ptrfiletable->ptrinode->FileName,name);
UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
UFDTArr[i].ptrfiletable->ptrinode->LinkCount= 1;
UFDTArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
UFDTArr[i].ptrfiletable->ptrinode->FileActualSize =0;
UFDTArr[i].ptrfiletable->ptrinode->permission = permission;
UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

return i;
}

//rm_File("Demo.txt")
int rm_File(char *name)
{
	int fd = 0;

	fd = GetFDFromName(name);
	if (fd == -1)
		return -1;

(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;

if (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount ==0)
{
	UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
	//free (UFDTArr[fd].ptrfiletable->ptrinode->Buffer);
	free(UFDTArr[fd].ptrfiletable);
}

UFDTArr[fd].ptrfiletable = NULL;
(SUPERBLOCKobj.FreeInode)++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	1]	Name Of Function   :  ReadFile
//	2]	Input Parameters   :  int fd , char *arr, int isize .
//	3]	The Return Value Of Parameter : int 
//	4]	Description : The main perpose of the Function is Read The file with the permission
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ReadFile(int fd, char *arr, int isize)
{
	int read_size = 0;

	if (UFDTArr[fd].ptrfiletable == NULL)       return -1;

	if (UFDTArr[fd].ptrfiletable->mode != READ && UFDTArr[fd].ptrfiletable->mode != READ + WRITE)      	return -2;

if (UFDTArr[fd].ptrfiletable->ptrinode->permission != READ && UFDTArr[fd].ptrfiletable->ptrinode->permission != READ + WRITE )  return -2;

if (UFDTArr[fd].ptrfiletable->readoffset == UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)   return -3;

if (UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)  return-4;

read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) - (UFDTArr[fd].ptrfiletable->readoffset);
if (read_size < isize)
{
	strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),read_size);

UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + read_size;
}
else
{
	strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),isize);

	(UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + isize;
}

return isize;
}

int WriteFile(int fd, char *arr, int isize)
{
	if(((UFDTArr[fd].ptrfiletable->mode)!= WRITE) && ((UFDTArr[fd].ptrfiletable->mode)!= READ + WRITE))  return -1;

	if(((UFDTArr[fd].ptrfiletable->ptrinode->permission) != WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->permission) != READ + WRITE))    return -1;

if ((UFDTArr[fd].ptrfiletable->writeoffset) == MAXFILESIZE)    return -2;

if ((UFDTArr[fd].ptrfiletable->ptrinode->FileType)!= REGULAR)   return -3;

strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset),arr,isize);

(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + isize;

(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + isize;

return isize;
}

int OpenFile(char *name, int mode)
{
	int i = 0;
	PINODE temp = NULL;

	 if (name == NULL || mode <= 0)
	 	return -1;

	 temp = Get_Inode(name);
	 if (temp == NULL)
	 	return -2;

	 if (temp->permission < mode)
	 	return-3;

	 while(i < MAXINODE)
	 {
	 	if (UFDTArr[i].ptrfiletable == NULL)
	 		break;
	 	i++;
	 }

UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
if (UFDTArr[i].ptrfiletable == NULL)   return -1;
UFDTArr[i].ptrfiletable->Count = 1;
UFDTArr[i].ptrfiletable->mode = mode;

if (mode == READ + WRITE)
{
	UFDTArr[i].ptrfiletable->readoffset = 0 ;
	UFDTArr[i].ptrfiletable->writeoffset =0 ;
}
else if (mode == READ)
{
	UFDTArr[i].ptrfiletable->readoffset = 0;
}
else if (mode == WRITE)
{
	UFDTArr[i].ptrfiletable->writeoffset = 0;
}
	UFDTArr[i].ptrfiletable->ptrinode = temp;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;
	
	return i;
}

void CloseFileByName(int fd)
{
	UFDTArr[fd].ptrfiletable->readoffset = 0;
	UFDTArr[fd].ptrfiletable->writeoffset = 0;
	(UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
}

int CloseFileByName(char *name)
{
	int i = 0;
	i = GetFDFromName(name);
	if (i == -1)
		return -1;

	UFDTArr[i].ptrfiletable->readoffset = 0;
	UFDTArr[i].ptrfiletable->writeoffset = 0;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;

	return 0;
}

void ClodeAllFile()
{
	int i = 0;
	while(i < MAXINODE)
	{
		if(UFDTArr[i].ptrfiletable != NULL)
		{
			UFDTArr[i].ptrfiletable->readoffset = 0;
			UFDTArr[i].ptrfiletable->writeoffset = 0;
			(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
			break;
		}
	i++;
	}
}

int LseekFile(int fd,int size, int from)
{
	if ((fd < 0)||(from > 2))         return -1;
	if (UFDTArr[fd].ptrfiletable == NULL)      return -1;

	if ((UFDTArr[fd].ptrfiletable->mode == READ) || (UFDTArr[fd].ptrfiletable->mode == READ + WRITE))
	{
		if (from == CURRENT)
		{
			if (((UFDTArr[fd].ptrfiletable->readoffset)+size) > UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)   return -1;
				if (((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0)  return -1;
				 (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + size;
		}
		else if (from == START)
		{
			if (size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))      return -1;
			if (size < 0)     return -1;
			(UFDTArr[fd].ptrfiletable->readoffset) = size;
		}
		else if (from == END)
		{
			if ((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size > MAXFILESIZE)     return -1;
			if (((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0 )      return -1;
			 (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+size;
		}
	}
	else if (UFDTArr[fd].ptrfiletable->mode == WRITE)
	{
		if (from == CURRENT)
		{
			if (((UFDTArr[fd].ptrfiletable->writeoffset)+ size)> MAXFILESIZE)        return -1;
			if (((UFDTArr[fd].ptrfiletable->writeoffset)+ size) < 0)         return   -1;
			if (((UFDTArr[fd].ptrfiletable->writeoffset)+ size) > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
			(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->writeoffset)+size;
			(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset)+ size;
		}
		else if (from == START)
		{
			if (size > MAXFILESIZE)        return -1;
			if (size < 0)           return -1;
			if (size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
				(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)=size;
				(UFDTArr[fd].ptrfiletable->writeoffset)=size;
		}
		else if (from == END)
		{
			if ((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+size>MAXFILESIZE)   return -1;
			if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0)          return -1;
			(UFDTArr[fd].ptrfiletable->writeoffset)=(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+ size;		
			}
		}
}

void ls_file()
{
	int i = 0;
	PINODE temp = head ;

	if (SUPERBLOCKobj.FreeInode == MAXINODE)
	{
		printf("Error : There are no files\n");
		return;
	}

	printf("\nFile Name\tInode Number\tFile size\tLink Count\n");
	printf("----------------------------------------------------------------------------\n");
	while(temp != NULL)
	{
		if (temp->FileType != 0)
		{
			printf("%s\t\t%d\t\t%d\t\t%d\n",temp->FileName,temp->InodeNumber,temp->FileActualSize,temp->LinkCount);
		}
		temp = temp->next;
	}
	printf("----------------------------------------------------------------------------\n");
}

int fstat_file(int fd)
{
	PINODE temp = head;
	int i = 0;

	if (fd < 0)         return -1;

	if (UFDTArr[fd].ptrfiletable == NULL)         return -2;

temp = UFDTArr[fd].ptrfiletable->ptrinode;

printf("\n------------ Statistical Information About File-------------\n");
printf("File Name       : %s\n",temp->FileName );
printf("Inode Number 	: %d\n",temp->InodeNumber );
printf("File Size    	: %d\n",temp->FileSize );
printf("Actual File Size: %d\n",temp->FileActualSize );
printf("Link Count   	: %d\n",temp->LinkCount );
printf("Reference Count : %d\n",temp->ReferenceCount );

if (temp->permission == 1)
	printf("\nFile Permission : Read Only\n");
	else if (temp->permission == 2)
		printf("File Permission : Write Only \n");
	else if (temp->permission == 3)
		printf("File Permition : Read And Write\n");
printf("---------------------------------------------------------------\n");

return 0;
}

int stat_file(char *name)
{
	PINODE temp = head;
	int i =0;

	if (name == NULL)      return -1;

	while(temp != NULL)
	{
		if(stricmp(name,temp->FileName)==0)
			break;
		temp = temp->next;
}

if (temp == NULL)        return -2;

printf("\n------------ Statistical Information About File-------------\n");
printf("File Name   	 : %s\n",temp->FileName );
printf("Inode Number 	 : %d\n",temp->InodeNumber );
printf("File Size   	 : %d\n",temp->FileSize );
printf("Actual File Size : %d\n",temp->FileActualSize );
printf("Link Count 	     : %d\n",temp->LinkCount );
printf("Reference Count  : %d\n",temp->ReferenceCount );

if (temp->permission ==1)
	printf("\nFile Permition is : read Only\n");
else if(temp ->permission == 2)
	printf("File Permission is : Write\n");
else if(temp->permission ==3 )
	printf("File Permition is : Read And Write\n");
printf("---------------------------------------------------------------\n");

return 0;
}

int truncat_File(char *name)
{
	int fd = GetFDFromName(name);
	if(fd == -1)
		return -1;

	memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer,0,1024);
	UFDTArr[fd].ptrfiletable->readoffset = 0;
	UFDTArr[fd].ptrfiletable->writeoffset = 0;
	UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	This is the Entry Point Function, frome where our programm gets start to execute 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	char *ptr = NULL;
	int ret = 0, fd = 0,count = 0;
char command[4][80], str[80], arr[1024];  // Terminal

InitialiseSuperBlock();
CreateDILB();

while(1)
{
	fflush(stdin);
	strcpy(str,"");

	printf("\nDeveloper@Coder VFS : >");

	fgets(str,80,stdin);  // scanf("%[^'\n']s",str);

	 count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);

if(count == 1)
{
	if (stricmp(command[0],"ls") == 0)
	{
		ls_file();
	}
	else if (stricmp(command[0],"closeall")==0)
	{
		ClodeAllFile();
		printf("All File Close Successfully\n");
		continue;
	}
	else if (stricmp(command[0],"clear")==0)
	{
		system("cls");
		continue;
	}
	else if (stricmp(command[0],"help")==0)
	{
		DisplayHelp();
		continue;
	}
	else if (stricmp(command[0],"exit")==0)
	{
		printf("Terminating The Virtual File System\n");
		break;
	}
	else
	{
		printf("\nError : Command not Found !!!\n");
		continue;
	}
}
	else if (count ==2)
	{
		if (stricmp(command[0],"stat")==0)
		{
			ret =  stat_file(command[1]);
			if (ret == -1)
				printf("Error : Incorrect Parameters\n");
			if(ret == -2)
				printf("Error : There ls NO Such A File \n");
			continue;
			}
		else if (stricmp(command[0],"fstat") == 0)
		{
			ret = fstat_file(atoi(command[1]));
			if(ret == -1)
				printf("Error : Incorrect Parameters\n");
			if(ret == -2)
				printf("Error : There ls NO Such A File\n");
			continue;
		}
		else if(stricmp (command[0],"close") ==0)
		{
			ret = CloseFileByName(command[1]);
			if(ret == -1)
				printf("Error : There ls NO Such A File\n");
			continue;
		}
		else if(stricmp (command[0],"rm") ==0)
		{
			ret = rm_File(command[1]);
			if(ret == -1)
				printf("Error : There ls NO Such A File\n");
			continue;
		}
		else if(stricmp (command[0],"man") ==0)
		{
			man(command[1]);
		}
		else if(stricmp (command[0],"write") ==0)
		{
			fd = GetFDFromName(command[1]);
			if (fd == -1)
					{
						printf("Error : Incorrect Parameters\n");
						continue;
					}
					printf("Enter The Data :  \n");
					scanf("%[^\n]",arr);

					ret = strlen(arr);
					if (ret == 0)
					{
						printf("Error : Incorrect Parameters\n");
						continue;
					}
			ret = WriteFile(fd,arr,ret);
			if (ret == -1)
				printf("Error : Permition Deniede\n");
			if(ret == -2)
				printf("Error : There ls NO Sufficieant Memory to Write\n");
			if(ret == -3)
				printf("Error : It ls Not Regular File\n");
		}
		else if (stricmp(command[0],"truncate")==0)
		{
			ret = truncat_File(command[1]);
			if(ret == -1)
				printf("Error Incorrect Parameters\n");
		}
		else 
		{
			printf("Command Not Found!!!\n");
			continue;
		}
	}
		else if (count == 3)
		{
			if(stricmp(command[0],"create") ==0)
			{
				ret = CreateFile(command[1],atoi(command[2]));
				if(ret >= 0)
					printf("File Successfully Created With File Descriptor %d\n",ret);
				if(ret == -1)
					printf("Incorrect Parameters\n");
				if(ret == -2)
					printf("Error : There ls No Inodes\n");
				if( ret == -3)
					printf("Error : File Already Exist\n");
				if(ret ==-4)
					printf("Error : Memory Allocation Failure\n");
				continue;
			}
			else if (stricmp(command[0],"open") == 0)
			{
						ret = OpenFile(command[1],atoi(command[2]));
				if(ret >= 0)
					printf("File Successfully Opened With File Descriptor %d\n",ret);
				if(ret == -1)
					printf("Incorrect Parameters\n");
				if(ret == -2)
					printf("Error : File Not Present\n");
				if( ret == -3)
					printf("Error : Permition Denied\n");
				continue;
			}
		else if (stricmp(command[0],"read") ==0)
		{
			fd = GetFDFromName(command[1]);
			if(fd == -1)
			{
				printf("Error : Incorrect Parameter\n");
				continue;
			}
			ptr = (char*)malloc(sizeof(atoi(command[2]))+1);
			if (ptr == NULL)
			{
				printf("Error : Memory Allocation Failure\n");
				continue;
			}
			ret = ReadFile(fd,ptr,atoi(command[2]));
				if(ret == -1)
					printf("Error : File Not Exist\n");
				if(ret == -2)
					printf("Error : Permition Denied\n");
				if( ret == -3)
					printf("Error : Reached At The End Of The File  \n" );
				if(ret ==- 4)
					printf("Error : It ls Not Regular File\n");
				if(ret == 0)
					printf("Error : File Empty\n");
				if(ret > 0)
				{
					write(WRITE,ptr,ret);
				}
				continue;
		}
	else 
	{
		printf("\nError : Command Not Found!!!\n");
		continue;
	}
}
else if (count ==4 )
{
	if(stricmp(command[0],"lseek") ==0)
	{
		fd = GetFDFromName(command[1]);
			if(fd == -1)
			{
				printf("Error : Incorrect Parameters\n");
				continue;
			}
		ret = LseekFile(fd,atoi(command[2]),atoi(command[3]));
		if(ret == -1)
		{
			printf("Error : Unable To Perform lseek\n");
		}
	}
		else
		{
			printf("\nError : Command Not Found!!!\n");
			continue;
		}
}
	else
	{
		printf("\nError : Command Not Found!!!");
		continue;
	}
}
	return 0;
}