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
#include <QKeyEvent>

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
    treeView->installEventFilter(this); // Install event filter

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

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == treeView && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (m_currentMode == Mode::TREE)
        {
            QModelIndex currentIndex = treeView->currentIndex();
            switch (keyEvent->key())
            {
                case Qt::Key_J:
                    treeView->setCurrentIndex(treeView->indexBelow(currentIndex));
                    return true; // Event handled
                case Qt::Key_K:
                    treeView->setCurrentIndex(treeView->indexAbove(currentIndex));
                    return true; // Event handled
                case Qt::Key_L:
                    if (treeView->model()->hasChildren(currentIndex) && !treeView->isExpanded(currentIndex))
                    {
                        treeView->expand(currentIndex);
                    }
                    else if (treeView->model()->hasChildren(currentIndex))
                    {
                        treeView->setCurrentIndex(treeView->model()->index(0, 0, currentIndex));
                    }
                    return true; // Event handled
                case Qt::Key_H:
                    if (treeView->model()->hasChildren(currentIndex) && treeView->isExpanded(currentIndex))
                    {
                        treeView->collapse(currentIndex);
                    }
                    else if (currentIndex.parent().isValid())
                    {
                        treeView->setCurrentIndex(currentIndex.parent());
                    }
                    return true; // Event handled
                case Qt::Key_G:
                    if (keyEvent->modifiers() == Qt::ShiftModifier) // 'G'
                    {
                        QAbstractItemModel *model = treeView->model();
                        QModelIndex lastIndex;
                        int lastRow = model->rowCount() - 1;
                        if (lastRow >= 0) {
                           lastIndex = model->index(lastRow, 0);
                           while(model->rowCount(lastIndex) > 0 && treeView->isExpanded(lastIndex)) {
                               lastIndex = model->index(model->rowCount(lastIndex)-1, 0, lastIndex);
                           }
                        }
                        treeView->setCurrentIndex(lastIndex);
                    }
                    else if (keyEvent->modifiers() == Qt::NoModifier) // 'g'
                    {
                        treeView->setCurrentIndex(treeView->model()->index(0, 0));
                    }
                    return true; // Event handled
                default:
                    break; // Let base class handle it
            }
        }
    }
    // Pass the event on to the parent class
    return QMainWindow::eventFilter(obj, event);
}

