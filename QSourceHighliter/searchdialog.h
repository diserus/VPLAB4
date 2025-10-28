#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class SearchDialog; }
QT_END_NAMESPACE

class QLineEdit;
class QPushButton;
class QLabel;

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

private:

    QLineEdit *findLineEdit;
    QLineEdit *replaceLineEdit;
    QPushButton *findButton;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;
    QLabel *findLabel;
    QLabel *replaceLabel;

signals:
    void findNext(const QString &text);
    void replaceText(const QString &find, const QString &replace);
    void replaceAll(const QString &find, const QString &replace);

private slots:
    void on_findButton_clicked();
    void on_replaceButton_clicked();
    void on_replaceAllButton_clicked();
};

#endif // SEARCHDIALOG_H
