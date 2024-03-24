#define DEBUG_MODE 1
#define _DEBUG "DEBUG: "

#if DEBUG_MODE

#include <stdlib.h>

#define ASSERTION_NOT_MET -988
#define ASSERT(x) if(!(x)) { printf(_DEBUG"ASSERT FAILED at: %s @ %s:%d\n", __func__, __FILE__, __LINE__); exit(ASSERTION_NOT_MET); }
#define ASSERTE(x, e) if(!(x)) { printf(_DEBUG"ASSERT FAILED at: %s @ %s:%d\n", __func__, __FILE__, __LINE__); e; exit(ASSERTION_NOT_MET); }

#else

#define ASSERT(x) ;
#define ASSERTE(x, e) ;

#endif

#define FAIL ASSERT(false)
#define FAILE(e) ASSERT(false, e)

#define SET_PAWNS   false
#define SET_QUEENS  false
#define SET_BISHOPS false
#define SET_KNIGHTS false
#define SET_ROOKS   false
#define SET_KINGS   true