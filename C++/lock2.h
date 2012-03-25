#include "platform.h"
#include "abstractlock.h"
typedef unsigned int uint;

class lock2 : public abstractlock {
private:
    OPA_int_t next_ticket;
    uint now_serving;
public:
    lock2() {
        OPA_store_int(&next_ticket, 0);
        now_serving = 0;
    }
    virtual void acquire(void) {
        uint my_ticket = OPA_fetch_and_incr_int(&next_ticket);
        for (;;) {
            sleep_fn(my_ticket - now_serving);
            if (now_serving == my_ticket) return;
        }
    }
    virtual void release(void) {
        // precondition: the calling process must hold the lock
        now_serving++;
    }
};
