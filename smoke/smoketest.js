var cv = require('../lib/opencv')

var trainingData = []

for (var i = 1; i< 41; i++){
  for (var j = 1; j<10; j++){
    trainingData.push([i,"/Users/peterbraden/Downloads/orl_faces/s" + i + "/" + j + ".pgm" ])
  }
}

cv.readImage("/Users/peterbraden/Downloads/orl_faces/s6/10.pgm", function(e, im){

  var facerec = cv.FaceRecognizer.createFisherFaceRecognizer(); 
  facerec.trainSync(trainingData);

  console.log(facerec.predictSync(im));

  var x = new cv.NamedWindow("out")
  x.show(im);
  x.blockingWaitKey();
  x.destroy()
})



