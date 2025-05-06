# python_ml/train_pytorch.py

import torch
import torch.nn as nn
import torch.optim as optim
import pandas as pd
from sklearn.preprocessing import StandardScaler
import argparse

class SimpleLSTM(nn.Module):
    def __init__(self, input_size=1, hidden_size=32, num_layers=1, output_size=1):
        super().__init__()
        self.lstm = nn.LSTM(input_size, hidden_size, num_layers, batch_first=True)
        self.fc   = nn.Linear(hidden_size, output_size)
    def forward(self, x):
        # x: [batch, seq_len, 1]
        out, _ = self.lstm(x)
        # take last time-step
        out = out[:, -1, :]
        return torch.sigmoid(self.fc(out))  # probability

def load_data(csv_path, window=20):
    df = pd.read_csv(csv_path)
    prices = df['price'].values.astype(float)
    X, y = [], []
    for i in range(len(prices) - window - 1):
        X.append(prices[i:i+window])
        # next-step return up/down
        ret = (prices[i+window+1] - prices[i+window]) / prices[i+window]
        y.append(1.0 if ret > 0 else 0.0)
    X = torch.tensor(X).unsqueeze(-1)  # [N, window, 1]
    y = torch.tensor(y).unsqueeze(-1)  # [N, 1]
    # scale features
    N, W, _ = X.shape
    flat = X.view(N, W)
    scaler = StandardScaler().fit(flat)
    flat = scaler.transform(flat)
    X = torch.tensor(flat).view(N, W, 1).float()
    return X, y.float(), scaler

def train_and_script(symbol):
    window = 20
    csv_path = f"../data/{symbol}_prices.csv"
    X, y, scaler = load_data(csv_path, window)

    # Model, loss, optimizer
    model = SimpleLSTM(input_size=1, hidden_size=32)
    criterion = nn.BCELoss()
    optimizer = optim.Adam(model.parameters(), lr=1e-3)

    # Train briefly
    model.train()
    for epoch in range(20):
        optimizer.zero_grad()
        preds = model(X)
        loss  = criterion(preds, y)
        loss.backward()
        optimizer.step()
        if epoch % 5 == 0:
            print(f"Epoch {epoch}, Loss: {loss.item():.4f}")

    # Script & save
    scripted = torch.jit.script(model)
    model_dir = f"models/{symbol}"
    os.makedirs(model_dir, exist_ok=True)
    scripted.save(f"{model_dir}/{symbol}_lstm.pt")
    # Also save scaler
    import joblib
    joblib.dump(scaler, f"{model_dir}/{symbol}_scaler.joblib")
    print(f"Saved TorchScript model and scaler to {model_dir}/")

    # After training and scripting
    import json
    scaler_params = {"mean": scaler.mean_.tolist(),
                     "scale": scaler.scale_.tolist()}
    with open(f"{model_dir}/{symbol}_scaler.json","w") as f:
        json.dump(scaler_params, f)

if __name__ == "__main__":
    import os
    parser = argparse.ArgumentParser()
    parser.add_argument("symbol", help="Ticker symbol, e.g. AAPL")
    args = parser.parse_args()
    train_and_script(args.symbol)

