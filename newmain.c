// CS450 - Shell - Sean Downes
// Line 164 Make check for !! or !n to avoid returning errors to main
// add showHistory routine, since used twice in code
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


#define MAX_LINE 80 /* The maximum length command */
static char * history[11][10];
int numCommands = 0;
int parentWait = 0;
int should_run = 1;

void showHistory()
{
    int tempCounter;
    int i,z;
    tempCounter = numCommands;
        printf("Command History:\n");
        for (i = tempCounter; i > 0; i--) // Iterate through history array, by number of commands read in
        {
            z = 0;
            printf("%d. ", i); // eg "3." 
            while (history[i][z] != 0) /* If array spot is not null, output it, and check next. Elements are
                                        Array is initialized as null (static) and re-nulled at line 73
                                        * when shifting history array */
            {	
                printf("%s ", history[i][z]); // Output command string and a space
                z++;                            // Increment 2D array loc
            }
            printf("\n");
            tempCounter--;
            if (tempCounter ==  0) break; // Reached number of commands entered in total
        }
}

int repeatCommand(char *args[])
{
    int y , z;
    
    y = args[0][1]; // integer value of Second char
        if(args[0][1] == '!') // Check if second char is a !
        {
            if(numCommands == 1) // No commands ran except !!
            {
                printf("No Commands in History\n");
                return 1;
            }
            
            z = 0;
            while (history[1][z] != 0)
                {	
                    args[z] = history[1][z];
                    z++;
                }
            while(args[0][0] == '!')
            {   
                if (args[0][1] == '!')
                {
                    z = 0;
                    while (history[1][z] != 0)
                    {	
                        args[z] = history[1][z];
                        z++;
                    }
                }
            }
                return 1; // Refuse multiple layers of !! commands
        }
        if(strcmp(args[0], "history") == 0) //Check for history input token, same code as before, make a function?
        {
            showHistory();
            return 1; // Do not return args to exec
        }
           //return 0; // return indexed history command to main. Make a check for !! and !n to avoid errors
       
        else if (y > 57 || y < 49) // Index larger than history array, 1 thru 9
        {
            printf("Error, only 1-9 accepted.\n");
            return 1;
        }   
        else         // Correct index point for commands
        {
            y = y - 48; // Convert ascii to int value
            z = 0;
            while (history[numCommands - y][z] != 0)
            {
                args[z] = history[numCommands - y][z];
                z++;
            }
        }
    
}



void clearArgs(char *args[])
{
    int x;
    for (x = 0; x < 10; x++) // Reset arguments
            {
                args[x] = 0;
            }
}

int getInput(char *args[])
{
    int i = 0;
    int y = 0;
    int z = 0;
    int numTokensRead = 0;
    int tempCounter = 0;
    char *token;
    char *commandLine = NULL;
    
    size_t bufSize = MAX_LINE;
    commandLine = (char *) malloc(MAX_LINE);
    
    getline(&commandLine, &bufSize, stdin);
   
     token = strtok(commandLine, " \n"); 
    if (token != NULL) 
    {
        while(token != NULL) 
        {
           // printf("%s\n", token);
            if (token[0] == 38) // Check for ASCII value of &
            {
                parentWait = 1; // found an &
                token = NULL;
            }
        
            args[i] = token;
            i++;
            numTokensRead++;
            token = strtok(NULL, " \n");
        }
    }
    if(numTokensRead == 0) 
    {
        //printf("No command entered\n");
        return 1;
    }
     
    if(numCommands > 10) numCommands = 10; // Limit to 10 for history array
     
    if (numCommands == 10) // Check if need to shift elements down history array
    {
       
        
        z = 0;
        while (history[1][z] != 0)
        {
            history[0][z] = history[1][z];
            history[1][z] = 0;
            z++;
        }
        for (i = 1; i < 11; i++) // Shift history elements
        {
            z = 0;
            tempCounter = 0;
            
            while (history[i + 1][z] != 0)
            {
                history[i][z] = history[i + 1][z]; // Each element is shifted down 1 index
                z++;
            }
            while (tempCounter < 10)
            {
                history[i + 1][tempCounter] = 0; // Null out old entries so no straggler commands remain
                tempCounter++;
            }
            
        }
    }
    numCommands++; // Increment total number of commands read in by shell
    if(numCommands > 10) numCommands = 10; // Limit to 10 for history array
    tempCounter = numTokensRead;
    while(tempCounter > -1) // need to change to strcpy?
    {
        if (numTokensRead > 0)
        {
            history[numCommands][tempCounter] = args[tempCounter]; // Add token to most recent history
            tempCounter--;
        }
        else 
        {
            
        }
        
    }
    
   
        
    
    if(strcmp(args[0], "history") == 0) //Check for history input token
    {
        showHistory();
        return 1; // Do not return args to exec
    }
    
    else if(strcmp(args[0], "exit") == 0)
    {
        should_run = 0;
        return 1;
    }
    
    else if(args[0][0] == '!') // Check if first char is a !
    {
        y = args[0][1]; // integer value of second char
        if(args[0][1] == '!') // Check if second char is a !
        {
            if(numCommands == 1) // No commands ran except !!
            {
                printf("No Commands in History\n");
                return 1;
            }
            
            z = 0;
            while (history[numCommands -1][z] != 0)
                {	
                    args[z] = history[numCommands-1][z];
                    z++;
                }
            if(args[0][1] == '!' || args[0][0] == '!' ) return 1; // Refuse multiple layers of !! commands
            if(strcmp(args[0], "history") == 0) //Check for history input token, same code as before, make a function?
            {
                showHistory();
                return 1; // Do not return args to exec
            }
            return 0; // return indexed history command to main. Make a check for !! and !n to avoid errors
        }
        else if (y > 57 || y < 49) // Index larger than history array, 1 thru 9
        {
            printf("Error, only 1-9 accepted.\n");
            return 1;
        }   
        else         // Correct index point for commands
        {
            y = y - 48; // Convert ascii to int value
            if (numCommands == 10) y--; // Decrement since items have shifted down a spot
            z = 0;
            while (history[y][z] != 0)
            {
                args[z] = history[y][z];
                z++;
            }
        }
    }

       return 0;
}

int main(void)
{
   
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int x;
    pid_t pid;
    clearArgs(args);
    
    while (should_run) {

        printf("osh>");
        fflush(stdout);
        parentWait = 0; 

        if(getInput(args) == 0) // Get input command, only run on return of 0
        {
            pid = fork();

            if (pid < 0) 
            { 
                /* error occurred */ printf("Fork Failed"); 
                return 1;
            }       
            if (pid == 0) // Child process
            {
            	if (execvp(args[0], args) == -1) // Return -1 on error
                {
                    printf("Error executing command\n");
                               
                }
            }
                
            else if(pid > 0) // Parent Process
            {
                if (parentWait == 0) // Default to wait for child, if == 1, parent will run concurrently
                {
                    wait(NULL);
                   
                }
            }
            clearArgs(args);
           
   	 }
     }
        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) if command included &, parent will invoke wait() */

return 0;
}
