#include "searchdialog.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Поиск и замена");
    setFixedSize(400, 150);

    findLabel = new QLabel("Найти:", this);
    replaceLabel = new QLabel("Заменить на:", this);
    findLineEdit = new QLineEdit(this);
    replaceLineEdit = new QLineEdit(this);
    findButton = new QPushButton("Найти далее", this);
    replaceButton = new QPushButton("Заменить", this);
    replaceAllButton = new QPushButton("Заменить все", this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *findLayout = new QHBoxLayout();
    findLayout->addWidget(findLabel);
    findLayout->addWidget(findLineEdit);
    findLayout->addWidget(findButton);

    QHBoxLayout *replaceLayout = new QHBoxLayout();
    replaceLayout->addWidget(replaceLabel);
    replaceLayout->addWidget(replaceLineEdit);
    replaceLayout->addWidget(replaceButton);
    replaceLayout->addWidget(replaceAllButton);

    mainLayout->addLayout(findLayout);
    mainLayout->addLayout(replaceLayout);

    connect(findButton, &QPushButton::clicked, this, &SearchDialog::on_findButton_clicked);
    connect(replaceButton, &QPushButton::clicked, this, &SearchDialog::on_replaceButton_clicked);
    connect(replaceAllButton, &QPushButton::clicked, this, &SearchDialog::on_replaceAllButton_clicked);
}

SearchDialog::~SearchDialog()
{
}

void SearchDialog::on_findButton_clicked()
{
    emit findNext(findLineEdit->text());
}

void SearchDialog::on_replaceButton_clicked()
{
    emit replaceText(findLineEdit->text(), replaceLineEdit->text());
}

void SearchDialog::on_replaceAllButton_clicked()
{
    emit replaceAll(findLineEdit->text(), replaceLineEdit->text());
}
