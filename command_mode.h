//#include"resetandcurrdir.h"
#include"copy.h"

#include<stdio.h>
#include<iostream>
#include<termios.h>
#include<vector>
using namespace std;
void command_mode()
{
	
	//cout<<"\t\t\t\t\t\t\tCOMMAND MODE\n";
    
cout<<"\33[33;1H";	
char path[1000];

 while(1)
 { 
    
    int i=0;
    char c;
  vector<string> cmds;
  c=getchar();
  if(c==27)
   // cout<<"hii";
    //return;
  while(c!=10)
  {
     path[i++]=c;
     c=getchar();    
  }
  path[i]='\0';
   int l=0;
         char* token = strtok(path, " ");
          while (token != NULL) 
          {
        cmds.push_back(token);
        l++;
        token = strtok(NULL, " ");
      }
 
    
     if(cmds[0]=="copy")
    {
        for(int i=1;i<l-1;i++)
        {                      
          copy_file(cmds[i],cmds[l-1]);
        }
    }
    else if(cmds[0]=="create_file")
    {
        create_file(cmds[1],cmds[2]);
    }
    else if(cmds[0]=="create_dir")
    {
        create_dir(cmds[1],cmds[2]);
    }
    else if(cmds[0]=="rename")
    {
        rename(cmds[1],cmds[2]);
    }
    else if(cmds[0]=="delete_file")
    {
       delete_file(cmds[1]);
    }
 	
 }
}
