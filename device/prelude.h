#ifndef _PRELUDE_H_
#define _PRELUDE_H_

namespace Device {

    typedef unsigned int U32;
    typedef unsigned short U16;
    typedef unsigned char U8;

    typedef int I32;
    typedef short I16;
    typedef char I8;

} // namespace Device

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#endif // _PRELUDE_H_
