#include "MainWindow.h"
#include <QSplitter>
#include <QTreeView>
#include <QTextEdit>
#include <QStandardItemModel>
#include <QHeaderView>

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

    // Window settings
    setWindowTitle("Postage");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
}
