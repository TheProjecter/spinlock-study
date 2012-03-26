#include <cassert>
#include "platform.h"
#include "abstractlock.h"
typedef unsigned int uint;

#define SLOT_STEP (CACHE_LINE_SIZE_BYTES/sizeof(bool))

#define HAS_LOCK 0
#define MUST_WAIT 1

class lock3 : public abstractlock {
private:
    volatile bool *slots;
    OPA_int_t next_slot;
    int nprocs;
    
    int my_place; // temporary var (for lock holder)
public:
    lock3(const int _nprocs) {
        assert(_nprocs > 0);
        nprocs = _nprocs;
        slots = new volatile bool[nprocs*SLOT_STEP];
        slots[0] = HAS_LOCK;
        for (int i=SLOT_STEP;i<nprocs*SLOT_STEP;i+=SLOT_STEP) {
            slots[i] = MUST_WAIT;
        }
        OPA_store_int(&next_slot, 0);
    }
    ~lock3() {
        delete[] slots;
    }
    virtual void acquire(void) {
        int _my_place = OPA_fetch_and_incr_int(&next_slot);
        if ((_my_place % nprocs) == 0) {
            OPA_add_int(&next_slot, -nprocs);
        }
        _my_place %= nprocs;
        while (slots[_my_place] == MUST_WAIT);
        slots[_my_place] = MUST_WAIT;
        
        // now that we've obtained the lock, we can use var. my_place safely
        my_place = _my_place;
    }
    virtual void release(void) {
        // precondition: the calling process must hold the lock
        // note: this "my_place" must be the same as the last one obtained
        //       in acquire(), by anderson's algorithm, since only
        //       a process holding the lock can call release().
        slots[(my_place+1) % nprocs] = HAS_LOCK;
    }
};
