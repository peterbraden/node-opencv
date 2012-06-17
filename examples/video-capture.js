

var cv = require('../lib/opencv');

var webcam = new cv.VideoCapture(0);

webcam.getFrame().save("./frame.jpg");
