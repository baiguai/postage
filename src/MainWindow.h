#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSplitter;
class QTreeView;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QSplitter *splitter;
    QTreeView *treeView;
    QTextEdit *textEdit;
};

#endif // MAINWINDOW_H
