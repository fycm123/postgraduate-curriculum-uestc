# coding: utf-8
"""
Created on Mon Mar  10 19:33:40 2019
@author: 杨庆
"""

import os, time, random
import numpy as np

s = time.time()

import math
import re

import jieba
import pandas as pd

def get_content(path):
    """
    根据邮件路径提取邮件中的文本数据
    :param path: 邮件路径
    :return content: 邮件文本
    """
    # 邮件数据是 gbk 编码, 忽略无法解码的内容
    with open(path, 'r', encoding='gbk', errors='ignore') as f:
        lines = f.readlines()
    for i in range(len(lines)):
        if lines[i] == '\n':
            # 去除空行
            lines = lines[i:]
            break
    content = ''.join(''.join(lines).strip().split())
    return content

def create_string_word_dict(string, stop_list):
    """
    根据文本内容，创建这条文本的词典
    :param string: 字符串
    :param stop_list: 停用词列表
    :return
    """
    string = re.findall(u'[\u4E00-\u9FD5]', string)
    string = ''.join(string)
    
    word_list = []

    # 结巴分词
    seg_list = jieba.cut(string)
    for word in seg_list:
        if word != '' and word not in stop_list:
            word_list.append(word)
    word_dict = dict([(word, 1) for word in word_list])
    return word_dict

def load_stop_list():
    """
    加载停用词列表
    """
    # 加载停用词列表
    with open('stop', 'r') as f:
        lines = f.readlines()
    stop_list = [i.strip() for i in lines]
    return stop_list

def load_data():
    """
    加载数据集
    """
    # 加载数据集
    df = pd.read_csv('full/index', sep=' ', names=['spam', 'path'])
    df.spam = df.spam.apply(lambda x: 1 if x == 'spam' else 0)
    df.path = df.path.apply(lambda x: x[1:])
    return df

def train(df):
    """
    训练函数，产生分类器所需的参数
    :param df: dataframe
    :return train_word_dict: 词典
    """
    train_word_dict = {}
    for word_dict, spam in zip(df.word_dict, df.spam):
        # 统计测试邮件中词语在垃圾邮件和正常邮件中出现的总数
        for w in word_dict:
            train_word_dict.setdefault(w, {0: 0, 1: 0})
            train_word_dict[w][spam] += 1
    ham_count = df.spam.value_counts()[0]
    spam_count = df.spam.value_counts()[1]
    return train_word_dict, spam_count, ham_count

def predict(train_word_dict, spam_count, ham_count, row):
    """
    预测函数
    :param train_word_dict: 训练函数产生的统计词典
    :param spam_count: 垃圾邮件数
    :param ham_count: 正常邮件数
    :param row: 一行测试邮件数据
    """
    total_count = ham_count + spam_count
    word_dict = row['word_dict']

    # 正常邮件概率
    hp = math.log(float(ham_count) / total_count)
    # 垃圾邮件概率
    sp = math.log(float(spam_count) / total_count)

    for w in word_dict:
        w = w.strip()
        # 给该词在词典中设定一个默认数0
        train_word_dict.setdefault(w, {0: 0, 1: 0})

        # 该词在词典中正常邮件出现的次数
        pih = train_word_dict[w][0]
        # 平滑处理, 每个词汇基数+1，正常邮件数+2
        hp += math.log((float(pih) + 1) / (ham_count + 2))
        
        pis = train_word_dict[w][1]
        sp += math.log((float(pis) + 1) / (spam_count + 2))
    # 预测结果
    predict_spam = 1 if sp > hp else 0
    # 返回预测是否准确，用于统计
    return 1 if predict_spam == row['spam'] else 0

if __name__ == '__main__':
    # 加载邮件数据
    df = load_data()
    print('读取数据完毕， 用时：{}'.format(time.time() - s))

    # 加载停用词列表
    stop_list = load_stop_list()
    print('加载停用词列表完毕， 用时：{}'.format(time.time() - s))

    # 提取邮件文本内容
    df['content'] = df.path.apply(lambda x: get_content(x))
    print('提取邮件文本内容完毕， 用时：{}'.format(time.time() - s))

    # 创建邮件字典
    df['word_dict'] = df.content.apply(lambda x: create_string_word_dict(x, stop_list))
    print('创建邮件字典完毕， 用时：{}'.format(time.time() - s))

    # 产生训练集与测试集
    train_mails = df.loc[:len(df) * 0.98]
    test_mails = df.loc[len(df) * 0.98:]
    print('产生训练集与测试集完毕， 用时：{}'.format(time.time() - s))

    # 训练
    train_word_dict, spam_count, ham_count = train(train_mails)
    print('训练完毕， 用时：{}'.format(time.time() - s))

    # 预测
    test_mails_predict = test_mails.apply(
        lambda x: predict(train_word_dict, spam_count, ham_count, x), axis=1)

    result = 1 - float(test_mails_predict.value_counts().get(0, 0)) / test_mails_predict.value_counts().sum()

    print('总用时：{}'.format(time.time()-s))
    print('预测准确率为：', result)
