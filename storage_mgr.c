#include<stdio.h>
#include "dberror.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "storage_mgr.h"
void initStorageManager(void)
{
	printf("Sets the storage manager");
}


// Function to create a page of size 1 and fills it with /0 
RC createPageFile(char *filename)
{

	FILE *myfile = fopen(filename, "r"); // opening file in read mode 
	
        char *text; 
        text = (char *) calloc (PAGE_SIZE, sizeof(char)); //allocating multiple blocks of memory each of same size 
	 
	//int flag=0; 
	
	if(myfile != NULL)
        {
			
		RC_message = "FILE PRESENT";	
                 //myfile = fopen(filename,"w");
		
               //fwrite(text,sizeof(char),PAGE_SIZE,myfile)
		fclose(myfile);
		free(text);
		return RC_FILE_PRESENT; // returning file not present 
	}	
		
       else
        {
		myfile = fopen(filename,"w"); // opening file in write mode 
		long int pos = ftell(myfile);
                // getting the position of the file
 
		 if(pos == SEEK_END)
                    {
			return RC_FILE_POINTER_NOT_AT_FIRST; // pointer not in the begining 
		    }

		else
                  {
		       fseek(myfile, 0, SEEK_SET); // set pointer to begining
		       fwrite(text,sizeof(char),PAGE_SIZE,myfile);// write /0
		       free(text);
		       fclose(myfile);
		       return RC_OK;
		  }
	}
		
}


// to open an existing file page , if successful the fields of the handle will be initialised with the information of opened file 
RC openPageFile (char *fileName, SM_FileHandle *fHandle)
{
	
	char ch;
	int num_lines = 0;
        int page;

	FILE *myfile = fopen(fileName,"r"); // opening the file in read mode 
	
	if(myfile != NULL)
               {
		    	fHandle->mgmtInfo = myfile;
		    	fHandle->fileName = fileName;

			// setting pointer to begining  
                        fseek(fHandle->mgmtInfo,0,SEEK_SET);

			int start_pos = ftell(fHandle->mgmtInfo);
			fHandle->curPagePos = start_pos;
			fseek(fHandle->mgmtInfo, fHandle->curPagePos , SEEK_END);
			page = ftell(myfile);
			int num_pages = (page)/PAGE_SIZE; //calculates the total num of pages
			fHandle->totalNumPages = num_pages;
			//printf("%f",num_pages);

			//fclose(myfile);
			return RC_OK;
	
		}
	else
               {
		    return RC_FILE_NOT_FOUND;
	       }
}

//closes the open pge 
RC closePageFile (SM_FileHandle *fHandle)
{
	
	if(fHandle != NULL)
            {

		
	      
		if(fopen(fHandle->fileName,"r")== NULL)
                   {
		      return RC_FILE_NOT_OPEN;
	             
	           }
                 //checks if the file is open or not 
		else
                    {
			 int c = fclose(fHandle->mgmtInfo);
	              if(!c)
	              return RC_OK;
	
	 
		    }
             }
	
	return RC_FILE_NOT_FOUND;
}

//destroys the page

 RC destroyPageFile (char *fileName)
{
	FILE *myfile = fopen(fileName , "r");
        int rem;

	if(myfile!= NULL)
             {
	           fclose(myfile);
	           rem = remove(fileName);
		
	      }
           //check if file destroyed 
	     if(rem == 0)
	        return RC_OK;
	    else
	        return RC_FILE_NOT_DELETED;

	return RC_FILE_NOT_FOUND;
}

//readblock funtion to read from PageNUmth block from a file 
 RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{

	FILE *myfile = fopen(fHandle->mgmtInfo,"r");
	   
               if(fHandle->mgmtInfo == NULL) //no file to read
		     return RC_FILE_NOT_FOUND;
	
	        int seeker= fseek(fHandle->mgmtInfo, pageNum*sizeof(char), SEEK_SET);
	        int readBlock;
	        /*if(pageNum < 0 || fHandle->totalNumPages < pageNum)
	        return RC_READ_NON_EXISTING_PAGE;*/
	       if(seeker == 0 && !(fHandle->totalNumPages < pageNum ))
                  {
		          readBlock = fread(memPage,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);
		           fHandle->curPagePos = pageNum;
		                if(!readBlock)
			           return RC_READ_NOT_SUCCESSFULL;
		                else
			           return RC_OK;
	          }
	
	       else 
		  return RC_READ_NON_EXISTING_PAGE;
	      fclose(myfile);
	     //return RC_OK;
}

//gives the current position of the block
int getBlockPos (SM_FileHandle *fHandle)
{
	if(fHandle!= NULL)
		return fHandle->curPagePos;
	else 
		return RC_FILE_NOT_FOUND;
}

// reads the first page of the block
RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	//FILE *myfile = fopen(fHandle->mgmtInfo,"r");
	if(fHandle->mgmtInfo == NULL)
		return RC_FILE_NOT_FOUND;
	int read_page =0;
	int seeker;
        seeker= fseek(fHandle->mgmtInfo, read_page, SEEK_SET);
	int readBlock;
	fHandle->curPagePos = 0;
	/*if( fHandle->totalNumPages < read_page)
		return RC_READ_NON_EXISTING_PAGE;*/

	if(seeker == 0 && !(fHandle->totalNumPages < read_page ))
	{
		readBlock = fread(memPage,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);
		fHandle->curPagePos = read_page;
		// Unable to read the block
		if(!readBlock)
			return RC_READ_NOT_SUCCESSFULL;
		else
			return RC_OK;
	}
	// file containing less than pagenum pages
	else 
		return RC_READ_NON_EXISTING_PAGE;
	
	/*return readBlock(0,fHandle,memPage);
	return RC_OK;*/
}

//reads previous block in relation to current position of the block
RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle->mgmtInfo == NULL)
		return RC_FILE_NOT_FOUND;
	int read_page;
        read_page=fHandle->curPagePos - 1;
	int seeker;
        seeker= fseek(fHandle->mgmtInfo, fHandle->curPagePos, read_page);
	int readBlock;
	fHandle->curPagePos = fHandle->curPagePos - 1;
	/*if( fHandle->totalNumPages < read_page)
		return RC_READ_NON_EXISTING_PAGE;*/
	if(seeker == 0 && !(fHandle->totalNumPages < read_page ))
	{
		readBlock = fread(memPage,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);
		fHandle->curPagePos = read_page;
		// Unable to read the block
		if(!readBlock)
			return RC_READ_NOT_SUCCESSFULL;
		else
			return RC_OK;	
	}
	// file containing less than pagenum pages
	else 
		return RC_READ_NON_EXISTING_PAGE;
	//return RC_OK;
}

//reads current block in relation to current position of the block
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle->mgmtInfo == NULL)
		return RC_FILE_NOT_FOUND;
	int read_page ;
        read_page= getBlockPos(fHandle) ;
	int seeker;
        seeker= fseek(fHandle->mgmtInfo, fHandle->curPagePos, read_page);
	int readBlock;
	fHandle->curPagePos = read_page;
	/*if( fHandle->totalNumPages < read_page)
		return RC_READ_NON_EXISTING_PAGE;*/
	if(seeker == 0 && !(fHandle->totalNumPages < read_page ))
	{
		readBlock = fread(memPage,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);
		fHandle->curPagePos = read_page;
		// Unable to read the block
		if(!readBlock)
			return RC_READ_NOT_SUCCESSFULL;
		else
			return RC_OK;
	}
	// file containing less than pagenum pages
	else 
		return RC_READ_NON_EXISTING_PAGE;
}

//reads the next block 
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle->mgmtInfo == NULL)
		return RC_FILE_NOT_FOUND;
	int read_page;
        read_page =fHandle->curPagePos + 1;
	int seeker;
        seeker= fseek(fHandle->mgmtInfo, fHandle->curPagePos, read_page);
	int readBlock;
	fHandle->curPagePos = fHandle->curPagePos + 1;
	/*if( fHandle->totalNumPages < read_page)
		return RC_READ_NON_EXISTING_PAGE;*/
	if(seeker == 0 && !(fHandle->totalNumPages < read_page ))
                {
		readBlock = fread(memPage,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);
		fHandle->curPagePos = read_page;
		// Unable to read the block
		if(!readBlock)
			return RC_READ_NOT_SUCCESSFULL;
		else
			return RC_OK;
	        }
	// file containing less than pagenum pages
	else 
		return RC_READ_NON_EXISTING_PAGE;
}

//reads last block in relation to current position of the block
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle->mgmtInfo == NULL)
		return RC_FILE_NOT_FOUND;
	int read_page;
        read_page =fHandle->totalNumPages - 1;
	int seeker;
        seeker= fseek(fHandle->mgmtInfo, read_page, SEEK_END);
	int readBlock;
	/*fHandle->curPagePos = fHandle->curPagePos - 1;
	if( fHandle->totalNumPages < read_page)
		return RC_READ_NON_EXISTING_PAGE;*/
	if(seeker == 0 && !(fHandle->totalNumPages < read_page ))
	{
		readBlock = fread(memPage,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);
		fHandle->curPagePos = read_page;
		// Unable to read the block
		if(!readBlock)
			return RC_READ_NOT_SUCCESSFULL;
		else
			return RC_OK;
	}
	// file containing less than pagenum pages
	else 
		return RC_READ_NON_EXISTING_PAGE;
}

// writing blocks to a page file 
 RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	
	//FILE *myfile = fopen(fHandle->mgmtInfo,"r");
	fHandle->mgmtInfo = fopen(fHandle->fileName,"w+");
	if(fHandle->mgmtInfo == NULL)
		return RC_FILE_NOT_FOUND;
	
	int seeker;
        seeker= fseek(fHandle->mgmtInfo, pageNum*sizeof(char), SEEK_SET);
	int writeBlock;
	
	if(seeker == 0 &&!(fHandle->totalNumPages <= pageNum ))
              {
		writeBlock = fwrite(memPage,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);
		fHandle->curPagePos = pageNum;
		// Unable to write the block
		if(!writeBlock)
			return RC_WRITE_NOT_SUCCESSFULL;
		else
			return RC_OK;
	      }
	// file containing less than pagenum pages
	else 
		return RC_WRITE_FAILED;
	/*fclose(myfile);
	return RC_OK;*/
}

// writing current block to a page file
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	fHandle->mgmtInfo = fopen(fHandle->fileName,"w+");
	if(fHandle->mgmtInfo == NULL)
		return RC_FILE_NOT_FOUND;
	int write_page;
        write_page =getBlockPos(fHandle) ;
	int seeker;
        seeker= fseek(fHandle->mgmtInfo, fHandle->curPagePos, write_page);
	int writeBlock;
	fHandle->curPagePos = write_page;
	/*if( fHandle->totalNumPages < read_page)
		return RC_READ_NON_EXISTING_PAGE;*/

	if(seeker == 0 && !(fHandle->totalNumPages < write_page ))
             {
		writeBlock = fwrite(memPage,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);
		fHandle->curPagePos = write_page;
		// Unable to write the block
		if(!writeBlock)
			return RC_WRITE_NOT_SUCCESSFULL;
		else
			return RC_OK;
	      }
	// file containing less than pagenum pages
	else 
		return RC_WRITE_FAILED;
}

// Incrementing the pages by one till last page filling with 0 bytes
RC appendEmptyBlock (SM_FileHandle *fHandle)
{
	
	if(fHandle == NULL)
		return RC_FILE_NOT_FOUND;
	
	fHandle->mgmtInfo = fopen(fHandle->fileName,"a");
	fHandle->totalNumPages += 1;//increments totalnumpages by 1
	char *empty_block = (char *) calloc (PAGE_SIZE,sizeof(char));
	int seeker;
        seeker = fseek(fHandle->mgmtInfo, fHandle->totalNumPages*PAGE_SIZE, SEEK_END); // goes to end of page to append 
	int append_block;
        append_block = fwrite(empty_block, PAGE_SIZE, sizeof(char), fHandle->mgmtInfo);
        if(append_block == 1)
            {
		return RC_OK;
	    }
	else 
		return RC_WRITE_FAILED;
	free(empty_block);
}

// Adjusting to the size of the number of pages
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
	
	if(fHandle->mgmtInfo == NULL)
		return RC_FILE_NOT_FOUND;
	
	if(numberOfPages > fHandle->totalNumPages)
             {
		int fill_pages ;
                fill_pages = numberOfPages - fHandle->totalNumPages; // calculates the remaining pages to be filled
		fHandle->totalNumPages += numberOfPages;
		while(fill_pages < fHandle->totalNumPages)
                     {
		       appendEmptyBlock(fHandle);
		       fill_pages++;
		      }
	     }
	
	return RC_OK;
}
