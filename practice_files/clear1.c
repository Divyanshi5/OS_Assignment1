#include<unistd.h>
#include<stdio.h>
 #include<stdlib.h> 
 #include <sys/types.h> 
 #include <dirent.h> 
 #include <sys/stat.h>  
  
 // Define the color codes as macros 
 #define RESET_COLOR "\e[m" 
 #define MAKE_GREEN "\e[32m" 
 #define MAKE_BLUE "\e[36m" 

 char* cwd(){
  char buff[FILENAME_MAX];
  return getcwd( buff, FILENAME_MAX );  
 
}
  
int main()
{
	while(1)
	{
   
	 printf("'\033c");
    char *curr_dir = NULL; 
    DIR *dp = NULL; 
    struct dirent *dptr = NULL; 
    unsigned int count = 0; 
    struct stat filestat;
      curr_dir = cwd(); 
      if(NULL == curr_dir) 
    { 
        printf("\n ERROR : Could not get the working directory\n"); 
        return -1; 
   }      
    dp = opendir((const char*)curr_dir);    
    if(NULL == dp) 
    { 
        printf("\n ERROR : Could not open the working directory\n"); 
        return -1; 
    } 
   
    for(count = 0; NULL != (dptr = readdir(dp)); count++) 
    { 
        if(dptr->d_name[0] != '.') 
        { 
            // Check if the file is executable 
            if(!access(dptr->d_name,X_OK)) 
            { 
                int fd = -1; 
                struct stat st; 
  
                fd = open(dptr->d_name, O_RDONLY, 0); 
                if(-1 == fd) 
                { 
                    printf("\n Opening file/Directory failed\n"); 
                    return -1; 
                } 
                 
                fstat(fd, &st); 
                // Check if it actully was a directory with execute 
                // permissions on it. 
                if(S_ISDIR(st.st_mode)) 
                { 
                    // If it was a directory, print it in Blue 
                    printf(MAKE_BLUE"%s\t%f\n"RESET_COLOR,dptr->d_name,sizeof(dptr)); 
                } 
                else 
                {                                   
                    // If it was a normal executable 
                    // Print it in green 
                    printf(MAKE_GREEN"%s\t%f\n"RESET_COLOR,dptr->d_name,sizeof(dptr)); 
                } 
                close(fd); 
            } 
            else 
            { 
                // No executable flag ON 
                // Print it in black(default) 
                printf("%s\t%f\n",dptr->d_name,sizeof(dptr)); 
            } 
        } 
    } 
    printf("\n"); 
  }
    return 0; 
 }
