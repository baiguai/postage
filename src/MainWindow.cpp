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
    toolLabel = new QLabel(this);
    statusBar()->addWidget(modeLabel);
    statusBar()->addPermanentWidget(toolLabel);
    setMode(Mode::TREE); // Set initial mode

    // Set initial tool
    setTool(Tool::MAIL); 
    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onCurrentTreeItemChanged);

    // Set initial focus
    treeView->setFocus();
    treeView->setCurrentIndex(model->index(0, 0, QModelIndex())); // Select 'Mail'
    
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

void MainWindow::setTool(Tool newTool)
{
    m_currentTool = newTool;
    switch (m_currentTool)
    {
        case Tool::MAIL:
            toolLabel->setText("MAIL");
            break;
        case Tool::CALENDAR:
            toolLabel->setText("CALENDAR");
            break;
        case Tool::CONTACTS:
            toolLabel->setText("CONTACTS");
            break;
        case Tool::TASKS:
            toolLabel->setText("TASKS");
            break;
        default:
            toolLabel->setText("UNKNOWN");
            break;
    }
}

void MainWindow::onCurrentTreeItemChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QModelIndex index = current;
    while(index.parent().isValid())
    {
        index = index.parent();
    }

    QString toolName = index.data().toString();
    if (toolName == "Mail") {
        setTool(Tool::MAIL);
    } else if (toolName == "Calendar") {
        setTool(Tool::CALENDAR);
    } else if (toolName == "Contacts") {
        setTool(Tool::CONTACTS);
    } else if (toolName == "Tasks") {
        setTool(Tool::TASKS);
    } else {
        setTool(Tool::UNKNOWN);
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
                case Qt::Key_J: {
                    QModelIndex nextIndex = treeView->indexBelow(currentIndex);
                    if (nextIndex.isValid()) {
                        treeView->setCurrentIndex(nextIndex);
                    }
                    return true; // Event handled
                }
                case Qt::Key_K: {
                    QModelIndex prevIndex = treeView->indexAbove(currentIndex);
                    if (prevIndex.isValid()) {
                        treeView->setCurrentIndex(prevIndex);
                    }
                    return true; // Event handled
                }
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
                case Qt::Key_E:
                    if (keyEvent->modifiers() == Qt::ShiftModifier) // 'E'
                    {
                        treeView->expandAll();
                    }
                    return true;
                case Qt::Key_C:
                    if (keyEvent->modifiers() == Qt::ShiftModifier) // 'C'
                    {
                        treeView->collapseAll();
                    }
                    return true;
                default:
                    break; // Let base class handle it
            }
        }
    }
    // Pass the event on to the parent class
    return QMainWindow::eventFilter(obj, event);
}

