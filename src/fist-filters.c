/*
 * fist-filters.c --
 * 
 *   Filters for "fist" (filter-stack)
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "fist-filters.h"

#define container_of(ptr, type, member) \
	(type*)((char*)(ptr)-__builtin_offsetof(type,member))

/* ----------------------------------------------------------------------
   SinkBuffer
 */

struct FistSinkBuffer {
	struct FistSink sink;
	int error;
	unsigned avail;
	unsigned len;
	char* data;
};

static int fistSinkBuffer_write_fn(
	struct FistSink* obj, int len, char const* data)
{
	struct FistSinkBuffer* sb = container_of(obj, struct FistSinkBuffer, sink);
	if (sb->len + len > sb->avail) {
		sb->avail += (len + 1024);
		sb->data = realloc(sb->data, sb->avail);
	}
	memcpy(sb->data + sb->len, data, len);
	sb->len += len;
	return len;
}
static void fistSinkBuffer_sinkerror(
	struct FistSink* obj, int error, char const* error_str)
{
	struct FistSinkBuffer* sb = container_of(obj, struct FistSinkBuffer, sink);
	sb->error = error;
}

struct FistSink* fistSinkBuffer_create(struct FistSource* source)
{
	assert(source != NULL);
	struct FistSinkBuffer* sb = calloc(1, sizeof(struct FistSinkBuffer));
	sb->sink.write = fistSinkBuffer_write_fn;
	sb->sink.error = fistSinkBuffer_sinkerror;
	sb->sink.source = source;
	source->sink = &sb->sink;
	return &sb->sink;
}

void fistSinkBuffer_delete(struct FistSink* obj)
{
	struct FistSinkBuffer* sb = container_of(obj, struct FistSinkBuffer, sink);
	free(sb->data);
	free(sb);
}

unsigned int fistSinkBuffer_len(struct FistSink* obj)
{
	struct FistSinkBuffer* sb = container_of(obj, struct FistSinkBuffer, sink);
	return sb->len;
}

char const* fistSinkBuffer_data(struct FistSink* obj)
{
	struct FistSinkBuffer* sb = container_of(obj, struct FistSinkBuffer, sink);
	return sb->data;
}

int fistSinkBuffer_error(struct FistSink* obj)
{
	struct FistSinkBuffer* sb = container_of(obj, struct FistSinkBuffer, sink);
	return sb->error;
}
