def k_adcs_terminate():
    from cffi import FFI
    ffi = FFI()
    ffi.cdef("""
        void k_adcs_terminate(void);
    """)
    C = ffi.verify("""
        void k_adcs_terminate(void) {
            const struct timespec MUTEX_TIMEOUT = {.tv_sec = 1, .tv_nsec = 0 };

            /* Destroy the mutex */
            if (pthread_mutex_timedlock(&imtq_mutex, &MUTEX_TIMEOUT) != 0)
            {
                perror("Failed to take MTQ mutex");
                fprintf(stderr, "PID: %d TID: %ld", getpid(), syscall(SYS_gettid));
            }
            if (pthread_mutex_unlock(&imtq_mutex) != 0)
            {
                perror("Failed to unlock MTQ mutex");
                fprintf(stderr, "PID: %d TID: %ld", getpid(), syscall(SYS_gettid));
            }
            if (pthread_mutex_destroy(&imtq_mutex) != 0)
            {
                perror("Failed to destroy MTQ mutex");
                fprintf(stderr, "PID: %d TID: %ld", getpid(), syscall(SYS_gettid));
            }

            /* Close the I2C bus */
            k_i2c_terminate(i2c_bus);

            return;
         }
      """)
     C.k_adcs_terminate()

def k_imtq_watchdog_start():
    from cffi import FFI
    ffi = FFI()
    ffi.cdef("""
        KADCSStatus k_imtq_watchdog_start(void);
    """)
    C = ffi.verify("""
        KADCSStatus k_imtq_watchdog_start(void) {
            if (handle_watchdog != 0)
            {
                fprintf(stderr, "ADCS watchdog thread already started\n");
                return ADCS_OK;
            }
            if (wd_timeout == 0)
            {
                fprintf(
                    stderr,
                    "ADCS watchdog has been disabled. No thread will be startd\n");
                return ADCS_OK;
            }
            if (pthread_create(&handle_watchdog, NULL, kprv_imtq_watchdog_thread, NULL)
                != 0)
            {
                perror("Failed to create ADCS watchdog thread");
                handle_watchdog = 0;
                return ADCS_ERROR;
            }
            return ADCS_OK;
        }
        """)
    C.k_imtq_watchdog_start()
    
def k_imtq_watchdog_stop():
    from cffi import FFI
    ffi = FFI()
    ffi.cdef("""
        KADCSStatus k_imtq_watchdog_stop(void);
    """)
    C = ffi.verify("""
        KADCSStatus k_imtq_watchdog_stop(void) {
            if (pthread_cancel(handle_watchdog) != 0)
            {
                perror("Failed to cancel ADCS watchdog thread");
                return ADCS_ERROR;
            }
            /* Wait for the cancellation to complete */
            if (pthread_join(handle_watchdog, NULL) != 0)
            {
                perror("Failed to rejoin ADCS watchdog thread");
                return ADCS_ERROR;
            }

            handle_watchdog = 0;

            return ADCS_OK;
            }
            """)
        C.k_imtq_watchdog_stop()
    
def k_imtq_reset():
    from cffi import FFI
    ffi = FFI()
    ffi.cdef("""
        KADCSStatus k_imtq_reset(void);
    """)
    C = ffi.verify("""
        KADCSStatus k_imtq_reset(void) {
            return k_adcs_reset(SOFT_RESET);
        }
    """)
    C.k_imtq_reset()
    
    
