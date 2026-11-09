#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QTextDocument>

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
QT_END_NAMESPACE

namespace Ui {
class ReplaceDialog;
}

// 替换对话框
class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QPlainTextEdit *editor = nullptr, QWidget *parent = nullptr);
    ~ReplaceDialog() override;

    void setEditor(QPlainTextEdit *editor);

    // 打开对话框时把编辑器里选中的词填进查找框，方便直接替换这个词
    void setKeyword(const QString &keyword);

private slots:
    void on_pushButtonFindNext_clicked();
    void on_pushButtonReplace_clicked();
    void on_pushButtonReplaceAll_clicked();
    void on_lineEditFind_returnPressed();
    void on_pushButtonCancel_clicked();

private:
    Ui::ReplaceDialog *ui;
    QPlainTextEdit *m_editor = nullptr;

    QTextDocument::FindFlags findFlags() const;
    bool findNext(const QString &text, QTextDocument::FindFlags flags);
    bool selectionIsTarget(const QString &text, Qt::CaseSensitivity cs) const;
    void replaceSelection();
};

#endif // REPLACEDIALOG_H
