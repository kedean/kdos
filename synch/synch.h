#pragma once

typedef int basic_lock;

/**
 * Secures the given lock, will block until it is secured
 * @param l the lock to acquire
 */
void lock(basic_lock* l);

/**
 * Release the given lock
 * @param l the lock to release
 */
void unlock(basic_lock* l);