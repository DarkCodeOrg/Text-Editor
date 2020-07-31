#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<termios.h>



int main()
{
    char c;
    while (read(STDIN_FILENO,&c,1) == 1)   // Read is called to read 1 byte from stdin until there are no more bytes to read i.e the result becomes Zero
    {
        
    }
    

    return 0;
}