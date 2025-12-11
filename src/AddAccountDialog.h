#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QDialog>
#include <QIntValidator> // Required for QIntValidator

class QListWidget;
class QStackedWidget;
class QLineEdit; // Forward declaration for QLineEdit
class QComboBox; // Forward declaration for QComboBox
class QPushButton; // Forward declaration for QPushButton
class QTcpServer; // Forward declaration for QTcpServer
class QNetworkAccessManager; // Forward declaration for QNetworkAccessManager
class QNetworkReply; // Forward declaration for QNetworkReply

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
    QString smtpPassword() const; 

    QString imapServer() const;
    int imapPort() const;
    QString imapSecurity() const;
    QString imapUsername() const;
    QString imapPassword() const;

    QString accessToken() const; // OAuth Access Token Getter
    QString refreshToken() const; // OAuth Refresh Token Getter


private slots:
    void onStepChanged(int currentRow);
    void discoverSettings();
    void onAuthTypeChanged(int index);
    void startOAuthFlow();
    void onNewConnection(); // Handle new TCP connection (for redirect)
    void onReadyRead(); // Read data from TCP socket (for auth code)
    void onReplyFinished(QNetworkReply *reply); // Handle token exchange response

private:
    void exchangeCodeForTokens(); // Exchange auth code for tokens

    QListWidget *stepsList;
    QStackedWidget *pagesWidget;
    QLineEdit *accountNameEdit;
    QLineEdit *emailAddressEdit;
    QComboBox *authTypeCombo;
    QPushButton *authButton;

    // Sending (SMTP) fields
    QLineEdit *smtpServerEdit;
    QLineEdit *smtpPortEdit;
    QComboBox *smtpSecurityCombo;
    QLineEdit *smtpUsernameEdit;
    QLineEdit *smtpPasswordEdit;

    // Receiving (IMAP/POP3) fields
    QLineEdit *imapServerEdit;
    QLineEdit *imapPortEdit;
    QComboBox *imapSecurityCombo;
    QLineEdit *imapUsernameEdit;
    QLineEdit *imapPasswordEdit;

    // OAuth members
    QTcpServer *replyServer;
    QNetworkAccessManager *networkManager;
    QString m_authCode;
    QString m_accessToken;
    QString m_refreshToken;
    QString m_redirectUri;
};

#endif // ADDACCOUNTDIALOG_H
