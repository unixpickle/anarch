#!/usr/bin/env coffee --
#
# This tool exists because I hate the way clang handles wildcards with the `-c`
# flag. Specifically, this script allows you to compile multiple source files
# to multiple object files WITH A SPECIFIED OUTPUT DIRECTORY.
#

path = require 'path'
{spawn} = require 'child_process'

if process.argv.length < 4
  console.error 'Usage: build-objects <compiler> [flags ...]' +
    ' <source> [sources ...] [-o output dir]'
  process.exit 1

class CompilerCommand
  constructor: (@arguments) ->
    @outputDir = '.'
    @files = []
    if @arguments[@arguments.length - 2] is '-o'
      @outputDir = @arguments.pop()
      @arguments.pop()
    for x, i in @arguments
      continue if i is 0
      if x[0] isnt '-'
        @files = @arguments[i..]
        @arguments = @arguments[...i]
        break
  
  objectPath: (source) ->
    bn = path.basename source
    objName = bn[0...bn.length - path.extname(bn).length] + '.o'
    return path.join @outputDir, objName
  
  compileNext: ->
    return if @files.length is 0
    file = @files.pop()
    flags = @arguments[1..].concat [file, '-o', @objectPath file]
    console.log @arguments[0] + ' ' + flags.join ' '
    task = spawn @arguments[0], flags, stdio: 'inherit'
    task.on 'close', (code) =>
      process.exit code if code isnt 0
      @compileNext()

compiler = new CompilerCommand process.argv[2..]
compiler.compileNext()
