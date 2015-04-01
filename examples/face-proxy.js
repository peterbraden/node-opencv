// Face recognition proxy
var http = require('http'),
    request = require('request'),
    cv = require('../lib/opencv');

var server = http.createServer(function(req, resp){
  var url = req.url.slice(1);
  request({uri:url, encoding:'binary'}, function(err, r, body){
    if (err) return resp.end(err.stack);
    if (!/image\//.test(r.headers['content-type'])) return resp.end('Not an image');

    cv.readImage(new Buffer(body, 'binary'), function(err, im){
      if (err) return resp.end(err.stack);
      if (im.width() < 1 || im.height() < 1) return resp.end('Image has no size');

      im.detectObject('../data/haarcascade_frontalface_alt.xml', {}, function(err, faces) {
        if (err) return resp.end(err.stack);

        for (var i = 0; i < faces.length; i++){
          var face = faces[i];
          im.ellipse(face.x + face.width / 2, face.y + face.height / 2, face.width / 2, face.height / 2);
        }

        resp.writeHead(200, {'Content-Type': 'image/jpeg'});
        resp.end(im.toBuffer());
      });
    });
  });

})


//server.listen(3000, function(){ console.log('Listening on http://localhost:3000'); })
