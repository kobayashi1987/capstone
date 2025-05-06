# python_ml/train_price_regressor.py

import os
import joblib
import numpy as np
import pandas as pd

from sklearn.ensemble import GradientBoostingRegressor
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import mean_squared_error, r2_score

from data_loader_regression import load_regression_windows

def main(symbol: str, window: int = 20):
    # Paths
    data_path = f"../data/{symbol}_prices.csv"
    model_dir = f"models/{symbol}"
    os.makedirs(model_dir, exist_ok=True)
    model_file = f"{model_dir}/{symbol}_regressor.joblib"

    # 1) Load data
    X, y = load_regression_windows(data_path, window)
    if len(X) == 0:
        raise RuntimeError("Not enough data to build windows")

    # 2) Split
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.2, random_state=42
    )

    # 3) Scale features
    scaler = StandardScaler().fit(X_train)
    X_train_s = scaler.transform(X_train)
    X_test_s  = scaler.transform(X_test)

    # 4) Grid search for best params
    param_grid = {
        'n_estimators': [100, 200],
        'max_depth': [3, 5, None],
        'learning_rate': [0.01, 0.1]
    }
    base = GradientBoostingRegressor(random_state=42)
    gs = GridSearchCV(base, param_grid, cv=3,
                      scoring='neg_mean_squared_error', n_jobs=-1)
    gs.fit(X_train_s, y_train)
    print("Best params:", gs.best_params_)

    # 5) Final model
    best = gs.best_estimator_
    best.fit(np.vstack([X_train_s, X_test_s]),
             np.hstack([y_train, y_test]))
    joblib.dump({'model': best, 'scaler': scaler}, model_file)
    print(f"Saved regression model + scaler to {model_file}")

    # 6) Evaluate on test
    y_pred = best.predict(X_test_s)
    # compute RMSE manually
    mse  = mean_squared_error(y_test, y_pred)
    rmse = np.sqrt(mse)
    r2   = r2_score(y_test, y_pred)
    print(f"Test RMSE: {rmse:.4f}, R²: {r2:.4f}")

if __name__ == "__main__":
    import argparse
    p = argparse.ArgumentParser()
    p.add_argument("symbol", help="e.g. AAPL")
    p.add_argument("--window", type=int, default=20)
    args = p.parse_args()
    main(args.symbol, args.window)