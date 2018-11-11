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
    svg.style.width = '160pt';
    svg.style.height = '200pt';

    var svgNs = svg.namespaceURI;
    var gEle = document.createElementNS(svgNs, 'g');
    gEle.style.width = '160pt';
    gEle.style.height = '200pt';

    const gWidth = 160;
    const gHeight = 145;
    const cellCountX = 10;
    const cellCountY = new Date().getMonth() == 1 ? 3 : 4;
    const cellSpace = 4;

    const cellWidth = (gWidth - cellSpace * (cellCountX - 1)) / cellCountX;
    const cellHeight = cellWidth;
    //const cellHeight = (gHeight - cellSpace * (cellCountY - 1)) / cellCountY;

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

    var legendLine = document.createElementNS(svgNs, 'text');
    // legendLine.setAttribute('x', (cellCountX - 1) * (cellWidth + cellSpace) + cellWidth + unitSuffix);
    // legendLine.setAttribute('y', (cellCountY) * (cellHeight + cellSpace) + unitSuffix);
    var as = (cellCountY) * (cellHeight + cellSpace) + unitSuffix;
    legendLine.setAttribute('width', '60pt');
    legendLine.setAttribute('height', '20pt');
    // legendLine.setAttribute('text-anchor', 'start');
    legendLine.setAttribute('color', 'black');
    legendLine.innerHTML = "Less More";

    svg.appendChild(legendLine);

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