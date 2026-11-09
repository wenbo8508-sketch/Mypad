#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include "codeeditor.h"

#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QColorDialog>
#include <QFontDialog>
#include <QPlainTextEdit>

namespace {
// 剪贴板内容为空时 QClipboard::mimeData() 返回 nullptr，直接调用 hasText() 会崩溃
bool clipboardHasText()
{
    const QMimeData *mimeData = QApplication::clipboard()->mimeData();
    return mimeData && mimeData->hasText();
}
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textChanged = false;

    statusLabel.setMaximumWidth(150);
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("宋文博"));
    ui->statusbar->addPermanentWidget(author);

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);

    // 剪贴板里有文本时才允许粘贴（剪贴板为空时 mimeData() 返回 nullptr，必须先判空）
    ui->actionPaste->setEnabled(clipboardHasText());
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, [this]() {
        ui->actionPaste->setEnabled(clipboardHasText());
    });

    // 自动换行：默认关闭，勾选状态与实际操作保持一致
    ui->testEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    ui->actionAutoExchangeLine->setChecked(false);

    // 行号：默认显示
    m_showLineNumber = true;
    ui->actionLineNumber->setChecked(m_showLineNumber);
    ui->testEdit->setShowLineNumber(m_showLineNumber);

    ui->actionShowStatusBar->setChecked(true);
    ui->actionShowToolbar->setChecked(true);

    updateStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_A_triggered()
{
    AboutDialog dig(this);
    dig.exec();
}


void MainWindow::on_actionFind_triggered()
{
    if (!searchDialog)
        searchDialog = new SearchDialog(ui->testEdit, this);
    searchDialog->setEditor(ui->testEdit);
    searchDialog->show();
    searchDialog->raise();
    searchDialog->activateWindow();
}


void MainWindow::on_actionReplace_triggered()
{
    if (!replaceDialog)
        replaceDialog = new ReplaceDialog(ui->testEdit, this);
    replaceDialog->setEditor(ui->testEdit);
    replaceDialog->show();
    replaceDialog->raise();
    replaceDialog->activateWindow();
}


void MainWindow::on_actionOpen_triggered()
{
    if (!userEditConfirmed())
        return;

    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".",
                                                    tr("Text files (*.txt) ;; All (*.*)"));
    if (filename.isEmpty())
        return;

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "打开文件失败");
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    file.close();

    // 用 setPlainText 整体替换内容，而不是 insertPlainText 追加内容
    ui->testEdit->setPlainText(text);
    ui->testEdit->document()->clearUndoRedoStacks();

    filePath = filename;
    textChanged = false;
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    updateStatusBar();
}


// 把当前内容写到 filePath，成功返回 true
bool MainWindow::saveFile()
{
    if (filePath.isEmpty())
        return false;

    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..",
                             tr("保存文件失败:\n%1").arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out << ui->testEdit->toPlainText();
    file.flush();
    file.close();

    textChanged = false;
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    return true;
}


void MainWindow::on_actionSave_triggered()
{
    if (filePath.isEmpty()) {
        QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".",
                                                        tr("Text files (*.txt)"));
        if (filename.isEmpty())
            return;
        filePath = filename;
    }

    saveFile();
}


void MainWindow::on_actionNew_triggered()
{
    if (!userEditConfirmed())
        return;

    ui->testEdit->clear();
    ui->testEdit->document()->clearUndoRedoStacks();

    filePath.clear();
    textChanged = false;
    this->setWindowTitle(tr("无标题 - 编辑器"));
    updateStatusBar();
}


void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".",
                                                    tr("Text files (*.txt)"));
    // 用户取消保存，直接返回
    if (filename.isEmpty())
        return;

    filePath = filename;
    saveFile();
}


bool MainWindow::userEditConfirmed()
{
    if (textChanged) {
        QString path = filePath.isEmpty() ? tr("无标题.txt") : filePath;

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setText(tr("是否将更改保存到\n\"%1\" ?").arg(path));
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int r = msg.exec();
        switch (r) {
        case QMessageBox::Yes:
            if (filePath.isEmpty()) {
                QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".",
                                                                tr("Text files (*.txt)"));
                if (filename.isEmpty())
                    return false;
                filePath = filename;
            }
            if (!saveFile())
                return false;
            break;
        case QMessageBox::No:
            textChanged = false;
            break;
        case QMessageBox::Cancel:
            return false;
        default:
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
    QColor color = QColorDialog::getColor(Qt::black, this, "选择字体颜色");
    if (!color.isValid())
        return;

    // 用调色板而不是样式表，避免和背景色互相覆盖
    QPalette pal = ui->testEdit->palette();
    pal.setColor(QPalette::Text, color);
    ui->testEdit->setPalette(pal);
}


void MainWindow::on_actionSettingColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "选择编辑器背景色");
    if (!color.isValid())
        return;

    QPalette pal = ui->testEdit->palette();
    pal.setColor(QPalette::Base, color);
    // 行号区用的是 Window 角色，一起改掉，否则改了背景色后左边的行号栏还是灰色
    pal.setColor(QPalette::Window, color);
    ui->testEdit->setPalette(pal);
}


void MainWindow::on_actionLineNumber_triggered()
{
    m_showLineNumber = ui->actionLineNumber->isChecked();
    ui->testEdit->setShowLineNumber(m_showLineNumber);
}


void MainWindow::on_actionFort_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, ui->testEdit->font(), this, "选择字体");
    if (ok)
        ui->testEdit->setFont(font);
}


void MainWindow::on_actionFornColor_triggered()
{
    QTextCursor cursor = ui->testEdit->textCursor();
    if (!cursor.hasSelection()) {
        QMessageBox::information(this, tr("字体背景色"), tr("请先选中要设置背景色的文字!"));
        return;
    }

    QColor color = QColorDialog::getColor(Qt::yellow, this, "选择字体背景色");
    if (!color.isValid())
        return;

    QTextCharFormat format;
    format.setBackground(color);
    cursor.mergeCharFormat(format);
}


void MainWindow::on_actionAutoExchangeLine_triggered()
{
    bool checked = ui->actionAutoExchangeLine->isChecked();
    ui->testEdit->setLineWrapMode(checked ? QPlainTextEdit::WidgetWidth
                                          : QPlainTextEdit::NoWrap);
}


void MainWindow::on_actionShowStatusBar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionShowStatusBar->setChecked(!visible);
}


void MainWindow::on_actionShowToolbar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionShowToolbar->setChecked(!visible);
}


void MainWindow::on_actionAll_triggered()
{
    ui->testEdit->selectAll();
}


void MainWindow::on_actionExit_triggered()
{
    if (userEditConfirmed())
        close();
}


void MainWindow::on_testEdit_textChanged()
{
    if (!textChanged) {
        this->setWindowTitle("*" + this->windowTitle());
        textChanged = true;
    }
    updateStatusBar();
}


void MainWindow::on_testEdit_cursorPositionChanged()
{
    updateStatusBar();
}


// 状态栏：文本长度、行数，以及光标所在的行号与列号
void MainWindow::updateStatusBar()
{
    QTextDocument *doc = ui->testEdit->document();

    // characterCount() 末尾多算一个空字符，blockCount() 是段落数
    int length = qMax(0, doc->characterCount() - 1);
    int lineCount = qMax(1, doc->blockCount());

    statusLabel.setText(tr("length: %1  lines: %2").arg(length).arg(lineCount));

    QTextCursor cursor = ui->testEdit->textCursor();
    statusCursorLabel.setText(tr("Ln: %1  Col: %2")
                                  .arg(cursor.blockNumber() + 1)
                                  .arg(cursor.columnNumber() + 1));
}
