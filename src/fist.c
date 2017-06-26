#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "fist.h"


#define container_of(ptr, type, member) \
    (type*)((char*)(ptr)-__builtin_offsetof(type,member))


/* ----------------------------------------------------------------------
   Filter API;
 */
void fistFilter_push(struct FistFilter* f, struct FistSource* source)
{
	assert(source != NULL && source->sink != NULL);
	struct FistSink* sink = source->sink;

	assert(f->source.sink == NULL);
	assert(f->sink.source == NULL);

	f->sink.source = source;
	source->sink = &f->sink;

	f->source.sink = sink;
	sink->source = &f->source;
}


void fistFilter_pop(struct FistFilter* f)
{
	struct FistSource* source = f->sink.source;
	struct FistSink* sink = f->source.sink;

	assert(source != NULL);
	assert(sink != NULL);

	source->sink = sink;
	sink->source = source;

	f->sink.source = NULL;
	f->source.sink = NULL;
}

static void fistPassThrough_sinkerror(
	struct FistSink* obj, int reason, char const* reason_str)
{
	struct FistFilter* fp = container_of(obj, struct FistFilter, sink);
	struct FistSink* sink = fp->source.sink; /* The down-stream sink */
	if (sink->error != NULL) sink->error(sink, reason, reason_str);
}

static void fistPassThrough_sourceerror(
	struct FistSource* obj, int reason, char const* reason_str)
{
	struct FistFilter* fp = container_of(obj, struct FistFilter, source);
	struct FistSource* source = fp->sink.source; /* The up-stream source */
	if (source->error != NULL) source->error(source, reason, reason_str);
}

static int fistPassThrough_write(
	struct FistSink* obj, int len, char const* data)
{
	struct FistFilter* fp = container_of(obj, struct FistFilter, sink);
	struct FistSink* sink = fp->source.sink; /* The down-stream sink */
	return sink->write(sink, len, data);
}

void fistFilter_init(struct FistFilter* obj)
{
	memset(obj, 0, sizeof(*obj));
	obj->sink.write = fistPassThrough_write;
	obj->sink.error = fistPassThrough_sinkerror;
	obj->source.error = fistPassThrough_sourceerror;
}


