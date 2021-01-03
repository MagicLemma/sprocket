function Class(init)
   local c = {}

   -- the class is the metatable for all its objects
   c.__index = c
 
   -- expose a constructor which can be called by <classname>(<args>)
   local mt = {}
   mt.__call = function(class_tbl, ...)

      local obj = {}
      setmetatable(obj,c)
      init(obj,...)
      return obj
   end
   
   c.init = init
   c.is_a = function(self, klass)
      local m = getmetatable(self)
      while m do 
         if m == klass then return true end
         m = m._base
      end
      return false
   end
   setmetatable(c, mt)
   return c
end

Keyboard = {
   W = 87,
   A = 65,
   S = 83,
   D = 68,
   C = 67,
   Q = 81,
   E = 69,
   R = 82,
   F = 70,
   V = 86,
   X = 88,
   Y = 89,
   Z = 90,

   SPACE = 32,
   BACKSPACE = 259,
   LSHIFT = 340,
   ESC = 256,
   TAB = 258,
   ENTER = 257,

   KEYPAD_ENTER = 335,

   LEFT_ARROW = 263,
   RIGHT_ARROW = 262,
   UP_ARROW = 265,
   DOWN_ARROW = 264,

   PAGE_UP = 266,
   PAGE_DOWN = 267,

   HOME = 268,
   END = 269,
   INSERT = 260,
   DEL = 26,
}
