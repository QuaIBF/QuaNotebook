#ifndef QuaMainWindow_H
#define QuaMainWindow_H

#include <QMainWindow>

#include <QFileDialog>
#include <QTemporaryDir>
#include <QProcess>
#include <QFileSystemModel>
#include <QTextCodec>
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QTextEdit>
#include <QWebView>
#include <QGridLayout>
#include <QMenuBar>
#include <QTextDocumentWriter>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileIconProvider>
#include <QDesktopWidget>

#include "QuaZIP/JlCompress.h"

#include "IBFTreeView.h"
#include "QuaTextEdit.h"
#include "IBFWebView.h"

class QuaMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QuaMainWindow(QWidget * aParent = 0);
    ~QuaMainWindow();

    bool IBFNewDocument();
    bool IBFOpenDocument();
    bool IBFSaveDocument();
    bool QuaSaveAsDocument();
    bool QuaCloseDocument();

    private:
    QFile * mIBFDocument;
    QTemporaryDir * mIBFTemporaryDir;
    QFileSystemModel * mIBFFileSystemModel;

    //  Widgets:
    QSplitter * mIBFSplitter;
    IBFTreeView * mIBFTreeView;
    QuaTextEdit * mQuaTextEdit;

//    bool IBFSetupMenuBar();
    bool IBFSetupMenuFile();
    bool IBFSetupConnections();
    bool IBFUpdateTitle();

protected:
    void resizeEvent(QResizeEvent * e);
    void closeEvent(QCloseEvent * e);

public slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionQuit_triggered();

    void on_actionPreview_triggered();

    void on_mIBFTreeView_selectionChanged_triggered(const QModelIndex &current, const QModelIndex &previous);
    void on_mIBFFileSystemModelDirs_fileRenamed_triggered(const QString & path, const QString & oldName, const QString & newName);

    void on_SignalModified(){ this->setWindowModified(true); }
};

#endif // QuaMainWindow_H
