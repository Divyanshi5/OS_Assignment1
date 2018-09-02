#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<iostream>
using namespace std;
	void copy_file(string source,string destination)
	{
		
		int in,out;
		char c[20];
	in=open(source.c_str(),O_RDONLY);
	out=open(destination.c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
	while(read(in,&c,1)==1)
		write(out,&c,1);
    }
    void create_file(string filename,string destination)
    {
    	string file=destination+"/"+filename;
    	creat(file.c_str(), S_IRWXU  | S_IRWXG | S_IRWXO);
    }
     void create_dir(string dirname,string destination)
    {
    	string dir=destination+"/"+dirname;
        mkdir(dir.c_str(), S_IRWXU  | S_IRWXG | S_IRWXO);
    }
    void rename(string old,string newname)
    {
    	rename(old.c_str(), newname.c_str());
    }
    void delete_file(string path)
    {
    	remove(path.c_str());
    }