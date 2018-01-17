#ifndef _LIBPEEPHOLE_DECL_H_
#define _LIBPEEPHOLE_DECL_H_

// Declare the ph namespace
#define PH_NAMESPACE_DECL_BEGIN namespace peephole {
#define PH_NAMESPACE_DECL_END } \
    using namespace peephole;

#define DISALLOW_CONSTRUCT(class_name) class_name()
#define DISALLOW_COPY_CONSTRUCT(class_name) class_name(const class_name &)
#define DISALLOW_ASSIGNMENT(class_name) class_name & operator =(const class_name &)

#endif//_LIBPEEPHOLE_DECL_H_

