#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <unistd.h>

static void do_grep(regex_t *pat, FILE *f);

// -i, --ignore-case         ignore case distinctions     大文字小文字を区別しない
// -v, --invert-match        select non-matching lines    マッチしないものを表示

int opt_v = 0;

int main(int argc, char *argv[])
{
    regex_t pat;
    int err;
    int opt;
    int opt_i = 0;
    char *pattern;

    if(argc < 2)
    {
        fputs("no pattern\n", stderr);
        exit(1);
    }

    while((opt = getopt(argc, argv, "ive:h")) != -1)
    {
        switch (opt)
        {
            case 'i':
                opt_i = 1;
                break;
            case 'v':
                opt_v = REG_NOMATCH;
                break;
            case 'e':
                pattern = optarg;
                break;
            case 'h':
                fprintf(stdout, "Usage: %s -e Regex [-i] [-v] [FILE ...]\n", argv[0]);
                exit(0);
            case '?':
                fprintf(stderr, "Usage: %s -e Regex [-i] [-v] [FILE ...]\n", argv[0]);
                exit(1);
        }
    }

    printf("opt_i: %d\n", opt_i);
    printf("opt_v: %d\n", opt_v);
    printf("pattern: %s\n", pattern);

    if(opt_i == 0)
    {
        err = regcomp(&pat, pattern, REG_EXTENDED | REG_NOSUB | REG_NEWLINE);
        if(err != 0)
        {
            char buf[1024];

            regerror(err, &pat, buf, sizeof buf);
            puts(buf);
            exit(1);
        }
    }
    else if(opt_i == 1)
    {
        err = regcomp(&pat, pattern, REG_EXTENDED | REG_NOSUB | REG_NEWLINE | REG_ICASE);
        if(err != 0)
        {
            char buf[1024];

            regerror(err, &pat, buf, sizeof buf);
            puts(buf);
            exit(1);
        }
    }

    if(optind == argc)
    {
        do_grep(&pat, stdin);
    }
    else
    {
        int i;

        printf("optind: %d\n", optind);

        for(i = optind; i < argc; i++)
        {
            FILE *f;

            f = fopen(argv[i], "r");
            if(!f)
            {
                perror(argv[i]);
                exit(1);
            }
            do_grep(&pat, f);
            fclose(f);
        }
    }

    regfree(&pat);
    exit(0);
}

static void do_grep(regex_t *pat, FILE *src)
{
    char buf[4096];

    while(fgets(buf, sizeof buf, src))
    {
        if(regexec(pat, buf, 0, NULL, 0) == opt_v)
        {
            fputs(buf, stdout);
        }
    }
}
