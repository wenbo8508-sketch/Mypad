#include "replacedialog.h"
#include "ui_replacedialog.h"

#include <QMessageBox>
#include <QPlainTextEdit>
#include <QTextCursor>

ReplaceDialog::ReplaceDialog(QPlainTextEdit *editor, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
    , m_editor(editor)
{
    ui->setupUi(this);
    setModal(false);
    setWindowFlags(windowFlags() | Qt::Window);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::setEditor(QPlainTextEdit *editor)
{
    m_editor = editor;
}

QTextDocument::FindFlags ReplaceDialog::findFlags() const
{
    QTextDocument::FindFlags flags;
    if (ui->radioUp->isChecked())
        flags |= QTextDocument::FindBackward;
    if (ui->caseCheckBox->isChecked())
        flags |= QTextDocument::FindCaseSensitively;
    return flags;
}

bool ReplaceDialog::findNext(const QString &text, QTextDocument::FindFlags flags)
{
    if (!m_editor || text.isEmpty())
        return false;

    if (m_editor->find(text, flags))
        return true;

    QTextCursor cursor = m_editor->textCursor();
    cursor.movePosition(flags & QTextDocument::FindBackward
                            ? QTextCursor::End
                            : QTextCursor::Start);
    m_editor->setTextCursor(cursor);

    return m_editor->find(text, flags);
}

bool ReplaceDialog::selectionIsTarget(const QString &text, Qt::CaseSensitivity cs) const
{
    if (!m_editor || !m_editor->textCursor().hasSelection())
        return false;

    return m_editor->textCursor().selectedText().compare(text, cs) == 0;
}

void ReplaceDialog::replaceSelection()
{
    QTextCursor cursor = m_editor->textCursor();
    cursor.insertText(ui->lineEditReplace->text());
    m_editor->setTextCursor(cursor);
}

void ReplaceDialog::on_pushButtonFindNext_clicked()
{
    const QString text = ui->lineEditFind->text();
    if (text.isEmpty())
        return;

    if (!findNext(text, findFlags()))
        QMessageBox::information(this, tr("替换"), tr("找不到您想要查找的文本!"));
}

void ReplaceDialog::on_pushButtonReplace_clicked()
{
    const QString text = ui->lineEditFind->text();
    if (!m_editor || text.isEmpty())
        return;

    const Qt::CaseSensitivity cs = ui->caseCheckBox->isChecked()
                                       ? Qt::CaseSensitive
                                       : Qt::CaseInsensitive;

    // 当前选中的就是目标文本，直接替换；否则先查找再替换
    if (!selectionIsTarget(text, cs) && !findNext(text, findFlags())) {
        QMessageBox::information(this, tr("替换"), tr("找不到您想要查找的文本!"));
        return;
    }

    replaceSelection();
}

void ReplaceDialog::on_pushButtonReplaceAll_clicked()
{
    const QString text = ui->lineEditFind->text();
    if (!m_editor || text.isEmpty())
        return;

    QTextDocument::FindFlags flags;
    if (ui->caseCheckBox->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    QTextCursor cursor = m_editor->textCursor();
    cursor.movePosition(QTextCursor::Start);
    m_editor->setTextCursor(cursor);

    int count = 0;
    while (m_editor->find(text, flags)) {
        replaceSelection();
        if (++count >= 100000)   // 防止替换文本包含查找文本时无限循环
            break;
    }

    QMessageBox::information(this, tr("替换"),
                             tr("已完成 %1 处替换").arg(count));
}

void ReplaceDialog::on_lineEditFind_returnPressed()
{
    on_pushButtonFindNext_clicked();
}

void ReplaceDialog::on_pushButtonCancel_clicked()
{
    close();
}
