var test = require('tape')
  , glob = require('glob')
  , exec = require('child_process').exec

module.exports = function(){

  glob.sync('./examples/*.js').forEach(function(example){
    test("Example: " + example, function(assert){
      exec('node ' + example, function(error, stdout, stderr){
        assert.error(error)
        assert.end()
      })
    })
  })

}
