# python_ml/train_model.py

import os
import joblib
import numpy as np
import pandas as pd

from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import classification_report, roc_auc_score

def load_sliding_windows(csv_path, window=20):
    """
    Reads a one-column CSV ('price') and builds:
      X: shape (N, window) of rolling price windows
      y: binary label (1 if next return > 0 else 0)
    """
    df = pd.read_csv(csv_path)
    prices = df['price'].values
    X, y = [], []
    for i in range(len(prices) - window - 1):
        X.append(prices[i:i+window])
        ret = (prices[i+window+1] - prices[i+window]) / prices[i+window]
        y.append(1 if ret > 0 else 0)
    return np.array(X), np.array(y)

def main(symbol: str):
    # Paths
    data_path  = f"../data/{symbol}_prices.csv"
    model_dir  = f"models/{symbol}"
    os.makedirs(model_dir, exist_ok=True)
    model_file = f"{model_dir}/{symbol}_rf.joblib"

    # 1) Load data
    X, y = load_sliding_windows(data_path, window=20)
    if len(X) == 0:
        raise RuntimeError("Not enough data to build windows")

    # 2) Train/test split
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.2, random_state=42, stratify=y
    )

    # 3) Scale features
    scaler = StandardScaler().fit(X_train)
    X_train_s = scaler.transform(X_train)
    X_test_s  = scaler.transform(X_test)

    # 4) Hyperparameter tuning (optional)
    param_grid = {
        'n_estimators': [50, 100],
        'max_depth': [5, 10, None]
    }
    base_clf = RandomForestClassifier(random_state=42)
    clf = GridSearchCV(base_clf, param_grid, cv=3, scoring='roc_auc', n_jobs=-1)
    clf.fit(X_train_s, y_train)
    print("Best params:", clf.best_params_)

    # 5) Final train & save
    best = clf.best_estimator_
    best.fit(np.vstack([X_train_s, X_test_s]), np.hstack([y_train, y_test]))
    joblib.dump({'model': best, 'scaler': scaler}, model_file)
    print(f"Saved model + scaler to {model_file}")

    # 6) Evaluate
    y_pred = best.predict(X_test_s)
    y_proba = best.predict_proba(X_test_s)[:,1]
    print("\nClassification Report:\n", classification_report(y_test, y_pred))
    print("ROC AUC:", roc_auc_score(y_test, y_proba))

if __name__ == "__main__":
    import argparse
    p = argparse.ArgumentParser()
    p.add_argument("symbol", help="e.g. AAPL")
    args = p.parse_args()
    main(args.symbol)