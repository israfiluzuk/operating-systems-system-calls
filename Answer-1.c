#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER 64

char *read_command(void);
char **parse_line(char *line);
int execute(char **arguments);

int main(void)
{
    char *command = NULL;
    char **arguments;
    int status;
    char str[10] = "%";
    char ends[10] = "ends";
    char stop[10] = "halt";

    do
    {
        printf("%s", str);
        command = read_command();
        if(strcmp(command,ends) == 0) command=stop;
        arguments = parse_line(command);
        status = execute(arguments);

        free(arguments);
        free(command);

    }while(status);
}

char *read_command(void)
{
    char *command = NULL;
    ssize_t buf = 0;
    getline(&command, &buf, stdin);

    return command;
}

char **parse_line(char *line)
{
    int buffer = BUFFER;
    int pos = 0;
    char **tokens = malloc(buffer * sizeof(char*));
    char *token;
    if (!tokens)
    {
        fprintf(stderr, "Error allocating memory with malloc\n");
      
        exit(0);
    }
    token = strtok(line, " \t\r\n\a");
    while(token != NULL)
    {
        tokens[pos] = token;
        pos++;

        if (pos >= buffer)
        {
            buffer += BUFFER;
            tokens = realloc(tokens, buffer * sizeof(char*));
            if (!tokens)
            {
               fprintf(stderr, "Error reallocating memory!\n");
                exit(0);
            }
        }
        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[pos] = NULL;
    return tokens;
}

int execute(char **arguments)
{
   
    int pid, waitPid, status;

    pid = fork();

    if(pid == 0)    //child process
    {
        if (execvp(arguments[0], arguments) == -1)
            fprintf(stderr, "Error with EXECVP\n");
    }
    else if (pid < 0)
        perror("Error PID < 0\n");
    else    //parent process
    {
        do
        {
            waitPid = waitpid(pid, &status, WUNTRACED);
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}