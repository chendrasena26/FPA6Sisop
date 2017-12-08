#include <syscall.h>
#include <stdio.h>
#include <string.h>
#include <xv6/stdio.h>
#include <xv6/dirent.h>
#include <xv6/stat.h>
#include <xv6/fcntl.h>
#include <xv6/fs.h>
char *loc;

int checktype(char * namefile)
{
   int fd;
   struct stat setat;
   fd=open(namefile, 0_RDONLY);
   if(fd<0) return -1;
   if(fstat(fd, &setat)<0) {
	printf("stat failed. Program closed.\n");
	close(fd);
	sysexit();
   }
   close(fd);
   if(st.type==T_FILE) return 1;
   else if(st.type==T_DIR) return 0;
   else return -1;
}

int main(int argc, char *argv[])
{
   if(argc<=1) {
	printf("Usage: copy file to certain destination\n");
   }
   else if(!strcmp(argv[1], "-r") || !strcmp(argv[1], "-R")) {
   	if(checktype(argv[2])!=0 || checktype(argv[3])!=0) {
	   printf("syntax: cp -r/-R source_dir destination_dir \n");
	}
	else {
	   loc=argv[3];
	   tracker(argv[2], argv[3]);
	}
   }
   else if(!strcmp(argv[1], "*")) {
	if(checktype(argv[2])!=0) {
	   printf("syntax: cp * destination_dir\n");
	}
	else {	
	   loc=argv[2];
	   tracker(".",argv[2]);
	}
   }
   else {
	cp(argv[1],argv[2]);
   }
   sysexit();
}