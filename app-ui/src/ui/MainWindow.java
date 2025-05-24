package ui;

import java.awt.GridLayout;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import service.TradingServiceJNI;



public class MainWindow extends JFrame {
    private JButton btnPlaceOrder;
    private JButton btnViewPortfolio;
    private JButton btnViewHistory;
    private JButton btnViewPending;
    private JButton btnUpdatePrices;
    private JButton btnExecuteStrategies;
    private JButton btnGenerateReport;
    private TradingServiceJNI tradingService;

    public MainWindow() {
        super("Trading Application");
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(400, 350);
        setLocationRelativeTo(null);
        tradingService = new TradingServiceJNI();
        initComponents();
    }

    private void initComponents() {
        JPanel panel = new JPanel(new GridLayout(7, 1, 10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(15, 15, 15, 15));

        btnPlaceOrder         = new JButton("Place New Order");
        btnViewPortfolio      = new JButton("View Portfolio");
        btnViewHistory        = new JButton("View Trade History");
        btnViewPending        = new JButton("View Pending Orders");
        btnUpdatePrices       = new JButton("Update Market Prices");
        btnExecuteStrategies  = new JButton("Execute Strategies");
        btnGenerateReport     = new JButton("Generate P&L Report");

        // Attach stubs
        btnPlaceOrder        .addActionListener(e -> onPlaceOrder());
        btnViewPortfolio     .addActionListener(e -> onViewPortfolio());
        btnViewHistory       .addActionListener(e -> onViewHistory());
        btnViewPending       .addActionListener(e -> onViewPending());
        btnUpdatePrices      .addActionListener(e -> onUpdatePrices());
        btnExecuteStrategies .addActionListener(e -> onExecuteStrategies());
        btnGenerateReport    .addActionListener(e -> onGenerateReport());

        panel.add(btnPlaceOrder);
        panel.add(btnViewPortfolio);
        panel.add(btnViewHistory);
        panel.add(btnViewPending);
        panel.add(btnUpdatePrices);
        panel.add(btnExecuteStrategies);
        panel.add(btnGenerateReport);

        setContentPane(panel);
    }

    // TODO: integrate with backend
    private void onPlaceOrder() {
        OrderDialog dlg = new OrderDialog(this);
        dlg.setVisible(true);
    }
    private void onViewPortfolio()      { JOptionPane.showMessageDialog(this, "View Portfolio"); }
    private void onViewHistory()        { JOptionPane.showMessageDialog(this, "Trade History"); }
    private void onViewPending()        { JOptionPane.showMessageDialog(this, "Pending Orders"); }
    private void onUpdatePrices()       { JOptionPane.showMessageDialog(this, "Update Prices"); }
    private void onExecuteStrategies() {
        StrategyDialog dialog = new StrategyDialog(this, tradingService);
        dialog.setVisible(true);
        
        if (dialog.isConfirmed()) {
            String symbol = dialog.getSymbol();
            String strategy = dialog.getSelectedStrategy();
            Map<String, String> params = dialog.getParameters();
            
            // TODO: Call the backend service with the strategy and parameters
            StringBuilder message = new StringBuilder();
            message.append("Executing ").append(strategy).append(" strategy for ").append(symbol).append("\n");
            message.append("Parameters:\n");
            for (Map.Entry<String, String> param : params.entrySet()) {
                message.append("  ").append(param.getKey()).append(": ").append(param.getValue()).append("\n");
            }
            
            JOptionPane.showMessageDialog(this, message.toString(), 
                "Strategy Execution", JOptionPane.INFORMATION_MESSAGE);
        }
    }
    private void onGenerateReport()     { JOptionPane.showMessageDialog(this, "Generate P&L Report"); }


    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new MainWindow().setVisible(true);
        });
    }
}