#include <stdio.h>
#include <stdlib.h>

static void do_cat(FILE *f);

#define BUFFER_SIZE 2048

int main(int argc, char *argv[])
{
    int i;

    if(argc == 1){
        // do_cat(stdin);

        FILE *f;
        unsigned char path[BUFFER_SIZE];

        for(;;)
        {
            // scanf("%s", path);
            fgets(path, sizeof path, stdin);
            sscanf(path, "%s", path);

            f = fopen(path, "r");
            if (!f)
            {
                perror(path);
                exit(1);
            }
            do_cat(f);
            fclose(f);
        }
    }else{
        for(i = 1; i < argc; i++)
        {
            FILE *f;
            f = fopen(argv[i], "r");
            if (!f)
            {
                perror(argv[i]);
                exit(1);
            }
            do_cat(f);
            fclose(f);
        }
    }
}

static void do_cat(FILE *f)
{
    int c;
    while ((c = fgetc(f)) != EOF) {
        // if(fputc(c, stdout) < 0) exit(1);
        if (putchar(c) < 0) exit(1);
    }
}
