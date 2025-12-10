#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QDialog>

class QListWidget;
class QStackedWidget;
class QLineEdit; // Forward declaration for QLineEdit

class AddAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAccountDialog(QWidget *parent = nullptr);
    
    QString accountName() const;
    QString emailAddress() const;

private slots:
    void onStepChanged(int currentRow);

private:
    QListWidget *stepsList;
    QStackedWidget *pagesWidget;
    QLineEdit *accountNameEdit;
    QLineEdit *emailAddressEdit;
};

#endif // ADDACCOUNTDIALOG_H
