#ifndef IBFDIALOGPREFERENCES_H
#define IBFDIALOGPREFERENCES_H

#include <QDialog>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QFile>
#include <QTextStream>
#include <QDialog>

#include "QuaPreferenceWindow.h"

class IBFWidgetAutoReplace : public QWidget
{
    Q_OBJECT

    public:
    explicit IBFWidgetAutoReplace(QWidget *parent = 0);
    ~IBFWidgetAutoReplace();

    bool IBFLoadSettings();
    bool IBFSaveSettings();

    public slots:
    void on_mPushButtonInsert_clicked();
    void on_mPushButtonRemove_clicked();

    private:
    QTableWidget * mTableWidget;

    QLineEdit * mLineEditReplace;
    QLineEdit * mLineEditWith;

    QPushButton * mPushButtonInsert;
    QPushButton * mPushButtonRemove;
};



class IBFDialogPreferences : public QDialog
{
    Q_OBJECT

    public:
    IBFDialogPreferences(QWidget * aParent = 0);
    ~IBFDialogPreferences();

    public slots:
    void on_mPushButtonApply_clicked();
    void on_mPushButtonCancel_clicked();
    void on_mPushButtonOK_clicked();

    protected:

    private:
    IBFWidgetAutoReplace * mNotesWidgetAutoReplace;

    QPushButton * mPushButtonApply;
    QPushButton * mPushButtonCancel;
    QPushButton * mPushButtonOK;

    signals:
    //void SignalNotesDialogPreferencesUpdate();
};

#endif // IBFDIALOGPREFERENCES_H
