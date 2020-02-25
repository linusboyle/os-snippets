#include <signal.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>

struct sigaction old_sa;

void handler(int signum, siginfo_t* info, void* context) {
    if (info->si_code != FPE_INTDIV) {
        // call the old handler, if any.
        if (old_sa.sa_flags & SA_SIGINFO) {
            old_sa.sa_sigaction(signum, info, context);
        } else {
            old_sa.sa_handler(signum);
        }

        return;
    }
    
    // Now we are sure it's caused by divide-by-zero
    void* trigger_addr = info->si_addr;
    printf("Divide by Zero at %llx!\n", (long long)trigger_addr);
    // However, there is probably no general way to get the operands outside this boundary.
    exit(1);
}

int main(void) {
    int a, b, c;
    struct sigaction sa;

    printf("Enter two operands:\n");
    scanf("%d %d", &a, &b);

    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGFPE, &sa, &old_sa) != 0) {
        perror("Set handler failed");
        return 1;
    }

    c = a / b;
    printf("Division succeeded with result %d\n", c);

    return 0;
}
