var cv = require('../lib/opencv');

var createGaussianKernel = function(sigma){
  var cst = 1;
  var denom = (2*sigma*sigma);
  var factor = 100;
  return {
    get : function(i,j){
      //console.log(cst, denom, ((i/10)*(i/10)+(j/10)*(j/10)), denom, Math.exp((i*i+j*j)/denom))
      return cst*Math.exp(-1*(i*i+j*j)/denom);
    }
  }
};

var convolve = function(m1, m2){
  var nCols = m1.width();
  var nRows = m1.height();

  var max = 0;

  var res = new cv.Matrix(nRows, nCols, cv.Constants.CV_32FC1);

  for(var i = 0; i < nCols; i++){
    for(var j = 0; j < nRows; j++){
      sum = 0;

      for(var i2 = i-nCols+1; i2 <= i; i2++){
        for(var j2 = j-nRows+1; j2 <= j; j2++){
          sum += m1.get(i-i2,j-j2)*m2.get(i2,j2)
          if(isNaN(sum)){
            console.log(m1.get(i-i2,j-j2),m2.get(i2,j2), i-i2,i2,j-j2,j2)
            throw("sum is NaN")
          }
        }
      }
      //console.log(sum)

      if(sum > max){
        max = sum
      }

      res.set(i,j, sum);
    }
  }
  //console.log(max);

  for(var i = 0; i < nCols; i++){
    for(var j = 0; j < nRows; j++){
      res.set(i,j, res.get(i,j)/max);
    }
  }
  return res;

};

var getSalient = function(channel){
  var floatInputMatrix = new cv.Matrix();
  channel.convertTo(floatInputMatrix, cv.Constants.CV_32F);
  var floatAfterDct = floatInputMatrix.dct();

  var nCols = channel.width();
  var nRows = channel.height();

  var signs = new cv.Matrix(nRows, nCols, cv.Constants.CV_32FC1);
  var arr = [];
  for(var i = 0; i < nCols; i++){
    var row = [];
    for(var j = 0; j < nRows; j++){
      if(floatAfterDct.get(i,j) < 0){
        signs.set(i, j,  -1);
      } else if(floatAfterDct.get(i,j) == 0){
        signs.set(i, j, 0);
      } else {
        signs.set(i, j, 1);
      }
    }
    arr.push(row)
  }
  // for inverse dct, set argument to "true"
  var afterDoubleDct = signs.idct();
  //console.log(afterDoubleDct.getData())
  var squaredFloat = new cv.Matrix(nRows, nCols, cv.Constants.CV_32FC1);

  for(var i = 0; i < nCols; i++){
    for(var j = 0; j < nRows; j++){
      var v = afterDoubleDct.get(i,j)
      squaredFloat.set(i,j, v * v)
    }
  }

  var sigma = 0.3;

  var res = convolve(squaredFloat, createGaussianKernel(sigma))
  var outChannel = new cv.Matrix(nRows, nCols, cv.Constants.CV_8U);

  res.convertTo(outChannel, cv.Constants.CV_8U, 255);

  return outChannel;
};

cv.readImage("./files/mona.png", function(err, orig) {
  if (err) throw err;

  orig.resize(64, 64);

  var channels = orig.split();
  var outChannels = [];

  for(var chanIdx = 0; chanIdx < channels.length; chanIdx++){
    outChannels.push(getSalient(channels[chanIdx]));
  }

  var outImg = new cv.Matrix()

  outImg.merge(outChannels)

  outImg.save("./tmp/dct.png");
  console.log('Image saved to ./tmp/dct.png');

});
