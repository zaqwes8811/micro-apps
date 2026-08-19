import os
import sys

import pytest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from hanzi_db import Cache


def test_first_insert_returns_true():
    cache = Cache(max_count=2)
    assert cache.insert("a") is True


def test_repeat_insert_returns_false():
    cache = Cache(max_count=2)
    cache.insert("a")
    assert cache.insert("a") is False


def test_increments_existing_keys_on_insert():
    cache = Cache(max_count=5)
    cache.insert("a")
    cache.insert("b")
    assert cache.counters_["a"] == 1
    assert cache.counters_["b"] == 0


def test_evicts_key_when_counter_exceeds_max():
    cache = Cache(max_count=2)
    cache.insert("a")
    cache.insert("b")
    cache.insert("c")
    cache.insert("d")
    assert "a" not in cache.counters_
    assert cache.counters_["b"] == 2
    assert cache.counters_["c"] == 1
    assert cache.counters_["d"] == 0


def test_existing_key_can_be_evicted_on_repeat_insert():
    cache = Cache(max_count=1)
    cache.insert("a")
    cache.insert("b")
    assert cache.insert("a") is False
    assert "a" not in cache.counters_
