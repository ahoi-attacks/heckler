#ifndef RAW_SPINLOCK_H
#define RAW_SPINLOCK_H

#define INIT_SPINLOCK(X) (*X=1)


/**
 * @brief Aquire the lock
 * 
 * @param lock pointer to lock
 */
extern void raw_spinlock_lock(int* lock);

/**
 * @brief Release the lock
 * 
 * @param lock pointer to lock
 */
extern void raw_spinlock_unlock(int* lock);

#endif
