---@meta

---Inotify event data passed to callbacks.
---@class InotifyEvent
---@field wd integer        @ Watch descriptor
---@field mask integer      @ Bitmask of inotify events
---@field cookie integer    @ Unique ID for related events (e.g. renames)
---@field name string       @ Name of the file/directory affected (may be empty for some events)

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
---local wd, errmsg, errno = watchdog.init()
---if not wd then error(errmsg) end
---```
---@return Watchdog?        @ Watchdog object or `nil` on error
---@return string? errmsg   @ Error message if initialization failed
---@return integer? errno   @ System `errno` on failure
function watchdog.init() end

---Instance of a running inotify watcher.
---@class Watchdog
local meta = {}

---Poll for filesystem events and dispatch callbacks.
---
---Blocks until events are available or `timeout` expires.
---
---Usage:
---```lua
---local ok, errmsg, errno = wd:poll(500)
---if not ok then error(errmsg) end
---```
---@param self Watchdog
---@param timeout integer?  @ Timeout in milliseconds (-1 = infinite). Optional.
---@return integer?         @ Number of events processed or 0 on timeout
---@return string? errmsg
---@return integer? errno
function meta:poll(timeout) end

---Add a path to be watched for changes.
---
---Usage:
---```lua
---local watch_fd, errmsg, errno = wd:add("/tmp", watchdog.IN_CREATE, function(ev)
---  print("Created:", ev.name)
---end)
---```
---@param self Watchdog
---@param pathname string         @ Path to watch (directory or file)
---@param mask integer            @ Bitmask of inotify event flags
---@param callback fun(ev: InotifyEvent) @ Function called when event fires
---@return integer? wd            @ Watch descriptor or `nil` on error
---@return string? errmsg
---@return integer? errno
function meta:add(pathname, mask, callback) end

---Remove a watch from a given descriptor.
---
---Usage:
---```lua
---local ok, errmsg, errno = wd:remove(wd_id)
---if not ok then error(errmsg) end
---```
---@param self Watchdog
---@param wd integer              @ Watch descriptor previously returned by `add`
---@return integer?              @ 0 on success, `nil` on error
---@return string? errmsg
---@return integer? errno
function meta:remove(wd) end

---Close the inotify watcher and release all callbacks.
---
---Usage:
---```lua
---wd:close()
---```
---@param self Watchdog
---@return integer?              @ 0 on success, `nil` on error
---@return string? errmsg
---@return integer? errno
function meta:close() end

return watchdog
