#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<termios.h>

struct termios orig_termios; // the original terminal attributes are stored in this variable orig_termios
void disableRawmode()
{
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig_termios);
}

void enableRawmode()
{
    tcgetattr(STDIN_FILENO, &orig_termios); // The tcgetattr() function shall get the parameters associated with the terminal referred to by fildes and store them in the termios structure referenced by raw (here). The fildes argument is an open file descriptor associated with a terminal
    atexit(disableRawmode);
    struct termios raw = orig_termios ;
    raw.c_cflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw);

}


int main()
{
    enableRawmode();
    char c;
    while (read(STDIN_FILENO,&c,1) == 1)   // Read is called to read 1 byte from stdin until there are no more bytes to read i.e the result becomes Zero
    {
        
    }
    

    return 0;
}