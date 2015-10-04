#ifndef IBFTREEVIEW_H
#define IBFTREEVIEW_H

#include <QTreeView>

#include <QAction>
#include <QFileSystemModel>
#include <QMenu>
#include <QMimeData>
#include <QContextMenuEvent>
#include <QFileIconProvider>
#include <QMessageBox>

#include <QDebug>


class IBFFileIconProvider : public QFileIconProvider
{
//    Q_OBJECT

public:
    IBFFileIconProvider()
    {

    }

    ~IBFFileIconProvider()
    {

    }

    QIcon	icon(IconType type) const
    {
        return QIcon();
    }

    QIcon	icon(const QFileInfo & info) const
    {
        return QIcon();
    }
};

class IBFTreeView : public QTreeView
{
    Q_OBJECT

public:
    IBFTreeView(QWidget * aParent = 0);
    ~IBFTreeView();

public slots:
    void on_ActionMKFile();
    void on_mIBFTreeView_ActionMKRoot();
    void on_mIBFTreeView_ActionMKDir();
    void on_mIBFTreeView_ActionRNDir();
    void on_mIBFTreeView_ActionRMDir();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    QMenu * mContextMenu;

    signals:
    void SignalModified();
};

#endif // IBFTREEVIEW_H
