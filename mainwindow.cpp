#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTextEdit>
#include <QColorDialog>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textChanged = false;

    ui->statusbar;
    statusLabel.setMaximumWidth(150);
    statusLabel.setText("length " + QString::number(0) + "  lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln: " + QString::number(0) + "  Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("宋文博"));
    ui->statusbar->addPermanentWidget(author);

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionPaste->setEnabled(false);

    QPlainTextEdit::LineWrapMode mode = ui->testEdit->lineWrapMode();
    if ( mode == QPlainTextEdit::NoWrap ) {
        ui->testEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionAutoExchangeLine->setChecked(false);
    } else {
        ui->testEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionAutoExchangeLine->setChecked(true);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_A_triggered()
{
    AboutDialog dig;
    dig.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog dig;
    dig.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dig;
    dig.exec();
}


void MainWindow::on_actionOpen_triggered()
{
    if(!userEditConfirmed())
        return;


    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".",tr("Text files (*.txt) ;; All (*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "..", "打开文件失败");
        return;
    }

    filePath = filename;

    QTextStream in(&file);
    QString text = in.readAll();
    ui->testEdit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    textChanged = false;
}


void MainWindow::on_actionSave_triggered()
{
    if(filePath == ""){
        QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".",
        tr("Text files (*.txt)"));
        QFile file(filename);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "..", "打开保存文件失败");
            return;
        }
    }


    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".",
                                                        tr("Text files (*.txt)"));
        QFile file(filename);  // 重名！
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "..", "打开保存文件失败");
            return;
        }
        filePath = filename;
    }

    QTextStream out(&file);
    QString text = ui->testEdit->toPlainText();
    out << text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    textChanged = false;
}


void MainWindow::on_actionNew_triggered()
{
    ui->testEdit->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));
}


void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", tr("Text files (*.txt)"));
    // 用户取消保存，直接退出
    if (filename.isEmpty())
    {
        return;
    }
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "保存文件失败");
        return;
    }

    filePath = filename;
    QTextStream out(&file);
    QString text = ui->testEdit->toPlainText();
    out << text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
}


void MainWindow::on_actionSave_as_changed()
{
    if(!textChanged)
    {
        this->setWindowTitle("*" + this->windowTitle());
        textChanged = true;
    }
}

bool MainWindow::userEditConfirmed()
{
    if (textChanged) {
        QString path = (filePath != "") ? filePath : "无标题.txt";

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否将更改保存到\n") + "\"" + path + "\" ?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int r = msg.exec();
        switch (r) {
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            textChanged = false;
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }
    return true;
}



void MainWindow::on_actionCut_triggered()
{
    ui->testEdit->cut();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->testEdit->copy();
    ui->actionCopy->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    ui->testEdit->paste();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->testEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->testEdit->redo();
}


void MainWindow::on_testEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_testEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_testEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}


void MainWindow::on_actionColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::blue,this,"选择颜色");
    if(color.isValid()){
        ui->testEdit->setStyleSheet(QString("QPlainTextEdit {color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionSettingColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::blue,this,"选择颜色");
    if(color.isValid()){
        ui->testEdit->setStyleSheet(QString("QPlainTextEdit {background-color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionLineNumber_triggered()
{
    m_showLineNumber = !m_showLineNumber;
    ui->actionLineNumber->setChecked(m_showLineNumber);

    // 注意：工程里还没有 CodeEditor 这个类（也没有 codeeditor.h/.cpp），
    // 原来的 qobject_cast<CodeEditor*>(ui->testEdit) 编译不过，先注释掉。
    // 等你写到行号绘制那一步，再把它换成自定义控件并恢复下面两行：
    // CodeEditor *editor = qobject_cast<CodeEditor *>(ui->testEdit);
    // if (editor) editor->setShowLineNumber(m_showLineNumber);
}


void MainWindow::on_actionFornColor_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,this);

    if(ok)
        ui->testEdit->setFont(font);
}


void MainWindow::on_actionAutoExchangeLine_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->testEdit->lineWrapMode();
    if ( mode == QPlainTextEdit::NoWrap ) {
        ui->testEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionAutoExchangeLine->setChecked(true);
    } else {
        ui->testEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionAutoExchangeLine->setChecked(false);
    }

}

