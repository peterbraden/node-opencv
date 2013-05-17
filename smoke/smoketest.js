var cv = require('../lib/opencv')

var trainingData = []
/*
for (var i = 1; i< 41; i++){
  for (var j = 1; j<10; j++){
    trainingData.push([i,"/Users/peterbraden/Downloads/orl_faces/s" + i + "/" + j + ".pgm" ])
  }
}

cv.readImage("/Users/peterbraden/Downloads/orl_faces/s6/10.pgm", function(e, im){

  var facerec = cv.FaceRecognizer.createEigenFaceRecognizer(); 
  //facerec.trainSync(trainingData);
  facerec.loadSync("/Users/peterbraden/Desktop/ORL")

  console.log(facerec.predictSync(im));

})


*/
cv.readImage("/Users/peterbraden/Desktop/repos/node-opencv/examples/mona.png", function(e, mat){
  var th = mat.threshold(20, 200);
  th.save('out.png')

})

