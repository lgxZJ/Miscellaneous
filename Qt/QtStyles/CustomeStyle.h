#ifndef CUSTOMESTYLE_H
#define CUSTOMESTYLE_H

#include <QProxyStyle>

class CustomeStyle : public QProxyStyle
{
public:
    virtual void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    virtual void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;

    //  not used in our demo
    virtual QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const;
    virtual QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget) const;
    virtual QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const;

private:
    void drawArrowArea(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;

private:
    static bool m_arrowAreaHovered;
};

#endif // CUSTOMESTYLE_H
