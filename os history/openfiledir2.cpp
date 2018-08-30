#include<stdio.h>
#include <unistd.h>
#include<dirent.h>
#include <sys/stat.h> 
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<termios.h>
#include<string>
#include<string.h>
#include<iostream>
#include <fstream>

#define RESET "\e[m" 
#define GREEN "\e[32m" 
#define BLUE "\e[36m" 
using namespace std;
int nf,cursor;

struct dll
{
	struct dll* prev;
	char name[100];
	struct dll* next;
};
 dll *head=NULL;

void reset_screen()
{
  printf("\033c");
}

char* curr_work_dir()
{
  char buff[FILENAME_MAX];
  return getcwd( buff, FILENAME_MAX );   
}

void goto_cursor(int x,int y)
{
 printf("\33[%d;%dH",x,y);
}

void open_file(char** curr_dir)
{

 pid_t pid;
 pid=fork();
 if (pid == -1){	
	printf("can't fork, error occured\n");
	exit(EXIT_FAILURE);
}
else if (pid == 0)
{	
	execl("/usr/bin/xdg-open","xdg-open", *curr_dir,(char*)0);
	exit(EXIT_SUCCESS);
}
}

void display_list(char** curr_dir)
{  
   	
 if( *curr_dir==NULL) 
      printf("\n ERROR : Could not get the working directory\n"); 
 DIR *dp = NULL;
 dp = opendir((const char*)*curr_dir); 
 	
 if(dp==NULL) 
 {
 	open_file(curr_dir);
     // printf("\n ERROR : Could not open the working directory\n"); 
 }
 else
 {
 	head=NULL;
 	nf=0;
 	reset_screen();
  struct dirent *sd;
  struct stat status;
  struct passwd *owner; 
  struct group *gp;
  int i;
  char* ch;

  		char* tcurr_dir = (char*)malloc(strlen(*curr_dir)*sizeof(char));
  		strcpy(tcurr_dir,*curr_dir);

        strcat(tcurr_dir,"/");
        
           //cout<<tcurr_dir<<endl;
      while((sd=readdir(dp))!=NULL)
        {
          nf++; 
          char* temp= (char*)malloc(strlen(tcurr_dir)*sizeof(char));
          strcpy(temp,tcurr_dir); 
          strcat(temp,sd->d_name);              	       
               stat(temp,&status); 
               printf((S_ISDIR(status.st_mode)) ? "d" : "-");
    printf( (status.st_mode & S_IRUSR) ? "r" : "-");
    printf( (status.st_mode & S_IWUSR) ? "w" : "-");
    printf( (status.st_mode & S_IXUSR) ? "x" : "-");
    printf( (status.st_mode & S_IRGRP) ? "r" : "-");
    printf( (status.st_mode & S_IWGRP) ? "w" : "-");
    printf( (status.st_mode & S_IXGRP) ? "x" : "-");
    printf( (status.st_mode & S_IROTH) ? "r" : "-");
    printf( (status.st_mode & S_IWOTH) ? "w" : "-");
    printf( (status.st_mode & S_IXOTH) ? "x" : "-"); 
    printf(" ");
    owner=getpwuid(status.st_uid);
   printf("%s ",owner->pw_name);
    gp=getgrgid(status.st_gid);
    printf("%s ",gp->gr_name);
    printf("%ld ",status.st_size);
    ch=ctime(&status.st_mtime);
    for(i=4;i<=15;i++)
      printf("%c",ch[i]);
    printf(" ");
 if(S_ISDIR(status.st_mode)) 
                {                  
                    printf(BLUE "%s\n" RESET,sd->d_name);                  

                } 
                else if(access(sd->d_name,X_OK)) //checking executable file
                {         
                    printf("%s\n",sd->d_name);    
                    
                }
                else              
                 printf(GREEN "%s\n" RESET,sd->d_name); 
                     
                    
                   dll *node1;
                   dll *node=(dll*)malloc(sizeof(dll));  
                  // char* tempcurr_dir = (char*)malloc(strlen(*curr_dir)*sizeof(char));
                   //	strcpy(tempcurr_dir,tcurr_dir);
                     

                  
  	               //strcat(tempcurr_dir,sd->d_name);

                   // cout<<temp<<endl; 
                   strcpy(node->name,temp);   
                            
                    node->next=NULL;
                    if(head==NULL) 
                    {
                    	node->prev=NULL;
                       head=node;
                    }
                    else
                    {  node1=head;
                     while(node1->next!=NULL)
                    	node1=node1->next;
                       node1->next=node;
                       node->prev=node1;
                    }                 
                    free(temp);
                    //free(node);
                   // free(tempcurr_dir);
            
        }    
      free(tcurr_dir);
}
      cursor=1;    
      goto_cursor(0,0);
}

int main()
{

  reset_screen();  //calling screen reset function

  struct termios initial_setting, new_setting;
    tcgetattr(0,&initial_setting);
    new_setting=initial_setting;
    new_setting.c_lflag &= ~ICANON;
    new_setting.c_lflag &= ~ECHO;
    tcsetattr(0,TCSANOW,&new_setting);
  char *curr_dir;
  curr_dir = curr_work_dir(); //calling current directory function

  display_list(&curr_dir); //calling ls function

  char c='\0';
 
    while(1)
{

  c=getchar();
  if(c==':')
  {
   break;
  }
  if(c=='A')
  { 
  	if(cursor>1) 
  		cursor--;
   printf("\033[1A");
  
  }
  else if(c=='B')
  { 
  	if(cursor<nf)  	  
      	{
   	   	cursor++;
   	   	printf("\033[1B");
   	   	
   	    }
  }
  else if(c=='C')
   printf("\033[1C");
  else if(c=='D')
   printf("\033[1D");
  else if(c=='\n')
  {
  	 dll *tnode=head;
  	while(cursor>1)
  	{
  		tnode=tnode->next;
  		cursor--;
  	}

  	curr_dir = tnode->name;  	
  	//strcat(curr_dir,"/");
  	//strcat(curr_dir,tnode->name);  		  			
  	display_list(&curr_dir);
  }
}
tcsetattr(0,TCSANOW,&initial_setting);
  return 0;
}
