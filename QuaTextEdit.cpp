#include "QuaTextEdit.h"


QuaTextEdit::QuaTextEdit(QWidget *aParent) : QTextEdit(aParent)
{
    //
    //  Set Up Layout
    //
    this->setFont(QFont("Arial Unicode MS", 12));
    this->setTabStopWidth(this->document()->indentWidth());
    this->setAlignment(Qt::AlignJustify);
    this->setAutoFormatting(QTextEdit::AutoAll);

    //
    //  Set Up Context Menu
    //
    this->QuaSetUpContextMenu();

    //
    // Set Up Hash Replacement
    //
    this->QuaSetUpHashReplacement();

    //
    //  Connect signals and slots
    //
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(on_mNotesTextEdit_textChanged()));
}

QuaTextEdit::~QuaTextEdit()
{

}

bool QuaTextEdit::IBFSaveToFile(QString aString)
{
    this->setDocumentTitle("This is the title!!!");

    QTextDocumentWriter tTextDocumentWriter;
    tTextDocumentWriter.setFileName(aString);

    if ( !tTextDocumentWriter.write(this->document()) )
    {
        return false;
    }

    this->document()->setModified(false);
}

bool QuaTextEdit::IBFLoadFromFile(QString aString)
{
    return false;
}

void QuaTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    mContextMenu->exec(event->globalPos());
}

void QuaTextEdit::focusInEvent(QFocusEvent *e)
{
    this->IBFSaveToFile(this->windowFilePath());
    QTextEdit::focusInEvent(e);
}

void QuaTextEdit::focusOutEvent(QFocusEvent *e)
{
    this->IBFSaveToFile(this->windowFilePath());
    QTextEdit::focusOutEvent(e);
}

void QuaTextEdit::dragEnterEvent(QDragEnterEvent * event)
{
    if ( event->mimeData()->hasUrls() )
    {
        foreach ( QUrl tUrl, event->mimeData()->urls() )
        {
            QString tFileName = tUrl.toLocalFile();
            QString tFileExtension = QFileInfo(tFileName).suffix().toLower();

            if ( tFileExtension == "bmp" )
            {
                event->acceptProposedAction();
                return;
            }

            if ( tFileExtension == "jpg" )
            {
                event->acceptProposedAction();
                return;
            }

            if ( tFileExtension == "jpeg" )
            {
                event->acceptProposedAction();
                return;
            }

            if ( tFileExtension == "png" )
            {
                event->acceptProposedAction();
                return;
            }

            if ( tFileExtension == "ppm" )
            {
                event->acceptProposedAction();
                return;
            }

            if ( tFileExtension == "xbm" )
            {
                event->acceptProposedAction();
                return;
            }

            if ( tFileExtension == "xpn" )
            {
                event->acceptProposedAction();
                return;
            }

        }
    }
}

void QuaTextEdit::dropEvent(QDropEvent * event)
{
    if ( event->mimeData()->hasUrls() )
    {
        foreach ( QUrl tUrl, event->mimeData()->urls() )
        {
            QString tFileName = tUrl.toLocalFile();
            QString tFileExtension = QFileInfo(tFileName).suffix().toLower();

            QImage tImage(tFileName);

            if ( !tImage.isNull() )
            {
                QByteArray tByteArray;
                QBuffer tBuffer(&tByteArray);

                tImage.save(&tBuffer, "PNG");
                QString tBase64 = QString::fromLatin1(tByteArray.toBase64().data());

                QTextCursor tTextCursor = this->cursorForPosition(event->pos());
                tTextCursor.insertHtml("<img src=\"data:image/png;base64," + tBase64 + "/>");

                //
                //  Dummy Event to prevent cursor malfunction.
                QMimeData * tMimeData = new QMimeData();// = QMimeData();
                tMimeData->setText("");

                QDropEvent * dummyEvent = new QDropEvent(event->posF(), event->possibleActions(),
                                tMimeData, event->mouseButtons(), event->keyboardModifiers());

                QTextEdit::dropEvent(dummyEvent);

                return;
            }

        }
    }
}

void QuaTextEdit::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Space )
    {
        QTextCursor tCurrentWord = this->textCursor();
        tCurrentWord.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor, 2);

        if ( mHashReplacement.contains(tCurrentWord.selectedText()) )
        {
            tCurrentWord.insertText(mHashReplacement[tCurrentWord.selectedText()]);
        }
    }

    QTextEdit::keyPressEvent(e);
}

void QuaTextEdit::resizeEvent(QResizeEvent *e)
{
    QMargins tMargins;

    if ( this->rect().width() > 700 )
    {
        tMargins.setLeft((rect().width() - 700) / 2);
        tMargins.setRight((rect().width() - 700) / 2);
    }
    else
    {
        tMargins.setLeft(0);
        tMargins.setRight(0);
    }

    this->setViewportMargins(tMargins);

    return QTextEdit::resizeEvent(e);
}

bool QuaTextEdit::QuaSetUpContextMenu()
{
    mContextMenu = new QMenu();

    QMenu * tFileMenu = new QMenu("File");
    QMenu * tEditMenu = new QMenu("Edit");
    QMenu * tViewMenu = new QMenu("View");
    QMenu * tInsertMenu = new QMenu("Insert");
    QMenu * tFormatMenu = new QMenu("Format");
    QMenu * tHelpMenu = new QMenu("Help");

    this->mContextMenu->addMenu(tFileMenu);
    this->mContextMenu->addMenu(tEditMenu);
    this->mContextMenu->addMenu(tViewMenu);
    this->mContextMenu->addMenu(tInsertMenu);
    this->mContextMenu->addMenu(tFormatMenu);
    this->mContextMenu->addMenu(tHelpMenu);

    //
    //  Menu File:
    QAction * actionNew = new QAction(tr("&New"), this);
    connect(actionNew, SIGNAL(triggered()), this, SLOT(on_actionNew_triggered()));
    tFileMenu->addAction(actionNew);
    addAction(actionNew);

    QAction * actionOpen = new QAction(tr("&Open..."), this);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(on_actionOpen_triggered()));
    tFileMenu->addAction(actionOpen);
    addAction(actionOpen);

    tFileMenu->addSeparator();

    QAction * actionSave = new QAction(tr("&Save"), this);
    connect(actionSave, SIGNAL(triggered()), this, SLOT(on_actionSave_triggered()));
    tFileMenu->addAction(actionSave);
    addAction(actionSave);

    QAction * actionSaveAs = new QAction(tr("Save &As..."), this);
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(on_actionSaveAs_triggered()));
    tFileMenu->addAction(actionSaveAs);
    addAction(actionSaveAs);

    tFileMenu->addSeparator();

    QAction * actionQuit = new QAction(tr("&Quit"), this);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(on_actionQuit_triggered()));
    tFileMenu->addAction(actionQuit);
    addAction(actionQuit);


    //
    //  Menu Edit:
    QAction * actionUndo = new QAction(tr("&Undo"), this);
    actionUndo->setShortcut(QKeySequence::Undo);
    actionUndo->setEnabled(this->document()->isUndoAvailable());
    connect(this->document(), SIGNAL(undoAvailable(bool)), actionUndo, SLOT(setEnabled(bool)));
    connect(actionUndo, SIGNAL(triggered()), this, SLOT(undo()));
    tEditMenu->addAction(actionUndo);
    addAction(actionUndo);

    QAction *actionRedo = new QAction(tr("Redo"), this);
    actionRedo->setShortcut(QKeySequence::Redo);
    actionRedo->setEnabled(this->document()->isRedoAvailable());
    connect(this->document(), SIGNAL(redoAvailable(bool)), actionRedo, SLOT(setEnabled(bool)));
    connect(actionRedo, SIGNAL(triggered()), this, SLOT(redo()));
    tEditMenu->addAction(actionRedo);
    addAction(actionRedo);

    tEditMenu->addSeparator();

    QAction * actionCut = new QAction(tr("Cu&t"), this);
    actionCut->setShortcut(QKeySequence::Cut);
    actionCut->setEnabled(false);
    connect(actionCut, SIGNAL(triggered()), this, SLOT(cut()));
    connect(this, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
    tEditMenu->addAction(actionCut);
    addAction(actionCut);

    QAction * actionCopy = new QAction(tr("&Copy"), this);
    actionCopy->setShortcut(QKeySequence::Copy);
    actionCopy->setEnabled(false);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    connect(this, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));
    tEditMenu->addAction(actionCopy);
    addAction(actionCopy);

    QAction * actionPaste = new QAction(tr("&Paste"), this);
    actionPaste->setShortcut(QKeySequence::Paste);
    connect(actionPaste, SIGNAL(triggered()), this, SLOT(paste()));
    tEditMenu->addAction(actionPaste);
    addAction(actionPaste);

    QAction * actionPastePlainText = new QAction(tr("Paste Plain Text"), this);
    actionPastePlainText->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_V);
    connect(actionPastePlainText, SIGNAL(triggered()), this, SLOT(on_actionPastePlainText_triggered()));
    tEditMenu->addAction(actionPastePlainText);
    addAction(actionPastePlainText);

    //
    //  Menu Insert:
    QAction * actionInsertHorizontalLine = new QAction(tr("Insert Horizontal Line"), this);
    //actionPastePlainText->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_V);
    connect(actionInsertHorizontalLine, SIGNAL(triggered()), this, SLOT(on_actionInsertHorizontalLine_triggered()));
    tInsertMenu->addAction(actionInsertHorizontalLine);
    addAction(actionInsertHorizontalLine);


    //
    //  Menu Format:
    QAction * actionTextBold = new QAction(tr("&Bold"), this);
    actionTextBold->setShortcut(QKeySequence::Bold);
    connect(actionTextBold, SIGNAL(triggered()), this, SLOT(on_actionTextBold_triggered()));
    tFormatMenu->addAction(actionTextBold);
    addAction(actionTextBold);

    QAction * actionTextItalic = new QAction(tr("&Italic"), this);
    actionTextItalic->setShortcut(QKeySequence::Italic);
    connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(on_actionTextItalic_triggered()));
    tFormatMenu->addAction(actionTextItalic);
    addAction(actionTextItalic);

    QAction * actionTextUnderline = new QAction(tr("&Underline"), this);
    actionTextUnderline->setShortcut(QKeySequence::Underline);
    connect(actionTextUnderline, SIGNAL(triggered()), this, SLOT(on_actionTextUnderline_triggered()));
    tFormatMenu->addAction(actionTextUnderline);
    addAction(actionTextUnderline);

    QAction * actionTextStrikeOut = new QAction(tr("&Strike Out"), this);
    actionTextStrikeOut->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_U);
    connect(actionTextStrikeOut, SIGNAL(triggered()), this, SLOT(on_actionTextStrikeOut_triggered()));
    tFormatMenu->addAction(actionTextStrikeOut);
    addAction(actionTextStrikeOut);

    tFormatMenu->addSeparator();

    QAction * actionTextLeft = new QAction(tr("&Left"), this);
    actionTextLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    connect(actionTextLeft, SIGNAL(triggered()), this, SLOT(on_actionTextLeft_triggered()));
    tFormatMenu->addAction(actionTextLeft);
    addAction(actionTextLeft);

    QAction * actionTextCenter = new QAction(tr("C&enter"), this);
    actionTextCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    connect(actionTextCenter, SIGNAL(triggered()), this, SLOT(on_actionTextCenter_triggered()));
    tFormatMenu->addAction(actionTextCenter);
    addAction(actionTextCenter);

    QAction * actionTextRight = new QAction(tr("&Right"), this);
    actionTextRight->setShortcut(Qt::CTRL + Qt::Key_R);
    connect(actionTextRight, SIGNAL(triggered()), this, SLOT(on_actionTextRight_triggered()));
    tFormatMenu->addAction(actionTextRight);
    addAction(actionTextRight);

    QAction * actionTextJustify = new QAction(tr("&Justify"), this);
    actionTextJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    connect(actionTextJustify, SIGNAL(triggered()), this, SLOT(on_actionTextJustify_triggered()));
    tFormatMenu->addAction(actionTextJustify);
    addAction(actionTextJustify);

    tFormatMenu->addSeparator();

    QAction * actionTextSuperScript = new QAction(tr("SuperScript"), this);
    actionTextSuperScript->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_P);
    connect(actionTextSuperScript, SIGNAL(triggered()), this, SLOT(on_actionTextSuperScript_triggered()));
    tFormatMenu->addAction(actionTextSuperScript);
    addAction(actionTextSuperScript);

    QAction * actionTextSubScript = new QAction(tr("SubScript"), this);
    actionTextSubScript->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_B);
    connect(actionTextSubScript, SIGNAL(triggered()), this, SLOT(on_actionTextSubScript_triggered()));
    tFormatMenu->addAction(actionTextSubScript);
    addAction(actionTextSubScript);

    tFormatMenu->addSeparator();

    QAction * actionHeading1 = new QAction(tr("&Heading 1"), this);
    actionHeading1->setShortcut(Qt::CTRL + Qt::Key_1);
    connect(actionHeading1, SIGNAL(triggered()), this, SLOT(on_actionHeading1_triggered()));
    tFormatMenu->addAction(actionHeading1);
    addAction(actionHeading1);

    QAction * actionHeading2 = new QAction(tr("Heading 2"), this);
    actionHeading2->setShortcut(Qt::CTRL + Qt::Key_2);
    connect(actionHeading2, SIGNAL(triggered()), this, SLOT(on_actionHeading2_triggered()));
    tFormatMenu->addAction(actionHeading2);
    addAction(actionHeading2);

    QAction * actionHeading3 = new QAction(tr("Heading 3"), this);
    actionHeading3->setShortcut(Qt::CTRL + Qt::Key_3);
    connect(actionHeading3, SIGNAL(triggered()), this, SLOT(on_actionHeading3_triggered()));
    tFormatMenu->addAction(actionHeading3);
    addAction(actionHeading3);

    QAction * actionHeading4 = new QAction(tr("Heading 4"), this);
    actionHeading4->setShortcut(Qt::CTRL + Qt::Key_4);
    connect(actionHeading4, SIGNAL(triggered()), this, SLOT(on_actionHeading4_triggered()));
    tFormatMenu->addAction(actionHeading4);
    addAction(actionHeading4);

    QAction * actionHeading5 = new QAction(tr("Heading 5"), this);
    actionHeading5->setShortcut(Qt::CTRL + Qt::Key_5);
    connect(actionHeading5, SIGNAL(triggered()), this, SLOT(on_actionHeading5_triggered()));
    tFormatMenu->addAction(actionHeading5);
    addAction(actionHeading5);

    QAction * actionHeading6 = new QAction(tr("Heading 6"), this);
    actionHeading6->setShortcut(Qt::CTRL + Qt::Key_6);
    connect(actionHeading6, SIGNAL(triggered()), this, SLOT(on_actionHeading6_triggered()));
    tFormatMenu->addAction(actionHeading6);
    addAction(actionHeading6);

    tFormatMenu->addSeparator();

    QAction * actionBlockQuote = new QAction(tr("Block Quote"), this);
    actionBlockQuote->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_B);
    connect(actionBlockQuote, SIGNAL(triggered()), this, SLOT(on_actionBlockQuote_triggered()));
    tFormatMenu->addAction(actionBlockQuote);
    addAction(actionBlockQuote);

    tFormatMenu->addSeparator();

    QAction * actionUnorderedList = new QAction(tr("UnorderedList"), this);
    connect(actionUnorderedList, SIGNAL(triggered()), this, SLOT(on_actionUnorderedList_triggered()));
    tFormatMenu->addAction(actionUnorderedList);
    addAction(actionUnorderedList);

    QAction * actionOrderedList = new QAction(tr("Ordered List"), this);
    connect(actionOrderedList, SIGNAL(triggered()), this, SLOT(on_actionOrderedList_triggered()));
    tFormatMenu->addAction(actionOrderedList);
    addAction(actionOrderedList);

    tFormatMenu->addSeparator();

    QAction * actionIncreaseIndent = new QAction(tr("Increase Indent"), this);
    actionIncreaseIndent->setShortcut(Qt::CTRL + Qt::Key_M);
    connect(actionIncreaseIndent, SIGNAL(triggered()), this, SLOT(on_actionIncreaseIndent_triggered()));
    tFormatMenu->addAction(actionIncreaseIndent);
    addAction(actionIncreaseIndent);

    QAction * actionDecreaseIndent = new QAction(tr("Decrease Indent"), this);
    actionDecreaseIndent->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_M);
    connect(actionDecreaseIndent, SIGNAL(triggered()), this, SLOT(on_actionDecreaseIndent_triggered()));
    tFormatMenu->addAction(actionDecreaseIndent);
    addAction(actionDecreaseIndent);

    QAction * actionIncreaseFirstIndent = new QAction(tr("Increase First Line Indent"), this);
    actionIncreaseFirstIndent->setShortcut(Qt::CTRL + Qt::Key_T);
    connect(actionIncreaseFirstIndent, SIGNAL(triggered()), this, SLOT(on_actionIncreaseFirstIndent_triggered()));
    tFormatMenu->addAction(actionIncreaseFirstIndent);
    addAction(actionIncreaseFirstIndent);

    QAction * actionDecreaseFirstIndent = new QAction(tr("Decrease First Line Indent"), this);
    actionDecreaseFirstIndent->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_T);
    connect(actionDecreaseFirstIndent, SIGNAL(triggered()), this, SLOT(on_actionDecreaseFirstIndent_triggered()));
    tFormatMenu->addAction(actionDecreaseFirstIndent);
    addAction(actionDecreaseFirstIndent);

    tFormatMenu->addSeparator();

    QAction * actionClearBlock = new QAction(tr("&Clear Formatting"), this);
    //actionClearBlock->setShortcut(Qt::CTRL + Qt::Key_C);
    connect(actionClearBlock, SIGNAL(triggered()), this, SLOT(on_actionClearBlock_triggered()));
    tFormatMenu->addAction(actionClearBlock);
    addAction(actionClearBlock);


    //
    //  Menu View:
//    QAction * actionPreview = new QAction(tr("Preview"), this);
//    actionPreview->setShortcut(QKeySequence(tr("Ctrl+P")));
//    connect(actionPreview, SIGNAL(triggered()), this, SLOT(on_actionPreview_triggered()));
//    tViewMenu->addAction(actionPreview);
//    addAction(actionPreview);

//    actionCountWords = new QAction(tr("Count Words"), this);
//    //actionPreview->setShortcut(QKeySequence(tr("Ctrl+P")));
//    connect(actionCountWords, SIGNAL(triggered()), this, SLOT(on_actionCountWords_triggered()));
//    tViewMenu->addAction(actionCountWords);
//    addAction(actionCountWords);

    QAction * actionPreferences = new QAction(tr("Preferences"), this);
    //actionPreview->setShortcut(QKeySequence(tr("Ctrl+P")));
    connect(actionPreferences, SIGNAL(triggered()), this, SLOT(on_actionPreferences_triggered()));
    tViewMenu->addAction(actionPreferences);
    addAction(actionPreferences);


    //
    //  Menu Help:
    QAction * actionAbout = new QAction(tr("About"), this);
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(on_actionAbout_triggered()));
    tHelpMenu->addAction(actionAbout);
    addAction(actionAbout);

    return true;
}

bool QuaTextEdit::QuaSetUpHashReplacement()
{
    QFile tFile("HashReplacement.ini");

    if ( !tFile.open(QIODevice::ReadOnly) )
    {
        return false;
    }

    mHashReplacement.clear();

    QTextStream tTextStream(&tFile);

    while ( !tTextStream.atEnd() )
    {
        QString tString = tTextStream.readLine();
        QStringList tStringList = tString.split(",");

        mHashReplacement[tStringList.at(0)] = tStringList.at(1);
    }

    tFile.close();

    return true;
}

//
//  File Menu
//
void QuaTextEdit::on_actionNew_triggered()
{
    signal_actionNew();
}

void QuaTextEdit::on_actionOpen_triggered()
{
    signal_actionOpen();
}

void QuaTextEdit::on_actionSave_triggered()
{
    signal_actionSave();
}

void QuaTextEdit::on_actionSaveAs_triggered()
{
    signal_actionSaveAs();
}

void QuaTextEdit::on_actionQuit_triggered()
{
    signal_actionQuit();
}


//
//  Edit Menu
void QuaTextEdit::on_actionPastePlainText_triggered()
{
    this->textCursor().insertText(QApplication::clipboard()->text());
    this->ensureCursorVisible();
}

//
//  Insert Menu:
void QuaTextEdit::on_actionInsertHorizontalLine_triggered()
{
    QTextCursor tTextCursor = this->textCursor();

    tTextCursor.insertHtml( "<hr>" );
}

//
//  Format Menu
void QuaTextEdit::on_actionTextBold_triggered()
{
    QTextCharFormat tTextCharFormat;

    QTextCursor tTextCursor = this->textCursor();

    if (!tTextCursor.hasSelection())
    {
        tTextCursor.select(QTextCursor::WordUnderCursor);
    }
    else
    {
        int tStartSelection = tTextCursor.selectionStart();
        int tEndSelection = tTextCursor.selectionEnd();
        tTextCursor.setPosition(tStartSelection, QTextCursor::MoveAnchor);
        tTextCursor.setPosition(tEndSelection, QTextCursor::KeepAnchor);
    }

    if ( !tTextCursor.charFormat().font().bold() )
    {
        tTextCharFormat.setFontWeight(QFont::Bold);
        //actionTextBold->setChecked(true);
    }
    else
    {
        tTextCharFormat.setFontWeight(QFont::Normal);
        //actionTextBold->setChecked(false);
    }

    tTextCursor.mergeCharFormat(tTextCharFormat);
}

void QuaTextEdit::on_actionTextItalic_triggered()
{
    QTextCharFormat tTextCharFormat;

    QTextCursor tTextCursor = this->textCursor();

    if (!tTextCursor.hasSelection())
    {
        tTextCursor.select(QTextCursor::WordUnderCursor);
    }
    else
    {
        int tStartSelection = tTextCursor.selectionStart();
        int tEndSelection = tTextCursor.selectionEnd();
        tTextCursor.setPosition(tStartSelection, QTextCursor::MoveAnchor);
        tTextCursor.setPosition(tEndSelection, QTextCursor::KeepAnchor);
    }

    if ( !tTextCursor.charFormat().font().italic() )
    {
        tTextCharFormat.setFontItalic(true);
        //actionTextItalic->setChecked(true);
    }
    else
    {
        tTextCharFormat.setFontItalic(false);
        //actionTextItalic->setChecked(false);
    }

    tTextCursor.mergeCharFormat(tTextCharFormat);
}

void QuaTextEdit::on_actionTextUnderline_triggered()
{
    QTextCharFormat tTextCharFormat;

    QTextCursor tTextCursor = this->textCursor();

    if (!tTextCursor.hasSelection())
    {
        tTextCursor.select(QTextCursor::WordUnderCursor);
    }
    else
    {
        int tStartSelection = tTextCursor.selectionStart();
        int tEndSelection = tTextCursor.selectionEnd();
        tTextCursor.setPosition(tStartSelection, QTextCursor::MoveAnchor);
        tTextCursor.setPosition(tEndSelection, QTextCursor::KeepAnchor);
    }

    if ( !tTextCursor.charFormat().font().underline() )
    {
        tTextCharFormat.setFontUnderline(true);
        //actionTextUnderline->setChecked(true);
    }
    else
    {
        tTextCharFormat.setFontUnderline(false);
        //actionTextUnderline->setChecked(false);
    }

    tTextCursor.mergeCharFormat(tTextCharFormat);
}

void QuaTextEdit::on_actionTextStrikeOut_triggered()
{
    QTextCharFormat tTextCharFormat;

    QTextCursor tTextCursor = this->textCursor();

    if (!tTextCursor.hasSelection())
    {
        tTextCursor.select(QTextCursor::WordUnderCursor);
    }
    else
    {
        int tStartSelection = tTextCursor.selectionStart();
        int tEndSelection = tTextCursor.selectionEnd();
        tTextCursor.setPosition(tStartSelection, QTextCursor::MoveAnchor);
        tTextCursor.setPosition(tEndSelection, QTextCursor::KeepAnchor);
    }

    if ( !tTextCursor.charFormat().font().strikeOut() )
    {
        tTextCharFormat.setFontStrikeOut(true);
        //actionTextUnderline->setChecked(true);
    }
    else
    {
        tTextCharFormat.setFontStrikeOut(false);
        //actionTextUnderline->setChecked(false);
    }

    tTextCursor.mergeCharFormat(tTextCharFormat);
}

void QuaTextEdit::on_actionTextJustify_triggered()
{
    this->setAlignment(Qt::AlignJustify);
}

void QuaTextEdit::on_actionTextLeft_triggered()
{
    this->setAlignment(Qt::AlignLeft);
}

void QuaTextEdit::on_actionTextCenter_triggered()
{
    this->setAlignment(Qt::AlignHCenter);
}

void QuaTextEdit::on_actionTextRight_triggered()
{
    this->setAlignment(Qt::AlignRight);
}


void QuaTextEdit::on_actionTextSuperScript_triggered()
{
    QTextCharFormat tTextCharFormat;

    QTextCursor tTextCursor = this->textCursor();

    if (!tTextCursor.hasSelection())
    {
        tTextCursor.select(QTextCursor::WordUnderCursor);
    }
    else
    {
        int tStartSelection = tTextCursor.selectionStart();
        int tEndSelection = tTextCursor.selectionEnd();
        tTextCursor.setPosition(tStartSelection, QTextCursor::MoveAnchor);
        tTextCursor.setPosition(tEndSelection, QTextCursor::KeepAnchor);
    }

    if ( !(tTextCursor.charFormat().verticalAlignment() == QTextCharFormat::AlignSuperScript) )
    {
        tTextCharFormat.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
        //actionTextSuperScript->setChecked(true);
    }
    else
    {
        tTextCharFormat.setVerticalAlignment(QTextCharFormat::AlignNormal);
        //actionTextSuperScript->setChecked(false);
    }

    tTextCursor.mergeCharFormat(tTextCharFormat);
}

void QuaTextEdit::on_actionTextSubScript_triggered()
{
    QTextCharFormat tTextCharFormat;

    QTextCursor tTextCursor = this->textCursor();

    if (!tTextCursor.hasSelection())
    {
        tTextCursor.select(QTextCursor::WordUnderCursor);
    }
    else
    {
        int tStartSelection = tTextCursor.selectionStart();
        int tEndSelection = tTextCursor.selectionEnd();
        tTextCursor.setPosition(tStartSelection, QTextCursor::MoveAnchor);
        tTextCursor.setPosition(tEndSelection, QTextCursor::KeepAnchor);
    }

    if ( !(tTextCursor.charFormat().verticalAlignment() == QTextCharFormat::AlignSubScript) )
    {
        tTextCharFormat.setVerticalAlignment(QTextCharFormat::AlignSubScript);
        //actionTextSubScript->setChecked(true);
    }
    else
    {
        tTextCharFormat.setVerticalAlignment(QTextCharFormat::AlignNormal);
        //actionTextSubScript->setChecked(false);
    }

    tTextCursor.mergeCharFormat(tTextCharFormat);
}


void QuaTextEdit::on_actionHeading1_triggered()
{
    QTextCursor tTextCursor = this->textCursor();
    tTextCursor.select(QTextCursor::BlockUnderCursor);

    tTextCursor.insertHtml( "<h1>" + tTextCursor.selectedText() + "</h1>" );
}

void QuaTextEdit::on_actionHeading2_triggered()
{
    QTextCursor tTextCursor = this->textCursor();
    tTextCursor.select(QTextCursor::BlockUnderCursor);

    tTextCursor.insertHtml( "<h2>" + tTextCursor.selectedText() + "</h2>" );
}

void QuaTextEdit::on_actionHeading3_triggered()
{
    QTextCursor tTextCursor = this->textCursor();
    tTextCursor.select(QTextCursor::BlockUnderCursor);

    tTextCursor.insertHtml( "<h3>" + tTextCursor.selectedText() + "</h3>" );
}

void QuaTextEdit::on_actionHeading4_triggered()
{
    QTextCursor tTextCursor = this->textCursor();
    tTextCursor.select(QTextCursor::BlockUnderCursor);

    tTextCursor.insertHtml( "<h4>" + tTextCursor.selectedText() + "</h4>" );
}

void QuaTextEdit::on_actionHeading5_triggered()
{
    QTextCursor tTextCursor = this->textCursor();
    tTextCursor.select(QTextCursor::BlockUnderCursor);

    tTextCursor.insertHtml( "<h5>" + tTextCursor.selectedText() + "</h5>" );
}

void QuaTextEdit::on_actionHeading6_triggered()
{
    QTextCursor tTextCursor = this->textCursor();
    tTextCursor.select(QTextCursor::BlockUnderCursor);

    tTextCursor.insertHtml( "<h6>" + tTextCursor.selectedText() + "</h6>" );
}

void QuaTextEdit::on_actionBlockQuote_triggered()
{
    QTextCursor tTextCursor = this->textCursor();
    tTextCursor.select(QTextCursor::BlockUnderCursor);

    tTextCursor.insertHtml( "<blockquote>" + tTextCursor.selectedText() + "</blockquote>" );
}

void QuaTextEdit::on_actionClearBlock_triggered()
{
    QTextCharFormat tTextCharFormat;
    tTextCharFormat.setFontFamily("Arial Unicode MS");

    QTextCursor tTextCursor = this->textCursor();

    if (!tTextCursor.hasSelection())
    {
        tTextCursor.select(QTextCursor::WordUnderCursor);
    }

    tTextCursor.mergeCharFormat(tTextCharFormat);
}

void QuaTextEdit::on_actionUnorderedList_triggered()
{
    QTextCursor cursor = this->textCursor();

    QTextListFormat::Style style = QTextListFormat::ListDisc;

    cursor.beginEditBlock();

    QTextBlockFormat blockFmt = cursor.blockFormat();

    QTextListFormat listFmt;

    if ( cursor.currentList() )
    {
        listFmt = cursor.currentList()->format();
    }
    else
    {
        listFmt.setIndent(blockFmt.indent() + 1);
        blockFmt.setIndent(0);
        cursor.setBlockFormat(blockFmt);
    }

    listFmt.setStyle(style);

    cursor.createList(listFmt);

    cursor.endEditBlock();

}

void QuaTextEdit::on_actionOrderedList_triggered()
{
    QTextCursor cursor = this->textCursor();

    QTextListFormat::Style style = QTextListFormat::ListDecimal;

    cursor.beginEditBlock();

    QTextBlockFormat blockFmt = cursor.blockFormat();

    QTextListFormat listFmt;

    if ( cursor.currentList() )
    {
        listFmt = cursor.currentList()->format();
    }
    else
    {
        listFmt.setIndent(blockFmt.indent() + 1);
        blockFmt.setIndent(0);
        cursor.setBlockFormat(blockFmt);
    }

    listFmt.setStyle(style);

    cursor.createList(listFmt);

    cursor.endEditBlock();

}

void QuaTextEdit::on_actionIncreaseIndent_triggered()
{
    QTextCursor tTextCursor = this->textCursor();
    QTextBlockFormat tTextBlockFormat = tTextCursor.blockFormat();

    tTextBlockFormat.setLeftMargin(tTextBlockFormat.leftMargin() + 40);
    tTextCursor.mergeBlockFormat(tTextBlockFormat);
}

void QuaTextEdit::on_actionDecreaseIndent_triggered()
{
    QTextCursor tTextCursor = this->textCursor();
    QTextBlockFormat tTextBlockFormat = tTextCursor.blockFormat();

    tTextBlockFormat.setLeftMargin(tTextBlockFormat.leftMargin() - 40);
    tTextCursor.mergeBlockFormat(tTextBlockFormat);
}

void QuaTextEdit::on_actionIncreaseFirstIndent_triggered()
{
    QTextCursor tTextCursor = this->textCursor();
    QTextBlockFormat tTextBlockFormat = tTextCursor.blockFormat();

    tTextBlockFormat.setTextIndent( tTextBlockFormat.textIndent() + this->tabStopWidth() );
    tTextCursor.mergeBlockFormat(tTextBlockFormat);
}

void QuaTextEdit::on_actionDecreaseFirstIndent_triggered()
{
    QTextCursor tTextCursor = this->textCursor();
    QTextBlockFormat tTextBlockFormat = tTextCursor.blockFormat();

    tTextBlockFormat.setTextIndent( tTextBlockFormat.textIndent() - this->tabStopWidth() );
    tTextCursor.mergeBlockFormat(tTextBlockFormat);
}

//
//  View Menu
//
void QuaTextEdit::on_actionPreferences_triggered()
{
    IBFDialogPreferences tDialogPreferences(this);

    QObject::connect(&tDialogPreferences, SIGNAL(finished(int)), this, SLOT(on_actionPreferencesClose_triggered()));

    tDialogPreferences.exec();
}

void QuaTextEdit::on_actionPreferencesClose_triggered()
{
    //
    // Load mhash
    QFile tFile("QNotesPreferences.ini");

    if ( !tFile.open(QIODevice::ReadOnly) )
    {
        return ;
    }

    mHashReplacement.clear();

    QTextStream tTextStream(&tFile);

    while ( !tTextStream.atEnd() )
    {
        QString tString = tTextStream.readLine();
        QStringList tStringList = tString.split(",");

        mHashReplacement[tStringList.at(0)] = tStringList.at(1);
    }

    tFile.close();
}

void QuaTextEdit::on_actionAbout_triggered()
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("About QuaNotebook");

    msgBox.setText("<p align=\"justify\">QuaNotebook is a simple note-taking application.<br> Copyright (C) 2015 Iago Bozza Francisco</p>");
    msgBox.setInformativeText("<p align=\"justify\">This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.</p> <p align=\"justify\">This program is distributed in the hope that it will be useful, but without any warrany; without even the implied warranty of merchantability or fitness for a particular purpose.  See the GNU General Public License for more details.</p> <p align=\"justify\">You should have received a copy of the GNU General Public License along with this program.  If not, see <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>.</p>");

    msgBox.setIconPixmap(QPixmap(":/images/notepad.png"));

    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Close);
    msgBox.exec();
}
