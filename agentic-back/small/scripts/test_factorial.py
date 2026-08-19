from factorial import factorial

def test_factorial():
    assert factorial(5) == 120
    assert factorial(0) == 1

if __name__ == '__main__':
    test_factorial()