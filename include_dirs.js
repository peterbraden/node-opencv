// Outputs the path to public node-opencv header files.  This is used when
// building other native node modules that require access to the
// node-opencv Matrix base class.
//
// To use this file, add something like the following to your binding.gyp:
//
//      "include_dirs": [
//        "<!@(pkg-config --cflags opencv)",
//        "<!(node -e \"require('nan')\")",
//        "<!(node -e \"require('opencv/include_dirs')\")"
//      ],
//
var path = require('path');
console.log(path.relative('.', path.join(__dirname, 'inc')));
