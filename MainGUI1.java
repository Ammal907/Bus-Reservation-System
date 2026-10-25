import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.ExecutionException;

public class MainGUI1 extends JFrame {

    // =========================================
    // FILE PATHS
    // =========================================

    static final String BACKGROUND_IMAGE = "bus-background.jfif";

    // IMPORTANT: Use the actual compiled .exe path from Dev-C++.
    static final String CPP_PATH =
        "E:\\AMMAL\\University Material\\SEMESTER 2-WORK\\OOP LAB\\OOP PROJECT\\BUS RESERVATION.exe";

    private JTextField nameField;
    private JTextField departureField;
    private JTextField destinationField;
    private JTextArea outputArea;
    private JLabel statusLabel;

    private JButton bookButton;
    private JButton[] actionButtons;

    private final Color white = Color.WHITE;
    private final Color glass = new Color(10, 20, 35, 185);
    private final Color blue = new Color(15, 80, 135);
    private final Color lightBlue = new Color(35, 125, 190);

    // =========================================
    // CONSTRUCTOR
    // =========================================

    public MainGUI1() {
        setTitle("Bus Reservation System");
        setSize(1050, 720);
        setMinimumSize(new Dimension(850, 600));
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        createGUI();
    }

    // =========================================
    // CREATE GUI
    // =========================================

    private void createGUI() {

        BackgroundImagePanel root = new BackgroundImagePanel();
        root.setLayout(new BorderLayout(15, 15));
        root.setBorder(new EmptyBorder(20, 25, 20, 25));
        setContentPane(root);

        // HEADER

        JPanel header = new JPanel();
        header.setOpaque(false);
        header.setLayout(new BoxLayout(header, BoxLayout.Y_AXIS));

        JLabel title = new JLabel("BUS RESERVATION SYSTEM");
        title.setFont(new Font("Segoe UI", Font.BOLD, 30));
        title.setForeground(white);
        title.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel subtitle = new JLabel(
            "Book your journey with comfort"
        );
        subtitle.setFont(new Font("Segoe UI", Font.PLAIN, 15));
        subtitle.setForeground(new Color(225, 235, 245));
        subtitle.setAlignmentX(Component.CENTER_ALIGNMENT);

        header.add(title);
        header.add(Box.createVerticalStrut(5));
        header.add(subtitle);

        root.add(header, BorderLayout.NORTH);

        // MAIN CONTENT

        JPanel content = new JPanel(new GridBagLayout());
        content.setOpaque(false);

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(8, 8, 8, 8);
        gbc.fill = GridBagConstraints.BOTH;
        gbc.weighty = 1;

        // =====================================
        // BOOKING PANEL
        // =====================================

        JPanel bookingPanel = new GlassPanel();
        bookingPanel.setLayout(new GridBagLayout());

        GridBagConstraints b = new GridBagConstraints();
        b.insets = new Insets(9, 10, 9, 10);
        b.fill = GridBagConstraints.HORIZONTAL;
        b.weightx = 1;

        b.gridx = 0;
        b.gridy = 0;
        bookingPanel.add(makeLabel("Passenger Details", 20), b);

        b.gridy = 1;
        bookingPanel.add(makeLabel("Passenger Name", 14), b);

        nameField = createTextField();
        b.gridy = 2;
        bookingPanel.add(nameField, b);

        b.gridy = 3;
        bookingPanel.add(makeLabel("Departure City", 14), b);

        departureField = createTextField();
        b.gridy = 4;
        bookingPanel.add(departureField, b);

        b.gridy = 5;
        bookingPanel.add(makeLabel("Destination City", 14), b);

        destinationField = createTextField();
        b.gridy = 6;
        bookingPanel.add(destinationField, b);

        bookButton = createButton("Book Ticket");
        bookButton.addActionListener(e -> runCppAction(1));

        b.gridy = 7;
        b.insets = new Insets(18, 10, 8, 10);
        bookingPanel.add(bookButton, b);

        // =====================================
        // SYSTEM OPERATIONS PANEL
        // =====================================

        JPanel actionsPanel = new GlassPanel();
        actionsPanel.setLayout(new GridBagLayout());

        GridBagConstraints a = new GridBagConstraints();
        a.insets = new Insets(7, 8, 7, 8);
        a.fill = GridBagConstraints.HORIZONTAL;
        a.weightx = 1;

        a.gridx = 0;
        a.gridy = 0;
        a.gridwidth = 2;

        actionsPanel.add(makeLabel("System Operations", 20), a);

        a.gridwidth = 1;

        // Button order matches the C++ menu choices 2 through 8.
        String[] actionNames = {
            "Show Passengers",          // C++ choice 2
            "View Routes & Seats",      // C++ choice 3
            "Total Passengers",         // C++ choice 4
            "VIP Demonstration",        // C++ choice 5
            "Operator Overloading",     // C++ choice 6
            "Admin & Passenger Info",   // C++ choice 7
            "Reload Saved Bookings",    // C++ choice 8
            "Clear Output"              // GUI only
        };

        actionButtons = new JButton[actionNames.length];

        for (int i = 0; i < actionNames.length; i++) {

            final int action = i + 2;

            actionButtons[i] = createButton(actionNames[i]);

            if (i == 7) {
                actionButtons[i].addActionListener(e -> {
                    outputArea.setText("");
                    statusLabel.setText("Output cleared");
                });
            } else {
                actionButtons[i].addActionListener(
                    e -> runCppAction(action)
                );
            }

            a.gridx = i % 2;
            a.gridy = (i / 2) + 1;

            actionsPanel.add(actionButtons[i], a);
        }

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 0.9;
        content.add(bookingPanel, gbc);

        gbc.gridx = 1;
        gbc.weightx = 1.1;
        content.add(actionsPanel, gbc);

        root.add(content, BorderLayout.CENTER);

        // =====================================
        // SYSTEM OUTPUT PANEL
        // =====================================

        JPanel bottomPanel = new GlassPanel();
        bottomPanel.setLayout(new BorderLayout(8, 8));

        bottomPanel.add(makeLabel("System Output", 17), BorderLayout.NORTH);

        outputArea = new JTextArea(7, 30);
        outputArea.setEditable(false);
        outputArea.setLineWrap(true);
        outputArea.setWrapStyleWord(true);
        outputArea.setFont(new Font("Consolas", Font.PLAIN, 14));
        outputArea.setForeground(Color.WHITE);
        outputArea.setCaretColor(Color.WHITE);
        outputArea.setOpaque(false);
        outputArea.setBorder(new EmptyBorder(8, 8, 8, 8));

        JScrollPane scrollPane = new JScrollPane(outputArea);
        scrollPane.setOpaque(false);
        scrollPane.getViewport().setOpaque(false);
        scrollPane.setBorder(new LineBorder(
            new Color(255, 255, 255, 100), 1, true
        ));

        bottomPanel.add(scrollPane, BorderLayout.CENTER);

        statusLabel = new JLabel("Ready");
        statusLabel.setForeground(new Color(210, 235, 255));
        statusLabel.setFont(new Font("Segoe UI", Font.PLAIN, 12));

        bottomPanel.add(statusLabel, BorderLayout.SOUTH);

        root.add(bottomPanel, BorderLayout.SOUTH);

        setVisible(true);
    }

    // =========================================
    // LABEL
    // =========================================

    private JLabel makeLabel(String text, int size) {
        JLabel label = new JLabel(text);
        label.setFont(new Font("Segoe UI", Font.BOLD, size));
        label.setForeground(white);
        return label;
    }

    // =========================================
    // TEXT FIELD
    // =========================================

    private JTextField createTextField() {

        JTextField field = new JTextField(17);

        field.setFont(new Font("Segoe UI", Font.PLAIN, 14));
        field.setForeground(Color.WHITE);
        field.setCaretColor(Color.WHITE);
        field.setBackground(new Color(15, 30, 45, 190));
        field.setOpaque(false);

        field.setBorder(new CompoundBorder(
            new LineBorder(
                new Color(220, 235, 255, 150), 1, true
            ),
            new EmptyBorder(9, 10, 9, 10)
        ));

        return field;
    }

    // =========================================
    // BUTTON
    // =========================================

    private JButton createButton(String text) {

        JButton button = new JButton(text);

        button.setFont(new Font("Segoe UI", Font.BOLD, 13));
        button.setForeground(Color.WHITE);
        button.setBackground(blue);
        button.setFocusPainted(false);
        button.setBorderPainted(false);
        button.setOpaque(true);
        button.setCursor(new Cursor(Cursor.HAND_CURSOR));
        button.setBorder(new EmptyBorder(11, 10, 11, 10));

        button.addMouseListener(new MouseAdapter() {

            @Override
            public void mouseEntered(MouseEvent e) {
                if (button.isEnabled()) {
                    button.setBackground(lightBlue);
                }
            }

            @Override
            public void mouseExited(MouseEvent e) {
                button.setBackground(blue);
            }
        });

        return button;
    }

    // =========================================
    // VALIDATE INPUTS
    // =========================================

    private boolean validateInputs(int action) {

        String name = nameField.getText().trim();
        String departure = departureField.getText().trim();
        String destination = destinationField.getText().trim();

        if (action == 1) {

            if (name.isEmpty() ||
                departure.isEmpty() ||
                destination.isEmpty()) {

                JOptionPane.showMessageDialog(
                    this,
                    "Please enter passenger name, departure and destination.",
                    "Missing Information",
                    JOptionPane.WARNING_MESSAGE
                );

                return false;
            }

            if (departure.equalsIgnoreCase(destination)) {
                JOptionPane.showMessageDialog(
                    this,
                    "Departure and destination cannot be the same.",
                    "Invalid Route",
                    JOptionPane.WARNING_MESSAGE
                );

                return false;
            }
        }

        if (action == 5 && name.isEmpty()) {

            JOptionPane.showMessageDialog(
                this,
                "Enter a name for the VIP demonstration.",
                "Missing Name",
                JOptionPane.WARNING_MESSAGE
            );

            return false;
        }

        if (action == 6) {

            if (name.isEmpty() ||
                departure.isEmpty() ||
                destination.isEmpty()) {

                JOptionPane.showMessageDialog(
                    this,
                    "For Operator Overloading, enter a name, departure and destination.",
                    "Missing Information",
                    JOptionPane.WARNING_MESSAGE
                );

                return false;
            }

            // The C++ demonstration requires a previously loaded booking.
            File bookingFile = new File(
                new File(CPP_PATH).getAbsoluteFile().getParentFile(),
                "booking.txt"
            );

            if (!bookingFile.isFile() || bookingFile.length() == 0) {
                JOptionPane.showMessageDialog(
                    this,
                    "Please book a ticket first, then try Operator Overloading.",
                    "No Saved Booking",
                    JOptionPane.WARNING_MESSAGE
                );

                return false;
            }
        }

        return true;
    }

    // =========================================
    // RUN C++ ACTION
    // =========================================

    private void runCppAction(int action) {

        if (!validateInputs(action)) {
            return;
        }

        File executable = new File(CPP_PATH);

        if (!executable.isFile()) {

            outputArea.setText(
                "C++ executable was not found.\n\n" +
                "Current path:\n" + executable.getAbsolutePath() +
                "\n\nCompile your C++ program in Dev-C++ and update CPP_PATH."
            );

            statusLabel.setText("C++ executable not found");

            return;
        }

        setActionButtonsEnabled(false);
        statusLabel.setText("Processing...");
        outputArea.setText("Running C++ program...\n");

        SwingWorker<String, Void> worker =
            new SwingWorker<String, Void>() {

            @Override
            protected String doInBackground() throws Exception {

                ProcessBuilder pb = new ProcessBuilder(
                    executable.getAbsolutePath()
                );

                pb.redirectErrorStream(true);

                // Keep booking.txt beside the executable.
                pb.directory(executable.getAbsoluteFile().getParentFile());

                Process process = pb.start();

                StringBuilder result = new StringBuilder();

                try (
                    BufferedWriter writer = new BufferedWriter(
                        new OutputStreamWriter(
                            process.getOutputStream(),
                            StandardCharsets.UTF_8
                        )
                    );

                    BufferedReader reader = new BufferedReader(
                        new InputStreamReader(
                            process.getInputStream(),
                            StandardCharsets.UTF_8
                        )
                    )
                ) {

                    // Send the selected C++ menu choice.
                    writer.write(String.valueOf(action));
                    writer.newLine();

                    if (action == 1) {

                        // C++ asks for departure, destination, then name.
                        writer.write(departureField.getText().trim());
                        writer.newLine();

                        writer.write(destinationField.getText().trim());
                        writer.newLine();

                        writer.write(nameField.getText().trim());
                        writer.newLine();

                    } else if (action == 5) {

                        // VIP demonstration asks for a name.
                        writer.write(nameField.getText().trim());
                        writer.newLine();

                    } else if (action == 6) {

                        // Operator demonstration asks for name and route.
                        writer.write(nameField.getText().trim());
                        writer.newLine();

                        writer.write(departureField.getText().trim());
                        writer.newLine();

                        writer.write(destinationField.getText().trim());
                        writer.newLine();
                    }

                    // Tell the C++ menu to exit after the selected action.
                    writer.write("9");
                    writer.newLine();
                    writer.flush();

                    String line;

                    while ((line = reader.readLine()) != null) {
                        result.append(line).append(System.lineSeparator());
                    }
                }

                int exitCode = process.waitFor();

                result.append(System.lineSeparator())
                      .append("Process finished (exit code ")
                      .append(exitCode)
                      .append(").");

                return result.toString();
            }

            @Override
            protected void done() {

                try {
                    outputArea.setText(get());
                    outputArea.setCaretPosition(0);
                    statusLabel.setText("Operation completed");

                } catch (InterruptedException ex) {

                    Thread.currentThread().interrupt();

                    outputArea.setText("Operation interrupted.");
                    statusLabel.setText("Interrupted");

                } catch (ExecutionException ex) {

                    Throwable cause = ex.getCause();

                    String message = cause != null
                        ? cause.getMessage()
                        : ex.getMessage();

                    outputArea.setText(
                        "Error running C++ program:\n" + message
                    );

                    statusLabel.setText("Error");

                } finally {
                    setActionButtonsEnabled(true);
                }
            }
        };

        worker.execute();
    }

    // =========================================
    // ENABLE / DISABLE BUTTONS
    // =========================================

    private void setActionButtonsEnabled(boolean enabled) {

        if (bookButton != null) {
            bookButton.setEnabled(enabled);
        }

        if (actionButtons != null) {
            for (JButton button : actionButtons) {
                button.setEnabled(enabled);
            }
        }
    }

    // =========================================
    // TRANSLUCENT PANEL
    // =========================================

    class GlassPanel extends JPanel {

        GlassPanel() {
            setOpaque(false);

            setBorder(new CompoundBorder(
                new LineBorder(
                    new Color(255, 255, 255, 100), 1, true
                ),
                new EmptyBorder(12, 12, 12, 12)
            ));
        }

        @Override
        protected void paintComponent(Graphics g) {

            Graphics2D g2 = (Graphics2D) g.create();

            g2.setRenderingHint(
                RenderingHints.KEY_ANTIALIASING,
                RenderingHints.VALUE_ANTIALIAS_ON
            );

            g2.setColor(glass);

            g2.fillRoundRect(
                0, 0, getWidth(), getHeight(), 22, 22
            );

            g2.dispose();

            super.paintComponent(g);
        }
    }

    // =========================================
    // BACKGROUND IMAGE PANEL
    // =========================================

    static class BackgroundImagePanel extends JPanel {

        private Image backgroundImage;

        BackgroundImagePanel() {

            File imageFile = new File(BACKGROUND_IMAGE);

            System.out.println(
                "Image path: " + imageFile.getAbsolutePath()
            );

            System.out.println(
                "Image exists: " + imageFile.exists()
            );

            if (imageFile.isFile()) {
                ImageIcon icon = new ImageIcon(
                    imageFile.getAbsolutePath()
                );

                backgroundImage = icon.getImage();
            }

            setOpaque(true);
        }

        @Override
        protected void paintComponent(Graphics g) {

            super.paintComponent(g);

            Graphics2D g2 = (Graphics2D) g.create();

            if (backgroundImage != null) {

                g2.drawImage(
                    backgroundImage,
                    0, 0,
                    getWidth(), getHeight(),
                    this
                );

                // Dark overlay improves text readability.
                g2.setColor(new Color(0, 0, 0, 75));

                g2.fillRect(
                    0, 0, getWidth(), getHeight()
                );

            } else {

                g2.setColor(new Color(25, 35, 50));

                g2.fillRect(
                    0, 0, getWidth(), getHeight()
                );
            }

            g2.dispose();
        }
    }

    // =========================================
    // MAIN METHOD
    // =========================================

    public static void main(String[] args) {

        SwingUtilities.invokeLater(() -> {

            try {
                UIManager.setLookAndFeel(
                    UIManager.getSystemLookAndFeelClassName()
                );

            } catch (Exception ignored) {
                // Use default Swing appearance if unavailable.
            }

            new MainGUI1();
        });
    }
}
