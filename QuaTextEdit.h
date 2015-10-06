#ifndef QuaTextEdit_H
#define QuaTextEdit_H

#include <QTextEdit>

#include <QAction>
#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QDebug>
#include <QFileInfo>
#include <QImageReader>
#include <QMenu>
#include <QMimeData>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QTextListFormat>

#include "QuaPreferenceWindow.h"

class QuaTextEdit : public QTextEdit
{
    Q_OBJECT

    public:
    QuaTextEdit(QWidget * aParent = 0);
    ~QuaTextEdit();

    bool IBFSaveToFile(QString aString);
    bool IBFLoadFromFile(QString aString);

    protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void focusInEvent(QFocusEvent * e);
    void focusOutEvent(QFocusEvent * e);
    void dragEnterEvent(QDragEnterEvent * event);
    void dropEvent(QDropEvent * event);
    void keyPressEvent(QKeyEvent * e);
    void resizeEvent(QResizeEvent * e);

    private:
    QMenu * mContextMenu;

    QHash<QString, QString> mHashReplacement;

    bool QuaSetUpLayout(){ return true; }
    bool QuaSetUpContextMenu();
    bool QuaSetUpHashReplacement();

signals:
    void signal_actionNew();
    void signal_actionOpen();
    void signal_actionSave();
    void signal_actionSaveAs();
    void signal_actionQuit();

public slots:
    //
    //  File Menu:
    //
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionQuit_triggered();

    //
    //  Edit Menu:
    //
    void on_actionPastePlainText_triggered();

    //
    //  Insert Menu:
    //
    void on_actionInsertHorizontalLine_triggered();

    //
    //  Format Menu:
    //
    void on_actionTextBold_triggered();
    void on_actionTextItalic_triggered();
    void on_actionTextUnderline_triggered();
    void on_actionTextStrikeOut_triggered();
    void on_actionTextJustify_triggered();
    void on_actionTextLeft_triggered();
    void on_actionTextCenter_triggered();
    void on_actionTextRight_triggered();

    void on_actionTextSuperScript_triggered();
    void on_actionTextSubScript_triggered();

    void on_actionHeading1_triggered();
    void on_actionHeading2_triggered();
    void on_actionHeading3_triggered();
    void on_actionHeading4_triggered();
    void on_actionHeading5_triggered();
    void on_actionHeading6_triggered();

    void on_actionBlockQuote_triggered();

    void on_actionUnorderedList_triggered();
    void on_actionOrderedList_triggered();

    void on_actionIncreaseIndent_triggered();
    void on_actionDecreaseIndent_triggered();

    void on_actionIncreaseFirstIndent_triggered();
    void on_actionDecreaseFirstIndent_triggered();

    void on_actionClearBlock_triggered();


    //
    //  View Menu:
    //
//    void on_actionPreview_triggered();
//    void on_actionCountWords_triggered();

    void on_actionPreferences_triggered();
    void on_actionPreferencesClose_triggered();


    //
    //  Help Menu:
    //
    void on_actionAbout_triggered();


    //
    //  Misc Slots:
    //
//    void on_mNotesTextEdit_textChanged(){ parentWidget()->parentWidget()->setWindowModified(true); }

};

#endif // QuaTextEdit_H

