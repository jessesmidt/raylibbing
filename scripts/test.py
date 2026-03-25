import ctypes
import time
import os

filename: str = "build/libengine.so"

if not os.path.exists(filename):
    raise FileNotFoundError(f"Configuration file '{filename}' was not found")
engine = ctypes.CDLL("build/libengine.so")
engine.init.restype = None
engine.update.restype = None
engine.shutdown.restype = None
engine.should_close.restype = ctypes.c_int
engine.init()

try:
    while not engine.should_close():
        engine.update()
        time.sleep(0.016)
except KeyboardInterrupt:
    engine.shutdown()