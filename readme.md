.........................................................................................
ADO ASSIGNMENT 1 - STORAGE MANAGER
..........................................................................................

-->Instruction to run
  Clear the screen and
  1. Navigate to the folder where the assignment 1 files are present in the terminal 
  2. Run the make file with the command: make -f makefile.mk
  3. Then Execute : ./assign1
.........................................................................................
->Functions used :
/* We have added additional test cases as exceptions and we have also added additional error and return messages */

  1.createPageFile :
    .open in read and write mode 
    .allocate memory 
    .check if file is Null ; if "File Present " else 
    .open file and write using seek
    .close and free the pointer and return "RC_OK"

  2.openPageFile :
    . open file in read mode
    . check if file is null ; if " file not found " else 
    . mgmtInfo gives the file pointer 
    . current position is zero and totalnumpages is 1 
    . closes and frees the pointer 

 3. closePageFile:
    .checks if file is null ; if file not found else 
    .opens in read mode 
    .closes the file using fclose
    .checks if file is closed and returns RC_OK
 
 4. destroyPageFile:
     .opens the file in read mode 
     .checks if null; if file not found else
     .remove the file 
     Check if file is removed if RC_OK else not deleted 
 
 5. readBlock:
     .open the file in read mode 
     .check if the mgmtInfo is null or not 
     . set the seeker to read the block 
     .checks if totalnumpages is less than pagenum 
     . if true then it reads the block with fread and the information from file pointer
     . set currpagepos to pagenum 
     .close the file 

 6. getBlockPos :
     .This function is to return the current position 

 7. readFirstBlock
     .open in read mode 
     .check if the file is present or not
     . use seek to point to the file and read the block 
     . same as read function 
     .but we set the currpagepos initially to zero and then point to the read_page 
 
 8. readPreviousBlock :
     .check if file present or not 
     .set the readpage to currpagepos-1
     .use seek and read the block 
     .Then same as read function 

 9. readCurrentBlock :
     .Same as read function except 
     .set read page using getblockpos function to retrieve the current page of the file 
 
 10. readNextBlock :
      . Same as read function except 
      . set the read page as curpagepos+1
 
 11. readLasttBlock :
      .Same as read function except 
      . read page is set to the last page (totalnumpages-1)

 12. WriteBlock :
     .open the file in write mode 
     . if file is null return file not found 
     . set write page using getblockpos function 
     . check if totalnumpages is less the pagenum
     . with seek write to file 
     .return ok and close the file 

 13. writeCurrentBlock :
      .open in write mode 
      . same as write block but here we will set write page to currentpagepos and also we check totalnumoages is less than write page
      .check if writing is successful else return write failed 

 14. appendEmptyBlock:
     .check if file is null or not 
     .open in append mode 
     . allocate the memory 
     .using seek and fwrite append a last block by incrementing totalnumpages by 1 
     .check if appended and the free the pointer 
 
 15. ensureCapacity :
      .check if file is present or not 
      . check if numofpages is more than totalnumpages 
      .increment the totalnumpages by numofpages 
      . check the insufficient size and append the pages using a while loop 

........................................................................................
->Team Members
   1)Minakshy Ramachandran A20396350 
   2)Nikita Jadhav A20401223
   3)Deepak Govind Mukundan A20396051
.........................................................................................






