import kagglehub
import pandas as pd

import numpy as np

from typing import Set

CACHE_MAX_ATTEMPTS = 20


class Cache(object):
    def __init__(self, max_count):
        self.max_count_ = max_count
        self.counters_ = {}

    def insert(self, key):
        is_first = key not in self.counters_

        for existing_key in list(self.counters_.keys()):
            self.counters_[existing_key] += 1
            if self.counters_[existing_key] > self.max_count_:
                del self.counters_[existing_key]

        if is_first:
            self.counters_[key] = 0
            return True

        return False


class Radicals(object):
    def __init__(self, filename):
        radical_to_data = {}

        try:
            self.df_ = pd.read_csv(filename,
                    na_values=['NA', 'N/A', 'missing'],
                    encoding='utf-8-sig',
                    header=0)

            for row in self.df_.itertuples():
                assert row.symbol not in radical_to_data
                radical_to_data[row.symbol] = row
        except FileNotFoundError as e:
            print(f"Error: {e}")

        self.radical_to_data_ = radical_to_data

    def get_by_radical(self, radical):
        if radical not in self.radical_to_data_:
            print(f"{self.__str__()}: Value: {radical}")
            return None
        
        return self.radical_to_data_[radical]


class HanziDBRadicalDownLink(object):
    def __init__(self):
        self.counter_ = 0

class HanziDB(object):
    def __init__(self, rls, luoxiaohei_words_path, cache_max_count=10):
        dirty_start_idx = 100  # For db first 100 lines have dublicates
        active_range = 1000
    
        path = kagglehub.dataset_download("ruddfawcett/hanzidb")

        hanzi_file_path = f'{path}/hanziDB.csv'

        print("Path to dataset files:", hanzi_file_path)

        # hanzi_file_path = f"{hanzidb_path}/dataSources/hanzi.csv"
        df = pd.read_csv(hanzi_file_path,
                na_values=['NA', 'N/A', 'missing'])
        
        self.df_orig_ = df
        self.df_orig_cut_ = self.df_orig_.iloc[dirty_start_idx:].copy()

        # Crop
        df = df.iloc[dirty_start_idx:dirty_start_idx + active_range].copy()
        df_100 = self.df_orig_.iloc[:dirty_start_idx].copy()

        # Norm
        self.df_ = self.__norm(df)
        self.df_100_ = self.__norm(df_100)
        self.rls_ = rls

        self.acc_rad_ = self.get_radicals()

        self.df_radicals_index_ = self.get_radicals_with_freq()

        df_luoxiaohei = pd.read_csv(luoxiaohei_words_path,
                na_values=['NA', 'N/A', 'missing'],
                encoding='utf-8-sig')
        self.df_luoxiaohei_ = self.__norm(df_luoxiaohei)

        self.cache_ = Cache(cache_max_count)
        self.cache_100_ = Cache(cache_max_count)
        self.cache_radical_ = Cache(cache_max_count)
        self.cache_luoxiaohei_ = Cache(cache_max_count)

    def __norm(self, df):
        # Zipf's law
        df['weight'] = 1 / df['frequency_rank']
        df['probability'] = df['weight'] / df['weight'].sum()
        df['row_number'] = range(1, len(df) + 1)

        return df

    def get_all(self):
        return self.df_
    
    def get_radicals_with_freq(self):
        df = self.__norm(self.df_orig_cut_)
        results = []
        results_norm = []
        total = 0

        for k, v in self.acc_rad_.items():
            probability_sum = 0

            indexes = ""
            for i in v:
                row = df.loc[i]
                freq = row.probability
                if k == row.radical:
                    probability_sum += freq

                    indexes += f",{i}"
            results.append((probability_sum, k, indexes[1:]))
            total += probability_sum

        for probability_sum, k, indexes in results:
            normalized = probability_sum / total
            results_norm.append([normalized, k, indexes])

        df_radicals = pd.DataFrame(results_norm, columns=['probability', 'radical', 'indexes'])
        df_radicals['row_number'] = range(1, len(df_radicals) + 1)

        return df_radicals
    
    def get_radicals(self):
        result = {}
        df = self.df_orig_cut_
        for row in df.itertuples():
            if not row.radical in result:
                result[row.radical] = []
            result[row.radical].append(row.Index)

        return result

    def __get_random(self, df):
        random_index = np.random.choice(
            df.index, 
            p=df['probability']
        )
        return df.loc[random_index]
    
    def __get_random_cached(self, df, cache, key_field):
        row = None
        for _ in range(CACHE_MAX_ATTEMPTS):
            row = self.__get_random(df)
            if cache.insert(getattr(row, key_field)):
                return row
        return row

    def __build_radical_examples(self, result, n=3):
        indexes = list(map(int, result.indexes.split(',')))
        indexes = np.array(indexes)

        np.random.shuffle(indexes)

        n = min(n, len(indexes))

        indexes = indexes[:n]

        examples = []

        for idx in indexes:
            examples.append(self.df_orig_cut_.loc[idx].charcter)

        return examples

    def get_random(self):
        return self.__get_random_cached(self.df_, self.cache_, "charcter")

    def get_random_100(self):
        return self.__get_random_cached(self.df_100_, self.cache_100_, "charcter")

    def get_random_luoxiaohei(self):
        return self.__get_random_cached(self.df_luoxiaohei_, self.cache_luoxiaohei_, "characters")

    def get_random_radical(self):
        result = None
        for _ in range(CACHE_MAX_ATTEMPTS):
            result = self.__get_random(self.df_radicals_index_)
            if self.cache_radical_.insert(result.radical):
                return result, self.__build_radical_examples(result)
        return result, self.__build_radical_examples(result)
    
    def get_if_have(self, symbol):
        df = self.df_orig_cut_
        for row in df.itertuples():
            if row.charcter == symbol:
                return row
            
        return None
    