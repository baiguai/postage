#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QDialog>
#include <QIntValidator> // Required for QIntValidator

class QListWidget;
class QStackedWidget;
class QLineEdit; // Forward declaration for QLineEdit
class QComboBox; // Forward declaration for QComboBox

class AddAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAccountDialog(QWidget *parent = nullptr);
    
    QString accountName() const;
    QString emailAddress() const;

    QString smtpServer() const;
    int smtpPort() const;
    QString smtpSecurity() const;
    QString smtpUsername() const;
    QString smtpPassword() const; // New: SMTP Password Getter

    QString imapServer() const;
    int imapPort() const;
    QString imapSecurity() const;
    QString imapUsername() const;
    QString imapPassword() const; // New: IMAP Password Getter


private slots:
    void onStepChanged(int currentRow);
    void discoverSettings(); // New slot for auto-discovery

private:
    QListWidget *stepsList;
    QStackedWidget *pagesWidget;
    QLineEdit *accountNameEdit;
    QLineEdit *emailAddressEdit;

    // Sending (SMTP) fields
    QLineEdit *smtpServerEdit;
    QLineEdit *smtpPortEdit;
    QComboBox *smtpSecurityCombo;
    QLineEdit *smtpUsernameEdit;
    QLineEdit *smtpPasswordEdit; // New: SMTP Password

    // Receiving (IMAP/POP3) fields
    QLineEdit *imapServerEdit;
    QLineEdit *imapPortEdit;
    QComboBox *imapSecurityCombo;
    QLineEdit *imapUsernameEdit;
    QLineEdit *imapPasswordEdit; // New: IMAP Password
};

#endif // ADDACCOUNTDIALOG_H
