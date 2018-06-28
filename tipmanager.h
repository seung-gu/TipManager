#ifndef TIPMANAGER_H
#define TIPMANAGER_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QTextBlock>
#include <QDataWidgetMapper>
#include <QTextEdit>
#include <QKeyEvent>
#include <QDirModel>
#include <QDateTime>

#include "window.h"

namespace Ui {
class TipManager;
}

class TipManager : public QWidget
{
    Q_OBJECT

public:
    explicit TipManager(QWidget *parent = 0);
    ~TipManager();

 //   bool eventFilter(QObject *watched, QEvent *event);

private:
  //control tree view
    void ctrTreeView();
  //load text file from directory   
    void loadFile(QString);
    void editLoadText(QString);
  //save text file from the program 
    void saveFile(QString);
    QString editSaveText();
  //clean textBox
    void cleanTextBox();
  //enable
    void enableTextBox();
    void enableButtons();
  //disable
    void disableTextBox();
  //change current path
    void trimCurrentPath(const QModelIndex &index);
    void changeCurrentPath(QString);

private slots:
  //button click event
    void on_newButton_clicked();
    void on_editButton_clicked();
    void on_searchButton_clicked();
    void on_loadButton_clicked();   //load directory
    void on_saveButton_clicked();
    void on_aboutButton_clicked();
    void on_treeView_clicked(const QModelIndex &index);
  //changed state event
    void on_titleEdit_textChanged(const QString &title);
    void on_codeTextEdit_textChanged();
    void on_memoTextEdit_textChanged();

public slots:
    void indexChanged(const QModelIndex &current);


private:
    Ui::TipManager *ui;  
    QString currentPath;
    bool flagTitleEdit, flagCodeTextEdit, flagMemoTextEdit;
    bool flagModified;
    QDirModel *model;
};

#endif // TIPMANAGER_H
