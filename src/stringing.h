#ifndef STRINGING_INCLUDED
#define STRINGING_INCLUDED

/** Gets the length of a string representation of a chare treated as a number.
 *  The length returned does not include the null character that would be needed
 *  to terminate the string. In other words, this just returns the number of
 *  digits of the char.*/
unsigned char stringlength(unsigned char value);

/** Converts an unsigned  char to a string representation of the value. Note
 *  that the length parameter does not include the null terminating character,
 *  it's just the number of digits. */
void chartostring(unsigned char value, char *string,
		unsigned char length);

#endif
