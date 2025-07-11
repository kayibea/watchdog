# InotifyEvent

## cookie


```lua
integer
```

Unique ID for related events (e.g. renames)

## mask


```lua
integer
```

Bitmask of inotify events

## name


```lua
string
```

Name of the file/directory affected (may be empty for some events)

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
  -> wd: integer?
  2. errmsg: string?
  3. errno: integer?
```

Add a path to be watched for changes.

Usage:
```lua
local watch_fd, errmsg, errno = wd:add("/tmp", watchdog.IN_CREATE, function(ev)
  print("Created:", ev.name)
end)
```

@*param* `pathname` — Path to watch (directory or file)

@*param* `mask` — Bitmask of inotify event flags

@*param* `callback` — Function called when event fires

@*return* `wd` — Watch descriptor or `nil` on error

@*return* `errmsg`

@*return* `errno`

## close


```lua
(method) Watchdog:close()
  -> integer?
  2. errmsg: string?
  3. errno: integer?
```

Close the inotify watcher and release all callbacks.

Usage:
```lua
wd:close()
```

@*return* — 0 on success, `nil` on error

@*return* `errmsg`

@*return* `errno`

## poll


```lua
(method) Watchdog:poll(timeout?: integer)
  -> integer?
  2. errmsg: string?
  3. errno: integer?
```

Poll for filesystem events and dispatch callbacks.

Blocks until events are available or `timeout` expires.

Usage:
```lua
local ok, errmsg, errno = wd:poll(500)
if not ok then error(errmsg) end
```

@*param* `timeout` — Timeout in milliseconds (-1 = infinite). Optional.

@*return* — Number of events processed or 0 on timeout

@*return* `errmsg`

@*return* `errno`

## remove


```lua
(method) Watchdog:remove(wd: integer)
  -> integer?
  2. errmsg: string?
  3. errno: integer?
```

Remove a watch from a given descriptor.

Usage:
```lua
local ok, errmsg, errno = wd:remove(wd_id)
if not ok then error(errmsg) end
```

@*param* `wd` — Watch descriptor previously returned by `add`

@*return* — 0 on success, `nil` on error

@*return* `errmsg`

@*return* `errno`


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
  -> Watchdog?
  2. errmsg: string?
  3. errno: integer?
```

Initialize a new inotify watcher.

Usage:
```lua
local wd, errmsg, errno = watchdog.init()
if not wd then error(errmsg) end
```

@*return* — Watchdog object or `nil` on error

@*return* `errmsg` — Error message if initialization failed

@*return* `errno` — System `errno` on failure