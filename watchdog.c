#define _GNU_SOURCE
#include "watchdog.h"

#include <errno.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>

#ifndef LUA_OK
#define LUA_OK 0
#endif

#define WATCHDOG_MT "watchdog"
#define EVENT_BUF_LEN (1024 * (sizeof(struct inotify_event) + 256))

typedef struct {
  int fd;
  int cb_table_ref;
} Watchdog;

static int handle_error(lua_State *L) {
  lua_pushnil(L);
  lua_pushstring(L, strerror(errno));
  lua_pushinteger(L, errno);
  return 3;
}

static void push_event(lua_State *L, struct inotify_event *ev) {
  lua_newtable(L);

  lua_pushstring(L, "wd");
  lua_pushinteger(L, ev->wd);
  lua_settable(L, -3);

  lua_pushstring(L, "mask");
  lua_pushinteger(L, ev->mask);
  lua_settable(L, -3);

  lua_pushstring(L, "cookie");
  lua_pushinteger(L, ev->cookie);
  lua_settable(L, -3);

  lua_pushstring(L, "name");
  lua_pushstring(L, (ev->len > 0) ? ev->name : "");
  lua_settable(L, -3);
}

static int l_add(lua_State *L) {
  Watchdog *wd = luaL_checkudata(L, 1, WATCHDOG_MT);
  const char *path = luaL_checkstring(L, 2);
  uint32_t mask = luaL_checkinteger(L, 3);
  luaL_checktype(L, 4, LUA_TFUNCTION);

  int watch_fd = inotify_add_watch(wd->fd, path, mask);
  if (watch_fd < 0) return handle_error(L);

  lua_rawgeti(L, LUA_REGISTRYINDEX, wd->cb_table_ref);
  lua_pushvalue(L, 4);
  lua_rawseti(L, -2, watch_fd);
  lua_pop(L, 1);

  lua_pushinteger(L, watch_fd);
  return 1;
}

static int l_remove(lua_State *L) {
  Watchdog *wd = luaL_checkudata(L, 1, WATCHDOG_MT);
  int watch_fd = luaL_checkinteger(L, 2);

  if (inotify_rm_watch(wd->fd, watch_fd) < 0) return handle_error(L);

  lua_rawgeti(L, LUA_REGISTRYINDEX, wd->cb_table_ref);
  lua_pushnil(L);
  lua_rawseti(L, -2, watch_fd);
  lua_pop(L, 1);

  lua_pushinteger(L, 0);
  return 1;
}

static int l_poll(lua_State *L) {
  Watchdog *wd = luaL_checkudata(L, 1, WATCHDOG_MT);
  int timeout = luaL_optinteger(L, 2, -1);

  struct pollfd pfd = {
      .fd = wd->fd,
      .events = POLLIN,
  };

  int ret = poll(&pfd, 1, timeout);
  if (ret < 0) return handle_error(L);
  if (ret == 0) {
    lua_pushinteger(L, ret);
    return 1;
  }

  char buf[EVENT_BUF_LEN];
  ssize_t len = read(wd->fd, buf, sizeof(buf));
  if (len < 0) return handle_error(L);
  if (len == 0) {
    lua_pushinteger(L, len);
    return 1;
  };

  lua_rawgeti(L, LUA_REGISTRYINDEX, wd->cb_table_ref);

  ssize_t i = 0;
  while (i < len) {
    struct inotify_event *ev = (struct inotify_event *)(buf + i);

    lua_rawgeti(L, -1, ev->wd);
    if (lua_isfunction(L, -1)) {
      push_event(L, ev);
      if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
        fprintf(stderr, "Callback error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
      }
    } else {
      lua_pop(L, 1);
    }

    i += sizeof(struct inotify_event) + ev->len;
  }

  lua_pop(L, 1);
  lua_pushinteger(L, ret);
  return 1;
}

static int l_close(lua_State *L) {
  Watchdog *wd = luaL_checkudata(L, 1, WATCHDOG_MT);
  int ret = 0;
  if (wd->fd >= 0) {
    luaL_unref(L, LUA_REGISTRYINDEX, wd->cb_table_ref);
    if ((ret = close(wd->fd)) < 0) return handle_error(L);
    wd->fd = -1;
  }
  lua_pushinteger(L, ret);
  return 1;
}

static int l_gc(lua_State *L) { return l_close(L); }

static int l_init(lua_State *L) {
  int fd = inotify_init1(IN_NONBLOCK);
  if (fd < 0) return handle_error(L);

  Watchdog *wd = lua_newuserdata(L, sizeof(Watchdog));
  wd->fd = fd;

  luaL_getmetatable(L, WATCHDOG_MT);
  lua_setmetatable(L, -2);

  lua_newtable(L);
  wd->cb_table_ref = luaL_ref(L, LUA_REGISTRYINDEX);

  return 1;
}

static const luaL_Reg methods[] = {{"add", l_add},   {"remove", l_remove},
                                   {"poll", l_poll}, {"close", l_close},
                                   {"__gc", l_gc},   {NULL, NULL}};

static const luaL_Reg lib[] = {{"init", l_init}, {NULL, NULL}};

#define ADD_CONST(L, name)  \
  lua_pushinteger(L, name); \
  lua_setfield(L, -2, #name);

int luaopen_watchdog(lua_State *L) {
  luaL_newmetatable(L, WATCHDOG_MT);
#if LUA_VERSION_NUM > 501
  luaL_setfuncs(L, methods, 0);
#else
  luaL_register(L, NULL, methods);
#endif
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_pop(L, 1);

  lua_newtable(L);
#if LUA_VERSION_NUM > 501
  luaL_setfuncs(L, lib, 0);
#else
  luaL_register(L, NULL, lib);
#endif

  ADD_CONST(L, IN_ACCESS);
  ADD_CONST(L, IN_MODIFY);
  ADD_CONST(L, IN_ATTRIB);
  ADD_CONST(L, IN_CLOSE_WRITE);
  ADD_CONST(L, IN_CLOSE_NOWRITE);
  ADD_CONST(L, IN_OPEN);
  ADD_CONST(L, IN_MOVED_FROM);
  ADD_CONST(L, IN_MOVED_TO);
  ADD_CONST(L, IN_CREATE);
  ADD_CONST(L, IN_DELETE);
  ADD_CONST(L, IN_DELETE_SELF);
  ADD_CONST(L, IN_MOVE_SELF);
  ADD_CONST(L, IN_UNMOUNT);
  ADD_CONST(L, IN_Q_OVERFLOW);
  ADD_CONST(L, IN_IGNORED);
  ADD_CONST(L, IN_ONLYDIR);
  ADD_CONST(L, IN_DONT_FOLLOW);
  ADD_CONST(L, IN_EXCL_UNLINK);
  ADD_CONST(L, IN_MASK_ADD);
  ADD_CONST(L, IN_ISDIR);
  ADD_CONST(L, IN_ONESHOT);
  ADD_CONST(L, IN_CLOSE);
  ADD_CONST(L, IN_MOVE);
  ADD_CONST(L, IN_ALL_EVENTS);

  return 1;
}
