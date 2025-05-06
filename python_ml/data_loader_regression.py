# python_ml/data_loader_regression.py

# For each time i, we take the window previous prices and aim to predict the price at i+window (i.e., the next day’s open/close).

import numpy as np
import pandas as pd

def load_regression_windows(csv_path: str, window: int = 20):
    """
    Reads a CSV with column 'price' and returns:
      X: shape (N, window) of past prices
      y: shape (N,) next-day price
    """
    df = pd.read_csv(csv_path)
    prices = df['price'].values
    X, y = [], []
    for i in range(len(prices) - window - 1):
        X.append(prices[i : i + window])
        y.append(prices[i + window])  # predict the price at t = i+window
    return np.array(X), np.array(y)