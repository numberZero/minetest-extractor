local real_os = os

os = {}

function os.getenv()
	return nil
end

os.date = real_os.date
os.time = real_os.time
os.clock = real_os.clock

function os.rename(old, new)
	return false
end

real_os = nil
