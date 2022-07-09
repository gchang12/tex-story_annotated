// https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/

#include <stdio.h>
#include <string.h> // To invoke "strlen" function on "fname" buffer
#include <dirent.h> // Essential bit for this function
#include <stdlib.h>

#define SRCDIR "./annotations-tex/"

void unboldifyFile(char *filename){
    FILE *fp;
    char line[1024];
    char unbfline[1024];
    char *header;
    char *comment;
    int header_len;
    int lineno=0;
    char my_lines[256][1024]={0};
    
    fp=fopen(filename,"r");
    while ( (fgets(line,sizeof(line),fp)) != NULL ){
// "{\bf ...:} rest\n
        if (line[0] != '{')
            continue;
        comment=strchr(line,'}');
        header_len=comment-line-5;
        header=malloc(header_len);
        strncpy(header,line+5,header_len);
        strcpy(unbfline,header);
        free(header);
        strcat(unbfline,comment+1);
        strcpy(my_lines[lineno++],unbfline);
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
        unboldifyFile(fullpath);
    }
    closedir(src_dir);
    return 0;
}
