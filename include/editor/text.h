#pragma once
#include <QPlainTextEdit>
#include <QWidget>

class TextArea;

class LineNumberArea : public QWidget {
public:
    LineNumberArea(TextArea* editor);
    QSize sizeHint() const override;
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
private:
    TextArea* m_textArea;
};

class TextArea : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit TextArea(QWidget* parent = nullptr);
    void setContent(const QString& content);
    QString getContent() const;
    
    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();
    
protected:
    void resizeEvent(QResizeEvent* event) override;
    
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect& rect, int dy);
    
private:
    LineNumberArea* m_lineNumberArea;
};