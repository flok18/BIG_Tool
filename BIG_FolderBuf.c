

#define  _FolderBuf_Max_Path_Size   256

typedef struct {
	char             FolderPath[_FolderBuf_Max_Path_Size];
	size_t           FolderPathLen;
} Struct_FolderBuf;


static Struct_FolderBuf*   aFolderBuf   = NULL;
static int                 nFolderBuf   = 0;
static int                 mFolderBuf   = 0;



void FolderBuf_Release(void)
{
	if (aFolderBuf)
	{
		free(aFolderBuf);
		aFolderBuf = NULL;
	}
	
	nFolderBuf = 0;
	mFolderBuf = 0;
}


Struct_FolderBuf* FolderBuf_Add(const char* New_FolderPath, const char* Additional_01, const char* Additional_02)
{
	Struct_FolderBuf*  NewPtr;
	char*              PosInStr;
	
	++nFolderBuf;
	if (nFolderBuf > mFolderBuf)
	{
		mFolderBuf += 32;
		
		if (aFolderBuf)
		{
			aFolderBuf = (Struct_FolderBuf*)realloc(aFolderBuf, mFolderBuf * sizeof(Struct_FolderBuf));
		}
		else
		{
			aFolderBuf = (Struct_FolderBuf*)malloc(mFolderBuf * sizeof(Struct_FolderBuf));		
		}
	}
	NewPtr = aFolderBuf + (nFolderBuf - 1);

	
	strcpy_s(NewPtr->FolderPath, _FolderBuf_Max_Path_Size, New_FolderPath);
	NewPtr->FolderPathLen = strlen(NewPtr->FolderPath);

	if (NewPtr->FolderPathLen)
	{
		PosInStr = NewPtr->FolderPath + (NewPtr->FolderPathLen - 1);
	
		if (*PosInStr != '\\')
		{
			++PosInStr;
			*PosInStr = '\\';
			++PosInStr;
			*PosInStr = '\0';
			
			++(NewPtr->FolderPathLen);
		}
	}
	
	if (Additional_01) strcat_s(NewPtr->FolderPath, _FolderBuf_Max_Path_Size, Additional_01);
	if (Additional_02) strcat_s(NewPtr->FolderPath, _FolderBuf_Max_Path_Size, Additional_02);
	
	return NewPtr;
}


Struct_FolderBuf* FolderBuf_Get(int Index)
{
	if (Index < 0) return NULL;
	if (Index < nFolderBuf)
	{
		return aFolderBuf + Index;
	}
	return NULL;
}




