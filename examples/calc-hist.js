var cv = require('../lib/opencv');

// (B)lue, (G)reen, (R)ed
var histSize = 256;

cv.readImage('./files/car1.jpg', function(err, im) {
  if (err) throw err;
  if (im.width() < 1 || im.height() < 1) throw new Error('Image has no size');

  var bgrPlanes = im.split();

  var size = 256,
      range = [0, 256],
      uniform = true,
      accumulate = true,
      histFile = 'files/chart2.png';


  /// Compute the histograms:
  var hist64 = cv.histogram.calcHist( im, [0, 1, 2], [4, 4, 4], [[0, 256], [0, 256], [0, 256]], uniform, accumulate );

  var bHist = cv.histogram.calcHist( im, [0], [size], [range], uniform, accumulate );
  var gHist = cv.histogram.calcHist( im, [1], [size], [range], uniform, accumulate );
  var rHist = cv.histogram.calcHist( im, [2], [size], [range], uniform, accumulate );

//////
//  Uncommentand run `npm install chartjs-node` to draw the histogram !
///
/*
  var ChartjsNode = require('chartjs-node');
  var chartNode = new ChartjsNode(1200, 1200);
  chartNode.drawChart({
      type: 'bar',
      data: {
        labels: bHist.map(function(a,i){return i.toString()}),
        datasets : [{
          data : bHist,
          backgroundColor : "#4183c4",
          borderColor : "#0c4b8a",
          label : 'Blue'
        },{
          data : gHist,
          backgroundColor : "#83c441",
          borderColor : "#0c4b8a",
          label : 'Green'
        },{
          data : rHist,
          backgroundColor : "#c44183",
          borderColor : "#0c4b8a",
          label : 'Red'
        }]
      },
      options: {
        title: {
            display: true,
            text: 'RGB Histograms'
        }
      }
  }).then(function(){
    return chartNode.writeImageToFile('image/png', histFile);
  }).then(function(){
    console.log("result has been written in "+histFile)
  }).catch(function(e){
    console.log("error",e)
  });
*/
});
