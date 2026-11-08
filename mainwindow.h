#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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

    void on_actionSave_as_changed();

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

    void on_actionFornColor_triggered();

    void on_actionAutoExchangeLine_triggered();

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString filePath;
    bool textChanged;
    bool m_showLineNumber = true;

    bool userEditConfirmed();

};
#endif // MAINWINDOW_H
