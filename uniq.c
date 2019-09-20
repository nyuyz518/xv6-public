#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

char to_lower(char a){
    if (a >= 'A' && a <= 'Z'){
        return a + 0x20; 
    } else {
        return a;
    }
}

int strccmp(char* s1, char* s2, int ci){

    int ret = 0;
    char c1, c2;
    while( *s1!= 0 && *s2 != 0){
        if(ci) {
            c1 = to_lower(*s1);
            c2 = to_lower(*s2);
        } else {
            c1 = *s1;
            c2 = *s2;
        }
        if((ret = c2 - c1) != 0){
            return ret;
        } 
        s1++;
        s2++;
    } 
    return *s2 - *s1;
}

void
uniq(int fd, int opt_c, int opt_i, int opt_d)
{
    int n,i, cl = 0;
    char* last_line = 0;
    char* curr_line = 0;
    curr_line = malloc(sizeof(char)*1024);
    memset(curr_line, 0, sizeof(char)*1024);
    
    while((n = read(fd, buf, sizeof(buf))) > 0) {
    for(i = 0; i < n; i++){
        curr_line[cl] = buf[i];
        cl ++ ;
        if(buf[i] == '\n'){
        if (last_line == 0 || strccmp(last_line, curr_line, opt_i)!=0) {
            /*if last != current print out current */
            if (write(1, curr_line, cl) != cl) {
                printf(1, "uniq: write error\n");
                exit();
            }  

            /* assign last_line*/

            if(last_line != 0){
            /*clear last line*/
                free(last_line);
            last_line = 0;
            }
            last_line = curr_line;

            /* new curr line */
                    curr_line = malloc(sizeof(char)*1024);
            cl = 0;
                    memset(curr_line, 0, sizeof(char)*1024);
        } else {
        /*if strcmp last == curr? no output clear curr_line*/
            cl = 0;
                    memset(curr_line, 0, sizeof(char)*1024);
            }
        }
    }   
    }
    if(n < 0){
        printf(1, "uniq: read error\n");
        exit();
    }
}

int
main(int argc, char *argv[])
{
    int fd, i;
    int opt_c = 0, opt_d = 0, opt_i = 0;
    char* filename = 0;

    for(i = 1; i < argc; i++){
    if(strcmp(argv[i],"-c") == 0){
        opt_c = 1;
    } else if (strcmp(argv[i], "-d") == 0){
        opt_d = 1;
    } else if (strcmp(argv[i], "-i")== 0){
        opt_i = 1;
    } else {
        filename = argv[i];     
    } 
    }
    if(filename == 0){
        uniq(0, opt_c, opt_i, opt_d);
        exit();
    } else {
        if((fd = open(filename, 0)) < 0){
            printf(1, "uniq: cannot open %s\n", argv[i]);
            exit();
        }
        uniq(fd, opt_c, opt_i, opt_d);
        close(fd);
    }
    exit();
}
