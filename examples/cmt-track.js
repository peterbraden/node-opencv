var path = require('path');
var cv = require('../lib/opencv');
 
var vid = new cv.VideoCapture(path.join(__dirname, 'files', 'motion.mov'));

vid.read(function(err, mat){
	if(err) throw err;

	var track = new cv.CMT(mat, [420,  110, 490, 170]);
	var x = 0;

	var iter = function(){
		vid.read(function(err, m2){
			x++;
			var ob = track.ctrack(m2);
			console.log('>>', x, ':', '[top-left-x: ', ob[0], ', top-left-y: ', ob[1], ', width: ', ob[2], ', height: ', ob[3], ', center-x: ', Math.round(ob[4]), ', center-y: ', Math.round(ob[5]), ']');
			m2.rectangle([ob[0], ob[1]], [ob[2], ob[3]]);
			m2.save('./out-cmttrack-' + x + '.jpg');
			if(x<241)
				iter();
		});
	}
	iter();
});
