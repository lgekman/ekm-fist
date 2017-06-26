#ifndef fist_filters_h
#define fist_filters_h
/*
 * fist-filters.h --
 * 
 *   Filters for "fist" (filter-stack)
 */

#include "fist.h"

/* SinkBuffer:
   Just pile-up incoming data in a byte array. This is intended for tests
   mainly but can be of generic use.
 */
struct FistSink* fistSinkBuffer_create(struct FistSource* source);
void fistSinkBuffer_delete(struct FistSink* obj);
unsigned int fistSinkBuffer_len(struct FistSink* obj);
char const* fistSinkBuffer_data(struct FistSink* obj);
int fistSinkBuffer_error(struct FistSink* obj);


#endif
