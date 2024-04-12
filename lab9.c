#include <stdio.h>
#include <stdlib.h>
#define SIZE 23

// RecordType
struct RecordType
{
  int		id;
  char	name;
  int		order;
};

// Fill out this structure
struct HashType
{
  struct HashNode** array;
  int size;
};

struct HashNode
{
    struct RecordType record;
    struct HashNode* next;
};

// Compute the hash function
int hash(int x)
{
  return x % SIZE;
}


// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
  FILE* inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  int i, n;
  char c;
  struct RecordType *pRecord;
  *ppData = NULL;

  if (inFile)
  {
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
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

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
  int i;

  for (i=0;i<hashSz;++i)
  {
    printf("Index %d -> ", i);
    struct HashNode* current = pHashArray->array[i];
    while (current != NULL)
    {
        printf("%d, %c, %d -> ", current->record.id, current->record.name, current->record.order);
        current = current->next;
    }
    printf("\n");
  }
  printf("\n\n");
}

int main(void)
{
  struct RecordType *pRecords;
  int recordSz = 0;

  recordSz = parseData("input.txt", &pRecords);
  printRecords(pRecords, recordSz);
  int hashSize = SIZE;
  // Initialize hash table
  struct HashType hashTable;
  hashTable.size = hashSize;

  hashTable.array = (struct HashNode**) malloc(sizeof(struct HashNode*) * hashSize);

  for (int i = 0; i < hashSize; ++i)
    {
      hashTable.array[i] = NULL;
    }

   for (int i = 0; i < recordSz; ++i)
  {
      int index = hash(pRecords[i].id);
    struct HashNode* newNode = (struct HashNode*)malloc(sizeof(struct HashNode));
    newNode->record = pRecords[i];
    newNode->next = NULL;

    if (hashTable.array[index] == NULL)
    {
        hashTable.array[index] = newNode;
    }
    else
    {
        struct HashNode* current = hashTable.array[index];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
  }


  displayRecordsInHash(&hashTable, hashSize);

  for (int i = 0; i < hashSize; ++i)
  {
      struct HashNode* current = hashTable.array[i];
      while (current != NULL)
      {
          struct HashNode* temp = current;
          current = current->next;
          free(temp);
      }
  }
  free(hashTable.array);
  free(pRecords);
  return 0;
}
