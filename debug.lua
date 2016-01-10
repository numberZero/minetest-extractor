local real_debug = debug

debug = {}

function debug.getinfo()
	return {}
end

debug.traceback = real_debug.traceback

real_debug = nil
