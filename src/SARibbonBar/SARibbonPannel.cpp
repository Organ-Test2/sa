#include "SARibbonPannel.h"
#include "SARibbonToolButton.h"
#include <QAction>
#include <QIcon>
#include <QDebug>
#include <QGridLayout>
#include <QFontMetrics>
#include <QPainter>
#include "SARibbonPannelOptionButton.h"
#include "SARibbonSeparatorWidget.h"
SARibbonPannel::SARibbonPannel(QWidget *parent):QWidget(parent)
  ,m_nextElementPosition(3,3)
  ,m_row(0)
  ,m_optionActionButton(nullptr)
  ,m_titleOptionButtonSpace(6)
  ,m_titleHeight(21)
  ,m_titleY(77)
{
    setFixedHeight(98);
    setMinimumWidth(50);
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setSpacing(0);
    m_gridLayout->setContentsMargins(3,2,3,21);
}

SARibbonToolButton *SARibbonPannel::addLargeAction(QAction *action)
{
    SARibbonToolButton* btn = new SARibbonToolButton(this);
    btn->setButtonType(SARibbonToolButton::LargeButton);
    btn->setAutoRaise(true);
    btn->setDefaultAction(action);
    QSize iconSize = maxHightIconSize(action->icon().actualSize(QSize(32,32)),32);
    btn->setIconSize(iconSize);
    if(action->menu())
        btn->setPopupMode(QToolButton::MenuButtonPopup);
    m_gridLayout->addWidget(btn,0,m_gridLayout->columnCount(),6,1);
    m_row = 0;
    addAction(action);
    return btn;
}

SARibbonToolButton* SARibbonPannel::addSmallAction(QAction *action)
{
    SARibbonToolButton* btn = new SARibbonToolButton(this);
    btn->setButtonType(SARibbonToolButton::SmallButton);
    btn->setAutoRaise(true);
    btn->setDefaultAction(action);
    QSize iconSize = maxHightIconSize(action->icon().actualSize(QSize(16,16)),16);
    btn->setIconSize(iconSize);
    if(action->menu())
        btn->setPopupMode(QToolButton::MenuButtonPopup);
    if(0 == m_row)
        m_gridLayout->addWidget(btn,m_row,m_gridLayout->columnCount(),2,1);
    else
        m_gridLayout->addWidget(btn,m_row,m_gridLayout->columnCount()-1,2,1);
    m_row += 2;
    if(m_row >= 6)
        m_row = 0;
    addAction(action);
    return btn;
}

void SARibbonPannel::addSeparator()
{
#if 0
    QAction*action = new QAction(this);
    action->setSeparator(true);
    addAction(action);
#else
    SARibbonSeparatorWidget* sep = new SARibbonSeparatorWidget(height() - 10,this);
    m_gridLayout->addWidget(sep,0,m_gridLayout->columnCount(),6,1);
#endif
}

void SARibbonPannel::addWidget(QWidget *w,int row)
{
    if(row<0)
    {
        m_gridLayout->addWidget(w,0,m_gridLayout->columnCount(),6,1);
    }
    else
    {
        if(row > 4)
        {
            row = 4;
        }
        m_gridLayout->addWidget(w,row,m_gridLayout->columnCount(),2,1);
    }
}

void SARibbonPannel::addOptionAction(QAction *action)
{
    if(nullptr == action)
    {
        if(m_optionActionButton)
        {
            delete m_optionActionButton;
            m_optionActionButton = nullptr;
        }
        return;
    }
    if(nullptr == m_optionActionButton)
    {
        m_optionActionButton = new SARibbonPannelOptionButton(this);
    }
    m_optionActionButton->setDefaultAction(action);
    repaint();
}


QSize SARibbonPannel::maxHightIconSize(const QSize &size, int height)
{
    if(size.height() < height)
    {
        return  (size * ((float)height/size.height()));
    }
    return size;
}

void SARibbonPannel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QFont f = font();
    f.setPixelSize(11);
    p.setFont(f);
    if(m_optionActionButton)
    {
        p.drawText(0,m_titleY
                   ,width()-m_optionActionButton->width() - m_titleOptionButtonSpace
                   ,m_titleHeight,Qt::AlignCenter,windowTitle());
    }
    else
    {
        p.drawText(0,m_titleY,width(),m_titleHeight,Qt::AlignCenter,windowTitle());
    }

}

QSize SARibbonPannel::sizeHint() const
{
    QSize laySize = layout()->sizeHint();
    QFontMetrics fm = fontMetrics();
    QSize titleSize = fm.size(Qt::TextShowMnemonic,windowTitle());
    int maxWidth = qMax(laySize.width(),titleSize.width());
    return QSize(maxWidth,laySize.height());
}

void SARibbonPannel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if(m_optionActionButton)
    {
        m_optionActionButton->move(width()-m_titleOptionButtonSpace/2 - m_optionActionButton->width()
                                   ,m_titleY+(m_titleHeight-m_optionActionButton->height())/2);
        qDebug() << "m_optionActionButton geometry" <<m_optionActionButton->geometry();
    }
}