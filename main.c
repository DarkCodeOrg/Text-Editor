// ******  INCLUDES ************//
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<termios.h>
#include<errno.h>

/******** Defines *********/
#define CTRL_KEY(k)  ((k) & 0x1f)  //  this CTRL_KEY macro bitwise and's a key (k) with the binaryvalue 00011111 (0x1f) i.e it turns off the upper 3 bits of the character to zero 
                                    // it depicts the function of ctrl-key which strips off the 5th and 6th bit from the char which u send in combination with it
/****** DATA *******/
struct termios orig_termios; // the original terminal attributes are stored in this variable orig_termios

/******** TERMINAL ********/
void die(const char *s)
{
    perror(s);   // perror() comes from <stdio.h>, and exit() comes from <stdlib.h>.
                 // Most C library functions that fail will set the global errno variable to indicate what the error was. 
                 // perror() looks at the global errno variable and prints a descriptive error message for it. It also prints the string given to it before it prints the error message, which is meant to provide context about what part of your code caused the error.
    exit(1);
}
void disableRawmode()
{
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig_termios) == -1) // tcsetattr(), tcgetattr(), and read() all return -1 on failure, and set the errno value to indicate the error.
    die("tcsetattr");
}
void enableRawmode()
{
    if(tcgetattr(STDIN_FILENO, &orig_termios) == -1)die("tcgetattr"); // The tcgetattr() function shall get the parameters associated with the terminal referred to by fildes and store them in the termios structure referenced by raw (here). The fildes argument is an open file descriptor associated with a terminal
    atexit(disableRawmode);
    struct termios raw = orig_termios ;
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | ISTRIP | INPCK ); // disbe ctrl-s and ctrl-q
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_cflag &= ~(ECHO | ICANON | ISIG | IEXTEN);  // ICANON is for disabling the canonical mode // ISIG id for disabling ctrl-c and ctrl-Z signals // IEXTEN is for disabling ctrl-V
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw) == -1)die(tcsetattr);

}
char editorReadKey() // its work is to wait for a KeyPress and return it
{  
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO,&c,1) != 1)){
        if (nread == -1 && errno != EAGAIN) die("read");// Read is called to read 1 byte from stdin until there are no more bytes to read i.e the result becomes Zero
                                                        // In Cygwin, when read() times out it returns -1 with an errno of EAGAIN, instead of just returning 0 like it’s supposed to. To make it work in Cygwin, we won’t treat EAGAIN as an error.
    }
    return c;
    
}

/****** input ************/
void editorProcessKeypress()  // waits for a keypress and then handles it 
{
    char c = editorReadKey();

    switch (c)
    {
    case CTRL_KEY('q'):
           exit(0);
           break;
    
    default:
        break;
    }
}

/******** output ***********/
void editorRefreshScreen()
{
    write(STDOUT_FILENO,'\x1b[2J',4);
}
int main()
{
    enableRawmode();
    while(1){
      editorRefreshScreen();
      editorProcessKeypress();

    }

    return 0;
}