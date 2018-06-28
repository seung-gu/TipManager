#include "tipmanager.h"
#include "ui_tipmanager.h"

#include <QDebug>

TipManager::TipManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TipManager)
{
    ui->setupUi(this);

    flagTitleEdit = flagCodeTextEdit = flagMemoTextEdit = false;
    flagModified = false;

  //change the amount of tap space(instead of 8 blanks) in codeTextEdit
    ui->codeTextEdit->setTabStopWidth(20);
  //setting directory
    changeCurrentPath(QDir::currentPath());
  //setting dirEdit
    ui->dirEdit->setReadOnly(true);
  //setting ctrTreeView
    ctrTreeView();

  //key event
    //  ui->treeView->installEventFilter(this);
}



TipManager::~TipManager()
{
    delete ui;
}

void TipManager::ctrTreeView()
{
    model = new QDirModel;
    model->setReadOnly(true);

  //sorting order
    model->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);

  //filtering (text and directory file only)
    model->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    model->setNameFilters(QStringList() << "*.txt" );

  //setting directory ( current directory -> file )
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(currentPath));

  //showing only name in directory path
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

  //signal connection
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &TipManager::indexChanged);
}

void TipManager::loadFile(QString fileName)
{
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream in(&file);
        editLoadText(in.readAll());
        file.close();

        disableTextBox();
    }
}

void TipManager::editLoadText(QString totalText)
{
    ui->codeTextEdit->setText(totalText);
    totalText = totalText.trimmed();

    int it_start = 0, it_end = 0;

    it_start = totalText.indexOf("*\n", it_end);
    it_end = totalText.indexOf("\n*",it_start+2);

    QString titleText = totalText.mid(it_start+2, it_end-it_start-2);
    ui->titleEdit->setText(titleText);

    it_start = totalText.indexOf("*\n", it_end);
    it_end = totalText.indexOf("\n*",it_start+2);

    QString memoText = totalText.mid(it_start+2, it_end-it_start-2);
    ui->memoTextEdit->setText(memoText);

    it_start = totalText.indexOf("*/\n", it_end);

    QString codeText = totalText.mid(it_start+3);
    ui->codeTextEdit->setText(codeText);

    flagModified = false;
}

void TipManager::saveFile(QString fileName)
{
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream out(&file);
        out << editSaveText();
        file.close();
    }
}

QString TipManager::editSaveText()
{
    QString titleLine = "/* * * * * * * * * * * * * * * * * Title * * * * * * * * * * * * * * * * * * *\n";
    QString titleText = ui->titleEdit->text().trimmed()+"\n";
    QString memoLine = "* * * * * * * * * * * * * * * * * * Memo * * * * * * * * * * * * * * * * * * *\n";
    QString memoText = ui->memoTextEdit->toPlainText().trimmed()+"\n";
    QString codeLine = "* * * * * * * * * * * * * * * * * * Code * * * * * * * * * * * * * * * * * * */\n";
    QString codeText = ui->codeTextEdit->toPlainText().trimmed();

    QString editText = titleLine+titleText+memoLine+memoText+codeLine+codeText;

    return editText;
}

void TipManager::cleanTextBox()
{
    ui->titleEdit->clear();
    ui->codeTextEdit->clear();
    ui->memoTextEdit->clear();

    enableTextBox();
}

void TipManager::enableButtons()
{
    //disable new button when all text boxes are empty
    flagTitleEdit|flagCodeTextEdit|flagMemoTextEdit
            ? ui->newButton->setEnabled(true)
            : ui->newButton->setEnabled(false);
    //enable save button when only both title and codeText bit are true
    flagTitleEdit&flagCodeTextEdit
            ? ui->saveButton->setEnabled(true)
            : ui->saveButton->setEnabled(false);
    //enable edit button when title is disabled
    ui->titleEdit->isEnabled()
            ? ui->editButton->setEnabled(false)
            : ui->editButton->setEnabled(true);
}

void TipManager::enableTextBox()
{
    ui->titleEdit->setEnabled(true);
    ui->codeTextEdit->setEnabled(true);
    ui->memoTextEdit->setEnabled(true);

    enableButtons();
}

void TipManager::disableTextBox()
{
    ui->titleEdit->setEnabled(false);
    ui->codeTextEdit->setEnabled(false);
    ui->memoTextEdit->setEnabled(false);

    enableButtons();
}

void TipManager::on_titleEdit_textChanged(const QString &title)
{
    //when title text is empty, title bit is false
    QString text = title;
    text.trimmed().isEmpty() ? flagTitleEdit = false
                   : flagTitleEdit = true;
    //memo title edit is modified
    flagModified = true;

    enableButtons();
}

void TipManager::on_codeTextEdit_textChanged()
{
    //when code text is empty, codeText bit is false
    QString text = ui->codeTextEdit->toPlainText();
    text.trimmed().isEmpty() ? flagCodeTextEdit = false
                    : flagCodeTextEdit = true;
    //code text edit is modified
    flagModified = true;

    enableButtons();
}

void TipManager::on_memoTextEdit_textChanged()
{
    //when memo text is empty, memoText bit is false
    QString text = ui->memoTextEdit->toPlainText();
    text.trimmed().isEmpty() ? flagMemoTextEdit = false
                    : flagMemoTextEdit = true;
    //memo text edit is modified
    flagModified = true;

    enableButtons();
}

void TipManager::on_newButton_clicked()
{
    //asking save dialog box -> save dialog
    QMessageBox msgBox;
    msgBox.setWindowTitle("New document");
    msgBox.setText("Are you sure?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch (ret) {
        case QMessageBox::Yes:
            // Yes was clicked
            cleanTextBox();
            break;
        case QMessageBox::No:
            // No was clicked
            break;
        default:
            // should never be reached
            break;
    }
}

void TipManager::on_editButton_clicked()
{
    enableTextBox();
    enableButtons();
}


void TipManager::on_searchButton_clicked()
{
    Window* findDialog = new Window();
    findDialog->setStyleSheet("background-color: rgb(94,96,98)"); //set color
    findDialog->show();
}

//load directory
void TipManager::on_loadButton_clicked()
{
  //load folder directory dialog
    QString dirName = QFileDialog::getExistingDirectory(this,
          tr("Open Directory"), currentPath, QFileDialog::ShowDirsOnly
                                             | QFileDialog::DontResolveSymlinks);
  //to avoid when it is an empty directory
    if(!dirName.isEmpty()){
        changeCurrentPath(dirName);
      //to show the directory folder in the tree view
        ctrTreeView();
    }
}

void TipManager::on_saveButton_clicked()
{
    //save dialog
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Tip Code"), currentPath +"/"+ ui->titleEdit->text().trimmed(),
            tr("Tip Code (*.txt);;All Files (*)"));
    saveFile(fileName);
}

void TipManager::on_aboutButton_clicked()
{

}

void TipManager::on_treeView_clicked(const QModelIndex &index)
{
    trimCurrentPath(index);
}

void TipManager::trimCurrentPath(const QModelIndex &index)
{
    QString dirPath = model->filePath(index);
    int it = dirPath.indexOf(".txt");

    if(it==-1){         //in case of folder
      //the path(dir) can be changed into new directory in case of folder
        changeCurrentPath(dirPath);
    }else{              //in case of txt file
      //load the text file from the path
        loadFile(dirPath);
      //remove the last file name to show the current directory
        it = dirPath.lastIndexOf(QChar('/'));
        changeCurrentPath(dirPath.left(it));
    }
}

void TipManager::changeCurrentPath(QString m_currentPath)
{
    currentPath = m_currentPath;
    ui->dirEdit->setText(currentPath);
}

/*
bool TipManager::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->treeView && event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        QModelIndex currentIndex = ui->treeView->currentIndex();

        if( keyEvent->key() == Qt::Key_Up )
        {
            //currentIndex = currentIndex.child(currentIndex.row()+1,currentIndex.column());
        }
        if( keyEvent->key() == Qt::Key_Down )
        {
            //currentIndex = currentIndex.child(currentIndex.row()-1,currentIndex.column());
        }
        if(currentIndex.isValid())
        {
            ui->treeView->setCurrentIndex(currentIndex);
            trimCurrentPath(currentIndex);
        }
    }
    return QWidget::eventFilter(watched, event);
}*/

void TipManager::indexChanged(const QModelIndex &current)
{
    trimCurrentPath(current);
}

