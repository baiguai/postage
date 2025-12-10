#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void setMode(Mode newMode);

    QSplitter *splitter;
    QTreeView *treeView;
    QTextEdit *textEdit;
    QLabel *modeLabel;
    Mode m_currentMode;
};

#endif // MAINWINDOW_H
