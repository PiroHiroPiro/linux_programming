#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

static void do_traverse(char *path);
static char *filetype(mode_t mode);

int main(int argc, char *argv[])
{
    int i;

    if(argc < 2)
    {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }

    for(i = 1; i < argc; i++)
    {
        do_traverse(argv[i]);
    }
    exit(0);
}

static void do_traverse(char *path)
{
    DIR *d;
    char filepath[2048];
    struct dirent *ent;
    struct stat st;

    d = opendir(path);
    if(!d)
    {
        perror(path);
        exit(1);
    }

    while(ent = readdir(d))
    {
        strcpy(filepath, path);
        int pos = strlen(filepath) - 1;
        if(pos >= 0 && filepath[pos] != '/') strcat(filepath, "/");
        strcat(filepath, ent->d_name);

        printf("%s\n", filepath);

        if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;

        if(lstat(filepath, &st) < 0)
        {
            perror(filepath);
            exit(1);
        }
        if(filetype(st.st_mode) == "file" || filetype(st.st_mode) == "symlink") continue;
        if(filetype(st.st_mode) == "directory") do_traverse(filepath);
    }

    closedir(d);
}

static char *filetype(mode_t mode)
{
    if(S_ISREG(mode)) return "file";
    if(S_ISDIR(mode)) return "directory";
    if(S_ISCHR(mode)) return "chardev";
    if(S_ISBLK(mode)) return "blockdev";
    if(S_ISFIFO(mode)) return "fifo";
    if(S_ISLNK(mode)) return "symlink";
    if(S_ISSOCK(mode)) return "socket";

    return "unknown";
}
