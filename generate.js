// this is used to generate node based webgl bindings
var get = require('request').get;
var fs = require('fs');

function X(type, name) {
  console.log('UNCAUGHT', type, name);
}

get('http://www.khronos.org/registry/egl/api/1.1/EGL/egl.h', function(err, res, headerString) {

  var cc = [
    '#include <node.h>',
    '#include <node_buffer.h>',
    '#include <v8.h>',
    '#include "arch_wrapper.h"',
    '',
    'using namespace v8;',
    'using namespace node;',
    '',
    ''
  ],
  init = [
    'void init(Handle<Object> target) {',
  ],
  initPost = [
    '}',
    '',
    'NODE_MODULE(gles2, init)',
  ];

  // Methods
  init.push('');
  init.push('  // Methods')
  headerString.match(/GLAPI .+ APIENTRY egl[^;]+/g).forEach(function(fn) {
      var parts = fn.replace(/[\(\),]/g, ' ')
                    .replace(/ *APIENTRY/g, '')
                    .replace(/GLAPI /g, '')
                    .replace(/ \*/g, '* ')
                    .split(' ');

      var signature = {};

      signature.returnType = parts.shift();
      if ( signature.returnType === 'const') {
         signature.returnType += ' ' + parts.shift();
      }

      var fnName;
      while (!fnName || fnName === '') {
        var part = parts.shift();

        if (part === "*") {
          signature.returnType += ' *';
        } else {
          fnName = signature.name = part;
        }
      }

      if (signature.name === 'eglGetProcAddress') {
        console.log('WARNING: extensions currently disabled.');
        return;
      }

      var upper = fnName[0].toUpperCase() + fnName.substring(1);
      signature.arguments = {};
      signature.list = [];

      while (parts.length) {
        var argType = parts.shift();
        if (argType === 'const' || argType === 'const*') {
          argType += ' ' + parts.shift();
        } else if (argType === '') {
          continue;
        } else if (parts.length === 0 && argType === 'void') {
          break;
        }


        var argName = parts.shift();
        if (argName === '') {
         continue
        } else if (argName === 'const*') {
          argType += ' ' + argName;
          argName = parts.shift();
        }

        if (!argName || !argType) {
          throw new Error(argName + ' : ' + argType + ' : ' + fn);
        }
        signature.arguments[argName] = argType;
        signature.list.push(argName);
      }


      cc.push('Handle<Value> ' + upper  + '(const Arguments& args) {');
      cc.push('  HandleScope scope;');
      cc.push('');
      var skipReturn = false, skipCall = false, beforeCall = false;
      // collect arguments
      signature.list.forEach(function(name, i) {

        var type = this.arguments[name];

        switch (type) {
          case 'EGLint':
            cc.push('  ' + type + ' ' + name + ' = args[' + i + ']->Int32Value();')
          break;

          case 'const char*':
            cc.push('  v8::String::Utf8Value string_' + name + '(args[' + i + ']);')
            cc.push('  ' + type + ' ' + name + ' = *string_' + name + ';');
          break;

          case 'const EGLint*':
            if (signature.name !== 'glShaderSource') {
              cc.push('');
              cc.push('  Handle<Array> array_' + name + ' = Handle<Array>::Cast(args[' + i + ']);');
              cc.push('  int length_' + i + ' = array_' + name + '->Get(String::New("length"))->ToObject()->Uint32Value();');
              cc.push('  ' + type.replace('*', '').replace('const ', '') + ' ' + name + '[length_' + i + '];');
              cc.push('  for (int i=0; i<length_' + i + '; i++) {');
              cc.push('    ' + name + '[i] = array_' + name + '->Get(i)->ToObject()->Uint32Value();');
              cc.push('  }');
              cc.push('');
            }
          break;

          // outgoing params

          case 'GLuint*':
            // handle the glGen* cases
            if (!this.arguments.length && (this.arguments.maxcount || this.arguments.n)) {

              var arg = this.arguments.maxcount ? 'maxcount' : 'n';
              cc.push('');
              cc.push('  Handle<Array> ret = Array::New(' + arg + ');');
              cc.push('  ' + type + ' ' + name + ' = 0;');

              var out = [''];
              out.push('  for (int i_' + i + '=0; i_' + i + ' < ' + arg + '; i_' + i + '++) {');
              out.push('    ret->Set(Number::New(i_' + i + '), Number::New(' + name + '[i_' + i + ']));');
              out.push('  }');
              out.push('  return scope.Close(ret);');

              skipReturn = out.join('\n')
            } else {
              X(type, name);
            }
          break;

          case 'EGLint*':
            cc.push('  ' + type.replace('*', '') + ' ' + name + '_base = 0;');
            cc.push('  ' + type + ' ' + name + ' = &' + name + '_base;');

            // no need to return the # of configs to js as it will be an array
            if (name !== 'num_config') {
              skipReturn = '\n  return scope.Close(Number::New(' + name + '_base));';
            }
          break;

          case 'EGLDisplay':
          case 'NativePixmapType':
          case 'NativeDisplayType':
          case 'NativeWindowType':
          case 'EGLConfig':
          case 'EGLSurface':
          case 'EGLContext':
            cc.push('  ' + type + ' ' + name + ' = External::Unwrap(args[' + i + ']);')
          break;

          case 'EGLConfig*':
            beforeCall = [
            '  ' + type + ' ' + name + ';',
            '',
            '  Handle<Array> array_' + name + ' = Array::New(*num_config);',
            '  for (int i=0; i<*num_config; i++) {',
            '    array_' + name + '->Set(i, External::New(' + name + '[i]));',
            '  }',
            ''].join('\n');

            skipReturn = '  return scope.Close(array_' + name + ');';
          break;

          default:
            X(type, name);
          break;

        }

      }.bind(signature));


        if (beforeCall) {
          cc.push(beforeCall);
        }

        switch (signature.returnType) {
/*          case 'void':
            // TODO: out args
            !skipCall && cc.push('  ' + signature.name + '(' + Object.keys(signature.arguments).join(', ') + ');');
            !skipReturn && cc.push('  return scope.Close(Undefined());');
          break;
*/
          case 'EGLint':
            !skipCall && cc.push('  ' + signature.returnType + ' ret = ' + signature.name + '(' + Object.keys(signature.arguments).join(', ') + ');');
            !skipReturn && cc.push('  return scope.Close(Number::New(ret));');
          break;

          case 'EGLBoolean':
            !skipCall && cc.push('  ' + signature.returnType + ' ret = ' + signature.name + '(' + Object.keys(signature.arguments).join(', ') + ');');
            if (skipReturn) {
              cc.push('  if (!ret) {');
              cc.push('    printf("' + signature.name + ' failed with error 0x%x", eglGetError());');
              cc.push('    assert(false);');
              cc.push('  }');
            }
            !skipReturn && cc.push('  return scope.Close(Number::New(ret));');
          break;

          case 'const char*':
          case 'const char *':
          console.log('ret const char', signature.name, skipCall, skipCall);
            !skipCall && cc.push('  ' + signature.returnType + ' ret = ' + signature.name + '(' + Object.keys(signature.arguments).join(', ') + ');');
            !skipReturn && cc.push('  return scope.Close(String::New(ret));');
          break;

          case 'EGLContext':
          case 'EGLDisplay':
          case 'EGLSurface':
            !skipCall && cc.push('  ' + signature.returnType + ' ret = ' + signature.name + '(' + Object.keys(signature.arguments).join(', ') + ');');
            !skipReturn && cc.push('  return scope.Close(External::New(ret));');
          break;

          default:
            X(signature);
            !skipReturn && cc.push('  return scope.Close(Undefined());');
          break;
        }

      skipReturn && cc.push((typeof skipReturn === 'function') ? skipReturn() : skipReturn);

      init.push('  SetMethod(target, "' + fnName + '", ' + upper + ');');
      cc.push('}');
      cc.push('');
  });


  // CONSTANTS
  init.push('');
  init.push('  // Constants')
  headerString.match(/#define (EGL_[^ \t]+).*/g).forEach(function(constant) {

    if (constant.indexOf('*/') > -1) { return; }

    constant = constant.replace(/#define */g, '')
                       .replace(/\t/g, ' ')
                       .replace(/ +/, ' ')
                       .replace(/[ \t]*\/\*.*\*\/[ ]*/, '')
                       .replace(/ /,'", ')
                       .replace(/\(\([^\)]+\)([^\)])\)/, "$1");

    init.push('  DEFINE_CONSTANT(target, "' + constant + ');');
  });
  init.push('');

  var out = cc.join('\n') + init.join('\n') + initPost.join('\n') + '\n';

  fs.writeFile(__dirname + '/src/egl.cc', out, function() {});
});
