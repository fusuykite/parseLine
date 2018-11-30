//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"

int main() {
    int i = 0;
    char cmd_line[MAX_CMD_LEN] = { 0 };
    char cmd_line_cpy[MAX_CMD_LEN] = { 0 };
    char *arg_token[MAX_CMD_LEN];
    char *pipe_token[MAX_CMD_PIPE];
    int *arg_count = calloc(1, sizeof(int));
    int *pipes = calloc(1, sizeof(int));
    *arg_count = 0;
    *pipes = 0;
    printf("line: ");
    
    if (read_cmd_line(cmd_line) != 0) {
        return 1;
    }

    /* Strcpy for the possibility of the second strtok */
    strcpy(cmd_line_cpy, cmd_line);
    if (token_args(arg_token, cmd_line, &arg_count, &pipes) != 0) {
        return 1;
    }
    
    /* Error check to make sure that the input is proper */
    if (err_check_input(arg_token, *arg_count, *pipes) != 0) {
        return 1;
    }

    /* If there are pipes strtok using pipes as delimeters */
    if (*pipes != 0) {
        token_pipe(pipe_token, cmd_line_cpy);
        if (print_stages(pipe_token, *pipes) != 0) {
            return 1; 
        }
    }
    else {
        if (no_pipe_print(arg_token, *arg_count, *pipes, cmd_line_cpy) != 0) {
            return 1;
        }
    }
    
    free(pipes);
    free(arg_count);
    return 0;
    
}
