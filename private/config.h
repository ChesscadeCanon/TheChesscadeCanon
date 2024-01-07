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
