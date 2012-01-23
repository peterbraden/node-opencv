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

      , '.Image imports' : function(topic){
      	assert.ok(!!topic.Image)

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
      assert.ok(new cv.Matrix(1,2,0));
    }

    , "empty": function(cv){
      assert.equal(new cv.Matrix(1,2).empty(), true);
    }

  }

  , "Image" : {
    topic : require('../lib/opencv')

    , "constructor(filename)": function(cv){
      assert.ok(new cv.Image("./examples/mona.jpg"))
    }

    , "constructor(buffer)" : function(cv){
      var im = new cv.Image(fs.readFileSync('./examples/mona.jpg'))
      assert.ok(im);
      assert.equal(im.width, 500);
      assert.equal(im.height, 756)
      assert.equal(im.empty(), false)
    }

    
    , "inherits from matrix": function(cv){
      assert.equal(new cv.Image("./examples/mona.jpg").empty(), false)
    }

    , ".width / .height" : function(cv){
      assert.equal(new cv.Image("./examples/mona.jpg").width, 500)
      assert.equal(new cv.Image("./examples/mona.jpg").height, 756)
    }

    , ".ellipse": function(cv){
      assert.equal(new cv.Image("./examples/mona.jpg").ellipse(10, 10, 10, 10), undefined)  
    }
    
    , "toBuffer": function(cv){
        var buf = fs.readFileSync('./examples/mona.jpg')
          , im = new cv.Image(buf.slice(0))
          , buf0 = im.toBuffer()
        assert.ok(buf0);
        
        //assert.equal(buf.toString('base64'), buf0.toString('base64'));
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
          , im = new cv.Image("./examples/mona.jpg")
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