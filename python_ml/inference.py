# python_ml/inference.py

import os
import joblib
import numpy as np
import pandas as pd

def load_recent_window(csv_path, window=20):
    df = pd.read_csv(csv_path)
    prices = df['price'].values
    if len(prices) < window:
        raise RuntimeError("Not enough data for inference")
    return prices[-window:]

def main(symbol: str):
    model_file = f"models/{symbol}/{symbol}_rf.joblib"
    data_file  = f"../data/{symbol}_prices.csv"
    if not os.path.exists(model_file):
        raise FileNotFoundError(f"Model not found: {model_file}")

    # Load model + scaler
    art = joblib.load(model_file)
    clf, scaler = art['model'], art['scaler']

    # Prepare recent window
    window = load_recent_window(data_file, window=20).reshape(1, -1)
    window_s = scaler.transform(window)

    # Predict
    proba = clf.predict_proba(window_s)[0,1]
    signal = "BUY" if proba > 0.6 else "SELL" if proba < 0.4 else "HOLD"

    # Output
    out = pd.DataFrame({
        'symbol': [symbol],
        'prob_up': [proba],
        'signal': [signal]
    })
    out_file = f"models/{symbol}/{symbol}_signal.csv"
    out.to_csv(out_file, index=False)
    print(f"Wrote signal to {out_file}")
    print(out.to_string(index=False))

if __name__ == "__main__":
    import argparse
    p = argparse.ArgumentParser()
    p.add_argument("symbol", help="Ticker, e.g. AAPL")
    args = p.parse_args()
    main(args.symbol)