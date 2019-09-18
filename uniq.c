#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void
uniq(int fd)
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
		if (last_line == 0 || strcmp(last_line, curr_line)!=0) {
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
    
    if(argc <= 1){
        uniq(0);
        exit();
    }
    
    for(i = 1; i < argc; i++){
        if((fd = open(argv[i], 0)) < 0){
            printf(1, "uniq: cannot open %s\n", argv[i]);
            exit();
        }
        uniq(fd);
        close(fd);
    }
    exit();
}
