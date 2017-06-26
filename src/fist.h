#ifndef fist_h
#define fist_h
/*
 * fist.h --
 * 
 *  Filter Stack.
 *
 */

#define FIST_ALIGN(x) (((x) + 3)&((int)0 - 4))
struct FistSink;
struct FistSource;

/*
 * Write data to a sink-node.
 *
 * len=0 means End-of-Data. 
 * @returns; The number of bytes "consumed", or <0 in case of error.
 */
typedef int (*fistWriteFn_t)(
	struct FistSink* obj, int len, char const* data);

/* "Downstream" error notification */
typedef void (*fistSinkErrorFn_t)(
	struct FistSink* obj, int error, char const* error_str);

/* "Upstream" error notification */
typedef void (*fistSourceErrorFn_t)(
	struct FistSource* obj, int error, char const* error_str);

/**
 * If this function is defined the sink will collect data until the
 * "flush" function is called.
 */
typedef int (*fistFlushFn_t)(struct FistSink* obj);


struct FistSource {
	fistSourceErrorFn_t error;
	struct FistSink* sink;
};

struct FistSink {
	fistWriteFn_t write;
	fistSinkErrorFn_t error;
	fistFlushFn_t flush;
	struct FistSource* source;
};

struct FistFilter {
	struct FistSource source;
	struct FistSink sink;
};


void fistFilter_push(struct FistFilter* obj, struct FistSource* source);
void fistFilter_pop(struct FistFilter* obj);

/* Set all functions to "pass-through", except "flush" which is set to NULL.*/
void fistFilter_init(struct FistFilter* obj);


/* Pass-through filter */
struct FistFilter* fistPassThrough_create(void);
void fistPassThrough_delete(struct FistFilter* f);
unsigned long fistPassThrough_count(struct FistFilter* f);


#endif
