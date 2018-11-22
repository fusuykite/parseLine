//CREATED BY YUSUF BAHADUR AND ANDREW YAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

   char cmnd_line[515];
   int i;
   int arg_cnt = 0;
   char * tokens[11];
   int pipes = 0;

   /*read in command line*/
   for(i = 0; i < 514; i++){

      cmnd_line[i] = '\0';
   }
   printf("\nline: ");

   fgets(cmnd_line, 515, stdin);

   /*check if command line too long */
   if (cmnd_line[511] != '\0'){

      printf("Command Line too Long");

      exit(1);
   }

   for (i = 0; i < 514; i++)
   {
      if (cmnd_line[i] == '\n')

         cmnd_line[i] = '\0';
   }

   /* arg_cnt == number of arguments */
   /* SET UP *TOKENS[] ARRAY TO HOLD EACH ARGUMENTS */
   char * token = strtok(cmnd_line, " ");
   i = 0;
   while (token != NULL) {

      arg_cnt += 1;

      tokens[i] = token;

      i++;
      
      if(i == 11){

         printf("Arguments Greater than 10: Error");

         exit(1);
      }
      /* Counter for # of Pipes */
      if(*token == '|'){

         pipes += 1;
      }
      token = strtok(NULL, " ");
   }

   /* Error Out: If first Argument is Special Character*/
   if (*tokens[0] == '|' || *tokens[0] == '<' || *tokens[0] == '>'){
      printf("invalid null command\nfailed to parse pipeline\n");
      exit(1);
   }

   /*Code for Stages*/
   int pipe = 0;
   int pipe_input_flag = 0;
   int less = 0;
   int greater = 0;
   int beg = 0;
   int stage = 0;
   int x = 0;
   int d = 0;

   /*pointers to hold input/output message for each stage*/
   char * input = (char*)calloc(50, sizeof(char));
   input = " original stdin";
   char * output = (char*)calloc(50, sizeof(char));
   output = " original stdout";

   /*cycle thru each argument in tokens using total arg_cnt */
   for(i = 0; i < arg_cnt; i++){

      /*if arg = '|' print out current stage & refresh variables for new stage*/
      if (*tokens[i] == '|'){
      
         pipe += 1;
         stage += 1;

         /*print out Stage Heading */
         printf("\n--------\n");

         printf("Stage %d: \"", stage-1);

         x = i;
         --x;

         while((*tokens[x] != '|') && (x > 0)){

            --x;
         }
         for(d = x; d < i; ++d){

            if (*tokens[d] != '|')

               printf(" %s", tokens[d]);
         }
         printf(" \"\n--------\n");

         /*print input/output for stage.*/
         if (pipe_input_flag == 1){
            printf("     input: pipe from stage %d\n    output: pipe to stage %d\n\n", (pipe-2), pipe);
         }
         else{
            printf("     input: %s\n    output: pipe to stage %d\n\n", input, pipe);
         }

         /* reset variables for next stage */
         pipe_input_flag = 1;
         output = " original stdout";
      }

      /*if arg = '<', set input equal to the next argument*/
      if (*tokens[i] == '<'){
         less += 1;
         input = tokens[i+1];

         /*beg tracks first argument before </> for redirrection errors"*/
         if (beg == 0 || (i-1) < beg){
             beg = i-1;
         }
      }

      /*if arg = '>', set input equal to the next argument*/
      if (*tokens[i] == '>'){
         greater += 1;
         output = tokens[i+1];

         if (beg == 0 || (i-1) < beg){
             beg = i -1 ;
         }
      }

      /* error out: duplicate '>' or '<' in command line */
      if(less > 1 || greater > 1){
         if (less > 1){
            printf("%s: bad input redirection\nfailed to parse pipeline", tokens[beg]);
            exit(1);
         }
         else{
            printf("%s: bad output redirection\nfailed to parse pipeline", tokens[beg]);
            exit(1);
         }
      }
   }

   /*Out of for Loop - last stage printout*/
   stage++;
   printf("\n--------\n");

   printf("Stage %d: \"", stage-1);

   x = i;
   --x;
   
   while((*tokens[x] != '|') && (x > 0)){
      --x;
   }
   for(d = x; d < i; ++d){

      if (*tokens[d] != '|')

         printf(" %s", tokens[d]);
   }
   printf(" \"\n--------\n");

   if (pipe_input_flag == 1){

      printf("     input: pipe from stage %d\n    output: %s\n\n", (pipe-1), output);
   }
   else{

      printf("     input: %s\n    output: %s\n\n", input, output);
   }

   exit(1);
}


