//////////////////////////////////////////////////////////////////////////////////////
//// NOTE(Elias): Types

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

//////////////////////////////////////////////////////////////////////////////////////
//// NOTE(Elias): Helper Macros

#define internal static
#define global_variable static
#define local_persist static

#define true 1
#define false 0

#define Stmnt(S) do{S}while(0)

#define ArrayLength(a) (sizeof(a)/sizeof(*(a)))
#define Min(a,b)        (((a)<(b))?(a):(b))
#define Max(a,b)        (((a)>(b))?(a):(b))
#define Clamp(a,x,b)    (((x)<(a))?(a):((b)<(x))?(b):(x))
#define ClampBot(a,b)   (Max((a),(b)))
#define ClampTop(a,b)   (Min((a),(b)))
#define Abs(a)          (((a)<0)?(-(a)):(a))

#define AssertBreak() (*(int *)0 = 0)
#if ENABLE_ASSERT
#define Assert(c) Stmnt( if (!(c)){ AssertBreak(); })
#else
#define Assert(c)
#endif

#if ENABLE_DEBUGLOG
#define DebugLog(format, ...)       printf(format"\n", ##__VA_ARGS__);
#define DebugLogError(format, ...)  Stmnt(\
                                        fprintf(stderr, "%s %d: ",__FILE__, __LINE__);\
                                        fprintf(stderr, format,##__VA_ARGS__);\
                                        fprintf(stderr, "\n");\
                                        )
#define DebugLogInt(a)              printf(#a" = %ld\n", (int64)a)
#define DebugLogUint(a)             printf(#a" = %lu\n", (uin64)a)
#define DebugLogReal(a)             printf(#a" = %f\n", (real64)a)
#define DebugLogChar(s)             printf(#s" = %c\n", (char)s)
#define DebugLogString(s)           printf(#s" = %s\n", (char *)s)
#else
#define DebugLog(format, ...)
#define DebugLogError(format, ...)
#define DebugLogint(a)
#define DebugLoguint(a)
#define DebugLogReal(a)
#define DebugLogChar(s)
#define DebugLogString(s)
#endif

#define DLLPushBack_NP(f,l,n,next,prev) (((f)==0)?\
                                         ((f)=(l)=(n),(n)->next=(n)->prev=0):\
                                         ((n)->prev=(l),(l)->next=(n),(l)=(n),(n)->next=0))
#define DLLRemove_NP(f,l,n,next,prev) (((f)==(n)?\
                                       ((f)=(f)->next, (f)->prev=0)\
                                       (l)==(n)?\
                                       ((l)=(l)->prev,(l)->next=0):\
                                       ((n)->next->prev=(n)->prev,\
                                        (n)->prev->next=(n)->next)))
#define DLLPushBack(f,l,n)  DLLPushBack_NP(f,l,n,next,prev)
#define DLLPushFront(f,l,n) DLLPushBack_NP(1,f,n,prev,next)
#define DLLRemove(f,l,n)    DLLRemove_NP(f,l,n,next,prev)



//////////////////////////////////////////////////////////////////////////////////////
//// NOTE(Elias): Some Constants

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

