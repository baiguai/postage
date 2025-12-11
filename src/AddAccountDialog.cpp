#include "AddAccountDialog.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QLineEdit> // Added for input fields
#include <QFormLayout> // Added for form layout
#include <QComboBox> // Added for security selection
#include <QUrl> // For domain parsing
#include <QPushButton> // For the OAuth authenticate button
#include <QDesktopServices> // For opening URLs in browser

AddAccountDialog::AddAccountDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Add New Account");

    // Main horizontal splitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    // Left panel: List of steps
    stepsList = new QListWidget(splitter);
    stepsList->addItems({"Email", "Sending", "Receiving"});
    stepsList->setMaximumWidth(150);

    // Right panel: Stack of pages
    pagesWidget = new QStackedWidget(splitter);

    // Create a page for each step
    QWidget *emailPage = new QWidget;
    QFormLayout *emailFormLayout = new QFormLayout(emailPage);
    accountNameEdit = new QLineEdit(emailPage); // Assign to member variable
    emailAddressEdit = new QLineEdit(emailPage); // Assign to member variable
    authTypeCombo = new QComboBox(emailPage); // Assign to member variable
    authTypeCombo->addItems({"Password", "OAuth2 (Gmail)"});
    authButton = new QPushButton("Authenticate with Google", emailPage); // Assign to member variable
    authButton->setVisible(false); // Hidden by default

    emailFormLayout->addRow("Account Name:", accountNameEdit);
    emailFormLayout->addRow("Email Address:", emailAddressEdit);
    emailFormLayout->addRow("Authentication:", authTypeCombo);
    emailFormLayout->addRow("", authButton); // Add button without a label
    emailPage->setLayout(emailFormLayout);
    pagesWidget->addWidget(emailPage);
    connect(emailAddressEdit, &QLineEdit::editingFinished, this, &AddAccountDialog::discoverSettings); // Connect for auto-discovery
    connect(authTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AddAccountDialog::onAuthTypeChanged);
    connect(authButton, &QPushButton::clicked, this, &AddAccountDialog::startOAuthFlow);

    QWidget *sendingPage = new QWidget;
    QFormLayout *sendingFormLayout = new QFormLayout(sendingPage);
    smtpServerEdit = new QLineEdit(sendingPage);
    smtpPortEdit = new QLineEdit(sendingPage);
    smtpPortEdit->setValidator(new QIntValidator(1, 65535, this)); // Ports 1-65535
    smtpSecurityCombo = new QComboBox(sendingPage);
    smtpSecurityCombo->addItems({"SSL/TLS", "STARTTLS", "None"});
    smtpUsernameEdit = new QLineEdit(sendingPage);

    sendingFormLayout->addRow("Server:", smtpServerEdit);
    sendingFormLayout->addRow("Port:", smtpPortEdit);
    sendingFormLayout->addRow("Security:", smtpSecurityCombo);
    sendingFormLayout->addRow("Username:", smtpUsernameEdit);
    smtpPasswordEdit = new QLineEdit(sendingPage);
    smtpPasswordEdit->setEchoMode(QLineEdit::Password);
    sendingFormLayout->addRow("Password:", smtpPasswordEdit);
    sendingPage->setLayout(sendingFormLayout);
    pagesWidget->addWidget(sendingPage);

    QWidget *receivingPage = new QWidget;
    QFormLayout *receivingFormLayout = new QFormLayout(receivingPage);
    imapServerEdit = new QLineEdit(receivingPage);
    imapPortEdit = new QLineEdit(receivingPage);
    imapPortEdit->setValidator(new QIntValidator(1, 65535, this)); // Ports 1-65535
    imapSecurityCombo = new QComboBox(receivingPage);
    imapSecurityCombo->addItems({"SSL/TLS", "STARTTLS", "None"});
    imapUsernameEdit = new QLineEdit(receivingPage);
    
    receivingFormLayout->addRow("Server:", imapServerEdit);
    receivingFormLayout->addRow("Port:", imapPortEdit);
    receivingFormLayout->addRow("Security:", imapSecurityCombo);
    receivingFormLayout->addRow("Username:", imapUsernameEdit);
    imapPasswordEdit = new QLineEdit(receivingPage);
    imapPasswordEdit->setEchoMode(QLineEdit::Password);
    receivingFormLayout->addRow("Password:", imapPasswordEdit);
    receivingPage->setLayout(receivingFormLayout);
    pagesWidget->addWidget(receivingPage);

    // Set all relevant QLineEdit widgets to read-only initially (will be enabled/disabled by onAuthTypeChanged)
    smtpServerEdit->setReadOnly(true);
    smtpPortEdit->setReadOnly(true);
    smtpUsernameEdit->setReadOnly(true);
    smtpPasswordEdit->setReadOnly(true);
    imapServerEdit->setReadOnly(true);
    imapPortEdit->setReadOnly(true);
    imapUsernameEdit->setReadOnly(true);
    imapPasswordEdit->setReadOnly(true);

    splitter->addWidget(stepsList);
    splitter->addWidget(pagesWidget);
    splitter->setSizes({100, 300});

    // Dialog buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Main layout for the dialog
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(splitter);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    // Connect the list widget to the stacked widget
    connect(stepsList, &QListWidget::currentRowChanged, pagesWidget, &QStackedWidget::setCurrentIndex);
    
    // Set initial state
    stepsList->setCurrentRow(0);
    resize(700, 400); // Adjusted size to accommodate more fields

    // Trigger initial state setup
    onAuthTypeChanged(authTypeCombo->currentIndex());
}

QString AddAccountDialog::accountName() const
{
    return accountNameEdit->text();
}

QString AddAccountDialog::emailAddress() const
{
    return emailAddressEdit->text();
}

QString AddAccountDialog::smtpServer() const
{
    return smtpServerEdit->text();
}

int AddAccountDialog::smtpPort() const
{
    return smtpPortEdit->text().toInt();
}

QString AddAccountDialog::smtpSecurity() const
{
    return smtpSecurityCombo->currentText();
}

QString AddAccountDialog::smtpUsername() const
{
    return smtpUsernameEdit->text();
}

QString AddAccountDialog::smtpPassword() const
{
    return smtpPasswordEdit->text();
}

QString AddAccountDialog::imapServer() const
{
    return imapServerEdit->text();
}

int AddAccountDialog::imapPort() const
{
    return imapPortEdit->text().toInt();
}

QString AddAccountDialog::imapSecurity() const
{
    return imapSecurityCombo->currentText();
}

QString AddAccountDialog::imapUsername() const
{
    return imapUsernameEdit->text();
}

QString AddAccountDialog::imapPassword() const
{
    return imapPasswordEdit->text();
}

void AddAccountDialog::onStepChanged(int currentRow)
{
    pagesWidget->setCurrentIndex(currentRow);
}

void AddAccountDialog::discoverSettings()
{
    QString email = emailAddressEdit->text();
    if (email.isEmpty() || !email.contains('@')) {
        return; // No valid email to discover from
    }

    QString domain = email.section('@', 1);

    // Simple auto-discovery logic (placeholder)
    if (domain == "gmail.com") {
        authTypeCombo->setCurrentIndex(1); // Select OAuth2
        // SMTP
        smtpServerEdit->setText("smtp.gmail.com");
        smtpPortEdit->setText("587");
        smtpSecurityCombo->setCurrentText("STARTTLS");
        smtpUsernameEdit->setText(email);
        // IMAP
        imapServerEdit->setText("imap.gmail.com");
        imapPortEdit->setText("993");
        imapSecurityCombo->setCurrentText("SSL/TLS");
        imapUsernameEdit->setText(email);
    } else if (domain == "outlook.com" || domain == "hotmail.com" || domain == "live.com") {
        authTypeCombo->setCurrentIndex(0); // Select Password
        // SMTP
        smtpServerEdit->setText("smtp.office365.com");
        smtpPortEdit->setText("587");
        smtpSecurityCombo->setCurrentText("STARTTLS");
        smtpUsernameEdit->setText(email);
        // IMAP
        imapServerEdit->setText("outlook.office365.com");
        imapPortEdit->setText("993");
        imapSecurityCombo->setCurrentText("SSL/TLS");
        imapUsernameEdit->setText(email);
    } else {
        authTypeCombo->setCurrentIndex(0); // Select Password
        // Generic inference
        smtpServerEdit->setText("smtp." + domain);
        smtpPortEdit->setText("587"); // Common default
        smtpSecurityCombo->setCurrentText("STARTTLS"); // Common default
        smtpUsernameEdit->setText(email);

        imapServerEdit->setText("imap." + domain);
        imapPortEdit->setText("993"); // Common default
        imapSecurityCombo->setCurrentText("SSL/TLS"); // Common default
        imapUsernameEdit->setText(email);
    }
}

void AddAccountDialog::onAuthTypeChanged(int index)
{
    bool isPasswordAuth = (index == 0); // 0: Password, 1: OAuth2

    smtpServerEdit->setReadOnly(!isPasswordAuth);
    smtpPortEdit->setReadOnly(!isPasswordAuth);
    smtpSecurityCombo->setEnabled(isPasswordAuth);
    smtpUsernameEdit->setReadOnly(!isPasswordAuth);
    smtpPasswordEdit->setReadOnly(!isPasswordAuth);

    imapServerEdit->setReadOnly(!isPasswordAuth);
    imapPortEdit->setReadOnly(!isPasswordAuth);
    imapSecurityCombo->setEnabled(isPasswordAuth);
    imapUsernameEdit->setReadOnly(!isPasswordAuth);
    imapPasswordEdit->setReadOnly(!isPasswordAuth);

    authButton->setVisible(!isPasswordAuth); // Show/hide OAuth button
}

void AddAccountDialog::startOAuthFlow()
{
    // These need to come from your Google API Console project
    const QString CLIENT_ID = "YOUR_CLIENT_ID_HERE"; // Replace with your actual client ID
    const QString REDIRECT_URI = "http://127.0.0.1:8888"; // Use a local port
    const QString SCOPES = "https://www.googleapis.com/auth/gmail.compose https://www.googleapis.com/auth/gmail.send https://www.googleapis.com/auth/gmail.readonly https://www.googleapis.com/auth/userinfo.email";
    
    QString authUrl = QString("https://accounts.google.com/o/oauth2/v2/auth?"
                              "client_id=%1&"
                              "redirect_uri=%2&"
                              "scope=%3&"
                              "response_type=code&"
                              "access_type=offline") // Request a refresh token
                        .arg(CLIENT_ID)
                        .arg(REDIRECT_URI)
                        .arg(SCOPES);

    QDesktopServices::openUrl(QUrl(authUrl));
    
    // For now, this just opens the browser.
    // In the next step, we'll need a QTcpServer to intercept the redirect.
}
