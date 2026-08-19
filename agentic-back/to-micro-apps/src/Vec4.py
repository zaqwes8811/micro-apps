class Vec4:
    def __init__(self, x, y, z, w):
        self.x = x
        self.y = y
        self.z = z
        self.w = w

    def length(self):
        return (self.x ** 2 + self.y ** 2 + self.z ** 2 + self.w ** 2) ** 0.5

    def add(self, other):
        return Vec4(self.x + other.x, self.y + other.y, self.z + other.z, self.w + other.w)

    def subtract(self, other):
        return Vec4(self.x - other.x, self.y - other.y, self.z - other.z, self.w - other.w)

    def multiply(self, scalar):
        return Vec4(self.x * scalar, self.y * scalar, self.z * scalar, self.w * scalar)

    def divide(self, scalar):
        return Vec4(self.x / scalar, self.y / scalar, self.z / scalar, self.w / scalar)

    def abs(self):
        return Vec4(abs(self.x), abs(self.y), abs(self.z), abs(self.w))
