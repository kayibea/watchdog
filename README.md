# 🐾 watchdog

**`watchdog`** is a fast and minimal Lua module (written in C) that wraps the Linux `inotify` API, providing filesystem monitoring with Lua callbacks. Designed for performance and ease of use — ideal for file watchers, and reactive systems.

---

## 📦 Features

- Thin native wrapper around `inotify`
- Callback-based design (no busy loops)
- Object-like instances for multiple watchers
- Fully non-blocking `poll()` support
- Includes all `inotify` constants
- Simple API — clean and minimal

---

## 🔧 Installation

Via [LuaRocks](https://luarocks.org/modules/your-name/watchdog):

```bash
luarocks install watchdog
```

```lua
local posix = require("posix")
local watchdog = require("watchdog")

local wd = watchdog.init()

wd:add("/tmp", watchdog.IN_CREATE | watchdog.IN_DELETE, function(ev)
  print("Event:", ev.name, ev.mask)
end)

local running = true
local signal = require("posix.signal")

signal.signal(signal.SIGINT, function()
  print("\nShutting down...")
  running = false
end)

while running do
  wd:poll()
  posix.sleep(1)
end

wd:close()
```

## 🧪 Testing

Run tests with Busted:

```bash
luarocks test
```
The test suite will create and remove temporary files in `/tmp`.


## 🐧 Requirements
Linux (inotify is Linux-specific)

Lua 5.1–5.4 or LuaJIT

C compiler (for building the module)

## 📄 License
MIT License — do whatever you want.