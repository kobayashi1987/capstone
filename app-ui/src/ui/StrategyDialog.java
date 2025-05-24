package ui;

import java.awt.BorderLayout;
import java.awt.EventQueue;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.GridLayout;
import java.util.HashMap;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import service.TradingServiceJNI;

public class StrategyDialog extends JDialog {
    private JComboBox<String> strategySelector;
    private JPanel parameterPanel;
    private Map<String, JTextField> parameterFields;
    private JButton executeButton;
    private JButton cancelButton;
    private JTextField symbolField;
    private JTextArea resultsArea;
    private boolean confirmed = false;
    private TradingServiceJNI tradingService;

    // Strategy parameters
    private static final Map<String, String[]> STRATEGY_PARAMETERS = new HashMap<>();
    static {
        STRATEGY_PARAMETERS.put("Moving Average Crossover", new String[]{"Short Period", "Long Period"});
        STRATEGY_PARAMETERS.put("RSI", new String[]{"Period", "Overbought Level", "Oversold Level"});
        STRATEGY_PARAMETERS.put("Bollinger Bands", new String[]{"Period", "Standard Deviations"});
        STRATEGY_PARAMETERS.put("Momentum", new String[]{"Period", "Threshold"});
        STRATEGY_PARAMETERS.put("Breakout", new String[]{"Period", "Threshold"});
        STRATEGY_PARAMETERS.put("Mean Reversion", new String[]{"Period", "Threshold"});
    }

    public StrategyDialog(Frame parent, TradingServiceJNI tradingService) {
        super(parent, "Execute Trading Strategy", true);
        this.tradingService = tradingService;
        parameterFields = new HashMap<>();
        initComponents();
        pack();
        setLocationRelativeTo(parent);
    }

    private void initComponents() {
        setLayout(new BorderLayout(10, 10));

        // Top panel with symbol and strategy selection
        JPanel topPanel = new JPanel(new GridLayout(2, 2, 5, 5));
        
        // Symbol input
        topPanel.add(new JLabel("Stock Symbol:"));
        symbolField = new JTextField(10);
        symbolField.setToolTipText("Enter stock symbol (e.g., AAPL, GOOGL)");
        topPanel.add(symbolField);
        
        // Strategy selection
        topPanel.add(new JLabel("Select Strategy:"));
        strategySelector = new JComboBox<>(STRATEGY_PARAMETERS.keySet().toArray(new String[0]));
        strategySelector.addActionListener(e -> updateParameterPanel());
        topPanel.add(strategySelector);

        // Parameter panel
        parameterPanel = new JPanel(new GridLayout(0, 2, 5, 5));
        updateParameterPanel();

        // Results panel
        resultsArea = new JTextArea(10, 40);
        resultsArea.setEditable(false);
        resultsArea.setLineWrap(true);
        resultsArea.setWrapStyleWord(true);
        JScrollPane resultsScroll = new JScrollPane(resultsArea);

        // Button panel
        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        executeButton = new JButton("Execute");
        cancelButton = new JButton("Close");
        
        executeButton.addActionListener(e -> {
            if (validateInputs()) {
                executeStrategy();
            }
        });
        
        cancelButton.addActionListener(e -> {
            confirmed = false;
            setVisible(false);
        });

        buttonPanel.add(executeButton);
        buttonPanel.add(cancelButton);

        // Add panels to dialog
        add(topPanel, BorderLayout.NORTH);
        add(new JScrollPane(parameterPanel), BorderLayout.CENTER);
        add(resultsScroll, BorderLayout.SOUTH);
        add(buttonPanel, BorderLayout.SOUTH);

        // Add padding
        ((JPanel)getContentPane()).setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
    }

    private void executeStrategy() {
        String symbol = symbolField.getText().trim().toUpperCase();
        String strategy = (String) strategySelector.getSelectedItem();
        Map<String, String> params = getParameters();
        
        // Convert parameters to arrays for JNI call
        String[] paramNames = params.keySet().toArray(new String[0]);
        String[] paramValues = params.values().toArray(new String[0]);
        
        // Execute strategy in a background thread
        EventQueue.invokeLater(() -> {
            executeButton.setEnabled(false);
            resultsArea.setText("Executing strategy...\n");
            
            try {
                String result = tradingService.executeStrategy(symbol, strategy, paramNames, paramValues);
                resultsArea.setText(result);
            } catch (Exception ex) {
                resultsArea.setText("Error executing strategy: " + ex.getMessage());
            } finally {
                executeButton.setEnabled(true);
            }
        });
    }

    private boolean validateInputs() {
        // Validate stock symbol
        String symbol = symbolField.getText().trim().toUpperCase();
        if (symbol.isEmpty()) {
            JOptionPane.showMessageDialog(this, 
                "Please enter a stock symbol", 
                "Validation Error", 
                JOptionPane.ERROR_MESSAGE);
            symbolField.requestFocus();
            return false;
        }

        // Validate strategy parameters
        for (Map.Entry<String, JTextField> entry : parameterFields.entrySet()) {
            String value = entry.getValue().getText().trim();
            if (value.isEmpty()) {
                JOptionPane.showMessageDialog(this, 
                    "Please enter a value for " + entry.getKey(), 
                    "Validation Error", 
                    JOptionPane.ERROR_MESSAGE);
                entry.getValue().requestFocus();
                return false;
            }
            
            // Validate numeric parameters
            try {
                double numValue = Double.parseDouble(value);
                if (numValue <= 0) {
                    JOptionPane.showMessageDialog(this, 
                        entry.getKey() + " must be greater than 0", 
                        "Validation Error", 
                        JOptionPane.ERROR_MESSAGE);
                    entry.getValue().requestFocus();
                    return false;
                }
            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(this, 
                    entry.getKey() + " must be a valid number", 
                    "Validation Error", 
                    JOptionPane.ERROR_MESSAGE);
                entry.getValue().requestFocus();
                return false;
            }
        }
        
        return true;
    }

    private void updateParameterPanel() {
        parameterPanel.removeAll();
        parameterFields.clear();

        String selectedStrategy = (String) strategySelector.getSelectedItem();
        String[] parameters = STRATEGY_PARAMETERS.get(selectedStrategy);

        for (String param : parameters) {
            parameterPanel.add(new JLabel(param + ":"));
            JTextField field = new JTextField(10);
            field.setToolTipText("Enter " + param.toLowerCase());
            parameterFields.put(param, field);
            parameterPanel.add(field);
        }

        parameterPanel.revalidate();
        parameterPanel.repaint();
        pack();
    }

    public boolean isConfirmed() {
        return confirmed;
    }

    public String getSelectedStrategy() {
        return (String) strategySelector.getSelectedItem();
    }

    public String getSymbol() {
        return symbolField.getText().trim().toUpperCase();
    }

    public Map<String, String> getParameters() {
        Map<String, String> params = new HashMap<>();
        for (Map.Entry<String, JTextField> entry : parameterFields.entrySet()) {
            params.put(entry.getKey(), entry.getValue().getText().trim());
        }
        return params;
    }
} 