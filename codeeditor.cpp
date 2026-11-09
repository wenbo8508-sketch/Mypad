#include "codeeditor.h"

#include <QPainter>
#include <QTextBlock>
#include <QWidget>

namespace {
// 当前行的强调色，和 img/png/24px_blue 这套蓝色图标保持一致
const QColor kAccentColor(26, 115, 232);
}

LineNumberArea::LineNumberArea(CodeEditor *editor)
    : QWidget(editor)
    , m_codeEditor(editor)
{
}

QSize LineNumberArea::sizeHint() const
{
    return QSize(m_codeEditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    m_codeEditor->lineNumberAreaPaintEvent(event);
}

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
    , m_lineNumberArea(new LineNumberArea(this))
{
    connect(this, &CodeEditor::blockCountChanged,
            this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest,
            this, &CodeEditor::updateLineNumberArea);

    // 光标换行时，行号栏里的“当前行”高亮要跟着走
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, [this]() {
        if (m_showLineNumber)
            m_lineNumberArea->update();
    });

    updateLineNumberAreaWidth(0);
}

int CodeEditor::lineNumberAreaWidth()
{
    if (!m_showLineNumber)
        return 0;

    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    // 3 个字符宽度的边距，保证数字不会贴边
    int space = 13 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * qMax(3, digits);
    return space;
}

void CodeEditor::setShowLineNumber(bool show)
{
    if (m_showLineNumber == show)
        return;

    m_showLineNumber = show;
    updateLineNumberAreaWidth(blockCount());
    m_lineNumberArea->update();
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(
        QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    const int gutterWidth = m_lineNumberArea->width();
    const int lineHeight = fontMetrics().height();

    // 背景沿用编辑器调色板（“格式-背景色”改的是 QPalette::Window），稍微压暗一点和正文区分
    QColor background = palette().color(QPalette::Window);
    background = background.darker(105);
    painter.fillRect(event->rect(), background);

    // 右侧一条分隔线
    painter.setPen(QColor(0, 0, 0, 28));
    painter.drawLine(gutterWidth - 1, event->rect().top(),
                     gutterWidth - 1, event->rect().bottom());

    if (!m_showLineNumber)
        return;

    const int currentBlockNumber = textCursor().blockNumber();

    QFont boldFont = font();
    boldFont.setBold(true);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            const bool isCurrent = (blockNumber == currentBlockNumber);
            const QString number = QString::number(blockNumber + 1);

            if (isCurrent) {
                QColor band = kAccentColor;
                band.setAlpha(28);
                painter.fillRect(QRect(0, top, gutterWidth - 1, bottom - top), band);
                painter.setPen(kAccentColor);
                painter.setFont(boldFont);
            } else {
                QColor dim = palette().color(QPalette::WindowText);
                dim.setAlpha(130);
                painter.setPen(dim);
                painter.setFont(font());
            }

            painter.drawText(0, top,
                             gutterWidth - 8,
                             lineHeight,
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
