var vows = require('vows')
  , assert = require('assert')



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
      assert.ok(p1.dot(p2));
          

    }

    , '.inside' : function(){}
  }


  , "Image" : {
    

  }
}).run();