import os
import sys

import numpy as np
import pytest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import hanzi_db
from hanzi_db import CACHE_MAX_ATTEMPTS, Cache, HanziDB, Radicals

BASE = os.path.dirname(os.path.abspath(__file__))
MATERIALS = os.path.join(BASE, "..", "materials")
RADICALS_CSV = os.path.join(MATERIALS, "radicals_fixed.csv")
LUOXIAOHEI_CSV = os.path.join(MATERIALS, "luoxiaohei_words.csv")

ACTIVE_RANGE = 1000
DIRTY_START = 100


@pytest.fixture(scope="module")
def hanzi():
    # HanziDB downloads the dataset via kagglehub; redirect it to the local
    # materials folder which contains hanziDB.csv, so the test needs no network.
    hanzi_db.kagglehub.dataset_download = lambda *a, **k: MATERIALS
    np.random.seed(0)
    rls = Radicals(RADICALS_CSV)
    return HanziDB(rls, LUOXIAOHEI_CSV)


def test_active_range_size(hanzi):
    assert len(hanzi.get_all()) == ACTIVE_RANGE


def test_df_100_size(hanzi):
    assert len(hanzi.df_100_) == DIRTY_START


def test_norm_columns_present(hanzi):
    for col in ("weight", "probability", "row_number"):
        assert col in hanzi.get_all().columns


def test_probability_normalized(hanzi):
    assert hanzi.get_all()["probability"].sum() == pytest.approx(1.0)


def test_get_random_in_range(hanzi):
    row = hanzi.get_random()
    assert row.name in hanzi.get_all().index
    assert isinstance(row.charcter, str)


def test_get_random_100_in_range(hanzi):
    row = hanzi.get_random_100()
    assert row.name in hanzi.df_100_.index


def test_hanzi_has_caches(hanzi):
    assert hasattr(hanzi, "cache_")
    assert hasattr(hanzi, "cache_100_")
    assert hasattr(hanzi, "cache_radical_")
    assert hasattr(hanzi, "cache_luoxiaohei_")
    assert hanzi.cache_ is not hanzi.cache_100_
    assert hanzi.cache_radical_ is not hanzi.cache_
    assert hanzi.cache_luoxiaohei_ is not hanzi.cache_100_


def test_luoxiaohei_loaded(hanzi):
    assert len(hanzi.df_luoxiaohei_) == 605
    for col in ("characters", "pronunciation", "translation", "frequency", "old_version", "frequency_rank"):
        assert col in hanzi.df_luoxiaohei_.columns


def test_luoxiaohei_probability_normalized(hanzi):
    assert hanzi.df_luoxiaohei_["probability"].sum() == pytest.approx(1.0)


def test_get_random_luoxiaohei_in_range(hanzi):
    row = hanzi.get_random_luoxiaohei()
    assert row.name in hanzi.df_luoxiaohei_.index
    assert isinstance(row.characters, str)
    assert len(row.characters) == 2


def test_get_random_luoxiaohei_skips_cached_character(monkeypatch, hanzi):
    first = hanzi.df_luoxiaohei_.iloc[0]
    second = hanzi.df_luoxiaohei_.iloc[1]
    calls = iter([first, first, second])

    def fake_get_random(df):
        return next(calls)

    monkeypatch.setattr(hanzi, "_HanziDB__get_random", fake_get_random)
    hanzi.cache_luoxiaohei_.insert(first.characters)

    row = hanzi.get_random_luoxiaohei()
    assert row.characters == second.characters


def test_get_random_luoxiaohei_limited_to_20_attempts(monkeypatch, hanzi):
    hanzi.cache_luoxiaohei_ = Cache(100)
    first = hanzi.df_luoxiaohei_.iloc[0]
    attempts = []

    def fake_get_random(df):
        attempts.append(1)
        return first

    monkeypatch.setattr(hanzi, "_HanziDB__get_random", fake_get_random)
    hanzi.cache_luoxiaohei_.insert(first.characters)

    row = hanzi.get_random_luoxiaohei()
    assert len(attempts) == CACHE_MAX_ATTEMPTS
    assert row.characters == first.characters


def test_get_random_100_skips_cached_character(monkeypatch, hanzi):
    first = hanzi.df_100_.iloc[0]
    second = hanzi.df_100_.iloc[1]
    calls = iter([first, first, second])

    def fake_get_random(df):
        return next(calls)

    monkeypatch.setattr(hanzi, "_HanziDB__get_random", fake_get_random)
    hanzi.cache_100_.insert(first.charcter)

    row = hanzi.get_random_100()
    assert row.charcter == second.charcter


def test_get_random_100_limited_to_20_attempts(monkeypatch, hanzi):
    hanzi.cache_100_ = Cache(100)
    first = hanzi.df_100_.iloc[0]
    attempts = []

    def fake_get_random(df):
        attempts.append(1)
        return first

    monkeypatch.setattr(hanzi, "_HanziDB__get_random", fake_get_random)
    hanzi.cache_100_.insert(first.charcter)

    row = hanzi.get_random_100()
    assert len(attempts) == CACHE_MAX_ATTEMPTS
    assert row.charcter == first.charcter


def test_get_random_skips_cached_character(monkeypatch, hanzi):
    first = hanzi.get_all().iloc[0]
    second = hanzi.get_all().iloc[1]
    calls = iter([first, first, second])

    def fake_get_random(df):
        return next(calls)

    monkeypatch.setattr(hanzi, "_HanziDB__get_random", fake_get_random)
    hanzi.cache_.insert(first.charcter)

    row = hanzi.get_random()
    assert row.charcter == second.charcter


def test_get_random_limited_to_max_attempts(monkeypatch, hanzi):
    hanzi.cache_ = Cache(100)
    first = hanzi.get_all().iloc[0]
    attempts = []

    def fake_get_random(df):
        attempts.append(1)
        return first

    monkeypatch.setattr(hanzi, "_HanziDB__get_random", fake_get_random)
    hanzi.cache_.insert(first.charcter)

    row = hanzi.get_random()
    assert len(attempts) == CACHE_MAX_ATTEMPTS
    assert row.charcter == first.charcter


def test_get_if_have_found(hanzi):
    sym = hanzi.df_orig_cut_.iloc[0].charcter
    found = hanzi.get_if_have(sym)
    assert found is not None
    assert found.charcter == sym


def test_get_if_have_missing(hanzi):
    assert hanzi.get_if_have("zzz-not-a-hanzi") is None


def test_get_radicals_structure(hanzi):
    radicals = hanzi.get_radicals()
    assert isinstance(radicals, dict)
    assert len(radicals) > 0
    sample_key = next(iter(radicals))
    assert isinstance(radicals[sample_key], list)
    assert len(radicals[sample_key]) > 0


def test_radicals_index_normalized(hanzi):
    df = hanzi.df_radicals_index_
    assert df["probability"].sum() == pytest.approx(1.0)
    assert {"probability", "radical", "indexes"}.issubset(set(df.columns))


def test_get_random_radical(hanzi):
    result, examples = hanzi.get_random_radical()
    assert isinstance(examples, list)
    assert 1 <= len(examples) <= 3
    assert all(isinstance(e, str) for e in examples)


def test_get_random_radical_skips_cached_radical(monkeypatch, hanzi):
    first = hanzi.df_radicals_index_.iloc[0]
    second = hanzi.df_radicals_index_.iloc[1]
    calls = iter([first, first, second])

    def fake_get_random(df):
        return next(calls)

    monkeypatch.setattr(hanzi, "_HanziDB__get_random", fake_get_random)
    hanzi.cache_radical_.insert(first.radical)

    result, examples = hanzi.get_random_radical()
    assert result.radical == second.radical
    assert isinstance(examples, list)


def test_get_random_radical_limited_to_max_attempts(monkeypatch, hanzi):
    hanzi.cache_radical_ = Cache(100)
    first = hanzi.df_radicals_index_.iloc[0]
    attempts = []

    def fake_get_random(df):
        attempts.append(1)
        return first

    monkeypatch.setattr(hanzi, "_HanziDB__get_random", fake_get_random)
    hanzi.cache_radical_.insert(first.radical)

    result, examples = hanzi.get_random_radical()
    assert len(attempts) == CACHE_MAX_ATTEMPTS
    assert result.radical == first.radical
    assert isinstance(examples, list)
