#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QDialog>

class QListWidget;
class QStackedWidget;

class AddAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAccountDialog(QWidget *parent = nullptr);

private slots:
    void onStepChanged(int currentRow);

private:
    QListWidget *stepsList;
    QStackedWidget *pagesWidget;
};

#endif // ADDACCOUNTDIALOG_H
