#include <string.h>
short strlen(const char *__s){
    short i = 0;
    while(*__s){
        i++;
        __s++;
    }
    return i;
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

char *strcat(char *dest, const char *src)
{
    char *ret = dest;
    while (*dest)
        dest++;
    while (*dest++ = *src++);
    return ret;
}

char *strchr(const char *s, int c)
{
    while (*s != (char)c)
        if (!*s++)
            return 0;
    return (char *)s;
}

char *strcpy(char *dest, const char* src)
{
    char *ret = dest;
    while (*dest++ = *src++);
    return ret;
}

short strspn(const char *s1, const char *s2)
{
    short ret=0;
    while(*s1 && strchr(s2,*s1++))
        ret++;
    return ret;    
}

char *strtok(char * str, const char * delim)
{
    static char* p=0;
    if(str)
        p=str;
    else if(!p)
        return 0;
    str=p+strspn(p,delim);
    p=str+strcspn(str,delim);
    if(p==str)
        return p=0;
    p = *p ? *p=0,p+1 : 0;
    return str;
}

short strcspn(const char *s1, const char *s2)
{
    short ret=0;
    while(*s1)
        if(strchr(s2,*s1))
            return ret;
        else
            s1++,ret++;
    return ret;
}

char * itoa( int value, char * str, int base ){
    char * rc;
    char * ptr;
    char * low;
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    if ( value < 0 && base == 10 ){
        *ptr++ = '-';
    }
    low = ptr;
    do{
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    *ptr-- = '\0';
    while ( low < ptr ){
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

char* memset(char *__s, short __c, short __n){
	char* ret = __s;
	while(__n){
		*__s = __c;
		__s++;
		__n--;
	}
	return ret;
}

char* memcpy(char* __s, const char *__src, short __n){
	while(__n){
		*__s = *__src;
		__s++;
		__src++;
		__n--;
	}
	return __s;
}

char* errmesg(char* __errmesg, char errcode){
	static char int_buff[10];
	memset(int_buff, 0, 10);
	memset(errbuff, 0, 30);
	strcat(errbuff, __errmesg);
	itoa(errcode, int_buff, 16);
	strcat(errbuff, int_buff);
	return errbuff;
}

int strncmp(const char *s1, const char *s2, short n)
{
    for ( ; n > 0; s1++, s2++, --n)
	if (*s1 != *s2)
	    return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
	else if (*s1 == '\0')
	    return 0;
    return 0;
}

int   strcnt(const char *__s, char c){
	int cnt= 0;
	while(*__s){
		if(*__s == c) cnt++;
		__s++;
	}
	return cnt;
}

int  strpos(const char *__s, char c){
	int pos = 0;
	while(*__s){
		if(*__s == c) return pos;
		pos++;
		__s++;
	}
	return -1;
}
