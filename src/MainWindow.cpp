#include "MainWindow.h"
#include <QSplitter>
#include <QTreeView>
#include <QTextEdit>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QLabel>
#include <QStatusBar>
#include <QShortcut> // For hotkey
#include <QApplication> // For QApplication::quit()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Main splitter
    splitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(splitter);

    // Left panel: Tree view
    treeView = new QTreeView(splitter);
    treeView->header()->hide();
    QStandardItemModel *model = new QStandardItemModel(this);
    QStandardItem *rootNode = model->invisibleRootItem();

    QStandardItem *mail = new QStandardItem("Mail");
    QStandardItem *inbox = new QStandardItem("Inbox");
    mail->appendRow(inbox);
    rootNode->appendRow(mail);
    
    QStandardItem *calendar = new QStandardItem("Calendar");
    rootNode->appendRow(calendar);

    QStandardItem *contacts = new QStandardItem("Contacts");
    rootNode->appendRow(contacts);

    QStandardItem *tasks = new QStandardItem("Tasks");
    rootNode->appendRow(tasks);

    treeView->setModel(model);
    treeView->expandAll();
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Right panel: Text edit (placeholder)
    textEdit = new QTextEdit(splitter);
    textEdit->setPlaceholderText("Email content will be displayed here.");

    // Add widgets to splitter
    splitter->addWidget(treeView);
    splitter->addWidget(textEdit);

    // Set initial sizes for the panels
    splitter->setSizes({200, 600});

    // Status bar
    modeLabel = new QLabel(this);
    statusBar()->addWidget(modeLabel);
    setMode(Mode::TREE); // Set initial mode

    // Set initial focus
    treeView->setFocus();

    // Hotkeys
    QShortcut *quitShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q), this);
    connect(quitShortcut, &QShortcut::activated, qApp, &QApplication::quit);

    // Window settings
    setWindowTitle("Postage");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setMode(Mode newMode)
{
    m_currentMode = newMode;
    switch (m_currentMode)
    {
        case Mode::TREE:
            modeLabel->setText("TREE");
            break;
        case Mode::NORMAL:
            modeLabel->setText("NORMAL");
            break;
        case Mode::INSERT:
            modeLabel->setText("INSERT");
            break;
        case Mode::VISUAL:
            modeLabel->setText("VISUAL");
            break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (m_currentMode == Mode::TREE)
    {
        QModelIndex currentIndex = treeView->currentIndex();
        switch (event->key())
        {
            case Qt::Key_J:
                treeView->setCurrentIndex(treeView->indexBelow(currentIndex));
                break;
            case Qt::Key_K:
                treeView->setCurrentIndex(treeView->indexAbove(currentIndex));
                break;
            case Qt::Key_L:
                if (treeView->model()->hasChildren(currentIndex) && !treeView->isExpanded(currentIndex))
                {
                    treeView->expand(currentIndex);
                }
                else
                {
                    treeView->setCurrentIndex(treeView->indexBelow(currentIndex));
                }
                break;
            case Qt::Key_H:
                if (treeView->model()->hasChildren(currentIndex) && treeView->isExpanded(currentIndex))
                {
                    treeView->collapse(currentIndex);
                }
                else
                {
                    treeView->setCurrentIndex(currentIndex.parent());
                }
                break;
            case Qt::Key_G:
                if (event->modifiers() & Qt::ShiftModifier) // 'G'
                {
                    QAbstractItemModel *model = treeView->model();
                    QModelIndex lastIndex = model->index(model->rowCount() - 1, 0);
                    treeView->setCurrentIndex(lastIndex);
                }
                else // 'g'
                {
                    treeView->setCurrentIndex(treeView->model()->index(0, 0));
                }
                break;
            default:
                QMainWindow::keyPressEvent(event);
        }
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}
