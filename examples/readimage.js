var cv = require('../lib/opencv');


// read as a buffer for decode operations later
var fs = require('fs');
var imgdata = fs.readFileSync("./files/mona.png");



var img = cv.readImage("./files/mona.png");
console.log('Synchronous readImage("./files/mona.png")'+img.width()+'x'+img.height());

cv.readImage("./files/mona.png", function(err, im){
	console.log('callback readImage("./files/mona.png", fn(){})'+im.width()+'x'+im.height());
});

img = cv.readImage( 100, 100 );
console.log('Synchronous readImage(100, 100) (create mat)'+img.width()+'x'+img.height()+' type '+img.type());

cv.readImage(100, 100, function(err, im){
	console.log('callback readImage(100, 100, fn(){}) (create mat)'+im.width()+'x'+im.height());
});

img = cv.readImage(imgdata);
console.log('Synchronous readImage(imgdata:Buffer)'+img.width()+'x'+img.height());

cv.readImage(imgdata, function(err, im){
	console.log('callback readImage(imgdata:Buffer, fn(){})'+im.width()+'x'+im.height());
});


// try with flags now
console.log('Now with flags');

img = cv.readImage("./files/mona.png", 0);
console.log('Synchronous readImage("./files/mona.png", 0) (monochrome)'+img.width()+'x'+img.height()+' type '+img.type());

cv.readImage("./files/mona.png", 1, function(err, im){
	console.log('callback readImage("./files/mona.png", 1, fn(){}) (colour)'+im.width()+'x'+im.height()+' type '+im.type());
});

img = cv.readImage( 100, 100, cv.Constants.CV_8UC3 );
console.log('Synchronous readImage(100, 100, cv.Constants.CV_8UC3) (create 8 bit 3 channel mat)'+img.width()+'x'+img.height()+' type '+img.type());

cv.readImage(100, 100, cv.Constants.CV_8UC1, function(err, im){
	console.log('callback readImage(100, 100, cv.Constants.CV_8UC1, fn(){}) (create mat)'+im.width()+'x'+im.height()+' type '+im.type());
});

img = cv.readImage(imgdata, 0);
console.log('Synchronous readImage(imgdata:Buffer, 0) (monochrome)'+img.width()+'x'+img.height()+' type '+img.type());

cv.readImage(imgdata, 1, function(err, im){
	console.log('callback readImage(imgdata:Buffer, 1, fn(){}) (colour)'+im.width()+'x'+im.height()+' type '+im.type());
});





// try with readImageAsync
console.log('Now with readImageAsync');

img = cv.readImageAsync("./files/mona.png");
console.log('Synchronous readImageAsync("./files/mona.png")'+img.width()+'x'+img.height());

cv.readImageAsync("./files/mona.png", function(err, im){
	console.log('Asynchronous callback readImageAsync("./files/mona.png", fn(){})'+im.width()+'x'+im.height());
});

img = cv.readImageAsync( 100, 100 );
console.log('Synchronous readImageAsync(100, 100) (create mat)'+img.width()+'x'+img.height()+' type '+img.type());

cv.readImageAsync(100, 100, function(err, im){
	console.log('!!Synchronous!! callback readImageAsync(100, 100, fn(){}) (create mat)'+im.width()+'x'+im.height());
});

img = cv.readImageAsync(imgdata);
console.log('Synchronous readImageAsync(imgdata:Buffer)'+img.width()+'x'+img.height());

cv.readImageAsync(imgdata, function(err, im){
	console.log('Asynchronous callback readImageAsync(imgdata:Buffer, fn(){})'+im.width()+'x'+im.height());
});



// try with flags now
console.log('Now Async with flags');

img = cv.readImageAsync("./files/mona.png", 0);
console.log('Synchronous readImageAsync("./files/mona.png", 0) (monochrome)'+img.width()+'x'+img.height()+' type '+img.type());

cv.readImageAsync("./files/mona.png", 1, function(err, im){
	console.log('Asynchronous callback readImageAsync("./files/mona.png", 1, fn(){}) (colour)'+im.width()+'x'+im.height()+' type '+im.type());
});

img = cv.readImageAsync( 100, 100, cv.Constants.CV_8UC3 );
console.log('Synchronous readImageAsync(100, 100, cv.Constants.CV_8UC3) (create 8 bit 3 channel mat)'+img.width()+'x'+img.height()+' type '+img.type());

cv.readImageAsync(100, 100, cv.Constants.CV_8UC1, function(err, im){
	console.log('!!Synchronous!! callback readImageAsync(100, 100, cv.Constants.CV_8UC1, fn(){}) (create mat)'+im.width()+'x'+im.height()+' type '+im.type());
});

img = cv.readImageAsync(imgdata, 0);
console.log('Synchronous readImageAsync(imgdata:Buffer, 0) (monochrome)'+img.width()+'x'+img.height()+' type '+img.type());

cv.readImageAsync(imgdata, 1, function(err, im){
	console.log('Asynchronous callback readImageAsync(imgdata:Buffer, 1, fn(){}) (colour)'+im.width()+'x'+im.height()+' type '+im.type());
});

console.log('\nTruely Async callbacks should be after this line\n');

