var cv = require('../lib/opencv')
  , assert = require('assert')
  , fs =require('fs')

  cv.readImage("./car1.jpg", function(err, im){

					im.detectObject("../data/hogcascade_cars_sideview.xml", {}, function(err, cars){  
 
								for (var i=0;i<cars.length; i++){
									var x = cars[i];
									im.rectangle([x.x, x.y], [x.width, x.height]);
								}

								im.save('./cars.jpg');   
  					});
});