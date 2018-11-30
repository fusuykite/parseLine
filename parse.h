//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Size defines */
#define MAX_CMD_LEN 512
#define MAX_CMD_PIPE 10
#define MAX_CMD_ARG 10

/* ASCII defines */
#define ASCII_PIPE 124
#define ASCII_INDIRECT 60
#define ASCII_OUTDIRECT 62

/* Read in the user input into a character array */
int read_cmd_line(char *cmd_line);

/* Tokenize the arguments to parse the lines */
int token_args(char **token_list, char *cmd_line, int **arg_num, int **pipes);

/* Tokenize the string and delimit it based on pipes */
int token_pipe(char **token_list, char *cmd_line);

/* Error checking the user input */
int err_check_input(char **token_list, int num_arg, int num_pipe);

/* Set up to print the stages of the parsed user input */
int print_stages(char **token_list, int num_pipe);

/* Another set up in order to print the stages probably could have
 * just combined thsi one and print stages */
int print_individual(char *token_arg, int idx, int num_pipe);

/* Function to print the information for the
 * first and last stage for the piping */
void print_first_last(char *token_arg, int num_pipe, int flFlag);

/* Function to print the information for the middle stages for piping */
void print_mid(char *token_arg, int idx);

/* Function to print the information when no pipes */
int no_pipe_print(char **token_list, int arg_num,
 int num_pipe, char *cmd_line);

/* Error print functions */
void err_in_redirect();

void err_out_redirect();

void err_in_ambig();

void err_out_ambig();

void err_null_cmd();

void err_usage_cmd();

void too_many_args();




