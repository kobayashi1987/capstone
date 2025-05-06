# python_ml/inference_regressor.py

import os
import joblib
import numpy as np
import pandas as pd

from data_loader_regression import load_regression_windows

def predict_next_price(symbol: str, window: int = 20) -> float:
    """
    Loads the saved regressor and scaler for `symbol`, reads the last `window` prices,
    and returns the model’s predicted next-day price.
    """
    model_file = f"models/{symbol}/{symbol}_regressor.joblib"
    data_file  = f"../data/{symbol}_prices.csv"
    if not os.path.exists(model_file):
        raise FileNotFoundError(f"Model not found: {model_file}")

    # Load
    art = joblib.load(model_file)
    model, scaler = art['model'], art['scaler']

    # Prepare last window
    df = pd.read_csv(data_file)
    prices = df['price'].values
    if len(prices) < window:
        raise RuntimeError("Not enough data for inference")

    X = prices[-window:].reshape(1, -1)
    X_s = scaler.transform(X)

    # Predict
    next_price = model.predict(X_s)[0]
    return float(next_price)

if __name__ == "__main__":
    # Quick CLI demo
    import argparse
    p = argparse.ArgumentParser()
    p.add_argument("symbol", help="e.g. AAPL")
    p.add_argument("--window", type=int, default=20)
    args = p.parse_args()

    pred = predict_next_price(args.symbol, args.window)
    print(f"Predicted next price for {args.symbol}: ${pred:.2f}")