local real_io = io

io = {}
io.open = __io_open
io.close = real_io.close

function io.lines(path)
	local file = io.open(path, "r")
	if not file then
		return nil
	end
	return (function()
		local line = file:read()
		if line == nil then
			file:close()
		end
		return line
	end)
end

real_io = nil
