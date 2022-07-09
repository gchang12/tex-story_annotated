// https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/

#include <stdio.h>
#include <stdlib.h> // To invoke "system"
#include <dirent.h> // To walk through directories
#include <sys/stat.h> // To make directories 
#include <string.h>

#define ROOT "perl tex-converter.pl "

int main(void){
    char argv[3][8]={"","annotations","annotations"};
    // Declare the source folder.
    DIR *src_dir=opendir(argv[1]);
    struct dirent *de;
    mkdir(argv[2],S_IRWXU);
    // It's probably better that the program raises an error if the target directory already exists. As it so happens, the system goes on uninterrupted even if the directory exists.
    char *s;
    char cmd[256];
    char perl_args[7][64] = {[1]="/",""," ",[5]="/",""};
    int file_indices[4]={2,6,0,4};
// Copying argv[1] and argv[2] to their places as indicated
    for (int i=2; i<4; i++)
        strcat(perl_args[file_indices[i]],argv[i-1]);
    while ((de=readdir(src_dir)) != NULL) {
        s=de->d_name;
// Matching '.' and '..' in the "walk" command
        if ((strlen(s) == 1 && s[0] == '.') || (strlen(s) == 2 && s[0] == '.' && s[1] == '.'))
            continue;
        strcpy(cmd,ROOT);
        for (int j=0; j<2; j++)
            strcpy(perl_args[file_indices[j]],s);
        for (int j=0; j<7; j++)
            strcat(cmd,perl_args[j]);

// Converting the extensions of the output files to "tex", assuming that the source files have "txt" extensions
        cmd[strlen(cmd)-2]='e';
        cmd[strlen(cmd)-1]='x';
        //printf("%s\n",cmd);
        system(cmd);
    }
    closedir(src_dir);
    char *tcmd="pdflatex main";
    char typeset[64];
    strcpy(typeset,tcmd);
    strcat(typeset," && ");
    strcat(typeset,tcmd);
    system(typeset);
    return 0;
}
