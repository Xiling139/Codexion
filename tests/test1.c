#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_f(void *arg) {

    time_t start = time(NULL);
    // ... code ...
    

    while (1) {
        usleep(20000000);

        time_t record = time(NULL);
        double duration = difftime(record, start);
        printf("Elapsed Time: %f\n", duration);
    }

    return (NULL);
}

int main(void) {

    pthread_t thread;
    
    thread = pthread_create(&thread, NULL, &thread_f, NULL);

    int i = 0;
    while (i >= 0)
    {
        printf("%d\n", i);
        i++;
    }
    pthread_join(thread, NULL);
    return 0;
}