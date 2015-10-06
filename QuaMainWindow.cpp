#include "QuaMainWindow.h"

QuaMainWindow::QuaMainWindow(QWidget * aParent) : QMainWindow(aParent)
{
    mIBFDocument = NULL;
    mIBFTemporaryDir = NULL;
    mIBFFileSystemModel = NULL;

    mIBFSplitter = new QSplitter(this);
    mIBFTreeView = new IBFTreeView(this);
    mQuaTextEdit = new QuaTextEdit(this);

    mIBFSplitter->addWidget(mIBFTreeView);
    mIBFSplitter->addWidget(mQuaTextEdit);

    this->setCentralWidget(mIBFSplitter);

    mIBFSplitter->setStretchFactor(0, 1);
    mIBFSplitter->setStretchFactor(1, 2);

    mIBFTreeView->setEnabled(false);
    mQuaTextEdit->setEnabled(false);

    this->setWindowTitle("No Document [*] - QuaNotebook");
    //this->setWindowIcon(QIcon("Icons/QNotes.png"));
    this->setWindowModified(false);

    this->IBFSetupMenuFile();
    this->IBFSetupConnections();

    this->showMaximized();
}

QuaMainWindow::~QuaMainWindow()
{
    if ( !mIBFDocument )
    {
        mIBFDocument->close();

        delete mIBFDocument;
        mIBFDocument = NULL;
    }

    if ( !mIBFTemporaryDir )
    {
        mIBFTemporaryDir->remove();

        delete mIBFTemporaryDir;
        mIBFTemporaryDir = NULL;
    }

   if ( !mIBFFileSystemModel )
   {
       delete mIBFFileSystemModel;
       mIBFFileSystemModel = NULL;
   }
}


bool QuaMainWindow::IBFNewDocument()
{
    if ( this->isWindowModified() )
    {
        QMessageBox::StandardButton tStandardButton;

        tStandardButton = QMessageBox::warning(this, tr("Save document?"), tr("The document has been modified. Do you want to save your changes? Your changes will be lost if you don't save them."), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if ( tStandardButton == QMessageBox::Save )
        {
            if ( !this->IBFSaveDocument() )
            {
                QMessageBox::critical(NULL, "Critical Error", "Something went wrong with this->IBFSaveDocument()...", QMessageBox::Ok);
                return false;
            }
        }
        else if ( tStandardButton == QMessageBox::Cancel )
        {
            return false;
        }
    }

    //QMessageBox::critical(NULL, "Critical Error", "So far so good...", QMessageBox::Ok);

    if ( !this->QuaCloseDocument() )
    {
        QMessageBox::critical(NULL, "Critical Error", "Something went wrong with this->QuaCloseDocument()...", QMessageBox::Ok);
        return false;
    }

    mIBFTemporaryDir = new QTemporaryDir();

    if ( !mIBFTemporaryDir->isValid() )
    {
        return false;
    }

    // create one folder and one file.

    mIBFFileSystemModel = new QFileSystemModel();
    mIBFFileSystemModel->setRootPath(mIBFTemporaryDir->path());
    mIBFFileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    mIBFFileSystemModel->setReadOnly(false);

    mIBFTreeView->setModel(mIBFFileSystemModel);
    mIBFTreeView->setRootIndex(mIBFFileSystemModel->index(mIBFTemporaryDir->path()));
    mIBFTreeView->setHeaderHidden(true);
    mIBFTreeView->setColumnHidden(1, true);
    mIBFTreeView->setColumnHidden(2, true);
    mIBFTreeView->setColumnHidden(3, true);
    mIBFTreeView->setWordWrap(true);

    QObject::connect(mIBFTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(on_mIBFTreeView_selectionChanged_triggered(const QModelIndex &, const QModelIndex &)));

    mIBFTreeView->setEnabled(true);
    mIBFTreeView->setFocus();

    this->setWindowModified(true);
    this->setWindowFilePath("");
    this->IBFUpdateTitle();

    return true;
}

bool QuaMainWindow::IBFOpenDocument()
{
    QString tFileName = QFileDialog::getOpenFileName(this, tr("Open File..."), QString(), tr("ZIP-Files (*.zip);;All Files (*)"));

    if ( tFileName.isEmpty() )
    {
        return false;
    }

    if ( !QFile::exists(tFileName) )
    {
        return false;
    }

    if ( !this->QuaCloseDocument() )
    {
        QMessageBox::critical(NULL, "Critical Error", "Something went wrong with this->QuaCloseDocument()...", QMessageBox::Ok);
        return false;
    }

    mIBFDocument = new QFile(tFileName);

    if ( !mIBFDocument->open(QFile::ReadOnly) )
    {
        return false;
    }

    mIBFTemporaryDir = new QTemporaryDir();

    if ( !mIBFTemporaryDir->isValid() )
    {
        return false;
    }

    if ( JlCompress::extractDir(tFileName, mIBFTemporaryDir->path()).isEmpty() )
    {
        QMessageBox::critical(NULL, "Critical Error", "Something went wrong with JlCompress::extractDir(aString, mTemporaryDir->path()).isEmpty()...", QMessageBox::Ok);
        return false;
    }

    mIBFFileSystemModel = new QFileSystemModel();
    mIBFFileSystemModel->setRootPath(mIBFTemporaryDir->path());
    mIBFFileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    mIBFFileSystemModel->setReadOnly(false);

    mIBFTreeView->setModel(mIBFFileSystemModel);
    mIBFTreeView->setRootIndex(mIBFFileSystemModel->index(mIBFTemporaryDir->path()));
    mIBFTreeView->setHeaderHidden(true);
    mIBFTreeView->setColumnHidden(1, true);
    mIBFTreeView->setColumnHidden(2, true);
    mIBFTreeView->setColumnHidden(3, true);
    mIBFTreeView->setWordWrap(true);

    QObject::connect(mIBFTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(on_mIBFTreeView_selectionChanged_triggered(const QModelIndex &, const QModelIndex &)));
    QObject::connect(mIBFFileSystemModel, SIGNAL(fileRenamed(QString,QString,QString)), this, SLOT(on_mIBFFileSystemModelDirs_fileRenamed_triggered(QString,QString,QString)));

    mIBFTreeView->setEnabled(true);
    mIBFTreeView->setFocus();

    this->setWindowModified(false);
    this->setWindowFilePath(tFileName);
    this->IBFUpdateTitle();

    return true;
}

bool QuaMainWindow::IBFSaveDocument()
{
    QString tFileName = this->windowFilePath();

    if ( mQuaTextEdit->document()->isModified() )
    {
        mQuaTextEdit->IBFSaveToFile(mQuaTextEdit->windowFilePath());
    }

    if ( tFileName.isEmpty() )
    {
        tFileName = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("ZIP-Files (*.zip);;All Files (*)"));

        if ( tFileName.isEmpty() )
        {
            return false;
        }

        if (! (tFileName.endsWith(".zip", Qt::CaseInsensitive) ) )
        {
            tFileName += ".zip";
        }
    }

    if ( QFile::exists(tFileName) )
    {
        QFile::remove(tFileName);
    }

    if ( !JlCompress::compressDir(tFileName, mIBFTemporaryDir->path()))
    {
        QMessageBox::critical(NULL, "Critical Error", "Something went wrong with JlCompress::compressDir(tFileName, mTemporaryDir->path())...", QMessageBox::Ok);
        return false;
    }

    if ( !QFile::exists(tFileName) )
    {
        QMessageBox::critical(NULL, "Critical Error", "Something went wrong with QFile::exists(tFileName)...", QMessageBox::Ok);
        return false;
    }

    mIBFDocument = new QFile(tFileName);

    if ( !mIBFDocument->open(QFile::ReadOnly) )
    {
        QMessageBox::critical(NULL, "Critical Error", "Something went wrong with mIBFDocument->open(QFile::ReadOnly)...", QMessageBox::Ok);
        return false;
    }

    this->setWindowModified(false);
    this->setWindowFilePath(tFileName);
    this->IBFUpdateTitle();

    return true;
}

bool QuaMainWindow::QuaSaveAsDocument()
{
    QString tFileName;

    if ( mQuaTextEdit->document()->isModified() )
    {
        mQuaTextEdit->IBFSaveToFile(mQuaTextEdit->windowFilePath());
    }

    tFileName = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("ZIP-Files (*.zip);;All Files (*)"));

    if ( tFileName.isEmpty() )
    {
        return false;
    }

    if (! (tFileName.endsWith(".zip", Qt::CaseInsensitive) ) )
    {
        tFileName += ".zip";
    }

    if ( !JlCompress::compressDir(tFileName, mIBFTemporaryDir->path()))
    {
        QMessageBox::critical(NULL, "Critical Error", "Something went wrong with JlCompress::compressDir(tFileName, mTemporaryDir->path())...", QMessageBox::Ok);
        return false;
    }

    if ( !QFile::exists(tFileName) )
    {
        QMessageBox::critical(NULL, "Critical Error", "Something went wrong with QFile::exists(tFileName)...", QMessageBox::Ok);
        return false;
    }

    //
    //  Open Saved Document
    //

    if ( tFileName.isEmpty() )
    {
        return false;
    }

    if ( !QFile::exists(tFileName) )
    {
        return false;
    }

    if ( !this->QuaCloseDocument() )
    {
        QMessageBox::critical(NULL, "Critical Error", "Something went wrong with this->QuaCloseDocument()...", QMessageBox::Ok);
        return false;
    }

    mIBFDocument = new QFile(tFileName);

    if ( !mIBFDocument->open(QFile::ReadOnly) )
    {
        return false;
    }

    mIBFTemporaryDir = new QTemporaryDir();

    if ( !mIBFTemporaryDir->isValid() )
    {
        return false;
    }

    if ( JlCompress::extractDir(tFileName, mIBFTemporaryDir->path()).isEmpty() )
    {
        QMessageBox::critical(NULL, "Critical Error", "Something went wrong with JlCompress::extractDir(aString, mTemporaryDir->path()).isEmpty()...", QMessageBox::Ok);
        return false;
    }

    mIBFFileSystemModel = new QFileSystemModel();
    mIBFFileSystemModel->setRootPath(mIBFTemporaryDir->path());
    mIBFFileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    mIBFFileSystemModel->setReadOnly(false);

    mIBFTreeView->setModel(mIBFFileSystemModel);
    mIBFTreeView->setRootIndex(mIBFFileSystemModel->index(mIBFTemporaryDir->path()));
    mIBFTreeView->setHeaderHidden(true);
    mIBFTreeView->setColumnHidden(1, true);
    mIBFTreeView->setColumnHidden(2, true);
    mIBFTreeView->setColumnHidden(3, true);
    mIBFTreeView->setWordWrap(true);

    QObject::connect(mIBFTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(on_mIBFTreeView_selectionChanged_triggered(const QModelIndex &, const QModelIndex &)));
    QObject::connect(mIBFFileSystemModel, SIGNAL(fileRenamed(QString,QString,QString)), this, SLOT(on_mIBFFileSystemModelDirs_fileRenamed_triggered(QString,QString,QString)));

    mIBFTreeView->setEnabled(true);
    mIBFTreeView->setFocus();

    this->setWindowModified(false);
    this->setWindowFilePath(tFileName);
    this->IBFUpdateTitle();

    return true;
}


bool QuaMainWindow::QuaCloseDocument()
{
    if ( mIBFDocument != NULL )
    {
        mIBFDocument->close();

        delete mIBFDocument;
        mIBFDocument = NULL;
    }

    if ( mIBFTemporaryDir != NULL )
    {
        mIBFTemporaryDir->remove();

        delete mIBFTemporaryDir;
        mIBFTemporaryDir = NULL;
    }

   if ( mIBFFileSystemModel != NULL )
   {
       delete mIBFFileSystemModel;
       mIBFFileSystemModel = NULL;
   }

   if ( mIBFTreeView->isEnabled() )
   {
       // disable and remove model
   }

   if ( mQuaTextEdit->isEnabled() )
   {
       // disable and clear
   }

   mIBFTreeView->setEnabled(false);
   mQuaTextEdit->setEnabled(false);

   this->setWindowModified(false);
   this->setWindowFilePath("");
   this->IBFUpdateTitle();

   return true;
}

bool QuaMainWindow::IBFSetupMenuFile()
{
    QAction * actionNew = new QAction(tr("&New"), this);
    actionNew->setShortcut(QKeySequence::New);
    connect(actionNew, SIGNAL(triggered()), this, SLOT(on_actionNew_triggered()));
    addAction(actionNew);

    QAction * actionOpen = new QAction(tr("&Open..."), this);
    actionOpen->setShortcut(QKeySequence::Open);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(on_actionOpen_triggered()));
    addAction(actionOpen);

    QAction * actionSave = new QAction(tr("&Save"), this);
    actionSave->setShortcut(QKeySequence::Save);
    connect(actionSave, SIGNAL(triggered()), this, SLOT(on_actionSave_triggered()));
    addAction(actionSave);

    QAction * actionSaveAs = new QAction(tr("Save &As..."), this);
    actionSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(on_actionSaveAs_triggered()));
    addAction(actionSaveAs);

    QAction * actionQuit = new QAction(tr("&Quit"), this);
    actionQuit->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    addAction(actionQuit);

//    QAction * actionPreview = new QAction(tr("Preview"), this);
//    actionPreview->setShortcut(QKeySequence(tr("Ctrl+P")));
//    connect(actionPreview, SIGNAL(triggered()), this, SLOT(on_actionPreview_triggered()));
//    //tViewMenu->addAction(actionPreview);
//    addAction(actionPreview);

    return true;
}

bool QuaMainWindow::IBFSetupConnections()
{
    connect(mQuaTextEdit, SIGNAL(signal_actionNew()), this, SLOT(on_actionNew_triggered()));
    connect(mQuaTextEdit, SIGNAL(signal_actionOpen()), this, SLOT(on_actionOpen_triggered()));
    connect(mQuaTextEdit, SIGNAL(signal_actionSave()), this, SLOT(on_actionSave_triggered()));
    connect(mQuaTextEdit, SIGNAL(signal_actionSaveAs()), this, SLOT(on_actionSaveAs_triggered()));
    connect(mQuaTextEdit, SIGNAL(signal_actionQuit()), this, SLOT(on_actionQuit_triggered()));

    connect(mQuaTextEdit, SIGNAL(textChanged()), this, SLOT(on_SignalModified()));
    connect(mIBFTreeView, SIGNAL(SignalModified()), this, SLOT(on_SignalModified()));

    return true;
}

bool QuaMainWindow::IBFUpdateTitle()
{
    if ( this->windowFilePath().isEmpty() )
    {
        setWindowTitle("Untitled Document [*] - QuaNotebook");
    }
    else
    {
        setWindowTitle( QFileInfo(this->windowFilePath()).fileName() + "[*] - QuaNotebook");
    }

    return true;
}

void QuaMainWindow::closeEvent(QCloseEvent *e)
{
    if ( isWindowModified() )
    {
        QMessageBox::StandardButton tStandardButton;

        tStandardButton = QMessageBox::warning(this,
                                               tr("Save document?"),
                                               tr("The document has been modified. Do you want to save your changes? Your changes will be lost if you don't save them."),
                                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if ( tStandardButton == QMessageBox::Save )
        {
            if ( !IBFSaveDocument() )
            {
                e->ignore();
                return;
            }
        }
        else if ( tStandardButton == QMessageBox::Cancel )
        {
            e->ignore();
            return;
        }
    }

    e->accept();
}

void QuaMainWindow::resizeEvent(QResizeEvent * e)
{
    if (this->width() <= QApplication::desktop()->screenGeometry().width() / 2 )
    {
        QList<int> tList;
        tList << 0;
        tList << 1;

        mIBFSplitter->setSizes(tList);
    }
    else
    {
        QList<int> tList;
        tList << (QApplication::desktop()->screenGeometry().width() / 3) * 1;
        tList << (QApplication::desktop()->screenGeometry().width() / 3) * 2;

        mIBFSplitter->setSizes(tList);
    }
}

void QuaMainWindow::on_actionNew_triggered()
{
    this->IBFNewDocument();
}

void QuaMainWindow::on_actionOpen_triggered()
{
    if ( this->isWindowModified() )
    {
        QMessageBox::StandardButton tStandardButton;

        tStandardButton = QMessageBox::warning(this, tr("Save document?"), tr("The document has been modified. Do you want to save your changes? Your changes will be lost if you don't save them."), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if ( tStandardButton == QMessageBox::Save )
        {
            if ( !this->IBFSaveDocument() )
            {
                return;
            }
        }
        else if ( tStandardButton == QMessageBox::Cancel )
        {
            return;
        }
    }

    this->IBFOpenDocument();
}

void QuaMainWindow::on_actionSave_triggered()
{
    this->IBFSaveDocument();
}

void QuaMainWindow::on_actionSaveAs_triggered()
{
    this->QuaSaveAsDocument();
}

void QuaMainWindow::on_actionQuit_triggered()
{

}

void QuaMainWindow::on_actionPreview_triggered()
{
    if ( mQuaTextEdit->isVisible() )
    {
        mQuaTextEdit->setVisible(false);

//        mIBFWebView->IBFSetHtml(mQuaTextEdit->toHtml());
//        mIBFWebView->setVisible(true);
    }
    else
    {
        mQuaTextEdit->setVisible(true);

//        mIBFWebView->setVisible(false);
    }
}

void QuaMainWindow::on_mIBFTreeView_selectionChanged_triggered(const QModelIndex & current, const QModelIndex & previous)
{
    if ( mQuaTextEdit->document()->isModified() )
    {
        mQuaTextEdit->IBFSaveToFile(mQuaTextEdit->windowFilePath());
    }

    if ( !mIBFFileSystemModel->fileInfo(current).isFile() )
    {
        const bool wasBlocked = mQuaTextEdit->blockSignals(true);

        mQuaTextEdit->clear();
        mQuaTextEdit->setEnabled(false);

        mQuaTextEdit->blockSignals(wasBlocked);

        return;
    }

    QString tFileName = mIBFFileSystemModel->fileInfo(current).absoluteFilePath();

    if ( tFileName.isEmpty() )
    {
        return;
    }

    if ( !QFile::exists(tFileName) )
    {
        return;
    }

    QFile tFile(tFileName);

    if ( !tFile.open(QFile::ReadOnly) )
    {
        return;
    }

    QByteArray tByteArray = tFile.readAll();
    QTextCodec * tTextCodec = Qt::codecForHtml(tByteArray);

    QString tString = tTextCodec->toUnicode(tByteArray);

    const bool wasBlocked = mQuaTextEdit->blockSignals(true);

    if ( Qt::mightBeRichText(tString) )
    {
        mQuaTextEdit->setHtml(tString);
    }
    else
    {
        tString = QString::fromLocal8Bit(tByteArray);
        mQuaTextEdit->setPlainText(tString);
    }

    mQuaTextEdit->blockSignals(wasBlocked);

    mQuaTextEdit->setWindowFilePath(tFileName);
    mQuaTextEdit->setEnabled(true);

}

void QuaMainWindow::on_mIBFFileSystemModelDirs_fileRenamed_triggered(const QString &path, const QString &oldName, const QString &newName)
{
//    QString tPath = path + "/" + newName;

//    delete mIBFFileSystemModelFiles;

//    mIBFFileSystemModelFiles = new QFileSystemModel();
//    mIBFFileSystemModelFiles->setRootPath(tPath);
//    mIBFFileSystemModelFiles->setFilter(QDir::Files | QDir::NoDotAndDotDot);
//    mIBFFileSystemModelFiles->setReadOnly(false);

//    mIBFFileSystemModelFiles->setIconProvider(mIBFIconProvider);

//    mIBFListView->setModel(mIBFFileSystemModelFiles);
//    mIBFListView->setRootIndex(mIBFFileSystemModelFiles->index(tPath));

//    mIBFListView->setWordWrap(true);
//    mIBFListView->setEnabled(true);

//    QObject::connect(mIBFListView->selectionModel(),
//                     SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
//                     this,
//                     SLOT(on_mIBFListView_selectionChanged_triggered(const QModelIndex &, const QModelIndex &)));

//    QObject::connect(mIBFFileSystemModelFiles,
//                     SIGNAL(fileRenamed(QString,QString,QString)),
//                     this,
//                     SLOT(on_mIBFFileSystemModelFiles_fileRenamed_triggered(QString,QString,QString)));

//    QObject::connect(mIBFFileSystemModelFiles,
//                     SIGNAL(directoryLoaded(QString)),
//                     mIBFListView,
//                     SLOT(on_mIBFListView_DirectoryLoaded(QString)));

    this->setWindowModified(true);
}
