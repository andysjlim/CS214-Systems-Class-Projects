#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

//Turns all nonalphabets into a space. Then, checks for any extra spaces.
//The counter is checked with how many occurences of consecutive spaces there are. 
//Ex: "             " will count for one, 
//but as mentioned before, any extra spaces are taken care of after making all sequences of spaces into one space.
int StringWithSpace(char* string){
    int i,j,counter = 1;
    for(i = 0; i < strlen(string); i++){
        if(!isalpha(string[i])){
            string[i] = ' ';
            while(!isalpha(string[i+1]) && string[i+1] != '\0'){
                for(j = i+1; j < strlen(string); j++){
                    string[j] = string[j+1];
                }
            }
            if(!(string[i+1] == '\0'))
                counter++;
        }
    }
    /*
     Test!
    printf("Now printing the new string\n");
    for(i = 0; i < strlen(string); i++){
        printf("%c",string[i]);
    }
    */
    if(string[0] == ' ' || string[0] == '\0') counter--;
    //printf("\nCounter is: %i\n", counter);
    return counter;
}

//Tokenizes with respect space.
char** TokenizeString(char* spaceystring, int count){
    char *token;
    char **string;
    int i = 1;

    string = malloc(count * sizeof(char *));

    token = strtok(spaceystring, " ");
    string[0] = token;
    while(token != NULL){
        token = strtok(NULL, " \0");
        if(token != '\0'){
            string[i] = (char *)malloc(sizeof(token) + 1);
            strcpy(string[i],token);
            i++;
        }
    }
    return string;
}

//Prints each "word/token"
void PrintString(char** strings, int wordcount){
    //printf("\nNow printing\n");
    int i;
    for(i = 0; i < wordcount; i++){
        printf("%s\n", strings[i]);
    }
}

/* Things to watch for:
 * Capital letters > lowercase
 * Length of string. If one is a concatenation of another aka longer string then, it shoudl appear after.
 */
int checkChars(char* one, char* two){
    int k = 0;
    while(one[k] != '\0'){
        if(two[k] == '\0'){
            return 1;
        }
        //printf("Comparing these two letters:%c and %c\n", one[k], two[k]);
        if(one[k] > two[k]){
            return 1;
        } 
        if(one[k] < two[k]){
            return 0;
        }
        k++;
    }
    return 0;
}

//Sorts
void SortString(char** string, int count){
    int i,j;
    char *temp;
    for(i=0; i < count; i++){
        for(j = i+1; j < count; j++){
            //printf("Now working on position %i and %i\n", i,j);
            if(checkChars(string[i], string[j]) == 1){
                //printf("%s\t%s\n", string[i], string[j]);
                temp = (char *)malloc(sizeof(string[i]));
                strcpy(temp, string[i]);
                string[i] = string[j];
                string[j] = temp;
                //printf("Position %i and %i has been changed\n", i,j);
            }
        }
    }
    return;
}

//Free stuff
void FreeString(char** string, int count){
    int i;
    for(i = 0; i < count; i++){
        free(string[i]);
    }
    free(string);
    return;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("WHAT ARE YOU TRYING TO DO");
        return 0;
    }
    int wordcount = StringWithSpace(argv[1]);
    //If there are actually words in the string then yeah it should run everything.
    if(wordcount>0){
        char **argvtokens = TokenizeString(argv[1], wordcount);
        SortString(argvtokens, wordcount);
        PrintString(argvtokens, wordcount);
        FreeString(argvtokens,wordcount);
    }
    return 0;
}
