#include "board.h"
#include "thread.h"

#define ENABLE_DEBUG    (1)
#include "debug.h"

void send_udp(char *addr_str, uint16_t port, uint8_t *data, uint16_t datalen);

char first_thread_stack[1024];
char second_thread_stack[1024];

#define FIRST_THREAD_PRIO 1
#define SECOND_THREAD_PRIO 0

void* second_thread(void*);

void* first_thread(void* arg) {
    (void) arg;

    kernel_pid_t second_thread_pid = thread_create(second_thread_stack, sizeof(second_thread_stack), SECOND_THREAD_PRIO, THREAD_CREATE_STACKTEST, second_thread, NULL, "second_thread");

    for (;;) {
        LED_ON;
        thread_wakeup(second_thread_pid);
    }
}

void* second_thread(void* arg) {
    for (;;) {
        thread_sleep();
        LED_OFF;
    }
}

int main(void) {
    // Disable global interrupts. This will prevent RIOT from functioning
    // correctly, but will make the context switch times more accurate.
    irq_disable();

    thread_create(first_thread_stack, sizeof(first_thread_stack), FIRST_THREAD_PRIO, THREAD_CREATE_STACKTEST, first_thread, NULL, "first_thread");

    return 0;
}
