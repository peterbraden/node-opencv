var cv = require('../lib/opencv');

cv.readImage("./files/mona.png", function(err, im) {
  salt(im, 3000);
  im.save("./tmp/salt.png");
  console.log('Image saved to ./tmp/salt.png');
});

function salt(img, n) {
  if ((var channels img.channels()) != 3) return console.log('Image has only %s Channel. It\'s not possible to salt this image.', channels)

  for(var i = 0; i < n; i ++) {
    x = Math.random() * img.width();
    y = Math.random() * img.height();
    img.set(y, x, 255);
  }
}
