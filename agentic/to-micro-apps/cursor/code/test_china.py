# https://pypi.org/project/chinese/
# https://www.kaggle.com/datasets/ruddfawcett/hanzidb

from app.bot.hanzi_db import (
    HanziDB,
    Radicals
)

from app.bot.handlers_china import (
    create_chinese_text_image,
    create_chinese_text_image_radical
)

import matplotlib.pyplot as plt
import numpy as np
from PIL import Image, ImageDraw, ImageFont
import os

import pandas as pd
import jieba

import srt

def config_jieba():
    try:
        jieba.del_word('多来')
    except:
        pass
    try:
        jieba.del_word('多來')
    except:
        pass

class Srt(object):
    def __init__(self, example_fn, hanzi_db: HanziDB):
        radical_to_data = {}

        config_jieba()

        unique_bigrams = {}
        single_to_pairs = {}

        try:
            with open(example_fn, 'r', encoding='utf-8') as f:
                # Parse directly from the file object
                subtitles = list(srt.parse(f))

                for s in subtitles:
                    content = s.content
                    words = jieba.cut(content, cut_all=False)
                    for word in words:
                        if len(word) == 2:
                            e1 = word[0]
                            e2 = word[1]
                            if word not in unique_bigrams:
                                unique_bigrams[word] = []
                                fr1 = 10000
                                fr2 = 10000
                                v1 = hanzi_db.get_if_have(e1)
                                if v1:
                                    fr1 = v1.frequency_rank

                                v2 = hanzi_db.get_if_have(e2)
                                if v2:
                                    fr2 = v2.frequency_rank

                                fr = min(fr1, fr2)

                                print(fr, word)

                            unique_bigrams[word].append(content)

                            if e1 not in single_to_pairs:
                                single_to_pairs[e1] = []

                            single_to_pairs[e1].append(word)

                            if e2 not in single_to_pairs:
                                single_to_pairs[e2] = []
                            
                            single_to_pairs[e2].append(word)

            #print(single_to_pairs)
        except FileNotFoundError as e:
            print(f"Error: {e}")
            raise


def main():
    rls = Radicals("app/backend/app/app/materials/radicals_fixed.csv")
    hanzi_db = HanziDB(rls)

    df = hanzi_db.get_all()
    for row in df.itertuples():
        print(f"Value: {row.frequency_rank} charcter: {row.charcter} pinyin: {row.pinyin} definition: {row.definition}, radical: {row.radical}")
        break

    value = hanzi_db.get_random()
    print(value)

    # Example usage
    create_chinese_text_image(f"{value.charcter}/{value.radical}", "china.png",
            size=(200, 240), font_size=60)
    
    # Radicals
    radicals = hanzi_db.get_radicals()

    value, examples = hanzi_db.get_random_radical()

    create_chinese_text_image_radical(value, examples, "china.png",
            size=(240, 200), font_size=60)

    # for i, r in enumerate(radicals):
    #     print(f'{i}, {r}, english translation, class')
    #     break

    # Matcher
    v = rls.get_by_radical(value.radical)
    print(v)

    if pd.isna(v.form_in_char):
        print("Naaan!")



    # # Splitter
    # text = "我爱学习中文，今天天气很好。"
    # words = jieba.cut(text, cut_all=False)
    # print("/ ".join(words))

    # Srt
    example_fn = "./app/backend/app/app/materials/罗小黑战记二 2025.srt"
    srt = Srt(example_fn=example_fn, hanzi_db=hanzi_db)

    

if __name__ == '__main__':
    main()