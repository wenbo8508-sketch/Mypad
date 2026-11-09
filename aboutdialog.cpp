#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QPixmap>
#include <QPushButton>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    // 标准按钮文字跟随系统语言，英文系统会显示 OK，这里固定成参考图上的“确定”
    if (QPushButton *ok = ui->buttonBox->button(QDialogButtonBox::Ok))
        ok->setText(tr("确定"));

    // 资源里的 logo 是 24x24，直接按控件尺寸硬拉伸会发虚，改用平滑缩放绘制
    QPixmap logo(QStringLiteral(":/img/png/24px_blue/logo.png"));
    if (!logo.isNull()) {
        ui->logoLabel->setScaledContents(false);
        ui->logoLabel->setAlignment(Qt::AlignCenter);
        ui->logoLabel->setPixmap(logo.scaled(ui->logoLabel->size(), Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
    }
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
