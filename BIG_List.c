





void Print_File_Data(HANDLE File_Handle)
{
	BIG_FILE   FileHdr;
	char       FileNameBuf[512];
	
	if (BIG_Read(File_Handle, &FileHdr, sizeof(BIG_FILE)) != sizeof(BIG_FILE))
	{
		puts("Error: Unable to Read File Header from BIG...");
		return;
	}
	
	Fixup_File(&FileHdr);
	
	if (Read_String_From_Big(File_Handle, FileNameBuf, sizeof(FileNameBuf)) == 0)
	{
		puts("Error: Unable to Read String from BIG...");
		return;	
	}

	printf("\n BIG_FILE.Offset   = %i (0x%X)\n", FileHdr.Offset, FileHdr.Offset);
	printf(  " BIG_FILE.Size     = %i\n", FileHdr.Size);
	printf(  " BIG_FILE.Name     = %s\n", FileNameBuf);
}



void Do_List_Contend(HANDLE File_Handle)
{
	BIG_HEADER           BigHdr;
	unsigned int         ii;
	
	if (BIG_Read(File_Handle, &BigHdr, sizeof(BIG_HEADER)) != sizeof(BIG_HEADER))
	{
		puts("Error: Unable to Read Header from BIG-File...");
		return;
	}
	
	Fixup_Header(&BigHdr);
	
	if (memcmp(BigHdr.Header, "BIGF", 4) != 0)
	{
		puts("Error: Incorrect BIG File Header...");
		return;
	}
	
	printf(" BIG_HEADER.Archive_Size       = %i\n", BigHdr.Archive_Size);
	printf(" BIG_HEADER.Number_Of_Files    = %i\n", BigHdr.Number_Of_Files);
	printf(" BIG_HEADER.First_File_Offset  = %i (0x%X)\n", BigHdr.First_File_Offset, BigHdr.First_File_Offset);
	
	for (ii = 0; ii < BigHdr.Number_Of_Files; ++ii)
	{
		Print_File_Data(File_Handle);
	}
}



void Start_List_Contend(int argc, char** argv)
{
	HANDLE  Big_File_Handle  = NULL;
	
	if (argc < 1)
	{
		puts("Error: Missing Arguments...");
		return;
	}
	
	Big_File_Handle = BIG_Open(argv[0]);
	if (Big_File_Handle == NULL)
	{
		printf("Error: Unable to open File: %s\n", argv[0]);
		return;
	}

	Do_List_Contend(Big_File_Handle);
	BIG_Close(Big_File_Handle);
}


