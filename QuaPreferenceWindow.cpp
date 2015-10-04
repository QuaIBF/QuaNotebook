#include "QuaPreferenceWindow.h"

IBFWidgetAutoReplace::IBFWidgetAutoReplace(QWidget *parent) : QWidget(parent)
{
    QLabel * tLabelReplace = new QLabel("Replace:");
    mLineEditReplace = new QLineEdit();

    QVBoxLayout * tVBoxLayoutReplace = new QVBoxLayout();
    tVBoxLayoutReplace->addWidget(tLabelReplace);
    tVBoxLayoutReplace->addWidget(mLineEditReplace);

    QLabel * tLabelWith = new QLabel("With:");
    mLineEditWith = new QLineEdit();

    QVBoxLayout * tVBoxLayoutWith = new QVBoxLayout();
    tVBoxLayoutWith->addWidget(tLabelWith);
    tVBoxLayoutWith->addWidget(mLineEditWith);

    QHBoxLayout * tHBoxLayoutReplaceWith = new QHBoxLayout();
    tHBoxLayoutReplaceWith->addLayout(tVBoxLayoutReplace);
    tHBoxLayoutReplaceWith->addLayout(tVBoxLayoutWith);

    mTableWidget = new QTableWidget(0, 2, this);

    mTableWidget->verticalHeader()->hide();
    mTableWidget->horizontalHeader()->hide();
    mTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    QVBoxLayout * tVBoxLayoutLabelTable = new QVBoxLayout();
    tVBoxLayoutLabelTable->addLayout(tHBoxLayoutReplaceWith);
    tVBoxLayoutLabelTable->addWidget(mTableWidget);

    mPushButtonInsert = new QPushButton("Insert Line");
    mPushButtonRemove = new QPushButton("Remove Line");

    QVBoxLayout * tVBoxLayoutButton = new QVBoxLayout();
    tVBoxLayoutButton->addWidget(new QLabel);
    tVBoxLayoutButton->addWidget(mPushButtonInsert);
    tVBoxLayoutButton->addWidget(mPushButtonRemove);
    tVBoxLayoutButton->insertStretch(-1);

    QHBoxLayout * tMainLayout = new QHBoxLayout;
    tMainLayout->addLayout(tVBoxLayoutLabelTable);
    tMainLayout->addLayout(tVBoxLayoutButton);

    setLayout(tMainLayout);

    connect(mPushButtonInsert, SIGNAL(clicked()), this, SLOT(on_mPushButtonInsert_clicked()));
    connect(mPushButtonRemove, SIGNAL(clicked()), this, SLOT(on_mPushButtonRemove_clicked()));

    this->IBFLoadSettings();
}

IBFWidgetAutoReplace::~IBFWidgetAutoReplace()
{

}


bool IBFWidgetAutoReplace::IBFLoadSettings()
{
        QFile tFile("QNotesPreferences.ini");

        if ( !tFile.open(QIODevice::ReadOnly) )
        {
            return false;
        }

        QTextStream tTextStream(&tFile);

        while ( !tTextStream.atEnd() )
        {
            QString tString = tTextStream.readLine();
            QStringList tStringList = tString.split(",");

            QTableWidgetItem * tTableWidgetItemReplace = new QTableWidgetItem(tStringList.at(0));
            QTableWidgetItem * tTableWidgetItemWith = new QTableWidgetItem(tStringList.at(1));

            int tRowCount = mTableWidget->rowCount();

            mTableWidget->insertRow(tRowCount);
            mTableWidget->setItem(tRowCount, 0, tTableWidgetItemReplace);
            mTableWidget->setItem(tRowCount, 1, tTableWidgetItemWith);
        }

        tFile.close();

        return true;
}

bool IBFWidgetAutoReplace::IBFSaveSettings()
{
    QFile tFile("QNotesPreferences.ini");

    if ( !tFile.open(QIODevice::WriteOnly | QIODevice::Truncate) )
    {
        return false;
    }

    QTextStream tTextStream(&tFile);

    int tRow = mTableWidget->rowCount();

    for ( int i = 0; i < tRow; i++ )
    {
        tTextStream << mTableWidget->item(i,0)->text() << "," << mTableWidget->item(i,1)->text() << "\n";
    }

    if ( !tFile.flush() )
    {
        return false;
    }

    tFile.close();

    return true;
}

void IBFWidgetAutoReplace::on_mPushButtonInsert_clicked()
{
    if ( !mLineEditReplace->text().isEmpty() && !mLineEditWith->text().isEmpty() )
    {
        QTableWidgetItem * tTableWidgetItemReplace = new QTableWidgetItem(mLineEditReplace->text());
        QTableWidgetItem * tTableWidgetItemWith = new QTableWidgetItem(mLineEditWith->text());

        int tRowCount = mTableWidget->rowCount();

        mTableWidget->insertRow(tRowCount);
        mTableWidget->setItem(tRowCount, 0, tTableWidgetItemReplace);
        mTableWidget->setItem(tRowCount, 1, tTableWidgetItemWith);

        mLineEditReplace->clear();
        mLineEditWith->clear();

        mLineEditReplace->setFocus();
    }
}

void IBFWidgetAutoReplace::on_mPushButtonRemove_clicked()
{
    if ( !mTableWidget->selectedItems().isEmpty() )
    {
        mTableWidget->removeRow(mTableWidget->selectedItems().first()->row());

        mTableWidget->setFocus();
    }
}





IBFDialogPreferences::IBFDialogPreferences(QWidget *aParent) : QDialog(aParent)
{
    mNotesWidgetAutoReplace = new IBFWidgetAutoReplace();

    QTabWidget * tTabWidget = new QTabWidget();
    tTabWidget->addTab(mNotesWidgetAutoReplace, "Auto Replace");

    mPushButtonApply = new QPushButton("Apply");
    mPushButtonCancel = new QPushButton("Cancel");
    mPushButtonOK = new QPushButton("OK");

    QHBoxLayout * tHBoxLayout = new QHBoxLayout();
    tHBoxLayout->addWidget(mPushButtonApply);
    tHBoxLayout->addWidget(mPushButtonCancel);
    tHBoxLayout->addWidget(mPushButtonOK);
    tHBoxLayout->insertStretch(0);

    QVBoxLayout * tVBoxLayout = new QVBoxLayout();
    tVBoxLayout->addWidget(tTabWidget);
    tVBoxLayout->addLayout(tHBoxLayout);

    setLayout(tVBoxLayout);

    setWindowTitle(tr("Preferences"));

    connect(mPushButtonApply, SIGNAL(clicked()), this, SLOT(on_mPushButtonApply_clicked()));
    connect(mPushButtonCancel, SIGNAL(clicked()), this, SLOT(on_mPushButtonCancel_clicked()));
    connect(mPushButtonOK, SIGNAL(clicked()), this, SLOT(on_mPushButtonOK_clicked()));
}

IBFDialogPreferences::~IBFDialogPreferences()
{

}

void IBFDialogPreferences::on_mPushButtonApply_clicked()
{
    mNotesWidgetAutoReplace->IBFSaveSettings();
}

void IBFDialogPreferences::on_mPushButtonCancel_clicked()
{
    // QMessageBox::warning(this, )
    this->close();
}

void IBFDialogPreferences::on_mPushButtonOK_clicked()
{
    mNotesWidgetAutoReplace->IBFSaveSettings();
    this->close();
}
