#ifndef __ULWATHLP__
#define __ULWATHLP__
unsigned int get_DS(void);

#pragma aux get_DS = \
"mov ax, ds" \
modify [ ax ] \
value [ ax ];
#endif
