// https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/

#include <stdio.h>
#include <string.h> // To invoke "strlen" function on "fname" buffer
#include <dirent.h> // Essential bit for this function
#include <stdlib.h> // For allocation functions

#define SRCDIR "./annotations/"

int boldifyFile(char *filename){
    FILE *fp;
    char line[1024];
    char bfline[1024];
    char *comment;
    char my_lines[256][1024]={0};
    int header_len;
    fp=fopen(filename,"r");
    int lineno=0;
    while ( (fgets(line,sizeof(line),fp)) != NULL ){
        if (line[0] == '{')
            strcpy(bfline,line);
        else {
            strcpy(bfline,line);
            strcpy(bfline,"{\\bf ");
// bfline: {\bf 
            comment=strchr(line,':');
            header_len=comment-line;
            strncat(bfline,line,header_len);
// bfline: {\bf (header)
            strcat(bfline,":}");
// bfline: {\bf (header):
            strcat(bfline,comment+1);
// bfline: {\bf (header):} (comment)
        }
        strcpy(my_lines[lineno++],bfline);
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
        boldifyFile(fullpath);
    }
    closedir(src_dir);
    return 0;
}
