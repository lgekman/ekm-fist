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

static int fistSinkBuffer_writev_fn(
	struct FistSink* obj, const struct iovec *iov, int iovcnt)
{
	struct FistSinkBuffer* sb = container_of(obj, struct FistSinkBuffer, sink);
	size_t len = 0;
	int i;
	const struct iovec *ip;
	for (i = 0, ip = iov; i < iovcnt; i++, ip++) {
		len += ip->iov_len;
	}
	if (sb->len + len > sb->avail) {
		sb->avail += (len + 1024);
		sb->data = realloc(sb->data, sb->avail);
	}
	char* cp = sb->data + sb->len;
	for (i = 0, ip = iov; i < iovcnt; i++, ip++) {
		memcpy(cp, ip->iov_base, ip->iov_len);
		cp += ip->iov_len;
	}

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
	sb->sink.writev = fistSinkBuffer_writev_fn;
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
