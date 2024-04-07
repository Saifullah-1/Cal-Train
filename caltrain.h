#include <pthread.h>

struct station {
    int empty_seats;
    int waiting_passengers;
    int in_train_passengers;

    pthread_mutex_t mutex;
    // condition variables
    pthread_cond_t train_arrived;
    pthread_cond_t train_loaded;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);