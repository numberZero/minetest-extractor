DIR_DELIM = "/"
INIT = "game"

core.worldpath = "/world" -- WORLDROOT
core.mods_root = "/mod" -- MODSROOT
core.builtin_root = "/builtin" -- "builtin"
core.print = print
core.modpathes = {}
core.modnames = {}

function setfenv(...)
-- 	core.log("setfenv")
end

function table.foreach(table, func)
	for k, v in ipairs(table) do
		func(k, v)
	end
end

function ItemStack(text)
	local t = string.split(text, " ")
	return {
		name = t[1],
		count = t[2] or 1,
		is_empty = function(self)
				return (self == nil) or (self.count == 0)
			end,
		get_name = function(self)
				return self.name
			end
	}
end

function Settings(path)
	return {
		to_table = function(self)
			return self
		end,
		get = function(self, key)
			return self[key]
		end,
		get_bool = function(self, key)
			if self:get(key) then
				return true
			else
				return false
			end
		end,
		set = function(self, key, value)
			self[key] = value
		end
	}
end

-- Register

function core.register_alias_raw(name, convert_to)
	core.log("Registering alias [" .. name .. "] → [" .. convert_to .. "]")
end

function core.register_item_raw(desc)
	core.log("Registering item [" .. desc.name .. "]")
end

function core.register_globalstep(fn)
	core.log("Registering global step handler [" .. tostring(fn) .. "]")
end

function core.register_craft(desc)
	if desc.output then
		local res = desc.output:split(" ")
		core.log("Registering craft for [" .. res[1] .. "]")
	else
		core.log("Registering craft disabling")
	end
end

function core.register_biome()
end

function core.register_ore()
end

function core.register_decoration()
end

function core.create_detached_inventory_raw(name)
	return {
		set_size = function() end
	}
end

-- Public API

function PerlinNoise(seed, octaves, persistence, scale)
	core.log("PerlinNoise class")
end

function core.add_particlespawner()
end

function core.get_voxel_manip()
end

function core.get_builtin_path()
	return core.builtin_root
end

function core.setting_getbool(key)
--	print("Bool Setting: " .. key)
	return false
end

function core.setting_get(key)
--	print("Setting: " .. key)
	if key == "dedicated_server_step" then
		return 0.05
	end
	return nil
end

function core.setting_set(key, value)
end

function core.get_worldpath()
	return core.worldpath
end

function core.get_modpath()
	return core.modpath
end

function core.get_current_modname()
	return core.modname
end

function core.get_mapgen_params()
	return { mgname = "singlenode" }
end

function core.get_last_run_mod()
	return core.modname
end

function core.is_singleplayer()
	return true
end

function core.get_content_id()
	return -1
end

-- Internals

function core.real_load_mod(name, path)
	core.log("Loading mod " .. name .. " from " .. path)
	core.modname = name
	core.modpath = path
	core.modpathes[name] = path
	core.modnames[#core.modnames + 1] = name
	dofile(path .. "/init.lua")
end

-- function core.load_mod(modpack, modname)
-- 	local worldpath = core.worldpath
-- 	local modpath = core.mods_root
-- 	if modpack then
-- 		modpath = modpath .. "/" .. modpack
-- 	else
-- 		modpack = ""
-- 	end
-- 	modpath = modpath .. "/" .. modname
-- 	core.log("Loading mod " .. modpack .. "::" .. modname)
-- 	core.real_load_mod(modname, modpath)
-- end

-- Initializing

local scriptpath = core.get_builtin_path()..DIR_DELIM
local commonpath = scriptpath.."common"..DIR_DELIM
local gamepath = scriptpath.."game"..DIR_DELIM

dofile(commonpath.."strict.lua")
dofile(commonpath.."serialize.lua")
dofile(commonpath.."misc_helpers.lua")
dofile(commonpath.."vector.lua")

dofile(gamepath.."constants.lua")
dofile(gamepath.."item.lua")
dofile(gamepath.."register.lua")
dofile(gamepath.."item_entity.lua")
dofile(gamepath.."deprecated.lua")
dofile(gamepath.."misc.lua")
dofile(gamepath.."privileges.lua")
-- dofile(gamepath.."auth.lua")
dofile(gamepath.."chatcommands.lua")
dofile(gamepath.."static_spawn.lua")
dofile(gamepath.."detached_inventory.lua")
dofile(gamepath.."falling.lua")
dofile(gamepath.."features.lua")
dofile(gamepath.."voxelarea.lua")
dofile(gamepath.."forceloading.lua")
dofile(gamepath.."statbars.lua")

-- dofile("builtin/init.lua")
-- dofile("builtin/common/vector.lua")
-- dofile("builtin/game/register.lua")
-- dofile("builtin/game/misc.lua")
-- dofile("builtin/game/item.lua")
-- dofile("builtin/game/chatcommands.lua")

core.register_on_mapgen_init = function(func) end
core.get_modpath = function(name) return core.modpathes[name] end
core.get_modnames = function(name) return core.modnames end

function __builtin__load_mod(name)
	core.real_load_mod(name, "/mod/" .. name)
end

minetest = core
