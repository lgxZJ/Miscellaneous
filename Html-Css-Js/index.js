function onContentIframeLoad() {
    var contentIframe = document.getElementById('content-iframe');
    if (contentIframe === null) {
        console.log('elemet `float-personal-block` not found!');
        throw 'elemet `float-personal-block` not found!';
    }

    // make the iframe document fit to its content size
    const contentWidth = contentIframe.contentWindow.document.body.scrollWidth;
    const contentHeight = contentIframe.contentWindow.document.body.scrollHeight;

    contentIframe.style.width = contentWidth;
    contentIframe.style.height = contentHeight;

    //	post to iframe to make it leave personal block at right.
    var realWidth = document.body.clientWidth / 5.0 * 4;
    contentIframe.contentWindow.postMessage('action.leave-personal-block=' + realWidth, 'http://47.98.116.187');
}

window.addEventListener('message', function(event) {
    if (event.data === 'reply.leave-personal-block') {
	var contentIframe = document.getElementById('content-iframe');
	if (contentIframe === null) {
            console.log('elemet `float-personal-block` not found!');
            throw 'elemet `float-personal-block` not found!';
	}

	contentIframe.style.visibility = 'visible';
    }

    this.console.log('msg received in parent window', event.origin, event.data, event.source)
}, false);

window.addEventListener('resize', function() {
    console.log('resize event in parent');

    // location.reload() will cause a nginx load balane, we refresh manully here
    layoutLoader();
    window.location.reload();
    //onContentIframeLoad();
}, false);

/////////////////////////////////////////////////////////////////////////////
//      Global codes to be executed by documents which reference this js file.
/////////////////////////////////////////////////////////////////////////////
function layoutLoader(event) {
    console.log('DOMContentLoaded');

    //first hidden
    var contentIframe = document.getElementById('content-iframe');
    if (contentIframe === null) {
        console.log('elemet `float-personal-block` not found!');
        throw 'elemet `float-personal-block` not found!';
    }
    contentIframe.style.visibility = 'hidden';
    contentIframe.src = contentIframe.src;
    ////

    var documentHeight = document.body.clientHeight;
    var documentWidth = document.body.clientWidth;

    var floatPersonalBlock = document.getElementById('float-personal-block');
    if (floatPersonalBlock == null) {
        console.log('elemet `float-personal-block` not found!');
        return
    }

    const personalBlockWidth = documentWidth / 5.0;
    const personalBlockHeight = documentHeight;
    floatPersonalBlock.style.width = personalBlockWidth
    floatPersonalBlock.style.height = personalBlockHeight
    
    const weightSvgWidth = parseFloat(floatPersonalBlock.style.width);
    const weightSvgHeight = weightSvgWidth / 2;
    //todo:
    
    //////////////
    function getCellSpace(totalWidth, cellCountX) {
        const cellWidth = totalWidth / (cellCountX + (cellCountX + 1) / 7.0);
        const cellSpace = cellWidth / 7.0;

        return cellSpace;
    }

    const fwdSectionHeight = (personalBlockHeight - weightSvgHeight) / (3 + 6 * 0.5 + 2 + 0.5);
    const weightWrapperWidth = weightSvgWidth;
    const weightWrapperHeight = fwdSectionHeight / 2;

    var weightWrapper = document.getElementById('weight-wrapper');
    if (weightWrapper === null) {
        console.log('elemet `weight-wrapper` not found!');
        return
    }
    weightWrapper.style.width = weightWrapperWidth;
    weightWrapper.style.height = weightWrapperHeight;

    var weightInput = document.getElementById('weight-input');
    if (weightInput === null) {
        console.log('elemet `weight-input` not found!');
        return
    }
    weightInput.style.width = weightWrapperWidth - weightWrapperHeight - getCellSpace(weightWrapperWidth, 10);
    weightInput.style.height = weightWrapperHeight;
    weightInput.style.fontSize = (weightWrapperHeight / 2) + 'px';

    var weightButton = document.getElementById('weight-button');
    if (weightButton === null) {
        console.log('elemet `weight-button` not found!');
        return
    }
    weightButton.style.width = weightWrapperHeight - getCellSpace(weightWrapperWidth, 10);
    weightButton.style.height = weightWrapperHeight;
    //  values must be with units
    weightButton.style.backgroundSize = weightWrapperHeight + 'px ' + weightWrapperHeight + 'px';

    //////////////////
    const sectionHeight = (personalBlockHeight - weightSvgHeight) / (3 + 6 * 0.5 + 2 + 0.5);
    const sectionWidth = personalBlockWidth;

    var sections = document.getElementsByTagName('section');
    for (var i = 0; i < sections.length; ++i)  {
        sections[i].style.width = sectionWidth;
        sections[i].style.height = sectionHeight;

        sections[i].style.lineHeight = sectionHeight + 'px';
        sections[i].style.fontSize = sectionHeight / 2;
    }

    //////////////
    const portraitHeight = 2 * sectionHeight;
    const portraitWidth = personalBlockWidth;

    var beneth = document.getElementById('float-personal-block-portrait-beneth');
    if (beneth === null) {
        console.log('elemet `float-personal-block-portrait-beneth` not found!');
        return
    }
    beneth.style.width = personalBlockWidth;
    beneth.style.height = portraitHeight;

    var portrait = document.getElementById('float-personal-block-portrait');
    if (portrait === null) {
        console.log('elemet `float-personal-block-portrait` not found!');
        return
    }

    var realHeight = portraitHeight / 4 * 3;
    portrait.style.top = (portraitHeight - realHeight) / 2;
    portrait.style.right = (portraitWidth - realHeight) / 2;
    portrait.style.width = realHeight;
    portrait.style.height = realHeight;
    portrait.style.borderRadius = realHeight + 'px';
    portrait.style.backgroundSize = realHeight + 'px';

    ////

    const incrementStep = portraitHeight / 4 / 4;
    console.log('increment step is:', incrementStep);
    for (var i = 1; i < 5; ++i) {
        realHeight += incrementStep;

        var portraitBorder = document.getElementById('float-personal-block-portrait-border' + i);
        if (portraitBorder === null) {
            console.log('elemet `float-personal-block-portrait-border${i}` not found!');
            return
        }

        portraitBorder.style.top = (portraitHeight - realHeight) / 2;
        portraitBorder.style.right = (portraitWidth - realHeight) / 2;
        portraitBorder.style.width = realHeight;
        portraitBorder.style.height = realHeight;
        portraitBorder.style.borderRadius = realHeight + 2 + 'px';
        portraitBorder.style.zIndex = -i;

        if (i != 0 || true) {
            portraitBorder.style.animation = 'portrait-animation' + i + ' ' + 0.1 * 10 + 's linear ' + 0.1 * i * 2 + 's alternate infinite';
        }
    }

    ///////////
    var headerText = document.getElementById('header-text');
    if (headerText === null) {
        console.log('the count of `header-text` elements found is not 1');
        return
    }
    headerText.style.fontSize = portraitHeight / 2;
    //  one half of header motto's right padding
    headerText.style.paddingRight = portraitHeight / 2 / 3 / 2; 

    var headerMotto = document.getElementById('header-motto');
    if (headerMotto === null) {
        console.log('the count of `header-motto` elements found is not 1');
        return
    }

    headerMotto.style.fontSize = portraitHeight / 2 / 3;
    headerMotto.style.paddingLeft = portraitHeight / 2 / 3;

    ////////////
    var headers = document.getElementsByTagName('header');
    if (headers === null || headers.length !== 1) {
        console.log('the count of header elements found is not 1');
        return
    }

    var header = headers[0];
    header.style.height = portraitHeight;


    ////////////
    var navs = document.getElementsByTagName('nav');
    if (navs === null || navs.length !== 1) {
        console.log('the count of nav elements found is not 1');
        return
    }

    var nav = navs[0];
    nav.style.height = sectionHeight;

    ////

    //  no need to resize <ul>s, they will auto fit theie childern's size.
    var navButtons = document.getElementsByClassName('nav-button');
    if (navButtons == null) {
        console.log('the count of nav-button elements not found');
        return
    }

    var searchInputPadding = function() {
        var searchInput = document.getElementById('search-input');
        if (searchInput === null) {
            console.log('the count of search-button elements not found');
            return
        }

        var style = window.getComputedStyle(document.getElementById("search-input"), null);
        return parseFloat(style.getPropertyValue("padding"));
    }()

    for (var i = 0; i < navButtons.length; ++i) {
        navButtons[i].style.height = sectionHeight;
        navButtons[i].style.lineHeight = sectionHeight + 'px';
        navButtons[i].style.fontSize = sectionHeight / 4 + 'px';

        //todo:
        //  minus the section width, search input width, search button width
        navButtons[i].style.width = (documentWidth - sectionWidth - sectionHeight / 3 * 2 - sectionWidth) / 6;
    }

    ////
    var top = portraitHeight + sectionHeight / 6 + 'px';

    var searchButton = document.getElementById('search-button');
    if (searchButton === null) {
        console.log('the count of search-button elements not found');
        return
    }

    searchButton.style.top = top;
    searchButton.style.right = portraitWidth;
    searchButton.style.width = sectionHeight / 3 * 2 + 'px';
    searchButton.style.height = sectionHeight / 3 * 2 + 'px';
    searchButton.style.backgroundSize = sectionHeight / 3 * 2 + 'px ' + sectionHeight / 3 * 1 + 'px';

    ////

    var searchInput = document.getElementById('search-input');
    if (searchInput === null) {
        console.log('the count of search-input elements not found');
        return
    }

    searchInput.style.top = top;
    searchInput.style.right = portraitWidth + parseFloat(searchButton.style.width);
    searchInput.style.width = sectionWidth;
    searchInput.style.height = sectionHeight/ 3 * 2;
    searchInput.style.fontSize = parseFloat(searchInput.style.height) / 2;

    ////

    // var searchContainer = document.getElementById('search-container');
    // if (searchContainer === null) {
    //     console.log('the count of search-container elements not found');
    //     return
    // }

    // searchContainer.style.top = portraitHeight;
    //searchContainer.style.right = sectionWidth + ;

    ////////////
    const itemHeight = sectionHeight / 2;
    //const itemWidth = 

    var items = document.getElementsByClassName('item');
    for (var i = 0; i < items.length; ++i) {
        items[i].style.height = itemHeight;
        items[i].style.lineHeight = itemHeight + 'px';
        items[i].style.fontSize = itemHeight / 5 * 3;
    }

    //todo:

    const svgNamespace = 'http://www.w3.org/2000/svg';
    var svg = document.createElementNS(svgNamespace, 'svg');
    svg.setAttribute('width', portraitWidth);
    svg.setAttribute('height', portraitWidth / 2);

    const cellCountX = 10;
    const cellCountY = new Date().getMonth() == 1 ? 3 : 4;
    
    createAndAppendFirstG(portraitWidth, svg, svgNamespace, cellCountX, cellCountY);
    createAndAppendSecondG(floatPersonalBlock, svgNamespace, svg, portraitWidth, cellCountX, cellCountY);
    floatPersonalBlock.appendChild(svg);
};
document.addEventListener('DOMContentLoaded', layoutLoader, false);

function createAndAppendFirstG( portraitWidth, svgElement, svgNs, cellCountX, cellCountY) {
    var gEle = document.createElementNS(svgNs, 'g');

    const cellWidth = portraitWidth / (cellCountX + (cellCountX + 1) / 7.0);
    const cellHeight = cellWidth;
    const cellSpace = cellWidth / 7.0;
    const unitSuffix = 'px';
    
    for (var y = 0; y < cellCountY; ++y) {
        for (var x = 0; x < cellCountX; ++x) {
            var cell = document.createElementNS(svgNs, 'rect');

            cell.setAttribute('x', cellSpace + x * (cellWidth + cellSpace) + unitSuffix)
            cell.setAttribute('y', cellSpace + y * (cellHeight + cellSpace) + unitSuffix);
            cell.setAttribute('width', cellWidth);
            cell.setAttribute('height', cellHeight);
            cell.setAttribute('fill', 'rgb(235, 237, 240)');

            gEle.appendChild(cell);
        }
    }
    svgElement.appendChild(gEle);
}


function createAndAppendSecondG(
    floatPersonalBlock, svgNs, svg, 
    portraitWidth, cellCountX, cellCountY) {

    function appendTo(parentElement, svgElement, x, y) {
        svgElement.setAttribute('x', x);
        svgElement.setAttribute('y', y);
        parentElement.appendChild(svgElement);
    }

    const cellWidth = portraitWidth / (cellCountX + (cellCountX + 1) / 7.0);
    const cellHeight = cellWidth;
    const cellSpace = cellWidth / 7.0;

    var textMonth = createSvgText(svgNs, 'rgb(78, 86, 95)', getCurrentMonthText())
    var textLess = createSvgText(svgNs, 'rgb(78, 86, 95)', 'Less');
    var rect1 = createSvgRect(svgNs, cellWidth, cellHeight, 'rgb(235, 237, 240)');
    var rect2 = createSvgRect(svgNs, cellWidth, cellHeight, 'rgb(198, 228, 139)');
    var rect3 = createSvgRect(svgNs, cellWidth, cellHeight, 'rgb(123, 201, 111)');
    var textMore = createSvgText(svgNs, 'rgb(78, 86, 95)', 'More');

    gEle = document.createElementNS(svgNs, 'g');
    gEle.setAttribute('font-size', cellHeight / 5.0 * 4.0 + 'px');
    
    const textMargin = 4;
    var endX = portraitWidth;
    var startY = (cellHeight + cellSpace) * (cellCountY) + cellSpace;
    var textLessSize = getSvgTextSize(floatPersonalBlock, svg, gEle, textLess);
    var textMoreSize = getSvgTextSize(floatPersonalBlock, svg, gEle, textMore);

    appendTo(gEle, textMonth, cellSpace, startY + cellHeight);    

    var xVal = endX - textLessSize.width - textMargin * 2 - (cellWidth + cellSpace) * 3 - (textMoreSize.width + cellSpace);
    appendTo(gEle, textLess, xVal, startY + cellHeight);

    xVal += (textLessSize.width + cellSpace + textMargin);
    appendTo(gEle, rect1, xVal, startY);

    xVal += (cellWidth + cellSpace);
    appendTo(gEle, rect2, xVal, startY);

    xVal += (cellWidth + cellSpace);
    appendTo(gEle, rect3, xVal, startY);

    xVal += (cellWidth + cellSpace + textMargin);
    appendTo(gEle, textMore, xVal, startY + cellHeight);
    
    svg.appendChild(gEle);
}

function createSvgText(svgNamespace, textColor, textStr) {
    var text = document.createElementNS(svgNamespace, 'text');

    text.setAttribute('text-anchor', 'start');
    text.setAttribute('fill', textColor);

    text.innerHTML = textStr;
    return text;
}

function createSvgRect(svgNamespace, width, height, fillColor) {
    var rect = document.createElementNS(svgNamespace, 'rect');

    rect.setAttribute('width', width);
    rect.setAttribute('height', height);
    rect.setAttribute('fill', fillColor);

    return rect;
}

function getSvgTextSize(parentStaticElement, svgElement, gElement, textElement) {
    gElement.appendChild(textElement);
    svgElement.appendChild(gElement);
    parentStaticElement.appendChild(svgElement);

    var textWidth = textElement.getBBox().width;
    var textHeight = textElement.getBBox().height;

    parentStaticElement.removeChild(svgElement);
    svgElement.removeChild(gElement);
    gElement.removeChild(textElement);

    return {
        width: textWidth,
        height: textHeight
    }
}

function getCurrentMonthText() {
    const monthNames = [
        "Jan",  "Feb",  "Mar",  "Apr",  "May",  "Jun",
        "Jul",  "Aug",  "Sep",  "Oct",  "Nov",  "Dec"
    ];

    var monthIndex = new Date().getMonth();
    return monthNames[monthIndex];
}

/////////////////////////////////////////////////////////////////////////////
//      Callable functions.
/////////////////////////////////////////////////////////////////////////////

function onWeightInput() {
    var element = document.getElementById('weight-input');
    if (element == null) {
        console.log('could not found input elements of id `weight-input`')
        return
    }

    var floatValue = parseFloat(element.value)
    if (isNaN(floatValue)) {
        //todo: prompt
        return 
    }

    //  todo:
}

function onWeightFocus() {
    console.log('weight focus')
}

function onWeightBlur() {
    console.log('weight blur')

}
