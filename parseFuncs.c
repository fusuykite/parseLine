//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"

/* Read in the command line argument and error check 
 * return 0 on success and 1 on error */
int read_cmd_line(char *cmd_line) {
    int i = 0;
    int empty_ct = 0;
    if (fgets(cmd_line, MAX_CMD_LEN, stdin) == NULL) {
        fprintf(stderr, "%s", "invalid null command\n");
        return 1;
    }
    /* Check the length of the command line */
    if (cmd_line[MAX_CMD_LEN - 1] != '\0') {
        fprintf(stderr, "%s", "Command Line too Long\n");
        return 1;
    }
    for (i = 0; i < MAX_CMD_LEN; i++) {
        if (cmd_line[i] == '\n') {
            cmd_line[i] = '\0';
        }
        /* Empty line check */
        if (cmd_line[i] != ' ' && cmd_line[i] != '\0') {
           empty_ct++; 
        }
    }
    if (empty_ct == 0) {
        fprintf(stderr, "%s", "invalid null command\n");
        err_usage_cmd();
        return 1;
    }
    return 0;
}

/* Takes the command line and tokenizes the arguments 
 * stores the arguments within the token_list array */
int token_args(char **token_list, char *cmd_line, int **arg_num, int **pipes) {
    char *token = strtok(cmd_line, " ");
    
    /* Error Out: If the first Arg is a special character */
    if (*token == ASCII_PIPE || *token == ASCII_INDIRECT || 
    *token == ASCII_OUTDIRECT) {
        fprintf(stderr, "%s", "invalid null command\n");
        fprintf(stderr, "%s", "failed to parse pipeline\n");
        return 1;
    }

    while (token != NULL) {
        token_list[**arg_num] = token;
        if (*token_list[**arg_num] == ASCII_PIPE) {
            if (*token_list[**arg_num - 1] == ASCII_PIPE) {
                fprintf(stderr, "%s", "invalid null command\n");
                fprintf(stderr, "%s", "failed to parse pipeline\n");
                return 1;
            }
        }
        
        if (*token == ASCII_PIPE) {
            **pipes = **pipes + 1;
            /* Err check for stages many stages */
            if (**pipes >= MAX_CMD_PIPE) {
                fprintf(stderr, "%s", "pipeline too deep\n");
                err_usage_cmd();
                return 1;
            }
        }
        /* Increment argument counter*/
        **arg_num = **arg_num + 1;
        token = strtok(NULL, " ");
    }
    return 0;
}

/* Function to tokenize the command line based on pipes */
int token_pipe(char **token_list, char *cmd_line) {
    char *token = strtok(cmd_line, "|");
    int i = 0;

    while (token != NULL) {
        token_list[i] = token;
        i++;
        token = strtok(NULL, "|");
    }
    return 0;
}

/* Print stages for multiple pipes */
int print_stages(char **token_list, int num_pipe) {
    int i = 0;
    for (i = 0; i <= num_pipe; i++) {
        printf("\n--------\n");
        printf("Stage %d: \"%s\"", i, token_list[i]);
        printf("\n--------\n");
        print_individual(token_list[i], i, num_pipe);
    }
    return 0;
}

int print_individual(char *token_arg, int idx, int num_pipe) {
    int flag = 0; /* Flag 0 means first argument, flag 1 means last argument*/
    /* Starting stage */
    if (idx == 0) {
        print_first_last(token_arg, num_pipe, flag);
    }
    /* Ending stage */
    else if (idx == num_pipe) {
        flag++;
        print_first_last(token_arg, num_pipe, flag);
    }
    /* Middle stages */
    else {
        print_mid(token_arg, idx);
    }
    return 0;   
}

void print_mid(char *cmd_arg, int idx) {
    char * arg_list[MAX_CMD_LEN];
    char * token;
    int length = 0;
    int i = 0;

    /* Delimit the char array based on spaces */
    token = strtok(cmd_arg, " ");
    while (token != NULL) {
        arg_list[length] = token;
        length++;
        token = strtok(NULL, " ");
    }

    printf("     input: pipe from stage %d\n", idx - 1);
    printf("     output: pipe to stage %d\n", idx + 1);
    printf("     argc: %d\n", length);
    printf("     argv: ");

    /* Print based on the formatting */
    for (i = 0; i < length; i++) {
        if (i != length - 1) {
            printf("\"%s\",", arg_list[i]);
        }
        else {
            printf("\"%s\"\n", arg_list[i]);
        }
    }
}

void print_first_last(char *cmd_arg, int num_pipe, int flag) {
    int i = 0;
    char *token;
    char *pipe_args[MAX_CMD_PIPE];
    char *argv_print_list[MAX_CMD_PIPE];
    int pipe_len = 0; /* Keep track of arguments in pipe */
    int cmd_pipe = 0; /* Number of commands in pipe */
    int argc = 0; /* For the argc output */
    int argv_num = 0; /* Count of non special characters */
    int argv_idx = 0;
    char *input = NULL;
    char *output = NULL;

    token = strtok(cmd_arg, " ");
    while (token != NULL) {
        pipe_args[pipe_len] = token;
        pipe_len++;
        token = strtok(NULL, " ");
    }
    argc = pipe_len;
    argv_num = pipe_len;

    for (i = 0; i < pipe_len; i++) {
        if (*pipe_args[i] == ASCII_INDIRECT) {
            input = pipe_args[i + 1];
            argv_num--;
            argc = argc - 2;
        }
        else if (*pipe_args[i] == ASCII_OUTDIRECT) {
            output = pipe_args[i + 1];
            argv_num--;
            argc = argc - 2;
        }
    }
    
    /* Print input output information */
    if (input == NULL) {
        if (flag == 0) {
            printf("     input: original stdin\n");
        }
        else {
            printf("     input: pipe from stage %d\n", num_pipe - 1); 
        }
    }
    else { 
        printf("     input: %s\n", input);
    }
    if (output == NULL) {
        if (flag == 0) {
            printf("     output: pipe to stage 1\n"); 
        }
        else {
            printf("     output: original stdout\n");
        }
    }
    else { 
        printf("     output: %s\n", output);
    }
    printf("     argc: %d\n", argc);
    printf("     argv: ");

    /* Print argv */
    /* Case: Last */
    if (input == NULL && output != NULL) {
        for (i = 0; i < pipe_len; i++) {
            if (strcmp(pipe_args[i], output) != 0 &&
            *pipe_args[i] != ASCII_INDIRECT && 
            *pipe_args[i] != ASCII_OUTDIRECT) {
                argv_print_list[argv_idx] = pipe_args[i];
                argv_idx++;
            }
        }
    }
    
    /* Case: First */
    else if (input != NULL && output == NULL) {
        for (i = 0; i < pipe_len; i++) {
            if (strcmp(pipe_args[i], input) != 0 &&
            *pipe_args[i] != ASCII_INDIRECT && 
            *pipe_args[i] != ASCII_OUTDIRECT) {
                argv_print_list[argv_idx] = pipe_args[i];
                argv_idx++;
            }
        }
        
    }
    
    /* Case: Either */
    else if (input == NULL && output == NULL) {
        for (i = 0; i < pipe_len; i++) {
            argv_print_list[i] = pipe_args[i];
        }
    }
    
    for (i = 0; i < argc; i++) {
        if (i != argc - 1) {
            printf("\"%s\",", argv_print_list[i]);
        }
        else {
            printf("\"%s\"\n", argv_print_list[i]);
        }
    }

}

int no_pipe_print(char **token_list, int num_arg, 
int num_pipe, char *cmd_line) {
    int i = 0;
    int argc = num_arg;
    int arg_list_num = num_arg; /* count of non special char in list */
    int argv_idx = 0;
    char *argv_list[11];
    char *input = NULL;
    char *output = NULL;

    /* Print stage heading */
    printf("\n--------\n");
    printf("Stage %d: \"%s\"", num_pipe, cmd_line);
    printf("\n--------\n");
    
    for (i = 0; i < num_arg; i++) {
        if (*token_list[i] == ASCII_INDIRECT) {
            input = token_list[i + 1];
            arg_list_num--;
            argc = argc - 2;
        }
        else if (*token_list[i] == ASCII_OUTDIRECT) {
            output = token_list[i + 1];
            arg_list_num--;
            argc = argc - 2;
        }
    }

    /* Print input output information */    
    if (input == NULL) {
        printf("     input: %s\n", "original stdin");
    }
    else {
        printf("     input: %s\n", input);
    }

    if (output == NULL) {
        printf("     output: %s\n", "original stdout");
    }
    else {
        printf("     output: %s\n", output);
    }
    printf("     argc: %d\n", argc);
    printf("     argv: ");
    
    /* For printing the argv */
    if (input != NULL && output != NULL) {
        for (i = 0; i < num_arg; i++) {
            if (strcmp(token_list[i], input) != 0 &&
                strcmp(token_list[i], output) != 0 &&
                *token_list[i] != ASCII_INDIRECT &&
                *token_list[i] != ASCII_OUTDIRECT) {
                argv_list[argv_idx] = token_list[i];
                argv_idx++;
            }
        }
    }
    
    else if (input != NULL && output == NULL) {
        for (i = 0; i < num_arg; i++) {
            if (strcmp(token_list[i], input) != 0 &&
                *token_list[i] != ASCII_INDIRECT &&
                *token_list[i] != ASCII_OUTDIRECT) {
                argv_list[argv_idx] = token_list[i];
                argv_idx++;
            }
        }
    }
    
    else if (input == NULL && output != NULL) {
        for (i = 0; i < num_arg; i++) {
            if (strcmp(token_list[i], output) != 0 &&
                *token_list[i] != ASCII_INDIRECT &&
                *token_list[i] != ASCII_OUTDIRECT) {
                argv_list[argv_idx] = token_list[i];
                argv_idx++;
            }
        }
    }

    else if (input == NULL && output == NULL) {
        for (i = 0; i < arg_list_num; i++) {
            argv_list[i] = token_list[i];
        }
    }

    for (i = 0; i < argc; i++) {
        if (i != argc - 1) {
            printf("\"%s\",", argv_list[i]);
        }
        else {
            printf("\"%s\"\n", argv_list[i]);
        }
    }
    return 0;
}

int err_check_input(char **token_list, int num_arg, int num_pipe) {
    int i = 0;
    int j = 0;
    int max_index = num_arg - 1;
    int cmd_arg = 0;
    int pipe_count = 0;
    char * cmd_str = token_list[0];
    for (i = 0; i < num_arg; i++) {
        if (*token_list[i] == ASCII_PIPE) {
    
            /* Check if next token is special character */
            /* Boundary check to make sure i is not the last index */
            if (i + 1 < max_index) {
                if (*token_list[i + 1] == ASCII_INDIRECT) {
                    fprintf(stderr, "%s: ", token_list[i + 1]);
                    err_in_redirect();
                    return 1;
                }
                else if (*token_list[i + 1] == ASCII_OUTDIRECT) {
                    fprintf(stderr, "%s: ", token_list[i + 1]);
                    err_out_redirect();
                    return 1;
                } 
            }
                
            /* If last token is a Pipe command */
            if (i == max_index) {
                err_null_cmd();
                return 1;
            } 

            /* Reset cmd args num */
            cmd_arg = 0;
            cmd_str = token_list[i + 1];
            
            /* Modify counters for ambiguous IO */
            pipe_count++;
            
        }
        else if (*token_list[i] == ASCII_INDIRECT) {

            /* Middle stages if there is a redirect raise err */
            if (pipe_count != 0 && pipe_count != num_pipe) {
                fprintf(stderr, "%s: ", cmd_str);
                err_in_ambig();
                return 1;
            }

            /* Last stage */
            if (pipe_count == num_pipe && num_pipe != 0) {
                fprintf(stderr, "%s: ", cmd_str);
                err_in_ambig();
                return 1;
            }

            /* If last token is in redirect */
            if (i == max_index) {
                for (j = max_index; j > 0; j--) {
                    if (*token_list[j] == ASCII_PIPE) {
                        fprintf(stderr, "%s: ", token_list[j + 1]);
                        err_in_redirect();
                        return 1;
                    }
                }
                fprintf(stderr, "%s: ", token_list[j]);
                err_in_redirect();
                return 1;
            }
            
            /* Decrement the arg counter */
            cmd_arg--;
        }
        else if (*token_list[i] == ASCII_OUTDIRECT) {
            
            /* Middle stages if there is a redirect raise err */
            if (pipe_count != 0 && pipe_count != num_pipe) {
                fprintf(stderr, "%s: ", cmd_str);
                err_out_ambig();
                return 1;
            }

            /* First stage if there is redirect raise err */
            if (pipe_count == 0 && num_pipe != 0) {
                fprintf(stderr, "%s: ", cmd_str);
                err_out_ambig();
                return 1;
            }

            /* If last token is out redirect */
            if (i == max_index) {
                for (j = max_index; j > 0; j--) {
                    if (*token_list[j] == ASCII_PIPE) {
                        fprintf(stderr, "%s: ", token_list[j + 1]);
                        err_out_redirect();
                        return 1;
                    }
                }
                fprintf(stderr, "%s: ", token_list[j]);
                err_out_redirect();
                return 1;
            }
            
            /* Decrement the arg counter */
            cmd_arg--;
        }
        /* Not a special character */
        else {
            /* Err check for too many arguments */
            cmd_arg++;
            if (cmd_arg > MAX_CMD_ARG) {
                fprintf(stderr, "%s: ", cmd_str);
                too_many_args();
                return 1;
            }
        }
    }
    return 0;
}

void err_in_redirect() {
    fprintf(stderr, "%s", "bad input redirection\n");
    fprintf(stderr, "%s", "failed to parse pipeline\n");
}

void err_out_redirect() {
    fprintf(stderr, "%s", "bad output redirection\n");
    fprintf(stderr, "%s", "failed to parse pipeline\n");
}

void err_in_ambig() {
    fprintf(stderr, "%s", "ambiguous input\n");
    fprintf(stderr, "%s", "failed to parse pipeline\n");
}

void err_out_ambig() {
    fprintf(stderr, "%s", "ambiguous output\n");
    fprintf(stderr, "%s", "failed to parse pipeline\n");
}

void err_null_cmd() {
    fprintf(stderr, "%s", "invalid null command\n");
    fprintf(stderr, "%s", "failed to parse pipeline\n");
}

void err_usage_cmd() {
    fprintf(stderr, "%s", "usage: parseline [ -c ] [ -d ] set1 [ set2 ]\n");
    fprintf(stderr, "%s", "failed to parse pipeline\n");
}

void too_many_args() {
    fprintf(stderr, "%s", "too many arguments\n");
    fprintf(stderr, "%s", "failed to parse pipeline\n");
}


