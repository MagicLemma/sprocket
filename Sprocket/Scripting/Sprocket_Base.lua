-- CLASS DEFINITION

function Class(init)
   local c = {}
   c.__index = c
 
   local metatable = {
      __call = function(class_tbl, ...)
         local obj = setmetatable({}, c)
         init(obj,...)
         return obj
      end
   }
   setmetatable(c, metatable)

   return c
end

-- KEY CODES

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

-- MATHS LIBRARY

Vec3 = Class(function(self, x, y, z)
   self.x = x
   self.y = y
   self.z = z
end)

function Vec3:__add(other)
   return Vec3(self.x + other.x, self.y + other.y, self.z + other.z)
end

function Vec3:__sub(other)
   return Vec3(self.x - other.x, self.y - other.y, self.z - other.z)
end

function Vec3:__mul(other)
   if type(self) == 'number' then
       return Vec3(self * other.x, self * other.y, self * other.z)
   elseif type(other) == 'number' then
       return Vec3(self.x * other, self.y * other, self.z * other)
   else
       return Vec3(self.x * other.x, self.y * other.y, self.z * other.z)
   end
end

function Mag(vector)
   return math.sqrt(Dot(vector, vector))
end

function Dot(a, b)
   return a.x * b.x + a.y * b.y + a.z * b.z
end

function Cross(a, b)
   return Vec3(
       a.y * b.z - a.z * b.y,
       a.z * b.x - a.x * b.z,
       a.x * b.y - a.y * b.x
   )
end

function Clamp(value, low, high)
   return math.min(high, math.max(value, low))
end

function Normalised(vector)
   local mag = Mag(vector)
   if mag == 0 then
       return Vec3(0, 0, 0)
   end
   return Vec3(vector.x / mag, vector.y / mag, vector.z / mag)
end

function Sin(x)
   return math.sin(x)
end