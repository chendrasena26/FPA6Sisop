#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, int *argv[])
{
  if(argc<2) {
	printf(2,"tak ada nama file..\n");
	exit();
  }
  int i;
  for(i=1;i<argc;i++) {
	stat=unlink(argv[i]);
	if(stat!=0) {
	  printf(2, "gagal menghapus %s\n",argv[i]);
	  exit();
	  }
	else exit();
}
