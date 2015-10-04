#include "IBFTreeView.h"

IBFTreeView::IBFTreeView(QWidget *aParent) : QTreeView(aParent)
{
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setDefaultDropAction(Qt::MoveAction);

    mContextMenu = new QMenu();

    QAction * actionMakeFile = new QAction(tr("New Note"), this);
    connect(actionMakeFile, SIGNAL(triggered()), this, SLOT(on_ActionMKFile()));
    mContextMenu->addAction(actionMakeFile);

    QAction * actionMakeDirectory = new QAction(tr("New Folder"), this);
    connect(actionMakeDirectory, SIGNAL(triggered()), this, SLOT(on_mIBFTreeView_ActionMKDir()));
    mContextMenu->addAction(actionMakeDirectory);

    QAction * actionMakeRoot = new QAction(tr("New Notebook"), this);
    connect(actionMakeRoot, SIGNAL(triggered()), this, SLOT(on_mIBFTreeView_ActionMKRoot()));
    mContextMenu->addAction(actionMakeRoot);

    mContextMenu->addSeparator();

    QAction * actionRenameDirectory = new QAction(tr("Rename"), this);
    connect(actionRenameDirectory, SIGNAL(triggered()), this, SLOT(on_mIBFTreeView_ActionRNDir()));
    mContextMenu->addAction(actionRenameDirectory);

    QAction * actionRemoveDirectory = new QAction(tr("Remove"), this);
    connect(actionRemoveDirectory, SIGNAL(triggered()), this, SLOT(on_mIBFTreeView_ActionRMDir()));
    mContextMenu->addAction(actionRemoveDirectory);
}

IBFTreeView::~IBFTreeView()
{

}

void IBFTreeView::on_ActionMKFile()
{
    QFileSystemModel * tFileSystemModel = (QFileSystemModel *) this->model();

    QString tFileName;

    if ( tFileSystemModel->fileInfo(this->currentIndex()).isDir() )
    {
        tFileName = tFileSystemModel->fileInfo(this->currentIndex()).absoluteFilePath() + "/" + "Untitled Note";
    }
    else
    {
        tFileName = tFileSystemModel->fileInfo(this->currentIndex()).absolutePath() + "/" + "Untitled Note";
    }

    int i = 1;

    while ( QFile::exists(tFileName + "(" + QString::number(i) + ")" + ".html") )
    {
        i++;
    }

    QFile tFile(tFileName + "(" + QString::number(i) + ")" + ".html");

    if ( !tFile.open(QFile::ReadWrite) )
    {
        QMessageBox::warning(this, tr("IBFNotebook"), tr("An error has occured:\n\nvoid IBFListView::on_mIBFListView_ActionMKFile()\ntFile.open(QFile::ReadWrite) returned FALSE"), QMessageBox::Ok);
        return;
    }

    QModelIndex iModelIndex = tFileSystemModel->index(tFileName + "(" + QString::number(i) + ")" + ".html");

    this->setCurrentIndex(iModelIndex);
    this->edit(iModelIndex);

    // do this using slots an signals
    //this->setWindowModified(true);
    this->SignalModified();
}

void IBFTreeView::on_mIBFTreeView_ActionMKRoot()
{
    QFileSystemModel * tFileSystemModel = (QFileSystemModel*) this->model();

    QString tDirName = tFileSystemModel->rootPath()  + "/New Directory ";

    QDir tDir;

    int i = 1;

    while ( tDir.exists(tDirName + QString::number(i)) )
    {
        i++;
    }

    tDir.mkdir(tDirName + QString::number(i));

    this->expand(this->currentIndex());
    tFileSystemModel->fetchMore(tFileSystemModel->index( tFileSystemModel->rootPath()));

    QModelIndex iModelIndex = tFileSystemModel->index(tDirName + QString::number(i));

    this->setCurrentIndex(iModelIndex);
    this->edit(iModelIndex);

    //this->setWindowModified(true);
    this->SignalModified();
}

void IBFTreeView::on_mIBFTreeView_ActionMKDir()
{
    QFileSystemModel * tFileSystemModel = (QFileSystemModel*) this->model();

    QString tDirName;

    if ( this->currentIndex().isValid() )
    {
        tDirName = tFileSystemModel->filePath( this->currentIndex() )  + "/New Directory ";
    }
    else
    {
        tDirName = tFileSystemModel->rootPath()  + "/New Directory ";
    }

    QDir tDir;

    int i = 1;

    while ( tDir.exists(tDirName + QString::number(i)) )
    {
        i++;
    }

    tDir.mkdir(tDirName + QString::number(i));

    this->expand(this->currentIndex());
    tFileSystemModel->fetchMore(tFileSystemModel->index( tFileSystemModel->rootPath()));

    QModelIndex iModelIndex = tFileSystemModel->index(tDirName + QString::number(i));

    this->setCurrentIndex(iModelIndex);
    this->edit(iModelIndex);

    //this->setWindowModified(true);
    this->SignalModified();
}

void IBFTreeView::on_mIBFTreeView_ActionRNDir()
{
    this->edit(this->currentIndex());

    //this->setWindowModified(true);
    this->SignalModified();
}

void IBFTreeView::on_mIBFTreeView_ActionRMDir()
{
    QFileSystemModel * tFileSystemModel = (QFileSystemModel*) this->model();

    tFileSystemModel->remove(this->currentIndex());
    tFileSystemModel->fetchMore(tFileSystemModel->index(tFileSystemModel->rootPath()));

    //this->setWindowModified(true);
    this->SignalModified();
}

void IBFTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    mContextMenu->exec(event->globalPos());
}
