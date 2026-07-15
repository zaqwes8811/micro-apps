import unittest
from src.Vec4 import Vec4

class TestVec4(unittest.TestCase):
    def test_length(self):
        v = Vec4(3, 4, 0, 0)
        self.assertEqual(v.length(), 5)

    def test_add(self):
        v1 = Vec4(1, 2, 3, 4)
        v2 = Vec4(5, 6, 7, 8)
        expected = Vec4(6, 8, 10, 12)
        self.assertEqual(v1.add(v2), expected)

    def test_subtract(self):
        v1 = Vec4(1, 2, 3, 4)
        v2 = Vec4(5, 6, 7, 8)
        expected = Vec4(-4, -4, -4, -4)
        self.assertEqual(v1.subtract(v2), expected)

    def test_multiply(self):
        v = Vec4(3, 4, 0, 0)
        scalar = 5
        expected = Vec4(15, 20, 0, 0)
        self.assertEqual(v.multiply(scalar), expected)

    def test_divide(self):
        v = Vec4(6, 8, 0, 0)
        scalar = 2
        expected = Vec4(3, 4, 0, 0)
        self.assertEqual(v.divide(scalar), expected)

if __name__ == '__main__':
    unittest.main()
