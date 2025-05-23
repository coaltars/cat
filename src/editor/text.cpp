#include "editor/text.h"
#include "core/theme.h"
#include <QTextDocument>
#include <QPainter>
#include <QTextBlock>

LineNumberArea::LineNumberArea(TextArea* editor) : QWidget(editor), m_textArea(editor) {}

QSize LineNumberArea::sizeHint() const {
    return QSize(m_textArea->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent* event) {
    m_textArea->lineNumberAreaPaintEvent(event);
}

TextArea::TextArea(QWidget* parent) : QPlainTextEdit(parent) {
    m_lineNumberArea = new LineNumberArea(this);
    
    auto& theme = Theme::instance();
    setFont(theme.font);
    
    QString style = QString(
        "QPlainTextEdit {"
        "background-color: %1;"
        "color: %2;"
        "border: none;"
        "selection-background-color: %3;"
        "}"
    ).arg(theme.bg.name())
     .arg(theme.fg.name())
     .arg(theme.accent.name());
    
    setStyleSheet(style);
    setTabStopDistance(40);
    setLineWrapMode(QPlainTextEdit::NoWrap);
    
    connect(this, &QPlainTextEdit::blockCountChanged, this, &TextArea::updateLineNumberAreaWidth);
    connect(this, &QPlainTextEdit::updateRequest, this, &TextArea::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &TextArea::highlightCurrentLine);
    
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void TextArea::setContent(const QString& content) {
    setPlainText(content);
}

QString TextArea::getContent() const {
    return toPlainText();
}

int TextArea::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void TextArea::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void TextArea::updateLineNumberArea(const QRect& rect, int dy) {
    if (dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void TextArea::resizeEvent(QResizeEvent* e) {
    QPlainTextEdit::resizeEvent(e);
    
    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void TextArea::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;
    
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        
        auto& theme = Theme::instance();
        QColor lineColor = theme.border;
        lineColor.setAlpha(50);
        
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    
    setExtraSelections(extraSelections);
}

void TextArea::lineNumberAreaPaintEvent(QPaintEvent* event) {
    QPainter painter(m_lineNumberArea);
    
    auto& theme = Theme::instance();
    painter.fillRect(event->rect(), theme.tree_bg);
    painter.setPen(theme.fg);
    
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.drawText(0, top, m_lineNumberArea->width() - 3, fontMetrics().height(),
                           Qt::AlignRight, number);
        }
        
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}