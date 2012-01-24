/*

Face recognition proxy

*/

var http = require('http')
  , request = require('request')
  , cv = require('../lib/opencv')
  , face_cascade = new cv.CascadeClassifier("./examples/haarcascade_frontalface_alt.xml")
 
 
 
 http.createServer(function(req, resp){
 	var url = req.url.slice(1);
 	console.log(url);
 	
 	if (url.indexOf('http') != 0){
 		return request({uri:'http://google.com'}).pipe(resp)
 	}

 	// TODO make sure image
 	if (url.indexOf(".jpg", url.length - 4) !== -1 ||
        url.indexOf(".png", url.length - 4) !== -1){
        
        request({uri:url, encoding:'binary'}, function(err, r, body){
			if (err) throw err;
			
			var im = new cv.readImage(new Buffer(body, 'binary'));		
			
			im.faceDetect(im, {}, function(err, faces){ 	  
	          for (var i=0;i<faces.length; i++){
	            var x = faces[i]
	            im.ellipse(x.x + x.width/2, x.y + x.height/2, x.width/2, x.height/2);
	          }

	          //console.log(faces);
			  resp.writeHead(200, {'Content-Type': 'image/jpeg'});  
	    	  resp.end(im.toBuffer());  	                
        	});
		})
    } else {
    	request({uri:url || 'http://google.com'}).pipe(resp)
    }
 }).listen(1901) 

