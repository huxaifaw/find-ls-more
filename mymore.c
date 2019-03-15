//Percentage of file contents displayed-I/O redirection handing-reverse video feature using control sequence introducer \033[-read and print one page then pause for a few special commands ('q', ' ' , '\n')

#include <stdio.h>
#include <stdlib.h>
#define	PAGELEN	20
#define	LINELEN	512

void do_more(FILE *);
int get_input(FILE*, int, int);
int get_total_num_of_lines(FILE*);
int main(int argc , char *argv[])
{
	int i=0;
	if (argc == 1){
		do_more(stdin);
   	}
   	FILE * fp;
   	while(++i < argc){
      		fp = fopen(argv[i] , "r");
      		if (fp == NULL){
        		perror("Can't open file");
         		exit (1);
      		}
      	do_more(fp);
      	fclose(fp);
   	}  
	return 0;
}
int get_total_num_of_lines(FILE* fp)
{
	int n = 0;
	char buffer[LINELEN];
	while(fgets(buffer, LINELEN, fp))
		n++;
	return n;
}
void do_more(FILE *fp)
{
   	int num_of_lines = 0;
   	int rv, n = 0;
	int total_noOfLines = get_total_num_of_lines(fp);
	fseek(fp, 0, SEEK_SET);
   	char buffer[LINELEN];
   	FILE* fp_tty = fopen("/dev//tty", "r");
   	while (fgets(buffer, LINELEN, fp)){
      		fputs(buffer, stdout);
      		num_of_lines++;
      		n++;
	      	if (num_of_lines == PAGELEN){
		 	rv = get_input(fp_tty, n, total_noOfLines);		
		 	if (rv == 0) {//user pressed q
		    		printf("\033[1A \033[2K \033[1G");
		    		break;
		 	}
		 	else if (rv == 1) {//user pressed space bar
		    		printf("\033[1A \033[2K \033[1G");
		    		num_of_lines -= PAGELEN;
		 	}
		 	else if (rv == 2) {//user pressed return/enter
		    		printf("\033[1A \033[2K \033[1G");
			 	num_of_lines -= 1; //show one more line
		 	}
		 	else if (rv == 3) {//invalid character
		    		printf("\033[1A \033[2K \033[1G");
		    		break; 
		 	}
	      	}
	}
}
int get_input(FILE* cmdstream, int num_of_lines, int total_noOfLines)
{
   	int c;
	int percentage = (float)num_of_lines/(float)total_noOfLines*100;
   	printf("\033[7m --more--(%d%) \033[m", percentage);
     	c = getc(cmdstream);
      	if(c == 'q')
	 	return 0;
      	if ( c == ' ' )			
	 	return 1;
      	if ( c == '\n' )	
	 	return 2;	
      	return 3;
}