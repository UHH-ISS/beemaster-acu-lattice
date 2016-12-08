include ../rocksdb/make_config.mk

ifndef DISABLE_JEMALLOC
	ifdef JEMALLOC
		PLATFORM_CXXFLAGS += "-DROCKSDB_JEMALLOC"
	endif
	EXEC_LDFLAGS := $(JEMALLOC_LIB) $(EXEC_LDFLAGS) -lpthread
	PLATFORM_CXXFLAGS += $(JEMALLOC_INCLUDE)
endif
	
.PHONY: clean librocksdb

all: rocks 

rocks: librocksdb rocks.cc
	$(CXX) $(CXXFLAGS) $@.cc -o$@ ../rocksdb/librocksdb.a -I../include -O2 -std=c++11 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

latticeStorage: librocksdb latticeStorage.cc
	$(CXX) $(CXXFLAGS) $@.cc -o$@ ../rocksdb/librocksdb.a -I../include -O2 -std=c++11 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

rocks2: librocksdb rocks2.cc
	$(CXX) $(CXXFLAGS) $@.cc -o$@ ../rocksdb/librocksdb.a -I../include -O2 -std=c++11 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

simple-example: librocksdb simple_example.cc
	$(CXX) $(CXXFLAGS) $@.cc -o$@ ../include/rocksdb/librocksdb.a -I../include -O2 -std=c++11 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -I../include

clean:
	rm -rf ./rocks ./simple_example ./rocks2 ./latticeStorage
librocksdb:
	cd ../rocksdb && $(MAKE) static_lib
