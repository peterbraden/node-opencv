// see https://github.com/Itseez/opencv/blob/master/samples/cpp/mask_tmpl.cpp
const cv = require('../lib/opencv');

const TM_CCORR_NORMED = 3
const imageFilename = 'files/lena_tmpl.jpg';
const templateFilename = 'files/tmpl.png';
const maskFilename = 'files/mask.png';
const outFilename = 'tmp/match-template-mask.png';

cv.readImage(imageFilename, function(err, im) {
  if (err) throw err;
  cv.readImage(templateFilename, function (err, tmpl) {
    if (err) throw err;
    const res = im.matchTemplate(templateFilename, TM_CCORR_NORMED,  maskFilename);
    const minMax = res.minMaxLoc();
    const topLeft = minMax.maxLoc;
    im.rectangle([topLeft.x, topLeft.y], [tmpl.width(), tmpl.height()], [0, 255,0], 2);

    im.save(outFilename);
    console.log('Image saved to ' + outFilename);
  });
});
