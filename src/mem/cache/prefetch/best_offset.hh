#ifndef __BEST_OFFSET_PREFETCHER_HH__
#define __BEST_OFFSET_PREFETCHER_HH__

#include "mem/cache/prefetch/base.hh"

class BestOffsetPrefetcher : public BasePrefetcher {
public:
    BestOffsetPrefetcher(const BestOffsetPrefetcherParams *p);
    void calculatePrefetch(const PrefetchInfo &pfi, std::vector<AddrPriority> &addresses) override;

private:
    // Add private members and methods here
};

#endif // __BEST_OFFSET_PREFETCHER_HH__