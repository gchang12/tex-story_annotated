#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For memory-allocation function calls

// The files in this directory will be essential to this program
#define format_dir "./format/"
#define scene_dir "./scenes/"

int countInLine(char *sentence){
    char *pattern="markAnnot";
    int pattern_len=strlen(pattern);
    int spam_count=0;
    while ((sentence=strstr(sentence,pattern)) != NULL){
        sentence+=pattern_len;
        spam_count++;
    }
    return spam_count;
}

int countInFile(char *filename){
// Invokes the function that counts the number of words in a given line
    FILE *fp;
    int num_annot=0;
    char line[1024];
    fp=fopen(filename,"r");
    while ((fgets(line,sizeof(line),fp)) != NULL){
        num_annot+=countInLine(line);
    }
    fclose(fp);
    return num_annot;
}

void getEpilogue(char *epbuf){
// Stores the name of the epilogue in the "char"-pointer passed as the argument; note that the epilogue lacks an extension
    FILE *fp;
    char epname[64];
    char post_underscore[64];
// The buffer is equal to ./scenes/
    strcpy(epbuf,scene_dir);
    char other_file[64];
    strcpy(other_file,format_dir);
    strcat(other_file,"other.txt");
    fp=fopen(other_file,"r");
    fgets(epname,sizeof(epname),fp);
// In case the file contains an underscore
    while (epname[strlen(epname)-1] == '_'){
        fgets(post_underscore,sizeof(post_underscore),fp);
        strcat(epname,post_underscore);
    }
    fclose(fp);
// Stripping the newline character from the string
    epname[strlen(epname)-1]=0;
// Append the filename here
    strcat(epbuf,epname);
// The epilogue lacks an extension
    strcat(epbuf,".tex");
}

int countChapters(void){
// Loop over ordering file in order to determine length
// - Number of blank lines OR number of "groups"
    FILE *fp;
    char line[64];
    int num_blanks=0;
// Just economizing on global variable declaration
    char ordering[64];
    strcpy(ordering,format_dir);
    strcat(ordering,"ordering.txt");
    fp=fopen(ordering,"r");
    while ((fgets(line,sizeof(line),fp)) != NULL){
        if (strlen(line) == 1)
            num_blanks++;
    }
    fclose(fp);
// Must include the epilogue
    num_blanks++;
    return num_blanks;
}

int countLines(char *filename){
    FILE *fp;
    char s[1024];
    int linecount=0;
    fp=fopen(filename,"r");
// Keep reading lines until the EOF is reached; increment every time
    while (fgets(s,sizeof(s),fp) != NULL)
        linecount++;
    fclose(fp);
    return linecount;
}

// I don't know how to get the length of the "linecounts" array
int countNotes(int *linecounts,int num_chapters){
    char chapter[64];
// Must hold results of "itoa" call
    char my_str[4];
// ./annotations/chapter%d.txt
    char path[3][32]={"chapter",[2]=".txt"};
    for (int i=0; i<num_chapters; i++){
        sprintf(my_str,"%d",i+1);
        strcpy(path[1],my_str);
// Form the path here and store into chapter
        strcpy(chapter,"./annotations/");
        for (int j=0; j<3; j++){
            strcat(chapter,path[j]);
        }
        linecounts[i]=countLines(chapter);
    }
}

int main(void){
// To determine the dimensions of the integer arrays
    int num_chapters;
    num_chapters=countChapters();
// To be used to store lines, parts of underscored filenames, and epilogue (no extension)
    char line[1024];
// To be used to store filenames
    char filename[64];

// FREE ALL ALLOCATED VARIABLES AFTERWARDS WITH free CALL
// Store line-counts of note-files (already written)
    int *line_counts=calloc(sizeof(int),num_chapters);
// Store annotation-counts of prose-files
    int *note_counts=calloc(sizeof(int),num_chapters);

// Declare filename to be read from here
    char ordering[32];
    strcpy(ordering,format_dir);
    strcat(ordering,"ordering.txt");
    FILE  *fp;
// Essential for looping construct
    int chapter_no=0;

// Begin IO operation looping here
    fp=fopen(ordering,"r");

    while ( (fgets(line,sizeof(line),fp)) != NULL){
// The line is blank (i.e. only has the newline character)
        if (strlen(line) == 1){
            chapter_no++;
            *(note_counts+chapter_no-1)=0;
            continue;
        }
        strcpy(filename,line);
        while (filename[strlen(filename)-1] == '_'){
            fgets(line,sizeof(line),fp);
            strcat(filename,line);
        }
        // Strip newline here
        filename[strlen(filename)-1]=0;
// We're operating on files that have already been processed
        strcpy(line,scene_dir);
        strcat(line,filename);
        *(note_counts+chapter_no-1)+=countInFile(line);
    }

    fclose(fp);

    getEpilogue(line);
    *(note_counts+num_chapters-1)=countInFile(line);
    countNotes(line_counts,num_chapters);

    for (int i=0; i<num_chapters; i++){
        if (*(note_counts+i) != *(line_counts+i) )
            printf("%d: Annotations = %d; Marks = %d\n",i+1,*(note_counts+i),*(line_counts+i));
    }

    free(line_counts);
    free(note_counts);
    return 0;
}
