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

int checktype(char *namefile)
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
   if(setat.type==T_FILE) return 1;
   else if(setat.type==T_DIR) return 2;
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

void tracker(char *source, char *dest)
{
   int fd;
   struct dirent back;
   char *point;
   char buffsour[500], buffdest[500];
   if((fd=open(source,0))<0)
   {
	printf("open failed.\n");
	sysexit();
   }
   while(read(fd, &back, sizeof(back))==sizeof(back)) {
	strcpy(buffsour, source);
	strcpy(buffdest, dest);
	if(!strcmp(back.d_name, ".") || !strcmp(back.d_name,"..")) continue;
	point=buffsour+strlen(buffsour);
	if(*point!='/') strcat(buffsour, "/");
	point=buffdest+strlen(buffdest);
	if(*point!='/') strcat(buffdest, "/");
	strcat(buffsour, back.d_name);
    	strcat(buffsour, "\0");
    	strcat(buffdest, back.d_name);
    	strcat(buffdest, "\0");
	if(!strcmp(loc, buffsour)) continue;
	
	if(checktype(buffsour)==1) cp(buffsour, buffdest);
	else if(checktype(buffsour)==2) {
	   mkdir(buffdest);
	   tracker(buffsour, buffdest);
	}
	else {
	   printf("error.\n");
	   sysexit();
	}
   }
   close(fd);
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
