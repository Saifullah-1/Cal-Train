#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
    station->empty_seats = 0;
    station->waiting_passengers = 0;
    station->in_train_passengers = 0;
    pthread_mutex_init(&station->mutex, NULL);
    pthread_cond_init(&station->train_arrived, NULL);
    pthread_cond_init(&station->train_loaded, NULL);
}

void
station_load_train(struct station *station, int count) // count --> number of empty seats
{
    pthread_mutex_lock(&station->mutex);
    station->empty_seats = count;
    while (station->empty_seats > 0 && station->waiting_passengers > 0) {
        pthread_cond_broadcast(&station->train_arrived);
        pthread_cond_wait(&station->train_loaded, &station->mutex);
    }
    station->empty_seats = 0;
    pthread_mutex_unlock(&station->mutex);
}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->mutex);
    station->waiting_passengers++;
    while (station->empty_seats == 0) {
        pthread_cond_wait(&station->train_arrived, &station->mutex);
    }
    station->in_train_passengers++;
    station->empty_seats--;
    station->waiting_passengers--;
    pthread_mutex_unlock(&station->mutex);
}

void
station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->mutex);
    station->in_train_passengers--;
    // if the train is full then it must move
    if (station->in_train_passengers == 0 && (station->empty_seats == 0 || station->waiting_passengers == 0)) {
        pthread_cond_signal(&station->train_loaded);
    }
    pthread_mutex_unlock(&station->mutex);
}
