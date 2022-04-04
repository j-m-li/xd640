
#include <stdlib.h>
#include <string.h>

inline int fl_ucs2utf(unsigned int ucs, char *buf)
{
        buf[0] = ucs & 0xFF;
        return 1;
}

inline int fl_utflen(const unsigned char *buf, int len)
{
	if (len < 1) return 0;
	return 1;
}

int fl_utf2ucs( const unsigned char *buf, int len, unsigned int *ucs)
{
	if (len < 1) return 0;
	*ucs = (unsigned int) buf[0];
	return 1;
}

#define fl_utf_strncasecmp strncasecmp


