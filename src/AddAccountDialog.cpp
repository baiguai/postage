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
    
    emailFormLayout->addRow("Account Name:", accountNameEdit);
    emailFormLayout->addRow("Email Address:", emailAddressEdit);
    emailPage->setLayout(emailFormLayout);
    pagesWidget->addWidget(emailPage);
    connect(emailAddressEdit, &QLineEdit::editingFinished, this, &AddAccountDialog::discoverSettings); // Connect for auto-discovery

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
    receivingPage->setLayout(receivingFormLayout);
    pagesWidget->addWidget(receivingPage);

    // Set all relevant QLineEdit widgets to read-only initially
    smtpServerEdit->setReadOnly(true);
    smtpPortEdit->setReadOnly(true);
    smtpUsernameEdit->setReadOnly(true);
    imapServerEdit->setReadOnly(true);
    imapPortEdit->setReadOnly(true);
    imapUsernameEdit->setReadOnly(true);

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
        smtpServerEdit->setText("smtp.gmail.com");
        smtpPortEdit->setText("587");
        smtpSecurityCombo->setCurrentText("STARTTLS");
        smtpUsernameEdit->setText(email);

        imapServerEdit->setText("imap.gmail.com");
        imapPortEdit->setText("993");
        imapSecurityCombo->setCurrentText("SSL/TLS");
        imapUsernameEdit->setText(email);
    } else if (domain == "outlook.com" || domain == "hotmail.com" || domain == "live.com") {
        smtpServerEdit->setText("smtp.office365.com");
        smtpPortEdit->setText("587");
        smtpSecurityCombo->setCurrentText("STARTTLS");
        smtpUsernameEdit->setText(email);

        imapServerEdit->setText("outlook.office365.com");
        imapPortEdit->setText("993");
        imapSecurityCombo->setCurrentText("SSL/TLS");
        imapUsernameEdit->setText(email);
    } else {
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
