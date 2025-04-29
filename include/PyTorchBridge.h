////
//// Created by jack on 29/4/25.
//// include/PyTorchBridge.h
//
//#ifndef TRADING_APPLICATION_PYTORCH_BRIDGE_H
//#define TRADING_APPLICATION_PYTORCH_BRIDGE_H
//
//#include <string>
//#include <vector>
//#include <memory>
//#include <iostream>
//#include <sstream>
//#include <stdexcept>
//#include <cstdio>
//#include <array>
//#include "json.hpp"
//
//using json = nlohmann::json;
//
//class PyTorchBridge {
//public:
//    PyTorchBridge();
//    explicit PyTorchBridge(const std::string& modelPath);
//    ~PyTorchBridge();
//
//    // Load a PyTorch model
//    bool loadModel(const std::string& modelPath);
//
//    // Make a prediction with the loaded model
//    json predict(const json& marketData);
//
//    // Check if the bridge is ready
//    bool isReady() const;
//
//private:
//    std::string modelPath_;
//    bool isInitialized_;
//
//    // Execute Python script and get the output
//    json executePythonCommand(const json& inputData);
//
//    // Utility function to execute a command and capture its output
//    std::string executeCommand(const std::string& command, const std::string& input);
//};
//
//#endif // TRADING_APPLICATION_PYTORCH_BRIDGE_H