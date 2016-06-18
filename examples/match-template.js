const cv = require('../lib/opencv');

const TM_CCORR_NORMED = 3;
const imageFilename = 'files/stuff.png';
const templateFilename = 'files/stuff-template.png';
const outFilename = 'tmp/match-template.png';

cv.readImage(imageFilename, function (err, im) {
  if (err) throw err;
  cv.readImage(templateFilename, function (err, tmpl) {
    if (err) throw err;
    const res = im.matchTemplate(templateFilename, TM_CCORR_NORMED);
    const minMax = res.minMaxLoc();
    const topLeft = minMax.maxLoc;
    im.rectangle([topLeft.x, topLeft.y], [tmpl.width(), tmpl.height()], [0, 255,0], 2);

    im.save(outFilename);
    console.log('Image saved to ' + outFilename);
  });
});
