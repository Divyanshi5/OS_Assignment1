#include"command_mode.h"
#include"resetandcurrdir.h"
#include"openfile.h"

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
#include<vector>
#include<stack>

#define RESET "\e[m" 
#define GREEN "\e[32m" 
#define BLUE "\e[36m" 
using namespace std;

int nf,cursor;
stack<string> fhistory;
stack<string> bhistory;
string currpath;
vector<string> display_list(char** curr_dir)
{  
	string curr(*curr_dir);
	currpath=curr;
   	
 if( *curr_dir==NULL) 
      printf("\n ERROR : Could not get the working directory\n"); 
 DIR *dp = NULL;
 dp = opendir((const char*)*curr_dir); 	
 
 	vector<string> v;
 	nf=0;
 	reset_screen();
  struct dirent *sd;
  struct stat status;
  struct passwd *owner; 
  struct group *gp;
  int i;
  char* ch;
  		char* tcurr_dir = (char*)malloc(5000*sizeof(char));
  		strcpy(tcurr_dir,*curr_dir);
        strcat(tcurr_dir,"/");        
      while((sd=readdir(dp))!=NULL)
        {       	
          nf++; 
          char* temp= (char*)malloc(5000*sizeof(char));
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
                    string s1(tcurr_dir);                   
                    string s2(sd->d_name);
                    s1=s1+s2;                  
                     v.push_back(s1); 
                                 
                    free(temp);
                              
        }    
      free(tcurr_dir);   
      cursor=1;    
      goto_cursor(0,0);
      closedir(dp);
      return v;
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
    string path;
  char *curr_dir;
  curr_dir = curr_work_dir(); //calling current directory function
  std::vector<string> v;
  path=string(curr_dir);
  v=display_list(&curr_dir); //calling ls function

 

  char c='\0';
 
    while(1)
{

  c=getchar();
  if(c==':')
  {  
  	new_setting=initial_setting;
  	 new_setting.c_lflag &= ~ICANON;
  	 tcsetattr(0,TCSANOW,&new_setting);
      //cout<<"colon";
     string new_path=command_mode(currpath);  
     //break;   
     new_setting=initial_setting;
     new_setting.c_lflag &= ~ICANON;
     new_setting.c_lflag &= ~ECHO;
     tcsetattr(0,TCSANOW,&new_setting);
     goto_cursor(0,0);
     cursor=1;
     if(currpath==new_path)
     {

     }
     else
     {
     curr_dir=(char*)malloc(FILENAME_MAX*sizeof(char));
     curr_dir=(char*)new_path.c_str();
     v=display_list(&curr_dir);
  	}
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
  {
  	if(fhistory.empty())
  	{

  	}
  	else{
    bhistory.push(currpath);
   	 currpath=fhistory.top();
   	 fhistory.pop();
   	 curr_dir=(char*)malloc(FILENAME_MAX*sizeof(char));
   	 curr_dir=(char*)currpath.c_str();
   	 v=display_list(&curr_dir);
   	}
  }
  else if(c=='D')
   {
   	if(bhistory.empty())
   	{

   	}
   	else
   	{
   	 fhistory.push(currpath);
   	 currpath=bhistory.top();
   	 bhistory.pop();
   	 curr_dir=(char*)malloc(FILENAME_MAX*sizeof(char));
   	 curr_dir=(char*)currpath.c_str();
   	 v=display_list(&curr_dir);
   	}
   }
  else if(c=='\n')
  {
  	while(!fhistory.empty())
  	{
        fhistory.pop();
  	}
  	bhistory.push(currpath);
  	curr_dir=(char*)malloc(FILENAME_MAX*sizeof(char));
  	strcpy(curr_dir,v[cursor-1].c_str());
  	 path=string(curr_dir);
  	 currpath=path;
     DIR *dp = NULL;
 dp = opendir((const char*)curr_dir); 
 	
 if(dp==NULL) 
 {
 	open_file(&curr_dir);
 
 }
 else
 { 	 		  			
  	v=display_list(&curr_dir);
  }
  }
  else if(c==104)
  {
     curr_dir=(char*)malloc(FILENAME_MAX*sizeof(char));
     strcpy(curr_dir,curr_work_dir());
      path=string(curr_dir);
     v=display_list(&curr_dir);
  }
  else if(c==127)
  {
  	while(!fhistory.empty())
  		fhistory.pop();
  	string s1(curr_work_dir());
  	if(s1==path)
  	{

  	}
    else
    {
    curr_dir=(char*)malloc(FILENAME_MAX*sizeof(char));
    path=path+"/..";
     strcpy(curr_dir,path.c_str());
      path=string(curr_dir);
     v=display_list(&curr_dir);
 }
  }

}
tcsetattr(0,TCSANOW,&initial_setting);
  return 0;
}
