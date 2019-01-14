#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 2048

int main(int argc, char *argv[])
{
    int i;
    FILE *f;
    unsigned char path[BUFFER_SIZE];
    unsigned char buf[BUFFER_SIZE];

    for(;;)
    {
        fgets(path, sizeof path, stdin);
        sscanf(path, "%s", path);

        f = fopen(path, "r");
        if(!f)
        {
            perror(path);
            exit(1);
        }

        unsigned char line_count;
        line_count = 0;

        while(fgets(buf, sizeof buf, f) != NULL)
        {
            line_count++;
        }

        printf("%d\n", line_count);

        fclose(f);
    }
}
