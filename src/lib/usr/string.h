#ifndef GANJAX_STRING_H
#define GANJAX_STRING_H
short strlen(const char* __s);
int   strcmp(const char * s1, const char * s2);
char *strcat(char *dest, const char *src);
char *strchr(const char *s, int c);
char *strcpy(char *dest, const char* src);
short strspn(const char *s1, const char *s2);
char *strtok(char * str, const char * delim);
short strcspn(const char *s1, const char *s2);
#endif
