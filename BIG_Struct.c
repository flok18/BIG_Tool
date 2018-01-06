


#include "PshPack1.h"

typedef struct {
	char             Header[4];   //  "BIGF"
	unsigned int     Archive_Size;
	unsigned int     Number_Of_Files;
	unsigned int     First_File_Offset;
} BIG_HEADER;

typedef struct {
	unsigned int     Offset;
	unsigned int     Size;
	// char          Filename[X];
	// char          Null;
} BIG_FILE;

#include "PopPack.h"



void Fixup_UInt(unsigned int* TheInt)
{
	unsigned int  OldInt  = *TheInt;
	unsigned int  NewInt;
	
	NewInt  = (OldInt & 0x000000FF) << 24;
	NewInt |= (OldInt & 0x0000FF00) << 8;
	NewInt |= (OldInt & 0x00FF0000) >> 8;
	NewInt |= (OldInt & 0xFF000000) >> 24;

	*TheInt = NewInt;
}

void Fixup_Header(BIG_HEADER* TheHeader)
{
	Fixup_UInt(&TheHeader->Number_Of_Files);
	Fixup_UInt(&TheHeader->First_File_Offset);
}

void Fixup_File(BIG_FILE* TheFile)
{
	Fixup_UInt(&TheFile->Offset);
	Fixup_UInt(&TheFile->Size);
}




