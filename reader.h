#define _GNU_SOURCE
#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct county{
    char name[100];
    char state[3];
    float edu[2];
    float ethn[8];
    int inc[2];
    float pov;
    int pop;
}county;

typedef struct storage{
    int size;
    int cap;
    county *ptr;
}storage;

FILE *openingfile(int argc, char *argv[] , int i);
storage *makeheap();
county parseline(char *line);
void fillheap(storage *info , FILE *file);
void readcontents(FILE *file);
void realloccheck(storage *info);
storage *parsecsv(int argc , char *argv[]);

void readops(FILE *file ,storage *data);
void displayme(storage *data);
void filterme(storage *data , char *line);
void findstates(storage *data , char *statename);
void findgele(storage *data , char *field , int sign , float value);
void findgele_edu(storage *data , char *field , int sign , float value);
void findgele_ethn(storage *data , char *field , int sign , float value);
void findgele_inc(storage *data , char *field , int sign , float value);
void addentry(storage *newheap , storage *data , int i );
void populationme(storage *data , char *line);
int poptotal(storage *data);
float popfield(storage *data , char *line);
float findpop_edu(storage *data , char *field);
float findpop_eth(storage *data , char *field);
float findpop_pov(storage *data , char *field);
char *findfield(char *line);
void percentme(storage *data , char *line);


#endif