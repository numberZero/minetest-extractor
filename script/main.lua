function table.count(self)
	local n = 0
	for k, v in pairs(self) do
		n = n + 1
	end
	return n
end

core.log("Total items: ", table.count(core.registered_items))
core.log("Total craftitems: ", table.count(core.registered_craftitems))
core.log("Total nodes: ", table.count(core.registered_nodes))
core.log("Total tools: ", table.count(core.registered_tools))
