#include "reader.h"

int main(int argc , char *argv[])
{
//first step: parse the given csv
    FILE *csvfile = openingfile(argc , argv , 1);  
    storage *info = makeheap();
    fillheap(info , csvfile);

    //printf("%s", (info -> ptr)[0].name);
//second step: the the operations

    FILE *opfile = openingfile(argc , argv , 2);
    readops(opfile , info);
  
    free(info -> ptr);
    free(info);
    fclose(csvfile);
    fclose(opfile);

    
 


    return 0;
}
/*
storage *parsecsv(int argc , char *argv[]){
    FILE *csvfile = openingfile(argc , argv , 1);  
    storage *info = makeheap();
    fillheap(info , csvfile);
}
*/
FILE *openingfile(int argc, char *argv[] , int i){
    FILE *file;
    if (argc == 3){
        file = fopen(argv[i], "r");
        if (file == NULL) {
            perror("Error opening file");
            exit(0);
        }
    }
    else{
        perror("Incorrect # of files given");
        exit(1);
    }  
    return file;
}

//create a new empty storage container
storage *makeheap(){
    storage *newheap = (storage *)malloc(sizeof(newheap));
    newheap->cap = 4000;
    newheap->size = 0;
    newheap->ptr = (county *)malloc(newheap->cap * sizeof(county));
    return newheap;
}

void fillheap(storage *info , FILE *file){
    char *line = NULL;
    size_t size = 0;
    getline(&line , &size , file);
    free(line);
    while(getline(&line , &size , file) > 0){
        realloccheck(info);
        county newcounty = parseline(line);
        //printf("%s\n" , newcounty.name);

        //if ((newcounty.state)[0] != '0'){
            (info -> ptr)[info -> size] = newcounty;
            (info -> size)++;
        //}

        //free(line);
    }
    printf("%d records loaded\n" , info -> size);
}

void realloccheck(storage *info){
    if ((info -> size) + 5 == info -> cap){
        county *newarr = realloc(info -> ptr , (150 + info -> cap) * sizeof(county));
        if (newarr != NULL){
            info -> ptr = newarr;
            info -> cap += 150;
        }
        else{
            perror("Couldn't realloc");
        }
    }
}

county parseline(char *line){
    county cnt;
    memset(&cnt, 0, sizeof(cnt)); // Initialize the structure

    char *token;
    int tokenIndex = 0; // To keep track of the token being processed

    token = strtok(line, ","); // Get the first token
    while (token != NULL) {
        float value;
        int len;
        switch (tokenIndex) {
            case 0: // County name
                len = strlen(token);
                strncpy(cnt.name, token + 1, len - 2);
                cnt.name[len - 1] = '\0';
                //printf("%s\n", cnt.name);
                break;
            case 1: // State name
                cnt.state[0] = token[1];
                cnt.state[1] = token[2];
                cnt.state[2] = '\0';
/*
                len = strlen(token);
                strncpy(cnt.state, token + 1, len - 2);
                cnt.state[len - 1] = '\0';
                */
                break;
            case 5: // Education.Bachelor's Degree or Higher
                sscanf(token, "\"%f\"", &value);
                cnt.edu[0] = value;
                break;
            case 6: // Education.High School or Higher

                sscanf(token, "\"%f\"", &value);
                cnt.edu[1] = value;
                break;
            case 11: // Start of ethnicities
                sscanf(token, "\"%f\"", &value);
                cnt.ethn[0] = value;
                break;
            case 12:
                sscanf(token, "\"%f\"", &value);
                cnt.ethn[1] = value;
                break;
            case 13:
                sscanf(token, "\"%f\"", &value);
                cnt.ethn[2] = value;
                break;
            case 14:
                sscanf(token, "\"%f\"", &value);
                cnt.ethn[3] = value;
                break;
            case 15:
                sscanf(token, "\"%f\"", &value);
                cnt.ethn[4] = value;
                break;
            case 16:
                sscanf(token, "\"%f\"", &value);
                cnt.ethn[5] = value;
                break;
            case 17:
                sscanf(token, "\"%f\"", &value);
                cnt.ethn[6] = value;
                break;
            case 18:
                sscanf(token, "\"%f\"", &value);
                cnt.ethn[7] = value;
                break;
            case 25: // Income.Median Household Income
                sscanf(token, "\"%d\"", &len);
                cnt.inc[0]= len;
                break;
            case 26: // Income.Per Capita Income
                sscanf(token, "\"%d\"", &len);
                cnt.inc[1]= len;
                break;
            case 27: // Income.Persons Below Poverty Level
                sscanf(token, "\"%f\"", &value);
                cnt.pov = value;
                break;
            case 38:
                sscanf(token, "\"%d\"", &len);
                cnt.pop = len;
                break;
            default:
                // For fields not stored in the structure, do nothing
                break;
        }
        token = strtok(NULL, ","); // Get the next token
        tokenIndex++; // Move to the next field
    }

    return cnt;
}





void readops(FILE *file , storage *data){
    char *line = NULL;
    size_t size = 0;
    char *acceptable[] = {"display" , "filter" , "population" , "percent"};
    while(getline(&line , &size , file) > 0){
        char operation[100];
        int i = 0;

        while(line[i] != ':' && line[i] != '-' && line[i] != '\n' && line[i] != '\0' && i < 99){
            operation[i] = line[i];
            i++;
        }
        operation[i] = '\0';
        char op[i + 1];
        strcpy(op , operation);
        //printf("%s" , op);

        if (strcmp(op , acceptable[0]) == 0){
            //printf("hello");
            displayme(data);
        }
        else if(strcmp(op , acceptable[1]) == 0){
            filterme(data , line);
        }
        else if(strcmp(op , acceptable[2]) == 0){
            break;
        }
        else if(strcmp(op , acceptable[3]) == 0){
            break;
        }
        
    }
    free(line);
    
}


void displayme(storage *data){

    county *allcounties = data -> ptr;
    //printf("%d" , data -> size);
    for(size_t i =0 ; i < data -> size ; i++){
      //  printf("whatsup");
        county cur = allcounties[i];
        printf("%s , %s\n" , cur.name , cur.state);
        printf("    population: %d\n" , cur.pop);
        printf("    education:\n");
        printf("        High School: %f%%\n" , cur.edu[1]);
        printf("        Bachelor's: %f%%\n" , cur.edu[0]);
        printf("    Ethnicity percentages:\n");
        printf("        American Indian or Alaskan Native: %f\n" , cur.ethn[0]);
        printf("        Asian: %f%%\n" , cur.ethn[1]);
        printf("        Black: %f%%\n" , cur.ethn[2]);
        printf("        Hispanic: %f%%\n" , cur.ethn[3]);
        printf("        Pacific Islander: %f%%\n" , cur.ethn[4]);
        printf("        Two or more races: %f%%\n" , cur.ethn[5]);
        printf("        White: %f%%\n" , cur.ethn[6]);
        printf("        White, not hispanic or latino: %f%%\n" , cur.ethn[7]);
        printf("    Income:\n");
        printf("        Median Household: %d\n" , cur.inc[0]);
        printf("        Per Capita: %d\n" , cur.inc[1]);
        printf("        Below Poverty level: %f%%\n\n" , cur.pov);
    }
}

void filterme(storage *data , char *line){
    if (line[6] == '-'){
        char statename[3];
        statename[0] = line[13];
        statename[1] = line[14];
        statename[2] = '\0';
        findstates(data , statename);
    }
    
    if (line[6] == ':'){
        int cnt = 7;
        int i = 0;
        char buffer[100];
        while (line[cnt] != ':' && line[cnt] != '\0' && i < 99){
            buffer[i] = line[cnt];
            i++;
            cnt++;
        }
        buffer[i] = '\0';
        char field[i + 1];
        strcpy(field , buffer);

        int sign = 0;
        if (line[cnt + 1] == 'g'){
            sign = 1;
        }

        cnt += 4;
        int j = 0;
        char buffer2[100];
        while (line[cnt] != '\n' && line[cnt] != '\0' && j < 99){
            buffer2[j] = line[cnt];
            j++;
            cnt++;
        }
        char nums[j + 1];
        buffer2[j] = '\0';
        strcpy(nums , buffer2);
        float value;
        value = atof(nums);
        //printf("found gele\n");
        findgele(data , field , sign , value);
    }    

}


void findstates(storage *data , char *statename){
    storage *newheap = makeheap();
    for (size_t i = 0 ; i < data -> size ; i ++){
        if (strcmp(statename , (data -> ptr)[i].state) == 0){
            realloccheck(newheap);
            (newheap -> ptr)[newheap -> size] = (data -> ptr)[i];
            (newheap -> size)++;
        }
    }
    free(data -> ptr);
    data -> cap = newheap -> cap;
    data -> size = newheap -> size;
    data -> ptr = newheap -> ptr;

    printf("filter: state == %s (%d entries)\n" , statename , data -> size);

}

void findgele(storage *data , char *field , int sign , float value){
    //printf("field being tested: %s , sign: %d , value: %f", field , sign , value);
    //storage *newheap = makeheap();
    char *acceptable[] = {"Education" , "Ethnicities" , "Income"};

    int j =0;
    char buffer2[100];
    while (field[j] != '.' && field[j] != '\0' && j < 99){
        buffer2[j] = field[j];
        j++;
    }
    char nums[j + 1];
    buffer2[j] = '\0';
    strcpy(nums , buffer2);
    //printf("my nums: %s", nums);
    if (strcmp(nums , acceptable[0]) == 0){
        //printf("found edu");
        findgele_edu(data , field, sign , value);
    }
    else if (strcmp(nums , acceptable[1]) == 0){
        findgele_ethn(data , field, sign , value);   
    }
    else if (strcmp(nums , acceptable[2]) == 0){
        findgele_inc(data , field, sign , value);
    }

}


void findgele_edu(storage *data , char *field , int sign , float value){
    int j = 0;
    //printf("\n%c\n" , field[10]);
    //printf("%d" , sign);
    if (field[10] == 'H'){
        j = 1;
    }
    else if (field[10] == 'B'){
        j = 0;
    }
    else{
        perror("your mom :3"); //need to say something about this not being valid
    }
    storage *newheap = makeheap();
    //displayme(data);
    for (int i = 0 ; i < (data -> size) ; i ++){
        county cur = (data -> ptr)[i];
        float perc = cur.edu[j];
/*
        printf("%s , %s\n" , cur.name , cur.state);
        printf("    population: %d\n" , cur.pop);
        printf("    education:\n");
        printf("        High School: %f%%\n" , cur.edu[1]);
        printf("        Bachelor's: %f%%\n" , cur.edu[0]);
        printf("    Ethnicity percentages:\n");
        printf("        American Indian or Alaskan Native: %f\n" , cur.ethn[0]);
        printf("        Asian: %f%%\n" , cur.ethn[1]);
        printf("        Black: %f%%\n" , cur.ethn[2]);
        printf("        Hispanic: %f%%\n" , cur.ethn[3]);
        printf("        Pacific Islander: %f%%\n" , cur.ethn[4]);
        printf("        Two or more races: %f%%\n" , cur.ethn[5]);
        printf("        White: %f%%\n" , cur.ethn[6]);
        printf("        White, not hispanic or latino: %f%%\n" , cur.ethn[7]);
        printf("    Income:\n");
        printf("        Median Household: %d\n" , cur.inc[0]);
        printf("        Per Capita: %d\n" , cur.inc[1]);
        printf("        Below Poverty level: %f%%\n\n" , cur.pov);
*/

        
        if (sign){
            if (perc >= value){
                realloccheck(newheap);
                (newheap -> ptr)[newheap -> size] = (data -> ptr)[i];
                (newheap -> size)++;
                //addentry(newheap , data , i);
            }
        }
        else{
            if (perc <= value){
                printf("%f\n" , perc);
                printf("number: %d\n", i);
                realloccheck(newheap);
                (newheap -> ptr)[newheap -> size] = (data -> ptr)[i];
                (newheap -> size)++;
                printf(" countystate %s\n" , (data -> ptr)[i].state);
                //addentry(newheap , data , i);
            }
        }
    }
    free(data -> ptr);
    data -> cap = newheap -> cap;
    data -> size = newheap -> size;
    data -> ptr = newheap -> ptr;
    printf("filter: %s (%d entries)\n" , field , data -> size);

}

void findgele_ethn(storage *data , char *field , int sign , float value){

}

void findgele_inc(storage *data , char *field , int sign , float value){

}

void addentry(storage *newheap , storage *data , int i ){
    realloccheck(newheap);
    (newheap -> ptr)[newheap -> size] = (data -> ptr)[i];
    (newheap -> size)++;
}







