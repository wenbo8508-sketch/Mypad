#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class SearchDialog;
class ReplaceDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_action_A_triggered();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionNew_triggered();

    void on_actionSave_as_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_testEdit_copyAvailable(bool b);

    void on_testEdit_undoAvailable(bool b);

    void on_testEdit_redoAvailable(bool b);

    void on_actionColor_triggered();

    void on_actionSettingColor_triggered();

    void on_actionLineNumber_triggered();

    void on_actionFort_triggered();

    void on_actionFornColor_triggered();

    void on_actionAutoExchangeLine_triggered();

    void on_actionShowStatusBar_triggered();

    void on_actionShowToolbar_triggered();

    void on_actionAll_triggered();

    void on_actionExit_triggered();

    void on_testEdit_textChanged();

    void on_testEdit_cursorPositionChanged();

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString filePath;
    bool textChanged;
    bool m_showLineNumber = true;

    // 查找/替换对话框（非模态，重复打开时复用同一个）
    SearchDialog *searchDialog = nullptr;
    ReplaceDialog *replaceDialog = nullptr;

    bool userEditConfirmed();
    bool saveFile();
    void updateStatusBar();
};
#endif // MAINWINDOW_H
