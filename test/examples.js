var test = require('tape')
  , glob = require('glob')
  , exec = require('child_process').exec
  , path = require('path')

module.exports = function(){

  glob.sync('./examples/*.js').forEach(function(example){
    test("Example: " + example, function(assert){

      var fullName = path.resolve(example)
        , examples = path.resolve('./examples')

      exec('node ' + fullName, {cwd: examples}, function(error, stdout, stderr){
        assert.error(error)
        assert.end()
      })
    })
  })

}
