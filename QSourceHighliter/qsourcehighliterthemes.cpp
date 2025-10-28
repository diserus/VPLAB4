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

#include "qsourcehighliterthemes.h"

namespace QSourceHighlite {

static QHash<QSourceHighliter::Token, QTextCharFormat> formats()
{
    QHash<QSourceHighliter::Token, QTextCharFormat> _formats;

    QTextCharFormat defaultFormat = QTextCharFormat();

    _formats[QSourceHighliter::Token::CodeBlock] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeKeyWord] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeString] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeComment] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeType] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeOther] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeNumLiteral] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeBuiltIn] = defaultFormat;

    return _formats;
}

static QHash<QSourceHighliter::Token, QTextCharFormat> monokai()
{
    QHash<QSourceHighliter::Token, QTextCharFormat> _formats = formats();

    _formats[QSourceHighliter::Token::CodeBlock].setForeground(QColor(227, 226, 214));
    _formats[QSourceHighliter::Token::CodeKeyWord].setForeground(QColor(249, 38, 114));
    _formats[QSourceHighliter::Token::CodeString].setForeground(QColor(230, 219, 116));
    _formats[QSourceHighliter::Token::CodeComment].setForeground(QColor(117, 113, 94));
    _formats[QSourceHighliter::Token::CodeType].setForeground(QColor(102, 217, 239));
    _formats[QSourceHighliter::Token::CodeOther].setForeground(QColor(249, 38, 114));
    _formats[QSourceHighliter::Token::CodeNumLiteral].setForeground(QColor(174, 129, 255));
    _formats[QSourceHighliter::Token::CodeBuiltIn].setForeground(QColor(166, 226, 46));

    return _formats;
}

static QHash<QSourceHighliter::Token, QTextCharFormat> darkTheme()
{
    QHash<QSourceHighliter::Token, QTextCharFormat> _formats = formats();

    // Темная тема - глубокие цвета на темном фоне
    _formats[QSourceHighliter::Token::CodeBlock].setForeground(QColor(220, 220, 220));
    _formats[QSourceHighliter::Token::CodeBlock].setBackground(QColor(30, 30, 30));
    _formats[QSourceHighliter::Token::CodeKeyWord].setForeground(QColor(86, 156, 214));
    _formats[QSourceHighliter::Token::CodeKeyWord].setFontWeight(QFont::Bold);
    _formats[QSourceHighliter::Token::CodeString].setForeground(QColor(214, 157, 133));
    _formats[QSourceHighliter::Token::CodeComment].setForeground(QColor(87, 166, 74));
    _formats[QSourceHighliter::Token::CodeComment].setFontItalic(true);
    _formats[QSourceHighliter::Token::CodeType].setForeground(QColor(78, 201, 176));
    _formats[QSourceHighliter::Token::CodeOther].setForeground(QColor(197, 134, 192));
    _formats[QSourceHighliter::Token::CodeNumLiteral].setForeground(QColor(181, 206, 168));
    _formats[QSourceHighliter::Token::CodeBuiltIn].setForeground(QColor(220, 220, 170));

    return _formats;
}

static QHash<QSourceHighliter::Token, QTextCharFormat> lightTheme()
{
    QHash<QSourceHighliter::Token, QTextCharFormat> _formats = formats();

    // Светлая тема - яркие цвета на светлом фоне
    _formats[QSourceHighliter::Token::CodeBlock].setForeground(QColor(30, 30, 30));
    _formats[QSourceHighliter::Token::CodeBlock].setBackground(QColor(255, 255, 255));
    _formats[QSourceHighliter::Token::CodeKeyWord].setForeground(QColor(136, 18, 128));
    _formats[QSourceHighliter::Token::CodeKeyWord].setFontWeight(QFont::Bold);
    _formats[QSourceHighliter::Token::CodeString].setForeground(QColor(163, 21, 21));
    _formats[QSourceHighliter::Token::CodeComment].setForeground(QColor(0, 128, 0));
    _formats[QSourceHighliter::Token::CodeComment].setFontItalic(true);
    _formats[QSourceHighliter::Token::CodeType].setForeground(QColor(36, 138, 145));
    _formats[QSourceHighliter::Token::CodeOther].setForeground(QColor(200, 40, 41));
    _formats[QSourceHighliter::Token::CodeNumLiteral].setForeground(QColor(28, 0, 207));
    _formats[QSourceHighliter::Token::CodeBuiltIn].setForeground(QColor(58, 62, 180));

    return _formats;
}

QHash<QSourceHighliter::Token, QTextCharFormat> QSourceHighliterTheme::theme(QSourceHighliter::Themes theme)
{
    switch (theme) {
        case QSourceHighliter::Themes::Monokai:
            return monokai();
        case QSourceHighliter::Themes::DarkTheme:
            return darkTheme();
        case QSourceHighliter::Themes::LightTheme:
            return lightTheme();
        default:
            return monokai();
    }
}
}
