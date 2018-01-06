






void FillOut_FileData(unsigned int* Ret_Total_Size, unsigned int* Ret_Index_Size)
{
	int              ii;
	size_t           IndexSize    = 0;
	size_t           FileOffset   = 0;
	Struct_FileBuf*  AkFile;
	
	
	for (ii = 0; ii < nFileBuf; ++ii)
	{
		AkFile = FileBuf_Get(ii);
		
		IndexSize   += (sizeof(BIG_FILE) + 1 + AkFile->FilePathLen);
	}

	IndexSize  += sizeof(BIG_HEADER);
	FileOffset  = IndexSize;

	
	for (ii = 0; ii < nFileBuf; ++ii)
	{
		AkFile = FileBuf_Get(ii);
		
		AkFile->Offset = (unsigned int)FileOffset;
		
		FileOffset += AkFile->Size;		
	}	
	
	
	if (Ret_Index_Size)
	{
		*Ret_Index_Size = (unsigned int)IndexSize;
	}
	if (Ret_Total_Size)
	{
		*Ret_Total_Size = (unsigned int)FileOffset;
	}
}






void Do_Create_Archive(HANDLE File_Handle)
{
	BIG_HEADER         Big_Hdr;
	BIG_FILE           Big_Fle;
	int                ii;
	Struct_FileBuf*    AkFile;
	void*              TempBuf;
	HANDLE             Input_Handle    = NULL;
	

	memcpy(Big_Hdr.Header, "BIGF", 4);
	Big_Hdr.Number_Of_Files = nFileBuf;
	FillOut_FileData(&Big_Hdr.Archive_Size, &Big_Hdr.First_File_Offset);

	Fixup_Header(&Big_Hdr);
	BIG_Write(File_Handle, &Big_Hdr, sizeof(BIG_HEADER));


	for (ii = 0; ii < nFileBuf; ++ii)
	{
		AkFile = FileBuf_Get(ii);

		Big_Fle.Offset = AkFile->Offset;
		Big_Fle.Size   = AkFile->Size;
		
		Fixup_File(&Big_Fle);
		
		BIG_Write(File_Handle, &Big_Fle, sizeof(BIG_FILE));
		Write_String_To_Big(File_Handle, AkFile->FilePath);		
	}	
	
	for (ii = 0; ii < nFileBuf; ++ii)
	{
		AkFile = FileBuf_Get(ii);

		// Build Full FilePath
		strcpy_s(Output_Dir + Output_Dir_Len, sizeof(Output_Dir) - Output_Dir_Len, AkFile->FilePath);
		
		puts(Output_Dir);
		
		Input_Handle = BIG_Open(Output_Dir);
		if (Input_Handle == NULL)
		{
			printf("Error: Unable to open File: %s\n", Output_Dir);
			break;
		}
			
		TempBuf = malloc(AkFile->Size);
		if (TempBuf == NULL)
		{
			puts("ERROR: Not enough Memory...");
			BIG_Close(Input_Handle);
			break;
		}
		
		BIG_Read(Input_Handle, TempBuf, AkFile->Size);
		BIG_Write(File_Handle, TempBuf, AkFile->Size);
		
		BIG_Close(Input_Handle);
		free(TempBuf);
	}	
}





void Found_File_Handler(Struct_FolderBuf* InFolder, struct _finddata_t* FoundData)
{
	char    FullBuf[512];
	size_t  Len_Of_Main_Dir;
	
	// Complete The path
	memcpy_s(FullBuf, sizeof(FullBuf), InFolder->FolderPath, InFolder->FolderPathLen);
	strcpy_s(FullBuf + InFolder->FolderPathLen, sizeof(FullBuf) - InFolder->FolderPathLen, FoundData->name);
	
	if (FoundData->attrib & _A_SUBDIR)
	{
		// Ignore Pseodo Folders
		if (FoundData->name[0] == '.')
		{
			if (FoundData->name[1] == '\0') return;
			if (FoundData->name[1] == '.')
			{
				if (FoundData->name[2] == '\0') return;
			}
		}
			
		FolderBuf_Add(FullBuf, NULL, "*");
	}
	else
	{
		// Subtract The Main Folder Len
		Len_Of_Main_Dir = FolderBuf_Get(0)->FolderPathLen;
		
		FileBuf_Add(0, FoundData->size, FullBuf + Len_Of_Main_Dir);
	}
}



void Do_Create_FileList(const char* MainFolder)
{
	int                   ii            = 0;
	Struct_FolderBuf*     AkFolder;
	intptr_t              FindHandle;
	struct _finddata_t    FileData;

	// Add First Folder
	FolderBuf_Add(MainFolder, NULL, "*");


	while (ii < nFolderBuf)
	{
		AkFolder = FolderBuf_Get(ii);
		if (AkFolder == NULL) break;
		
		FindHandle = _findfirst(AkFolder->FolderPath, &FileData);
		if (FindHandle < 0) break;
		
		Found_File_Handler(AkFolder, &FileData);
		
		while (_findnext(FindHandle, &FileData) == 0)
		{
			Found_File_Handler(AkFolder, &FileData);
		}
	
       _findclose( FindHandle );

		++ii;
	}

	FolderBuf_Release();
}




void Start_Create_Archive(int argc, char** argv)
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

	
	Big_File_Handle = BIG_Create(argv[0]);
	if (Big_File_Handle == NULL)
	{
		printf("Error: Unable to open File: %s\n", argv[0]);
		return;
	}

	
	Do_Create_FileList(argv[1]);
	Set_Output_Directory(argv[1]);
	Do_Create_Archive(Big_File_Handle);

	BIG_Close(Big_File_Handle);
	FileBuf_Release();
}


