#include "customthemedialog.h"
#include "ui_customthemedialog.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QPushButton>

CustomThemeDialog::CustomThemeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomThemeDialog)
{
    ui->setupUi(this);
    setWindowTitle("Создание пользовательской темы");

    themeColors["background"] = QColor(255, 255, 255);
    themeColors["text"] = QColor(0, 0, 0);
    themeColors["keyword"] = QColor(136, 18, 128);
    themeColors["string"] = QColor(163, 21, 21);
    themeColors["comment"] = QColor(0, 128, 0);
    themeColors["type"] = QColor(36, 138, 145);
    themeColors["number"] = QColor(28, 0, 207);

    updateColorButton(ui->chooseBackgroundColor, themeColors["background"]);
    updateColorButton(ui->chooseTextColor, themeColors["text"]);
    updateColorButton(ui->chooseKeywordColor, themeColors["keyword"]);
    updateColorButton(ui->chooseStringColor, themeColors["string"]);
    updateColorButton(ui->chooseCommentColor, themeColors["comment"]);
    updateColorButton(ui->chooseTypeColor, themeColors["type"]);
    updateColorButton(ui->chooseNumberColor, themeColors["number"]);

    connect(ui->themeNameEdit, &QLineEdit::textChanged, this, &CustomThemeDialog::updatePreview);

    updatePreview();
}

CustomThemeDialog::~CustomThemeDialog()
{
    delete ui;
}

void CustomThemeDialog::chooseColor(const QString &colorName, QPushButton *button)
{
    QColor color = QColorDialog::getColor(themeColors[colorName], this, "Выберите цвет");
    if (color.isValid()) {
        themeColors[colorName] = color;
        updateColorButton(button, color);
        updatePreview();
    }
}

void CustomThemeDialog::updateColorButton(QPushButton *button, const QColor &color)
{
    QString style = QString("background-color: %1; color: %2; border: 1px solid black;")
                    .arg(color.name())
                    .arg(color.lightness() > 128 ? "black" : "white");
    button->setStyleSheet(style);
    button->setText(color.name());
}

void CustomThemeDialog::on_chooseBackgroundColor_clicked()
{
    chooseColor("background", ui->chooseBackgroundColor);
}

void CustomThemeDialog::on_chooseTextColor_clicked()
{
    chooseColor("text", ui->chooseTextColor);
}

void CustomThemeDialog::on_chooseKeywordColor_clicked()
{
    chooseColor("keyword", ui->chooseKeywordColor);
}

void CustomThemeDialog::on_chooseStringColor_clicked()
{
    chooseColor("string", ui->chooseStringColor);
}

void CustomThemeDialog::on_chooseCommentColor_clicked()
{
    chooseColor("comment", ui->chooseCommentColor);
}

void CustomThemeDialog::on_chooseTypeColor_clicked()
{
    chooseColor("type", ui->chooseTypeColor);
}

void CustomThemeDialog::on_chooseNumberColor_clicked()
{
    chooseColor("number", ui->chooseNumberColor);
}

QHash<QString, QColor> CustomThemeDialog::getThemeColors() const
{
    return themeColors;
}

QString CustomThemeDialog::getThemeName() const
{
    return ui->themeNameEdit->text();
}

void CustomThemeDialog::on_saveTheme_clicked()
{
    if (ui->themeNameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите название темы!");
        return;
    }
    accept();
}

void CustomThemeDialog::updatePreview()
{
    QColor bgColor = themeColors["background"];
    QColor textColor = themeColors["text"];
    QColor keywordColor = themeColors["keyword"];
    QColor stringColor = themeColors["string"];
    QColor commentColor = themeColors["comment"];
    QColor numberColor = themeColors["number"];

    QString previewStyle = QString("background-color: %1; padding: 10px; border: 1px solid gray;")
                          .arg(bgColor.name());
    ui->previewLabel->setStyleSheet(previewStyle);

    QString previewText = QString(
        "<span style=\"color: %1;\"><strong>Предпросмотр темы: %2</strong></span><br/>"
        "<span style=\"color: %3;\">// Это комментарий</span><br/>"
        "<span style=\"color: %4;\">function</span> <span style=\"color: %1;\">example()</span> {<br/>"
        "&nbsp;&nbsp;&nbsp;&nbsp;<span style=\"color: %4;\">var</span> number = <span style=\"color: %5;\">123</span>;<br/>"
        "&nbsp;&nbsp;&nbsp;&nbsp;string = <span style=\"color: %6;\">\"текст\"</span>;<br/>"
        "&nbsp;&nbsp;&nbsp;&nbsp;<span style=\"color: %4;\">return</span> true;<br/>"
        "}")
        .arg(textColor.name())
        .arg(ui->themeNameEdit->text().isEmpty() ? "Новая тема" : ui->themeNameEdit->text())
        .arg(commentColor.name())
        .arg(keywordColor.name())
        .arg(numberColor.name())
        .arg(stringColor.name());

    ui->previewLabel->setText(previewText);
    ui->previewLabel->setTextFormat(Qt::RichText);
}
