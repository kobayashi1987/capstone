#include "service_TradingServiceJNI.h"
#include "TradingEngine.h"
#include "MarketDataFeed.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>

// A singleton or global engine instance
static TradingEngine* engine = nullptr;
static MarketDataFeed* feed = nullptr;

JNIEXPORT void JNICALL Java_service_TradingServiceJNI_initNative(JNIEnv* env, jclass) {
    if (!engine) {
        std::vector<std::string> symbols = {"AAPL","GOOGL","MSFT","AMZN"};
        std::unordered_map<std::string,double> initPrices = {
            {"AAPL",150}, {"GOOGL",280}, {"MSFT",300}, {"AMZN",350}};
        feed = new MarketDataFeed(symbols, initPrices);
        engine = new TradingEngine(1e6);
    }
}

JNIEXPORT jdouble JNICALL Java_service_TradingServiceJNI_getCashBalance
  (JNIEnv* env, jobject) {
    return engine->getCashBalance();
}

JNIEXPORT jobjectArray JNICALL Java_service_TradingServiceJNI_getOpenPositions
  (JNIEnv* env, jobject) {
    const auto& positions = engine->getPositions();
    jclass stringCls = env->FindClass("java/lang/String");
    jobjectArray arr = env->NewObjectArray(positions.size(), stringCls, nullptr);
    int idx = 0;
    for (auto& [sym, pos] : positions) {
        std::ostringstream ss;
        ss << sym << ":" << pos.quantity << ":$" << pos.averagePrice;
        env->SetObjectArrayElement(arr, idx++,
            env->NewStringUTF(ss.str().c_str()));
    }
    return arr;
}

JNIEXPORT void JNICALL Java_service_TradingServiceJNI_placeOrder
  (JNIEnv* env, jobject, jstring jsym, jint jtype, jint jstyle, jint jqty, jdouble jprice) {
    const char* sym = env->GetStringUTFChars(jsym, nullptr);
    engine->userPlaceOrder(
        std::string(sym),
        static_cast<OrderType>(jtype),
        static_cast<OrderStyle>(jstyle),
        jqty,
        jprice,
        /*stopLoss*/0.0,
        /*takeProfit*/0.0,
        feed->getPrices()
    );
    env->ReleaseStringUTFChars(jsym, sym);
}

JNIEXPORT jstring JNICALL Java_service_TradingServiceJNI_executeStrategy
  (JNIEnv* env, jobject obj, jstring symbol, jstring strategy, jobjectArray paramNames, jobjectArray paramValues) {
    
    // Convert Java strings to C++ strings
    const char* symbolStr = env->GetStringUTFChars(symbol, nullptr);
    const char* strategyStr = env->GetStringUTFChars(strategy, nullptr);
    
    // Get array lengths
    jsize paramCount = env->GetArrayLength(paramNames);
    
    // Convert parameters to C++ map
    std::map<std::string, std::string> params;
    for (jsize i = 0; i < paramCount; i++) {
        jstring nameObj = (jstring)env->GetObjectArrayElement(paramNames, i);
        jstring valueObj = (jstring)env->GetObjectArrayElement(paramValues, i);
        
        const char* nameStr = env->GetStringUTFChars(nameObj, nullptr);
        const char* valueStr = env->GetStringUTFChars(valueObj, nullptr);
        
        params[nameStr] = valueStr;
        
        env->ReleaseStringUTFChars(nameObj, nameStr);
        env->ReleaseStringUTFChars(valueObj, valueStr);
        env->DeleteLocalRef(nameObj);
        env->DeleteLocalRef(valueObj);
    }
    
    // TODO: Call the actual strategy implementation
    // For now, return a formatted result string
    std::stringstream result;
    result << "Strategy Execution Results\n";
    result << "------------------------\n";
    result << "Symbol: " << symbolStr << "\n";
    result << "Strategy: " << strategyStr << "\n";
    result << "Parameters:\n";
    
    for (const auto& param : params) {
        result << "  " << param.first << ": " << param.second << "\n";
    }
    
    result << "\nAnalysis Results:\n";
    result << "  Signal: BUY\n";
    result << "  Confidence: 85%\n";
    result << "  Target Price: $150.00\n";
    result << "  Stop Loss: $145.00\n";
    
    // Clean up
    env->ReleaseStringUTFChars(symbol, symbolStr);
    env->ReleaseStringUTFChars(strategy, strategyStr);
    
    // Return the result as a Java string
    return env->NewStringUTF(result.str().c_str());
}
