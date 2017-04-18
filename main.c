#include <stdio.h>
#include <stdlib.h>

int main() 
{
    int menuInput;
    int mainMemorySize;
    int pageSize;
    int repPolicy;
    int virtualAddress;
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
            printf("\n");
        }
        else if(menuInput==2)
        {
            virtualAddressInput:
            virtualAddress = takeInput("Enter virtual memory address to access: ");
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

