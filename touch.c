#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char* argv[])
{

	if(argc<2)
    {
        printf(1, "Syntax Error, touch FILENAME\n");
        exit();
    }

    int i, file;
    for(i = 1; i<argc; i++)
    {
        file = open(argv[i], O_CREATE | O_RDWR);
        if(file < 0) 
        {
            printf(2, "Failed to create file %s\n", argv[i]);
            exit();
        }
    }
    exit();
}
