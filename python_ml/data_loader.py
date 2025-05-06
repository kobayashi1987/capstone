import pandas as pd
from sklearn.preprocessing import StandardScaler

def load_price_series(csv_path):
    """
    Load a single-column CSV of historical prices, return a sliding-window feature matrix.
    """
    df = pd.read_csv(csv_path)
    prices = df['price'].values
    # Example: build windows of length 20 to predict next-step return
    window_size = 20
    X, y = [], []
    for i in range(len(prices) - window_size - 1):
        window = prices[i : i + window_size]
        target = (prices[i + window_size + 1] - prices[i + window_size]) / prices[i + window_size]
        X.append(window)
        y.append(1 if target > 0 else 0)  # binary up/down
    X = pd.DataFrame(X)
    y = pd.Series(y)
    # Standardize features
    scaler = StandardScaler().fit(X)
    X_scaled = scaler.transform(X)
    return X_scaled, y.values, scaler