/*
 * Copyright (c) 2019-2020 Waqar Ahmed -- <waqar.17a@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qsourcehighliter.h"
#include "qsourcehighliterthemes.h"
#include "customthemedialog.h"
#include "QJsonObject"
#include "QJsonDocument"
#include "QFileDialog"
#include "QMessageBox"
#include "searchdialog.h"
#include "QToolBar"
#include <QHBoxLayout>
#include <QProcess>
#include <QTemporaryFile>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>

using namespace QSourceHighlite;

QHash<QString, QSourceHighliter::Language> MainWindow::_langStringToEnum;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // === Инициализация оригинальных компонентов ===
    initLangsEnum();
    initLangsComboBox();
    initThemesComboBox();

    QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    ui->plainTextEdit->setFont(f);
    highlighter = new QSourceHighliter(ui->plainTextEdit->document());

    connect(ui->langComboBox,
            static_cast<void (QComboBox::*) (const QString&)>(&QComboBox::currentTextChanged),
            this, &MainWindow::languageChanged);
    connect(ui->themeComboBox,
            static_cast<void (QComboBox::*) (int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::themeChanged);

    connect(ui->actionJSON, &QAction::triggered, this, &MainWindow::on_actionSaveJson_triggered);
    connect(ui->actionJSON_2, &QAction::triggered, this, &MainWindow::on_actionOpenJson_triggered);
    connect(ui->actionTXT, &QAction::triggered, this, &MainWindow::on_actionSaveTxt_triggered);
    connect(ui->actionTXT_3, &QAction::triggered, this, &MainWindow::on_actionOpenTxt_triggered);
    connect(ui->action_11, &QAction::triggered, this, &MainWindow::on_actionSearch_triggered);

    ui->langComboBox->setCurrentText("Asm");
    languageChanged("Asm");

    // Иконки
    ui->actionTXT->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->actionJSON->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->actionTXT_3->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->actionJSON_2->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->action_11->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    ui->action_5->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    ui->action_6->setIcon(style()->standardIcon(QStyle::SP_DirLinkIcon));
    ui->action_7->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    ui->action_9->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    ui->action_10->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    ui->action_8->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));

    createToolBar();

    // QMenu *themeMenu = menuBar()->addMenu("Темы");
    // QAction *customThemeAction = new QAction("Создать пользовательскую тему", this);
    // themeMenu->addAction(customThemeAction);
    // connect(customThemeAction, &QAction::triggered, this, &MainWindow::on_actionCustomTheme_triggered);

    // === ДОБАВЛЕНИЕ "Выполнить" и вывода ===

    // Создаём виджет вывода
    outputTextEdit = new QPlainTextEdit(this);
    outputTextEdit->setReadOnly(true);
    outputTextEdit->setPlaceholderText("Результат выполнения скрипта появится здесь...");
    outputTextEdit->setMaximumWidth(300); // ограничим ширину

    // Кнопка "Выполнить"
    runButton = new QPushButton("Выполнить", this);
    runButton->setFixedWidth(100);
    runButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    // Получаем текущий layout центрального виджета
    QWidget *central = ui->centralwidget;
    QLayout *existingLayout = central->layout();

    // Если layout уже есть — оборачиваем его в QHBoxLayout (если ещё не горизонтальный)
    QHBoxLayout *mainLayout = nullptr;
    if (existingLayout) {
        mainLayout = qobject_cast<QHBoxLayout*>(existingLayout);
        if (!mainLayout) {
            // Переносим содержимое в новый layout
            QWidget *container = new QWidget(this);
            container->setLayout(existingLayout);

            mainLayout = new QHBoxLayout;
            mainLayout->addWidget(container);
            central->setLayout(mainLayout);
        }
    } else {
        // Если layout не задан — создаём новый
        mainLayout = new QHBoxLayout;
        // Предположим, что в .ui plainTextEdit — единственный виджет
        mainLayout->addWidget(ui->plainTextEdit);
        central->setLayout(mainLayout);
    }

    // Добавляем кнопку и вывод СПРАВА от редактора
    mainLayout->addWidget(runButton);
    mainLayout->addWidget(outputTextEdit);

    // Подключаем слот
    connect(runButton, &QPushButton::clicked, this, &MainWindow::on_runButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initLangsEnum()
{
    MainWindow::_langStringToEnum = QHash<QString, QSourceHighliter::Language> {
        { QLatin1String("Asm"), QSourceHighliter::CodeAsm },
        { QLatin1String("Bash"), QSourceHighliter::CodeBash },
        { QLatin1String("C"), QSourceHighliter::CodeC },
        { QLatin1String("C++"), QSourceHighliter::CodeCpp },
        { QLatin1String("CMake"), QSourceHighliter::CodeCMake },
        { QLatin1String("CSharp"), QSourceHighliter::CodeCSharp },
        { QLatin1String("Css"), QSourceHighliter::CodeCSS },
        { QLatin1String("Go"), QSourceHighliter::CodeGo },
        { QLatin1String("Html"), QSourceHighliter::CodeXML },
        { QLatin1String("Ini"), QSourceHighliter::CodeINI },
        { QLatin1String("Java"), QSourceHighliter::CodeJava },
        { QLatin1String("Javascript"), QSourceHighliter::CodeJava },
        { QLatin1String("Json"), QSourceHighliter::CodeJSON },
        { QLatin1String("Lua"), QSourceHighliter::CodeLua },
        { QLatin1String("Make"), QSourceHighliter::CodeMake },
        { QLatin1String("Php"), QSourceHighliter::CodePHP },
        { QLatin1String("Python"), QSourceHighliter::CodePython },
        { QLatin1String("Qml"), QSourceHighliter::CodeQML },
        { QLatin1String("Rhai"), QSourceHighliter::CodeRhai },
        { QLatin1String("Rust"), QSourceHighliter::CodeRust },
        { QLatin1String("Sql"), QSourceHighliter::CodeSQL },
        { QLatin1String("Typescript"), QSourceHighliter::CodeTypeScript },
        { QLatin1String("V"), QSourceHighliter::CodeV },
        { QLatin1String("Vex"), QSourceHighliter::CodeVex },
        { QLatin1String("Xml"), QSourceHighliter::CodeXML },
        { QLatin1String("Yaml"), QSourceHighliter::CodeYAML }
    };
}

void MainWindow::initThemesComboBox()
{
    ui->themeComboBox->addItem("Default", 0);
    ui->themeComboBox->addItem("Monokai", 1);
    ui->themeComboBox->addItem("Dark Theme", 2);
    ui->themeComboBox->addItem("Light Theme", 3);
    loadCustomThemes();

    connect(ui->themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
               this, &MainWindow::themeChanged);
}

void MainWindow::applyEditorBackground(int themeValue)
{
    // Преобразуем int в enum
    QSourceHighliter::Themes theme = static_cast<QSourceHighliter::Themes>(themeValue);

    // Получаем форматы для текущей темы
    auto themeFormats = QSourceHighliterTheme::theme(theme);
    // Берём формат блока – он содержит фон всей строки
    QTextCharFormat blockFormat = themeFormats.value(QSourceHighliter::Token::CodeBlock);

    // Получаем цвет фона. Если его нет – используем белый
    QColor bgColor = blockFormat.background().color();
    if (!bgColor.isValid()) {
        bgColor = Qt::white;
    }

    // Устанавливаем стиль только для фона
    QString styleSheet = QString("QPlainTextEdit { background-color: %1; }").arg(bgColor.name());
    // Применяем стиль ко всему редактору
    ui->plainTextEdit->setStyleSheet(styleSheet);
}

void MainWindow::initLangsComboBox() {
    ui->langComboBox->addItem("Asm");
    ui->langComboBox->addItem("Bash");
    ui->langComboBox->addItem("C");
    ui->langComboBox->addItem("C++");
    ui->langComboBox->addItem("CMake");
    ui->langComboBox->addItem("CSharp");
    ui->langComboBox->addItem("Css");
    ui->langComboBox->addItem("Go");
    ui->langComboBox->addItem("Html");
    ui->langComboBox->addItem("Ini");
    ui->langComboBox->addItem("Javascript");
    ui->langComboBox->addItem("Java");
    ui->langComboBox->addItem("Lua");
    ui->langComboBox->addItem("Make");
    ui->langComboBox->addItem("Php");
    ui->langComboBox->addItem("Python");
    ui->langComboBox->addItem("Qml");
    ui->langComboBox->addItem("Rust");
    ui->langComboBox->addItem("Sql");
    ui->langComboBox->addItem("Typescript");
    ui->langComboBox->addItem("V");
    ui->langComboBox->addItem("Vex");
    ui->langComboBox->addItem("Xml");
    ui->langComboBox->addItem("Yaml");
}

void MainWindow::themeChanged(int index)
{
    int themeValue = ui->themeComboBox->itemData(index).toInt();

    if (themeValue == 0) {
        ui->plainTextEdit->setStyleSheet("");
        if (highlighter) {
            highlighter->setTheme(static_cast<QSourceHighliter::Themes>(1));
        }
    } else if (themeValue >= 100) {
        applyCustomTheme(themeValue);
    } else {
        if (highlighter) {
            highlighter->setTheme(static_cast<QSourceHighliter::Themes>(themeValue));
        }
        applyEditorBackground(themeValue);
    }

    if (highlighter) {
        highlighter->rehighlight();
    }
}

void MainWindow::languageChanged(const QString &lang){
    highlighter->setCurrentLanguage(_langStringToEnum.value(lang));
    highlighter->rehighlight();
}

void MainWindow::on_actionSaveTxt_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Сохранить текстовый файл", "", "Текстовые файлы (*.txt)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui->statusbar->showMessage("Ошибка: не удалось открыть файл для записи", 3000);
        return;
    }

    QTextStream out(&file);
    out << ui->plainTextEdit->toPlainText();
    file.close();

    ui->statusbar->showMessage("Сохранен файл " + fileName, 3000);
    ui->plainTextEdit->document()->setModified(false);
}

void MainWindow::on_actionOpenTxt_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Открыть текстовый файл", "", "Текстовые файлы (*.txt)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->statusbar->showMessage("Ошибка: не удалось открыть файл для чтения", 3000);
        return;
    }

    QTextStream in(&file);
    ui->plainTextEdit->setPlainText(in.readAll());
    file.close();

    ui->statusbar->showMessage("Открыт файл " + fileName, 3000);
    ui->plainTextEdit->document()->setModified(false);
}

void MainWindow::on_actionSaveJson_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить JSON файл", "", "JSON файлы (*.json)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui->statusbar->showMessage("Ошибка: не удалось открыть файл для записи", 3000);
        return;
    }

    QJsonObject root;
   root["language"] = ui->langComboBox->currentText();
   root["text"] = ui->plainTextEdit->toPlainText();
   root["theme"] = ui->themeComboBox->currentData().toInt();

    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();
    ui->statusbar->showMessage("Coхранен файл "+fileName, 3000);
    ui->plainTextEdit->document()->setModified(false);
}

void MainWindow::on_actionOpenJson_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Открыть JSON файл", "", "JSON файлы (*.json)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->statusbar->showMessage("Ошибка: не удалось открыть файл для чтения", 3000);
        return;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();

    if (error.error != QJsonParseError::NoError) {
        ui->statusbar->showMessage("Ошибка парсинга JSON", 3000);
        return;
    }
    QJsonObject obj = doc.object();
    QString lang = obj["language"].toString();
    QString text = obj["text"].toString();
    int theme = obj["theme"].toInt();

    // Установка текста
    ui->plainTextEdit->setPlainText(text);

    // Установка языка
    if (!lang.isEmpty()) {
    ui->langComboBox->setCurrentText(lang);
    }

    // Устанавливаем тему
    if (theme >= 0) {
        int index = ui->themeComboBox->findData(theme);
        if (index >= 0) {
            ui->themeComboBox->setCurrentIndex(index);
        }
    }

    ui->statusbar->showMessage("Открыт JSON файл: " + fileName, 3000);
    ui->plainTextEdit->document()->setModified(false);
}

void MainWindow::on_actionSearch_triggered() {
    SearchDialog dialog(this);
    connect(&dialog, &SearchDialog::findNext, this, &MainWindow::onFindText);
    connect(&dialog, &SearchDialog::replaceText, this, &MainWindow::onReplaceText);
    connect(&dialog, &SearchDialog::replaceAll, this, &MainWindow::onReplaceAll);
    dialog.exec(); // показываем модальное окно
}

void MainWindow::onFindText(const QString &text) {
    QTextCursor cursor = ui->plainTextEdit->textCursor();

    QString content = ui->plainTextEdit->toPlainText();

    int pos = content.indexOf(text, cursor.position(), Qt::CaseSensitive);

    if (pos >= 0)
    {
        cursor.setPosition(pos);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, text.length());
        ui->plainTextEdit->setTextCursor(cursor);
        ui->statusbar->showMessage(tr("Найдено: ") + text, 3000);
    }
    else
    {
        pos = content.indexOf(text, 0, Qt::CaseSensitive);
        if (pos >= 0)
        {
        cursor.setPosition(pos);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, text.length());
        ui->plainTextEdit->setTextCursor(cursor);
        ui->statusbar->showMessage("Начали поиск с начала. Найдено: " + text, 3000);
        }
        else
        {
        QMessageBox::information(this, "Поиск", "Совпадений не найдено");
        ui->statusbar->showMessage(tr("Совпадений не найдено"), 3000);
        }
    }
}
void MainWindow::onReplaceText(const QString &find, const QString &replace) {
    QTextDocument *doc = ui->plainTextEdit->document();

    QTextCursor cursor = ui->plainTextEdit->textCursor();

    QString selected = cursor.selectedText();

    if (!selected.isEmpty() && selected == find)
    {
        cursor.insertText(replace);
        ui->plainTextEdit->setTextCursor(cursor);
        return;
    }

    int pos = cursor.position();
    QTextCursor found = doc->find(find, pos, QTextDocument::FindCaseSensitively);

    if (!found.isNull()) {
        int start = found.selectionStart();
        int end = found.selectionEnd();

        found.insertText(replace);

        QTextCursor newCursor = ui->plainTextEdit->textCursor();

        newCursor.setPosition(start);
        newCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, replace.length());
        ui->plainTextEdit->setTextCursor(newCursor);
    }
    else
    {
        QMessageBox::information(this, "Поиск", "Совпадений не найдено");
    }
    highlighter->rehighlight();
}

void MainWindow::onReplaceAll(const QString &find, const QString &replace) {
    QString content = ui->plainTextEdit->toPlainText();
    QString newContent = content.replace(find, replace, Qt::CaseSensitive);

    ui->plainTextEdit->setPlainText(newContent);
    highlighter->rehighlight();

    QString message = (QString("Все вхождения '%1' заменены на '%2'").arg(find).arg(replace));
    ui->statusbar->showMessage(message, 3000);
}

void MainWindow::createToolBar()
{
    mainToolBar = addToolBar("Основная панель");
    mainToolBar->setIconSize(QSize(24, 24));
    mainToolBar->setMovable(false);


    QAction *openAction = new QAction(style()->standardIcon(QStyle::SP_DialogOpenButton), "Открыть", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::on_actionOpenTxt_triggered);
    mainToolBar->addAction(openAction);

    QAction *saveAction = new QAction(style()->standardIcon(QStyle::SP_DialogSaveButton), "Сохранить", this);
    connect(saveAction, &QAction::triggered, this, [this]() {
        on_actionSaveTxt_triggered();
    });
    mainToolBar->addAction(saveAction);

    mainToolBar->addSeparator();

    QAction *searchAction = new QAction(style()->standardIcon(QStyle::SP_FileDialogContentsView), "Поиск", this);
    connect(searchAction, &QAction::triggered, this, &MainWindow::on_actionSearch_triggered);
    mainToolBar->addAction(searchAction);

    mainToolBar->addSeparator();

    QAction *copyAction = new QAction(style()->standardIcon(QStyle::SP_FileIcon), "Копировать", this);
    connect(copyAction, &QAction::triggered, ui->plainTextEdit, &QPlainTextEdit::copy);
    mainToolBar->addAction(copyAction);

    QAction *pasteAction = new QAction(style()->standardIcon(QStyle::SP_DirLinkIcon), "Вставить", this);
    connect(pasteAction, &QAction::triggered, ui->plainTextEdit, &QPlainTextEdit::paste);
    mainToolBar->addAction(pasteAction);

    QAction *cutAction = new QAction(style()->standardIcon(QStyle::SP_TrashIcon), "Вырезать", this);
    connect(cutAction, &QAction::triggered, ui->plainTextEdit, &QPlainTextEdit::cut);
    mainToolBar->addAction(cutAction);

    mainToolBar->addSeparator();

    QAction *undoAction = new QAction(style()->standardIcon(QStyle::SP_ArrowBack), "Отменить", this);
    connect(undoAction, &QAction::triggered, ui->plainTextEdit, &QPlainTextEdit::undo);
    mainToolBar->addAction(undoAction);

    QAction *redoAction = new QAction(style()->standardIcon(QStyle::SP_ArrowForward), "Повторить", this);
    connect(redoAction, &QAction::triggered, ui->plainTextEdit, &QPlainTextEdit::redo);
    mainToolBar->addAction(redoAction);
}

void MainWindow::on_actionCustomTheme_triggered()
{
    CustomThemeDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString themeName = dialog.getThemeName();
        QHash<QString, QColor> colors = dialog.getThemeColors();

        saveCustomTheme(themeName, colors);

        ui->themeComboBox->addItem(themeName, 100 + ui->themeComboBox->count()); // ID начиная с 100

        ui->statusbar->showMessage("Пользовательская тема '" + themeName + "' создана!", 3000);
    }
}

void MainWindow::saveCustomTheme(const QString &name, const QHash<QString, QColor> &colors)
{
    QJsonObject themeObject;
    themeObject["name"] = name;

    QJsonObject colorsObject;
    for (auto it = colors.begin(); it != colors.end(); ++it) {
        colorsObject[it.key()] = it.value().name();
    }
    themeObject["colors"] = colorsObject;

    QJsonDocument doc(themeObject);

    QString fileName = "themes/" + name + ".json";
    QDir().mkpath("themes");

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}
void MainWindow::loadCustomThemes()
{
    QDir themesDir("themes");
    if (!themesDir.exists()) return;

    QStringList themeFiles = themesDir.entryList(QStringList() << "*.json", QDir::Files);

    for (const QString &file : themeFiles) {
        QFile themeFile("themes/" + file);
        if (themeFile.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(themeFile.readAll());
            QJsonObject themeObject = doc.object();

            QString themeName = themeObject["name"].toString();
            int themeId = 100 + ui->themeComboBox->count();

            ui->themeComboBox->addItem(themeName, themeId);
            themeFile.close();
        }
    }
}

void MainWindow::applyCustomTheme(int themeId)
{
    QString themeName = ui->themeComboBox->itemText(ui->themeComboBox->currentIndex());
    QString fileName = "themes/" + themeName + ".json";

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject themeObject = doc.object();
        QJsonObject colorsObject = themeObject["colors"].toObject();

        // Создаем хэш цветов для передачи в хайлайтер
        QHash<QString, QColor> colors;
        for (auto it = colorsObject.begin(); it != colorsObject.end(); ++it) {
            colors[it.key()] = QColor(it.value().toString());
        }

        // ПРИМЕНЯЕМ ФОН
        if (colors.contains("background")) {
            QColor bgColor = colors["background"];
            QString styleSheet = QString("QPlainTextEdit { background-color: %1; }")
                                .arg(bgColor.name());
            ui->plainTextEdit->setStyleSheet(styleSheet);
        }

        // ПРИМЕНЯЕМ ПОЛНУЮ ПОДСВЕТКУ СИНТАКСИСА
        if (highlighter) {
            highlighter->setCustomTheme(colors);
        }

        file.close();

        ui->statusbar->showMessage("Применена тема: " + themeName, 2000);
    }
}
void MainWindow::on_runButton_clicked()
{
    // Проверяем, что выбран Python
    if (ui->langComboBox->currentText() != "Python") {
        QMessageBox::warning(this, "Предупреждение", "Выполнение поддерживается только для Python.");
        return;
    }

    QString code = ui->plainTextEdit->toPlainText();
    if (code.trimmed().isEmpty()) {
        outputTextEdit->setPlainText("Нет кода для выполнения.");
        return;
    }

    // Создаём временный файл
    QTemporaryFile tempFile(QDir::tempPath() + "/python_script_XXXXXX.py");
    if (!tempFile.open()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать временный файл.");
        return;
    }

    QTextStream out(&tempFile);
    out << code;
    tempFile.close(); // Важно: закрыть файл, чтобы Python мог его прочитать

    // Запускаем процесс
    QProcess process;
    process.start("python", QStringList() << tempFile.fileName());

    if (!process.waitForStarted()) {
        outputTextEdit->setPlainText("Ошибка: Python не установлен или не найден в PATH.");
        return;
    }

    // Ждём завершения (максимум 3 секунды)
    bool finished = process.waitForFinished(3000);

    QString output = process.readAllStandardOutput();
    QString error = process.readAllStandardError();

    // Формируем результат
    QString result;
    if (!output.isEmpty()) {
        result += "Вывод:\n" + output + "\n";
    }
    if (!error.isEmpty()) {
        result += "Ошибки:\n" + error;
    }
    if (result.isEmpty()) {
        if (finished && process.exitStatus() == QProcess::NormalExit) {
            result = "Скрипт выполнен успешно (без вывода).";
        } else {
            result = "Скрипт не завершился вовремя или завершился с ошибкой.";
        }
    }

    outputTextEdit->setPlainText(result);
}
