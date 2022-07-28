Vector2 = {}
Vector2.__index = Vector2

local function is_vector(t)
    return getmetatable(t) == Vector2
end

-- global utility function
function calculateRotation(targetDir)
    assert(is_vector(targetDir), "error: expected <Vector2> and <Vector2>")
    return math.atan2(targetDir.y, targetDir.x) * 180 / 3.14
end

---------------------
-- Vector constructor
---------------------
function Vector2.new(x, y)
    return setmetatable({x = x or 0, y = y or 0}, Vector2)
end


---------------------
-- Vector methods
---------------------
function Vector2:length()
    return math.sqrt(self.x * self.x + self.y * self.y)
end

function Vector2:normalize()
    local len = self:length()
    if len ~= 0 and len ~= 1 then
        self.x = self.x / len
        self.y = self.y / len
    end
end

function Vector2:dot(v)
    assert(is_vector(v), "error: expected <Vector2>")
    return self.x * v.x + self.y * v.y
end

function Vector2:isZero()
    return self.x == 0 and self.y == 0
end


---------------------
-- Operator overloading
---------------------
function Vector2.__add(lhs, rhs)
    assert(is_vector(lhs) and is_vector(rhs), "error: expected <Vector2> and <Vector2>")
    return Vector2.new(lhs.x + rhs.x, lhs.y + rhs.y)
end

function Vector2.__sub(lhs, rhs)
    assert(is_vector(lhs) and is_vector(rhs), "error: expected <Vector2> and <Vector2>")
    return Vector2.new(lhs.x - rhs.x, lhs.y - rhs.y)
end

function Vector2.__mul(lhs, rhs)
    if type(lhs) == 'number' and is_vector(rhs) then

        return Vector2.new(rhs.x * lhs, rhs.y * lhs)

    elseif type(rhs) == 'number' and is_vector(lhs) then

        return Vector2.new(lhs.x * rhs, lhs.y * rhs)

    elseif is_vector(rhs) and is_vector(lhs) then

        return Vector2.new(lhs.x * rhs.x, lhs.y * rhs.y)
    end
end

function Vector2:__tostring()
    return "(".. self.x ..", ".. self.y ..")"
end

function Vector2.__eq(lhs, rhs)
    return lhs.x == rhs.x and lhs.y == rhs.y
end

function Vector2.__lt(lhs, rhs)
    return lhs.x < rhs.x or (not (rhs.x < lhs.x) and lhs.y < rhs.y)
end

function Vector2.__le(lhs, rhs)
    return lhs.x <= rhs.x or lhs.y <= rhs.y
end