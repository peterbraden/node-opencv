var path = require('path'),
    cv = require('../lib/opencv');

var bg = null;

require("v8").setFlagsFromString('--expose_gc');

var gc = require("vm").runInNewContext('gc');

var do_sync = function(done){
	// When opening a file, the full path must be passed to opencv
	var vid = new cv.VideoCapture(path.join(__dirname, 'files', 'motion.mov'));
	var x = 0;

	// synchronous
	var iter = function(){
		vid.read(function(err, m2){
		  if (err) throw err;
		  var mat = bg.apply(m2);
		  delete m2;
		  // mat is a monochrome img where moving objects are white
		  if (x++<100){
		    //console.log("iter "+x);
			setTimeout(iter, 1);
		  } else {
	        delete vid;
		    console.log("bg sync done");
			gc();
			if (undefined !== done)
			  done();
		  }
		})
	};

	iter();
};

var do_async = function(done){
	// restart  video read
	var vid = new cv.VideoCapture(path.join(__dirname, 'files', 'motion.mov'));
	var x = 0;

	// asynchronous
	var iter = function(){
		vid.read(function(err, m2){
		  if (err) throw err;
		  bg.apply(m2, function(err, mat){
	        delete mat;
		    if (err) throw err;
			// mat is a monochrome img where moving objects are white
		    if (x++<100){
		      //console.log("iter "+x);
			  iter();
		    } else {
		      console.log("bg async done");
		      delete vid;
			  gc();
			  if (undefined !== done)
				done();
		    }
		  });
		})
	};

	iter();
};

var do_default = function( done ){
	console.log("doing Default");
	bg = new cv.BackgroundSubtractor();
	do_sync( function(){
		do_async(done);
	});
}

var do_gmg = function( done ){
	console.log("doing GMG");
	bg = cv.BackgroundSubtractor.createGMG();
	do_sync( function(){
		//console.log("not doing GMG Async - crashes my pi");
		do_async(done);
	});
}

var do_mog = function( done ){
	console.log("doing MOG");
	bg = cv.BackgroundSubtractor.createMOG();
	do_sync( function(){
		do_async(done);
	});
}

var do_mog2 = function( done ){
	console.log("doing MOG2");
	bg = cv.BackgroundSubtractor.createMOG2();
	do_sync( function(){
		do_async(done);
	});
}

do_default(function(){
	do_mog(function(){
		do_mog2(function(){
			do_gmg(function(){

			});
		});
	});
});

