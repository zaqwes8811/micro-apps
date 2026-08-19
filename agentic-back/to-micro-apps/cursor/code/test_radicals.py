import os
import sys

import pytest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from hanzi_db import Radicals

CSV_PATH = os.path.join(
    os.path.dirname(os.path.abspath(__file__)),
    "..", "materials", "radicals_fixed.csv",
)


@pytest.fixture(scope="module")
def radicals():
    return Radicals(CSV_PATH)


def test_loads_all_radicals(radicals):
    assert len(radicals.radical_to_data_) == 214


def test_expected_columns(radicals):
    expected = {"n", "new_symbol", "translation", "type", "form_in_char", "pinyin", "symbol"}
    assert expected.issubset(set(radicals.df_.columns))


def test_get_known_radical(radicals):
    row = radicals.get_by_radical("火")
    assert row is not None
    assert row.pinyin == "huǒ"
    assert "огонь" in row.translation


def test_traditional_symbol_maps_to_simplified(radicals):
    # The dict is keyed on the (traditional) `symbol` column.
    row = radicals.get_by_radical("黃")
    assert row is not None
    assert row.new_symbol == "黄"


def test_unchanged_radical_keeps_symbol(radicals):
    row = radicals.get_by_radical("自")
    assert row is not None
    assert row.new_symbol == row.symbol == "自"


def test_unknown_radical_returns_none(radicals):
    assert radicals.get_by_radical("notaradical") is None


def test_missing_file_is_handled():
    rls = Radicals("does_not_exist.csv")
    assert rls.radical_to_data_ == {}
