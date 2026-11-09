#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QTextDocument>

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
QT_END_NAMESPACE

namespace Ui {
class SearchDialog;
}

// 查找对话框
class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QPlainTextEdit *editor = nullptr, QWidget *parent = nullptr);
    ~SearchDialog() override;

    void setEditor(QPlainTextEdit *editor);

    // 打开对话框时把编辑器里选中的词填进查找框，方便直接查找这个词
    void setKeyword(const QString &keyword);

private slots:
    void on_pushButtonFind_clicked();
    void on_lineEdit_returnPressed();
    void on_pushButtonCancel_clicked();

private:
    Ui::SearchDialog *ui;
    QPlainTextEdit *m_editor = nullptr;

    QTextDocument::FindFlags findFlags() const;
    bool findNext(const QString &text, QTextDocument::FindFlags flags);
};

#endif // SEARCHDIALOG_H
