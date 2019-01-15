#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static void do_tail(FILE *f, long nlines);

int main(int argc, char *argv[])
{
    long nlines;

    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s n [file file ...]\n", argv[0]);
        exit(1);
    }

    nlines = atol(argv[1]);

    if(nlines <= 0)
    {
        fprintf(stderr, "Usage: %s n [file file ...]\n", argv[0]);
        exit(1);
    }

    int i;

    for(i = 2; i < argc; i++)
    {
        FILE *f;

        f = fopen(argv[i], "r");
        if(!f)
        {
            perror(argv[i]);
            exit(1);
        }

        do_tail(f, nlines);
        fclose(f);
    }

    exit(0);
}

static void do_tail(FILE *f, long nlines)
{
    int c;
    int offset;
    int i = 0;
    long idx = 0;
    long tail_idxs[nlines + 1];

    while((c = fgetc(f)) != EOF)
    {
        if(c == '\n')
        {
            for(i = 1; i <= nlines; i++)
            {
                tail_idxs[i - 1] = tail_idxs[i];
            }
            tail_idxs[nlines] = idx;
        }
        idx++;
    }

    rewind(f);
    idx = 0;
    while((c = fgetc(f)) != EOF)
    {
        if(idx > tail_idxs[0]) fputc(c, stdout);
        idx++;
    }
}
