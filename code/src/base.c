/* types */
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef float    real32;
typedef double   real64;
typedef int8     bool8;
typedef int16    bool16;
typedef int32    bool32;
typedef int64    bool64;

/* macros */
#define stmnt(S) do{S}while(0)

#define arrcnt(a) (sizeof(a)/sizeof(*(a)))
#define clamp(l,v,b) ((v<l)?l:(v>b)?b:v)

#define log(format, ...)    printf(format"\n", ##__VA_ARGS__);
#define logerr(format, ...) fprintf(stderr, "%s %d: "format"\n",__FILE__, __LINE__,##__VA_ARGS__);
#define logint(a)           printf(#a" = %ld\n", a);
#define loguint(a)          printf(#a" = %lu\n", a);
#define logreal(a)          printf(#a" = %f\n", a);

#define internal static
#define global_variable static
#define local_persist static
#define true 1
#define false 0

global_variable int8  min_int8  = (int8)  0x80;
global_variable int16 min_int16 = (int16) 0x8000;
global_variable int32 min_int32 = (int32) 0x80000000;
global_variable int64 min_int64 = (int64) 0x8000000000000000llu;

global_variable int8  max_int8  = (int8)  0x7f;
global_variable int16 max_int16 = (int16) 0x7fff;
global_variable int32 max_int32 = (int32) 0x7fffffff;
global_variable int64 max_int64 = (int64) 0x7fffffffffffffffllu;

global_variable uint8  max_uint8  = (uint8)  0xff;
global_variable uint16 max_uint16 = (uint16) 0xffff;
global_variable uint32 max_uint32 = (uint32) 0xffffffff;
global_variable uint64 max_uint64 = (uint64) 0xffffffffffffffffllu;

global_variable real32 pi32         = 3.14159265359f;
global_variable real32 tau32        = 6.28318530718f;
global_variable real32 e32          = 2.71828182846f;
global_variable real32 gold_big32   = 1.61803398875f;
global_variable real32 gold_small32 = 0.61803398875f;

global_variable real64 pi64         = 3.14159265359;
global_variable real64 tau64        = 6.28318530718;
global_variable real64 e64          = 2.71828182846;
global_variable real64 gold_big64   = 1.61803398875;
global_variable real64 gold_small64 = 0.61803398875;


