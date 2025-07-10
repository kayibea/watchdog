# 🐾 watchdog

**`watchdog`** is a fast and minimal Lua module (written in C) that wraps the Linux `inotify` API

---

## 📦 Features

- Thin native wrapper around `inotify`
- Callback-based design (no busy loops)
- Object-like instances for multiple watchers
- Fully non-blocking `poll()` support
- Includes all `inotify` constants

---

## 🔧 Installation

Via [LuaRocks](https://luarocks.org/modules/gloirekiba/watchdog):

```bash
luarocks install watchdog
```

```lua
local signal = require("posix.signal")
local watchdog = require("watchdog")

local running = true

signal.signal(signal.SIGINT, function()
  print("\nShutting down...")
  running = false
end)

local wd = watchdog.init()

wd:add("/tmp", watchdog.IN_CREATE | watchdog.IN_DELETE, function(ev)
  print("Event:", ev.name, ev.mask)
end)

while running do
  wd:poll()
end

wd:close()
```

## 🧪 Testing

Run tests with Busted:

```bash
luarocks test
```
The test suite will create and remove temporary files in `/tmp`.

## 📄 License
MIT License — do whatever you want.