#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

class QSplitter;
class QTreeView;
class QTextEdit;
class QLabel;

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

    QSplitter *splitter;
    QTreeView *treeView;
    QTextEdit *textEdit;
    QLabel *modeLabel;
    QLabel *toolLabel;
    Mode m_currentMode;
    Tool m_currentTool;
};

#endif // MAINWINDOW_H
