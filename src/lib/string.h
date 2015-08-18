#ifndef GANJAX_STRING_H
#define GANJAX_STRING_H
static char errbuff[30];
short strlen(const char* __s);
int   strcmp(const char * s1, const char * s2);
char *strcat(char *dest, const char *src);
char *strchr(const char *s, int c);
char *strcpy(char *dest, const char* src);
short strspn(const char *s1, const char *s2);
char *strtok(char * str, const char * delim);
short strcspn(const char *s1, const char *s2);
char* itoa(int value, char* str, int base);
char* memset(char* __s, short __c, short __n);
char* memcpy(char* __s, const char *__src, short __n);
char* errmesg(char* __errmesg, char errcode);
int   strncmp(const char *s1, const char *s2, short n);

#endif
