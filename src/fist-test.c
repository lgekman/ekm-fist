#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "fist-filters.h"

static void test_sink_buffer(void)
{
	printf("%s...\n", __FUNCTION__);

	assert(FIST_ALIGN(8) == 8);
	assert(FIST_ALIGN(9) == 12);
	assert(FIST_ALIGN(10) == 12);
	assert(FIST_ALIGN(11) == 12);
	assert(FIST_ALIGN(12) == 12);

	struct FistSource source = {NULL, NULL};
	struct FistSink* fs = fistSinkBuffer_create(&source);
	assert(fs != NULL);
	assert(fs->write != NULL);
	assert(fs->error != NULL);
	assert(fistSinkBuffer_len(fs) == 0);
	assert(fistSinkBuffer_data(fs) == NULL);
	assert(fistSinkBuffer_error(fs) == 0);
	assert(source.sink == fs);

	static char const* const msg = "Hello world\n";
	int len = strlen(msg)+1;
	int rc = source.sink->write(source.sink, len, msg);
	assert(rc == len);
	assert(fistSinkBuffer_len(fs) == len);
	assert(fistSinkBuffer_data(fs) != NULL);
	assert(strcmp(fistSinkBuffer_data(fs), msg) == 0);

	char dbuf[2000];
	rc = source.sink->write(source.sink, sizeof(dbuf), dbuf);
	assert(rc == sizeof(dbuf));
	assert(fistSinkBuffer_len(fs) == (len + sizeof(dbuf)));
	assert(fistSinkBuffer_data(fs) != NULL);

	source.sink->error(source.sink, -55, "Fail");
	assert(fistSinkBuffer_error(fs) == -55);

	fistSinkBuffer_delete(fs);
	printf("%s - OK\n", __FUNCTION__);
}

static void test_pass_through(void)
{
	printf("%s...\n", __FUNCTION__);
	struct FistSource source = {NULL, NULL};
	struct FistSink* fs = fistSinkBuffer_create(&source);
	assert(fs->source == &source);

	struct FistFilter f;
	struct FistFilter* fp = &f;
	fistFilter_init(fp);

	fistFilter_push(fp, &source);
	assert(source.sink == &fp->sink);
	assert(fs->source == &fp->source);

	static char const* const msg = "Hello world\n";
	int len = strlen(msg)+1;
	int rc = source.sink->write(source.sink, len, msg);
	assert(rc == len);
	assert(fistSinkBuffer_len(fs) == len);
	assert(fistSinkBuffer_data(fs) != NULL);
	assert(strcmp(fistSinkBuffer_data(fs), msg) == 0);

	source.sink->error(source.sink, -30, msg);
	assert(fistSinkBuffer_error(fs) == -30);

	fistFilter_pop(fp);
	assert(source.sink == fs);
	assert(fs->source == &source);

	fistSinkBuffer_delete(fs);
	printf("%s - OK\n", __FUNCTION__);
}

int
main(int argc, char* argv[])
{
	test_sink_buffer();
	test_pass_through();
	return 0;
}
