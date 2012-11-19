var vows = require('vows')
  , assert = require('assert')
  , fs = require('fs');

assertDeepSimilar = function(res, exp){
  for (var i = 0; i < res.length; i++){
 //   res[i] = Math.round(res[i]/100)*100;  
  }
  assert.deepEqual(res, exp)
}

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

      , 'importing library multiple times is ok' : function(){
        var cv1 = require('../lib/opencv')
          , cv2 = require('../lib/opencv')
          cv1.readImage('./examples/mona.png', function(){});
          cv2.readImage('./examples/mona.png', function(){});
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

    , "set/row" : function(cv){
      var mat = new cv.Matrix(1, 2);
      mat.set(0,0,3)
      mat.set(0,1,5000)
      assert.deepEqual(mat.row(0), [3,5000]);
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


    , "resize" : function(cv){
      var mat = new cv.Matrix(6,7);
      assert.equal(mat.width(), 7);
      mat.resize(8,9);
      assert.equal(mat.width(), 8);

    }

    , 'row' : function(cv){
      var mat = new cv.Matrix.Eye(4,4)
      assertDeepSimilar(mat.row(1), [0,1,0,0])
      assertDeepSimilar(mat.row(2), [0,0,1,0])
    }

    , 'col' : function(cv){
      var mat = new cv.Matrix.Eye(4,4);
      assertDeepSimilar(mat.col(1), [0,1,0,0])
      assertDeepSimilar(mat.col(2), [0,0,1,0])
    }

    , "empty": function(cv){
      assert.equal(new cv.Matrix().empty(), true);
    }

    , "toBuffer": function(cv){
        var buf = fs.readFileSync('./examples/mona.png')
        
        cv.readImage(buf.slice(0), function(err, mat){
          var buf0 = mat.toBuffer()
        
          assert.ok(buf0);
        //assert.equal(buf.toString('base64'), buf0.toString('base64'));
        })

    }
    
    , "toBuffer Async": {
      topic: function(cv){
          var buf = fs.readFileSync('./examples/mona.png')
            , cb = this.callback
          cv.readImage(buf.slice(0), function(err, mat){
            var buff = mat.toBuffer(function(){
              cb.apply(this, arguments)
            })
          })
        }
    ,  'gives a buffer' : function(e, res){
        assert.ok(!e)
        assert.ok(res);
        assert.ok(res.length > 100);
      }
    }

    , "detectObject": {
      
     topic : function(){
        var cv = require('../lib/opencv')
          , cb = this.callback

        cv.readImage("./examples/mona.png", function(err, im){
          im.detectObject("./data/haarcascade_frontalface_alt.xml", {}, cb)
        })  
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
      cv.readImage("./examples/mona.png", function(err, im){
        assert.ok(im);
        assert.equal(im.width(), 500);
        assert.equal(im.height(), 756)
        assert.equal(im.empty(), false)      
      })
    }

    , ".readImage from buffer" : function(cv){
      cv.readImage(fs.readFileSync('./examples/mona.png'), function(err, im){
        assert.ok(im);
        assert.equal(im.width(), 500);
        assert.equal(im.height(), 756)
        assert.equal(im.empty(), false)      
      })

    }

  }


  , "CascadeClassifier": {
    topic : require('../lib/opencv')

    , "constructor" : function(cv){
      assert.ok(new cv.CascadeClassifier("./data/haarcascade_frontalface_alt.xml"))
    }

    , "face detection": {
      topic : function(){
        var cv = require('../lib/opencv')
          , self = this
        
        cv.readImage("./examples/mona.png", function(err, im){
          cascade = new cv.CascadeClassifier("./data/haarcascade_frontalface_alt.xml");
          cascade.detectMultiScale(im, self.callback)//, 1.1, 2, [30, 30]);          
        })

      }

      , "finds face": function(err, faces){
          assert.isNull(err);
          assert.isArray(faces);
          assert.equal(faces.length, 1)

      }
    }

  }


  , "ImageStream" : {
    topic : require('../lib/opencv')

    , "pipe" : {
      topic : function(cv){
        var s = new cv.ImageStream()
          , self = this
        s.on('load', function(im){ 
          assert.ok(im)
          assert.equal(im.empty(), false);
          self.callback()
        }) 
        fs.createReadStream('./examples/mona.png').pipe(s);
      }

      , "loaded" : function(im){
        //assert.ok(im)
        //assert.equal(im.empty(), false);
      }
    }


  }



}).export(module);
