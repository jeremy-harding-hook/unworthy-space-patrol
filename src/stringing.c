#include <stringing.h>

unsigned char stringlength(unsigned char value){
	if(value < 10)
		return 1;
	if(value < 100)
		return 2;
	return 3;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wchar-subscripts"
void chartostring(unsigned char value, char *string,
		unsigned char length){
	string[length] = '\0';
	do{
		string[--length] = (char)(value % 10)+ '0';
		value /= 10;
	}
	while(length > 0);
}
#pragma GCC diagnostic pop
