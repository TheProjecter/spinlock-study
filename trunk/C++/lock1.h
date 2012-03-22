#include "platform.h"
#include "abstractlock.h"

class lock1 : public abstractlock {
private:
    OPA_int_t lock;
public:
    lock1() {
        OPA_store_int(&lock, 0);
    }
    virtual void acquire(void) {
        int delay = 1;
        while (OPA_cas_int(&lock, 0, 1)) {
            sleep(delay);
            delay <<= 1;
        }
    }
    virtual void release(void) {
        // precondition: the calling process must hold the lock
        OPA_store_int(&lock, 0);
    }
};
