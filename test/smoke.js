var vows = require('vows')
  , assert = require('assert')
  , fs = require('fs');



vows.describe('Smoke Tests OpenCV').addBatch({
	  "Importing": {

    	topic: require('../lib/opencv')

      , "returns obj": function(topic){
        assert.ok(!!topic)
      }

    	, '.version' : function(topic){
    		assert.ok(!!topic.version)
    	  }

    	, '.Point imports': function(topic){
    		assert.ok(!!topic.Point)
    	} 
      
      , '.Matrix imports': function(topic){
        assert.ok(!!topic.Matrix)
      } 
    }

  , "Point" : {
    topic : require('../lib/opencv')
    
    , 'constructor' : function(cv){
      assert.ok(!!new cv.Point(1, 2))
      assert.throws(function () { cv.Point(1, 2)}, TypeError); // cannot call without new
    }    

    , 'accessors' : function(cv){
        assert.equal(new cv.Point(1, 2).x, 1)
        assert.equal(new cv.Point(1, 2).y, 2)
        assert.equal(Math.round(new cv.Point(1.1, 2).x * 100), 110)
        assert.equal(Math.round(new cv.Point(1.2, 2.75).y *100), 275)

        assert.throws(function () {new cv.Point(1.1, 2).x = 5}, Error); // Points are immutable
        assert.throws(function () {new cv.Point(1.1, 2).y = 5}, Error); // Points are immutable

    }


    , '.dot': function(cv){
      var p1 = new cv.Point(3, 6)
        , p2 = new cv.Point(5, 7)

      assert.ok(p1.dot);
      assert.equal(p1.dot(p2), 57);

    }

    , '.inside' : function(){}
  }


  , "Matrix": {
    topic : require('../lib/opencv')

    , "constructor" : function(cv){
      assert.ok(cv.Matrix);
      assert.ok(new cv.Matrix);
      assert.ok(new cv.Matrix(1,2));
    }
    , "get/set" : function(cv){
       var mat = new cv.Matrix(1,2);
       assert.equal(mat.set(0,0,3), undefined);
       assert.equal(mat.get(0,0), 3);
    }

    , ".width" : function(cv){
      var mat = new cv.Matrix(6,7);
      assert.equal(mat.width(), 7);
    }

    , ".height" : function(cv){
      var mat = new cv.Matrix(6,7);
      assert.equal(mat.height(), 6);
    }

    , ".size" : function(cv){
      var mat = new cv.Matrix(6,7);
      assert.deepEqual(mat.size(), [6, 7]);
    }

    , "empty": function(cv){
      assert.equal(new cv.Matrix().empty(), true);
    }

    , "toBuffer": function(cv){
        var buf = fs.readFileSync('./examples/mona.jpg')
          , im = cv.readImage(buf.slice(0))
          , buf0 = im.toBuffer()
        
        assert.ok(buf0);
        //assert.equal(buf.toString('base64'), buf0.toString('base64'));
    }

    , "faceDetect": {
      
     topic : function(){
        var cv = require('../lib/opencv')
          , im = cv.readImage("./examples/mona.jpg")
          im.faceDetect("./examples/haarcascade_frontalface_alt.xml", {}, this.callback)
      }

      , "finds face": function(err, faces){
          assert.isNull(err);
          assert.isArray(faces);
          assert.equal(faces.length, 1)
      }
    }

    
  }



  , "Image" : {
    topic : require('../lib/opencv')

    , ".readImage from file": function(cv){
      var im = cv.readImage("./examples/mona.jpg")
      assert.ok(im)
      assert.equal(im.width(), 500);
      assert.equal(im.height(), 756)
      assert.equal(im.empty(), false)
    }

    , ".readImage from buffer" : function(cv){
      var im = cv.readImage(fs.readFileSync('./examples/mona.jpg'))
      assert.ok(im);
      assert.equal(im.width(), 500);
      assert.equal(im.height(), 756)
      assert.equal(im.empty(), false)
    }

  }


  , "CascadeClassifier": {
    topic : require('../lib/opencv')

    , "constructor" : function(cv){
      assert.ok(new cv.CascadeClassifier("./examples/haarcascade_frontalface_alt.xml"))
    }

    , "face detection": {
      topic : function(){
        var cv = require('../lib/opencv')
          , im = cv.readImage("./examples/mona.jpg")
          , cascade = new cv.CascadeClassifier("./examples/haarcascade_frontalface_alt.xml");
        
        cascade.detectMultiScale(im, this.callback, 1.1, 2, [30, 30]); 
      }

      , "finds face": function(err, faces){
          assert.isNull(err);
          assert.isArray(faces);
          assert.equal(faces.length, 1)

      }
    }

  }

}).run();