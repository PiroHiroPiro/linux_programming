#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>

struct cmd
{
    char **argv;
    int argc;  /* used length of argv */
    long capa;  /* allocated length of argv */
};

static struct cmd* read_cmd(void);
static struct cmd* parse_cmd(char *cmdline);
static void* safe_malloc(size_t sz);
static void* safe_realloc(void *ptr, size_t sz);
static void invoke_cmd(struct cmd *cmd);

#define PROMPT "$ "
#define BUFFER_SIZE 2048
#define INIT_CAPA 16

int main(int argc, char *argv[])
{
    while (1)
    {
        struct cmd *cmd;

        fprintf(stdout, PROMPT);
        fflush(stdout);
        cmd = read_cmd();
        if (cmd->argc > 0)
        {
            invoke_cmd(cmd);
        }
        free(cmd->argv);
        free(cmd);
    }
    exit(0);

    return 1;
}

static struct cmd* read_cmd(void)
{
    static char buf[BUFFER_SIZE];

    if (fgets(buf, BUFFER_SIZE, stdin) == NULL)
    {
        exit(0);    /* allow exit by Ctrl-D (EOF) */
    }
    return parse_cmd(buf);
}

static struct cmd*
parse_cmd(char *cmdline)
{
    char *p = cmdline;
    struct cmd *cmd;

    cmd = safe_malloc(sizeof(struct cmd));
    cmd->argc = 0;
    cmd->argv = safe_malloc(sizeof(char*) * INIT_CAPA);
    cmd->capa = INIT_CAPA;
    while (*p)
    {
        while (*p && isspace((int)*p))
        {
            *p++ = '\0';
        }
        if (*p)
        {
            if (cmd->capa <= cmd->argc + 1)  /* +1 for final NULL */
            {
                cmd->capa *= 2;
                cmd->argv = safe_realloc(cmd->argv, cmd->capa);
            }
            cmd->argv[cmd->argc] = p;
            cmd->argc++;
        }
        while (*p && !isspace((int)*p))
        {
            p++;
        }
    }
    cmd->argv[cmd->argc] = NULL;
    return cmd;
}

static void* safe_malloc(size_t size)
{
    void *p;

    p = malloc(size);
    if (!p)
    {
        perror("malloc");
        exit(1);
    }
    return p;
}

static void* safe_realloc(void *ptr, size_t size)
{
    void *p;

    if (!ptr) return safe_malloc(size);
    p = realloc(ptr, size);
    if (!p)
    {
        perror("realloc");
        exit(1);
    }
    return p;
}


static void invoke_cmd(struct cmd *cmd)
{
    pid_t pid;

    pid = fork();
    if(pid < 0)
    {
        perror("fork");
        exit(1);
    }
    if(pid > 0)
    {
        waitpid(pid, NULL, 0);
    }
    else
    {
        execvp(cmd->argv[0], cmd->argv);
        fprintf(stderr, "command not found: %s\n", cmd->argv[0]);
        exit(1);
    }

}
