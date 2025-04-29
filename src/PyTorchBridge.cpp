//
// Created by jack on 29/4/25.
//
// src/PyTorchBridge.cpp

#include "PyTorchBridge.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

PyTorchBridge::PyTorchBridge() : isInitialized_(false) {}

PyTorchBridge::PyTorchBridge(const std::string& modelPath)
        : modelPath_(modelPath), isInitialized_(false) {
    isInitialized_ = loadModel(modelPath);
}

PyTorchBridge::~PyTorchBridge() {
    // Cleanup if needed
}

bool PyTorchBridge::loadModel(const std::string& modelPath) {
    if (!fs::exists(modelPath)) {
        std::cerr << "Model file not found: " << modelPath << std::endl;
        return false;
    }

    modelPath_ = modelPath;

    // Check if the Python bridge script exists
    std::string pythonScriptPath = "python/ml_bridge.py";
    if (!fs::exists(pythonScriptPath)) {
        std::cerr << "Python bridge script not found: " << pythonScriptPath << std::endl;
        return false;
    }

    // Prepare JSON input for loading the model
    json inputData = {
            {"command", "load_model"},
            {"model_path", modelPath}
    };

    // Execute the Python script to load the model
    json result = executePythonCommand(inputData);

    // Check if the model loaded successfully
    if (result.contains("success") && result["success"].get<bool>()) {
        isInitialized_ = true;
        return true;
    }

    std::cerr << "Failed to load model: " << result.dump() << std::endl;
    return false;
}

json PyTorchBridge::predict(const json& marketData) {
    if (!isInitialized_) {
        return {{"error", "Model not initialized"}};
    }

    // Prepare JSON input for prediction
    json inputData = {
            {"command", "predict"},
            {"model_path", modelPath_},
            {"data", marketData}
    };

    // Execute the Python script to make a prediction
    return executePythonCommand(inputData);
}

bool PyTorchBridge::isReady() const {
    return isInitialized_;
}

json PyTorchBridge::executePythonCommand(const json& inputData) {
    // Convert input JSON to string
    std::string inputStr = inputData.dump();

    // Execute Python script with the input
    std::string pythonPath = "python";  // or "python3" depending on your system
    std::string scriptPath = "python/ml_bridge.py";
    std::string command = pythonPath + " " + scriptPath;

    std::string output = executeCommand(command, inputStr);

    // Parse the output as JSON
    try {
        return json::parse(output);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing Python output: " << e.what() << std::endl;
        std::cerr << "Raw output: " << output << std::endl;
        return {{"error", "Failed to parse Python output"}};
    }
}

std::string PyTorchBridge::executeCommand(const std::string& command, const std::string& input) {
    std::array<char, 128> buffer;
    std::string result;

    // Create pipes for communication
#ifdef _WIN32
    FILE* pipe = _popen((command + " 2>&1").c_str(), "w+");
#else
    FILE* pipe = popen((command + " 2>&1").c_str(), "w+");
#endif

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Write input to the process
    fwrite(input.c_str(), sizeof(char), input.length(), pipe);
    fflush(pipe);

    // Read the output
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    // Close the pipe
#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif

    return result;
}