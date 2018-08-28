#include<stdio.h>
#include <unistd.h>
#include<dirent.h>
#include <sys/stat.h> 
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<termios.h>

#define RESET "\e[m" 
#define GREEN "\e[32m" 
#define BLUE "\e[36m" 

struct dll
{
	struct dll* prev;
	string name;
	struct dll* next;
}

void reset_screen()
{
  printf("\033c");
}

char* curr_work_dir()
{
  char buff[FILENAME_MAX];
  return getcwd( buff, FILENAME_MAX );   
}

void display_list(char *curr_dir)
{
	reset_screen();
 if( curr_dir==NULL) 
      printf("\n ERROR : Could not get the working directory\n"); 
 DIR *dp = NULL;
 dp = opendir((const char*)curr_dir); 
 if(dp==NULL) 
      printf("\n ERROR : Could not open the working directory\n"); 
  struct dirent *sd;
  struct stat status;
  struct passwd *owner; 
  struct group *gp;
  int i;
  char* ch;
      while((sd=readdir(dp))!=NULL)
        {
          
  
               stat(sd->d_name,&status); 
               printf( (S_ISDIR(status.st_mode)) ? "d" : "-");
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
                    printf(BLUE"%s\n"RESET,sd->d_name); 
                } 
                else if(access(sd->d_name,X_OK))
                {         
                    printf("%s\n",sd->d_name);    
                    
                }
                else              
                 printf(GREEN"%s\n"RESET,sd->d_name); 

            
        }     
    
}

void goto_cursor(int x,int y)
{
 printf("\33[%d;%dH",x,y);
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
  display_list(curr_dir); //calling ls function
  goto_cursor(0,0);
  char c='\0';

  while(c!=':')
{
  c=getchar();
  if(c=='A')
   printf("\033[1A");
  else if(c=='B')
   printf("\033[1B");
  else if(c=='C')
   printf("\033[1C");
  else if(c=='D')
   printf("\033[1D");
}
tcsetattr(0,TCSANOW,&initial_setting);
  return 0;
}
