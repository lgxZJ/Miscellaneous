//#ifndef THEMEHOLDER_H
//#define THEMEHOLDER_H

#include <QObject>
#include <QColor>

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Macros

#define DEFINE_PROPERTY(getterName, setterName, notifierName)                                           \
    public:                                                                                             \
        Q_PROPERTY(QColor getterName READ getterName WRITE setterName NOTIFY notifierName)              \
                                                                                                        \
        QColor getterName() const { return m_##getterName; }                                            \
        void   setterName(const QColor& color) { m_##getterName = color; Q_EMIT notifierName();}        \
                                                                                                        \
        Q_SIGNAL void notifierName();                                                                   \
                                                                                                        \
    private:                                                                                            \
        QColor m_##getterName;


#define DEFINE_GROUP_ATTRIBUTES(groupName, groupType)                   \
    public:                                                             \
        Q_PROPERTY(groupType* groupName READ groupName CONSTANT)        \
        groupType* groupName() { return &m_##groupName; }               \
                                                                        \
    private:                                                            \
        groupType m_##groupName;



/////////////////////////////////////////////////////////////////////////////////////
//                              Group Attributes
/////////////////////////////////////////////////////////////////////////////////////

class ListView : public QObject
{
    Q_OBJECT
public:
    DEFINE_PROPERTY(bkColor,            setBkColor,             borderBkChanged)
    DEFINE_PROPERTY(borderColor,        setBorderColor,         borderColorChanged)
    DEFINE_PROPERTY(selectedBkColor,    setSelectedBkColor,     selectedBkColorChanged)
    DEFINE_PROPERTY(selectedBorderColor,setSelectedBorderColor, selectedBorderColorChanged)
    DEFINE_PROPERTY(enteredBorderColor, setEnteredBorderColor,  enteredBorderColorChanged)
    DEFINE_PROPERTY(enteredBkColor,     setEnteredBkColor,      enteredBkColorChanged)

public:
    void changeToThemeDark();
    void changeToThemeLight();
};

class ColorBlock : public QObject
{
    Q_OBJECT
public:
    DEFINE_PROPERTY(hoveredOuterBorderColor,    setHoveredOuterBorderColor,     hoveredOuterBorderColorChanged)
    DEFINE_PROPERTY(originOuterBorderColor,     setOriginOuterBorderColor,      originOuterBorderColorChanged)
    DEFINE_PROPERTY(originOuterColor,           setOriginOuterColor,            originOuterColorChanged)
    DEFINE_PROPERTY(originInerBorderColor,      setOriginInerBorderColor,       originInerBorderColorChanged)
    DEFINE_PROPERTY(originInerColor,            setOriginInerColor,             originInerColorChanged)
    DEFINE_PROPERTY(grayOuterBorderColor,       setGrayOuterBorderColor,        grayOuterBorderColorChanged)
    DEFINE_PROPERTY(grayOuterColor,             setGrayOuterColor,              grayOuterColorChanged)
    DEFINE_PROPERTY(grayInnerBorderColor,       setGrayInnerBorderColor,        grayInnerBorderColorChanged)

public:
    void changeToThemeDark();
    void changeToThemeLight();
};

class TabView : public QObject
{
    Q_OBJECT
public:
    DEFINE_PROPERTY(tabviewBorderColor,             setTabviewBorderColor,              tabviewBorderColorChanged)
    DEFINE_PROPERTY(selectedGradientTopColor,       setSelectedGradientTopColor,        selectedGradientTopColorChanged)
    DEFINE_PROPERTY(selectedGradientBottomColor,    setSelectedGradientBottomColor,     selectedGradientBottomColorChanged)
    DEFINE_PROPERTY(nonSelectedGradientTopColor,    setNonSelectedGradientTopColor,     nonSelectedGradientTopColorChanged)
    DEFINE_PROPERTY(nonSelectedGradientBottomColor, setNonSelectedGradientBottomColor,  nonSelectedGradientBottomColorChanged)
    DEFINE_PROPERTY(frameBorderColor,               setFrameBorderColor,                frameBorderColorChanged)
    DEFINE_PROPERTY(frameBkColor,                   setFrameBkColor,                    frameBkColorChanged)

public:
    void changeToThemeDark();
    void changeToThemeLight();
};


class TextEdit : public QObject
{
    Q_OBJECT
public:
    DEFINE_PROPERTY(normalSelectionColor,   setNormalSelectionColor,    normalSelectionColorChanged)
    DEFINE_PROPERTY(pressedSelectionColor,  setPressedSelectionColor,   pressedSelectionColorChanged)
    DEFINE_PROPERTY(selectionTextColor,     setSelectionTextColor,      selectionTextColorChanged)

public:
    void changeToThemeDark();
    void changeToThemeLight();
};

///////////////////

class AboutQuassel : public QObject
{
    Q_OBJECT
public:
    DEFINE_PROPERTY(ircBlockOuterBorderColor,   setIrcBlockOuterBorderColor,    ircBlockOuterBorderColorChanged)
    DEFINE_PROPERTY(ircBlockInnerBorderColor,   setIrcBlockInnerBorderColor,    ircBlockInnerBorderColorChanged)
    DEFINE_PROPERTY(ircBlockBackgroundColor,    setIrcBlockBackgroundColor,     ircBlockBackgroundColorChanged)

public:
    void changeToThemeDark();
    void changeToThemeLight();
};

///////////////////

class ConfigureQuasselWindow : public QObject
{
    Q_OBJECT
public:
    DEFINE_PROPERTY(backgroundColor,    setBackgroundColor,     backgroundColorChanged)

public:
    void changeToThemeDark();
    void changeToThemeLight();
};

////////////////////

class SimpleTreeView : public QObject
{
    Q_OBJECT
public:
    DEFINE_PROPERTY(focusedItemBorderColor, setFocusedItemBorderColor,  focusedItemBorderColorChanged)
    DEFINE_PROPERTY(currentItemBkColor,     setCurrentItemBkColor,      currentItemBkColorChanged)
    DEFINE_PROPERTY(hoveredItemBkColor,     setHoveredItemBkColor,      hoveredItemBkColorChanged)
    DEFINE_PROPERTY(normalItemBkColor,      setNormalItemBkColor,       normalItemBkColorChanged)
    DEFINE_PROPERTY(normalHeaderBkColor,    setNormalHeaderBkColor,     normalHeaderBkColorChanged)

public:
    void changeToThemeDark();
    void changeToThemeLight();
};

////////////////////

class ToolBar : public QObject
{
    Q_OBJECT
public:
    DEFINE_PROPERTY(firstTopBorderColor, setFirstTopBorderColor,  firstTopBorderColorChanged)
    DEFINE_PROPERTY(secondTopBorderColor,setSecondTopBorderColor, secondTopBorderColorChanged)
    DEFINE_PROPERTY(firstBottomBorderColor, setFirstBottomBorderColor,  firstBottomBorderColorChanged)
    DEFINE_PROPERTY(secondBottomBorderColor,setSecondBottomBorderColor, secondBottomBorderColorChanged)

public:
    void changeToThemeDark();
    void changeToThemeLight();
};

///////////////////////////////////////////////////////////////////////////////////
///                             ThemeHolder
///////////////////////////////////////////////////////////////////////////////////


class ThemeHolder : public QObject
{
    Q_OBJECT
public:
    ThemeHolder(QObject* parent = nullptr) : QObject(parent) {}

    enum Themes { ThemeDark, ThemeLight };
    Q_ENUM(Themes)

    Q_INVOKABLE void changeToTheme(Themes theme);

    DEFINE_PROPERTY(originTextColor,            setOriginTextColor,         originTextColorChanged)
    DEFINE_PROPERTY(grayedTextColor,            setGrayedTextColor,         grayedTextColorChanged)
    DEFINE_PROPERTY(backgroundColor,            setBackgroundColor,         backgroundColorChanged)
    DEFINE_PROPERTY(buttonBlockColor,           setButtonBlockColor,        buttonBlockColorChanged)
    DEFINE_PROPERTY(statusBarBkColor,           setStatusBarBkColor,        statusBarBkColorChanged)
    DEFINE_PROPERTY(customedTitleBkColor,       setCustomedTitleBkColor,    customedTitleBkColorChanged)
    DEFINE_PROPERTY(customedTitleBorderColor,   setCustomedTitleBorderColor,customedTitleBorderColorChanged)

    ///////////////////////////////////////////////
    //          COMMON COLORS
    ///////////////////////////////////////////////
    DEFINE_PROPERTY(commonWindowBkColor,    setCommonWindowBkColor,     commonWindowColorChanged)
    DEFINE_PROPERTY(commonTextBkColor,      setCommonTextBkColor,       commonTextColorChanged)
    DEFINE_PROPERTY(commonBorderColor,      setCommonBorderColor,       commonBorderColorChanged)


    DEFINE_GROUP_ATTRIBUTES(listView,               ListView)
    DEFINE_GROUP_ATTRIBUTES(colorBlock,             ColorBlock)
    DEFINE_GROUP_ATTRIBUTES(tabView,                TabView)
    DEFINE_GROUP_ATTRIBUTES(textEdit,               TextEdit)
    DEFINE_GROUP_ATTRIBUTES(aboutQuassel,           AboutQuassel)
    DEFINE_GROUP_ATTRIBUTES(configureQuasselWindow, ConfigureQuasselWindow)
    DEFINE_GROUP_ATTRIBUTES(simpleTreeView,         SimpleTreeView)
    DEFINE_GROUP_ATTRIBUTES(toolBar,                ToolBar)


private:
    void changeToThemeDark();
    void changeToThemeLight();
};

#undef DEFINE_PROPERTY

//#endif // THEMEHOLDER_H
