package service;

import java.io.File;

public class TradingServiceJNI {
    static {
        try {
            // Try multiple possible library locations
            String[] possiblePaths = {
                "lib/libtrading.dylib",  // Relative to current directory
                "../lib/libtrading.dylib", // One level up
                System.getProperty("user.dir") + "/lib/libtrading.dylib", // Absolute path
                System.getProperty("user.dir") + "/../lib/libtrading.dylib" // Absolute path one level up
            };

            boolean loaded = false;
            for (String path : possiblePaths) {
                File libFile = new File(path);
                if (libFile.exists()) {
                    System.load(libFile.getAbsolutePath());
                    loaded = true;
                    System.out.println("Successfully loaded library from: " + libFile.getAbsolutePath());
                    break;
                }
            }

            if (!loaded) {
                throw new UnsatisfiedLinkError("Could not find libtrading.dylib in any of the expected locations");
            }
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Failed to load native library: " + e.getMessage());
            e.printStackTrace();
            throw e; // Re-throw to make the error more visible
        }
    }

    // Native methods
    public native double getCashBalance();
    public native String[] getOpenPositions();      // e.g. ["AAPL:100:$150.0", …]
    public native void placeOrder(String symbol, int type, int style, int qty, double price);
    public native String executeStrategy(String symbol, String strategy, String[] paramNames, String[] paramValues);
    // … add more as needed

    // Convenience enums matching C++ OrderType/OrderStyle
    public static final int BUY  = 0;
    public static final int SELL = 1;
    public static final int MARKET = 0;
    public static final int LIMIT  = 1;
}