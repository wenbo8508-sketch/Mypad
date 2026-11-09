#include "searchdialog.h"
#include "ui_searchdialog.h"

#include <QMessageBox>
#include <QPlainTextEdit>
#include <QTextCursor>

SearchDialog::SearchDialog(QPlainTextEdit *editor, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
    , m_editor(editor)
{
    ui->setupUi(this);
    setModal(false);
    setWindowFlags(windowFlags() | Qt::Window);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::setEditor(QPlainTextEdit *editor)
{
    m_editor = editor;
}

QTextDocument::FindFlags SearchDialog::findFlags() const
{
    QTextDocument::FindFlags flags;
    if (ui->radioUp->isChecked())
        flags |= QTextDocument::FindBackward;
    if (ui->caseCheckBox->isChecked())
        flags |= QTextDocument::FindCaseSensitively;
    return flags;
}

bool SearchDialog::findNext(const QString &text, QTextDocument::FindFlags flags)
{
    if (!m_editor || text.isEmpty())
        return false;

    if (m_editor->find(text, flags))
        return true;

    // 没找到就回到文档另一端再找一次（循环查找）
    QTextCursor cursor = m_editor->textCursor();
    cursor.movePosition(flags & QTextDocument::FindBackward
                            ? QTextCursor::End
                            : QTextCursor::Start);
    m_editor->setTextCursor(cursor);

    return m_editor->find(text, flags);
}

void SearchDialog::on_pushButtonFind_clicked()
{
    const QString text = ui->lineEdit->text();
    if (text.isEmpty())
        return;

    if (!findNext(text, findFlags()))
        QMessageBox::information(this, tr("查找"), tr("找不到您想要查找的文本!"));
}

void SearchDialog::on_lineEdit_returnPressed()
{
    on_pushButtonFind_clicked();
}

void SearchDialog::on_pushButtonCancel_clicked()
{
    close();
}
