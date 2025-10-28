#ifndef CUSTOMTHEMEDIALOG_H
#define CUSTOMTHEMEDIALOG_H

#include <QDialog>
#include <QHash>
#include <QColor>

namespace Ui {
class CustomThemeDialog;
}

class CustomThemeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomThemeDialog(QWidget *parent = nullptr);
    ~CustomThemeDialog();

    QHash<QString, QColor> getThemeColors() const;
    QString getThemeName() const;

private slots:
    void on_chooseBackgroundColor_clicked();
    void on_chooseTextColor_clicked();
    void on_chooseKeywordColor_clicked();
    void on_chooseStringColor_clicked();
    void on_chooseCommentColor_clicked();
    void on_chooseTypeColor_clicked();
    void on_chooseNumberColor_clicked();
    void on_saveTheme_clicked();
    void updatePreview();

private:
    Ui::CustomThemeDialog *ui;
    QHash<QString, QColor> themeColors;
    void chooseColor(const QString &colorName, QPushButton *button);
    void updateColorButton(QPushButton *button, const QColor &color);
};

#endif // CUSTOMTHEMEDIALOG_H
