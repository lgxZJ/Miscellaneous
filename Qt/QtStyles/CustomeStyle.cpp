#include "CustomeStyle.h"

#include <QStyleOptionComboBox>
#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QDebug>

bool CustomeStyle::m_arrowAreaHovered = false;

void CustomeStyle::drawComplexControl(QStyle::ComplexControl control,
                                      const QStyleOptionComplex *option,
                                      QPainter *painter,
                                      const QWidget *widget) const
{
    switch (control) {
    case CC_ComboBox:
        {
            drawArrowArea(option, painter, widget);
            drawControl(CE_ComboBoxLabel, option, painter, widget);
            break;
        }
    default:
        QProxyStyle::drawComplexControl(control, option, painter, widget);
    }
}

void CustomeStyle::drawArrowArea(const QStyleOptionComplex *option,
                                 QPainter *painter,
                                 const QWidget *widget) const
{
    QRect arrowBoxRect = option->rect;
    arrowBoxRect.adjust(option->rect.width() * 0.8, 0, 0, 0);

    auto arrowAreaColor = Qt::darkCyan;
    m_arrowAreaHovered = arrowBoxRect.contains(widget->mapFromGlobal(QCursor::pos()));
    if (option->state & State_MouseOver && m_arrowAreaHovered)
        arrowAreaColor = Qt::cyan;
    else if (option->state & State_On && m_arrowAreaHovered)
        arrowAreaColor = Qt::darkMagenta;

    QRadialGradient gradient(arrowBoxRect.center(),
                             arrowBoxRect.width());
    gradient.setColorAt(0.0, Qt::gray);
    gradient.setColorAt(1.0, arrowAreaColor);
    painter->fillRect(arrowBoxRect, QBrush(gradient));


    auto arrowDownOption = *option;
    auto adjustPixel = arrowBoxRect.width() * 0.2;
    arrowDownOption.rect = arrowBoxRect.adjusted(adjustPixel,
                                                 adjustPixel,
                                                 -adjustPixel,
                                                 -adjustPixel);
    drawPrimitive(PE_IndicatorArrowDown, &arrowDownOption, painter, widget);
}

void CustomeStyle::drawControl(QStyle::ControlElement element,
                               const QStyleOption *option,
                               QPainter *painter,
                               const QWidget *widget) const
{
    switch (element) {
    case CE_ComboBoxLabel:
        {
            auto comboBoxOption = qstyleoption_cast<const QStyleOptionComboBox*>(option);
            if (comboBoxOption == nullptr)
                return;

            QColor gradientColors[] = {
                Qt::yellow,
                Qt::green,
                Qt::blue,
                Qt::red
            };
            QColor penColor = Qt::white;
            if (option->state & State_MouseOver && !m_arrowAreaHovered) {
                for (auto& color : gradientColors)
                    color.setAlpha(80);
                penColor.setAlpha(80);
            } else if (option->state & State_On && !m_arrowAreaHovered) {
                for (auto& color : gradientColors)
                    color = color.darker(300);
                penColor = penColor.darker(300);
            }

            QRect labelRect = comboBoxOption->rect;
            labelRect.adjust(0, 0, -(labelRect.width() * 0.2), 0);

            QLinearGradient linearGradient(labelRect.topLeft(), labelRect.bottomRight());
            for (int i = 0; i < 4; ++i) {
                linearGradient.setColorAt(0.25 *i, gradientColors[i]);
            }

            painter->fillRect(labelRect, QBrush(linearGradient));

            painter->setPen(QPen(penColor));
            painter->drawText(labelRect, comboBoxOption->currentText, QTextOption(Qt::AlignCenter));
            break;
        }
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}

void CustomeStyle::drawPrimitive(QStyle::PrimitiveElement element,
                                 const QStyleOption *option,
                                 QPainter *painter,
                                 const QWidget *widget) const
{
    switch (element) {
    default:
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}

QSize CustomeStyle::sizeFromContents(QStyle::ContentsType type,
                                     const QStyleOption *option,
                                     const QSize &size,
                                     const QWidget *widget) const
{
    switch (type) {
    default:
        return QProxyStyle::sizeFromContents(type, option, size, widget);
    }
}

QRect CustomeStyle::subControlRect(QStyle::ComplexControl cc,
                                   const QStyleOptionComplex *opt,
                                   QStyle::SubControl sc,
                                   const QWidget *widget) const
{
    switch (cc) {
    ////////////////////////////////////////////////////////////
    //  sub controls used by ComboBox

    case SC_ComboBoxFrame:
        {
            //  An QEditLine is created according to this return value.
            //  In this Qt version, `opt` is an entire widget rectangle.
            QRect labelRect = opt->rect;
            return labelRect.adjusted(1, 1, -(labelRect.width() * 0.2)-1, -1);
        }

    //  this value is not used by QStyle!
    case SC_ComboBoxListBoxPopup:   return QRect(0, 0, 0, 0);
    case SC_ComboBoxEditField:      return QRect(0, 0, 0, 0);
    case SC_ComboBoxArrow:          return QRect(0, 0, 0, 0);

    default:
        return QProxyStyle::subControlRect(cc, opt, sc, widget);
    }
}

QRect CustomeStyle::subElementRect(QStyle::SubElement element,
                                   const QStyleOption *option,
                                   const QWidget *widget) const
{
    return QProxyStyle::subElementRect(element, option, widget);
}
