/////////////////////////////////////////////////////////////////////////////
//      Global codes to be executed by documents which reference this js file.
/////////////////////////////////////////////////////////////////////////////
document.addEventListener('DOMContentLoaded', function(event) {
    console.log('DOMContentLoaded');

    var documentHeight = document.body.clientHeight;
    var documentWidth = document.body.clientWidth;
    //  todo: fit to the document sizes

    
    /*var svg  = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    var svgNs = svg.namespaceURI;

    var rect = document.createElementNS(svgNs, 'rect');
    rect.setAttribute('width',500);
    rect.setAttribute('height',900);
    rect.setAttribute('fill','#95B3D7');

    svg.appendChild(rect);
    document.body.appendChild(svg);
    */

    var svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');

    var svgNs = svg.namespaceURI;
    var gEle = document.createElementNS(svgNs, 'g');

    const gWidth = 160;
    const gHeight = 145;
    const cellCountX = 10;
    const cellCountY = new Date().getMonth() == 1 ? 3 : 4;
    const cellSpace = 4;

    const cellWidth = (gWidth - cellSpace * (cellCountX - 1)) / cellCountX;
    const cellHeight = cellWidth;

    const unitSize = 12;
    const unitSuffix = 'pt';
    const unitSizeWithSuffix = '12pt';

    for (var y = 0; y < cellCountY; ++y) {
    for (var x = 0; x < cellCountX; ++x) {
            var cell = document.createElementNS(svgNs, 'rect');

            cell.setAttribute('x', x * (cellWidth + cellSpace) + unitSuffix)
            cell.setAttribute('y', y * (cellHeight + cellSpace) + unitSuffix);
            cell.setAttribute('width', unitSizeWithSuffix);
            cell.setAttribute('height', unitSizeWithSuffix);
            cell.setAttribute('fill', 'rgb(235, 237, 240)');

            gEle.appendChild(cell);
        }
    }
    svg.appendChild(gEle);

    function createAndAppendTextTo(parentElement, svgNamespace, x, y, width, height,  textAnchor, textColor, textStr) {
        var text = document.createElementNS(svgNamespace, 'text');
        text.setAttribute('x', x);
        text.setAttribute('y', y);

        text.setAttribute('width', width);
        text.setAttribute('height', height);
        text.setAttribute('text-anchor', textAnchor);
        text.setAttribute('color', textColor);
        text.innerHTML = textStr;
        parentElement.appendChild(text);
    }

    function createAndAppendRectTo(parentElement, svgNamespace, x, y, width, height, fillColor) {
        var rect = document.createElementNS(svgNamespace, 'rect');

        rect.setAttribute('x', x);
        rect.setAttribute('y', y);
    
        rect.setAttribute('width', width);
        rect.setAttribute('height', height);
        rect.setAttribute('fill', fillColor);
        parentElement.appendChild(rect);
    }

    gEle = document.createElementNS(svgNs, 'g');
    createAndAppendTextTo(gEle, svgNs, 
        (cellCountX - 3) * (cellWidth + cellSpace) + cellWidth + unitSuffix,
        (cellCountY) * (cellHeight + cellSpace) + cellHeight - cellSpace + unitSuffix,
        '60pt',
        '20pt',
        'end',
        'black',
        'Less');

    createAndAppendRectTo(gEle, 
        svgNs,
        (cellCountX - 2) * (cellWidth + cellSpace) + cellWidth + unitSuffix,
        (cellCountY) * (cellHeight + cellSpace) + unitSuffix,
        unitSizeWithSuffix,
        unitSizeWithSuffix,
        'rgb(25, 97, 39)');
    createAndAppendRectTo(gEle,
        svgNs,
        (cellCountX - 3) * (cellWidth + cellSpace) + cellWidth + unitSuffix,
        (cellCountY) * (cellHeight + cellSpace) + unitSuffix,
        unitSizeWithSuffix,
        unitSizeWithSuffix,
        'rgb(123, 201, 111)');
    createAndAppendRectTo(gEle,
        svgNs,
        (cellCountX - 4) * (cellWidth + cellSpace) + cellWidth + unitSuffix,
        (cellCountY) * (cellHeight + cellSpace) + unitSuffix,
        unitSizeWithSuffix,
        unitSizeWithSuffix,
        'rgb(198, 228, 139)');

    createAndAppendTextTo(gEle, svgNs, 
        (cellCountX - 1) * (cellWidth + cellSpace) + cellWidth + unitSuffix,
        (cellCountY) * (cellHeight + cellSpace) + cellHeight - cellSpace + unitSuffix,
        '60pt',
        '20pt',
        'end',
        'black',
        'More');
    
    svg.appendChild(gEle);

    var floatPersonalBlock = document.getElementById('float-personal-block');
    if (floatPersonalBlock == null) {
        console.log('elemet `float-personal-block` not found!');
        return
    }
    floatPersonalBlock.appendChild(svg);
});

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