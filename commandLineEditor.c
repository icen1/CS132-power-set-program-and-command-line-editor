/* Importing modules */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

/* Declare the size of the arrays that i will be using */
#define BUFFSIZE 255

/* Declare the size that i will be using with my log */
#define EXTRABUFFSIZE 4096

/* Sets the value of True to 1 */
#define True 1

/* Sets the value of False to 0*/
#define False 0

/* Declaring functions that will be used ahead of their actual declartion */
int appendLineForLog(char const fileToAppend[BUFFSIZE], char const dataToAppend[BUFFSIZE]);
void printFile(FILE *filePointer);
void deleteLine(FILE *srcFile, FILE *tempFile, const int line);
void insertLine(FILE *srcFile, FILE *tempFile, const int line, char insertedLine[BUFFSIZE]);
int indexOf(FILE *filePointer, const char *word, int *line, int *column);

/**
 * @brief Asks user for input and copies the string the user inputs to another so it can be used in the program.
 * Includes error checking.
 * @param stringToCopy Copies what the user inputted to it
 * @return 0 if the operation is succesful
 */
int userinput(char stringToCopy[BUFFSIZE])
{
    size_t slen;
    char string[BUFFSIZE];

    /* read input of one string, with error checking */
    if (fgets(string, BUFFSIZE, stdin) == NULL)
    {
        fprintf(stderr, "Error reading string into string.\n");
        exit(EXIT_FAILURE);
    }
    /* removing newline from string, along with checking for overflow from string */
    slen = strlen(string);
    if (slen > 0)
    {
        if (string[slen - 1] == '\n')
        {
            string[slen - 1] = '\0';
        }
        else if (slen >= BUFFSIZE)
        {
            printf("Exceeded string length of %d.\n", BUFFSIZE);
            exit(EXIT_FAILURE);
        }
    }

    /* checking if nothing was entered */
    if (!*string)
    {
        printf("No string entered.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(stringToCopy, string);
    return 0;
}

/**
 * @brief Creates a file with the name the user inputted.
 * @return 0 if the creation is succesful, and exits if it is not.
 */
int createFile()
{
    /* Declare the file pointer */
    FILE *filePointer;

    /* Declare the string that the log message will be saved to*/
    char logMessage[BUFFSIZE]; 

    /* Get the data to be written in file */
    printf("Enter the name you want to save the file as, with its extension: ");
    char fileName[BUFFSIZE];
    userinput(fileName);

    /* Open the existing file using fopen() in write mode using "w" attribute */
    filePointer = fopen(fileName, "w");

    /* Check if this filePointer is null which would be the case if the file does not exist */
    if (filePointer == NULL)
    {
        printf("File file failed to be created.");
                
        snprintf(logMessage, EXTRABUFFSIZE,"failed to create file %s\n", fileName);
        appendLineForLog("log.txt", logMessage);
        exit(EXIT_FAILURE);
    }
    else
    {

        /* Closing the file using fclose() */
        fclose(filePointer);
        
        snprintf(logMessage, EXTRABUFFSIZE,"Created file %s\n", fileName);
        appendLineForLog("log.txt", logMessage);
        
        printf("The file is now created.\n");
    }
    return 0;
}

/**
 * @brief Copies the content of the source file to another file. The user gets to choose both files
 * @return 0 if the copying of data is successful, and exits the program if it is not.
 */
int copyFile()
{
    /* Declare the file names for both source and target fike, also the ch that will be used to copy every character
       to the targer file */
    char ch; 
    char source_file[BUFFSIZE]; 
    char target_file[BUFFSIZE];
    
    /* Declare the string that the log message will be saved to*/
    char logMessage[BUFFSIZE]; 
    
    FILE *source; 
    FILE *target;
    
    printf("Enter name of file to copy data from: ");
    userinput(source_file);

    source = fopen(source_file, "r");

    if (source == NULL)
    {
        printf("File doesn't exist, please create the file first\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter name of target file: ");
    userinput(target_file);

    target = fopen(target_file, "w");

    if (target == NULL)
    {
        fclose(source);
        printf("Target file doesn't exist, please create the file first\n");
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(source)) != EOF)
    {
        fputc(ch, target);
    }
    printf("File copied successfully.\n");
    snprintf(logMessage, EXTRABUFFSIZE,"Copied file %s to %s\n", source_file, target_file);
    appendLineForLog("log.txt", logMessage);
    fclose(source);
    fclose(target);

    return 0;
}

/**
 * @brief Deletes a file the user specifies.
 * @return 0 if the file is deleted successfully, and exits the program if it didn't.
 */
int deleteFile()
{
    char fileToDelete[BUFFSIZE];
    
    /* Declare the string that the log message will be saved to*/
    char logMessage[BUFFSIZE]; 

    printf("The file name you want to delete: ");
    userinput(fileToDelete);
    if (remove(fileToDelete) == 0)
    {
        printf("Deleted successfully\n");
        snprintf(logMessage, EXTRABUFFSIZE, "Deleted %s\n",fileToDelete);
    }
    else
    {
        printf("Unable to delete the file\n");
        snprintf(logMessage, EXTRABUFFSIZE,"Unable to delete %s\n",fileToDelete);
    }
    appendLineForLog("log.txt", logMessage);

    return 0;
}

/**
 * @brief Reads the contents of a file the user specifies line by line.
 * @return 0 if the file can be read, and it exits the program if sufficeient permissions are not given.
 */
int readFile()
{
    /* File pointer to hold reference to our file */
    FILE *filePointer;
    char ch;
    char fileToRead[BUFFSIZE];

    /* Declare the string that the log message will be saved to*/
    char logMessage[BUFFSIZE]; 

    printf("Enter the file name you want to read: ");
    userinput(fileToRead);
    filePointer = fopen(fileToRead, "r");

    /* fopen() return NULL if last operation was unsuccessful */
    if (filePointer == NULL)
    {
        /* Unable to open file hence exit */
        printf("Unable to open file.\n");
        printf("Please check whether file exists and you have read privilege.\n");
        snprintf(logMessage, EXTRABUFFSIZE,"Couldn't read %s\n", fileToRead);
        appendLineForLog("log.txt", logMessage);
        exit(EXIT_FAILURE);
    }

    /* File open success message */
    printf("File opened successfully. Reading file contents character by character.\n");
    snprintf(logMessage, EXTRABUFFSIZE,"Read file %s\n", fileToRead);
    appendLineForLog("log.txt", logMessage);
    
    /* Repeat this if last read character is not EOF */
    do
    {
        /* Read single character from file */
        ch = fgetc(filePointer);

        /* Print character read on console */
        putchar(ch);

    } while (ch != EOF);
    printf("\n");

    /* Done with this file, close file to release resource */
    fclose(filePointer);

    return 0;
}

/**
 * @brief Adds a line at the end of the file specified by the user.
 * @return 0 if the line was added and exits if the file can't be accessedin read/write mode.
 */
int appendLine()
{
    /* File pointer to hold reference of input file */
    FILE *filePointer;

    char fileToAppend[BUFFSIZE];
    char dataToAppend[BUFFSIZE];

    /* Declare the string that the log message will be saved to*/
    char logMessage[BUFFSIZE]; 

    /* Input file path to remove empty lines from user */
    printf("Enter file name you want to append a line to: ");
    userinput(fileToAppend);

    /*  Open all file in append mode. */
    filePointer = fopen(fileToAppend, "a");

    /* fopen() return NULL if unable to open file in given mode. */
    if (filePointer == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open '%s' file.\n", fileToAppend);
        printf("Please check whether file exists and you have write privilege.\n");
        snprintf(logMessage, EXTRABUFFSIZE, "Couldn't open file  %s\n",fileToAppend);
        appendLineForLog("log.txt", logMessage);
        exit(EXIT_FAILURE);
    }

    /* Input data to append from user */
    printf("Enter data to append: ");
    userinput(dataToAppend);

    /* Append data to file */
    fputs(dataToAppend, filePointer);
    printf("Successfully appended data to file. \n");
    snprintf(logMessage, EXTRABUFFSIZE,"Appended line %s to file %s\n", dataToAppend, fileToAppend);
    appendLineForLog("log.txt", logMessage);

    /* Done with file, hence close file. */
    fclose(filePointer);

    return 0;
}

/**
 * @brief Combines the delete and insert line code in one function as they are very similar. Asks the user for the file name,
 * checks if it exists and creates a temporary file. Asks which line the user wants to operate on, and goes to the
 * delete or insert function accordingly. It then deletes the source file, and renames the temporary file to the source
 * file name with the operation finished, and then prints the new content of the file.
 * @param isDelete Checks if the user wants to delete a line or inser a line
 * @return 0 if the operation is successful and exits the program if anything foes wrong.
 */
int deleteInsert(int isDelete)
{
    FILE *srcFile;
    FILE *tempFile;
    int line;

    char fileName[BUFFSIZE];
    char insertedLine[BUFFSIZE];
    char *endPointer;
    char lineToOperate[BUFFSIZE];
    
    /* Declare the string that the log message will be saved to*/
    char logMessage[BUFFSIZE]; 

    /* Input file fileName and line number */
    if(isDelete == True)
    {
        printf("Enter file name you want to delete line in: ");
        userinput(fileName);
    }
    else
    {
        printf("Enter file name you want to insert line in: ");
        userinput(fileName);
    }

    /* try to open file */
    srcFile = fopen(fileName, "r");
    tempFile = fopen("tempFile.txt", "w");

    /* Exit if file not opened successfully */
    if (srcFile == NULL || tempFile == NULL)
    {
        printf("Unable to open file.\n");
        printf("Please check you have read/write privileges.\n");
        snprintf(logMessage, EXTRABUFFSIZE,"Couldn't operate on the file %s, possibly due to no having the required priveliges\n", fileName);
        appendLineForLog("log.txt", logMessage);

        exit(EXIT_FAILURE);
    }

    printf("\nFile contents before operating on line.\n\n");
    printFile(srcFile);
    printf("\n");

    printf("Enter line number to operate on: ");
    userinput(lineToOperate);
    line = strtol(lineToOperate, &endPointer, 10);

    /* Move src file pointer to beginning */
    rewind(srcFile);

    /* Operates on given line from file.*/

    if (isDelete == True)
    {
        deleteLine(srcFile, tempFile, line);
        printf("deleted line %d in %s\n", line, fileName);
        snprintf(logMessage, EXTRABUFFSIZE,"deleted line %d in %s\n", line, fileName);
        appendLineForLog("log.txt", logMessage);
    }
    else
    {
        printf("Input the line you want to insert: ");
        userinput(insertedLine);
        insertLine(srcFile, tempFile, line, insertedLine);
        printf("inserted line %s to %d in %s\n", insertedLine, line, fileName);
        snprintf(logMessage, EXTRABUFFSIZE,"inserted line %s to %d in %s\n", insertedLine, line, fileName);
        appendLineForLog("log.txt",logMessage);
    }

    /* Close all open files */
    fclose(srcFile);
    fclose(tempFile);

    /* Delete src file and rename temp file as src */
    remove(fileName);
    rename("tempFile.txt", fileName);

    return 0;
}

/**
 * @brief prints contents of a file. Different from readFile as this one was used inside other functions without the need of
 * the user to (re-)specify the file name, since they already specified it before.
 * @param filePointer the file we want to print its contents
 */
void printFile(FILE *filePointer)
{
    char ch;

    while ((ch = fgetc(filePointer)) != EOF)
    {
        putchar(ch);
    }
}

/**
 * @brief Function to delete a given line from file.
 * @param srcFile The file to delete the line from
 * @param tempFile The file will copy the original file to without the line to be deleted
 * @param line the liine number in the file to delete
 */
void deleteLine(FILE *srcFile, FILE *tempFile, const int line)
{
    char buffer[BUFFSIZE];
    int count = 1;

    while ((fgets(buffer, BUFFSIZE, srcFile)) != NULL)
    {
        /* If current line is not the line user wanted to remove */
        if (line != count)
        {
            fputs(buffer, tempFile);
        }
        count++;
    }
}

/**
 * @brief Function to insert a given line to a file.
 * @param srcFile the file we want to insert the line to
 * @param tempFile is the temporary file we will copy the file to and insert the extra line
 * @param line the line number we want to insert the line to
 * @param insertedLine The data we want to insert
 */
void insertLine(FILE *srcFile, FILE *tempFile, const int line, char insertedLine[BUFFSIZE])
{
    char buffer[BUFFSIZE];
    int count = 1;

    while ((fgets(buffer, BUFFSIZE, srcFile)) != NULL)
    {
        /* If current line is not the line user wanted to remove */
        if (line == count)
        {
            fputs(insertedLine, tempFile);
            fputs("\n", tempFile);
            fputs(buffer, tempFile);
        }
        else
        {
            fputs(buffer, tempFile);
        }
        count++;
    }
}

/**
 * @brief Shows a line the user specifies from the file the user wants
 * @return 0 if operation is successful and exits program if it isn't
 */
int showLine()
{
    FILE *filePointer;
    
    char fileToRead[BUFFSIZE];
    char ch;
    char lineToShowChar[BUFFSIZE];
    char *endPointer;

    /* Declare the string that the log message will be saved to*/
    char logMessage[BUFFSIZE]; 
    
    int lineToShow;
    int line = 1;

    printf("Enter the file name you want to show a line in: ");
    userinput(fileToRead);
    filePointer = fopen(fileToRead, "r");

    do 
    {
    printf("Which line do you want to read, please input a positive number: ");
    userinput(lineToShowChar);

    lineToShow = strtol(lineToShowChar, &endPointer, 10);
    } while(lineToShow <= 0);
    while ((ch = fgetc(filePointer)) != EOF)
    {
        if (lineToShow == line)
        {
            putchar(ch);
        }
        if (ch == '\n')
        {
            line++;
        }
    }
    snprintf(logMessage, EXTRABUFFSIZE,"showed line %d in %s\n", lineToShow, fileToRead);
    appendLineForLog("log.txt", logMessage);
    fclose(filePointer);
    return 0;
}

/**
 * @brief Appends line in the log
 * @param fileToAppend Appends information in the file specified
 * @param dataToAppend The data it appends to the file
 * @return 0 if data is succesfully appended in the log
 */
int appendLineForLog(char const fileToAppend[BUFFSIZE], char const dataToAppend[BUFFSIZE])
{
    /* File pointer to hold reference of input file */
    FILE *filePointer;

    /*  Open all file in append mode. */
    filePointer = fopen(fileToAppend, "a");

    /* fopen() return NULL if unable to open file in given mode. */
    if (filePointer == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open %s file. No logging will take place this session\n", fileToAppend);
        printf("Please check whether file exists and you have write privilege.\n");
    }

    /* Append data to file */
    fputs(dataToAppend, filePointer);

    /* Done with file, hence close file. */
    fclose(filePointer);

    return 0;
}

/**
 * @brief Creates log file at the start of a program
 * @param fileName is the name of the file we want
 * @return 0 if the file is successfully created
 */
int createLog(char fileName[BUFFSIZE])
{
    /* Declare the file pointer */
    FILE *filePointer;

    /* Open the existing file using fopen() in write mode using "w" attribute */
    filePointer = fopen(fileName, "w");

    /* Check if this filePointer is null which might be the case if the file does not exist */
    if (filePointer == NULL)
    {
        printf("failed to create log file.");
    }
    else
    {
        /* Closing the file using fclose() */
        fclose(filePointer);
    }

    return 0;
}

/**
 * @brief Lists all the files in a the current folder by using a structure from the dirent library.
 * Uses readdir() to return the pointer to the directory at the current point in the directory stream
 * readdir() also returns null at the end so we know when the directory stream reaches its end.
 * @param folderName The current folder the program is in
 * @return 0 if the operation is successful, exits otherwise
 */
int listFiles(char folderName[BUFFSIZE])
{
    /* Pointer for directory entry */
    struct dirent *de;

    /* opendir() returns a pointer of DIR type. */
    DIR *dr = opendir(folderName);

    /* Declare the string that the log message will be saved to*/
    char logMessage[BUFFSIZE]; 

    /* opendir returns NULL if couldn't open directory */
    if (dr == NULL)
    {
        printf("Could not open current directory");
        appendLineForLog("log.txt", "Could not open the directory the file is in\n");
        exit(EXIT_FAILURE);
    }

    /* prints the file names by using the structure from dirent and readdir as loong as readdir value is not null */
    appendLineForLog("log.txt", "Files in current directory: \n");
    printf("Files in current directory: \n" );
    while ((de = readdir(dr)) != NULL)
    {
        printf("%s\n", de->d_name);
        snprintf(logMessage, EXTRABUFFSIZE,"%s ", de->d_name);
        appendLineForLog("log.txt",logMessage);
    }
    appendLineForLog("log.txt","\n");
    closedir(dr);
    return 0;
}

/**
 * @brief Finds the first occuernence of the word the user wants, and gives its line and column it was in.
 * @return 0 if the operartion is successful and exits if it is not
 */
int findWord()
{
    FILE *filePointer;
    
    char fileName[BUFFSIZE];
    char word[BUFFSIZE];

    /* Declare the string that the log message will be saved to*/
    char logMessage[BUFFSIZE]; 

    int line; 
    int column;

    /* Input file name */
    printf("Enter file name: ");
    userinput(fileName);

    /* Try to open file */
    filePointer = fopen(fileName, "r");

    /* Exit if file not opened successfully */
    if (filePointer == NULL)
    {
        printf("Unable to open file.\n");
        printf("Please check you have read/write privileges.\n");
        snprintf(logMessage, EXTRABUFFSIZE,"Couldn't open the file %s",fileName);
        appendLineForLog("log.txt",logMessage);

        exit(EXIT_FAILURE);
    }

    /* Input word to search in file */
    printf("Enter word to search in file: ");
    userinput(word);

    /* Find index of word in filePointer */
    indexOf(filePointer, word, &line, &column);

    /* Checks whether the word was found or not, and elt's the user know */
    if (line != -1)
    {
        printf("'%s' found at line: %d and column: %d\n", word, line + 1, column + 1);
        snprintf(logMessage, EXTRABUFFSIZE, "Found the first occurence of %s at line %d and column %d\n",word,line,column);
        appendLineForLog("log.txt", logMessage);
    }
    else
    {
        printf("'%s' does not exist.", word);
        snprintf(logMessage, EXTRABUFFSIZE, "Couldn't fing the word %s in the file\n",word);
        appendLineForLog("log.txt", logMessage);
    }

    
    /* Close file */
    fclose(filePointer);

    return 0;
}

/**
 * @brief Finds the first occurence of a word in the given file
 * @param filePointer the file stream to work on
 * @param word the word to find the first occurence of in a file
 * @param line the line number where the word is in
 * @param column find which column the first letter of the word the user wanted to look for
 * @return column so we know which column the word starts in
 */
int indexOf(FILE *filePointer, const char *word, int *line, int *column)
{
    char str[BUFFSIZE];
    char *pos;

    /* Sets them to -1 to make it easier to know whether there are any errors or not */
    *line = -1;
    *column = -1;

    while ((fgets(str, BUFFSIZE, filePointer)) != NULL)
    {
        *line += 1;

        /* Find first occurrence of word in string */
        pos = strstr(str, word);

        if (pos != NULL)
        {
            /* Finds first index of word in str and get its memory address using pos - memory address of str. */
            *column = (pos - str);
            break;
        }
    }

    /* If word is not found then set line to -1 again */
    if (*column == -1)
    {
        *line = -1;
    }
    return *column;
}

int main(int argc, char *argv[])
{
    int i = 1;

    /* Checks if there were no arguments beside the compiled code name, if it is, then it exits the program as there is
     * no commands to execute */
    if (argc == 1)
    {
        printf("One argument expected.\n");
        exit(EXIT_FAILURE);
    }
    createLog("log.txt");
    do
    {
        if (strcmp(argv[i], "create") == 0)
        {
            createFile();
            i++;
        }
        else if (strcmp(argv[i], "copy") == 0)
        {
            copyFile();
            i++;
        }
        else if (strcmp(argv[i], "delete") == 0)
        {
            deleteFile();
            i++;
        }
        else if (strcmp(argv[i], "show") == 0)
        {
            readFile();
            i++;
        }
        else if (strcmp(argv[i], "append") == 0)
        {
            appendLine();
            i++;
        }
        else if (strcmp(argv[i], "deleteLine") == 0)
        {
            deleteInsert(True);
            i++;
        }
        else if (strcmp(argv[i], "insertLine") == 0)
        {
            deleteInsert(False);
            i++;
        }
        else if (strcmp(argv[i], "showLine") == 0)
        {
            showLine();
            i++;
        }
        else if (strcmp(argv[i], "list") == 0)
        {
            listFiles(".");
            i++;
        }
        else if (strcmp(argv[i], "find") == 0)
        {
            findWord();
            i++;
        }
        else if (strcmp(argv[i], "exit") == 0)
        {
            printf("Closing program");
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Couldn't resolve argument");
            i++;
        }
    } while (i < argc );
}