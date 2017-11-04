var cv = require('../lib/opencv');



var img = cv.readImage("./files/mona.png");
console.log('Synchronous readImage("./files/mona.png")'+img.width()+'x'+img.height());

cv.readImage("./files/mona.png", function(err, im){
	console.log('callback readImage("./files/mona.png", fn(){})'+im.width()+'x'+im.height());
});

img = cv.readImage( 100, 100 );
console.log('Synchronous readImage(100, 100) (create mat)'+img.width()+'x'+img.height());

cv.readImage(100, 100, function(err, im){
	console.log('callback readImage(100, 100, fn(){}) (create mat)'+im.width()+'x'+im.height());
});

var fs = require('fs');
var imgdata = fs.readFileSync("./files/mona.png");

img = cv.readImage(imgdata);
console.log('Synchronous readImage(imgdata:Buffer)'+img.width()+'x'+img.height());

cv.readImage(imgdata, function(err, im){
	console.log('callback readImage(imgdata:Buffer, fn(){})'+im.width()+'x'+im.height());
});
