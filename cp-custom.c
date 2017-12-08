#include <syscall.h>
#include <stdio.h>
#include <string.h>
#include <xv6/stdio.h>
#include <xv6/dirent.h>
#include <xv6/stat.h>
#include <xv6/fcntl.h>
#include <xv6/fs.h>
char *loc;
char buff[500];

int checktype(char * namefile)
{
   int fd;
   struct stat setat;
   fd=open(namefile, 0);
   if(fd<0) return -1;
   if(fstat(fd, &setat)<0) {
	printf("stat failed. Program closed.\n");
	close(fd);
	sysexit();
   }
   close(fd);
   if(st.type==T_FILE) return 1;
   else if(st.type==T_DIR) return 2;
   else return -1;
}

void cp(char *sour, char *dest)
{
   int i,source, destination,flag=0;
   if(checktype(sour)==2) {
	printf("%s is a directory\n");
	sysexit();
   }
   else if(checktype(dest)==2) {
	char *check=dest+strlen(dest);
	if(*check!='/') strcat(dest,"/");
	strcat(dest, sour);
   }
   if((source=open(sour, 0))<0) {
	printf("opening %s failed. Closing program\n",source);
	flag++;
   }
   else if((destination=open(dest, O_CREAT | O_RDWR))<0) {
	printf("opening %s failed. Closing program\n",destination);
	flag++;
   }
   else {
	while((i=read(source, buff, sizeof(buff)))>0) {
	   if(i<0) {
		printf("Read error, closing program\n");
		flag++;
		break;
	   }
	   if(write(destination, buff, i)!=i) {
		printf("write error. closing program\n");
		flag++;		
		break;
	   }
	}
   }
   close(source);
   close(destination);
   if(flag!=0) sysexit();
}

int main(int argc, char *argv[])
{
   if(argc<=1) {
	printf("Usage: copy file to certain destination\n");
   }
   else if(!strcmp(argv[1], "*")) {
	if(checktype(argv[2])!=2) {
	   printf("not correct. syntax: cp * destination_dir\n");
	}
	else {	
	   loc=argv[2];
	   tracker(".",argv[2]);
	}
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
   else {
	cp(argv[1],argv[2]);
   }
   sysexit();
}
