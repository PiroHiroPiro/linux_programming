#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

int n = 0;
int t = 0;

static struct option longopts[] = {
    // {"tab", no_argument, &t, 1},
    // {"newline", no_argument, &n, 1},
    {"tab", no_argument, NULL, 't'},
    {"newline", no_argument, NULL, 'n'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0}
};

int main(int argc, char *argv[])
{
    int opt;

    while((opt = getopt_long(argc, argv, "tn", longopts, NULL)) != -1)
    {
        switch (opt)
        {
            case 't':
                t = 1;
                break;
            case 'n':
                n = 1;
                break;
            case 'h':
                fprintf(stdout, "Usage: %s [-t] [-n] [FILE ...]\n", argv[0]);
                exit(0);
            case '?':
                fprintf(stderr, "Usage: %s [-t] [-n] [FILE ...]\n", argv[0]);
                exit(1);
        }
    }

    int i;

    for(i = optind; i < argc; i++)
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
            if(t == 1 && c == 9)  // \t => '\t'
            {
                if(putchar(92) < 0) exit(1);
                if(putchar(116) < 0) exit(1);
            }
            else if(n == 1 && c == 10)  // \n => '$' + \n
            {
                if(putchar(36) < 0) exit(1);
                if(putchar(10) < 0) exit(1);
            }
            else
            {
                if(putchar(c) < 0) exit(1);
            }
        }
        fclose(f);
    }
}
