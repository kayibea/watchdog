rockspec_format = "3.0"
package = "watchdog"
version = "0.1.0-1"
source = {
  url = "git://github.com/kayibea/watchdog.git"
}
description = {
  summary = "A fast and minimal Lua module (written in C) that wraps the Linux `inotify` API.",
  detailed = [[
A fast and minimal Lua module (written in C) that wraps the Linux `inotify` API.
]],
  homepage = "https://github.com/kayibea/watchdog",
  license = "MIT",
  issues_url = "https://github.com/kayibea/watchdog/issues",
  labels = {
    "lua",
    "inotify"
  }
}
supported_platforms = {
  "linux"
}
dependencies = {
  "lua >= 5.1"
}
test_dependencies = {
  "bit32 >= 5.3.5.1-1",
  "busted >= 2.2.0-1",
  "penlight >= 1.14.0-3"
}
build = {
  type = "builtin",
  modules = {
    watchdog = {
      sources = "watchdog.c",
      incdirs = { "include" }
    }
  }
}
