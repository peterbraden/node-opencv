var opencv = require('../lib/opencv');
var mat = new opencv.Matrix(256,256,opencv.Constants.CV_8UC1);
var buf = Buffer(256*256);
buf.fill(0);
for(var i=0;i<256*256;i++){
  buf[i] = (i%2===1)?230:0;
}
mat.put(buf);
var window = new opencv.NamedWindow('Mat-Put', 0);
window.show(mat);
window.blockingWaitKey(0, 50);
setTimeout(function(){
  // keep the windows 30sec open
},30000);
