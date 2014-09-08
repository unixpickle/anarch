path = require 'path'

includes = [
  'src/stdlib/h'
  'src/stdlib/hpp'
  'include'
  'dependencies/ansa/include'
  'dependencies/analloc2/include'
]

sourceDirs = [
  'src/arch/api'
  'src/util'
  'src/memory'
  'dependencies/ansa/src'
  'dependencies/analloc2/src'
]

module.exports = (finder) ->
  arch = process.env['TARGET_ARCH']
  if not (arch in ['x64'])
    throw new Error 'unknown architecture: ' + arch
  
  for source in sourceDirs
    finder.search __dirname + '/' + source
  finder.search "#{__dirname}/src/arch/" + arch
  
  return (__dirname + '/' + x for x in includes)
