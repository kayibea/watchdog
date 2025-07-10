---@meta

---Inotify event data passed to callbacks.
---@class InotifyEvent
---@field wd integer        @ Watch descriptor
---@field len integer       @ Length of name (internal)
---@field name? string      @ Name of the file or directory affected
---@field mask integer      @ Bitmask of events
---@field cookie integer    @ Unique ID for related events (e.g., rename)

---Main module for interacting with inotify from Lua.
---@class watchdog
---@field IN_ACCESS integer
---@field IN_MODIFY integer
---@field IN_ATTRIB integer
---@field IN_CLOSE_WRITE integer
---@field IN_CLOSE_NOWRITE integer
---@field IN_OPEN integer
---@field IN_MOVED_FROM integer
---@field IN_MOVED_TO integer
---@field IN_CREATE integer
---@field IN_DELETE integer
---@field IN_DELETE_SELF integer
---@field IN_MOVE_SELF integer
---@field IN_UNMOUNT integer
---@field IN_Q_OVERFLOW integer
---@field IN_IGNORED integer
---@field IN_ONLYDIR integer
---@field IN_DONT_FOLLOW integer
---@field IN_EXCL_UNLINK integer
---@field IN_MASK_ADD integer
---@field IN_ISDIR integer
---@field IN_ONESHOT integer
---@field IN_CLOSE integer
---@field IN_MOVE integer
---@field IN_ALL_EVENTS integer
local watchdog = {}

---Initialize a new inotify watcher.
---
---Usage:
---```lua
---local wd = watchdog.init()
---```
---@return Watchdog
function watchdog.init() end

---Instance of a running inotify watcher.
---@class Watchdog
local meta = {}

---Polls for filesystem events and dispatches callbacks.
---
---This blocks until an event occurs or until `timeout` expires.
---
---Usage:
---```lua
---wd:poll(100) -- Waits 100ms
---```
---@param self Watchdog
---@param timeout integer? Timeout in milliseconds (-1 = infinite)
function meta:poll(timeout) end

---Add a path to be watched.
---
---Usage:
---```lua
---wd:add("/tmp", watchdog.IN_CREATE, function(ev)
---  print("File created:", ev.name)
---end)
---```
---@param self Watchdog
---@param pathname string       Path to file or directory
---@param mask integer          Bitmask of inotify event types
---@param callback fun(ev: InotifyEvent) Callback invoked on matching events
---@return integer wd           Watch descriptor (used to remove later)
function meta:add(pathname, mask, callback) end

---Remove a specific watch descriptor.
---
---Usage:
---```lua
---local wd_id = wd:add("/tmp", mask, cb)
---wd:remove(wd_id)
---```
---@param self Watchdog
---@param wd integer Watch descriptor returned by `add`
function meta:remove(wd) end

---Stop all monitoring and close the inotify file descriptor.
---
---This also releases all registered callbacks.
---@param self Watchdog
function meta:close() end

return watchdog
