/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <base/system.h>
#include <engine/shared/datafile.h>
#include <engine/storage.h>

int main(int argc, const char **argv)
{
	tw::CCmdlineFix CmdlineFix(&argc, &argv);

	IStorage *pStorage = CreateStorage(IStorage::STORAGETYPE_BASIC, argc, argv);
	if(!pStorage || argc != 3)
		return -1;

	CDataFileReader Reader;
	if(!Reader.Open(pStorage, argv[1], IStorage::TYPE_ABSOLUTE))
		return -1;

	CDataFileWriter Writer;
	if(!Writer.Open(pStorage, argv[2]))
		return -1;

	// add all items
	for(int Index = 0; Index < Reader.NumItems(); Index++)
	{
		int Type, ID;
		void *pPtr = Reader.GetItem(Index, &Type, &ID);

		// filter ITEMTYPE_EX items, they will be automatically added again
		if(Type == ITEMTYPE_EX)
			continue;

		int Size = Reader.GetItemSize(Index);
		Writer.AddItem(Type, ID, Size, pPtr);
	}

	// add all data
	for(int Index = 0; Index < Reader.NumData(); Index++)
	{
		void *pPtr = Reader.GetData(Index);
		int Size = Reader.GetDataSize(Index);
		Writer.AddData(Size, pPtr);
	}

	Reader.Close();
	Writer.Finish();
	return 0;
}
