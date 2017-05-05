#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct pageEntry
{
    int virtualPage;
    int policyCounter;
};
typedef struct pageEntry pageEntry;
pageEntry* updateCounters(pageEntry* arr, int size, int newestIndex);
int main() 
{
    int menuInput;
    int mainMemorySize=-1;
    int pageSize;
    int repPolicy;
    int virtualAddress;
    int numPageFrames;
    pageEntry* pageEntries;
    int offset;
    int currPage = -1;
    while(1)
    {
        printf("Virtual memory to Main memory mapping:\n");
        printf("--------------------------------------\n");
        printf("1) Enter parameters\n");
        printf("2) Map virtual address\n");
        printf("3) Quit\n");
       
        menuInput=takeInput("Enter selection: ");
        
        if(menuInput==1)
        {
            if(mainMemorySize!=-1)
            {
                printf("Error: Cannot enter parameters again. Please restart program in order to do so.\n");
                continue;
            }
            memorySizeInput:
            mainMemorySize=takeInput("Enter main memory size (words): ");
            if(isBaseTwo(mainMemorySize)==0)
            {
                printf("Error: Main memory size must be a base two number.\n");
                goto memorySizeInput;
            }
            pageSizeInput:
            pageSize=takeInput("Enter page size (words/page): ");
            if(isBaseTwo(pageSize)==0)
            {
                printf("Error: Page size must be a base two number\n");
                goto pageSizeInput;
            }
            repPolicyInput:
            repPolicy=takeInput("Enter replacement policy (0=LRU, 1=FIFO): ");
            if(repPolicy!=1 && repPolicy!=0)
            {
                printf("Error: Only valid inputs are 1 and 0\n");
                goto repPolicyInput;
            }
            numPageFrames = mainMemorySize/pageSize;
            pageEntries = malloc(sizeof(pageEntry)*numPageFrames);
            initializePageEntries(pageEntries,numPageFrames);
            printf("\n");
        }
        
        
        else if(menuInput==2)
        {
            if(mainMemorySize == -1)
            {
                printf("Error: Must enter parameters before mapping virtual address.\n");
                continue;
            }
            virtualAddressInput:
            virtualAddress = takeInput("Enter virtual memory address to access: "); //any positive integer, as well as 0, is valid
            int virtualPage = virtualAddress/pageSize;
            if((currPage = containsPage(pageEntries,numPageFrames,virtualPage)) != -1)
            {
                //don't need to hit up any arrays for modification in this case, just output stuff
                offset = virtualAddress % pageSize;
                int physicalAddress = (pageSize*currPage) + offset;
                printf("Virtual address %i maps to physical address %i\n", virtualAddress,physicalAddress);
                if(repPolicy == 0) // in the case of LRU policy, must update counters in case of non page-fault
                {
                    pageEntries = updateCounters(pageEntries, numPageFrames, currPage);
                }
            }
            else
            {
                if(isFull(pageEntries, numPageFrames))
                {
                    //Page fault. handle replacement policy here
                    printf("Page Fault!\n");
                    currPage = getIndexToReplace(pageEntries, numPageFrames);
                    pageEntries[currPage].virtualPage = virtualPage;
                }
                else
                {
                    //Page Fault. Simply insert here
                    printf("Page Fault!\n");
                    currPage = insertPage(pageEntries,numPageFrames,virtualPage); 
                }
                pageEntries = updateCounters(pageEntries, numPageFrames, currPage);
            }
            //call method to print table after the insertion, here
            displayTable(pageEntries, numPageFrames);
            currPage = -1;
        }
        else if(menuInput==3)
        {
            break;
        }
        else
        {
            printf("\nError: Invalid menu input. Only input numbers from 1-3.\n\n");
        }
    }
}
int isValidInt(char str[])
{
    char* c = str;
    while(*c)
    {
        if((isdigit(*c) == 0))
        {
            printf("\nError: Ensure that no extraneous characters or non-positive integers were entered and try again\n\n");
            return -1;
        }
        c++;
    }
    int num = atoi(str);
    return num;
}
int takeInput(char message[256])
{
    int input;
    char str[256];
    do
    {
        printf("%s",message);
        scanf("%s",&str);           
    }
    while((input = isValidInt(str)) == -1);
    return input;
}
int isBaseTwo(int num)
{
    if((double)(log10(num)/log10(2)) - (int)(log10(num)/log10(2)) == 0)
    {
        return 1;
    }
    return 0;
}
int isFull(pageEntry* arr, int size)
{
    for(int i = 0;i<size;i++)
    {
        if(arr[i].virtualPage==-1)
        {
            return 0;
        }
    }
    return 1;
}
int containsPage(pageEntry* arr, int size, int page)
{
    for(int i = 0;i<size;i++)
    {
        if(arr[i].virtualPage == page)
        {
            return i;
        }
    }
    return -1;
}
int insertPage(pageEntry* arr, int size, int page)
{
    for(int i = 0;i<size;i++)
    {
        if(arr[i].virtualPage == -1)
        {
            arr[i].virtualPage = page;
            return i;
        }
    }
    return -1;
}
void displayTable(pageEntry* pageEntries, int size)
{
    printf("-----------------\n| VP    | PF    |\n-----------------\n");
    for(int i = 0;i<size;i++)
    {
        if(pageEntries[i].virtualPage!=-1)
        {
            printf("| %i     | %i     |\n", pageEntries[i].virtualPage, i);
        }
    }
    printf("-----------------\n");
}
void initializePageEntries(pageEntry* pageEntries, int size)
{
    for(int i = 0;i<size;i++)
    {
        pageEntries[i].virtualPage=-1;
        pageEntries[i].policyCounter = -1;
    }
}
pageEntry* updateCounters(pageEntry* arr, int size, int newestIndex) //updates counters assuming newestIndex is the one that was updated (in the case of LRU) or inserted (LRU as well as FIFO)
{
    //in case of LRU, the entry with the highest counter is the first to go
    //In case of FIFO, the entry with the lowest counter is the first to go
    int counter = arr[newestIndex].policyCounter;
    if(counter == -1)
    {
        arr[newestIndex].policyCounter = 0;
        for(int i = 0;i<size;i++)
        {
            if(arr[i].policyCounter!=-1 && i!=newestIndex)
            {
                arr[i].policyCounter = arr[i].policyCounter+1;
            }
        }
        return arr;
    }
        
    for(int i = 0;i<size;i++)
    {
        if(arr[i].policyCounter!=-1 && i!=newestIndex && arr[i].policyCounter<=counter)
        {
            arr[i].policyCounter = arr[i].policyCounter+1;
        }
    }
    arr[newestIndex].policyCounter = 0;
    return arr;
}
int getIndexToReplace(pageEntry* arr, int size)
{
    int index=0;
    for(int i = 0;i<size;i++)
    {
        if(arr[i].policyCounter > arr[index].policyCounter)
        {
            index = i;
        }
    }
    return index;
}

//for testing

void displayTestingTable(pageEntry* arr, int size, int pageSize)
{
    for(int i = 0;i<size;i++)
    {
        printf("VP: %i  PF: %i  Ctr: %i\n",arr[i].virtualPage, i, arr[i].policyCounter);
    }
}



