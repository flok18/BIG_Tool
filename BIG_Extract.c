

static char     Output_Dir[MAX_PATH]  = {'\0'};
static size_t   Output_Dir_Len        = 0;




void Set_Output_Directory(const char* ODir)
{
	char*  PosInStr;

	strcpy_s(Output_Dir, sizeof(Output_Dir), ODir);
	Output_Dir_Len = strlen(Output_Dir);

	if (Output_Dir_Len)
	{
		PosInStr = Output_Dir + (Output_Dir_Len - 1);
	
		if (*PosInStr != '\\')
		{
			++PosInStr;
			*PosInStr = '\\';
			++PosInStr;
			*PosInStr = '\0';
			
			++Output_Dir_Len;
		}
	}
}


const char* Build_Folder_Structure(const char* RelFilePath)
{
	char*    FoundPos;
	size_t   Cur_Len;
	size_t   Num_Char;
	

	// Reset
	Cur_Len = Output_Dir_Len;
	Output_Dir[Output_Dir_Len] = '\0';
	
	
	while ((FoundPos = strchr(RelFilePath, '\\')) != NULL)
	{
		Num_Char = FoundPos - RelFilePath;
		if (Num_Char)
		{
			if ((Cur_Len + Num_Char + 2) > sizeof(Output_Dir))
			{
				puts("ERROR: Buffer Overflow...");
				return NULL;
			}
		
			memcpy(Output_Dir + Cur_Len, RelFilePath, Num_Char);
			Cur_Len += Num_Char;
			Output_Dir[Cur_Len] = '\0';
			
			if (!DirExists(Output_Dir))
			{
				if (_mkdir(Output_Dir) < 0)
				{
					printf("ERROR: Unable to create Dir: %s\n", Output_Dir);
					return NULL;
				}
			}
			
			Output_Dir[Cur_Len] = '\\';
			++Cur_Len;
			Output_Dir[Cur_Len] = '\0';	
		}
		
		++Num_Char;
		RelFilePath += Num_Char;
	}
	
	strcat_s(Output_Dir, sizeof(Output_Dir), RelFilePath);

	return Output_Dir;
}









void Extract_File(HANDLE File_Handle, Struct_FileBuf* FileData)
{
	const char*  Output_File_Name  = NULL;
	HANDLE       Output_Handle     = NULL;
	void*        TempBuffer        = NULL;
	
	
	if (FileData == NULL) return;

	Output_File_Name = Build_Folder_Structure(FileData->FilePath);
	if (Output_File_Name == NULL) return;
	
	Output_Handle = BIG_Create(Output_File_Name);
	if (Output_Handle == NULL)
	{
		printf("Error: Unable to create File: %s\n", Output_File_Name);
		return;
	}

	TempBuffer = malloc(FileData->Size);
	if (TempBuffer == NULL)
	{
		puts("ERROR: Not enough Memory...");
		BIG_Close(Output_Handle);
		return;
	}
	
	puts(Output_File_Name);
	
	BIG_Seek(File_Handle, (size_t)FileData->Offset);
	BIG_Read(File_Handle, TempBuffer, FileData->Size);
	
	BIG_Write(Output_Handle, TempBuffer, FileData->Size);
	BIG_Close(Output_Handle);
	
	free(TempBuffer);
}




void Store_File_Data(HANDLE File_Handle)
{
	BIG_FILE   FileHdr;
	char       FileNameBuf[256];
	
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

	FileBuf_Add(FileHdr.Offset, FileHdr.Size, FileNameBuf);
}



void Do_Extract_Archive(HANDLE File_Handle)
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

	for (ii = 0; ii < BigHdr.Number_Of_Files; ++ii)
	{
		Store_File_Data(File_Handle);
	}

	for (ii = 0; ii < BigHdr.Number_Of_Files; ++ii)
	{
		Extract_File(File_Handle, FileBuf_Get(ii));
	}
	
	FileBuf_Release();
}






void Start_Extract_Archive(int argc, char** argv)
{
	HANDLE  Big_File_Handle  = NULL;
	
	if (argc < 2)
	{
		puts("Error: Missing Arguments...");
		return;
	}

	
	if (!DirExists(argv[1]))
	{
		printf("Error: Directory not Exist: %s\n", argv[1]);
		return;
	}
	Set_Output_Directory(argv[1]);

	
	Big_File_Handle = BIG_Open(argv[0]);
	if (Big_File_Handle == NULL)
	{
		printf("Error: Unable to open File: %s\n", argv[0]);
		return;
	}

	Do_Extract_Archive(Big_File_Handle);

	BIG_Close(Big_File_Handle);
}



