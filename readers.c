#include "reader.h"

int main(int argc , char *argv[])
{
//first step: parse the given csv
    FILE *csvfile = openingfile(argc , argv , 1);  
    if (csvfile == NULL) {
        perror("Error opening file");
        exit(1);
    }
    storage *info = makeheap();
    fillheap(info , csvfile);

//second step: the the operations
    FILE *opfile = openingfile(argc , argv , 2);
    if (opfile == NULL) {
        perror("Error opening file");
        free(info -> ptr);
        free(info);
        fclose(csvfile);
        //fclose(opfile);
        exit(1);
    }
    readops(opfile , info);
  
//close all my pointers
    free(info -> ptr);
    free(info);
    fclose(csvfile);
    fclose(opfile);
    return 0;
}

FILE *openingfile(int argc, char *argv[] , int i){
    FILE *file;
    if (argc == 3){
        file = fopen(argv[i], "r");
    }
    else{
        perror("Incorrect # of files given");
        exit(0);
    }  
    return file;
}

//create a new empty storage container
storage *makeheap(){
    storage *newheap = (storage *)malloc(sizeof(storage));
    newheap->cap =  150; //change it back later
    newheap->size = 0;
    newheap->ptr = (county *)calloc(((newheap->cap) + 1) , sizeof(county));
    
    return newheap;
}


void fillheap(storage *info , FILE *file){
    char *line = NULL;
    size_t size = 0;
    getline(&line , &size , file);
    //free(line);
    while(getline(&line , &size , file) > 0){
        realloccheck(info);
        county newcounty = parseline(line);
        if (newcounty.state[3] == 'Z'){
            printf(" could not parse county at line %d\n" , info -> size);
        }
        else{
            if (info -> size == 152){
                (info -> ptr)[info -> size] = newcounty;
            }
        (info -> ptr)[info -> size] = newcounty;
        (info -> size)++;
        }
    }
    printf("%d records loaded\n" , info -> size);
    free(line);
}

/*
void realloccheck(storage *info){
    if ((info -> size) == (info -> cap)){
        county *newptr = (county *)malloc((151 + (info -> cap)) * sizeof(county));
        for (int i =0 ; i < (info -> size) ;  i++){
            newptr[i] = (info -> ptr)[i];
        }
        (info -> cap) += 150;
        free(info -> ptr);
        (info -> ptr) = newptr;
    }
}
*/

void realloccheck(storage *info){
    if ((info -> size) + 1 == info -> cap){
        county *newarr = realloc(info -> ptr , (155 + (info -> cap)) * sizeof(county));
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
        int x = 1;
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
                x = sscanf(token, "\"%f\"", &value);
                cnt.edu[0] = value;
                break;
            case 6: // Education.High School or Higher
                x = sscanf(token, "\"%f\"", &value);
                cnt.edu[1] = value;
                break;
            case 11: // Start of ethnicities
                x = sscanf(token, "\"%f\"", &value);
                cnt.ethn[0] = value;
                break;
            case 12:
                x = sscanf(token, "\"%f\"", &value);
                cnt.ethn[1] = value;
                break;
            case 13:
                x = sscanf(token, "\"%f\"", &value);
                cnt.ethn[2] = value;
                break;
            case 14:
                x = sscanf(token, "\"%f\"", &value);
                cnt.ethn[3] = value;
                break;
            case 15:
                x = sscanf(token, "\"%f\"", &value);
                cnt.ethn[4] = value;
                break;
            case 16:
                x = sscanf(token, "\"%f\"", &value);
                cnt.ethn[5] = value;
                break;
            case 17:
                x = sscanf(token, "\"%f\"", &value);
                cnt.ethn[6] = value;
                break;
            case 18:
                x = sscanf(token, "\"%f\"", &value);
                cnt.ethn[7] = value;
                break;
            case 25: // Income.Median Household Income
                x = sscanf(token, "\"%d\"", &len);
                cnt.inc[0]= len;
                break;
            case 26: // Income.Per Capita Income
                x = sscanf(token, "\"%d\"", &len);
                cnt.inc[1]= len;
                break;
            case 27: // Income.Persons Below Poverty Level
                x = sscanf(token, "\"%f\"", &value);
                cnt.pov = value;
                break;
            case 38:
                x = sscanf(token, "\"%d\"", &len);
                cnt.pop = len;
                break;
            default:
                // For fields not stored in the structure, do nothing
                break;
        }
        token = strtok(NULL, ","); // Get the next token
        tokenIndex++; // Move to the next field
        if (x == 0){
            cnt.state[3] = 'Z';
            return cnt;
        }
    }
    //printf("%d\n", tokenIndex);
    
    if (tokenIndex != 52){
        printf("whattttt\n");
        cnt.state[3] = 'Z';
        return cnt;   
    }
    

    return cnt;
}





void readops(FILE *file , storage *data){
    char *line = NULL;
    size_t size = 0;
    char *acceptable[] = {"display" , "filter" , "population" , "percent"};
    int j = 0;
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
            populationme(data , line);
        }
        else if(strcmp(op , acceptable[3]) == 0){
            percentme(data, line);
        }
        else{
            printf("must be a valid operation (display, filter , population, percent) %d\n" , j);
        }
        j++;
        
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
    else if (line[6] == ':'){
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
    else{
        printf("command formated incorrectly, must be ':' or '-' \n");
        return;
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
    
    free(newheap);


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
    //printf("hi\n\n");
    //printf("%s\n\n" , nums);
    //printf("my nums: %s", nums);
    if (strcmp(nums , acceptable[0]) == 0){
        //printf("hi\n\n");
        //printf("found edu");
        findgele_edu(data , field, sign , value);
    }
    else if (strcmp(nums , acceptable[1]) == 0){
        //printf("hi\n\n");
        findgele_ethn(data , field, sign , value);   
    }
    else if (strcmp(nums , acceptable[2]) == 0){
        findgele_inc(data , field, sign , value);
    }
    else{
        printf("field formatted incorrectly, must be Education, Ethnicities, Income\n");
        return;
    }

}


void findgele_edu(storage *data , char *field , int sign , float value){
    int j = 0;
    if (field[10] == 'H'){
        j = 1;
    }
    else if (field[10] == 'B'){
        j = 0;
    }
    else{
        perror("field formatted incorrectly, education"); //need to say something about this not being valid
        return;
    }
    storage *newheap = makeheap();
    //displayme(data);
    for (int i = 0 ; i < (data -> size) ; i ++){
        county cur = (data -> ptr)[i];
        float perc = cur.edu[j];
        if (sign){
            if (perc >= value){
                addentry(newheap , data , i);
            }
        }
        else{
            if (perc <= value){
                addentry(newheap , data , i);
            }
        }
    }
    free(data -> ptr);
    data -> cap = newheap -> cap;
    data -> size = newheap -> size;
    data -> ptr = newheap -> ptr;
    free(newheap);
    char *signer = "le";
    if (sign){
        signer = "ge";
    }
    printf("filter: %s %s %f (%d entries)\n" , field , signer , value, data -> size);

}

void findgele_ethn(storage *data , char *field , int sign , float value){
    int j = 0; //(s)
    //printf("%s\n\n" , field);
    if (strcmp("Ethnicities.Asian Alone" , field) == 0){
        j = 1;
    }
    else if (strcmp("Ethnicities.Black Alone" , field) == 0){
        j = 2;
    }
    else if(strcmp("Ethnicities.Hispanic or Latino" , field) == 0){
        j = 3;
    }
    else if(strcmp("Ethnicities.Native Hawaiian and Other Pacific Islander Alone" , field) == 0){
        j = 4;
    }
    else if(strcmp("Ethnicities.Two or More Races" , field) == 0){
        j =5;
    }
    else if(strcmp("Ethnicities.White Alone" , field) == 0){
        j=6;
    }
    else if(strcmp("Ethnicities.White Alone not Hispanic or Latino" , field) == 0){
        j = 7;
    }
    else if(strcmp("Ethnicities.American Indian and Alaska Native Alone" , field) == 0){
        j = 0;
    }
    else{
        perror("field formatted incorrectly: ethnicities"); //need to say something about this not being valid
        return;
    }
    storage *newheap = makeheap();
    //displayme(data);
    for (int i = 0 ; i < (data -> size) ; i ++){
        county cur = (data -> ptr)[i];
        float perc = cur.ethn[j];
        if (sign){
            if (perc >= value){
                addentry(newheap , data , i);
            }
        }
        else{
            if (perc <= value){
                addentry(newheap , data , i);
            }
        }
    }
    free(data -> ptr);
    data -> cap = newheap -> cap;
    data -> size = newheap -> size;
    data -> ptr = newheap -> ptr;
    free(newheap);
    char *signer = "le";
    if (sign){
        signer = "ge";
    }
    printf("filter: %s %s %f (%d entries)\n" , field , signer , value, data -> size);

}

void findgele_inc(storage *data , char *field , int sign , float value){
    storage *newheap = makeheap();
    for (int i = 0 ; i < (data -> size) ; i ++){
        county cur = (data -> ptr)[i];
        float perc = cur.pov;
        if (sign){
            if (perc >= value){
                addentry(newheap , data , i);
            }
        }
        else{
            if (perc <= value){
                addentry(newheap , data , i);
            }
        }
    }
    free(data -> ptr);
    data -> cap = newheap -> cap;
    data -> size = newheap -> size;
    data -> ptr = newheap -> ptr;
    free(newheap);
    char *signer = "le";
    if (sign){
        signer = "ge";
    }
    printf("filter: %s %s %f (%d entries)\n" , field , signer , value, data -> size);

}

void addentry(storage *newheap , storage *data , int i ){
    realloccheck(newheap);
    (newheap -> ptr)[newheap -> size] = (data -> ptr)[i];
    (newheap -> size)++;
}


void populationme(storage *data , char *line){

    int x;
    float y;
    if (line[10] == '-'){
        
        x= poptotal(data);
        printf("2014 population: %d\n" , x);
    }
    else if (line[10] == ':'){
        y = popfield(data , line);
        if (y == -1){
            return;
        }
        char *field = findfield(line);
        if (field == NULL){
            return;
        }
        printf("2014 %s population: %f\n", field, y);
        free(field);
    }
    else{
        printf("field formatted incorrectly, must be '-' or ':' after population\n");
    }
    
}

int poptotal(storage *data){
    int total = 0;
    for (size_t i = 0 ; i < data -> size ; i++){
        county specific = (data -> ptr)[i];
        int curpop = specific.pop;
        total += curpop;
        //printf("%d\n" , curpop);
    }
    return total;
}

char *findfield(char *line){
    int i = 0;
    int check = 0;
    while (line[i] != '.'){
        if (line[i] == '\0'){
            check = 1;
            break;
        }
        i++;
    }
    if (check){
        return NULL;
    }
    i++;
    int m = 0;
    char buffer3[100];
    while (line[i] != '.' && line[i] != '\0'&& line[i] != '\n' && i < 99){
        buffer3[m] = line[i];
        m++;
        i++;
    }
    char *field = (char *)malloc((m + 1)*sizeof(char));
    buffer3[m] = '\0';
    strcpy(field , buffer3);
    return field;
}

float popfield(storage *data , char *line){
    char *acceptable[] = {"Education" , "Ethnicities" , "Income"};
    int j = 0;
    while (line[j] != ':'){
        if (line[j] == '\0'){
            perror("formating error");
            return -1;
        }
        j++;
    }
    j++;

    int i = 0;
    char buffer2[100];
    while (line[j] != '.' && line[j] != '\0'&& line[j] != '\n' && j < 99){
        buffer2[i] = line[j];
        j++;
        i++;
    }
    char nums[i + 1];
    buffer2[i] = '\0';
    strcpy(nums , buffer2);
    j++;
    char *field = findfield(line); 
    int x;
    //printf("current nums: %s\n" , nums);
    if (strcmp(nums , acceptable[0]) == 0){
        //printf("hi\n\n");
        //printf("found edu");
        x= findpop_edu(data , field);
    }
    else if (strcmp(nums , acceptable[1]) == 0){
        //printf("hi\n\n");
         x = findpop_eth(data , field);   
    }
    else if (strcmp(nums , acceptable[2]) == 0){
        x = findpop_pov(data , field);
    }
    else{
        printf("incorrect format");
        return -1;
    }
    free(field);
    return x;
}

float findpop_edu(storage *data , char *field){
    int j = 0;
    if (field[0] == 'H'){
        j = 1;
    }
    else if (field[0] == 'B'){
        j = 0;
    }
    else{
        perror("not a valid field (edu)"); //need to say something about this not being valid
        return -1;
    }

    float pop_total = 0;
    for (int i = 0 ; i < (data -> size) ; i ++){
        county cur = (data -> ptr)[i];
        float perc = cur.edu[j];
        float pop = cur.pop * perc * 0.01;
        pop_total += pop;
    }
    //printf("2014 %s population: %f\n", field, pop_total);
    return pop_total;
}

float findpop_eth(storage *data , char *field){
    //printf("%s\n" , field);
    int j = 0; //(s)
    //printf("%s\n\n" , field);
    if (strcmp("Asian Alone" , field) == 0){
        j = 1;
    }
    else if (strcmp("Black Alone" , field) == 0){
        j = 2;
    }
    else if(strcmp("Hispanic or Latino" , field) == 0){
        j = 3;
    }
    else if(strcmp("Native Hawaiian and Other Pacific Islander Alone" , field) == 0){
        j = 4;
    }
    else if(strcmp("Two or More Races" , field) == 0){
        j =5;
    }
    else if(strcmp("White Alone" , field) == 0){
        j=6;
    }
    else if(strcmp("White Alone, not Hispanic or Latino" , field) == 0){
        j = 7;
    }
    else if(strcmp("American Indian and Alaska Native Alone" , field) == 0){
        j = 0;
    }
    else{
        printf("field: %s" , field);
        perror("not a valid field(education)" ); //need to say something about this not being valid
        return -1;
    }
    float pop_total = 0;
    for (int i = 0 ; i < (data -> size) ; i ++){
        county cur = (data -> ptr)[i];
        float perc = cur.ethn[j];
        float pop = cur.pop * perc * .01;
        pop_total += pop;
    }    

    //printf("2014 %s population: %f\n", field, pop_total);
    return pop_total;
}
float findpop_pov(storage *data , char *field){
    float pop_total = 0;
    for (int i = 0 ; i < (data -> size) ; i ++){
        county cur = (data -> ptr)[i];
        float perc = cur.pov;
        float pop = cur.pop * perc * .01;
        pop_total += pop;
    }
    //printf("2014 %s population: %f\n", field, pop_total); 
    return pop_total;   

}

void percentme(storage *data , char *line){
    int totalpop; 
    totalpop = poptotal(data);
    float tot_type = popfield(data, line);
    if (totalpop == -1){
        printf("not a valid field\n");
        return;
    }
    float fintot = (tot_type / totalpop) * 100;
    char *field = findfield(line);
    printf("2014 %s percentage: %f\n" , field , fintot);
    free(field);
}




