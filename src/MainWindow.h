#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "AddAccountDialog.h" // Include for AddAccountDialog class

class QSplitter;
class QTreeView;
class QTextEdit;
class QLabel;
class QStandardItem; // Forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum class Mode {
        TREE,
        NORMAL,
        INSERT,
        VISUAL
    };

    enum class Tool {
        MAIL,
        CALENDAR,
        CONTACTS,
        TASKS,
        UNKNOWN
    };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onCurrentTreeItemChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    void setMode(Mode newMode);
    void setTool(Tool newTool);
    void createAccountConfig(const QString &accountName, const QString &emailAddress, const AddAccountDialog &dialog);
    void loadAccounts();
    void deleteAccount(const QModelIndex &indexToDelete); // New: Delete an account

    QSplitter *splitter;
    QTreeView *treeView;
    QTextEdit *textEdit;
    QLabel *modeLabel;
    QLabel *toolLabel;
    QStandardItem *m_mailRootItem;
    Mode m_currentMode;
    Tool m_currentTool;
};

#endif // MAINWINDOW_H
