// https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/

#include <stdio.h>
#include <string.h> // To invoke "strlen" function on "fname" buffer
#include <dirent.h> // Essential bit for this function
#include <stdlib.h> // For allocation functions

#define SRCDIR "./annotations-tex/"

void boldifyFile(char *filename){
    FILE *fp;
    char line[1024];
    char bfline[1024];
    char *header;
    char *comment;
    char my_lines[256][1024]={0};
    int header_len;
    fp=fopen(filename,"r");
    int lineno=0;
    while ( (fgets(line,sizeof(line),fp)) != NULL ){
// {\bf header:} content...\n
        if (line[0] == '{')
            continue;
        strcpy(bfline,"{\\bf ");
        comment=strchr(line,':');
        header_len=comment-line;
        header=malloc(header_len);
        strncpy(header,line,header_len);
        strcat(bfline,header);
        free(header);
        strcat(bfline,":}");
        strcat(bfline,comment+1);
        strcpy(my_lines[lineno++],bfline);
        strcpy(header,"");
    }
    fclose(fp);

    if (lineno>0){
        fp=fopen(filename,"w");
        for (int i=0; i<lineno; i++){
            fputs(my_lines[i],fp);
        }
        fclose(fp);
    }
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