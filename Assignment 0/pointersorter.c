#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

int StringWithSpace(char* string){
    int i,counter = 1;
    for(i = 0; i < strlen(string); i++){
        if(!isalpha(string[i])){
            string[i] = ' ';
            counter++;
        }
    }
    return counter;
}

char** TokenizeString(char* spaceystring, int count){
    char *token;
    char **string;
    int i = 1;

    string = malloc(count * sizeof(char *));

    token = strtok(spaceystring, " ");
    string[0] = token;
    while(token != NULL){
        token = strtok(NULL, " ");
        string[i] = (char *)malloc(sizeof(token) + 1);
        string[i] = token;
        i++;
    }
    return string;
}

void PrintString(char** strings, int wordcount){
    int i;
    for(i = 0; i < wordcount; i++){
        printf("%s\n", strings[i]);
    }
}

void Merge(){

}

void MergeSort(char** string, int wordcount){
    int i,j,k;
    
}

int main(int argc, char* argv[]){
    if(argc > 2){
        perror("WHAT ARE YOU TRYING TO DO");
        return 0;
    }
    int wordcount = StringWithSpace(argv[1]);
    char** argvtokens = TokenizeString(argv[1], wordcount);
    MergeSort(argvtokens);
    PrintString(argvtokens, wordcount);
    
    return 0;
}
