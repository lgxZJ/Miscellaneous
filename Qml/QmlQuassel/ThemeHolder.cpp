#include "ThemeHolder.h"
#include <QDebug>

///////////////////////////////////////////////////////////////////////////////////
///                             Group Attributes
///////////////////////////////////////////////////////////////////////////////////

void ListView::changeToThemeDark()
{
    this->setBkColor(QColor(45, 45, 48));
    this->setBorderColor(QColor(255, 255, 255));
    this->setSelectedBkColor(QColor(27, 27, 28));
    this->setSelectedBorderColor(QColor(51, 51, 55));
    this->setEnteredBkColor(QColor(62, 62, 64));
    this->setEnteredBorderColor(QColor(45, 45, 48));
}

void ListView::changeToThemeLight()
{
    this->setBkColor(QColor(255, 255, 255));
    this->setBorderColor(QColor(0, 0, 0));
    this->setSelectedBkColor(QColor(203, 232, 246));
    this->setSelectedBorderColor(QColor(38, 160, 218));
    this->setEnteredBkColor(QColor(229, 243, 252));
    this->setEnteredBorderColor(QColor(112, 192, 231));
}

//////////////////////

void ColorBlock::changeToThemeDark()
{
    //  todo:
}

void ColorBlock::changeToThemeLight()
{
    this->setHoveredOuterBorderColor(QColor(0, 120, 215));

    this->setOriginOuterBorderColor(QColor(173, 173, 173));
    this->setOriginOuterColor(QColor(204, 204, 204));
    this->setOriginInerBorderColor(QColor(204, 204, 204));
    this->setOriginInerColor(QColor(0, 0, 255));

    this->setGrayOuterBorderColor(QColor(191, 191, 191));
    this->setGrayOuterColor(QColor(204, 204, 204));
    this->setGrayInnerBorderColor(QColor(204, 204, 204));
}

/////////////////////////////

void TabView::changeToThemeDark()
{
    this->setTabviewBorderColor(QColor(51, 51, 55));
    this->setFrameBorderColor(QColor(25, 25, 27));
    this->setFrameBkColor(QColor(63, 63, 70));

    this->setSelectedGradientBottomColor(QColor(27, 27, 28));
    this->setSelectedGradientTopColor(QColor(27, 27, 28));

    this->setNonSelectedGradientTopColor(QColor(37, 37, 38));
    this->setNonSelectedGradientBottomColor(m_nonSelectedGradientTopColor.lighter());
}

void TabView::changeToThemeLight()
{
    this->setTabviewBorderColor(QColor(170, 170, 170));
    this->setFrameBorderColor(QColor(171, 171, 171));
    this->setFrameBkColor(QColor(255, 255, 255));

    this->setSelectedGradientBottomColor(QColor(255, 255, 255));
    this->setSelectedGradientTopColor(QColor(255, 255, 255));

    this->setNonSelectedGradientTopColor(QColor(240, 240, 240));
    this->setNonSelectedGradientBottomColor(QColor(227, 227, 227));
}

////////////////////////////

void TextEdit::changeToThemeDark()
{

}

void TextEdit::changeToThemeLight()
{
    this->setNormalSelectionColor(QColor(0, 120, 215));
    this->setPressedSelectionColor(QColor(240, 240, 240));
    this->setSelectionTextColor(QColor());
}

/////////////////////////////

void AboutQuassel::changeToThemeDark()
{
    this->setIrcBlockBackgroundColor(QColor(63, 63, 70));
    this->setIrcBlockOuterBorderColor(QColor(51, 51, 55));
    this->setIrcBlockInnerBorderColor(m_ircBlockOuterBorderColor.darker());
}

void AboutQuassel::changeToThemeLight()
{
    this->setIrcBlockBackgroundColor(QColor(240, 240, 240));
    this->setIrcBlockOuterBorderColor(QColor(130, 135, 144));
    this->setIrcBlockInnerBorderColor(QColor(255, 255, 255));
}

///////////////////////////////

void ConfigureQuasselWindow::changeToThemeDark()
{

}

void ConfigureQuasselWindow::changeToThemeLight()
{
    this->setBackgroundColor(QColor(240, 240, 240));
}

///////////////////////////////

void SimpleTreeView::changeToThemeDark()
{

}

void SimpleTreeView::changeToThemeLight()
{
    this->setFocusedItemBorderColor(QColor(153, 209, 255));
    this->setCurrentItemBkColor(QColor(205, 232, 255));
    this->setHoveredItemBkColor(QColor(229, 243, 255));
    this->setNormalItemBkColor(QColor(255, 255, 255));
    this->setNormalHeaderBkColor(QColor(255, 255, 255));
}

///////////////////////////////

void ToolBar::changeToThemeDark()
{
    this->setSecondBottomBorderColor(QColor(0, 0, 0));
}

void ToolBar::changeToThemeLight()
{
    this->setFirstTopBorderColor(QColor(255, 255, 255));
    this->setSecondTopBorderColor(QColor(242, 242, 242));
    this->setFirstBottomBorderColor(QColor(130, 135, 144));
    this->setSecondBottomBorderColor(QColor(185, 185, 185));
}

///////////////////////////////////////////////////////////////////////////////////
///                             ThemeHolder
///////////////////////////////////////////////////////////////////////////////////

void ThemeHolder::changeToTheme(Themes theme)
{
    switch (theme)
    {
    case ThemeDark:      changeToThemeDark();    break;
    case ThemeLight:     changeToThemeLight();   break;

    default:    throw std::logic_error("unexpected theme:" + std::to_string(theme));
    }
}

void ThemeHolder::changeToThemeDark()
{
    this->setOriginTextColor(QColor(241, 241, 241));
    this->setGrayedTextColor(QColor(183, 185, 197));

    this->setBackgroundColor(QColor(37, 37, 38));
    this->setStatusBarBkColor(QColor(45, 45, 48));
    this->setButtonBlockColor(QColor(37, 37, 38));
    this->setCustomedTitleBkColor(QColor(37, 37, 38));
    this->setCustomedTitleBorderColor(QColor(67, 67, 70));

    this->setCommonWindowBkColor(QColor(45, 45, 48));
    this->setCommonTextBkColor(QColor(241, 241, 241));
    this->setCommonBorderColor(QColor(51, 51, 55));

    m_listView.changeToThemeDark();
    m_colorBlock.changeToThemeDark();
    m_tabView.changeToThemeDark();
    m_textEdit.changeToThemeDark();
    m_aboutQuassel.changeToThemeDark();
    m_configureQuasselWindow.changeToThemeDark();
    m_simpleTreeView.changeToThemeDark();
    m_toolBar.changeToThemeDark();
}

void ThemeHolder::changeToThemeLight()
{
    this->setOriginTextColor(QColor(0, 0, 0));
    this->setGrayedTextColor(QColor(120, 120, 120));

    this->setBackgroundColor(QColor(255, 255, 255));
    this->setStatusBarBkColor(QColor(240, 240, 240));
    this->setButtonBlockColor(QColor(240, 240, 240));
    this->setCustomedTitleBkColor(QColor(218, 218, 218));
    this->setCustomedTitleBorderColor(QColor(185, 185, 185));

    this->setCommonWindowBkColor(QColor(240, 240, 240));
    this->setCommonTextBkColor(QColor(0, 0, 0));
    this->setCommonBorderColor(QColor(173, 173, 173));

    m_listView.changeToThemeLight();
    m_colorBlock.changeToThemeLight();
    m_tabView.changeToThemeLight();
    m_textEdit.changeToThemeLight();
    m_aboutQuassel.changeToThemeLight();
    m_configureQuasselWindow.changeToThemeLight();
    m_simpleTreeView.changeToThemeLight();
    m_toolBar.changeToThemeLight();
}
