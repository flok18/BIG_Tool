

#include <windows.h>



void BIG_Seek(HANDLE Big_File_Handle, size_t Offset)
{
	LARGE_INTEGER  NewPos;
	
#ifdef _WIN64
	NewPos.QuadPart   = (LONGLONG)Offset;
#else
	NewPos.u.LowPart  = (DWORD)Offset;
	NewPos.u.HighPart = 0;
#endif

	SetFilePointerEx(Big_File_Handle, NewPos, NULL, FILE_BEGIN);
}


unsigned int BIG_Write(HANDLE Big_File_Handle, void* Source_Buf, unsigned int Num_Bytes)
{
	BOOL   Res;
	DWORD  NumWritten;

	if (Big_File_Handle && Source_Buf && Num_Bytes)
	{
		Res = WriteFile(Big_File_Handle, Source_Buf, (DWORD)Num_Bytes, &NumWritten, NULL);
		if (Res)
		{
			return (unsigned int)NumWritten;
		}
	}
	return 0;
}

unsigned int BIG_Read(HANDLE Big_File_Handle, void* Target_Buf, unsigned int Num_Bytes)
{
	BOOL   Res;
	DWORD  NumRead;

	if (Big_File_Handle && Target_Buf && Num_Bytes)
	{
		Res = ReadFile(Big_File_Handle, Target_Buf, (DWORD)Num_Bytes, &NumRead, NULL);
		if (Res)
		{
			return (unsigned int)NumRead;
		}
	}
	return 0;
}


HANDLE BIG_Create(const char* Big_File_Path)
{
	HANDLE  Ret_Handle;

	Ret_Handle = CreateFile(Big_File_Path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	
	if (Ret_Handle == INVALID_HANDLE_VALUE) return NULL;
	return Ret_Handle;
}


HANDLE BIG_Open(const char* Big_File_Path)
{
	HANDLE  Ret_Handle;

	Ret_Handle = CreateFile(Big_File_Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	
	if (Ret_Handle == INVALID_HANDLE_VALUE) return NULL;
	return Ret_Handle;
}

void BIG_Close(HANDLE Big_File_Handle)
{
	if (Big_File_Handle)
	{
		CloseHandle(Big_File_Handle);
	}
}


BOOL DirExists(const char* dirName)
{
	DWORD attribs = GetFileAttributesA(dirName);

	if (attribs == INVALID_FILE_ATTRIBUTES)
	{
		return FALSE;
	}
	return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}




int Read_String_From_Big(HANDLE File_Handle, char* Buf, size_t Buflen)
{
	while(Buflen)
	{
		if (BIG_Read(File_Handle, Buf, 1))
		{
			if (*Buf == '\0') return 1;
		}
		else
		{
			return 0;
		}
		
		++Buf;
		--Buflen;
	}

	return 0;
}


int Write_String_To_Big(HANDLE File_Handle, char* Buf)
{
	unsigned int  LenOfBuf;
	
	LenOfBuf = (unsigned int)strlen(Buf) + 1;
	if (BIG_Write(File_Handle, Buf, LenOfBuf) == LenOfBuf)
	{
		return 1;
	}

	return 0;
}



