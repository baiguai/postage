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

    QWidget *sendingPage = new QWidget;
    QVBoxLayout *sendingLayout = new QVBoxLayout(sendingPage);
    sendingLayout->addWidget(new QLabel("Sending (SMTP) server settings will go here."));
    sendingPage->setLayout(sendingLayout);
    pagesWidget->addWidget(sendingPage);
    QWidget *receivingPage = new QWidget;
    QVBoxLayout *receivingLayout = new QVBoxLayout(receivingPage);
    receivingLayout->addWidget(new QLabel("Receiving (IMAP/POP3) server settings will go here."));
    receivingPage->setLayout(receivingLayout);
    pagesWidget->addWidget(receivingPage);

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
    resize(500, 300);
}

QString AddAccountDialog::accountName() const
{
    return accountNameEdit->text();
}

QString AddAccountDialog::emailAddress() const
{
    return emailAddressEdit->text();
}

void AddAccountDialog::onStepChanged(int currentRow)
{
    pagesWidget->setCurrentIndex(currentRow);
}
