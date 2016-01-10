local real_debug = debug

debug = {}
debug.getinfo = real_debug.getinfo
debug.traceback = real_debug.traceback

real_debug = nil
