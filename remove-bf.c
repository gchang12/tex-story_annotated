// https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/

#include <stdio.h>
#include <string.h> // To invoke "strlen" function on "fname" buffer
#include <dirent.h> // Essential bit for this function
#include <stdlib.h>

#define SRCDIR "./annotations/"
#define LINESIZE 1024

int unboldifyFile(char *filename){
    FILE *fp;
    char line[LINESIZE];
    char unbfline[LINESIZE];
    char *comment;
    int header_len;
    int lineno=0;
    char my_lines[256][LINESIZE]={0};

    fp=fopen(filename,"r");
    while ( (fgets(line,sizeof(line),fp)) != NULL ){
        if (line[0] != '{')
            strcpy(unbfline,line);
        else{
// "} (comment)"
            comment=strchr(line,'}');
// The length of "{\bf " is five, and "}" is being skipped ("comment+1")
// Length of header=(length of line) - (length of comment, which includes the extra "}") - (length of string that prefixes the bold-face command)
            header_len=comment-line-5;
// "{\bf " is five characters, so the start of the header would be on index five
            line[5+header_len]=0;
            strncpy(unbfline,line+5,header_len+1);
// (header):
            strcat(unbfline,comment+1);
// (header): (comment)
        }
        strcpy(my_lines[lineno++],unbfline);
    }
    fclose(fp);

    fp=fopen(filename,"w");
    for (int i=0; i<lineno; i++){
        fputs(my_lines[i],fp);
    }
    fclose(fp);
}

int main(int argc, char **argv){
    DIR *src_dir=opendir(SRCDIR);
    struct dirent *de;
    char *fname;
    char fullpath[64];
    while ((de=readdir(src_dir)) != NULL) {
        fname=de->d_name;
        if ((strlen(fname) == 1 && fname[0] == '.') || (strlen(fname) == 2 && fname[0] == '.' && fname[1] == '.'))
            continue;
        strcpy(fullpath,SRCDIR);
        strcat(fullpath,fname);
        unboldifyFile(fullpath);
    }
    closedir(src_dir);
    return 0;
}
