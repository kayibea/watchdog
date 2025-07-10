# InotifyEvent

## cookie


```lua
integer
```

Unique ID for related events (e.g., rename)

## len


```lua
integer
```

Length of name (internal)

## mask


```lua
integer
```

Bitmask of events

## name


```lua
string?
```

Name of the file or directory affected

## wd


```lua
integer
```

Watch descriptor


---

# LuaLS


---

# Watchdog

## add


```lua
(method) Watchdog:add(pathname: string, mask: integer, callback: fun(ev: InotifyEvent))
  -> wd: integer
```

Add a path to be watched.

Usage:
```lua
wd:add("/tmp", watchdog.IN_CREATE, function(ev)
  print("File created:", ev.name)
end)
```

@*param* `pathname` — Path to file or directory

@*param* `mask` — Bitmask of inotify event types

@*param* `callback` — Callback invoked on matching events

@*return* `wd` — Watch descriptor (used to remove later)

## close


```lua
(method) Watchdog:close()
```

Stop all monitoring and close the inotify file descriptor.

This also releases all registered callbacks.

## poll


```lua
(method) Watchdog:poll(timeout?: integer)
```

Polls for filesystem events and dispatches callbacks.

This blocks until an event occurs or until `timeout` expires.

Usage:
```lua
wd:poll(100) -- Waits 100ms
```

@*param* `timeout` — Timeout in milliseconds (-1 = infinite)

## remove


```lua
(method) Watchdog:remove(wd: integer)
```

Remove a specific watch descriptor.

Usage:
```lua
local wd_id = wd:add("/tmp", mask, cb)
wd:remove(wd_id)
```

@*param* `wd` — Watch descriptor returned by `add`


---

# watchdog

## IN_ACCESS


```lua
integer
```

## IN_ALL_EVENTS


```lua
integer
```

## IN_ATTRIB


```lua
integer
```

## IN_CLOSE


```lua
integer
```

## IN_CLOSE_NOWRITE


```lua
integer
```

## IN_CLOSE_WRITE


```lua
integer
```

## IN_CREATE


```lua
integer
```

## IN_DELETE


```lua
integer
```

## IN_DELETE_SELF


```lua
integer
```

## IN_DONT_FOLLOW


```lua
integer
```

## IN_EXCL_UNLINK


```lua
integer
```

## IN_IGNORED


```lua
integer
```

## IN_ISDIR


```lua
integer
```

## IN_MASK_ADD


```lua
integer
```

## IN_MODIFY


```lua
integer
```

## IN_MOVE


```lua
integer
```

## IN_MOVED_FROM


```lua
integer
```

## IN_MOVED_TO


```lua
integer
```

## IN_MOVE_SELF


```lua
integer
```

## IN_ONESHOT


```lua
integer
```

## IN_ONLYDIR


```lua
integer
```

## IN_OPEN


```lua
integer
```

## IN_Q_OVERFLOW


```lua
integer
```

## IN_UNMOUNT


```lua
integer
```

## init


```lua
function watchdog.init()
  -> Watchdog
```

Initialize a new inotify watcher.

Usage:
```lua
local wd = watchdog.init()
```