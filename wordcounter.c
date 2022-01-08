/*
	CO222 - Programming Methodology
	Project 2
	E/17/212 - Morais K.W.G.A.N.D.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct _ { 	//user defined data type to store characters & characters' count
    int index;
    char cha;
}cha_t;

typedef struct wd {	//user defined data type to store words & words' count
    char *str;
    int count; 
    struct wd *next; 
}words;

//prototypes of functions

void append(char *word);
void sortlis(cha_t a[], int size);
int countdig(int x);
void printspace(int cnt);
void sortList(words *wordLis);
void delete();

words* root = NULL;		//initialise the root variable to NULL 

int main(int argc, char** argv){
	FILE *fp;
	char argvstr[20],argvstr2[20],ch,cword[100];
	int i,method1 = 1,scale = 0,x=0,k=0,cn=0,t;
	int erm,er1,count=0,printcount=10;
	cha_t chx[37];
    		

	for(i=1;i<argc;i++){								//handling command line arguments	
		strcpy(argvstr,argv[i]);						//copy the argv arguments to the string array
	
		if((argvstr[0]=='-') && ((strcmp(argv[i],"-c")==0) || (strcmp(argv[i],"-w")==0) || (strcmp(argv[i],"--scaled")==0))){	//condition for checking input files 
			erm = 1;
			if(strcmp(argv[i],"-c")==0) {
				er1=1;
			}
			if(er1==1 && (strcmp(argv[i],"-w")==0)){	//condition for checking '-c' & '-w' use together
				t=1;
			}
		}
		else if((argvstr[0]=='-') && (strcmp(argv[i],"-l")==0)){
			if(i != argc-1){
				 
				if(atoi(argv[i+1])>0){					//condition for checking '-l +positive number'
					erm = 1;
					i++;				
				}
				else if (strcmp(argv[i+1],"0")==0){		//condition for checking '-l 0'
					return 0;				
				}
				else if (atoi(argv[i+1])<0){			//condition for checking '-l +negative number'
					printf("Invalid option(negative) for [-l]\n");
        				printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        				return 0;				
				}
				else{
					printf("Invalid options for [-l]\n");	//condition for checking innvalid options for '-l'
        				printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        				return 0;
				}				
			}
			else{
				printf("Not enough options for [-l]\n");
        			printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        			return 0;			
			}	
		}
		else {											//condition to check that the input files are given
			erm = 0;
			break;
		}
	}
	if(t==1){		
		printf("[-c] and [-w] cannot use together\n");
        	printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        	return 0;
	}
	if(erm == 1 || argc ==1){							//condition for checking that files are not given
		printf("No input files were given\n");
        	printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        	return 0;
	}
	else {
		for(i=1;i<argc;i++){
			if(strcmp(argv[i],"-c")==0){
				method1 = 0;
				er1 = 1;	
			}
			else if(er1==1 && (strcmp(argv[i],"-w")==0)){
				t=1;
			}
			else if(strcmp(argv[i],"-w")==0){
				method1 = 1;
			}
			else if(strcmp(argv[i],"--scaled")==0){
				scale = 1;
			}
			else if(strcmp(argv[i],"-l")==0){
				if(i != argc-1){
					if(atoi(argv[i+1])>0){				//condition for checking '-l +positive number'
						printcount = atoi(argv[i+1]);
						i++;				
					}
					else if (strcmp(argv[i+1],"0")==0){	//condition for checking '-l 0'
						return 0;				
					}
					else if (atoi(argv[i+1])<0){		//condition for checking '-l +negative number'
						printf("Invalid option(negative) for [-l]\n");
        					printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        					return 0;				
					}
					else{
						printf("Invalid options for [-l]\n");
        					printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        					return 0;
					}				
				}
				else{
					printf("Not enough options for [-l]\n");
        				printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        				return 0;			
				}

			}
			else if(fopen(argv[i],"r") == NULL){		//condition to check that files can't be read
				strcpy(argvstr2,argv[i]);				//copy argv argument to an array
				if(argvstr2[0]=='-'){
					printf("Invalid option [%s]\n",argv[i]);
        				printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        				return 0;
				}
				else {									//condition to check that file can't be opened
					t=2;
									
				}
			}	
		}
	}
	if(t==1){					//printing appropriate error message
		printf("[-c] and [-w] cannot use together\n");
        	printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        	return 0;
	}
	else if(t==2){				//printing appropriate error message
		printf("Cannot open one or more given files\n");
       		return 0;
	}
	if(method1 == 0){			//condition to check that input arguments for counting characters
		int m=1,j=1;
		for(i=0;i<36;i++){		//initialize the index value of all the elements in 'chx' to '0' 
        	chx[i].index=0;
    		}
		while(m<argc){			//condition to read all the arguments that can be read
			fp = fopen(argv[m], "r");	//open the files to read
			if(fp==NULL){				//skipping the files that cannot be read
                		m++;    
            		}
			else{
				while((ch = fgetc(fp)) != EOF){		//reading the file one by one of characters
					if(ch>= 'A' && ch <= 'Z'){
                        			ch=ch+32;		//convert uppercase letters to lowercase letters
                   			 }
					int k,n=0;
                    			if((ch>= 'a' && ch <= 'z') || (ch>='0' && ch<='9')){
                       				count++;		//counting characters of read files
                        			for(k=0;k<j;k++){
                            				if(ch==chx[k].cha){
                                				chx[k].index++;		//counting same characters' count
                                    				n=-1;
                           				}            
                        			}
                       			 	if(n == 0){
                            				chx[j-1].cha=ch;			//adding new characters
                            				chx[j-1].index++;
                            				j++;
                        			}            
                    			}
				}
				m++;
            			fclose(fp);	//closing the file
			}
		}
		if (count==0){					//condition to check that read files are empty
			printf("No data to process\n");
			return 0;	
		}
		sortlis(chx,36);				//sorting the list in descending order
		int v,g;
		for(v=0;v<36;v++){
        	float k=(float)chx[v].index*100.0/(float)count;		//calculating percentages of count of each characters
        	if(k>0.0){
           		cn++;  					//counting different characters      
        	}
    	}
	if(printcount<cn){				//comparing user givern printcount & counted different characters
        	g=printcount;
    	}
    	else{
        	g=cn;
	}
    	printf("\n");
	// printing the graph
    	for(i=0;i<g;i++){
		float maxcount = (float)chx[0].index*100.0/(float)count;	//finding maximum percentage(for the purpose of scaling) 
		int k3 = countdig(maxcount);								//finding string length of maximum percentage
		int bar = 72-k3;
        	float k=(float)chx[i].index*100.0/(float)count;		//finding other characters' percentages
        	int h;
	
        	if(scale==1){		//condition for scaling 
          		h=k*bar/maxcount;
        	}
        	else {				//condition for non scaling
            	h=bar*(chx[i].index)/count;
        	}
		//printing bar graph
        	printf("   \u2502");
        	int j;
        	for( j=0;j<h;j++){
            	printf("\u2591");        
        	}
        	printf("\n %c ",chx[i].cha);//printing relevent character 
        	printf("\u2502");
        	for(j=0;j<h;j++){
            	printf("\u2591");        
        	}
        	printf("%.2f%%",k);			//printing percentage value
        	printf("\n");
        	printf("   \u2502");
        	for(j=0;j<h;j++){
            	printf("\u2591");        
        	}
        	printf("\n");
        	printf("   \u2502\n");
    	}
    	printf("   \u2514");
    	for(i=0;i<76;i++){
        	printf("\u2500");         
    	}
    	printf("\n");  
	}

	else if(method1==1){		//condition for printing words' frquency graph
	int m=1;
        while (m <argc){		//condition to read all the arguments that can be read
            fp = fopen(argv[m], "r");	//open the files to read
            if(fp==NULL){		//skipping the files that cannot be read
                m++;    
            }
            else{
                while ((ch = fgetc (fp)) != EOF){
                    if(ch==' ' || ch=='\n'  || ch=='\t'){	//condition to check that there is a space or a new line or a tab
                        if(x==1){
                            x=0;
                            cword[k] = '\0';	//making last character to null
                            append(cword);		//adding word into the list
                            k=0;            
                        }                
                    }
                    else{						//condition to check that there is no space or new line
                        if((ch>= 'A' && ch <= 'Z')||(ch>= 'a' && ch <= 'z')){
                            x = 1;
                            cword[k++] = tolower (ch);		//adding lowercase letter to the word
                        }
                        else if((ch>= '0' && ch <= '9')){
                            x = 1;
                            cword[k++] = ch;   				//adding numbers to the word
                        }
                    }
            	}
            	m++;
            	fclose(fp);	//close the file
        	}
    	}
	delete();
    	sortList(root);	//sort the list
    	words *nowd;    
    	nowd = root;
        while (nowd != NULL) { 
        	if(nowd->count>0){
           		cn++;		//counting differnt words
		} 
        	count=count+nowd->count; //counting all the words
            	nowd = nowd->next;
        }
	if (count==0){		//condition to check read files are empty
		printf("No data to process\n");
		return 0;	
	}
    	nowd = root;
    	int g,d=0;
    	if(printcount<cn){	//comparing user givern printcount & counted different characters
        	g=printcount;
    	}
    	else{
        	g=cn;
    	}
    	int max=0;
    	while (d != g){
    		int len=strlen(nowd->str);	//finding length of the longest word in given range of printing
    		if(max<len)
        		max=len;
    		nowd = nowd->next;
    		d++;
    	}
    	nowd = root;
    	printf("\n");
	float k2;
    	for(i=0;i<g;i++){
        	int maxcount;
        	if(i==0){					//to check maximum percentage word (for the purpose of scaling)
            	maxcount = nowd->count;
			k2=(maxcount)*100.0/count;		//calculate percentage
        	}
        	float k=(nowd->count)*100.0/count;	//calculate percentages of each words
			int k1 = countdig(k2);				//finding digit count of maximum percentage
			int bar = 73-(max + k1);			//for scaling purpose			
        	int h;
        	if(scale==1){
            		h=((float)(nowd->count)*bar/(float)maxcount);		//scaling
        	}
        	else{
            		h=k*(float)bar/100.0;
        	}
        	printspace(max+2);
        	printf("\u2502");
        	int j;
        	for(j=0;j<h;j++){
            		printf("\u2591");        
        	}
        	int u=strlen(nowd->str);			//finding length of words
        	printf("\n %s",nowd->str);			//printing words
        	printspace(max-u+1);
        	printf("\u2502");
        	for(j=0;j<h;j++){
            		printf("\u2591");        
        	}
        	printf("%.2f%%",k);					//printing percentages
        	printf("\n");
        	printspace(max+2);
        	printf("\u2502");
        	for(j=0;j<h;j++){
            		printf("\u2591");        
        	}
        	printf("\n");
        	printspace(max+2);
        	printf("\u2502\n");
        	nowd = nowd->next;
    	}
    	printspace(max+2);
    	printf("\u2514");
    	for(i=0;i<77-max;i++){
        	printf("\u2500");
        }    
    	printf("\n");
	}

return 0;
}

/* FUNCTIONS*/

//function to  sort characters' count list (Bubble sort method is used)
void sortlis(cha_t a[], int size){
    int done=0,temp;
    char c;
    
    while(!done){
        done=1;
        int i;
        for(i=0;i<size-1;++i){
            if(a[i].index<a[i+1].index){
                temp = a[i].index;
                c = a[i].cha;
                a[i].index = a[i+1].index;
                a[i].cha = a[i+1].cha;
                a[i+1].index = temp;
                a[i+1].cha = c;
                done = 0;
            }}}}

//function to count numbers of integer
int countdig(int x){
	int y=0;
	while(x>0){
		x=x/10;
		y++;
	}
	return y;
}


//function to add new word, search a same word & increment the count of the words
void append(char *word){
	int len = strlen(word);		//finding length of the word	
	int y=0;
	words* temp;		
	temp = (words*)malloc(sizeof(words));		//dynamically allocate memory for temp
	temp->str = (char *) malloc (len + 1);		//dynamically memory allocate for the string 
	strcpy(temp->str,word);				//copy word 
	temp->count = temp->count+1;			//increase the count by 1
	temp->next=NULL;				//making next node to NULL

	if(root == NULL){				//condition for checking root is empty
		root=temp;
	}
	else{						//condition for checking that root is not null
		words* p;
		p =root;
		
		while(p->next != NULL){			//condition to loop until root becomes to null 
			if(strcmp(p->str,word)==0){	//checking that the word is in previous nodes
				p->count++;		//increasing the count
				y=1;
				break;			//if found same word , skipping to next word
							
			}
				p = p->next;
		}
		if(y==0){			//condition to check that word (that is not in previous nodes)
			p->next = temp;		//that word adding to bottom of the list
		}
	}
}

//function to sort a linked list(Bubble sort method)
void sortList(words *wordLis){
    words *current,*index=NULL;
    int temp,swapped;
    char *ch;
    if(wordLis==NULL){
        return;
    }
    do{    
        swapped = 0;
        current = wordLis;
        while(current->next != index){
        
            if(current->count < current->next->count){
                temp = current->count;
                ch = current->str;
                current->count=current->next->count;
                current->str=current->next->str;
                current->next->count=temp;
                current->next->str=ch;
                swapped = 1;
            }
        current=current->next;
        }
        index = current;
        }
    while(swapped);
    }

//function to print spaces for a given count
void printspace(int cnt){
	int b;
    	for(b=0;b<cnt;b++){
        	printf(" ");
	}
}

//function to delete the words that are same in linked list
void delete(){
	if(root != NULL){
		words* q;
		q = root;
		while(q->next != NULL){
			if(strcmp(q->str,q->next->str)==0){
				q->count++;
				free(q->next);	
				if(q->next->next != NULL){
					q->next = q->next->next;
				}
			}
			q=q->next;
		}
	}
}

//End of the code
