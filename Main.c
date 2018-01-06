

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <io.h>

#include "BIG_Struct.c"
#include "BIG_FileBuf.c"
#include "BIG_FolderBuf.c"
#include "BIG_IO_win32.c"
#include "BIG_List.c"
#include "BIG_Extract.c"
#include "BIG_Create.c"



int main(int argc, char** argv)
{
    char*            RunMode;

    if (argc < 3)
    {
        puts("\n  BIG_Tool.exe list    [BIG File]\n"
               "  BIG_Tool.exe create  [BIG File] [Folder]\n"
               "  BIG_Tool.exe extract [BIG File] [Folder]\n");
        return 0;
    }

    // Skip Exe-Name
    ++argv;
    --argc;

    // Get First Arg
    RunMode = *argv;
    ++argv;
    --argc;

	
	if (strcmp(RunMode, "list") == 0)
	{
		Start_List_Contend(argc, argv);
	}
	else if (strcmp(RunMode, "create") == 0)
	{
		Start_Create_Archive(argc, argv);
	}
	else if (strcmp(RunMode, "extract") == 0)
	{
		Start_Extract_Archive(argc, argv);
	}
	else
	{
		printf("Unknown Option: %s\n", RunMode);
	}

    return 0;
}


