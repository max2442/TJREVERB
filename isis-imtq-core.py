def k_adcs_terminate():
    from cffi import FFI
    ffi = FFI()
    ffi.cdef("""
        void k_adcs_terminate(void);
    """)
    C = ffi.dlopen(None)
    C.k_adcs_terminate()

def k_imtq_watchdog_start():
    from cffi import FFI
    ffi = FFI()
    ffi.cdef("""
        KADCSStatus k_imtq_watchdog_start(void);
    """)
    C = ffi.dlopen(None)
    C.k_imtq_watchdog_start()
    
def k_imtq_watchdog_stop():
    from cffi import FFI
    ffi = FFI()
    ffi.cdef("""
        KADCSStatus k_imtq_watchdog_stop(void);
    """)
    C = ffi.dlopen(None)
    C.k_imtq_watchdog_stop()
    
def k_imtq_reset():
    from cffi import FFI
    ffi = FFI()
    ffi.cdef("""
        KADCSStatus k_imtq_reset(void);
    """)
    C = ffi.dlopen(None)
    C.k_imtq_reset()
