#include "tdt4260/cache_lab/cache_impl/simple_cache.hh"

#include <bits/stdc++.h>

#include "base/trace.hh"
#include "debug/AllAddr.hh"
#include "debug/AllCacheLines.hh"
#include "debug/TDTSimpleCache.hh"


namespace gem5
{
struct Entry {
    int tag;    // Tag for the cache line
    bool valid; // Indicates if the cache line is valid

    // Constructor to initialize members
    Entry() : tag(0), valid(false) {}
};
SimpleCache::SimpleCache(int size, int blockSize, int associativity,
                         statistics::Group *parent, const char *name)
    : size(size), blockSize(blockSize), associativity(associativity), cacheName(name),
      stats(parent, name)
{
    numEntries = this->size / this->blockSize;
    numSets = this->numEntries / this->associativity;

    // allocate entries for all sets and ways
    for (int i = 0; i < this->numSets; i++) {
        std::vector<Entry *> vec;

        // TODO: Associative: Allocate as many entries as there are ways
        // i.e. replace vector of single entry with vector of way number of entries
        vec.push_back(new Entry());
        entries.push_back(vec);
    }
}

SimpleCache::SimpleCacheStats::SimpleCacheStats(
    statistics::Group *parent, const char *name)
    : statistics::Group(parent, name),
    ADD_STAT(reqsReceived, statistics::units::Count::get(),
        "Number of requests received from cpu side"),
    ADD_STAT(reqsServiced, statistics::units::Count::get(),
        "Number of requests serviced at this cache level"),
    ADD_STAT(respsReceived, statistics::units::Count::get(),
        "Number of responses received from mem side") {}

void
SimpleCache::recvReq(Addr req, int size)
{
    ++stats.reqsReceived;

    int index = calculateIndex(req);
    int tag = calculateTag(req);

    DPRINTF(TDTSimpleCache, "Debug: Addr: %#x, index: %d, tag: %d, in %s\n",
            req, index, tag, cacheName);
    DPRINTF(AllAddr, "%#x\n", req);
    DPRINTF(AllCacheLines, "%#x\n", req >> ((int) std::log2(blockSize)));

    // if cache line already in cache
    if (hasLine(index, tag)) {
        ++stats.reqsServiced;
        int way = lineWay(index, tag);
        DPRINTF(TDTSimpleCache, "Hit: way: %d\n", way);

        // TODO: Associative: Update LRU info for line in entries

        sendResp(req);
    } else{
        sendReq(req, size);
    }
}

void
SimpleCache::recvResp(Addr resp)
{
    ++stats.respsReceived;

    int index = calculateIndex(resp);
    int tag = calculateTag(resp);

    // there should never be a request (and thus a response) for a line already in the cache
    assert(!hasLine(index, tag));
    //insure that the line is not already in the cache

    //replace the cache line at the give given index
    // TODO: Direct-Mapped: Record new cache line in entries
    entries[index][0]->tag = tag;

    DPRINTF(TDTSimpleCache, "Miss: Replaced line at index %d with tag %d\n", index, tag);



    // TODO: Associative: Record LRU info for new line in entries
    sendResp(resp);
}

int
SimpleCache::calculateTag(Addr req)
{
    // TODO: Direct-Mapped: Calculate tag
    // hint: req >> ((int)std::log2(...
    return req >> ((int)std::log2(blockSize) + (int)std::log2(numSets));
}

int
SimpleCache::calculateIndex(Addr req)
{
    // TODO: Direct-Mapped: Calculate index
    return (req >> (int)std::log2(blockSize)) & (numSets - 1);
}

bool
SimpleCache::hasLine(int index, int tag)
{
    // TODO: Direct-Mapped: Check if line is already in cache
    return entries[index][0]->tag == tag;
    // TODO: Associative: Check all possible ways

}

int
SimpleCache::lineWay(int index, int tag)
{
    // TODO: Associative: Find in which way a cache line is stored
    return 0;
}

int
SimpleCache::oldestWay(int index)
{
    // TODO: Associative: Determine the oldest way
    return 0;
}

void
SimpleCache::sendReq(Addr req, int size)
{
    memSide->recvReq(req, size);
}

void
SimpleCache::sendResp(Addr resp)
{
    cpuSide->recvResp(resp);
}

}






