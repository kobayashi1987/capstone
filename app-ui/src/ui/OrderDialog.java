package ui;

import java.awt.*;
import javax.swing.*;
//
public class OrderDialog extends JDialog {
    private JTextField txtSymbol, txtQuantity, txtPrice;
    private JComboBox<String> cbType, cbStyle;
    private JButton btnSubmit, btnCancel;

    public OrderDialog(Frame owner) {
        super(owner, "Place Order", true);
        setSize(300, 250);
        setLocationRelativeTo(owner);
        initComponents();
    }

    private void initComponents() {
        setLayout(new GridLayout(6, 2, 5, 5));
        add(new JLabel("Symbol:"));
        txtSymbol = new JTextField();
        add(txtSymbol);

        add(new JLabel("Quantity:"));
        txtQuantity = new JTextField();
        add(txtQuantity);

        add(new JLabel("Type:"));
        cbType = new JComboBox<>(new String[]{"Buy", "Sell"});
        add(cbType);

        add(new JLabel("Style:"));
        cbStyle = new JComboBox<>(new String[]{"Market", "Limit"});
        add(cbStyle);

        add(new JLabel("Price (if limit):"));
        txtPrice = new JTextField();
        add(txtPrice);

        btnSubmit = new JButton("Submit");
        btnCancel = new JButton("Cancel");
        btnSubmit.addActionListener(e -> onSubmit());
        btnCancel.addActionListener(e -> dispose());

        add(btnSubmit);
        add(btnCancel);
    }

    private void onSubmit() {
        String symbol   = txtSymbol.getText().trim();
        int qty         = Integer.parseInt(txtQuantity.getText().trim());
        String type     = (String) cbType.getSelectedItem();
        //String style    = (String) cbStyle.getSelectedItem();
        double price    = txtPrice.getText().isEmpty() ? 0.0 : Double.parseDouble(txtPrice.getText());

        // TODO: call backend.placeOrder(symbol, type, style, qty, price);
        System.out.printf("Placing %s %s order for %d @ %.2f%n", type, symbol, qty, price);

        dispose();
    }
}