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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include "qsourcehighliter.h"
#include "qsourcehighliterthemes.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSourceHighlite::QSourceHighliter *highlighter;
    static QHash<QString, QSourceHighlite::QSourceHighliter::Language> _langStringToEnum;
    QPlainTextEdit *outputTextEdit;
    QPushButton *runButton;
    QToolBar *mainToolBar;
    void createToolBar();

    /* FUNCTIONS */
    void initLangsEnum();
    void initLangsComboBox();
    void initThemesComboBox();


private slots:
    void themeChanged(int index);
    void languageChanged(const QString &lang);
    void on_actionSaveJson_triggered();
    void on_actionOpenJson_triggered();
    void on_actionSaveTxt_triggered();
    void on_actionOpenTxt_triggered();
  void on_runButton_clicked();
    void on_actionSearch_triggered();
    void onFindText(const QString &text);
    void onReplaceText(const QString &find, const QString &replace);
    void onReplaceAll(const QString &find, const QString &replace);
    void applyEditorBackground(int themeValue);
    void on_actionCustomTheme_triggered();
    void saveCustomTheme(const QString &name, const QHash<QString, QColor> &colors);
    void loadCustomThemes();
    void applyCustomTheme(int themeId);
};
#endif // MAINWINDOW_H
