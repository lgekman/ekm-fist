# ekm-fist
Filter Stack in C

The `fist` filter-stack is a **minimalistic** implementation of data
filters in C. The idea of a chain or stack of filters is well known,
e.g. unix `pipes`.


    FistSource -----------------------------------> FistSink

    # fistFilter_push()

    FistSource -----> FistSink + FistSource ------> FistSink
                     |      FistFilter     |


Features;

* Up and Downstream errors
* Downstream wite
* Flush
* Noting else!

You can push a filter with e.g. compression and/or encryption without
the source even knows about it. The filters can of course also be
re-used.

## Build

    cd src
    make
    # or;
    make test

The objects will be built in `/tmp/$USER-fist` by default. I like to
keep object and source files separeted. If you want them in the src
dir do;

    cd src
    make O=.


## Main structures

```C
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
```

