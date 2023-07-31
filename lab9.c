#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int id;
	char name;
	int order;
};

// HashNode for separate chaining
struct HashNode
{
	struct RecordType record;
	struct HashNode* next;
};

// HashType
struct HashType
{
	struct HashNode* head;
};

// Compute the hash function
int hash(int x)
{
	return x % 31;
}

// Function to initialize the hash table
void initHash(struct HashType* pHashArray, int hashSz)
{
	int i;
	for (i = 0; i < hashSz; ++i)
		pHashArray[i].head = NULL;
}

// Function to insert a record into the hash table
void insertRecord(struct HashType* pHashArray, int hashSz, struct RecordType record)
{
	int index = hash(record.id);
	struct HashNode* newNode = (struct HashNode*)malloc(sizeof(struct HashNode));
	if (newNode == NULL)
	{
		printf("Memory allocation error\n");
		exit(-1);
	}
	newNode->record = record;
	newNode->next = pHashArray[index].head;
	pHashArray[index].head = newNode;
}

// Function to display records in the hash structure
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
	int i;
	for (i = 0; i < hashSz; ++i)
	{
		struct HashNode* current = pHashArray[i].head;
		if (current != NULL)
		{
			printf("index %d -> ", i);
			while (current != NULL)
			{
				printf("%d %c %d -> ", current->record.id, current->record.name, current->record.order);
				current = current->next;
			}
			printf("\n");
		}
	}
}

void freeHash(struct HashType* pHashArray, int hashSz)
{
	int i;
	for (i = 0; i < hashSz; ++i)
	{
		struct HashNode* current = pHashArray[i].head;
		while (current != NULL)
		{
			struct HashNode* temp = current;
			current = current->next;
			free(temp);
		}
		pHashArray[i].head = NULL;
	}
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType* pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

int main(void)
{
	struct RecordType* pRecords;
	int recordSz = 0;
	int hashSz = 31;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Initialize the hash table
	struct HashType* pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
	if (pHashArray == NULL)
	{
		printf("Memory allocation error\n");
		free(pRecords);
		exit(-1);
	}

	initHash(pHashArray, hashSz);

	// Insert records into the hash table
	int i;
	for (i = 0; i < recordSz; ++i)
	{
		insertRecord(pHashArray, hashSz, pRecords[i]);
	}

	// Display records in the hash table
	displayRecordsInHash(pHashArray, hashSz);

	// Free allocated memory
	freeHash(pHashArray, hashSz);
	free(pRecords);

	return 0;
}
