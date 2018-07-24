/////////////////////////////////////////////////////////////
//  Functions to dynamically load components (NOTE TO IMPORT
//  QTQUICK 2.0 BEFORE USE)
/////////////////////////////////////////////////////////////

var _component;
var _object;
var _parentWindow;

//  todo: maybe a better name
function showDialog(qmlFilePath, parentWindow) {
    _component = Qt.createComponent(qmlFilePath)
    _parentWindow = parentWindow

    if (_component.status === Component.Ready)
        _finishCreation();
    else
        _component.statusChanged.connect(_finishCreation);

    return _component
}

function _finishCreation() {
    if (_component.status === Component.Ready) {
        _object = _component.createObject(_parentWindow);
        if (_object === null) {
            // Error Handling
            console.log("Error creating object, error string:", _component.errorString());
            return;
        }

        //  For Window we have to call `show()` to show the window. However, for Dialog we needn't.
        //  Use a try catch block to hold the potential exceptions.
        try {
            _object.show()
        } catch (error) {
            console.log("Error catched:", error)
        }
    } else if (_component.status === Component.Error) {
        // Error Handling
        console.log("Error loading component:", _component.errorString());
    }
}

//////////////////////////////////////////////////////////////
//  Common BackgroundListView functions
//////////////////////////////////////////////////////////////

function _checkParameterBackgroundListView(backgroundListView) {
    if (backgroundListView === null || backgroundListView.listView === null) {
        throw "Parameter passed in is invalid, object:" + JSON.stringify(backgroundListView);
    }
}

function backgroundListNotEmpty(backgroundListView) {
    _checkParameterBackgroundListView(backgroundListView)

    return backgroundListView.listView.count === 0 ? false : true
}

function backgroundListIsEmpty(backgroundListView) {
    _checkParameterBackgroundListView(backgroundListView)

    return backgroundListView.listView.count === 0;
}

function currentIsFirstInBackgroundList(backgroundListView) {
    _checkParameterBackgroundListView(backgroundListView)

    return backgroundListView.listView.currentIndex === 0;
}

function currentIsLastInBackgroundList(backgroundListView) {
    _checkParameterBackgroundListView(backgroundListView)

    return backgroundListView.listView.currentIndex === backgroundListView.listView.count - 1;
}

function currentIsFirstOrLastInBackgroundList(backgroundListView) {
    _checkParameterBackgroundListView(backgroundListView)

    return isFirstInList(backgroundListView) || isLastInList(backgroundListView);
}

//////////////////////////////////////////////////////////////////////////////
//  Common utils
//////////////////////////////////////////////////////////////////////////////

function linkWordToUrl(word, url)
{
    if (typeof url != "string")
        throw "the url passed in is not a string!"
    if (typeof word != "string")
        throw "the word passed in is not a string!"

    return "<a href=\"" + url + "\">" + word + "</a>";
}

function htmlTab()
{
    return "&nbsp;&nbsp;"
}

function max(lhs, rhs)
{
    return lhs > rhs ? lhs : rhs
}

function rgb(r, g, b)
{
    return Qt.rgba(r / 255.0, g / 255.0, b / 255.0, 255)
}

//  todo: change parameter types to color to make this function convienent
function moreGray(r, g, b) {
    var averageValue = (r + g + b) / 3.0
    var grayValue = (90 / 255.0 + averageValue) > 1.0
                    ? 0.9
                    : 90 / 255.0 + averageValue

    return Qt.rgba(grayValue, grayValue, grayValue)
}
