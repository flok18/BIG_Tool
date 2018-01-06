

#define  _FileBuf_Max_Path_Size   256

typedef struct {
	unsigned int     Offset;
	unsigned int     Size;
	char             FilePath[_FileBuf_Max_Path_Size];
	size_t           FilePathLen;
} Struct_FileBuf;


static Struct_FileBuf*   aFileBuf   = NULL;
static int               nFileBuf   = 0;
static int               mFileBuf   = 0;



void FileBuf_Release(void)
{
	if (aFileBuf)
	{
		free(aFileBuf);
		aFileBuf = NULL;
	}
	
	nFileBuf = 0;
	mFileBuf = 0;
}


Struct_FileBuf* FileBuf_Add(unsigned int New_Offset, unsigned int New_Size, const char* New_FilePath)
{
	Struct_FileBuf*  NewPtr;
	
	++nFileBuf;
	if (nFileBuf > mFileBuf)
	{
		mFileBuf += 64;
		
		if (aFileBuf)
		{
			aFileBuf = (Struct_FileBuf*)realloc(aFileBuf, mFileBuf * sizeof(Struct_FileBuf));
		}
		else
		{
			aFileBuf = (Struct_FileBuf*)malloc(mFileBuf * sizeof(Struct_FileBuf));		
		}
	}
	NewPtr = aFileBuf + (nFileBuf - 1);

	NewPtr->Offset = New_Offset;
	NewPtr->Size   = New_Size;
	strcpy_s(NewPtr->FilePath, _FileBuf_Max_Path_Size, New_FilePath);
	NewPtr->FilePathLen = strlen(NewPtr->FilePath);
	
	return NewPtr;
}

Struct_FileBuf* FileBuf_Get(int Index)
{
	if (Index < 0) return NULL;
	if (Index < nFileBuf)
	{
		return aFileBuf + Index;
	}
	return NULL;
}




