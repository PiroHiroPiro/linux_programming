#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i;

    for(i = 1; i < argc; i++)
    {
        FILE *f;
        int c;

        f = fopen(argv[i], "r");
        if(!f)
        {
            perror(argv[i]);
            exit(1);
        }
        while((c = fgetc(f)) != EOF)
        {
            if(c == 9)  // \t => '\t'
            {
                if (putchar(92) < 0) exit(1);
                if (putchar(116) < 0) exit(1);
            }
            else if(c == 10)  // \n => '$' + \n
            {
                if (putchar(36) < 0) exit(1);
                if (putchar(10) < 0) exit(1);
            }
            else
            {
                if (putchar(c) < 0) exit(1);
            }
        }
        fclose(f);
    }
}
