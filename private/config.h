#define DROP_KEY ' '
#define QUIT_KEY 'q'
#define PAUSE_KEY 'p'
#define LEFT_KEY 'A'
#define RIGHT_KEY 'D'
#define DOWN_KEY 'S'

#define VERBOSE 0
#define VERBOSE_MALLOC_FREE 0
#if VERBOSE
#define ERRLOG(M) fprintf(stderr, M)
#define ERRLOGF(M, F) fprintf(stderr, M, F)
#else
#define ERRLOG(M) ;
#define ERRLOGF(M, F) ;
#endif
#define MEMLOG(M) ERRLOG(M)
#define MEMLOGF(M, F) ERRLOGF(M, F)
