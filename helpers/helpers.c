#include "helpers.h"

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}
char* strcat(char *dest, const char *src)
{
    size_t i,j;
    for (i = 0; dest[i] != '\0'; i++)
        ;
    for (j = 0; src[j] != '\0'; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
}
char* strcpy(char *strDest, const char *strSrc)
{
    char *temp = strDest;
    while(*strDest++ = *strSrc++); // or while((*strDest++=*strSrc++) != '\0');
    return temp;
}
