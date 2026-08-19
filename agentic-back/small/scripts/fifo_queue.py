from queue import Queue

def add_two(a, b):
    return a + b

class _FifoQueue():
    _queue = Queue()
