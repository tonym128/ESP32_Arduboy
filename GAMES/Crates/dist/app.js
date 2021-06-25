(function e(t,n,r){function s(o,u){if(!n[o]){if(!t[o]){var a=typeof require=="function"&&require;if(!u&&a)return a(o,!0);if(i)return i(o,!0);var f=new Error("Cannot find module '"+o+"'");throw f.code="MODULE_NOT_FOUND",f}var l=n[o]={exports:{}};t[o][0].call(l.exports,function(e){var n=t[o][1][e];return s(n?n:e)},l,l.exports,e,t,n,r)}return n[o].exports}var i=typeof require=="function"&&require;for(var o=0;o<r.length;o++)s(r[o]);return s})({1:[function(require,module,exports){
'use strict'

exports.byteLength = byteLength
exports.toByteArray = toByteArray
exports.fromByteArray = fromByteArray

var lookup = []
var revLookup = []
var Arr = typeof Uint8Array !== 'undefined' ? Uint8Array : Array

var code = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
for (var i = 0, len = code.length; i < len; ++i) {
  lookup[i] = code[i]
  revLookup[code.charCodeAt(i)] = i
}

revLookup['-'.charCodeAt(0)] = 62
revLookup['_'.charCodeAt(0)] = 63

function placeHoldersCount (b64) {
  var len = b64.length
  if (len % 4 > 0) {
    throw new Error('Invalid string. Length must be a multiple of 4')
  }

  // the number of equal signs (place holders)
  // if there are two placeholders, than the two characters before it
  // represent one byte
  // if there is only one, then the three characters before it represent 2 bytes
  // this is just a cheap hack to not do indexOf twice
  return b64[len - 2] === '=' ? 2 : b64[len - 1] === '=' ? 1 : 0
}

function byteLength (b64) {
  // base64 is 4/3 + up to two characters of the original data
  return (b64.length * 3 / 4) - placeHoldersCount(b64)
}

function toByteArray (b64) {
  var i, l, tmp, placeHolders, arr
  var len = b64.length
  placeHolders = placeHoldersCount(b64)

  arr = new Arr((len * 3 / 4) - placeHolders)

  // if there are placeholders, only get up to the last complete 4 chars
  l = placeHolders > 0 ? len - 4 : len

  var L = 0

  for (i = 0; i < l; i += 4) {
    tmp = (revLookup[b64.charCodeAt(i)] << 18) | (revLookup[b64.charCodeAt(i + 1)] << 12) | (revLookup[b64.charCodeAt(i + 2)] << 6) | revLookup[b64.charCodeAt(i + 3)]
    arr[L++] = (tmp >> 16) & 0xFF
    arr[L++] = (tmp >> 8) & 0xFF
    arr[L++] = tmp & 0xFF
  }

  if (placeHolders === 2) {
    tmp = (revLookup[b64.charCodeAt(i)] << 2) | (revLookup[b64.charCodeAt(i + 1)] >> 4)
    arr[L++] = tmp & 0xFF
  } else if (placeHolders === 1) {
    tmp = (revLookup[b64.charCodeAt(i)] << 10) | (revLookup[b64.charCodeAt(i + 1)] << 4) | (revLookup[b64.charCodeAt(i + 2)] >> 2)
    arr[L++] = (tmp >> 8) & 0xFF
    arr[L++] = tmp & 0xFF
  }

  return arr
}

function tripletToBase64 (num) {
  return lookup[num >> 18 & 0x3F] + lookup[num >> 12 & 0x3F] + lookup[num >> 6 & 0x3F] + lookup[num & 0x3F]
}

function encodeChunk (uint8, start, end) {
  var tmp
  var output = []
  for (var i = start; i < end; i += 3) {
    tmp = (uint8[i] << 16) + (uint8[i + 1] << 8) + (uint8[i + 2])
    output.push(tripletToBase64(tmp))
  }
  return output.join('')
}

function fromByteArray (uint8) {
  var tmp
  var len = uint8.length
  var extraBytes = len % 3 // if we have 1 byte left, pad 2 bytes
  var output = ''
  var parts = []
  var maxChunkLength = 16383 // must be multiple of 3

  // go through the array every three bytes, we'll deal with trailing stuff later
  for (var i = 0, len2 = len - extraBytes; i < len2; i += maxChunkLength) {
    parts.push(encodeChunk(uint8, i, (i + maxChunkLength) > len2 ? len2 : (i + maxChunkLength)))
  }

  // pad the end with zeros, but make sure to not forget the extra bytes
  if (extraBytes === 1) {
    tmp = uint8[len - 1]
    output += lookup[tmp >> 2]
    output += lookup[(tmp << 4) & 0x3F]
    output += '=='
  } else if (extraBytes === 2) {
    tmp = (uint8[len - 2] << 8) + (uint8[len - 1])
    output += lookup[tmp >> 10]
    output += lookup[(tmp >> 4) & 0x3F]
    output += lookup[(tmp << 2) & 0x3F]
    output += '='
  }

  parts.push(output)

  return parts.join('')
}

},{}],2:[function(require,module,exports){
/*!
 * The buffer module from node.js, for the browser.
 *
 * @author   Feross Aboukhadijeh <https://feross.org>
 * @license  MIT
 */
/* eslint-disable no-proto */

'use strict'

var base64 = require('base64-js')
var ieee754 = require('ieee754')

exports.Buffer = Buffer
exports.SlowBuffer = SlowBuffer
exports.INSPECT_MAX_BYTES = 50

var K_MAX_LENGTH = 0x7fffffff
exports.kMaxLength = K_MAX_LENGTH

/**
 * If `Buffer.TYPED_ARRAY_SUPPORT`:
 *   === true    Use Uint8Array implementation (fastest)
 *   === false   Print warning and recommend using `buffer` v4.x which has an Object
 *               implementation (most compatible, even IE6)
 *
 * Browsers that support typed arrays are IE 10+, Firefox 4+, Chrome 7+, Safari 5.1+,
 * Opera 11.6+, iOS 4.2+.
 *
 * We report that the browser does not support typed arrays if the are not subclassable
 * using __proto__. Firefox 4-29 lacks support for adding new properties to `Uint8Array`
 * (See: https://bugzilla.mozilla.org/show_bug.cgi?id=695438). IE 10 lacks support
 * for __proto__ and has a buggy typed array implementation.
 */
Buffer.TYPED_ARRAY_SUPPORT = typedArraySupport()

if (!Buffer.TYPED_ARRAY_SUPPORT && typeof console !== 'undefined' &&
    typeof console.error === 'function') {
  console.error(
    'This browser lacks typed array (Uint8Array) support which is required by ' +
    '`buffer` v5.x. Use `buffer` v4.x if you require old browser support.'
  )
}

function typedArraySupport () {
  // Can typed array instances can be augmented?
  try {
    var arr = new Uint8Array(1)
    arr.__proto__ = {__proto__: Uint8Array.prototype, foo: function () { return 42 }}
    return arr.foo() === 42
  } catch (e) {
    return false
  }
}

function createBuffer (length) {
  if (length > K_MAX_LENGTH) {
    throw new RangeError('Invalid typed array length')
  }
  // Return an augmented `Uint8Array` instance
  var buf = new Uint8Array(length)
  buf.__proto__ = Buffer.prototype
  return buf
}

/**
 * The Buffer constructor returns instances of `Uint8Array` that have their
 * prototype changed to `Buffer.prototype`. Furthermore, `Buffer` is a subclass of
 * `Uint8Array`, so the returned instances will have all the node `Buffer` methods
 * and the `Uint8Array` methods. Square bracket notation works as expected -- it
 * returns a single octet.
 *
 * The `Uint8Array` prototype remains unmodified.
 */

function Buffer (arg, encodingOrOffset, length) {
  // Common case.
  if (typeof arg === 'number') {
    if (typeof encodingOrOffset === 'string') {
      throw new Error(
        'If encoding is specified then the first argument must be a string'
      )
    }
    return allocUnsafe(arg)
  }
  return from(arg, encodingOrOffset, length)
}

// Fix subarray() in ES2016. See: https://github.com/feross/buffer/pull/97
if (typeof Symbol !== 'undefined' && Symbol.species &&
    Buffer[Symbol.species] === Buffer) {
  Object.defineProperty(Buffer, Symbol.species, {
    value: null,
    configurable: true,
    enumerable: false,
    writable: false
  })
}

Buffer.poolSize = 8192 // not used by this implementation

function from (value, encodingOrOffset, length) {
  if (typeof value === 'number') {
    throw new TypeError('"value" argument must not be a number')
  }

  if (isArrayBuffer(value)) {
    return fromArrayBuffer(value, encodingOrOffset, length)
  }

  if (typeof value === 'string') {
    return fromString(value, encodingOrOffset)
  }

  return fromObject(value)
}

/**
 * Functionally equivalent to Buffer(arg, encoding) but throws a TypeError
 * if value is a number.
 * Buffer.from(str[, encoding])
 * Buffer.from(array)
 * Buffer.from(buffer)
 * Buffer.from(arrayBuffer[, byteOffset[, length]])
 **/
Buffer.from = function (value, encodingOrOffset, length) {
  return from(value, encodingOrOffset, length)
}

// Note: Change prototype *after* Buffer.from is defined to workaround Chrome bug:
// https://github.com/feross/buffer/pull/148
Buffer.prototype.__proto__ = Uint8Array.prototype
Buffer.__proto__ = Uint8Array

function assertSize (size) {
  if (typeof size !== 'number') {
    throw new TypeError('"size" argument must be a number')
  } else if (size < 0) {
    throw new RangeError('"size" argument must not be negative')
  }
}

function alloc (size, fill, encoding) {
  assertSize(size)
  if (size <= 0) {
    return createBuffer(size)
  }
  if (fill !== undefined) {
    // Only pay attention to encoding if it's a string. This
    // prevents accidentally sending in a number that would
    // be interpretted as a start offset.
    return typeof encoding === 'string'
      ? createBuffer(size).fill(fill, encoding)
      : createBuffer(size).fill(fill)
  }
  return createBuffer(size)
}

/**
 * Creates a new filled Buffer instance.
 * alloc(size[, fill[, encoding]])
 **/
Buffer.alloc = function (size, fill, encoding) {
  return alloc(size, fill, encoding)
}

function allocUnsafe (size) {
  assertSize(size)
  return createBuffer(size < 0 ? 0 : checked(size) | 0)
}

/**
 * Equivalent to Buffer(num), by default creates a non-zero-filled Buffer instance.
 * */
Buffer.allocUnsafe = function (size) {
  return allocUnsafe(size)
}
/**
 * Equivalent to SlowBuffer(num), by default creates a non-zero-filled Buffer instance.
 */
Buffer.allocUnsafeSlow = function (size) {
  return allocUnsafe(size)
}

function fromString (string, encoding) {
  if (typeof encoding !== 'string' || encoding === '') {
    encoding = 'utf8'
  }

  if (!Buffer.isEncoding(encoding)) {
    throw new TypeError('"encoding" must be a valid string encoding')
  }

  var length = byteLength(string, encoding) | 0
  var buf = createBuffer(length)

  var actual = buf.write(string, encoding)

  if (actual !== length) {
    // Writing a hex string, for example, that contains invalid characters will
    // cause everything after the first invalid character to be ignored. (e.g.
    // 'abxxcd' will be treated as 'ab')
    buf = buf.slice(0, actual)
  }

  return buf
}

function fromArrayLike (array) {
  var length = array.length < 0 ? 0 : checked(array.length) | 0
  var buf = createBuffer(length)
  for (var i = 0; i < length; i += 1) {
    buf[i] = array[i] & 255
  }
  return buf
}

function fromArrayBuffer (array, byteOffset, length) {
  if (byteOffset < 0 || array.byteLength < byteOffset) {
    throw new RangeError('\'offset\' is out of bounds')
  }

  if (array.byteLength < byteOffset + (length || 0)) {
    throw new RangeError('\'length\' is out of bounds')
  }

  var buf
  if (byteOffset === undefined && length === undefined) {
    buf = new Uint8Array(array)
  } else if (length === undefined) {
    buf = new Uint8Array(array, byteOffset)
  } else {
    buf = new Uint8Array(array, byteOffset, length)
  }

  // Return an augmented `Uint8Array` instance
  buf.__proto__ = Buffer.prototype
  return buf
}

function fromObject (obj) {
  if (Buffer.isBuffer(obj)) {
    var len = checked(obj.length) | 0
    var buf = createBuffer(len)

    if (buf.length === 0) {
      return buf
    }

    obj.copy(buf, 0, 0, len)
    return buf
  }

  if (obj) {
    if (isArrayBufferView(obj) || 'length' in obj) {
      if (typeof obj.length !== 'number' || numberIsNaN(obj.length)) {
        return createBuffer(0)
      }
      return fromArrayLike(obj)
    }

    if (obj.type === 'Buffer' && Array.isArray(obj.data)) {
      return fromArrayLike(obj.data)
    }
  }

  throw new TypeError('First argument must be a string, Buffer, ArrayBuffer, Array, or array-like object.')
}

function checked (length) {
  // Note: cannot use `length < K_MAX_LENGTH` here because that fails when
  // length is NaN (which is otherwise coerced to zero.)
  if (length >= K_MAX_LENGTH) {
    throw new RangeError('Attempt to allocate Buffer larger than maximum ' +
                         'size: 0x' + K_MAX_LENGTH.toString(16) + ' bytes')
  }
  return length | 0
}

function SlowBuffer (length) {
  if (+length != length) { // eslint-disable-line eqeqeq
    length = 0
  }
  return Buffer.alloc(+length)
}

Buffer.isBuffer = function isBuffer (b) {
  return b != null && b._isBuffer === true
}

Buffer.compare = function compare (a, b) {
  if (!Buffer.isBuffer(a) || !Buffer.isBuffer(b)) {
    throw new TypeError('Arguments must be Buffers')
  }

  if (a === b) return 0

  var x = a.length
  var y = b.length

  for (var i = 0, len = Math.min(x, y); i < len; ++i) {
    if (a[i] !== b[i]) {
      x = a[i]
      y = b[i]
      break
    }
  }

  if (x < y) return -1
  if (y < x) return 1
  return 0
}

Buffer.isEncoding = function isEncoding (encoding) {
  switch (String(encoding).toLowerCase()) {
    case 'hex':
    case 'utf8':
    case 'utf-8':
    case 'ascii':
    case 'latin1':
    case 'binary':
    case 'base64':
    case 'ucs2':
    case 'ucs-2':
    case 'utf16le':
    case 'utf-16le':
      return true
    default:
      return false
  }
}

Buffer.concat = function concat (list, length) {
  if (!Array.isArray(list)) {
    throw new TypeError('"list" argument must be an Array of Buffers')
  }

  if (list.length === 0) {
    return Buffer.alloc(0)
  }

  var i
  if (length === undefined) {
    length = 0
    for (i = 0; i < list.length; ++i) {
      length += list[i].length
    }
  }

  var buffer = Buffer.allocUnsafe(length)
  var pos = 0
  for (i = 0; i < list.length; ++i) {
    var buf = list[i]
    if (!Buffer.isBuffer(buf)) {
      throw new TypeError('"list" argument must be an Array of Buffers')
    }
    buf.copy(buffer, pos)
    pos += buf.length
  }
  return buffer
}

function byteLength (string, encoding) {
  if (Buffer.isBuffer(string)) {
    return string.length
  }
  if (isArrayBufferView(string) || isArrayBuffer(string)) {
    return string.byteLength
  }
  if (typeof string !== 'string') {
    string = '' + string
  }

  var len = string.length
  if (len === 0) return 0

  // Use a for loop to avoid recursion
  var loweredCase = false
  for (;;) {
    switch (encoding) {
      case 'ascii':
      case 'latin1':
      case 'binary':
        return len
      case 'utf8':
      case 'utf-8':
      case undefined:
        return utf8ToBytes(string).length
      case 'ucs2':
      case 'ucs-2':
      case 'utf16le':
      case 'utf-16le':
        return len * 2
      case 'hex':
        return len >>> 1
      case 'base64':
        return base64ToBytes(string).length
      default:
        if (loweredCase) return utf8ToBytes(string).length // assume utf8
        encoding = ('' + encoding).toLowerCase()
        loweredCase = true
    }
  }
}
Buffer.byteLength = byteLength

function slowToString (encoding, start, end) {
  var loweredCase = false

  // No need to verify that "this.length <= MAX_UINT32" since it's a read-only
  // property of a typed array.

  // This behaves neither like String nor Uint8Array in that we set start/end
  // to their upper/lower bounds if the value passed is out of range.
  // undefined is handled specially as per ECMA-262 6th Edition,
  // Section 13.3.3.7 Runtime Semantics: KeyedBindingInitialization.
  if (start === undefined || start < 0) {
    start = 0
  }
  // Return early if start > this.length. Done here to prevent potential uint32
  // coercion fail below.
  if (start > this.length) {
    return ''
  }

  if (end === undefined || end > this.length) {
    end = this.length
  }

  if (end <= 0) {
    return ''
  }

  // Force coersion to uint32. This will also coerce falsey/NaN values to 0.
  end >>>= 0
  start >>>= 0

  if (end <= start) {
    return ''
  }

  if (!encoding) encoding = 'utf8'

  while (true) {
    switch (encoding) {
      case 'hex':
        return hexSlice(this, start, end)

      case 'utf8':
      case 'utf-8':
        return utf8Slice(this, start, end)

      case 'ascii':
        return asciiSlice(this, start, end)

      case 'latin1':
      case 'binary':
        return latin1Slice(this, start, end)

      case 'base64':
        return base64Slice(this, start, end)

      case 'ucs2':
      case 'ucs-2':
      case 'utf16le':
      case 'utf-16le':
        return utf16leSlice(this, start, end)

      default:
        if (loweredCase) throw new TypeError('Unknown encoding: ' + encoding)
        encoding = (encoding + '').toLowerCase()
        loweredCase = true
    }
  }
}

// This property is used by `Buffer.isBuffer` (and the `is-buffer` npm package)
// to detect a Buffer instance. It's not possible to use `instanceof Buffer`
// reliably in a browserify context because there could be multiple different
// copies of the 'buffer' package in use. This method works even for Buffer
// instances that were created from another copy of the `buffer` package.
// See: https://github.com/feross/buffer/issues/154
Buffer.prototype._isBuffer = true

function swap (b, n, m) {
  var i = b[n]
  b[n] = b[m]
  b[m] = i
}

Buffer.prototype.swap16 = function swap16 () {
  var len = this.length
  if (len % 2 !== 0) {
    throw new RangeError('Buffer size must be a multiple of 16-bits')
  }
  for (var i = 0; i < len; i += 2) {
    swap(this, i, i + 1)
  }
  return this
}

Buffer.prototype.swap32 = function swap32 () {
  var len = this.length
  if (len % 4 !== 0) {
    throw new RangeError('Buffer size must be a multiple of 32-bits')
  }
  for (var i = 0; i < len; i += 4) {
    swap(this, i, i + 3)
    swap(this, i + 1, i + 2)
  }
  return this
}

Buffer.prototype.swap64 = function swap64 () {
  var len = this.length
  if (len % 8 !== 0) {
    throw new RangeError('Buffer size must be a multiple of 64-bits')
  }
  for (var i = 0; i < len; i += 8) {
    swap(this, i, i + 7)
    swap(this, i + 1, i + 6)
    swap(this, i + 2, i + 5)
    swap(this, i + 3, i + 4)
  }
  return this
}

Buffer.prototype.toString = function toString () {
  var length = this.length
  if (length === 0) return ''
  if (arguments.length === 0) return utf8Slice(this, 0, length)
  return slowToString.apply(this, arguments)
}

Buffer.prototype.equals = function equals (b) {
  if (!Buffer.isBuffer(b)) throw new TypeError('Argument must be a Buffer')
  if (this === b) return true
  return Buffer.compare(this, b) === 0
}

Buffer.prototype.inspect = function inspect () {
  var str = ''
  var max = exports.INSPECT_MAX_BYTES
  if (this.length > 0) {
    str = this.toString('hex', 0, max).match(/.{2}/g).join(' ')
    if (this.length > max) str += ' ... '
  }
  return '<Buffer ' + str + '>'
}

Buffer.prototype.compare = function compare (target, start, end, thisStart, thisEnd) {
  if (!Buffer.isBuffer(target)) {
    throw new TypeError('Argument must be a Buffer')
  }

  if (start === undefined) {
    start = 0
  }
  if (end === undefined) {
    end = target ? target.length : 0
  }
  if (thisStart === undefined) {
    thisStart = 0
  }
  if (thisEnd === undefined) {
    thisEnd = this.length
  }

  if (start < 0 || end > target.length || thisStart < 0 || thisEnd > this.length) {
    throw new RangeError('out of range index')
  }

  if (thisStart >= thisEnd && start >= end) {
    return 0
  }
  if (thisStart >= thisEnd) {
    return -1
  }
  if (start >= end) {
    return 1
  }

  start >>>= 0
  end >>>= 0
  thisStart >>>= 0
  thisEnd >>>= 0

  if (this === target) return 0

  var x = thisEnd - thisStart
  var y = end - start
  var len = Math.min(x, y)

  var thisCopy = this.slice(thisStart, thisEnd)
  var targetCopy = target.slice(start, end)

  for (var i = 0; i < len; ++i) {
    if (thisCopy[i] !== targetCopy[i]) {
      x = thisCopy[i]
      y = targetCopy[i]
      break
    }
  }

  if (x < y) return -1
  if (y < x) return 1
  return 0
}

// Finds either the first index of `val` in `buffer` at offset >= `byteOffset`,
// OR the last index of `val` in `buffer` at offset <= `byteOffset`.
//
// Arguments:
// - buffer - a Buffer to search
// - val - a string, Buffer, or number
// - byteOffset - an index into `buffer`; will be clamped to an int32
// - encoding - an optional encoding, relevant is val is a string
// - dir - true for indexOf, false for lastIndexOf
function bidirectionalIndexOf (buffer, val, byteOffset, encoding, dir) {
  // Empty buffer means no match
  if (buffer.length === 0) return -1

  // Normalize byteOffset
  if (typeof byteOffset === 'string') {
    encoding = byteOffset
    byteOffset = 0
  } else if (byteOffset > 0x7fffffff) {
    byteOffset = 0x7fffffff
  } else if (byteOffset < -0x80000000) {
    byteOffset = -0x80000000
  }
  byteOffset = +byteOffset  // Coerce to Number.
  if (numberIsNaN(byteOffset)) {
    // byteOffset: it it's undefined, null, NaN, "foo", etc, search whole buffer
    byteOffset = dir ? 0 : (buffer.length - 1)
  }

  // Normalize byteOffset: negative offsets start from the end of the buffer
  if (byteOffset < 0) byteOffset = buffer.length + byteOffset
  if (byteOffset >= buffer.length) {
    if (dir) return -1
    else byteOffset = buffer.length - 1
  } else if (byteOffset < 0) {
    if (dir) byteOffset = 0
    else return -1
  }

  // Normalize val
  if (typeof val === 'string') {
    val = Buffer.from(val, encoding)
  }

  // Finally, search either indexOf (if dir is true) or lastIndexOf
  if (Buffer.isBuffer(val)) {
    // Special case: looking for empty string/buffer always fails
    if (val.length === 0) {
      return -1
    }
    return arrayIndexOf(buffer, val, byteOffset, encoding, dir)
  } else if (typeof val === 'number') {
    val = val & 0xFF // Search for a byte value [0-255]
    if (typeof Uint8Array.prototype.indexOf === 'function') {
      if (dir) {
        return Uint8Array.prototype.indexOf.call(buffer, val, byteOffset)
      } else {
        return Uint8Array.prototype.lastIndexOf.call(buffer, val, byteOffset)
      }
    }
    return arrayIndexOf(buffer, [ val ], byteOffset, encoding, dir)
  }

  throw new TypeError('val must be string, number or Buffer')
}

function arrayIndexOf (arr, val, byteOffset, encoding, dir) {
  var indexSize = 1
  var arrLength = arr.length
  var valLength = val.length

  if (encoding !== undefined) {
    encoding = String(encoding).toLowerCase()
    if (encoding === 'ucs2' || encoding === 'ucs-2' ||
        encoding === 'utf16le' || encoding === 'utf-16le') {
      if (arr.length < 2 || val.length < 2) {
        return -1
      }
      indexSize = 2
      arrLength /= 2
      valLength /= 2
      byteOffset /= 2
    }
  }

  function read (buf, i) {
    if (indexSize === 1) {
      return buf[i]
    } else {
      return buf.readUInt16BE(i * indexSize)
    }
  }

  var i
  if (dir) {
    var foundIndex = -1
    for (i = byteOffset; i < arrLength; i++) {
      if (read(arr, i) === read(val, foundIndex === -1 ? 0 : i - foundIndex)) {
        if (foundIndex === -1) foundIndex = i
        if (i - foundIndex + 1 === valLength) return foundIndex * indexSize
      } else {
        if (foundIndex !== -1) i -= i - foundIndex
        foundIndex = -1
      }
    }
  } else {
    if (byteOffset + valLength > arrLength) byteOffset = arrLength - valLength
    for (i = byteOffset; i >= 0; i--) {
      var found = true
      for (var j = 0; j < valLength; j++) {
        if (read(arr, i + j) !== read(val, j)) {
          found = false
          break
        }
      }
      if (found) return i
    }
  }

  return -1
}

Buffer.prototype.includes = function includes (val, byteOffset, encoding) {
  return this.indexOf(val, byteOffset, encoding) !== -1
}

Buffer.prototype.indexOf = function indexOf (val, byteOffset, encoding) {
  return bidirectionalIndexOf(this, val, byteOffset, encoding, true)
}

Buffer.prototype.lastIndexOf = function lastIndexOf (val, byteOffset, encoding) {
  return bidirectionalIndexOf(this, val, byteOffset, encoding, false)
}

function hexWrite (buf, string, offset, length) {
  offset = Number(offset) || 0
  var remaining = buf.length - offset
  if (!length) {
    length = remaining
  } else {
    length = Number(length)
    if (length > remaining) {
      length = remaining
    }
  }

  // must be an even number of digits
  var strLen = string.length
  if (strLen % 2 !== 0) throw new TypeError('Invalid hex string')

  if (length > strLen / 2) {
    length = strLen / 2
  }
  for (var i = 0; i < length; ++i) {
    var parsed = parseInt(string.substr(i * 2, 2), 16)
    if (numberIsNaN(parsed)) return i
    buf[offset + i] = parsed
  }
  return i
}

function utf8Write (buf, string, offset, length) {
  return blitBuffer(utf8ToBytes(string, buf.length - offset), buf, offset, length)
}

function asciiWrite (buf, string, offset, length) {
  return blitBuffer(asciiToBytes(string), buf, offset, length)
}

function latin1Write (buf, string, offset, length) {
  return asciiWrite(buf, string, offset, length)
}

function base64Write (buf, string, offset, length) {
  return blitBuffer(base64ToBytes(string), buf, offset, length)
}

function ucs2Write (buf, string, offset, length) {
  return blitBuffer(utf16leToBytes(string, buf.length - offset), buf, offset, length)
}

Buffer.prototype.write = function write (string, offset, length, encoding) {
  // Buffer#write(string)
  if (offset === undefined) {
    encoding = 'utf8'
    length = this.length
    offset = 0
  // Buffer#write(string, encoding)
  } else if (length === undefined && typeof offset === 'string') {
    encoding = offset
    length = this.length
    offset = 0
  // Buffer#write(string, offset[, length][, encoding])
  } else if (isFinite(offset)) {
    offset = offset >>> 0
    if (isFinite(length)) {
      length = length >>> 0
      if (encoding === undefined) encoding = 'utf8'
    } else {
      encoding = length
      length = undefined
    }
  } else {
    throw new Error(
      'Buffer.write(string, encoding, offset[, length]) is no longer supported'
    )
  }

  var remaining = this.length - offset
  if (length === undefined || length > remaining) length = remaining

  if ((string.length > 0 && (length < 0 || offset < 0)) || offset > this.length) {
    throw new RangeError('Attempt to write outside buffer bounds')
  }

  if (!encoding) encoding = 'utf8'

  var loweredCase = false
  for (;;) {
    switch (encoding) {
      case 'hex':
        return hexWrite(this, string, offset, length)

      case 'utf8':
      case 'utf-8':
        return utf8Write(this, string, offset, length)

      case 'ascii':
        return asciiWrite(this, string, offset, length)

      case 'latin1':
      case 'binary':
        return latin1Write(this, string, offset, length)

      case 'base64':
        // Warning: maxLength not taken into account in base64Write
        return base64Write(this, string, offset, length)

      case 'ucs2':
      case 'ucs-2':
      case 'utf16le':
      case 'utf-16le':
        return ucs2Write(this, string, offset, length)

      default:
        if (loweredCase) throw new TypeError('Unknown encoding: ' + encoding)
        encoding = ('' + encoding).toLowerCase()
        loweredCase = true
    }
  }
}

Buffer.prototype.toJSON = function toJSON () {
  return {
    type: 'Buffer',
    data: Array.prototype.slice.call(this._arr || this, 0)
  }
}

function base64Slice (buf, start, end) {
  if (start === 0 && end === buf.length) {
    return base64.fromByteArray(buf)
  } else {
    return base64.fromByteArray(buf.slice(start, end))
  }
}

function utf8Slice (buf, start, end) {
  end = Math.min(buf.length, end)
  var res = []

  var i = start
  while (i < end) {
    var firstByte = buf[i]
    var codePoint = null
    var bytesPerSequence = (firstByte > 0xEF) ? 4
      : (firstByte > 0xDF) ? 3
      : (firstByte > 0xBF) ? 2
      : 1

    if (i + bytesPerSequence <= end) {
      var secondByte, thirdByte, fourthByte, tempCodePoint

      switch (bytesPerSequence) {
        case 1:
          if (firstByte < 0x80) {
            codePoint = firstByte
          }
          break
        case 2:
          secondByte = buf[i + 1]
          if ((secondByte & 0xC0) === 0x80) {
            tempCodePoint = (firstByte & 0x1F) << 0x6 | (secondByte & 0x3F)
            if (tempCodePoint > 0x7F) {
              codePoint = tempCodePoint
            }
          }
          break
        case 3:
          secondByte = buf[i + 1]
          thirdByte = buf[i + 2]
          if ((secondByte & 0xC0) === 0x80 && (thirdByte & 0xC0) === 0x80) {
            tempCodePoint = (firstByte & 0xF) << 0xC | (secondByte & 0x3F) << 0x6 | (thirdByte & 0x3F)
            if (tempCodePoint > 0x7FF && (tempCodePoint < 0xD800 || tempCodePoint > 0xDFFF)) {
              codePoint = tempCodePoint
            }
          }
          break
        case 4:
          secondByte = buf[i + 1]
          thirdByte = buf[i + 2]
          fourthByte = buf[i + 3]
          if ((secondByte & 0xC0) === 0x80 && (thirdByte & 0xC0) === 0x80 && (fourthByte & 0xC0) === 0x80) {
            tempCodePoint = (firstByte & 0xF) << 0x12 | (secondByte & 0x3F) << 0xC | (thirdByte & 0x3F) << 0x6 | (fourthByte & 0x3F)
            if (tempCodePoint > 0xFFFF && tempCodePoint < 0x110000) {
              codePoint = tempCodePoint
            }
          }
      }
    }

    if (codePoint === null) {
      // we did not generate a valid codePoint so insert a
      // replacement char (U+FFFD) and advance only 1 byte
      codePoint = 0xFFFD
      bytesPerSequence = 1
    } else if (codePoint > 0xFFFF) {
      // encode to utf16 (surrogate pair dance)
      codePoint -= 0x10000
      res.push(codePoint >>> 10 & 0x3FF | 0xD800)
      codePoint = 0xDC00 | codePoint & 0x3FF
    }

    res.push(codePoint)
    i += bytesPerSequence
  }

  return decodeCodePointsArray(res)
}

// Based on http://stackoverflow.com/a/22747272/680742, the browser with
// the lowest limit is Chrome, with 0x10000 args.
// We go 1 magnitude less, for safety
var MAX_ARGUMENTS_LENGTH = 0x1000

function decodeCodePointsArray (codePoints) {
  var len = codePoints.length
  if (len <= MAX_ARGUMENTS_LENGTH) {
    return String.fromCharCode.apply(String, codePoints) // avoid extra slice()
  }

  // Decode in chunks to avoid "call stack size exceeded".
  var res = ''
  var i = 0
  while (i < len) {
    res += String.fromCharCode.apply(
      String,
      codePoints.slice(i, i += MAX_ARGUMENTS_LENGTH)
    )
  }
  return res
}

function asciiSlice (buf, start, end) {
  var ret = ''
  end = Math.min(buf.length, end)

  for (var i = start; i < end; ++i) {
    ret += String.fromCharCode(buf[i] & 0x7F)
  }
  return ret
}

function latin1Slice (buf, start, end) {
  var ret = ''
  end = Math.min(buf.length, end)

  for (var i = start; i < end; ++i) {
    ret += String.fromCharCode(buf[i])
  }
  return ret
}

function hexSlice (buf, start, end) {
  var len = buf.length

  if (!start || start < 0) start = 0
  if (!end || end < 0 || end > len) end = len

  var out = ''
  for (var i = start; i < end; ++i) {
    out += toHex(buf[i])
  }
  return out
}

function utf16leSlice (buf, start, end) {
  var bytes = buf.slice(start, end)
  var res = ''
  for (var i = 0; i < bytes.length; i += 2) {
    res += String.fromCharCode(bytes[i] + (bytes[i + 1] * 256))
  }
  return res
}

Buffer.prototype.slice = function slice (start, end) {
  var len = this.length
  start = ~~start
  end = end === undefined ? len : ~~end

  if (start < 0) {
    start += len
    if (start < 0) start = 0
  } else if (start > len) {
    start = len
  }

  if (end < 0) {
    end += len
    if (end < 0) end = 0
  } else if (end > len) {
    end = len
  }

  if (end < start) end = start

  var newBuf = this.subarray(start, end)
  // Return an augmented `Uint8Array` instance
  newBuf.__proto__ = Buffer.prototype
  return newBuf
}

/*
 * Need to make sure that buffer isn't trying to write out of bounds.
 */
function checkOffset (offset, ext, length) {
  if ((offset % 1) !== 0 || offset < 0) throw new RangeError('offset is not uint')
  if (offset + ext > length) throw new RangeError('Trying to access beyond buffer length')
}

Buffer.prototype.readUIntLE = function readUIntLE (offset, byteLength, noAssert) {
  offset = offset >>> 0
  byteLength = byteLength >>> 0
  if (!noAssert) checkOffset(offset, byteLength, this.length)

  var val = this[offset]
  var mul = 1
  var i = 0
  while (++i < byteLength && (mul *= 0x100)) {
    val += this[offset + i] * mul
  }

  return val
}

Buffer.prototype.readUIntBE = function readUIntBE (offset, byteLength, noAssert) {
  offset = offset >>> 0
  byteLength = byteLength >>> 0
  if (!noAssert) {
    checkOffset(offset, byteLength, this.length)
  }

  var val = this[offset + --byteLength]
  var mul = 1
  while (byteLength > 0 && (mul *= 0x100)) {
    val += this[offset + --byteLength] * mul
  }

  return val
}

Buffer.prototype.readUInt8 = function readUInt8 (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 1, this.length)
  return this[offset]
}

Buffer.prototype.readUInt16LE = function readUInt16LE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 2, this.length)
  return this[offset] | (this[offset + 1] << 8)
}

Buffer.prototype.readUInt16BE = function readUInt16BE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 2, this.length)
  return (this[offset] << 8) | this[offset + 1]
}

Buffer.prototype.readUInt32LE = function readUInt32LE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 4, this.length)

  return ((this[offset]) |
      (this[offset + 1] << 8) |
      (this[offset + 2] << 16)) +
      (this[offset + 3] * 0x1000000)
}

Buffer.prototype.readUInt32BE = function readUInt32BE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 4, this.length)

  return (this[offset] * 0x1000000) +
    ((this[offset + 1] << 16) |
    (this[offset + 2] << 8) |
    this[offset + 3])
}

Buffer.prototype.readIntLE = function readIntLE (offset, byteLength, noAssert) {
  offset = offset >>> 0
  byteLength = byteLength >>> 0
  if (!noAssert) checkOffset(offset, byteLength, this.length)

  var val = this[offset]
  var mul = 1
  var i = 0
  while (++i < byteLength && (mul *= 0x100)) {
    val += this[offset + i] * mul
  }
  mul *= 0x80

  if (val >= mul) val -= Math.pow(2, 8 * byteLength)

  return val
}

Buffer.prototype.readIntBE = function readIntBE (offset, byteLength, noAssert) {
  offset = offset >>> 0
  byteLength = byteLength >>> 0
  if (!noAssert) checkOffset(offset, byteLength, this.length)

  var i = byteLength
  var mul = 1
  var val = this[offset + --i]
  while (i > 0 && (mul *= 0x100)) {
    val += this[offset + --i] * mul
  }
  mul *= 0x80

  if (val >= mul) val -= Math.pow(2, 8 * byteLength)

  return val
}

Buffer.prototype.readInt8 = function readInt8 (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 1, this.length)
  if (!(this[offset] & 0x80)) return (this[offset])
  return ((0xff - this[offset] + 1) * -1)
}

Buffer.prototype.readInt16LE = function readInt16LE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 2, this.length)
  var val = this[offset] | (this[offset + 1] << 8)
  return (val & 0x8000) ? val | 0xFFFF0000 : val
}

Buffer.prototype.readInt16BE = function readInt16BE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 2, this.length)
  var val = this[offset + 1] | (this[offset] << 8)
  return (val & 0x8000) ? val | 0xFFFF0000 : val
}

Buffer.prototype.readInt32LE = function readInt32LE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 4, this.length)

  return (this[offset]) |
    (this[offset + 1] << 8) |
    (this[offset + 2] << 16) |
    (this[offset + 3] << 24)
}

Buffer.prototype.readInt32BE = function readInt32BE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 4, this.length)

  return (this[offset] << 24) |
    (this[offset + 1] << 16) |
    (this[offset + 2] << 8) |
    (this[offset + 3])
}

Buffer.prototype.readFloatLE = function readFloatLE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 4, this.length)
  return ieee754.read(this, offset, true, 23, 4)
}

Buffer.prototype.readFloatBE = function readFloatBE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 4, this.length)
  return ieee754.read(this, offset, false, 23, 4)
}

Buffer.prototype.readDoubleLE = function readDoubleLE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 8, this.length)
  return ieee754.read(this, offset, true, 52, 8)
}

Buffer.prototype.readDoubleBE = function readDoubleBE (offset, noAssert) {
  offset = offset >>> 0
  if (!noAssert) checkOffset(offset, 8, this.length)
  return ieee754.read(this, offset, false, 52, 8)
}

function checkInt (buf, value, offset, ext, max, min) {
  if (!Buffer.isBuffer(buf)) throw new TypeError('"buffer" argument must be a Buffer instance')
  if (value > max || value < min) throw new RangeError('"value" argument is out of bounds')
  if (offset + ext > buf.length) throw new RangeError('Index out of range')
}

Buffer.prototype.writeUIntLE = function writeUIntLE (value, offset, byteLength, noAssert) {
  value = +value
  offset = offset >>> 0
  byteLength = byteLength >>> 0
  if (!noAssert) {
    var maxBytes = Math.pow(2, 8 * byteLength) - 1
    checkInt(this, value, offset, byteLength, maxBytes, 0)
  }

  var mul = 1
  var i = 0
  this[offset] = value & 0xFF
  while (++i < byteLength && (mul *= 0x100)) {
    this[offset + i] = (value / mul) & 0xFF
  }

  return offset + byteLength
}

Buffer.prototype.writeUIntBE = function writeUIntBE (value, offset, byteLength, noAssert) {
  value = +value
  offset = offset >>> 0
  byteLength = byteLength >>> 0
  if (!noAssert) {
    var maxBytes = Math.pow(2, 8 * byteLength) - 1
    checkInt(this, value, offset, byteLength, maxBytes, 0)
  }

  var i = byteLength - 1
  var mul = 1
  this[offset + i] = value & 0xFF
  while (--i >= 0 && (mul *= 0x100)) {
    this[offset + i] = (value / mul) & 0xFF
  }

  return offset + byteLength
}

Buffer.prototype.writeUInt8 = function writeUInt8 (value, offset, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) checkInt(this, value, offset, 1, 0xff, 0)
  this[offset] = (value & 0xff)
  return offset + 1
}

Buffer.prototype.writeUInt16LE = function writeUInt16LE (value, offset, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) checkInt(this, value, offset, 2, 0xffff, 0)
  this[offset] = (value & 0xff)
  this[offset + 1] = (value >>> 8)
  return offset + 2
}

Buffer.prototype.writeUInt16BE = function writeUInt16BE (value, offset, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) checkInt(this, value, offset, 2, 0xffff, 0)
  this[offset] = (value >>> 8)
  this[offset + 1] = (value & 0xff)
  return offset + 2
}

Buffer.prototype.writeUInt32LE = function writeUInt32LE (value, offset, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) checkInt(this, value, offset, 4, 0xffffffff, 0)
  this[offset + 3] = (value >>> 24)
  this[offset + 2] = (value >>> 16)
  this[offset + 1] = (value >>> 8)
  this[offset] = (value & 0xff)
  return offset + 4
}

Buffer.prototype.writeUInt32BE = function writeUInt32BE (value, offset, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) checkInt(this, value, offset, 4, 0xffffffff, 0)
  this[offset] = (value >>> 24)
  this[offset + 1] = (value >>> 16)
  this[offset + 2] = (value >>> 8)
  this[offset + 3] = (value & 0xff)
  return offset + 4
}

Buffer.prototype.writeIntLE = function writeIntLE (value, offset, byteLength, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) {
    var limit = Math.pow(2, (8 * byteLength) - 1)

    checkInt(this, value, offset, byteLength, limit - 1, -limit)
  }

  var i = 0
  var mul = 1
  var sub = 0
  this[offset] = value & 0xFF
  while (++i < byteLength && (mul *= 0x100)) {
    if (value < 0 && sub === 0 && this[offset + i - 1] !== 0) {
      sub = 1
    }
    this[offset + i] = ((value / mul) >> 0) - sub & 0xFF
  }

  return offset + byteLength
}

Buffer.prototype.writeIntBE = function writeIntBE (value, offset, byteLength, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) {
    var limit = Math.pow(2, (8 * byteLength) - 1)

    checkInt(this, value, offset, byteLength, limit - 1, -limit)
  }

  var i = byteLength - 1
  var mul = 1
  var sub = 0
  this[offset + i] = value & 0xFF
  while (--i >= 0 && (mul *= 0x100)) {
    if (value < 0 && sub === 0 && this[offset + i + 1] !== 0) {
      sub = 1
    }
    this[offset + i] = ((value / mul) >> 0) - sub & 0xFF
  }

  return offset + byteLength
}

Buffer.prototype.writeInt8 = function writeInt8 (value, offset, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) checkInt(this, value, offset, 1, 0x7f, -0x80)
  if (value < 0) value = 0xff + value + 1
  this[offset] = (value & 0xff)
  return offset + 1
}

Buffer.prototype.writeInt16LE = function writeInt16LE (value, offset, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) checkInt(this, value, offset, 2, 0x7fff, -0x8000)
  this[offset] = (value & 0xff)
  this[offset + 1] = (value >>> 8)
  return offset + 2
}

Buffer.prototype.writeInt16BE = function writeInt16BE (value, offset, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) checkInt(this, value, offset, 2, 0x7fff, -0x8000)
  this[offset] = (value >>> 8)
  this[offset + 1] = (value & 0xff)
  return offset + 2
}

Buffer.prototype.writeInt32LE = function writeInt32LE (value, offset, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) checkInt(this, value, offset, 4, 0x7fffffff, -0x80000000)
  this[offset] = (value & 0xff)
  this[offset + 1] = (value >>> 8)
  this[offset + 2] = (value >>> 16)
  this[offset + 3] = (value >>> 24)
  return offset + 4
}

Buffer.prototype.writeInt32BE = function writeInt32BE (value, offset, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) checkInt(this, value, offset, 4, 0x7fffffff, -0x80000000)
  if (value < 0) value = 0xffffffff + value + 1
  this[offset] = (value >>> 24)
  this[offset + 1] = (value >>> 16)
  this[offset + 2] = (value >>> 8)
  this[offset + 3] = (value & 0xff)
  return offset + 4
}

function checkIEEE754 (buf, value, offset, ext, max, min) {
  if (offset + ext > buf.length) throw new RangeError('Index out of range')
  if (offset < 0) throw new RangeError('Index out of range')
}

function writeFloat (buf, value, offset, littleEndian, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) {
    checkIEEE754(buf, value, offset, 4, 3.4028234663852886e+38, -3.4028234663852886e+38)
  }
  ieee754.write(buf, value, offset, littleEndian, 23, 4)
  return offset + 4
}

Buffer.prototype.writeFloatLE = function writeFloatLE (value, offset, noAssert) {
  return writeFloat(this, value, offset, true, noAssert)
}

Buffer.prototype.writeFloatBE = function writeFloatBE (value, offset, noAssert) {
  return writeFloat(this, value, offset, false, noAssert)
}

function writeDouble (buf, value, offset, littleEndian, noAssert) {
  value = +value
  offset = offset >>> 0
  if (!noAssert) {
    checkIEEE754(buf, value, offset, 8, 1.7976931348623157E+308, -1.7976931348623157E+308)
  }
  ieee754.write(buf, value, offset, littleEndian, 52, 8)
  return offset + 8
}

Buffer.prototype.writeDoubleLE = function writeDoubleLE (value, offset, noAssert) {
  return writeDouble(this, value, offset, true, noAssert)
}

Buffer.prototype.writeDoubleBE = function writeDoubleBE (value, offset, noAssert) {
  return writeDouble(this, value, offset, false, noAssert)
}

// copy(targetBuffer, targetStart=0, sourceStart=0, sourceEnd=buffer.length)
Buffer.prototype.copy = function copy (target, targetStart, start, end) {
  if (!start) start = 0
  if (!end && end !== 0) end = this.length
  if (targetStart >= target.length) targetStart = target.length
  if (!targetStart) targetStart = 0
  if (end > 0 && end < start) end = start

  // Copy 0 bytes; we're done
  if (end === start) return 0
  if (target.length === 0 || this.length === 0) return 0

  // Fatal error conditions
  if (targetStart < 0) {
    throw new RangeError('targetStart out of bounds')
  }
  if (start < 0 || start >= this.length) throw new RangeError('sourceStart out of bounds')
  if (end < 0) throw new RangeError('sourceEnd out of bounds')

  // Are we oob?
  if (end > this.length) end = this.length
  if (target.length - targetStart < end - start) {
    end = target.length - targetStart + start
  }

  var len = end - start
  var i

  if (this === target && start < targetStart && targetStart < end) {
    // descending copy from end
    for (i = len - 1; i >= 0; --i) {
      target[i + targetStart] = this[i + start]
    }
  } else if (len < 1000) {
    // ascending copy from start
    for (i = 0; i < len; ++i) {
      target[i + targetStart] = this[i + start]
    }
  } else {
    Uint8Array.prototype.set.call(
      target,
      this.subarray(start, start + len),
      targetStart
    )
  }

  return len
}

// Usage:
//    buffer.fill(number[, offset[, end]])
//    buffer.fill(buffer[, offset[, end]])
//    buffer.fill(string[, offset[, end]][, encoding])
Buffer.prototype.fill = function fill (val, start, end, encoding) {
  // Handle string cases:
  if (typeof val === 'string') {
    if (typeof start === 'string') {
      encoding = start
      start = 0
      end = this.length
    } else if (typeof end === 'string') {
      encoding = end
      end = this.length
    }
    if (val.length === 1) {
      var code = val.charCodeAt(0)
      if (code < 256) {
        val = code
      }
    }
    if (encoding !== undefined && typeof encoding !== 'string') {
      throw new TypeError('encoding must be a string')
    }
    if (typeof encoding === 'string' && !Buffer.isEncoding(encoding)) {
      throw new TypeError('Unknown encoding: ' + encoding)
    }
  } else if (typeof val === 'number') {
    val = val & 255
  }

  // Invalid ranges are not set to a default, so can range check early.
  if (start < 0 || this.length < start || this.length < end) {
    throw new RangeError('Out of range index')
  }

  if (end <= start) {
    return this
  }

  start = start >>> 0
  end = end === undefined ? this.length : end >>> 0

  if (!val) val = 0

  var i
  if (typeof val === 'number') {
    for (i = start; i < end; ++i) {
      this[i] = val
    }
  } else {
    var bytes = Buffer.isBuffer(val)
      ? val
      : new Buffer(val, encoding)
    var len = bytes.length
    for (i = 0; i < end - start; ++i) {
      this[i + start] = bytes[i % len]
    }
  }

  return this
}

// HELPER FUNCTIONS
// ================

var INVALID_BASE64_RE = /[^+/0-9A-Za-z-_]/g

function base64clean (str) {
  // Node strips out invalid characters like \n and \t from the string, base64-js does not
  str = str.trim().replace(INVALID_BASE64_RE, '')
  // Node converts strings with length < 2 to ''
  if (str.length < 2) return ''
  // Node allows for non-padded base64 strings (missing trailing ===), base64-js does not
  while (str.length % 4 !== 0) {
    str = str + '='
  }
  return str
}

function toHex (n) {
  if (n < 16) return '0' + n.toString(16)
  return n.toString(16)
}

function utf8ToBytes (string, units) {
  units = units || Infinity
  var codePoint
  var length = string.length
  var leadSurrogate = null
  var bytes = []

  for (var i = 0; i < length; ++i) {
    codePoint = string.charCodeAt(i)

    // is surrogate component
    if (codePoint > 0xD7FF && codePoint < 0xE000) {
      // last char was a lead
      if (!leadSurrogate) {
        // no lead yet
        if (codePoint > 0xDBFF) {
          // unexpected trail
          if ((units -= 3) > -1) bytes.push(0xEF, 0xBF, 0xBD)
          continue
        } else if (i + 1 === length) {
          // unpaired lead
          if ((units -= 3) > -1) bytes.push(0xEF, 0xBF, 0xBD)
          continue
        }

        // valid lead
        leadSurrogate = codePoint

        continue
      }

      // 2 leads in a row
      if (codePoint < 0xDC00) {
        if ((units -= 3) > -1) bytes.push(0xEF, 0xBF, 0xBD)
        leadSurrogate = codePoint
        continue
      }

      // valid surrogate pair
      codePoint = (leadSurrogate - 0xD800 << 10 | codePoint - 0xDC00) + 0x10000
    } else if (leadSurrogate) {
      // valid bmp char, but last char was a lead
      if ((units -= 3) > -1) bytes.push(0xEF, 0xBF, 0xBD)
    }

    leadSurrogate = null

    // encode utf8
    if (codePoint < 0x80) {
      if ((units -= 1) < 0) break
      bytes.push(codePoint)
    } else if (codePoint < 0x800) {
      if ((units -= 2) < 0) break
      bytes.push(
        codePoint >> 0x6 | 0xC0,
        codePoint & 0x3F | 0x80
      )
    } else if (codePoint < 0x10000) {
      if ((units -= 3) < 0) break
      bytes.push(
        codePoint >> 0xC | 0xE0,
        codePoint >> 0x6 & 0x3F | 0x80,
        codePoint & 0x3F | 0x80
      )
    } else if (codePoint < 0x110000) {
      if ((units -= 4) < 0) break
      bytes.push(
        codePoint >> 0x12 | 0xF0,
        codePoint >> 0xC & 0x3F | 0x80,
        codePoint >> 0x6 & 0x3F | 0x80,
        codePoint & 0x3F | 0x80
      )
    } else {
      throw new Error('Invalid code point')
    }
  }

  return bytes
}

function asciiToBytes (str) {
  var byteArray = []
  for (var i = 0; i < str.length; ++i) {
    // Node's code seems to be doing this and not & 0x7F..
    byteArray.push(str.charCodeAt(i) & 0xFF)
  }
  return byteArray
}

function utf16leToBytes (str, units) {
  var c, hi, lo
  var byteArray = []
  for (var i = 0; i < str.length; ++i) {
    if ((units -= 2) < 0) break

    c = str.charCodeAt(i)
    hi = c >> 8
    lo = c % 256
    byteArray.push(lo)
    byteArray.push(hi)
  }

  return byteArray
}

function base64ToBytes (str) {
  return base64.toByteArray(base64clean(str))
}

function blitBuffer (src, dst, offset, length) {
  for (var i = 0; i < length; ++i) {
    if ((i + offset >= dst.length) || (i >= src.length)) break
    dst[i + offset] = src[i]
  }
  return i
}

// ArrayBuffers from another context (i.e. an iframe) do not pass the `instanceof` check
// but they should be treated as valid. See: https://github.com/feross/buffer/issues/166
function isArrayBuffer (obj) {
  return obj instanceof ArrayBuffer ||
    (obj != null && obj.constructor != null && obj.constructor.name === 'ArrayBuffer' &&
      typeof obj.byteLength === 'number')
}

// Node 0.10 supports `ArrayBuffer` but lacks `ArrayBuffer.isView`
function isArrayBufferView (obj) {
  return (typeof ArrayBuffer.isView === 'function') && ArrayBuffer.isView(obj)
}

function numberIsNaN (obj) {
  return obj !== obj // eslint-disable-line no-self-compare
}

},{"base64-js":1,"ieee754":4}],3:[function(require,module,exports){
"use strict";

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

var _slicedToArray = function () { function sliceIterator(arr, i) { var _arr = []; var _n = true; var _d = false; var _e = undefined; try { for (var _i = arr[Symbol.iterator](), _s; !(_n = (_s = _i.next()).done); _n = true) { _arr.push(_s.value); if (i && _arr.length === i) break; } } catch (err) { _d = true; _e = err; } finally { try { if (!_n && _i["return"]) _i["return"](); } finally { if (_d) throw _e; } } return _arr; } return function (arr, i) { if (Array.isArray(arr)) { return arr; } else if (Symbol.iterator in Object(arr)) { return sliceIterator(arr, i); } else { throw new TypeError("Invalid attempt to destructure non-iterable instance"); } }; }();

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _toConsumableArray(arr) { if (Array.isArray(arr)) { for (var i = 0, arr2 = Array(arr.length); i < arr.length; i++) { arr2[i] = arr[i]; } return arr2; } else { return Array.from(arr); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

module.exports = { bind: bind, inject: inject, getInstanceOf: getInstanceOf, getPolicy: getPolicy };

/*

Welcome to DRY-DI.

*/

var knownInterfaces = [];
var interfaces = {};
var concretions = {};

var context = [{}];

var Ref = function () {
    function Ref(provider, ifid, scope) {
        _classCallCheck(this, Ref);

        this.ifid = ifid;
        this.count = provider.dependencyCount;
        this.dependencyCount = provider.dependencyCount;
        this.scope = scope;

        this.binds = {};
        this.injections = null;
        this.provider = provider;

        var pslot = scope[ifid] || (scope[ifid] = new Slot());

        if (provider.injections) {
            this.injections = {};
            Object.assign(this.injections, provider.injections);

            for (var key in this.injections) {
                var _ifid = this.injections[key];
                var slot = scope[_ifid] || (scope[_ifid] = new Slot());
                slot.addInjector(this);
            }
        }

        pslot.addProvider(this);
    }

    _createClass(Ref, [{
        key: "bindInjections",
        value: function bindInjections(injections) {
            var _this = this;

            injections.forEach(function (_ref) {
                var _ref2 = _slicedToArray(_ref, 2),
                    clazz = _ref2[0],
                    _interface = _ref2[1];

                var key = knownInterfaces.indexOf(_interface);
                var injection = injections[key];

                if (!(key in _this.binds)) {
                    var ifid = _this.injections[key];
                    _this.scope[_this.ifid].removeInjector(_this);
                    _this.satisfy();
                    _this.dependencyCount--;
                }

                _this.binds[key] = clazz;
            });
        }
    }, {
        key: "satisfy",
        value: function satisfy() {

            this.count--;

            if (this.count == 0) this.scope[this.ifid].addViable();
        }
    }]);

    return Ref;
}();

var Slot = function () {
    function Slot() {
        _classCallCheck(this, Slot);

        this.viableProviders = 0;
        this.providers = [];
        this.injectors = [];
    }

    _createClass(Slot, [{
        key: "addInjector",
        value: function addInjector(ref) {

            this.injectors.push(ref);
            if (this.viableProviders > 0) ref.satisfy();
        }
    }, {
        key: "removeInjector",
        value: function removeInjector(ref) {

            var index = this.injectors.indexOf(ref);
            if (index > -1) this.injectors.splice(index, 1);
        }
    }, {
        key: "addProvider",
        value: function addProvider(ref) {

            this.providers.push(ref);
            if (ref.count == 0) this.addViable();
        }
    }, {
        key: "addViable",
        value: function addViable() {

            this.viableProviders++;
            if (this.viableProviders == 1) {

                var injectors = this.injectors;
                for (var i = 0, l = injectors.length; i < l; ++i) {
                    injectors[i].satisfy();
                }
            }
        }
    }, {
        key: "getViable",
        value: function getViable(clazz, tags, multiple) {

            if (this.viableProviders == 0) {
                if (!multiple) throw new Error("No viable providers for " + clazz + ". #126");
                return [];
            }

            var ret = multiple ? [] : null;

            var mostViable = null;
            var maxPoints = -1;
            notViable: for (var i = 0, c; c = this.providers[i]; ++i) {
                if (c.count) continue;
                var points = c.dependencyCount;
                if (tags && c.tags) {
                    for (var tag in tags) {
                        if (c.tags[tag] !== tags[tag]) continue notViable;
                        points++;
                    }
                }
                if (multiple) ret[ret.length] = c.provider.policy.bind(c.provider, c.binds);else {
                    if (points > maxPoints) {
                        maxPoints = points;
                        mostViable = c;
                    }
                }
            }

            if (!multiple) {
                if (!mostViable) throw new Error("No viable providers for " + clazz + ". Tag mismatch.");

                return mostViable.provider.policy.bind(mostViable.provider, mostViable.binds);
            } else return ret;
        }
    }]);

    return Slot;
}();

function registerInterface(ifc) {

    var props = {},
        currifc = void 0;

    if (typeof ifc == "function") currifc = ifc.prototype;else if ((typeof ifc === "undefined" ? "undefined" : _typeof(ifc)) == "object") currifc = ifc;

    while (currifc && currifc !== Object.prototype) {

        var names = Object.getOwnPropertyNames(ifc.prototype);

        for (var i = 0, l = names.length; i < l; ++i) {
            var name = names[i];

            if (!props[name]) props[name] = _typeof(ifc.prototype[name]);
        }

        currifc = currifc.prototype;
    }

    var len = knownInterfaces.length;
    interfaces[len] = props;
    knownInterfaces[len] = ifc;

    return len;
}

var Provide = function () {
    function Provide() {
        _classCallCheck(this, Provide);

        this.injections = null;
        this.dependencyCount = 0;
        this.clazz = null;
        this.ctor = null;
        this.binds = null;

        // default policy is to create a new instance for each injection
        this.policy = function (binds, args) {
            return new this.ctor(binds, args);
        };
    }

    _createClass(Provide, [{
        key: "clone",
        value: function clone() {

            var ret = new Provide();

            ret.injections = this.injections;
            ret.dependencyCount = this.dependencyCount;
            ret.clazz = this.clazz;
            ret.policy = this.policy;
            ret.ctor = this.ctor;
            ret.binds = this.binds;

            return ret;
        }
    }, {
        key: "bindInjections",
        value: function bindInjections(injections) {

            var binds = this.binds = this.binds || [];
            var bindCount = this.binds.length;

            injections.forEach(function (_ref3) {
                var _ref4 = _slicedToArray(_ref3, 2),
                    clazz = _ref4[0],
                    _interface = _ref4[1];

                for (var i = 0; i < bindCount; ++i) {
                    if (binds[i][0] == clazz) return;
                }
                binds[binds.length] = [clazz, _interface];
            });

            return this;
        }
    }, {
        key: "getRef",
        value: function getRef(ifid, _interface) {

            var map = interfaces[ifid],
                clazz = this.clazz;

            for (var key in map) {
                if (_typeof(clazz.prototype[key]) == map[key]) continue;
                throw new Error("Class " + clazz.name + " can't provide to interface " + _interface.name + " because " + key + " is " + _typeof(clazz[key]) + " instead of " + map[key] + ".");
            }

            return new Ref(this, ifid, context[context.length - 1]);
        }
    }, {
        key: "setConcretion",
        value: function setConcretion(clazz) {

            this.clazz = clazz;
            if (typeof clazz == "function") {
                this.ctor = function (_clazz) {
                    _inherits(_class, _clazz);

                    function _class(binds, args) {
                        var _ref5;

                        _classCallCheck(this, _class);

                        return _possibleConstructorReturn(this, (_ref5 = _class.__proto__ || Object.getPrototypeOf(_class)).call.apply(_ref5, [this].concat(_toConsumableArray(args))));
                    }

                    return _class;
                }(clazz);
                // this.ctor.prototype = Object.create(clazz.prototype);
            } else {
                this.policy = function () {
                    return clazz;
                };
            }

            var cid = knownInterfaces.indexOf(clazz);
            if (cid == -1) cid = registerInterface(clazz);

            if (!concretions[cid]) concretions[cid] = [this];else concretions[cid].push(this);

            return this;
        }
    }, {
        key: "factory",
        value: function factory() {

            this.policy = function (binds, args) {
                var THIS = this;

                return function () {
                    for (var _len = arguments.length, args2 = Array(_len), _key = 0; _key < _len; _key++) {
                        args2[_key] = arguments[_key];
                    }

                    return new THIS.ctor(binds, args.concat(args2));
                };
            };

            return this;
        }
    }, {
        key: "singleton",
        value: function singleton() {

            var instance = null;
            this.policy = function (binds, args) {

                if (instance) return instance;

                instance = Object.create(this.ctor.prototype);
                instance.constructor = this.ctor;
                this.ctor.call(instance, binds, args);

                // new (class extends this.ctor{
                //     constructor( args ){
                //         instance = this; // cant do this :(
                //         super(args);
                //     }
                // }

                return instance;
            };

            return this;
        }
    }]);

    return Provide;
}();

function bind(clazz) {

    var cid = knownInterfaces.indexOf(clazz);
    if (cid == -1) {
        cid = registerInterface(clazz);
    }

    var providers = concretions[cid];
    var localProviders = [];

    if (!providers) {

        if (clazz && clazz["@inject"]) inject(clazz["@inject"]).into(clazz);else new Provide().setConcretion(clazz);

        providers = concretions[cid];
    }

    localProviders = providers.map(function (partial) {
        return partial.clone();
    });

    var refs = [];
    var tags = null;
    var ifid = void 0;

    var partialBind = {
        to: function to(_interface) {

            var ifid = knownInterfaces.indexOf(_interface);
            if (ifid == -1) ifid = registerInterface(_interface);

            localProviders.forEach(function (provider) {

                var ref = provider.getRef(ifid, _interface);
                ref.tags = tags;
                refs.push(ref);
            });

            return this;
        },

        withTags: function withTags(tags) {
            refs.forEach(function (ref) {
                return ref.tags = tags;
            });
            return this;
        },

        singleton: function singleton() {
            localProviders.forEach(function (provider) {
                return provider.singleton();
            });
            return this;
        },
        factory: function factory() {
            localProviders.forEach(function (provider) {
                return provider.factory();
            });
            return this;
        },
        inject: function inject(map) {
            return this.injecting(map);
        },
        injecting: function injecting() {
            for (var _len2 = arguments.length, args = Array(_len2), _key2 = 0; _key2 < _len2; _key2++) {
                args[_key2] = arguments[_key2];
            }

            refs.forEach(function (ref) {
                return ref.bindInjections(args);
            });
            localProviders.forEach(function (provider) {
                return provider.bindInjections(args);
            });
            return this;
        }

    };

    return partialBind;
}

var Inject = function () {
    function Inject(dependencies) {
        _classCallCheck(this, Inject);

        this.dependencies = dependencies;
        var tags = this.tags = {};
        for (var key in dependencies) {
            tags[key] = {};
        }
    }

    _createClass(Inject, [{
        key: "into",
        value: function into(clazz) {

            var cid = knownInterfaces.indexOf(clazz);
            if (cid == -1) cid = registerInterface(clazz);

            var injections = {},
                map = this.dependencies,
                dependencyCount = 0,
                tags = this.tags,
                multiple = {};

            for (var key in map) {

                var _interface = map[key];
                var dependency = _interface;
                if (Array.isArray(dependency)) {

                    _interface = _interface[0];
                    for (var i = 1; i < dependency.length; ++i) {

                        if (typeof dependency[i] == "string") tags[key][dependency[i]] = true;else if (Array.isArray(dependency[i])) multiple[key] = true;else if (dependency[i]) Object.assign(tags[key], dependency[i]);
                    }
                }

                var ifid = knownInterfaces.indexOf(_interface);

                if (ifid == -1) ifid = registerInterface(_interface);

                injections[key] = ifid;

                dependencyCount++;
            }

            var provider = new Provide().setConcretion(clazz),
                proto = clazz.prototype;
            var providers = concretions[cid];

            provider.injections = injections;
            provider.dependencyCount = dependencyCount;

            provider.ctor = function (binds, args) {
                resolveDependencies(binds, this);
                clazz.apply(this, args);
            };
            provider.ctor.prototype = Object.create(clazz.prototype);
            provider.ctor.prototype.constructor = clazz;

            // provider.ctor = class extends clazz {
            //     constructor( args ){
            //         resolveDependencies( this ); // *sigh*
            //         super(...args);
            //     }
            // };

            function resolveDependencies(binds, obj) {
                var slotset = context[context.length - 1];
                for (var _key3 in injections) {
                    if (binds && injections[_key3] in binds) {
                        obj[_key3] = binds[injections[_key3]];
                        continue;
                    }

                    var slot = slotset[injections[_key3]];
                    var policy = slot.getViable(_key3, tags[_key3], multiple[_key3]);
                    if (!multiple[_key3]) obj[_key3] = policy([]);else {
                        var out = obj[_key3] = [];
                        for (var _i2 = 0; _i2 < policy.length; ++_i2) {
                            out[_i2] = policy[_i2]([]);
                        }
                    }
                }
            }
        }
    }]);

    return Inject;
}();

function inject(dependencies) {

    return new Inject(dependencies);
}

function getInstanceOf(_interface) {
    for (var _len3 = arguments.length, args = Array(_len3 > 1 ? _len3 - 1 : 0), _key4 = 1; _key4 < _len3; _key4++) {
        args[_key4 - 1] = arguments[_key4];
    }

    // let ifid = knownInterfaces.indexOf( _interface );
    // let slot = context[ context.length-1 ][ ifid ];

    // if( !slot )
    //     throw new Error("No providers for " + (_interface.name || _interface) + ". #467");

    // let policy = slot.getViable( _interface.name || _interface );

    // return policy.call( null, args );
    return getPolicy({ _interface: _interface, args: args });
}

function getPolicy(desc) {
    desc = desc || {};
    if (!desc._interface) throw new Error("Policy descriptor has no interface.");
    var name = desc._interface.name || desc._interface;
    var tags = desc.tags;
    var multiple = desc.multiple;
    var args = desc.args;

    var ifid = knownInterfaces.indexOf(desc._interface);
    var slot = context[context.length - 1][ifid];

    if (!slot) throw new Error("No providers for " + name + ". #467");

    var policy = slot.getViable(name, tags, multiple);
    if (args) {
        if (multiple) policy = policy.map(function (p) {
            return p.call(null, args);
        });else policy = policy.call(null, args);
    }
    return policy;
}

},{}],4:[function(require,module,exports){
exports.read = function (buffer, offset, isLE, mLen, nBytes) {
  var e, m
  var eLen = nBytes * 8 - mLen - 1
  var eMax = (1 << eLen) - 1
  var eBias = eMax >> 1
  var nBits = -7
  var i = isLE ? (nBytes - 1) : 0
  var d = isLE ? -1 : 1
  var s = buffer[offset + i]

  i += d

  e = s & ((1 << (-nBits)) - 1)
  s >>= (-nBits)
  nBits += eLen
  for (; nBits > 0; e = e * 256 + buffer[offset + i], i += d, nBits -= 8) {}

  m = e & ((1 << (-nBits)) - 1)
  e >>= (-nBits)
  nBits += mLen
  for (; nBits > 0; m = m * 256 + buffer[offset + i], i += d, nBits -= 8) {}

  if (e === 0) {
    e = 1 - eBias
  } else if (e === eMax) {
    return m ? NaN : ((s ? -1 : 1) * Infinity)
  } else {
    m = m + Math.pow(2, mLen)
    e = e - eBias
  }
  return (s ? -1 : 1) * m * Math.pow(2, e - mLen)
}

exports.write = function (buffer, value, offset, isLE, mLen, nBytes) {
  var e, m, c
  var eLen = nBytes * 8 - mLen - 1
  var eMax = (1 << eLen) - 1
  var eBias = eMax >> 1
  var rt = (mLen === 23 ? Math.pow(2, -24) - Math.pow(2, -77) : 0)
  var i = isLE ? 0 : (nBytes - 1)
  var d = isLE ? 1 : -1
  var s = value < 0 || (value === 0 && 1 / value < 0) ? 1 : 0

  value = Math.abs(value)

  if (isNaN(value) || value === Infinity) {
    m = isNaN(value) ? 1 : 0
    e = eMax
  } else {
    e = Math.floor(Math.log(value) / Math.LN2)
    if (value * (c = Math.pow(2, -e)) < 1) {
      e--
      c *= 2
    }
    if (e + eBias >= 1) {
      value += rt / c
    } else {
      value += rt * Math.pow(2, 1 - eBias)
    }
    if (value * c >= 2) {
      e++
      c /= 2
    }

    if (e + eBias >= eMax) {
      m = 0
      e = eMax
    } else if (e + eBias >= 1) {
      m = (value * c - 1) * Math.pow(2, mLen)
      e = e + eBias
    } else {
      m = value * Math.pow(2, eBias - 1) * Math.pow(2, mLen)
      e = 0
    }
  }

  for (; mLen >= 8; buffer[offset + i] = m & 0xff, i += d, m /= 256, mLen -= 8) {}

  e = (e << mLen) | m
  eLen += mLen
  for (; eLen > 0; buffer[offset + i] = e & 0xff, i += d, e /= 256, eLen -= 8) {}

  buffer[offset + i - d] |= s * 128
}

},{}],5:[function(require,module,exports){
(function (global,Buffer){
/*!

JSZip v3.1.5 - A JavaScript class for generating and reading zip files
<http://stuartk.com/jszip>

(c) 2009-2016 Stuart Knightley <stuart [at] stuartk.com>
Dual licenced under the MIT license or GPLv3. See https://raw.github.com/Stuk/jszip/master/LICENSE.markdown.

JSZip uses the library pako released under the MIT license :
https://github.com/nodeca/pako/blob/master/LICENSE
*/
!function(a){if("object"==typeof exports&&"undefined"!=typeof module)module.exports=a();else if("function"==typeof define&&define.amd)define([],a);else{var b;b="undefined"!=typeof window?window:"undefined"!=typeof global?global:"undefined"!=typeof self?self:this,b.JSZip=a()}}(function(){return function a(b,c,d){function e(g,h){if(!c[g]){if(!b[g]){var i="function"==typeof require&&require;if(!h&&i)return i(g,!0);if(f)return f(g,!0);var j=new Error("Cannot find module '"+g+"'");throw j.code="MODULE_NOT_FOUND",j}var k=c[g]={exports:{}};b[g][0].call(k.exports,function(a){var c=b[g][1][a];return e(c?c:a)},k,k.exports,a,b,c,d)}return c[g].exports}for(var f="function"==typeof require&&require,g=0;g<d.length;g++)e(d[g]);return e}({1:[function(a,b,c){"use strict";var d=a("./utils"),e=a("./support"),f="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";c.encode=function(a){for(var b,c,e,g,h,i,j,k=[],l=0,m=a.length,n=m,o="string"!==d.getTypeOf(a);l<a.length;)n=m-l,o?(b=a[l++],c=l<m?a[l++]:0,e=l<m?a[l++]:0):(b=a.charCodeAt(l++),c=l<m?a.charCodeAt(l++):0,e=l<m?a.charCodeAt(l++):0),g=b>>2,h=(3&b)<<4|c>>4,i=n>1?(15&c)<<2|e>>6:64,j=n>2?63&e:64,k.push(f.charAt(g)+f.charAt(h)+f.charAt(i)+f.charAt(j));return k.join("")},c.decode=function(a){var b,c,d,g,h,i,j,k=0,l=0,m="data:";if(a.substr(0,m.length)===m)throw new Error("Invalid base64 input, it looks like a data url.");a=a.replace(/[^A-Za-z0-9\+\/\=]/g,"");var n=3*a.length/4;if(a.charAt(a.length-1)===f.charAt(64)&&n--,a.charAt(a.length-2)===f.charAt(64)&&n--,n%1!==0)throw new Error("Invalid base64 input, bad content length.");var o;for(o=e.uint8array?new Uint8Array(0|n):new Array(0|n);k<a.length;)g=f.indexOf(a.charAt(k++)),h=f.indexOf(a.charAt(k++)),i=f.indexOf(a.charAt(k++)),j=f.indexOf(a.charAt(k++)),b=g<<2|h>>4,c=(15&h)<<4|i>>2,d=(3&i)<<6|j,o[l++]=b,64!==i&&(o[l++]=c),64!==j&&(o[l++]=d);return o}},{"./support":30,"./utils":32}],2:[function(a,b,c){"use strict";function d(a,b,c,d,e){this.compressedSize=a,this.uncompressedSize=b,this.crc32=c,this.compression=d,this.compressedContent=e}var e=a("./external"),f=a("./stream/DataWorker"),g=a("./stream/DataLengthProbe"),h=a("./stream/Crc32Probe"),g=a("./stream/DataLengthProbe");d.prototype={getContentWorker:function(){var a=new f(e.Promise.resolve(this.compressedContent)).pipe(this.compression.uncompressWorker()).pipe(new g("data_length")),b=this;return a.on("end",function(){if(this.streamInfo.data_length!==b.uncompressedSize)throw new Error("Bug : uncompressed data size mismatch")}),a},getCompressedWorker:function(){return new f(e.Promise.resolve(this.compressedContent)).withStreamInfo("compressedSize",this.compressedSize).withStreamInfo("uncompressedSize",this.uncompressedSize).withStreamInfo("crc32",this.crc32).withStreamInfo("compression",this.compression)}},d.createWorkerFrom=function(a,b,c){return a.pipe(new h).pipe(new g("uncompressedSize")).pipe(b.compressWorker(c)).pipe(new g("compressedSize")).withStreamInfo("compression",b)},b.exports=d},{"./external":6,"./stream/Crc32Probe":25,"./stream/DataLengthProbe":26,"./stream/DataWorker":27}],3:[function(a,b,c){"use strict";var d=a("./stream/GenericWorker");c.STORE={magic:"\0\0",compressWorker:function(a){return new d("STORE compression")},uncompressWorker:function(){return new d("STORE decompression")}},c.DEFLATE=a("./flate")},{"./flate":7,"./stream/GenericWorker":28}],4:[function(a,b,c){"use strict";function d(){for(var a,b=[],c=0;c<256;c++){a=c;for(var d=0;d<8;d++)a=1&a?3988292384^a>>>1:a>>>1;b[c]=a}return b}function e(a,b,c,d){var e=h,f=d+c;a^=-1;for(var g=d;g<f;g++)a=a>>>8^e[255&(a^b[g])];return a^-1}function f(a,b,c,d){var e=h,f=d+c;a^=-1;for(var g=d;g<f;g++)a=a>>>8^e[255&(a^b.charCodeAt(g))];return a^-1}var g=a("./utils"),h=d();b.exports=function(a,b){if("undefined"==typeof a||!a.length)return 0;var c="string"!==g.getTypeOf(a);return c?e(0|b,a,a.length,0):f(0|b,a,a.length,0)}},{"./utils":32}],5:[function(a,b,c){"use strict";c.base64=!1,c.binary=!1,c.dir=!1,c.createFolders=!0,c.date=null,c.compression=null,c.compressionOptions=null,c.comment=null,c.unixPermissions=null,c.dosPermissions=null},{}],6:[function(a,b,c){"use strict";var d=null;d="undefined"!=typeof Promise?Promise:a("lie"),b.exports={Promise:d}},{lie:58}],7:[function(a,b,c){"use strict";function d(a,b){h.call(this,"FlateWorker/"+a),this._pako=null,this._pakoAction=a,this._pakoOptions=b,this.meta={}}var e="undefined"!=typeof Uint8Array&&"undefined"!=typeof Uint16Array&&"undefined"!=typeof Uint32Array,f=a("pako"),g=a("./utils"),h=a("./stream/GenericWorker"),i=e?"uint8array":"array";c.magic="\b\0",g.inherits(d,h),d.prototype.processChunk=function(a){this.meta=a.meta,null===this._pako&&this._createPako(),this._pako.push(g.transformTo(i,a.data),!1)},d.prototype.flush=function(){h.prototype.flush.call(this),null===this._pako&&this._createPako(),this._pako.push([],!0)},d.prototype.cleanUp=function(){h.prototype.cleanUp.call(this),this._pako=null},d.prototype._createPako=function(){this._pako=new f[this._pakoAction]({raw:!0,level:this._pakoOptions.level||-1});var a=this;this._pako.onData=function(b){a.push({data:b,meta:a.meta})}},c.compressWorker=function(a){return new d("Deflate",a)},c.uncompressWorker=function(){return new d("Inflate",{})}},{"./stream/GenericWorker":28,"./utils":32,pako:59}],8:[function(a,b,c){"use strict";function d(a,b,c,d){f.call(this,"ZipFileWorker"),this.bytesWritten=0,this.zipComment=b,this.zipPlatform=c,this.encodeFileName=d,this.streamFiles=a,this.accumulate=!1,this.contentBuffer=[],this.dirRecords=[],this.currentSourceOffset=0,this.entriesCount=0,this.currentFile=null,this._sources=[]}var e=a("../utils"),f=a("../stream/GenericWorker"),g=a("../utf8"),h=a("../crc32"),i=a("../signature"),j=function(a,b){var c,d="";for(c=0;c<b;c++)d+=String.fromCharCode(255&a),a>>>=8;return d},k=function(a,b){var c=a;return a||(c=b?16893:33204),(65535&c)<<16},l=function(a,b){return 63&(a||0)},m=function(a,b,c,d,f,m){var n,o,p=a.file,q=a.compression,r=m!==g.utf8encode,s=e.transformTo("string",m(p.name)),t=e.transformTo("string",g.utf8encode(p.name)),u=p.comment,v=e.transformTo("string",m(u)),w=e.transformTo("string",g.utf8encode(u)),x=t.length!==p.name.length,y=w.length!==u.length,z="",A="",B="",C=p.dir,D=p.date,E={crc32:0,compressedSize:0,uncompressedSize:0};b&&!c||(E.crc32=a.crc32,E.compressedSize=a.compressedSize,E.uncompressedSize=a.uncompressedSize);var F=0;b&&(F|=8),r||!x&&!y||(F|=2048);var G=0,H=0;C&&(G|=16),"UNIX"===f?(H=798,G|=k(p.unixPermissions,C)):(H=20,G|=l(p.dosPermissions,C)),n=D.getUTCHours(),n<<=6,n|=D.getUTCMinutes(),n<<=5,n|=D.getUTCSeconds()/2,o=D.getUTCFullYear()-1980,o<<=4,o|=D.getUTCMonth()+1,o<<=5,o|=D.getUTCDate(),x&&(A=j(1,1)+j(h(s),4)+t,z+="up"+j(A.length,2)+A),y&&(B=j(1,1)+j(h(v),4)+w,z+="uc"+j(B.length,2)+B);var I="";I+="\n\0",I+=j(F,2),I+=q.magic,I+=j(n,2),I+=j(o,2),I+=j(E.crc32,4),I+=j(E.compressedSize,4),I+=j(E.uncompressedSize,4),I+=j(s.length,2),I+=j(z.length,2);var J=i.LOCAL_FILE_HEADER+I+s+z,K=i.CENTRAL_FILE_HEADER+j(H,2)+I+j(v.length,2)+"\0\0\0\0"+j(G,4)+j(d,4)+s+z+v;return{fileRecord:J,dirRecord:K}},n=function(a,b,c,d,f){var g="",h=e.transformTo("string",f(d));return g=i.CENTRAL_DIRECTORY_END+"\0\0\0\0"+j(a,2)+j(a,2)+j(b,4)+j(c,4)+j(h.length,2)+h},o=function(a){var b="";return b=i.DATA_DESCRIPTOR+j(a.crc32,4)+j(a.compressedSize,4)+j(a.uncompressedSize,4)};e.inherits(d,f),d.prototype.push=function(a){var b=a.meta.percent||0,c=this.entriesCount,d=this._sources.length;this.accumulate?this.contentBuffer.push(a):(this.bytesWritten+=a.data.length,f.prototype.push.call(this,{data:a.data,meta:{currentFile:this.currentFile,percent:c?(b+100*(c-d-1))/c:100}}))},d.prototype.openedSource=function(a){this.currentSourceOffset=this.bytesWritten,this.currentFile=a.file.name;var b=this.streamFiles&&!a.file.dir;if(b){var c=m(a,b,!1,this.currentSourceOffset,this.zipPlatform,this.encodeFileName);this.push({data:c.fileRecord,meta:{percent:0}})}else this.accumulate=!0},d.prototype.closedSource=function(a){this.accumulate=!1;var b=this.streamFiles&&!a.file.dir,c=m(a,b,!0,this.currentSourceOffset,this.zipPlatform,this.encodeFileName);if(this.dirRecords.push(c.dirRecord),b)this.push({data:o(a),meta:{percent:100}});else for(this.push({data:c.fileRecord,meta:{percent:0}});this.contentBuffer.length;)this.push(this.contentBuffer.shift());this.currentFile=null},d.prototype.flush=function(){for(var a=this.bytesWritten,b=0;b<this.dirRecords.length;b++)this.push({data:this.dirRecords[b],meta:{percent:100}});var c=this.bytesWritten-a,d=n(this.dirRecords.length,c,a,this.zipComment,this.encodeFileName);this.push({data:d,meta:{percent:100}})},d.prototype.prepareNextSource=function(){this.previous=this._sources.shift(),this.openedSource(this.previous.streamInfo),this.isPaused?this.previous.pause():this.previous.resume()},d.prototype.registerPrevious=function(a){this._sources.push(a);var b=this;return a.on("data",function(a){b.processChunk(a)}),a.on("end",function(){b.closedSource(b.previous.streamInfo),b._sources.length?b.prepareNextSource():b.end()}),a.on("error",function(a){b.error(a)}),this},d.prototype.resume=function(){return!!f.prototype.resume.call(this)&&(!this.previous&&this._sources.length?(this.prepareNextSource(),!0):this.previous||this._sources.length||this.generatedError?void 0:(this.end(),!0))},d.prototype.error=function(a){var b=this._sources;if(!f.prototype.error.call(this,a))return!1;for(var c=0;c<b.length;c++)try{b[c].error(a)}catch(a){}return!0},d.prototype.lock=function(){f.prototype.lock.call(this);for(var a=this._sources,b=0;b<a.length;b++)a[b].lock()},b.exports=d},{"../crc32":4,"../signature":23,"../stream/GenericWorker":28,"../utf8":31,"../utils":32}],9:[function(a,b,c){"use strict";var d=a("../compressions"),e=a("./ZipFileWorker"),f=function(a,b){var c=a||b,e=d[c];if(!e)throw new Error(c+" is not a valid compression method !");return e};c.generateWorker=function(a,b,c){var d=new e(b.streamFiles,c,b.platform,b.encodeFileName),g=0;try{a.forEach(function(a,c){g++;var e=f(c.options.compression,b.compression),h=c.options.compressionOptions||b.compressionOptions||{},i=c.dir,j=c.date;c._compressWorker(e,h).withStreamInfo("file",{name:a,dir:i,date:j,comment:c.comment||"",unixPermissions:c.unixPermissions,dosPermissions:c.dosPermissions}).pipe(d)}),d.entriesCount=g}catch(h){d.error(h)}return d}},{"../compressions":3,"./ZipFileWorker":8}],10:[function(a,b,c){"use strict";function d(){if(!(this instanceof d))return new d;if(arguments.length)throw new Error("The constructor with parameters has been removed in JSZip 3.0, please check the upgrade guide.");this.files={},this.comment=null,this.root="",this.clone=function(){var a=new d;for(var b in this)"function"!=typeof this[b]&&(a[b]=this[b]);return a}}d.prototype=a("./object"),d.prototype.loadAsync=a("./load"),d.support=a("./support"),d.defaults=a("./defaults"),d.version="3.1.5",d.loadAsync=function(a,b){return(new d).loadAsync(a,b)},d.external=a("./external"),b.exports=d},{"./defaults":5,"./external":6,"./load":11,"./object":15,"./support":30}],11:[function(a,b,c){"use strict";function d(a){return new f.Promise(function(b,c){var d=a.decompressed.getContentWorker().pipe(new i);d.on("error",function(a){c(a)}).on("end",function(){d.streamInfo.crc32!==a.decompressed.crc32?c(new Error("Corrupted zip : CRC32 mismatch")):b()}).resume()})}var e=a("./utils"),f=a("./external"),g=a("./utf8"),e=a("./utils"),h=a("./zipEntries"),i=a("./stream/Crc32Probe"),j=a("./nodejsUtils");b.exports=function(a,b){var c=this;return b=e.extend(b||{},{base64:!1,checkCRC32:!1,optimizedBinaryString:!1,createFolders:!1,decodeFileName:g.utf8decode}),j.isNode&&j.isStream(a)?f.Promise.reject(new Error("JSZip can't accept a stream when loading a zip file.")):e.prepareContent("the loaded zip file",a,!0,b.optimizedBinaryString,b.base64).then(function(a){var c=new h(b);return c.load(a),c}).then(function(a){var c=[f.Promise.resolve(a)],e=a.files;if(b.checkCRC32)for(var g=0;g<e.length;g++)c.push(d(e[g]));return f.Promise.all(c)}).then(function(a){for(var d=a.shift(),e=d.files,f=0;f<e.length;f++){var g=e[f];c.file(g.fileNameStr,g.decompressed,{binary:!0,optimizedBinaryString:!0,date:g.date,dir:g.dir,comment:g.fileCommentStr.length?g.fileCommentStr:null,unixPermissions:g.unixPermissions,dosPermissions:g.dosPermissions,createFolders:b.createFolders})}return d.zipComment.length&&(c.comment=d.zipComment),c})}},{"./external":6,"./nodejsUtils":14,"./stream/Crc32Probe":25,"./utf8":31,"./utils":32,"./zipEntries":33}],12:[function(a,b,c){"use strict";function d(a,b){f.call(this,"Nodejs stream input adapter for "+a),this._upstreamEnded=!1,this._bindStream(b)}var e=a("../utils"),f=a("../stream/GenericWorker");e.inherits(d,f),d.prototype._bindStream=function(a){var b=this;this._stream=a,a.pause(),a.on("data",function(a){b.push({data:a,meta:{percent:0}})}).on("error",function(a){b.isPaused?this.generatedError=a:b.error(a)}).on("end",function(){b.isPaused?b._upstreamEnded=!0:b.end()})},d.prototype.pause=function(){return!!f.prototype.pause.call(this)&&(this._stream.pause(),!0)},d.prototype.resume=function(){return!!f.prototype.resume.call(this)&&(this._upstreamEnded?this.end():this._stream.resume(),!0)},b.exports=d},{"../stream/GenericWorker":28,"../utils":32}],13:[function(a,b,c){"use strict";function d(a,b,c){e.call(this,b),this._helper=a;var d=this;a.on("data",function(a,b){d.push(a)||d._helper.pause(),c&&c(b)}).on("error",function(a){d.emit("error",a)}).on("end",function(){d.push(null)})}var e=a("readable-stream").Readable,f=a("../utils");f.inherits(d,e),d.prototype._read=function(){this._helper.resume()},b.exports=d},{"../utils":32,"readable-stream":16}],14:[function(a,b,c){"use strict";b.exports={isNode:"undefined"!=typeof Buffer,newBufferFrom:function(a,b){return new Buffer(a,b)},allocBuffer:function(a){return Buffer.alloc?Buffer.alloc(a):new Buffer(a)},isBuffer:function(a){return Buffer.isBuffer(a)},isStream:function(a){return a&&"function"==typeof a.on&&"function"==typeof a.pause&&"function"==typeof a.resume}}},{}],15:[function(a,b,c){"use strict";function d(a){return"[object RegExp]"===Object.prototype.toString.call(a)}var e=a("./utf8"),f=a("./utils"),g=a("./stream/GenericWorker"),h=a("./stream/StreamHelper"),i=a("./defaults"),j=a("./compressedObject"),k=a("./zipObject"),l=a("./generate"),m=a("./nodejsUtils"),n=a("./nodejs/NodejsStreamInputAdapter"),o=function(a,b,c){var d,e=f.getTypeOf(b),h=f.extend(c||{},i);h.date=h.date||new Date,null!==h.compression&&(h.compression=h.compression.toUpperCase()),"string"==typeof h.unixPermissions&&(h.unixPermissions=parseInt(h.unixPermissions,8)),h.unixPermissions&&16384&h.unixPermissions&&(h.dir=!0),h.dosPermissions&&16&h.dosPermissions&&(h.dir=!0),h.dir&&(a=q(a)),h.createFolders&&(d=p(a))&&r.call(this,d,!0);var l="string"===e&&h.binary===!1&&h.base64===!1;c&&"undefined"!=typeof c.binary||(h.binary=!l);var o=b instanceof j&&0===b.uncompressedSize;(o||h.dir||!b||0===b.length)&&(h.base64=!1,h.binary=!0,b="",h.compression="STORE",e="string");var s=null;s=b instanceof j||b instanceof g?b:m.isNode&&m.isStream(b)?new n(a,b):f.prepareContent(a,b,h.binary,h.optimizedBinaryString,h.base64);var t=new k(a,s,h);this.files[a]=t},p=function(a){"/"===a.slice(-1)&&(a=a.substring(0,a.length-1));var b=a.lastIndexOf("/");return b>0?a.substring(0,b):""},q=function(a){return"/"!==a.slice(-1)&&(a+="/"),a},r=function(a,b){return b="undefined"!=typeof b?b:i.createFolders,a=q(a),this.files[a]||o.call(this,a,null,{dir:!0,createFolders:b}),this.files[a]},s={load:function(){throw new Error("This method has been removed in JSZip 3.0, please check the upgrade guide.")},forEach:function(a){var b,c,d;for(b in this.files)this.files.hasOwnProperty(b)&&(d=this.files[b],c=b.slice(this.root.length,b.length),c&&b.slice(0,this.root.length)===this.root&&a(c,d))},filter:function(a){var b=[];return this.forEach(function(c,d){a(c,d)&&b.push(d)}),b},file:function(a,b,c){if(1===arguments.length){if(d(a)){var e=a;return this.filter(function(a,b){return!b.dir&&e.test(a)})}var f=this.files[this.root+a];return f&&!f.dir?f:null}return a=this.root+a,o.call(this,a,b,c),this},folder:function(a){if(!a)return this;if(d(a))return this.filter(function(b,c){return c.dir&&a.test(b)});var b=this.root+a,c=r.call(this,b),e=this.clone();return e.root=c.name,e},remove:function(a){a=this.root+a;var b=this.files[a];if(b||("/"!==a.slice(-1)&&(a+="/"),b=this.files[a]),b&&!b.dir)delete this.files[a];else for(var c=this.filter(function(b,c){return c.name.slice(0,a.length)===a}),d=0;d<c.length;d++)delete this.files[c[d].name];return this},generate:function(a){throw new Error("This method has been removed in JSZip 3.0, please check the upgrade guide.")},generateInternalStream:function(a){var b,c={};try{if(c=f.extend(a||{},{streamFiles:!1,compression:"STORE",compressionOptions:null,type:"",platform:"DOS",comment:null,mimeType:"application/zip",encodeFileName:e.utf8encode}),c.type=c.type.toLowerCase(),c.compression=c.compression.toUpperCase(),"binarystring"===c.type&&(c.type="string"),!c.type)throw new Error("No output type specified.");f.checkSupport(c.type),"darwin"!==c.platform&&"freebsd"!==c.platform&&"linux"!==c.platform&&"sunos"!==c.platform||(c.platform="UNIX"),"win32"===c.platform&&(c.platform="DOS");var d=c.comment||this.comment||"";b=l.generateWorker(this,c,d)}catch(i){b=new g("error"),b.error(i)}return new h(b,c.type||"string",c.mimeType)},generateAsync:function(a,b){return this.generateInternalStream(a).accumulate(b)},generateNodeStream:function(a,b){return a=a||{},a.type||(a.type="nodebuffer"),this.generateInternalStream(a).toNodejsStream(b)}};b.exports=s},{"./compressedObject":2,"./defaults":5,"./generate":9,"./nodejs/NodejsStreamInputAdapter":12,"./nodejsUtils":14,"./stream/GenericWorker":28,"./stream/StreamHelper":29,"./utf8":31,"./utils":32,"./zipObject":35}],16:[function(a,b,c){b.exports=a("stream")},{stream:void 0}],17:[function(a,b,c){"use strict";function d(a){e.call(this,a);for(var b=0;b<this.data.length;b++)a[b]=255&a[b]}var e=a("./DataReader"),f=a("../utils");f.inherits(d,e),d.prototype.byteAt=function(a){return this.data[this.zero+a]},d.prototype.lastIndexOfSignature=function(a){for(var b=a.charCodeAt(0),c=a.charCodeAt(1),d=a.charCodeAt(2),e=a.charCodeAt(3),f=this.length-4;f>=0;--f)if(this.data[f]===b&&this.data[f+1]===c&&this.data[f+2]===d&&this.data[f+3]===e)return f-this.zero;return-1},d.prototype.readAndCheckSignature=function(a){var b=a.charCodeAt(0),c=a.charCodeAt(1),d=a.charCodeAt(2),e=a.charCodeAt(3),f=this.readData(4);return b===f[0]&&c===f[1]&&d===f[2]&&e===f[3]},d.prototype.readData=function(a){if(this.checkOffset(a),0===a)return[];var b=this.data.slice(this.zero+this.index,this.zero+this.index+a);return this.index+=a,b},b.exports=d},{"../utils":32,"./DataReader":18}],18:[function(a,b,c){"use strict";function d(a){this.data=a,this.length=a.length,this.index=0,this.zero=0}var e=a("../utils");d.prototype={checkOffset:function(a){this.checkIndex(this.index+a)},checkIndex:function(a){if(this.length<this.zero+a||a<0)throw new Error("End of data reached (data length = "+this.length+", asked index = "+a+"). Corrupted zip ?")},setIndex:function(a){this.checkIndex(a),this.index=a},skip:function(a){this.setIndex(this.index+a)},byteAt:function(a){},readInt:function(a){var b,c=0;for(this.checkOffset(a),b=this.index+a-1;b>=this.index;b--)c=(c<<8)+this.byteAt(b);return this.index+=a,c},readString:function(a){return e.transformTo("string",this.readData(a))},readData:function(a){},lastIndexOfSignature:function(a){},readAndCheckSignature:function(a){},readDate:function(){var a=this.readInt(4);return new Date(Date.UTC((a>>25&127)+1980,(a>>21&15)-1,a>>16&31,a>>11&31,a>>5&63,(31&a)<<1))}},b.exports=d},{"../utils":32}],19:[function(a,b,c){"use strict";function d(a){e.call(this,a)}var e=a("./Uint8ArrayReader"),f=a("../utils");f.inherits(d,e),d.prototype.readData=function(a){this.checkOffset(a);var b=this.data.slice(this.zero+this.index,this.zero+this.index+a);return this.index+=a,b},b.exports=d},{"../utils":32,"./Uint8ArrayReader":21}],20:[function(a,b,c){"use strict";function d(a){e.call(this,a)}var e=a("./DataReader"),f=a("../utils");f.inherits(d,e),d.prototype.byteAt=function(a){return this.data.charCodeAt(this.zero+a)},d.prototype.lastIndexOfSignature=function(a){return this.data.lastIndexOf(a)-this.zero},d.prototype.readAndCheckSignature=function(a){var b=this.readData(4);return a===b},d.prototype.readData=function(a){this.checkOffset(a);var b=this.data.slice(this.zero+this.index,this.zero+this.index+a);return this.index+=a,b},b.exports=d},{"../utils":32,"./DataReader":18}],21:[function(a,b,c){"use strict";function d(a){e.call(this,a)}var e=a("./ArrayReader"),f=a("../utils");f.inherits(d,e),d.prototype.readData=function(a){if(this.checkOffset(a),0===a)return new Uint8Array(0);var b=this.data.subarray(this.zero+this.index,this.zero+this.index+a);return this.index+=a,b},b.exports=d},{"../utils":32,"./ArrayReader":17}],22:[function(a,b,c){"use strict";var d=a("../utils"),e=a("../support"),f=a("./ArrayReader"),g=a("./StringReader"),h=a("./NodeBufferReader"),i=a("./Uint8ArrayReader");b.exports=function(a){var b=d.getTypeOf(a);return d.checkSupport(b),"string"!==b||e.uint8array?"nodebuffer"===b?new h(a):e.uint8array?new i(d.transformTo("uint8array",a)):new f(d.transformTo("array",a)):new g(a)}},{"../support":30,"../utils":32,"./ArrayReader":17,"./NodeBufferReader":19,"./StringReader":20,"./Uint8ArrayReader":21}],23:[function(a,b,c){"use strict";c.LOCAL_FILE_HEADER="PK",c.CENTRAL_FILE_HEADER="PK",c.CENTRAL_DIRECTORY_END="PK",c.ZIP64_CENTRAL_DIRECTORY_LOCATOR="PK",c.ZIP64_CENTRAL_DIRECTORY_END="PK",c.DATA_DESCRIPTOR="PK\b"},{}],24:[function(a,b,c){"use strict";function d(a){e.call(this,"ConvertWorker to "+a),this.destType=a}var e=a("./GenericWorker"),f=a("../utils");f.inherits(d,e),d.prototype.processChunk=function(a){this.push({data:f.transformTo(this.destType,a.data),meta:a.meta})},b.exports=d},{"../utils":32,"./GenericWorker":28}],25:[function(a,b,c){"use strict";function d(){e.call(this,"Crc32Probe"),this.withStreamInfo("crc32",0)}var e=a("./GenericWorker"),f=a("../crc32"),g=a("../utils");g.inherits(d,e),d.prototype.processChunk=function(a){this.streamInfo.crc32=f(a.data,this.streamInfo.crc32||0),this.push(a)},b.exports=d},{"../crc32":4,"../utils":32,"./GenericWorker":28}],26:[function(a,b,c){"use strict";function d(a){f.call(this,"DataLengthProbe for "+a),this.propName=a,this.withStreamInfo(a,0)}var e=a("../utils"),f=a("./GenericWorker");e.inherits(d,f),d.prototype.processChunk=function(a){if(a){var b=this.streamInfo[this.propName]||0;this.streamInfo[this.propName]=b+a.data.length}f.prototype.processChunk.call(this,a)},b.exports=d},{"../utils":32,"./GenericWorker":28}],27:[function(a,b,c){"use strict";function d(a){f.call(this,"DataWorker");var b=this;this.dataIsReady=!1,this.index=0,this.max=0,this.data=null,this.type="",this._tickScheduled=!1,a.then(function(a){b.dataIsReady=!0,b.data=a,b.max=a&&a.length||0,b.type=e.getTypeOf(a),b.isPaused||b._tickAndRepeat()},function(a){b.error(a)})}var e=a("../utils"),f=a("./GenericWorker"),g=16384;e.inherits(d,f),d.prototype.cleanUp=function(){f.prototype.cleanUp.call(this),this.data=null},d.prototype.resume=function(){return!!f.prototype.resume.call(this)&&(!this._tickScheduled&&this.dataIsReady&&(this._tickScheduled=!0,e.delay(this._tickAndRepeat,[],this)),!0)},d.prototype._tickAndRepeat=function(){this._tickScheduled=!1,this.isPaused||this.isFinished||(this._tick(),this.isFinished||(e.delay(this._tickAndRepeat,[],this),this._tickScheduled=!0))},d.prototype._tick=function(){if(this.isPaused||this.isFinished)return!1;var a=g,b=null,c=Math.min(this.max,this.index+a);if(this.index>=this.max)return this.end();switch(this.type){case"string":b=this.data.substring(this.index,c);break;case"uint8array":b=this.data.subarray(this.index,c);break;case"array":case"nodebuffer":b=this.data.slice(this.index,c)}return this.index=c,this.push({data:b,meta:{percent:this.max?this.index/this.max*100:0}})},b.exports=d},{"../utils":32,"./GenericWorker":28}],28:[function(a,b,c){"use strict";function d(a){this.name=a||"default",this.streamInfo={},this.generatedError=null,this.extraStreamInfo={},this.isPaused=!0,this.isFinished=!1,this.isLocked=!1,this._listeners={data:[],end:[],error:[]},this.previous=null}d.prototype={push:function(a){this.emit("data",a)},end:function(){if(this.isFinished)return!1;this.flush();try{this.emit("end"),this.cleanUp(),this.isFinished=!0}catch(a){this.emit("error",a)}return!0},error:function(a){return!this.isFinished&&(this.isPaused?this.generatedError=a:(this.isFinished=!0,this.emit("error",a),this.previous&&this.previous.error(a),this.cleanUp()),!0)},on:function(a,b){return this._listeners[a].push(b),this},cleanUp:function(){this.streamInfo=this.generatedError=this.extraStreamInfo=null,this._listeners=[]},emit:function(a,b){if(this._listeners[a])for(var c=0;c<this._listeners[a].length;c++)this._listeners[a][c].call(this,b)},pipe:function(a){return a.registerPrevious(this)},registerPrevious:function(a){if(this.isLocked)throw new Error("The stream '"+this+"' has already been used.");this.streamInfo=a.streamInfo,this.mergeStreamInfo(),this.previous=a;var b=this;return a.on("data",function(a){b.processChunk(a)}),a.on("end",function(){b.end()}),a.on("error",function(a){b.error(a)}),this},pause:function(){return!this.isPaused&&!this.isFinished&&(this.isPaused=!0,this.previous&&this.previous.pause(),!0)},resume:function(){if(!this.isPaused||this.isFinished)return!1;this.isPaused=!1;var a=!1;return this.generatedError&&(this.error(this.generatedError),a=!0),this.previous&&this.previous.resume(),!a},flush:function(){},processChunk:function(a){this.push(a)},withStreamInfo:function(a,b){return this.extraStreamInfo[a]=b,this.mergeStreamInfo(),this},mergeStreamInfo:function(){for(var a in this.extraStreamInfo)this.extraStreamInfo.hasOwnProperty(a)&&(this.streamInfo[a]=this.extraStreamInfo[a])},lock:function(){if(this.isLocked)throw new Error("The stream '"+this+"' has already been used.");this.isLocked=!0,this.previous&&this.previous.lock()},toString:function(){var a="Worker "+this.name;return this.previous?this.previous+" -> "+a:a}},b.exports=d},{}],29:[function(a,b,c){"use strict";function d(a,b,c){switch(a){case"blob":return h.newBlob(h.transformTo("arraybuffer",b),c);case"base64":return k.encode(b);default:return h.transformTo(a,b)}}function e(a,b){var c,d=0,e=null,f=0;for(c=0;c<b.length;c++)f+=b[c].length;switch(a){case"string":return b.join("");case"array":return Array.prototype.concat.apply([],b);case"uint8array":for(e=new Uint8Array(f),c=0;c<b.length;c++)e.set(b[c],d),d+=b[c].length;return e;case"nodebuffer":return Buffer.concat(b);default:throw new Error("concat : unsupported type '"+a+"'")}}function f(a,b){return new m.Promise(function(c,f){var g=[],h=a._internalType,i=a._outputType,j=a._mimeType;a.on("data",function(a,c){g.push(a),b&&b(c)}).on("error",function(a){g=[],f(a)}).on("end",function(){try{var a=d(i,e(h,g),j);c(a)}catch(b){f(b)}g=[]}).resume()})}function g(a,b,c){var d=b;switch(b){case"blob":case"arraybuffer":d="uint8array";break;case"base64":d="string"}try{this._internalType=d,this._outputType=b,this._mimeType=c,h.checkSupport(d),this._worker=a.pipe(new i(d)),a.lock()}catch(e){this._worker=new j("error"),this._worker.error(e)}}var h=a("../utils"),i=a("./ConvertWorker"),j=a("./GenericWorker"),k=a("../base64"),l=a("../support"),m=a("../external"),n=null;if(l.nodestream)try{n=a("../nodejs/NodejsStreamOutputAdapter")}catch(o){}g.prototype={accumulate:function(a){return f(this,a)},on:function(a,b){var c=this;return"data"===a?this._worker.on(a,function(a){b.call(c,a.data,a.meta)}):this._worker.on(a,function(){h.delay(b,arguments,c)}),this},resume:function(){return h.delay(this._worker.resume,[],this._worker),this},pause:function(){return this._worker.pause(),this},toNodejsStream:function(a){if(h.checkSupport("nodestream"),"nodebuffer"!==this._outputType)throw new Error(this._outputType+" is not supported by this method");return new n(this,{objectMode:"nodebuffer"!==this._outputType},a)}},b.exports=g},{"../base64":1,"../external":6,"../nodejs/NodejsStreamOutputAdapter":13,"../support":30,"../utils":32,"./ConvertWorker":24,"./GenericWorker":28}],30:[function(a,b,c){"use strict";if(c.base64=!0,c.array=!0,c.string=!0,c.arraybuffer="undefined"!=typeof ArrayBuffer&&"undefined"!=typeof Uint8Array,c.nodebuffer="undefined"!=typeof Buffer,c.uint8array="undefined"!=typeof Uint8Array,"undefined"==typeof ArrayBuffer)c.blob=!1;else{var d=new ArrayBuffer(0);try{c.blob=0===new Blob([d],{type:"application/zip"}).size}catch(e){try{var f=self.BlobBuilder||self.WebKitBlobBuilder||self.MozBlobBuilder||self.MSBlobBuilder,g=new f;g.append(d),c.blob=0===g.getBlob("application/zip").size}catch(e){c.blob=!1}}}try{c.nodestream=!!a("readable-stream").Readable}catch(e){c.nodestream=!1}},{"readable-stream":16}],31:[function(a,b,c){"use strict";function d(){i.call(this,"utf-8 decode"),this.leftOver=null}function e(){i.call(this,"utf-8 encode")}for(var f=a("./utils"),g=a("./support"),h=a("./nodejsUtils"),i=a("./stream/GenericWorker"),j=new Array(256),k=0;k<256;k++)j[k]=k>=252?6:k>=248?5:k>=240?4:k>=224?3:k>=192?2:1;j[254]=j[254]=1;var l=function(a){var b,c,d,e,f,h=a.length,i=0;for(e=0;e<h;e++)c=a.charCodeAt(e),55296===(64512&c)&&e+1<h&&(d=a.charCodeAt(e+1),56320===(64512&d)&&(c=65536+(c-55296<<10)+(d-56320),e++)),i+=c<128?1:c<2048?2:c<65536?3:4;for(b=g.uint8array?new Uint8Array(i):new Array(i),f=0,e=0;f<i;e++)c=a.charCodeAt(e),55296===(64512&c)&&e+1<h&&(d=a.charCodeAt(e+1),56320===(64512&d)&&(c=65536+(c-55296<<10)+(d-56320),e++)),c<128?b[f++]=c:c<2048?(b[f++]=192|c>>>6,b[f++]=128|63&c):c<65536?(b[f++]=224|c>>>12,b[f++]=128|c>>>6&63,b[f++]=128|63&c):(b[f++]=240|c>>>18,b[f++]=128|c>>>12&63,b[f++]=128|c>>>6&63,b[f++]=128|63&c);return b},m=function(a,b){var c;for(b=b||a.length,b>a.length&&(b=a.length),c=b-1;c>=0&&128===(192&a[c]);)c--;return c<0?b:0===c?b:c+j[a[c]]>b?c:b},n=function(a){var b,c,d,e,g=a.length,h=new Array(2*g);for(c=0,b=0;b<g;)if(d=a[b++],d<128)h[c++]=d;else if(e=j[d],e>4)h[c++]=65533,b+=e-1;else{for(d&=2===e?31:3===e?15:7;e>1&&b<g;)d=d<<6|63&a[b++],e--;e>1?h[c++]=65533:d<65536?h[c++]=d:(d-=65536,h[c++]=55296|d>>10&1023,h[c++]=56320|1023&d)}return h.length!==c&&(h.subarray?h=h.subarray(0,c):h.length=c),f.applyFromCharCode(h)};c.utf8encode=function(a){return g.nodebuffer?h.newBufferFrom(a,"utf-8"):l(a)},c.utf8decode=function(a){return g.nodebuffer?f.transformTo("nodebuffer",a).toString("utf-8"):(a=f.transformTo(g.uint8array?"uint8array":"array",a),n(a))},f.inherits(d,i),d.prototype.processChunk=function(a){var b=f.transformTo(g.uint8array?"uint8array":"array",a.data);if(this.leftOver&&this.leftOver.length){if(g.uint8array){var d=b;b=new Uint8Array(d.length+this.leftOver.length),b.set(this.leftOver,0),b.set(d,this.leftOver.length)}else b=this.leftOver.concat(b);this.leftOver=null}var e=m(b),h=b;e!==b.length&&(g.uint8array?(h=b.subarray(0,e),this.leftOver=b.subarray(e,b.length)):(h=b.slice(0,e),this.leftOver=b.slice(e,b.length))),this.push({data:c.utf8decode(h),meta:a.meta})},d.prototype.flush=function(){this.leftOver&&this.leftOver.length&&(this.push({data:c.utf8decode(this.leftOver),meta:{}}),this.leftOver=null)},c.Utf8DecodeWorker=d,f.inherits(e,i),e.prototype.processChunk=function(a){this.push({data:c.utf8encode(a.data),meta:a.meta})},c.Utf8EncodeWorker=e},{"./nodejsUtils":14,"./stream/GenericWorker":28,"./support":30,"./utils":32}],32:[function(a,b,c){"use strict";function d(a){var b=null;return b=i.uint8array?new Uint8Array(a.length):new Array(a.length),f(a,b)}function e(a){return a}function f(a,b){for(var c=0;c<a.length;++c)b[c]=255&a.charCodeAt(c);return b}function g(a){var b=65536,d=c.getTypeOf(a),e=!0;if("uint8array"===d?e=n.applyCanBeUsed.uint8array:"nodebuffer"===d&&(e=n.applyCanBeUsed.nodebuffer),e)for(;b>1;)try{return n.stringifyByChunk(a,d,b)}catch(f){b=Math.floor(b/2)}return n.stringifyByChar(a)}function h(a,b){for(var c=0;c<a.length;c++)b[c]=a[c];
return b}var i=a("./support"),j=a("./base64"),k=a("./nodejsUtils"),l=a("core-js/library/fn/set-immediate"),m=a("./external");c.newBlob=function(a,b){c.checkSupport("blob");try{return new Blob([a],{type:b})}catch(d){try{var e=self.BlobBuilder||self.WebKitBlobBuilder||self.MozBlobBuilder||self.MSBlobBuilder,f=new e;return f.append(a),f.getBlob(b)}catch(d){throw new Error("Bug : can't construct the Blob.")}}};var n={stringifyByChunk:function(a,b,c){var d=[],e=0,f=a.length;if(f<=c)return String.fromCharCode.apply(null,a);for(;e<f;)"array"===b||"nodebuffer"===b?d.push(String.fromCharCode.apply(null,a.slice(e,Math.min(e+c,f)))):d.push(String.fromCharCode.apply(null,a.subarray(e,Math.min(e+c,f)))),e+=c;return d.join("")},stringifyByChar:function(a){for(var b="",c=0;c<a.length;c++)b+=String.fromCharCode(a[c]);return b},applyCanBeUsed:{uint8array:function(){try{return i.uint8array&&1===String.fromCharCode.apply(null,new Uint8Array(1)).length}catch(a){return!1}}(),nodebuffer:function(){try{return i.nodebuffer&&1===String.fromCharCode.apply(null,k.allocBuffer(1)).length}catch(a){return!1}}()}};c.applyFromCharCode=g;var o={};o.string={string:e,array:function(a){return f(a,new Array(a.length))},arraybuffer:function(a){return o.string.uint8array(a).buffer},uint8array:function(a){return f(a,new Uint8Array(a.length))},nodebuffer:function(a){return f(a,k.allocBuffer(a.length))}},o.array={string:g,array:e,arraybuffer:function(a){return new Uint8Array(a).buffer},uint8array:function(a){return new Uint8Array(a)},nodebuffer:function(a){return k.newBufferFrom(a)}},o.arraybuffer={string:function(a){return g(new Uint8Array(a))},array:function(a){return h(new Uint8Array(a),new Array(a.byteLength))},arraybuffer:e,uint8array:function(a){return new Uint8Array(a)},nodebuffer:function(a){return k.newBufferFrom(new Uint8Array(a))}},o.uint8array={string:g,array:function(a){return h(a,new Array(a.length))},arraybuffer:function(a){return a.buffer},uint8array:e,nodebuffer:function(a){return k.newBufferFrom(a)}},o.nodebuffer={string:g,array:function(a){return h(a,new Array(a.length))},arraybuffer:function(a){return o.nodebuffer.uint8array(a).buffer},uint8array:function(a){return h(a,new Uint8Array(a.length))},nodebuffer:e},c.transformTo=function(a,b){if(b||(b=""),!a)return b;c.checkSupport(a);var d=c.getTypeOf(b),e=o[d][a](b);return e},c.getTypeOf=function(a){return"string"==typeof a?"string":"[object Array]"===Object.prototype.toString.call(a)?"array":i.nodebuffer&&k.isBuffer(a)?"nodebuffer":i.uint8array&&a instanceof Uint8Array?"uint8array":i.arraybuffer&&a instanceof ArrayBuffer?"arraybuffer":void 0},c.checkSupport=function(a){var b=i[a.toLowerCase()];if(!b)throw new Error(a+" is not supported by this platform")},c.MAX_VALUE_16BITS=65535,c.MAX_VALUE_32BITS=-1,c.pretty=function(a){var b,c,d="";for(c=0;c<(a||"").length;c++)b=a.charCodeAt(c),d+="\\x"+(b<16?"0":"")+b.toString(16).toUpperCase();return d},c.delay=function(a,b,c){l(function(){a.apply(c||null,b||[])})},c.inherits=function(a,b){var c=function(){};c.prototype=b.prototype,a.prototype=new c},c.extend=function(){var a,b,c={};for(a=0;a<arguments.length;a++)for(b in arguments[a])arguments[a].hasOwnProperty(b)&&"undefined"==typeof c[b]&&(c[b]=arguments[a][b]);return c},c.prepareContent=function(a,b,e,f,g){var h=m.Promise.resolve(b).then(function(a){var b=i.blob&&(a instanceof Blob||["[object File]","[object Blob]"].indexOf(Object.prototype.toString.call(a))!==-1);return b&&"undefined"!=typeof FileReader?new m.Promise(function(b,c){var d=new FileReader;d.onload=function(a){b(a.target.result)},d.onerror=function(a){c(a.target.error)},d.readAsArrayBuffer(a)}):a});return h.then(function(b){var h=c.getTypeOf(b);return h?("arraybuffer"===h?b=c.transformTo("uint8array",b):"string"===h&&(g?b=j.decode(b):e&&f!==!0&&(b=d(b))),b):m.Promise.reject(new Error("Can't read the data of '"+a+"'. Is it in a supported JavaScript type (String, Blob, ArrayBuffer, etc) ?"))})}},{"./base64":1,"./external":6,"./nodejsUtils":14,"./support":30,"core-js/library/fn/set-immediate":36}],33:[function(a,b,c){"use strict";function d(a){this.files=[],this.loadOptions=a}var e=a("./reader/readerFor"),f=a("./utils"),g=a("./signature"),h=a("./zipEntry"),i=(a("./utf8"),a("./support"));d.prototype={checkSignature:function(a){if(!this.reader.readAndCheckSignature(a)){this.reader.index-=4;var b=this.reader.readString(4);throw new Error("Corrupted zip or bug: unexpected signature ("+f.pretty(b)+", expected "+f.pretty(a)+")")}},isSignature:function(a,b){var c=this.reader.index;this.reader.setIndex(a);var d=this.reader.readString(4),e=d===b;return this.reader.setIndex(c),e},readBlockEndOfCentral:function(){this.diskNumber=this.reader.readInt(2),this.diskWithCentralDirStart=this.reader.readInt(2),this.centralDirRecordsOnThisDisk=this.reader.readInt(2),this.centralDirRecords=this.reader.readInt(2),this.centralDirSize=this.reader.readInt(4),this.centralDirOffset=this.reader.readInt(4),this.zipCommentLength=this.reader.readInt(2);var a=this.reader.readData(this.zipCommentLength),b=i.uint8array?"uint8array":"array",c=f.transformTo(b,a);this.zipComment=this.loadOptions.decodeFileName(c)},readBlockZip64EndOfCentral:function(){this.zip64EndOfCentralSize=this.reader.readInt(8),this.reader.skip(4),this.diskNumber=this.reader.readInt(4),this.diskWithCentralDirStart=this.reader.readInt(4),this.centralDirRecordsOnThisDisk=this.reader.readInt(8),this.centralDirRecords=this.reader.readInt(8),this.centralDirSize=this.reader.readInt(8),this.centralDirOffset=this.reader.readInt(8),this.zip64ExtensibleData={};for(var a,b,c,d=this.zip64EndOfCentralSize-44,e=0;e<d;)a=this.reader.readInt(2),b=this.reader.readInt(4),c=this.reader.readData(b),this.zip64ExtensibleData[a]={id:a,length:b,value:c}},readBlockZip64EndOfCentralLocator:function(){if(this.diskWithZip64CentralDirStart=this.reader.readInt(4),this.relativeOffsetEndOfZip64CentralDir=this.reader.readInt(8),this.disksCount=this.reader.readInt(4),this.disksCount>1)throw new Error("Multi-volumes zip are not supported")},readLocalFiles:function(){var a,b;for(a=0;a<this.files.length;a++)b=this.files[a],this.reader.setIndex(b.localHeaderOffset),this.checkSignature(g.LOCAL_FILE_HEADER),b.readLocalPart(this.reader),b.handleUTF8(),b.processAttributes()},readCentralDir:function(){var a;for(this.reader.setIndex(this.centralDirOffset);this.reader.readAndCheckSignature(g.CENTRAL_FILE_HEADER);)a=new h({zip64:this.zip64},this.loadOptions),a.readCentralPart(this.reader),this.files.push(a);if(this.centralDirRecords!==this.files.length&&0!==this.centralDirRecords&&0===this.files.length)throw new Error("Corrupted zip or bug: expected "+this.centralDirRecords+" records in central dir, got "+this.files.length)},readEndOfCentral:function(){var a=this.reader.lastIndexOfSignature(g.CENTRAL_DIRECTORY_END);if(a<0){var b=!this.isSignature(0,g.LOCAL_FILE_HEADER);throw b?new Error("Can't find end of central directory : is this a zip file ? If it is, see https://stuk.github.io/jszip/documentation/howto/read_zip.html"):new Error("Corrupted zip: can't find end of central directory")}this.reader.setIndex(a);var c=a;if(this.checkSignature(g.CENTRAL_DIRECTORY_END),this.readBlockEndOfCentral(),this.diskNumber===f.MAX_VALUE_16BITS||this.diskWithCentralDirStart===f.MAX_VALUE_16BITS||this.centralDirRecordsOnThisDisk===f.MAX_VALUE_16BITS||this.centralDirRecords===f.MAX_VALUE_16BITS||this.centralDirSize===f.MAX_VALUE_32BITS||this.centralDirOffset===f.MAX_VALUE_32BITS){if(this.zip64=!0,a=this.reader.lastIndexOfSignature(g.ZIP64_CENTRAL_DIRECTORY_LOCATOR),a<0)throw new Error("Corrupted zip: can't find the ZIP64 end of central directory locator");if(this.reader.setIndex(a),this.checkSignature(g.ZIP64_CENTRAL_DIRECTORY_LOCATOR),this.readBlockZip64EndOfCentralLocator(),!this.isSignature(this.relativeOffsetEndOfZip64CentralDir,g.ZIP64_CENTRAL_DIRECTORY_END)&&(this.relativeOffsetEndOfZip64CentralDir=this.reader.lastIndexOfSignature(g.ZIP64_CENTRAL_DIRECTORY_END),this.relativeOffsetEndOfZip64CentralDir<0))throw new Error("Corrupted zip: can't find the ZIP64 end of central directory");this.reader.setIndex(this.relativeOffsetEndOfZip64CentralDir),this.checkSignature(g.ZIP64_CENTRAL_DIRECTORY_END),this.readBlockZip64EndOfCentral()}var d=this.centralDirOffset+this.centralDirSize;this.zip64&&(d+=20,d+=12+this.zip64EndOfCentralSize);var e=c-d;if(e>0)this.isSignature(c,g.CENTRAL_FILE_HEADER)||(this.reader.zero=e);else if(e<0)throw new Error("Corrupted zip: missing "+Math.abs(e)+" bytes.")},prepareReader:function(a){this.reader=e(a)},load:function(a){this.prepareReader(a),this.readEndOfCentral(),this.readCentralDir(),this.readLocalFiles()}},b.exports=d},{"./reader/readerFor":22,"./signature":23,"./support":30,"./utf8":31,"./utils":32,"./zipEntry":34}],34:[function(a,b,c){"use strict";function d(a,b){this.options=a,this.loadOptions=b}var e=a("./reader/readerFor"),f=a("./utils"),g=a("./compressedObject"),h=a("./crc32"),i=a("./utf8"),j=a("./compressions"),k=a("./support"),l=0,m=3,n=function(a){for(var b in j)if(j.hasOwnProperty(b)&&j[b].magic===a)return j[b];return null};d.prototype={isEncrypted:function(){return 1===(1&this.bitFlag)},useUTF8:function(){return 2048===(2048&this.bitFlag)},readLocalPart:function(a){var b,c;if(a.skip(22),this.fileNameLength=a.readInt(2),c=a.readInt(2),this.fileName=a.readData(this.fileNameLength),a.skip(c),this.compressedSize===-1||this.uncompressedSize===-1)throw new Error("Bug or corrupted zip : didn't get enough informations from the central directory (compressedSize === -1 || uncompressedSize === -1)");if(b=n(this.compressionMethod),null===b)throw new Error("Corrupted zip : compression "+f.pretty(this.compressionMethod)+" unknown (inner file : "+f.transformTo("string",this.fileName)+")");this.decompressed=new g(this.compressedSize,this.uncompressedSize,this.crc32,b,a.readData(this.compressedSize))},readCentralPart:function(a){this.versionMadeBy=a.readInt(2),a.skip(2),this.bitFlag=a.readInt(2),this.compressionMethod=a.readString(2),this.date=a.readDate(),this.crc32=a.readInt(4),this.compressedSize=a.readInt(4),this.uncompressedSize=a.readInt(4);var b=a.readInt(2);if(this.extraFieldsLength=a.readInt(2),this.fileCommentLength=a.readInt(2),this.diskNumberStart=a.readInt(2),this.internalFileAttributes=a.readInt(2),this.externalFileAttributes=a.readInt(4),this.localHeaderOffset=a.readInt(4),this.isEncrypted())throw new Error("Encrypted zip are not supported");a.skip(b),this.readExtraFields(a),this.parseZIP64ExtraField(a),this.fileComment=a.readData(this.fileCommentLength)},processAttributes:function(){this.unixPermissions=null,this.dosPermissions=null;var a=this.versionMadeBy>>8;this.dir=!!(16&this.externalFileAttributes),a===l&&(this.dosPermissions=63&this.externalFileAttributes),a===m&&(this.unixPermissions=this.externalFileAttributes>>16&65535),this.dir||"/"!==this.fileNameStr.slice(-1)||(this.dir=!0)},parseZIP64ExtraField:function(a){if(this.extraFields[1]){var b=e(this.extraFields[1].value);this.uncompressedSize===f.MAX_VALUE_32BITS&&(this.uncompressedSize=b.readInt(8)),this.compressedSize===f.MAX_VALUE_32BITS&&(this.compressedSize=b.readInt(8)),this.localHeaderOffset===f.MAX_VALUE_32BITS&&(this.localHeaderOffset=b.readInt(8)),this.diskNumberStart===f.MAX_VALUE_32BITS&&(this.diskNumberStart=b.readInt(4))}},readExtraFields:function(a){var b,c,d,e=a.index+this.extraFieldsLength;for(this.extraFields||(this.extraFields={});a.index<e;)b=a.readInt(2),c=a.readInt(2),d=a.readData(c),this.extraFields[b]={id:b,length:c,value:d}},handleUTF8:function(){var a=k.uint8array?"uint8array":"array";if(this.useUTF8())this.fileNameStr=i.utf8decode(this.fileName),this.fileCommentStr=i.utf8decode(this.fileComment);else{var b=this.findExtraFieldUnicodePath();if(null!==b)this.fileNameStr=b;else{var c=f.transformTo(a,this.fileName);this.fileNameStr=this.loadOptions.decodeFileName(c)}var d=this.findExtraFieldUnicodeComment();if(null!==d)this.fileCommentStr=d;else{var e=f.transformTo(a,this.fileComment);this.fileCommentStr=this.loadOptions.decodeFileName(e)}}},findExtraFieldUnicodePath:function(){var a=this.extraFields[28789];if(a){var b=e(a.value);return 1!==b.readInt(1)?null:h(this.fileName)!==b.readInt(4)?null:i.utf8decode(b.readData(a.length-5))}return null},findExtraFieldUnicodeComment:function(){var a=this.extraFields[25461];if(a){var b=e(a.value);return 1!==b.readInt(1)?null:h(this.fileComment)!==b.readInt(4)?null:i.utf8decode(b.readData(a.length-5))}return null}},b.exports=d},{"./compressedObject":2,"./compressions":3,"./crc32":4,"./reader/readerFor":22,"./support":30,"./utf8":31,"./utils":32}],35:[function(a,b,c){"use strict";var d=a("./stream/StreamHelper"),e=a("./stream/DataWorker"),f=a("./utf8"),g=a("./compressedObject"),h=a("./stream/GenericWorker"),i=function(a,b,c){this.name=a,this.dir=c.dir,this.date=c.date,this.comment=c.comment,this.unixPermissions=c.unixPermissions,this.dosPermissions=c.dosPermissions,this._data=b,this._dataBinary=c.binary,this.options={compression:c.compression,compressionOptions:c.compressionOptions}};i.prototype={internalStream:function(a){var b=null,c="string";try{if(!a)throw new Error("No output type specified.");c=a.toLowerCase();var e="string"===c||"text"===c;"binarystring"!==c&&"text"!==c||(c="string"),b=this._decompressWorker();var g=!this._dataBinary;g&&!e&&(b=b.pipe(new f.Utf8EncodeWorker)),!g&&e&&(b=b.pipe(new f.Utf8DecodeWorker))}catch(i){b=new h("error"),b.error(i)}return new d(b,c,"")},async:function(a,b){return this.internalStream(a).accumulate(b)},nodeStream:function(a,b){return this.internalStream(a||"nodebuffer").toNodejsStream(b)},_compressWorker:function(a,b){if(this._data instanceof g&&this._data.compression.magic===a.magic)return this._data.getCompressedWorker();var c=this._decompressWorker();return this._dataBinary||(c=c.pipe(new f.Utf8EncodeWorker)),g.createWorkerFrom(c,a,b)},_decompressWorker:function(){return this._data instanceof g?this._data.getContentWorker():this._data instanceof h?this._data:new e(this._data)}};for(var j=["asText","asBinary","asNodeBuffer","asUint8Array","asArrayBuffer"],k=function(){throw new Error("This method has been removed in JSZip 3.0, please check the upgrade guide.")},l=0;l<j.length;l++)i.prototype[j[l]]=k;b.exports=i},{"./compressedObject":2,"./stream/DataWorker":27,"./stream/GenericWorker":28,"./stream/StreamHelper":29,"./utf8":31}],36:[function(a,b,c){a("../modules/web.immediate"),b.exports=a("../modules/_core").setImmediate},{"../modules/_core":40,"../modules/web.immediate":56}],37:[function(a,b,c){b.exports=function(a){if("function"!=typeof a)throw TypeError(a+" is not a function!");return a}},{}],38:[function(a,b,c){var d=a("./_is-object");b.exports=function(a){if(!d(a))throw TypeError(a+" is not an object!");return a}},{"./_is-object":51}],39:[function(a,b,c){var d={}.toString;b.exports=function(a){return d.call(a).slice(8,-1)}},{}],40:[function(a,b,c){var d=b.exports={version:"2.3.0"};"number"==typeof __e&&(__e=d)},{}],41:[function(a,b,c){var d=a("./_a-function");b.exports=function(a,b,c){if(d(a),void 0===b)return a;switch(c){case 1:return function(c){return a.call(b,c)};case 2:return function(c,d){return a.call(b,c,d)};case 3:return function(c,d,e){return a.call(b,c,d,e)}}return function(){return a.apply(b,arguments)}}},{"./_a-function":37}],42:[function(a,b,c){b.exports=!a("./_fails")(function(){return 7!=Object.defineProperty({},"a",{get:function(){return 7}}).a})},{"./_fails":45}],43:[function(a,b,c){var d=a("./_is-object"),e=a("./_global").document,f=d(e)&&d(e.createElement);b.exports=function(a){return f?e.createElement(a):{}}},{"./_global":46,"./_is-object":51}],44:[function(a,b,c){var d=a("./_global"),e=a("./_core"),f=a("./_ctx"),g=a("./_hide"),h="prototype",i=function(a,b,c){var j,k,l,m=a&i.F,n=a&i.G,o=a&i.S,p=a&i.P,q=a&i.B,r=a&i.W,s=n?e:e[b]||(e[b]={}),t=s[h],u=n?d:o?d[b]:(d[b]||{})[h];n&&(c=b);for(j in c)k=!m&&u&&void 0!==u[j],k&&j in s||(l=k?u[j]:c[j],s[j]=n&&"function"!=typeof u[j]?c[j]:q&&k?f(l,d):r&&u[j]==l?function(a){var b=function(b,c,d){if(this instanceof a){switch(arguments.length){case 0:return new a;case 1:return new a(b);case 2:return new a(b,c)}return new a(b,c,d)}return a.apply(this,arguments)};return b[h]=a[h],b}(l):p&&"function"==typeof l?f(Function.call,l):l,p&&((s.virtual||(s.virtual={}))[j]=l,a&i.R&&t&&!t[j]&&g(t,j,l)))};i.F=1,i.G=2,i.S=4,i.P=8,i.B=16,i.W=32,i.U=64,i.R=128,b.exports=i},{"./_core":40,"./_ctx":41,"./_global":46,"./_hide":47}],45:[function(a,b,c){b.exports=function(a){try{return!!a()}catch(b){return!0}}},{}],46:[function(a,b,c){var d=b.exports="undefined"!=typeof window&&window.Math==Math?window:"undefined"!=typeof self&&self.Math==Math?self:Function("return this")();"number"==typeof __g&&(__g=d)},{}],47:[function(a,b,c){var d=a("./_object-dp"),e=a("./_property-desc");b.exports=a("./_descriptors")?function(a,b,c){return d.f(a,b,e(1,c))}:function(a,b,c){return a[b]=c,a}},{"./_descriptors":42,"./_object-dp":52,"./_property-desc":53}],48:[function(a,b,c){b.exports=a("./_global").document&&document.documentElement},{"./_global":46}],49:[function(a,b,c){b.exports=!a("./_descriptors")&&!a("./_fails")(function(){return 7!=Object.defineProperty(a("./_dom-create")("div"),"a",{get:function(){return 7}}).a})},{"./_descriptors":42,"./_dom-create":43,"./_fails":45}],50:[function(a,b,c){b.exports=function(a,b,c){var d=void 0===c;switch(b.length){case 0:return d?a():a.call(c);case 1:return d?a(b[0]):a.call(c,b[0]);case 2:return d?a(b[0],b[1]):a.call(c,b[0],b[1]);case 3:return d?a(b[0],b[1],b[2]):a.call(c,b[0],b[1],b[2]);case 4:return d?a(b[0],b[1],b[2],b[3]):a.call(c,b[0],b[1],b[2],b[3])}return a.apply(c,b)}},{}],51:[function(a,b,c){b.exports=function(a){return"object"==typeof a?null!==a:"function"==typeof a}},{}],52:[function(a,b,c){var d=a("./_an-object"),e=a("./_ie8-dom-define"),f=a("./_to-primitive"),g=Object.defineProperty;c.f=a("./_descriptors")?Object.defineProperty:function(a,b,c){if(d(a),b=f(b,!0),d(c),e)try{return g(a,b,c)}catch(h){}if("get"in c||"set"in c)throw TypeError("Accessors not supported!");return"value"in c&&(a[b]=c.value),a}},{"./_an-object":38,"./_descriptors":42,"./_ie8-dom-define":49,"./_to-primitive":55}],53:[function(a,b,c){b.exports=function(a,b){return{enumerable:!(1&a),configurable:!(2&a),writable:!(4&a),value:b}}},{}],54:[function(a,b,c){var d,e,f,g=a("./_ctx"),h=a("./_invoke"),i=a("./_html"),j=a("./_dom-create"),k=a("./_global"),l=k.process,m=k.setImmediate,n=k.clearImmediate,o=k.MessageChannel,p=0,q={},r="onreadystatechange",s=function(){var a=+this;if(q.hasOwnProperty(a)){var b=q[a];delete q[a],b()}},t=function(a){s.call(a.data)};m&&n||(m=function(a){for(var b=[],c=1;arguments.length>c;)b.push(arguments[c++]);return q[++p]=function(){h("function"==typeof a?a:Function(a),b)},d(p),p},n=function(a){delete q[a]},"process"==a("./_cof")(l)?d=function(a){l.nextTick(g(s,a,1))}:o?(e=new o,f=e.port2,e.port1.onmessage=t,d=g(f.postMessage,f,1)):k.addEventListener&&"function"==typeof postMessage&&!k.importScripts?(d=function(a){k.postMessage(a+"","*")},k.addEventListener("message",t,!1)):d=r in j("script")?function(a){i.appendChild(j("script"))[r]=function(){i.removeChild(this),s.call(a)}}:function(a){setTimeout(g(s,a,1),0)}),b.exports={set:m,clear:n}},{"./_cof":39,"./_ctx":41,"./_dom-create":43,"./_global":46,"./_html":48,"./_invoke":50}],55:[function(a,b,c){var d=a("./_is-object");b.exports=function(a,b){if(!d(a))return a;var c,e;if(b&&"function"==typeof(c=a.toString)&&!d(e=c.call(a)))return e;if("function"==typeof(c=a.valueOf)&&!d(e=c.call(a)))return e;if(!b&&"function"==typeof(c=a.toString)&&!d(e=c.call(a)))return e;throw TypeError("Can't convert object to primitive value")}},{"./_is-object":51}],56:[function(a,b,c){var d=a("./_export"),e=a("./_task");d(d.G+d.B,{setImmediate:e.set,clearImmediate:e.clear})},{"./_export":44,"./_task":54}],57:[function(a,b,c){(function(a){"use strict";function c(){k=!0;for(var a,b,c=l.length;c;){for(b=l,l=[],a=-1;++a<c;)b[a]();c=l.length}k=!1}function d(a){1!==l.push(a)||k||e()}var e,f=a.MutationObserver||a.WebKitMutationObserver;if(f){var g=0,h=new f(c),i=a.document.createTextNode("");h.observe(i,{characterData:!0}),e=function(){i.data=g=++g%2}}else if(a.setImmediate||"undefined"==typeof a.MessageChannel)e="document"in a&&"onreadystatechange"in a.document.createElement("script")?function(){var b=a.document.createElement("script");b.onreadystatechange=function(){c(),b.onreadystatechange=null,b.parentNode.removeChild(b),b=null},a.document.documentElement.appendChild(b)}:function(){setTimeout(c,0)};else{var j=new a.MessageChannel;j.port1.onmessage=c,e=function(){j.port2.postMessage(0)}}var k,l=[];b.exports=d}).call(this,"undefined"!=typeof global?global:"undefined"!=typeof self?self:"undefined"!=typeof window?window:{})},{}],58:[function(a,b,c){"use strict";function d(){}function e(a){if("function"!=typeof a)throw new TypeError("resolver must be a function");this.state=s,this.queue=[],this.outcome=void 0,a!==d&&i(this,a)}function f(a,b,c){this.promise=a,"function"==typeof b&&(this.onFulfilled=b,this.callFulfilled=this.otherCallFulfilled),"function"==typeof c&&(this.onRejected=c,this.callRejected=this.otherCallRejected)}function g(a,b,c){o(function(){var d;try{d=b(c)}catch(e){return p.reject(a,e)}d===a?p.reject(a,new TypeError("Cannot resolve promise with itself")):p.resolve(a,d)})}function h(a){var b=a&&a.then;if(a&&("object"==typeof a||"function"==typeof a)&&"function"==typeof b)return function(){b.apply(a,arguments)}}function i(a,b){function c(b){f||(f=!0,p.reject(a,b))}function d(b){f||(f=!0,p.resolve(a,b))}function e(){b(d,c)}var f=!1,g=j(e);"error"===g.status&&c(g.value)}function j(a,b){var c={};try{c.value=a(b),c.status="success"}catch(d){c.status="error",c.value=d}return c}function k(a){return a instanceof this?a:p.resolve(new this(d),a)}function l(a){var b=new this(d);return p.reject(b,a)}function m(a){function b(a,b){function d(a){g[b]=a,++h!==e||f||(f=!0,p.resolve(j,g))}c.resolve(a).then(d,function(a){f||(f=!0,p.reject(j,a))})}var c=this;if("[object Array]"!==Object.prototype.toString.call(a))return this.reject(new TypeError("must be an array"));var e=a.length,f=!1;if(!e)return this.resolve([]);for(var g=new Array(e),h=0,i=-1,j=new this(d);++i<e;)b(a[i],i);return j}function n(a){function b(a){c.resolve(a).then(function(a){f||(f=!0,p.resolve(h,a))},function(a){f||(f=!0,p.reject(h,a))})}var c=this;if("[object Array]"!==Object.prototype.toString.call(a))return this.reject(new TypeError("must be an array"));var e=a.length,f=!1;if(!e)return this.resolve([]);for(var g=-1,h=new this(d);++g<e;)b(a[g]);return h}var o=a("immediate"),p={},q=["REJECTED"],r=["FULFILLED"],s=["PENDING"];b.exports=e,e.prototype["catch"]=function(a){return this.then(null,a)},e.prototype.then=function(a,b){if("function"!=typeof a&&this.state===r||"function"!=typeof b&&this.state===q)return this;var c=new this.constructor(d);if(this.state!==s){var e=this.state===r?a:b;g(c,e,this.outcome)}else this.queue.push(new f(c,a,b));return c},f.prototype.callFulfilled=function(a){p.resolve(this.promise,a)},f.prototype.otherCallFulfilled=function(a){g(this.promise,this.onFulfilled,a)},f.prototype.callRejected=function(a){p.reject(this.promise,a)},f.prototype.otherCallRejected=function(a){g(this.promise,this.onRejected,a)},p.resolve=function(a,b){var c=j(h,b);if("error"===c.status)return p.reject(a,c.value);var d=c.value;if(d)i(a,d);else{a.state=r,a.outcome=b;for(var e=-1,f=a.queue.length;++e<f;)a.queue[e].callFulfilled(b)}return a},p.reject=function(a,b){a.state=q,a.outcome=b;for(var c=-1,d=a.queue.length;++c<d;)a.queue[c].callRejected(b);return a},e.resolve=k,e.reject=l,e.all=m,e.race=n},{immediate:57}],59:[function(a,b,c){"use strict";var d=a("./lib/utils/common").assign,e=a("./lib/deflate"),f=a("./lib/inflate"),g=a("./lib/zlib/constants"),h={};d(h,e,f,g),b.exports=h},{"./lib/deflate":60,"./lib/inflate":61,"./lib/utils/common":62,"./lib/zlib/constants":65}],60:[function(a,b,c){"use strict";function d(a){if(!(this instanceof d))return new d(a);this.options=i.assign({level:s,method:u,chunkSize:16384,windowBits:15,memLevel:8,strategy:t,to:""},a||{});var b=this.options;b.raw&&b.windowBits>0?b.windowBits=-b.windowBits:b.gzip&&b.windowBits>0&&b.windowBits<16&&(b.windowBits+=16),this.err=0,this.msg="",this.ended=!1,this.chunks=[],this.strm=new l,this.strm.avail_out=0;var c=h.deflateInit2(this.strm,b.level,b.method,b.windowBits,b.memLevel,b.strategy);if(c!==p)throw new Error(k[c]);if(b.header&&h.deflateSetHeader(this.strm,b.header),b.dictionary){var e;if(e="string"==typeof b.dictionary?j.string2buf(b.dictionary):"[object ArrayBuffer]"===m.call(b.dictionary)?new Uint8Array(b.dictionary):b.dictionary,c=h.deflateSetDictionary(this.strm,e),c!==p)throw new Error(k[c]);this._dict_set=!0}}function e(a,b){var c=new d(b);if(c.push(a,!0),c.err)throw c.msg||k[c.err];return c.result}function f(a,b){return b=b||{},b.raw=!0,e(a,b)}function g(a,b){return b=b||{},b.gzip=!0,e(a,b)}var h=a("./zlib/deflate"),i=a("./utils/common"),j=a("./utils/strings"),k=a("./zlib/messages"),l=a("./zlib/zstream"),m=Object.prototype.toString,n=0,o=4,p=0,q=1,r=2,s=-1,t=0,u=8;d.prototype.push=function(a,b){var c,d,e=this.strm,f=this.options.chunkSize;if(this.ended)return!1;d=b===~~b?b:b===!0?o:n,"string"==typeof a?e.input=j.string2buf(a):"[object ArrayBuffer]"===m.call(a)?e.input=new Uint8Array(a):e.input=a,e.next_in=0,e.avail_in=e.input.length;do{if(0===e.avail_out&&(e.output=new i.Buf8(f),e.next_out=0,e.avail_out=f),c=h.deflate(e,d),c!==q&&c!==p)return this.onEnd(c),this.ended=!0,!1;0!==e.avail_out&&(0!==e.avail_in||d!==o&&d!==r)||("string"===this.options.to?this.onData(j.buf2binstring(i.shrinkBuf(e.output,e.next_out))):this.onData(i.shrinkBuf(e.output,e.next_out)))}while((e.avail_in>0||0===e.avail_out)&&c!==q);return d===o?(c=h.deflateEnd(this.strm),this.onEnd(c),this.ended=!0,c===p):d!==r||(this.onEnd(p),e.avail_out=0,!0)},d.prototype.onData=function(a){this.chunks.push(a)},d.prototype.onEnd=function(a){a===p&&("string"===this.options.to?this.result=this.chunks.join(""):this.result=i.flattenChunks(this.chunks)),this.chunks=[],this.err=a,this.msg=this.strm.msg},c.Deflate=d,c.deflate=e,c.deflateRaw=f,c.gzip=g},{"./utils/common":62,"./utils/strings":63,"./zlib/deflate":67,"./zlib/messages":72,"./zlib/zstream":74}],61:[function(a,b,c){"use strict";function d(a){if(!(this instanceof d))return new d(a);this.options=h.assign({chunkSize:16384,windowBits:0,to:""},a||{});var b=this.options;b.raw&&b.windowBits>=0&&b.windowBits<16&&(b.windowBits=-b.windowBits,0===b.windowBits&&(b.windowBits=-15)),!(b.windowBits>=0&&b.windowBits<16)||a&&a.windowBits||(b.windowBits+=32),b.windowBits>15&&b.windowBits<48&&0===(15&b.windowBits)&&(b.windowBits|=15),this.err=0,this.msg="",this.ended=!1,this.chunks=[],this.strm=new l,this.strm.avail_out=0;var c=g.inflateInit2(this.strm,b.windowBits);if(c!==j.Z_OK)throw new Error(k[c]);this.header=new m,g.inflateGetHeader(this.strm,this.header)}function e(a,b){var c=new d(b);if(c.push(a,!0),c.err)throw c.msg||k[c.err];return c.result}function f(a,b){return b=b||{},b.raw=!0,e(a,b)}var g=a("./zlib/inflate"),h=a("./utils/common"),i=a("./utils/strings"),j=a("./zlib/constants"),k=a("./zlib/messages"),l=a("./zlib/zstream"),m=a("./zlib/gzheader"),n=Object.prototype.toString;d.prototype.push=function(a,b){var c,d,e,f,k,l,m=this.strm,o=this.options.chunkSize,p=this.options.dictionary,q=!1;if(this.ended)return!1;d=b===~~b?b:b===!0?j.Z_FINISH:j.Z_NO_FLUSH,"string"==typeof a?m.input=i.binstring2buf(a):"[object ArrayBuffer]"===n.call(a)?m.input=new Uint8Array(a):m.input=a,m.next_in=0,m.avail_in=m.input.length;do{if(0===m.avail_out&&(m.output=new h.Buf8(o),m.next_out=0,m.avail_out=o),c=g.inflate(m,j.Z_NO_FLUSH),c===j.Z_NEED_DICT&&p&&(l="string"==typeof p?i.string2buf(p):"[object ArrayBuffer]"===n.call(p)?new Uint8Array(p):p,c=g.inflateSetDictionary(this.strm,l)),c===j.Z_BUF_ERROR&&q===!0&&(c=j.Z_OK,q=!1),c!==j.Z_STREAM_END&&c!==j.Z_OK)return this.onEnd(c),this.ended=!0,!1;m.next_out&&(0!==m.avail_out&&c!==j.Z_STREAM_END&&(0!==m.avail_in||d!==j.Z_FINISH&&d!==j.Z_SYNC_FLUSH)||("string"===this.options.to?(e=i.utf8border(m.output,m.next_out),f=m.next_out-e,k=i.buf2string(m.output,e),m.next_out=f,m.avail_out=o-f,f&&h.arraySet(m.output,m.output,e,f,0),this.onData(k)):this.onData(h.shrinkBuf(m.output,m.next_out)))),0===m.avail_in&&0===m.avail_out&&(q=!0)}while((m.avail_in>0||0===m.avail_out)&&c!==j.Z_STREAM_END);return c===j.Z_STREAM_END&&(d=j.Z_FINISH),d===j.Z_FINISH?(c=g.inflateEnd(this.strm),this.onEnd(c),this.ended=!0,c===j.Z_OK):d!==j.Z_SYNC_FLUSH||(this.onEnd(j.Z_OK),m.avail_out=0,!0)},d.prototype.onData=function(a){this.chunks.push(a)},d.prototype.onEnd=function(a){a===j.Z_OK&&("string"===this.options.to?this.result=this.chunks.join(""):this.result=h.flattenChunks(this.chunks)),this.chunks=[],this.err=a,this.msg=this.strm.msg},c.Inflate=d,c.inflate=e,c.inflateRaw=f,c.ungzip=e},{"./utils/common":62,"./utils/strings":63,"./zlib/constants":65,"./zlib/gzheader":68,"./zlib/inflate":70,"./zlib/messages":72,"./zlib/zstream":74}],62:[function(a,b,c){"use strict";var d="undefined"!=typeof Uint8Array&&"undefined"!=typeof Uint16Array&&"undefined"!=typeof Int32Array;c.assign=function(a){for(var b=Array.prototype.slice.call(arguments,1);b.length;){var c=b.shift();if(c){if("object"!=typeof c)throw new TypeError(c+"must be non-object");for(var d in c)c.hasOwnProperty(d)&&(a[d]=c[d])}}return a},c.shrinkBuf=function(a,b){return a.length===b?a:a.subarray?a.subarray(0,b):(a.length=b,a)};var e={arraySet:function(a,b,c,d,e){if(b.subarray&&a.subarray)return void a.set(b.subarray(c,c+d),e);for(var f=0;f<d;f++)a[e+f]=b[c+f]},flattenChunks:function(a){var b,c,d,e,f,g;for(d=0,b=0,c=a.length;b<c;b++)d+=a[b].length;for(g=new Uint8Array(d),e=0,b=0,c=a.length;b<c;b++)f=a[b],g.set(f,e),e+=f.length;return g}},f={arraySet:function(a,b,c,d,e){for(var f=0;f<d;f++)a[e+f]=b[c+f]},flattenChunks:function(a){return[].concat.apply([],a)}};c.setTyped=function(a){a?(c.Buf8=Uint8Array,c.Buf16=Uint16Array,c.Buf32=Int32Array,c.assign(c,e)):(c.Buf8=Array,c.Buf16=Array,c.Buf32=Array,c.assign(c,f))},c.setTyped(d)},{}],63:[function(a,b,c){"use strict";function d(a,b){if(b<65537&&(a.subarray&&g||!a.subarray&&f))return String.fromCharCode.apply(null,e.shrinkBuf(a,b));for(var c="",d=0;d<b;d++)c+=String.fromCharCode(a[d]);return c}var e=a("./common"),f=!0,g=!0;try{String.fromCharCode.apply(null,[0])}catch(h){f=!1}try{String.fromCharCode.apply(null,new Uint8Array(1))}catch(h){g=!1}for(var i=new e.Buf8(256),j=0;j<256;j++)i[j]=j>=252?6:j>=248?5:j>=240?4:j>=224?3:j>=192?2:1;i[254]=i[254]=1,c.string2buf=function(a){var b,c,d,f,g,h=a.length,i=0;for(f=0;f<h;f++)c=a.charCodeAt(f),55296===(64512&c)&&f+1<h&&(d=a.charCodeAt(f+1),56320===(64512&d)&&(c=65536+(c-55296<<10)+(d-56320),f++)),i+=c<128?1:c<2048?2:c<65536?3:4;for(b=new e.Buf8(i),g=0,f=0;g<i;f++)c=a.charCodeAt(f),55296===(64512&c)&&f+1<h&&(d=a.charCodeAt(f+1),56320===(64512&d)&&(c=65536+(c-55296<<10)+(d-56320),f++)),c<128?b[g++]=c:c<2048?(b[g++]=192|c>>>6,b[g++]=128|63&c):c<65536?(b[g++]=224|c>>>12,b[g++]=128|c>>>6&63,b[g++]=128|63&c):(b[g++]=240|c>>>18,b[g++]=128|c>>>12&63,b[g++]=128|c>>>6&63,b[g++]=128|63&c);return b},c.buf2binstring=function(a){return d(a,a.length)},c.binstring2buf=function(a){for(var b=new e.Buf8(a.length),c=0,d=b.length;c<d;c++)b[c]=a.charCodeAt(c);return b},c.buf2string=function(a,b){var c,e,f,g,h=b||a.length,j=new Array(2*h);for(e=0,c=0;c<h;)if(f=a[c++],f<128)j[e++]=f;else if(g=i[f],g>4)j[e++]=65533,c+=g-1;else{for(f&=2===g?31:3===g?15:7;g>1&&c<h;)f=f<<6|63&a[c++],g--;g>1?j[e++]=65533:f<65536?j[e++]=f:(f-=65536,j[e++]=55296|f>>10&1023,j[e++]=56320|1023&f)}return d(j,e)},c.utf8border=function(a,b){var c;for(b=b||a.length,b>a.length&&(b=a.length),c=b-1;c>=0&&128===(192&a[c]);)c--;return c<0?b:0===c?b:c+i[a[c]]>b?c:b}},{"./common":62}],64:[function(a,b,c){"use strict";function d(a,b,c,d){for(var e=65535&a|0,f=a>>>16&65535|0,g=0;0!==c;){g=c>2e3?2e3:c,c-=g;do e=e+b[d++]|0,f=f+e|0;while(--g);e%=65521,f%=65521}return e|f<<16|0;
}b.exports=d},{}],65:[function(a,b,c){"use strict";b.exports={Z_NO_FLUSH:0,Z_PARTIAL_FLUSH:1,Z_SYNC_FLUSH:2,Z_FULL_FLUSH:3,Z_FINISH:4,Z_BLOCK:5,Z_TREES:6,Z_OK:0,Z_STREAM_END:1,Z_NEED_DICT:2,Z_ERRNO:-1,Z_STREAM_ERROR:-2,Z_DATA_ERROR:-3,Z_BUF_ERROR:-5,Z_NO_COMPRESSION:0,Z_BEST_SPEED:1,Z_BEST_COMPRESSION:9,Z_DEFAULT_COMPRESSION:-1,Z_FILTERED:1,Z_HUFFMAN_ONLY:2,Z_RLE:3,Z_FIXED:4,Z_DEFAULT_STRATEGY:0,Z_BINARY:0,Z_TEXT:1,Z_UNKNOWN:2,Z_DEFLATED:8}},{}],66:[function(a,b,c){"use strict";function d(){for(var a,b=[],c=0;c<256;c++){a=c;for(var d=0;d<8;d++)a=1&a?3988292384^a>>>1:a>>>1;b[c]=a}return b}function e(a,b,c,d){var e=f,g=d+c;a^=-1;for(var h=d;h<g;h++)a=a>>>8^e[255&(a^b[h])];return a^-1}var f=d();b.exports=e},{}],67:[function(a,b,c){"use strict";function d(a,b){return a.msg=I[b],b}function e(a){return(a<<1)-(a>4?9:0)}function f(a){for(var b=a.length;--b>=0;)a[b]=0}function g(a){var b=a.state,c=b.pending;c>a.avail_out&&(c=a.avail_out),0!==c&&(E.arraySet(a.output,b.pending_buf,b.pending_out,c,a.next_out),a.next_out+=c,b.pending_out+=c,a.total_out+=c,a.avail_out-=c,b.pending-=c,0===b.pending&&(b.pending_out=0))}function h(a,b){F._tr_flush_block(a,a.block_start>=0?a.block_start:-1,a.strstart-a.block_start,b),a.block_start=a.strstart,g(a.strm)}function i(a,b){a.pending_buf[a.pending++]=b}function j(a,b){a.pending_buf[a.pending++]=b>>>8&255,a.pending_buf[a.pending++]=255&b}function k(a,b,c,d){var e=a.avail_in;return e>d&&(e=d),0===e?0:(a.avail_in-=e,E.arraySet(b,a.input,a.next_in,e,c),1===a.state.wrap?a.adler=G(a.adler,b,e,c):2===a.state.wrap&&(a.adler=H(a.adler,b,e,c)),a.next_in+=e,a.total_in+=e,e)}function l(a,b){var c,d,e=a.max_chain_length,f=a.strstart,g=a.prev_length,h=a.nice_match,i=a.strstart>a.w_size-la?a.strstart-(a.w_size-la):0,j=a.window,k=a.w_mask,l=a.prev,m=a.strstart+ka,n=j[f+g-1],o=j[f+g];a.prev_length>=a.good_match&&(e>>=2),h>a.lookahead&&(h=a.lookahead);do if(c=b,j[c+g]===o&&j[c+g-1]===n&&j[c]===j[f]&&j[++c]===j[f+1]){f+=2,c++;do;while(j[++f]===j[++c]&&j[++f]===j[++c]&&j[++f]===j[++c]&&j[++f]===j[++c]&&j[++f]===j[++c]&&j[++f]===j[++c]&&j[++f]===j[++c]&&j[++f]===j[++c]&&f<m);if(d=ka-(m-f),f=m-ka,d>g){if(a.match_start=b,g=d,d>=h)break;n=j[f+g-1],o=j[f+g]}}while((b=l[b&k])>i&&0!==--e);return g<=a.lookahead?g:a.lookahead}function m(a){var b,c,d,e,f,g=a.w_size;do{if(e=a.window_size-a.lookahead-a.strstart,a.strstart>=g+(g-la)){E.arraySet(a.window,a.window,g,g,0),a.match_start-=g,a.strstart-=g,a.block_start-=g,c=a.hash_size,b=c;do d=a.head[--b],a.head[b]=d>=g?d-g:0;while(--c);c=g,b=c;do d=a.prev[--b],a.prev[b]=d>=g?d-g:0;while(--c);e+=g}if(0===a.strm.avail_in)break;if(c=k(a.strm,a.window,a.strstart+a.lookahead,e),a.lookahead+=c,a.lookahead+a.insert>=ja)for(f=a.strstart-a.insert,a.ins_h=a.window[f],a.ins_h=(a.ins_h<<a.hash_shift^a.window[f+1])&a.hash_mask;a.insert&&(a.ins_h=(a.ins_h<<a.hash_shift^a.window[f+ja-1])&a.hash_mask,a.prev[f&a.w_mask]=a.head[a.ins_h],a.head[a.ins_h]=f,f++,a.insert--,!(a.lookahead+a.insert<ja)););}while(a.lookahead<la&&0!==a.strm.avail_in)}function n(a,b){var c=65535;for(c>a.pending_buf_size-5&&(c=a.pending_buf_size-5);;){if(a.lookahead<=1){if(m(a),0===a.lookahead&&b===J)return ua;if(0===a.lookahead)break}a.strstart+=a.lookahead,a.lookahead=0;var d=a.block_start+c;if((0===a.strstart||a.strstart>=d)&&(a.lookahead=a.strstart-d,a.strstart=d,h(a,!1),0===a.strm.avail_out))return ua;if(a.strstart-a.block_start>=a.w_size-la&&(h(a,!1),0===a.strm.avail_out))return ua}return a.insert=0,b===M?(h(a,!0),0===a.strm.avail_out?wa:xa):a.strstart>a.block_start&&(h(a,!1),0===a.strm.avail_out)?ua:ua}function o(a,b){for(var c,d;;){if(a.lookahead<la){if(m(a),a.lookahead<la&&b===J)return ua;if(0===a.lookahead)break}if(c=0,a.lookahead>=ja&&(a.ins_h=(a.ins_h<<a.hash_shift^a.window[a.strstart+ja-1])&a.hash_mask,c=a.prev[a.strstart&a.w_mask]=a.head[a.ins_h],a.head[a.ins_h]=a.strstart),0!==c&&a.strstart-c<=a.w_size-la&&(a.match_length=l(a,c)),a.match_length>=ja)if(d=F._tr_tally(a,a.strstart-a.match_start,a.match_length-ja),a.lookahead-=a.match_length,a.match_length<=a.max_lazy_match&&a.lookahead>=ja){a.match_length--;do a.strstart++,a.ins_h=(a.ins_h<<a.hash_shift^a.window[a.strstart+ja-1])&a.hash_mask,c=a.prev[a.strstart&a.w_mask]=a.head[a.ins_h],a.head[a.ins_h]=a.strstart;while(0!==--a.match_length);a.strstart++}else a.strstart+=a.match_length,a.match_length=0,a.ins_h=a.window[a.strstart],a.ins_h=(a.ins_h<<a.hash_shift^a.window[a.strstart+1])&a.hash_mask;else d=F._tr_tally(a,0,a.window[a.strstart]),a.lookahead--,a.strstart++;if(d&&(h(a,!1),0===a.strm.avail_out))return ua}return a.insert=a.strstart<ja-1?a.strstart:ja-1,b===M?(h(a,!0),0===a.strm.avail_out?wa:xa):a.last_lit&&(h(a,!1),0===a.strm.avail_out)?ua:va}function p(a,b){for(var c,d,e;;){if(a.lookahead<la){if(m(a),a.lookahead<la&&b===J)return ua;if(0===a.lookahead)break}if(c=0,a.lookahead>=ja&&(a.ins_h=(a.ins_h<<a.hash_shift^a.window[a.strstart+ja-1])&a.hash_mask,c=a.prev[a.strstart&a.w_mask]=a.head[a.ins_h],a.head[a.ins_h]=a.strstart),a.prev_length=a.match_length,a.prev_match=a.match_start,a.match_length=ja-1,0!==c&&a.prev_length<a.max_lazy_match&&a.strstart-c<=a.w_size-la&&(a.match_length=l(a,c),a.match_length<=5&&(a.strategy===U||a.match_length===ja&&a.strstart-a.match_start>4096)&&(a.match_length=ja-1)),a.prev_length>=ja&&a.match_length<=a.prev_length){e=a.strstart+a.lookahead-ja,d=F._tr_tally(a,a.strstart-1-a.prev_match,a.prev_length-ja),a.lookahead-=a.prev_length-1,a.prev_length-=2;do++a.strstart<=e&&(a.ins_h=(a.ins_h<<a.hash_shift^a.window[a.strstart+ja-1])&a.hash_mask,c=a.prev[a.strstart&a.w_mask]=a.head[a.ins_h],a.head[a.ins_h]=a.strstart);while(0!==--a.prev_length);if(a.match_available=0,a.match_length=ja-1,a.strstart++,d&&(h(a,!1),0===a.strm.avail_out))return ua}else if(a.match_available){if(d=F._tr_tally(a,0,a.window[a.strstart-1]),d&&h(a,!1),a.strstart++,a.lookahead--,0===a.strm.avail_out)return ua}else a.match_available=1,a.strstart++,a.lookahead--}return a.match_available&&(d=F._tr_tally(a,0,a.window[a.strstart-1]),a.match_available=0),a.insert=a.strstart<ja-1?a.strstart:ja-1,b===M?(h(a,!0),0===a.strm.avail_out?wa:xa):a.last_lit&&(h(a,!1),0===a.strm.avail_out)?ua:va}function q(a,b){for(var c,d,e,f,g=a.window;;){if(a.lookahead<=ka){if(m(a),a.lookahead<=ka&&b===J)return ua;if(0===a.lookahead)break}if(a.match_length=0,a.lookahead>=ja&&a.strstart>0&&(e=a.strstart-1,d=g[e],d===g[++e]&&d===g[++e]&&d===g[++e])){f=a.strstart+ka;do;while(d===g[++e]&&d===g[++e]&&d===g[++e]&&d===g[++e]&&d===g[++e]&&d===g[++e]&&d===g[++e]&&d===g[++e]&&e<f);a.match_length=ka-(f-e),a.match_length>a.lookahead&&(a.match_length=a.lookahead)}if(a.match_length>=ja?(c=F._tr_tally(a,1,a.match_length-ja),a.lookahead-=a.match_length,a.strstart+=a.match_length,a.match_length=0):(c=F._tr_tally(a,0,a.window[a.strstart]),a.lookahead--,a.strstart++),c&&(h(a,!1),0===a.strm.avail_out))return ua}return a.insert=0,b===M?(h(a,!0),0===a.strm.avail_out?wa:xa):a.last_lit&&(h(a,!1),0===a.strm.avail_out)?ua:va}function r(a,b){for(var c;;){if(0===a.lookahead&&(m(a),0===a.lookahead)){if(b===J)return ua;break}if(a.match_length=0,c=F._tr_tally(a,0,a.window[a.strstart]),a.lookahead--,a.strstart++,c&&(h(a,!1),0===a.strm.avail_out))return ua}return a.insert=0,b===M?(h(a,!0),0===a.strm.avail_out?wa:xa):a.last_lit&&(h(a,!1),0===a.strm.avail_out)?ua:va}function s(a,b,c,d,e){this.good_length=a,this.max_lazy=b,this.nice_length=c,this.max_chain=d,this.func=e}function t(a){a.window_size=2*a.w_size,f(a.head),a.max_lazy_match=D[a.level].max_lazy,a.good_match=D[a.level].good_length,a.nice_match=D[a.level].nice_length,a.max_chain_length=D[a.level].max_chain,a.strstart=0,a.block_start=0,a.lookahead=0,a.insert=0,a.match_length=a.prev_length=ja-1,a.match_available=0,a.ins_h=0}function u(){this.strm=null,this.status=0,this.pending_buf=null,this.pending_buf_size=0,this.pending_out=0,this.pending=0,this.wrap=0,this.gzhead=null,this.gzindex=0,this.method=$,this.last_flush=-1,this.w_size=0,this.w_bits=0,this.w_mask=0,this.window=null,this.window_size=0,this.prev=null,this.head=null,this.ins_h=0,this.hash_size=0,this.hash_bits=0,this.hash_mask=0,this.hash_shift=0,this.block_start=0,this.match_length=0,this.prev_match=0,this.match_available=0,this.strstart=0,this.match_start=0,this.lookahead=0,this.prev_length=0,this.max_chain_length=0,this.max_lazy_match=0,this.level=0,this.strategy=0,this.good_match=0,this.nice_match=0,this.dyn_ltree=new E.Buf16(2*ha),this.dyn_dtree=new E.Buf16(2*(2*fa+1)),this.bl_tree=new E.Buf16(2*(2*ga+1)),f(this.dyn_ltree),f(this.dyn_dtree),f(this.bl_tree),this.l_desc=null,this.d_desc=null,this.bl_desc=null,this.bl_count=new E.Buf16(ia+1),this.heap=new E.Buf16(2*ea+1),f(this.heap),this.heap_len=0,this.heap_max=0,this.depth=new E.Buf16(2*ea+1),f(this.depth),this.l_buf=0,this.lit_bufsize=0,this.last_lit=0,this.d_buf=0,this.opt_len=0,this.static_len=0,this.matches=0,this.insert=0,this.bi_buf=0,this.bi_valid=0}function v(a){var b;return a&&a.state?(a.total_in=a.total_out=0,a.data_type=Z,b=a.state,b.pending=0,b.pending_out=0,b.wrap<0&&(b.wrap=-b.wrap),b.status=b.wrap?na:sa,a.adler=2===b.wrap?0:1,b.last_flush=J,F._tr_init(b),O):d(a,Q)}function w(a){var b=v(a);return b===O&&t(a.state),b}function x(a,b){return a&&a.state?2!==a.state.wrap?Q:(a.state.gzhead=b,O):Q}function y(a,b,c,e,f,g){if(!a)return Q;var h=1;if(b===T&&(b=6),e<0?(h=0,e=-e):e>15&&(h=2,e-=16),f<1||f>_||c!==$||e<8||e>15||b<0||b>9||g<0||g>X)return d(a,Q);8===e&&(e=9);var i=new u;return a.state=i,i.strm=a,i.wrap=h,i.gzhead=null,i.w_bits=e,i.w_size=1<<i.w_bits,i.w_mask=i.w_size-1,i.hash_bits=f+7,i.hash_size=1<<i.hash_bits,i.hash_mask=i.hash_size-1,i.hash_shift=~~((i.hash_bits+ja-1)/ja),i.window=new E.Buf8(2*i.w_size),i.head=new E.Buf16(i.hash_size),i.prev=new E.Buf16(i.w_size),i.lit_bufsize=1<<f+6,i.pending_buf_size=4*i.lit_bufsize,i.pending_buf=new E.Buf8(i.pending_buf_size),i.d_buf=1*i.lit_bufsize,i.l_buf=3*i.lit_bufsize,i.level=b,i.strategy=g,i.method=c,w(a)}function z(a,b){return y(a,b,$,aa,ba,Y)}function A(a,b){var c,h,k,l;if(!a||!a.state||b>N||b<0)return a?d(a,Q):Q;if(h=a.state,!a.output||!a.input&&0!==a.avail_in||h.status===ta&&b!==M)return d(a,0===a.avail_out?S:Q);if(h.strm=a,c=h.last_flush,h.last_flush=b,h.status===na)if(2===h.wrap)a.adler=0,i(h,31),i(h,139),i(h,8),h.gzhead?(i(h,(h.gzhead.text?1:0)+(h.gzhead.hcrc?2:0)+(h.gzhead.extra?4:0)+(h.gzhead.name?8:0)+(h.gzhead.comment?16:0)),i(h,255&h.gzhead.time),i(h,h.gzhead.time>>8&255),i(h,h.gzhead.time>>16&255),i(h,h.gzhead.time>>24&255),i(h,9===h.level?2:h.strategy>=V||h.level<2?4:0),i(h,255&h.gzhead.os),h.gzhead.extra&&h.gzhead.extra.length&&(i(h,255&h.gzhead.extra.length),i(h,h.gzhead.extra.length>>8&255)),h.gzhead.hcrc&&(a.adler=H(a.adler,h.pending_buf,h.pending,0)),h.gzindex=0,h.status=oa):(i(h,0),i(h,0),i(h,0),i(h,0),i(h,0),i(h,9===h.level?2:h.strategy>=V||h.level<2?4:0),i(h,ya),h.status=sa);else{var m=$+(h.w_bits-8<<4)<<8,n=-1;n=h.strategy>=V||h.level<2?0:h.level<6?1:6===h.level?2:3,m|=n<<6,0!==h.strstart&&(m|=ma),m+=31-m%31,h.status=sa,j(h,m),0!==h.strstart&&(j(h,a.adler>>>16),j(h,65535&a.adler)),a.adler=1}if(h.status===oa)if(h.gzhead.extra){for(k=h.pending;h.gzindex<(65535&h.gzhead.extra.length)&&(h.pending!==h.pending_buf_size||(h.gzhead.hcrc&&h.pending>k&&(a.adler=H(a.adler,h.pending_buf,h.pending-k,k)),g(a),k=h.pending,h.pending!==h.pending_buf_size));)i(h,255&h.gzhead.extra[h.gzindex]),h.gzindex++;h.gzhead.hcrc&&h.pending>k&&(a.adler=H(a.adler,h.pending_buf,h.pending-k,k)),h.gzindex===h.gzhead.extra.length&&(h.gzindex=0,h.status=pa)}else h.status=pa;if(h.status===pa)if(h.gzhead.name){k=h.pending;do{if(h.pending===h.pending_buf_size&&(h.gzhead.hcrc&&h.pending>k&&(a.adler=H(a.adler,h.pending_buf,h.pending-k,k)),g(a),k=h.pending,h.pending===h.pending_buf_size)){l=1;break}l=h.gzindex<h.gzhead.name.length?255&h.gzhead.name.charCodeAt(h.gzindex++):0,i(h,l)}while(0!==l);h.gzhead.hcrc&&h.pending>k&&(a.adler=H(a.adler,h.pending_buf,h.pending-k,k)),0===l&&(h.gzindex=0,h.status=qa)}else h.status=qa;if(h.status===qa)if(h.gzhead.comment){k=h.pending;do{if(h.pending===h.pending_buf_size&&(h.gzhead.hcrc&&h.pending>k&&(a.adler=H(a.adler,h.pending_buf,h.pending-k,k)),g(a),k=h.pending,h.pending===h.pending_buf_size)){l=1;break}l=h.gzindex<h.gzhead.comment.length?255&h.gzhead.comment.charCodeAt(h.gzindex++):0,i(h,l)}while(0!==l);h.gzhead.hcrc&&h.pending>k&&(a.adler=H(a.adler,h.pending_buf,h.pending-k,k)),0===l&&(h.status=ra)}else h.status=ra;if(h.status===ra&&(h.gzhead.hcrc?(h.pending+2>h.pending_buf_size&&g(a),h.pending+2<=h.pending_buf_size&&(i(h,255&a.adler),i(h,a.adler>>8&255),a.adler=0,h.status=sa)):h.status=sa),0!==h.pending){if(g(a),0===a.avail_out)return h.last_flush=-1,O}else if(0===a.avail_in&&e(b)<=e(c)&&b!==M)return d(a,S);if(h.status===ta&&0!==a.avail_in)return d(a,S);if(0!==a.avail_in||0!==h.lookahead||b!==J&&h.status!==ta){var o=h.strategy===V?r(h,b):h.strategy===W?q(h,b):D[h.level].func(h,b);if(o!==wa&&o!==xa||(h.status=ta),o===ua||o===wa)return 0===a.avail_out&&(h.last_flush=-1),O;if(o===va&&(b===K?F._tr_align(h):b!==N&&(F._tr_stored_block(h,0,0,!1),b===L&&(f(h.head),0===h.lookahead&&(h.strstart=0,h.block_start=0,h.insert=0))),g(a),0===a.avail_out))return h.last_flush=-1,O}return b!==M?O:h.wrap<=0?P:(2===h.wrap?(i(h,255&a.adler),i(h,a.adler>>8&255),i(h,a.adler>>16&255),i(h,a.adler>>24&255),i(h,255&a.total_in),i(h,a.total_in>>8&255),i(h,a.total_in>>16&255),i(h,a.total_in>>24&255)):(j(h,a.adler>>>16),j(h,65535&a.adler)),g(a),h.wrap>0&&(h.wrap=-h.wrap),0!==h.pending?O:P)}function B(a){var b;return a&&a.state?(b=a.state.status,b!==na&&b!==oa&&b!==pa&&b!==qa&&b!==ra&&b!==sa&&b!==ta?d(a,Q):(a.state=null,b===sa?d(a,R):O)):Q}function C(a,b){var c,d,e,g,h,i,j,k,l=b.length;if(!a||!a.state)return Q;if(c=a.state,g=c.wrap,2===g||1===g&&c.status!==na||c.lookahead)return Q;for(1===g&&(a.adler=G(a.adler,b,l,0)),c.wrap=0,l>=c.w_size&&(0===g&&(f(c.head),c.strstart=0,c.block_start=0,c.insert=0),k=new E.Buf8(c.w_size),E.arraySet(k,b,l-c.w_size,c.w_size,0),b=k,l=c.w_size),h=a.avail_in,i=a.next_in,j=a.input,a.avail_in=l,a.next_in=0,a.input=b,m(c);c.lookahead>=ja;){d=c.strstart,e=c.lookahead-(ja-1);do c.ins_h=(c.ins_h<<c.hash_shift^c.window[d+ja-1])&c.hash_mask,c.prev[d&c.w_mask]=c.head[c.ins_h],c.head[c.ins_h]=d,d++;while(--e);c.strstart=d,c.lookahead=ja-1,m(c)}return c.strstart+=c.lookahead,c.block_start=c.strstart,c.insert=c.lookahead,c.lookahead=0,c.match_length=c.prev_length=ja-1,c.match_available=0,a.next_in=i,a.input=j,a.avail_in=h,c.wrap=g,O}var D,E=a("../utils/common"),F=a("./trees"),G=a("./adler32"),H=a("./crc32"),I=a("./messages"),J=0,K=1,L=3,M=4,N=5,O=0,P=1,Q=-2,R=-3,S=-5,T=-1,U=1,V=2,W=3,X=4,Y=0,Z=2,$=8,_=9,aa=15,ba=8,ca=29,da=256,ea=da+1+ca,fa=30,ga=19,ha=2*ea+1,ia=15,ja=3,ka=258,la=ka+ja+1,ma=32,na=42,oa=69,pa=73,qa=91,ra=103,sa=113,ta=666,ua=1,va=2,wa=3,xa=4,ya=3;D=[new s(0,0,0,0,n),new s(4,4,8,4,o),new s(4,5,16,8,o),new s(4,6,32,32,o),new s(4,4,16,16,p),new s(8,16,32,32,p),new s(8,16,128,128,p),new s(8,32,128,256,p),new s(32,128,258,1024,p),new s(32,258,258,4096,p)],c.deflateInit=z,c.deflateInit2=y,c.deflateReset=w,c.deflateResetKeep=v,c.deflateSetHeader=x,c.deflate=A,c.deflateEnd=B,c.deflateSetDictionary=C,c.deflateInfo="pako deflate (from Nodeca project)"},{"../utils/common":62,"./adler32":64,"./crc32":66,"./messages":72,"./trees":73}],68:[function(a,b,c){"use strict";function d(){this.text=0,this.time=0,this.xflags=0,this.os=0,this.extra=null,this.extra_len=0,this.name="",this.comment="",this.hcrc=0,this.done=!1}b.exports=d},{}],69:[function(a,b,c){"use strict";var d=30,e=12;b.exports=function(a,b){var c,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,A,B,C;c=a.state,f=a.next_in,B=a.input,g=f+(a.avail_in-5),h=a.next_out,C=a.output,i=h-(b-a.avail_out),j=h+(a.avail_out-257),k=c.dmax,l=c.wsize,m=c.whave,n=c.wnext,o=c.window,p=c.hold,q=c.bits,r=c.lencode,s=c.distcode,t=(1<<c.lenbits)-1,u=(1<<c.distbits)-1;a:do{q<15&&(p+=B[f++]<<q,q+=8,p+=B[f++]<<q,q+=8),v=r[p&t];b:for(;;){if(w=v>>>24,p>>>=w,q-=w,w=v>>>16&255,0===w)C[h++]=65535&v;else{if(!(16&w)){if(0===(64&w)){v=r[(65535&v)+(p&(1<<w)-1)];continue b}if(32&w){c.mode=e;break a}a.msg="invalid literal/length code",c.mode=d;break a}x=65535&v,w&=15,w&&(q<w&&(p+=B[f++]<<q,q+=8),x+=p&(1<<w)-1,p>>>=w,q-=w),q<15&&(p+=B[f++]<<q,q+=8,p+=B[f++]<<q,q+=8),v=s[p&u];c:for(;;){if(w=v>>>24,p>>>=w,q-=w,w=v>>>16&255,!(16&w)){if(0===(64&w)){v=s[(65535&v)+(p&(1<<w)-1)];continue c}a.msg="invalid distance code",c.mode=d;break a}if(y=65535&v,w&=15,q<w&&(p+=B[f++]<<q,q+=8,q<w&&(p+=B[f++]<<q,q+=8)),y+=p&(1<<w)-1,y>k){a.msg="invalid distance too far back",c.mode=d;break a}if(p>>>=w,q-=w,w=h-i,y>w){if(w=y-w,w>m&&c.sane){a.msg="invalid distance too far back",c.mode=d;break a}if(z=0,A=o,0===n){if(z+=l-w,w<x){x-=w;do C[h++]=o[z++];while(--w);z=h-y,A=C}}else if(n<w){if(z+=l+n-w,w-=n,w<x){x-=w;do C[h++]=o[z++];while(--w);if(z=0,n<x){w=n,x-=w;do C[h++]=o[z++];while(--w);z=h-y,A=C}}}else if(z+=n-w,w<x){x-=w;do C[h++]=o[z++];while(--w);z=h-y,A=C}for(;x>2;)C[h++]=A[z++],C[h++]=A[z++],C[h++]=A[z++],x-=3;x&&(C[h++]=A[z++],x>1&&(C[h++]=A[z++]))}else{z=h-y;do C[h++]=C[z++],C[h++]=C[z++],C[h++]=C[z++],x-=3;while(x>2);x&&(C[h++]=C[z++],x>1&&(C[h++]=C[z++]))}break}}break}}while(f<g&&h<j);x=q>>3,f-=x,q-=x<<3,p&=(1<<q)-1,a.next_in=f,a.next_out=h,a.avail_in=f<g?5+(g-f):5-(f-g),a.avail_out=h<j?257+(j-h):257-(h-j),c.hold=p,c.bits=q}},{}],70:[function(a,b,c){"use strict";function d(a){return(a>>>24&255)+(a>>>8&65280)+((65280&a)<<8)+((255&a)<<24)}function e(){this.mode=0,this.last=!1,this.wrap=0,this.havedict=!1,this.flags=0,this.dmax=0,this.check=0,this.total=0,this.head=null,this.wbits=0,this.wsize=0,this.whave=0,this.wnext=0,this.window=null,this.hold=0,this.bits=0,this.length=0,this.offset=0,this.extra=0,this.lencode=null,this.distcode=null,this.lenbits=0,this.distbits=0,this.ncode=0,this.nlen=0,this.ndist=0,this.have=0,this.next=null,this.lens=new s.Buf16(320),this.work=new s.Buf16(288),this.lendyn=null,this.distdyn=null,this.sane=0,this.back=0,this.was=0}function f(a){var b;return a&&a.state?(b=a.state,a.total_in=a.total_out=b.total=0,a.msg="",b.wrap&&(a.adler=1&b.wrap),b.mode=L,b.last=0,b.havedict=0,b.dmax=32768,b.head=null,b.hold=0,b.bits=0,b.lencode=b.lendyn=new s.Buf32(pa),b.distcode=b.distdyn=new s.Buf32(qa),b.sane=1,b.back=-1,D):G}function g(a){var b;return a&&a.state?(b=a.state,b.wsize=0,b.whave=0,b.wnext=0,f(a)):G}function h(a,b){var c,d;return a&&a.state?(d=a.state,b<0?(c=0,b=-b):(c=(b>>4)+1,b<48&&(b&=15)),b&&(b<8||b>15)?G:(null!==d.window&&d.wbits!==b&&(d.window=null),d.wrap=c,d.wbits=b,g(a))):G}function i(a,b){var c,d;return a?(d=new e,a.state=d,d.window=null,c=h(a,b),c!==D&&(a.state=null),c):G}function j(a){return i(a,sa)}function k(a){if(ta){var b;for(q=new s.Buf32(512),r=new s.Buf32(32),b=0;b<144;)a.lens[b++]=8;for(;b<256;)a.lens[b++]=9;for(;b<280;)a.lens[b++]=7;for(;b<288;)a.lens[b++]=8;for(w(y,a.lens,0,288,q,0,a.work,{bits:9}),b=0;b<32;)a.lens[b++]=5;w(z,a.lens,0,32,r,0,a.work,{bits:5}),ta=!1}a.lencode=q,a.lenbits=9,a.distcode=r,a.distbits=5}function l(a,b,c,d){var e,f=a.state;return null===f.window&&(f.wsize=1<<f.wbits,f.wnext=0,f.whave=0,f.window=new s.Buf8(f.wsize)),d>=f.wsize?(s.arraySet(f.window,b,c-f.wsize,f.wsize,0),f.wnext=0,f.whave=f.wsize):(e=f.wsize-f.wnext,e>d&&(e=d),s.arraySet(f.window,b,c-d,e,f.wnext),d-=e,d?(s.arraySet(f.window,b,c-d,d,0),f.wnext=d,f.whave=f.wsize):(f.wnext+=e,f.wnext===f.wsize&&(f.wnext=0),f.whave<f.wsize&&(f.whave+=e))),0}function m(a,b){var c,e,f,g,h,i,j,m,n,o,p,q,r,pa,qa,ra,sa,ta,ua,va,wa,xa,ya,za,Aa=0,Ba=new s.Buf8(4),Ca=[16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15];if(!a||!a.state||!a.output||!a.input&&0!==a.avail_in)return G;c=a.state,c.mode===W&&(c.mode=X),h=a.next_out,f=a.output,j=a.avail_out,g=a.next_in,e=a.input,i=a.avail_in,m=c.hold,n=c.bits,o=i,p=j,xa=D;a:for(;;)switch(c.mode){case L:if(0===c.wrap){c.mode=X;break}for(;n<16;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}if(2&c.wrap&&35615===m){c.check=0,Ba[0]=255&m,Ba[1]=m>>>8&255,c.check=u(c.check,Ba,2,0),m=0,n=0,c.mode=M;break}if(c.flags=0,c.head&&(c.head.done=!1),!(1&c.wrap)||(((255&m)<<8)+(m>>8))%31){a.msg="incorrect header check",c.mode=ma;break}if((15&m)!==K){a.msg="unknown compression method",c.mode=ma;break}if(m>>>=4,n-=4,wa=(15&m)+8,0===c.wbits)c.wbits=wa;else if(wa>c.wbits){a.msg="invalid window size",c.mode=ma;break}c.dmax=1<<wa,a.adler=c.check=1,c.mode=512&m?U:W,m=0,n=0;break;case M:for(;n<16;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}if(c.flags=m,(255&c.flags)!==K){a.msg="unknown compression method",c.mode=ma;break}if(57344&c.flags){a.msg="unknown header flags set",c.mode=ma;break}c.head&&(c.head.text=m>>8&1),512&c.flags&&(Ba[0]=255&m,Ba[1]=m>>>8&255,c.check=u(c.check,Ba,2,0)),m=0,n=0,c.mode=N;case N:for(;n<32;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}c.head&&(c.head.time=m),512&c.flags&&(Ba[0]=255&m,Ba[1]=m>>>8&255,Ba[2]=m>>>16&255,Ba[3]=m>>>24&255,c.check=u(c.check,Ba,4,0)),m=0,n=0,c.mode=O;case O:for(;n<16;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}c.head&&(c.head.xflags=255&m,c.head.os=m>>8),512&c.flags&&(Ba[0]=255&m,Ba[1]=m>>>8&255,c.check=u(c.check,Ba,2,0)),m=0,n=0,c.mode=P;case P:if(1024&c.flags){for(;n<16;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}c.length=m,c.head&&(c.head.extra_len=m),512&c.flags&&(Ba[0]=255&m,Ba[1]=m>>>8&255,c.check=u(c.check,Ba,2,0)),m=0,n=0}else c.head&&(c.head.extra=null);c.mode=Q;case Q:if(1024&c.flags&&(q=c.length,q>i&&(q=i),q&&(c.head&&(wa=c.head.extra_len-c.length,c.head.extra||(c.head.extra=new Array(c.head.extra_len)),s.arraySet(c.head.extra,e,g,q,wa)),512&c.flags&&(c.check=u(c.check,e,q,g)),i-=q,g+=q,c.length-=q),c.length))break a;c.length=0,c.mode=R;case R:if(2048&c.flags){if(0===i)break a;q=0;do wa=e[g+q++],c.head&&wa&&c.length<65536&&(c.head.name+=String.fromCharCode(wa));while(wa&&q<i);if(512&c.flags&&(c.check=u(c.check,e,q,g)),i-=q,g+=q,wa)break a}else c.head&&(c.head.name=null);c.length=0,c.mode=S;case S:if(4096&c.flags){if(0===i)break a;q=0;do wa=e[g+q++],c.head&&wa&&c.length<65536&&(c.head.comment+=String.fromCharCode(wa));while(wa&&q<i);if(512&c.flags&&(c.check=u(c.check,e,q,g)),i-=q,g+=q,wa)break a}else c.head&&(c.head.comment=null);c.mode=T;case T:if(512&c.flags){for(;n<16;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}if(m!==(65535&c.check)){a.msg="header crc mismatch",c.mode=ma;break}m=0,n=0}c.head&&(c.head.hcrc=c.flags>>9&1,c.head.done=!0),a.adler=c.check=0,c.mode=W;break;case U:for(;n<32;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}a.adler=c.check=d(m),m=0,n=0,c.mode=V;case V:if(0===c.havedict)return a.next_out=h,a.avail_out=j,a.next_in=g,a.avail_in=i,c.hold=m,c.bits=n,F;a.adler=c.check=1,c.mode=W;case W:if(b===B||b===C)break a;case X:if(c.last){m>>>=7&n,n-=7&n,c.mode=ja;break}for(;n<3;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}switch(c.last=1&m,m>>>=1,n-=1,3&m){case 0:c.mode=Y;break;case 1:if(k(c),c.mode=ca,b===C){m>>>=2,n-=2;break a}break;case 2:c.mode=_;break;case 3:a.msg="invalid block type",c.mode=ma}m>>>=2,n-=2;break;case Y:for(m>>>=7&n,n-=7&n;n<32;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}if((65535&m)!==(m>>>16^65535)){a.msg="invalid stored block lengths",c.mode=ma;break}if(c.length=65535&m,m=0,n=0,c.mode=Z,b===C)break a;case Z:c.mode=$;case $:if(q=c.length){if(q>i&&(q=i),q>j&&(q=j),0===q)break a;s.arraySet(f,e,g,q,h),i-=q,g+=q,j-=q,h+=q,c.length-=q;break}c.mode=W;break;case _:for(;n<14;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}if(c.nlen=(31&m)+257,m>>>=5,n-=5,c.ndist=(31&m)+1,m>>>=5,n-=5,c.ncode=(15&m)+4,m>>>=4,n-=4,c.nlen>286||c.ndist>30){a.msg="too many length or distance symbols",c.mode=ma;break}c.have=0,c.mode=aa;case aa:for(;c.have<c.ncode;){for(;n<3;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}c.lens[Ca[c.have++]]=7&m,m>>>=3,n-=3}for(;c.have<19;)c.lens[Ca[c.have++]]=0;if(c.lencode=c.lendyn,c.lenbits=7,ya={bits:c.lenbits},xa=w(x,c.lens,0,19,c.lencode,0,c.work,ya),c.lenbits=ya.bits,xa){a.msg="invalid code lengths set",c.mode=ma;break}c.have=0,c.mode=ba;case ba:for(;c.have<c.nlen+c.ndist;){for(;Aa=c.lencode[m&(1<<c.lenbits)-1],qa=Aa>>>24,ra=Aa>>>16&255,sa=65535&Aa,!(qa<=n);){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}if(sa<16)m>>>=qa,n-=qa,c.lens[c.have++]=sa;else{if(16===sa){for(za=qa+2;n<za;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}if(m>>>=qa,n-=qa,0===c.have){a.msg="invalid bit length repeat",c.mode=ma;break}wa=c.lens[c.have-1],q=3+(3&m),m>>>=2,n-=2}else if(17===sa){for(za=qa+3;n<za;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}m>>>=qa,n-=qa,wa=0,q=3+(7&m),m>>>=3,n-=3}else{for(za=qa+7;n<za;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}m>>>=qa,n-=qa,wa=0,q=11+(127&m),m>>>=7,n-=7}if(c.have+q>c.nlen+c.ndist){a.msg="invalid bit length repeat",c.mode=ma;break}for(;q--;)c.lens[c.have++]=wa}}if(c.mode===ma)break;if(0===c.lens[256]){a.msg="invalid code -- missing end-of-block",c.mode=ma;break}if(c.lenbits=9,ya={bits:c.lenbits},xa=w(y,c.lens,0,c.nlen,c.lencode,0,c.work,ya),c.lenbits=ya.bits,xa){a.msg="invalid literal/lengths set",c.mode=ma;break}if(c.distbits=6,c.distcode=c.distdyn,ya={bits:c.distbits},xa=w(z,c.lens,c.nlen,c.ndist,c.distcode,0,c.work,ya),c.distbits=ya.bits,xa){a.msg="invalid distances set",c.mode=ma;break}if(c.mode=ca,b===C)break a;case ca:c.mode=da;case da:if(i>=6&&j>=258){a.next_out=h,a.avail_out=j,a.next_in=g,a.avail_in=i,c.hold=m,c.bits=n,v(a,p),h=a.next_out,f=a.output,j=a.avail_out,g=a.next_in,e=a.input,i=a.avail_in,m=c.hold,n=c.bits,c.mode===W&&(c.back=-1);break}for(c.back=0;Aa=c.lencode[m&(1<<c.lenbits)-1],qa=Aa>>>24,ra=Aa>>>16&255,sa=65535&Aa,!(qa<=n);){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}if(ra&&0===(240&ra)){for(ta=qa,ua=ra,va=sa;Aa=c.lencode[va+((m&(1<<ta+ua)-1)>>ta)],qa=Aa>>>24,ra=Aa>>>16&255,sa=65535&Aa,!(ta+qa<=n);){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}m>>>=ta,n-=ta,c.back+=ta}if(m>>>=qa,n-=qa,c.back+=qa,c.length=sa,0===ra){c.mode=ia;break}if(32&ra){c.back=-1,c.mode=W;break}if(64&ra){a.msg="invalid literal/length code",c.mode=ma;break}c.extra=15&ra,c.mode=ea;case ea:if(c.extra){for(za=c.extra;n<za;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}c.length+=m&(1<<c.extra)-1,m>>>=c.extra,n-=c.extra,c.back+=c.extra}c.was=c.length,c.mode=fa;case fa:for(;Aa=c.distcode[m&(1<<c.distbits)-1],qa=Aa>>>24,ra=Aa>>>16&255,sa=65535&Aa,!(qa<=n);){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}if(0===(240&ra)){for(ta=qa,ua=ra,va=sa;Aa=c.distcode[va+((m&(1<<ta+ua)-1)>>ta)],qa=Aa>>>24,ra=Aa>>>16&255,sa=65535&Aa,!(ta+qa<=n);){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}m>>>=ta,n-=ta,c.back+=ta}if(m>>>=qa,n-=qa,c.back+=qa,64&ra){a.msg="invalid distance code",c.mode=ma;break}c.offset=sa,c.extra=15&ra,c.mode=ga;case ga:if(c.extra){for(za=c.extra;n<za;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}c.offset+=m&(1<<c.extra)-1,m>>>=c.extra,n-=c.extra,c.back+=c.extra}if(c.offset>c.dmax){a.msg="invalid distance too far back",c.mode=ma;break}c.mode=ha;case ha:if(0===j)break a;if(q=p-j,c.offset>q){if(q=c.offset-q,q>c.whave&&c.sane){a.msg="invalid distance too far back",c.mode=ma;break}q>c.wnext?(q-=c.wnext,r=c.wsize-q):r=c.wnext-q,q>c.length&&(q=c.length),pa=c.window}else pa=f,r=h-c.offset,q=c.length;q>j&&(q=j),j-=q,c.length-=q;do f[h++]=pa[r++];while(--q);0===c.length&&(c.mode=da);break;case ia:if(0===j)break a;f[h++]=c.length,j--,c.mode=da;break;case ja:if(c.wrap){for(;n<32;){if(0===i)break a;i--,m|=e[g++]<<n,n+=8}if(p-=j,a.total_out+=p,c.total+=p,p&&(a.adler=c.check=c.flags?u(c.check,f,p,h-p):t(c.check,f,p,h-p)),p=j,(c.flags?m:d(m))!==c.check){a.msg="incorrect data check",c.mode=ma;break}m=0,n=0}c.mode=ka;case ka:if(c.wrap&&c.flags){for(;n<32;){if(0===i)break a;i--,m+=e[g++]<<n,n+=8}if(m!==(4294967295&c.total)){a.msg="incorrect length check",c.mode=ma;break}m=0,n=0}c.mode=la;case la:xa=E;break a;case ma:xa=H;break a;case na:return I;case oa:default:return G}return a.next_out=h,a.avail_out=j,a.next_in=g,a.avail_in=i,c.hold=m,c.bits=n,(c.wsize||p!==a.avail_out&&c.mode<ma&&(c.mode<ja||b!==A))&&l(a,a.output,a.next_out,p-a.avail_out)?(c.mode=na,I):(o-=a.avail_in,p-=a.avail_out,a.total_in+=o,a.total_out+=p,c.total+=p,c.wrap&&p&&(a.adler=c.check=c.flags?u(c.check,f,p,a.next_out-p):t(c.check,f,p,a.next_out-p)),a.data_type=c.bits+(c.last?64:0)+(c.mode===W?128:0)+(c.mode===ca||c.mode===Z?256:0),(0===o&&0===p||b===A)&&xa===D&&(xa=J),xa)}function n(a){if(!a||!a.state)return G;var b=a.state;return b.window&&(b.window=null),a.state=null,D}function o(a,b){var c;return a&&a.state?(c=a.state,0===(2&c.wrap)?G:(c.head=b,b.done=!1,D)):G}function p(a,b){var c,d,e,f=b.length;return a&&a.state?(c=a.state,0!==c.wrap&&c.mode!==V?G:c.mode===V&&(d=1,d=t(d,b,f,0),d!==c.check)?H:(e=l(a,b,f,f))?(c.mode=na,I):(c.havedict=1,D)):G}var q,r,s=a("../utils/common"),t=a("./adler32"),u=a("./crc32"),v=a("./inffast"),w=a("./inftrees"),x=0,y=1,z=2,A=4,B=5,C=6,D=0,E=1,F=2,G=-2,H=-3,I=-4,J=-5,K=8,L=1,M=2,N=3,O=4,P=5,Q=6,R=7,S=8,T=9,U=10,V=11,W=12,X=13,Y=14,Z=15,$=16,_=17,aa=18,ba=19,ca=20,da=21,ea=22,fa=23,ga=24,ha=25,ia=26,ja=27,ka=28,la=29,ma=30,na=31,oa=32,pa=852,qa=592,ra=15,sa=ra,ta=!0;c.inflateReset=g,c.inflateReset2=h,c.inflateResetKeep=f,c.inflateInit=j,c.inflateInit2=i,c.inflate=m,c.inflateEnd=n,c.inflateGetHeader=o,c.inflateSetDictionary=p,c.inflateInfo="pako inflate (from Nodeca project)"},{"../utils/common":62,"./adler32":64,"./crc32":66,"./inffast":69,"./inftrees":71}],71:[function(a,b,c){"use strict";var d=a("../utils/common"),e=15,f=852,g=592,h=0,i=1,j=2,k=[3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258,0,0],l=[16,16,16,16,16,16,16,16,17,17,17,17,18,18,18,18,19,19,19,19,20,20,20,20,21,21,21,21,16,72,78],m=[1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577,0,0],n=[16,16,16,16,17,17,18,18,19,19,20,20,21,21,22,22,23,23,24,24,25,25,26,26,27,27,28,28,29,29,64,64];b.exports=function(a,b,c,o,p,q,r,s){var t,u,v,w,x,y,z,A,B,C=s.bits,D=0,E=0,F=0,G=0,H=0,I=0,J=0,K=0,L=0,M=0,N=null,O=0,P=new d.Buf16(e+1),Q=new d.Buf16(e+1),R=null,S=0;for(D=0;D<=e;D++)P[D]=0;for(E=0;E<o;E++)P[b[c+E]]++;for(H=C,G=e;G>=1&&0===P[G];G--);if(H>G&&(H=G),0===G)return p[q++]=20971520,p[q++]=20971520,s.bits=1,0;for(F=1;F<G&&0===P[F];F++);for(H<F&&(H=F),K=1,D=1;D<=e;D++)if(K<<=1,K-=P[D],K<0)return-1;if(K>0&&(a===h||1!==G))return-1;for(Q[1]=0,D=1;D<e;D++)Q[D+1]=Q[D]+P[D];for(E=0;E<o;E++)0!==b[c+E]&&(r[Q[b[c+E]]++]=E);if(a===h?(N=R=r,y=19):a===i?(N=k,O-=257,R=l,S-=257,y=256):(N=m,R=n,y=-1),M=0,E=0,D=F,x=q,I=H,J=0,v=-1,L=1<<H,w=L-1,a===i&&L>f||a===j&&L>g)return 1;for(;;){z=D-J,r[E]<y?(A=0,B=r[E]):r[E]>y?(A=R[S+r[E]],B=N[O+r[E]]):(A=96,B=0),t=1<<D-J,u=1<<I,F=u;do u-=t,p[x+(M>>J)+u]=z<<24|A<<16|B|0;while(0!==u);for(t=1<<D-1;M&t;)t>>=1;if(0!==t?(M&=t-1,M+=t):M=0,E++,0===--P[D]){if(D===G)break;D=b[c+r[E]]}if(D>H&&(M&w)!==v){for(0===J&&(J=H),x+=F,I=D-J,K=1<<I;I+J<G&&(K-=P[I+J],!(K<=0));)I++,K<<=1;if(L+=1<<I,a===i&&L>f||a===j&&L>g)return 1;v=M&w,p[v]=H<<24|I<<16|x-q|0}}return 0!==M&&(p[x+M]=D-J<<24|64<<16|0),s.bits=H,0}},{"../utils/common":62}],72:[function(a,b,c){"use strict";b.exports={2:"need dictionary",1:"stream end",0:"","-1":"file error","-2":"stream error","-3":"data error","-4":"insufficient memory","-5":"buffer error","-6":"incompatible version"}},{}],73:[function(a,b,c){"use strict";function d(a){for(var b=a.length;--b>=0;)a[b]=0}function e(a,b,c,d,e){this.static_tree=a,this.extra_bits=b,this.extra_base=c,this.elems=d,this.max_length=e,this.has_stree=a&&a.length}function f(a,b){this.dyn_tree=a,this.max_code=0,this.stat_desc=b}function g(a){return a<256?ia[a]:ia[256+(a>>>7)]}function h(a,b){a.pending_buf[a.pending++]=255&b,a.pending_buf[a.pending++]=b>>>8&255}function i(a,b,c){a.bi_valid>X-c?(a.bi_buf|=b<<a.bi_valid&65535,h(a,a.bi_buf),a.bi_buf=b>>X-a.bi_valid,a.bi_valid+=c-X):(a.bi_buf|=b<<a.bi_valid&65535,a.bi_valid+=c)}function j(a,b,c){i(a,c[2*b],c[2*b+1])}function k(a,b){var c=0;do c|=1&a,a>>>=1,c<<=1;while(--b>0);return c>>>1}function l(a){16===a.bi_valid?(h(a,a.bi_buf),a.bi_buf=0,a.bi_valid=0):a.bi_valid>=8&&(a.pending_buf[a.pending++]=255&a.bi_buf,a.bi_buf>>=8,a.bi_valid-=8)}function m(a,b){var c,d,e,f,g,h,i=b.dyn_tree,j=b.max_code,k=b.stat_desc.static_tree,l=b.stat_desc.has_stree,m=b.stat_desc.extra_bits,n=b.stat_desc.extra_base,o=b.stat_desc.max_length,p=0;for(f=0;f<=W;f++)a.bl_count[f]=0;for(i[2*a.heap[a.heap_max]+1]=0,
c=a.heap_max+1;c<V;c++)d=a.heap[c],f=i[2*i[2*d+1]+1]+1,f>o&&(f=o,p++),i[2*d+1]=f,d>j||(a.bl_count[f]++,g=0,d>=n&&(g=m[d-n]),h=i[2*d],a.opt_len+=h*(f+g),l&&(a.static_len+=h*(k[2*d+1]+g)));if(0!==p){do{for(f=o-1;0===a.bl_count[f];)f--;a.bl_count[f]--,a.bl_count[f+1]+=2,a.bl_count[o]--,p-=2}while(p>0);for(f=o;0!==f;f--)for(d=a.bl_count[f];0!==d;)e=a.heap[--c],e>j||(i[2*e+1]!==f&&(a.opt_len+=(f-i[2*e+1])*i[2*e],i[2*e+1]=f),d--)}}function n(a,b,c){var d,e,f=new Array(W+1),g=0;for(d=1;d<=W;d++)f[d]=g=g+c[d-1]<<1;for(e=0;e<=b;e++){var h=a[2*e+1];0!==h&&(a[2*e]=k(f[h]++,h))}}function o(){var a,b,c,d,f,g=new Array(W+1);for(c=0,d=0;d<Q-1;d++)for(ka[d]=c,a=0;a<1<<ba[d];a++)ja[c++]=d;for(ja[c-1]=d,f=0,d=0;d<16;d++)for(la[d]=f,a=0;a<1<<ca[d];a++)ia[f++]=d;for(f>>=7;d<T;d++)for(la[d]=f<<7,a=0;a<1<<ca[d]-7;a++)ia[256+f++]=d;for(b=0;b<=W;b++)g[b]=0;for(a=0;a<=143;)ga[2*a+1]=8,a++,g[8]++;for(;a<=255;)ga[2*a+1]=9,a++,g[9]++;for(;a<=279;)ga[2*a+1]=7,a++,g[7]++;for(;a<=287;)ga[2*a+1]=8,a++,g[8]++;for(n(ga,S+1,g),a=0;a<T;a++)ha[2*a+1]=5,ha[2*a]=k(a,5);ma=new e(ga,ba,R+1,S,W),na=new e(ha,ca,0,T,W),oa=new e(new Array(0),da,0,U,Y)}function p(a){var b;for(b=0;b<S;b++)a.dyn_ltree[2*b]=0;for(b=0;b<T;b++)a.dyn_dtree[2*b]=0;for(b=0;b<U;b++)a.bl_tree[2*b]=0;a.dyn_ltree[2*Z]=1,a.opt_len=a.static_len=0,a.last_lit=a.matches=0}function q(a){a.bi_valid>8?h(a,a.bi_buf):a.bi_valid>0&&(a.pending_buf[a.pending++]=a.bi_buf),a.bi_buf=0,a.bi_valid=0}function r(a,b,c,d){q(a),d&&(h(a,c),h(a,~c)),G.arraySet(a.pending_buf,a.window,b,c,a.pending),a.pending+=c}function s(a,b,c,d){var e=2*b,f=2*c;return a[e]<a[f]||a[e]===a[f]&&d[b]<=d[c]}function t(a,b,c){for(var d=a.heap[c],e=c<<1;e<=a.heap_len&&(e<a.heap_len&&s(b,a.heap[e+1],a.heap[e],a.depth)&&e++,!s(b,d,a.heap[e],a.depth));)a.heap[c]=a.heap[e],c=e,e<<=1;a.heap[c]=d}function u(a,b,c){var d,e,f,h,k=0;if(0!==a.last_lit)do d=a.pending_buf[a.d_buf+2*k]<<8|a.pending_buf[a.d_buf+2*k+1],e=a.pending_buf[a.l_buf+k],k++,0===d?j(a,e,b):(f=ja[e],j(a,f+R+1,b),h=ba[f],0!==h&&(e-=ka[f],i(a,e,h)),d--,f=g(d),j(a,f,c),h=ca[f],0!==h&&(d-=la[f],i(a,d,h)));while(k<a.last_lit);j(a,Z,b)}function v(a,b){var c,d,e,f=b.dyn_tree,g=b.stat_desc.static_tree,h=b.stat_desc.has_stree,i=b.stat_desc.elems,j=-1;for(a.heap_len=0,a.heap_max=V,c=0;c<i;c++)0!==f[2*c]?(a.heap[++a.heap_len]=j=c,a.depth[c]=0):f[2*c+1]=0;for(;a.heap_len<2;)e=a.heap[++a.heap_len]=j<2?++j:0,f[2*e]=1,a.depth[e]=0,a.opt_len--,h&&(a.static_len-=g[2*e+1]);for(b.max_code=j,c=a.heap_len>>1;c>=1;c--)t(a,f,c);e=i;do c=a.heap[1],a.heap[1]=a.heap[a.heap_len--],t(a,f,1),d=a.heap[1],a.heap[--a.heap_max]=c,a.heap[--a.heap_max]=d,f[2*e]=f[2*c]+f[2*d],a.depth[e]=(a.depth[c]>=a.depth[d]?a.depth[c]:a.depth[d])+1,f[2*c+1]=f[2*d+1]=e,a.heap[1]=e++,t(a,f,1);while(a.heap_len>=2);a.heap[--a.heap_max]=a.heap[1],m(a,b),n(f,j,a.bl_count)}function w(a,b,c){var d,e,f=-1,g=b[1],h=0,i=7,j=4;for(0===g&&(i=138,j=3),b[2*(c+1)+1]=65535,d=0;d<=c;d++)e=g,g=b[2*(d+1)+1],++h<i&&e===g||(h<j?a.bl_tree[2*e]+=h:0!==e?(e!==f&&a.bl_tree[2*e]++,a.bl_tree[2*$]++):h<=10?a.bl_tree[2*_]++:a.bl_tree[2*aa]++,h=0,f=e,0===g?(i=138,j=3):e===g?(i=6,j=3):(i=7,j=4))}function x(a,b,c){var d,e,f=-1,g=b[1],h=0,k=7,l=4;for(0===g&&(k=138,l=3),d=0;d<=c;d++)if(e=g,g=b[2*(d+1)+1],!(++h<k&&e===g)){if(h<l){do j(a,e,a.bl_tree);while(0!==--h)}else 0!==e?(e!==f&&(j(a,e,a.bl_tree),h--),j(a,$,a.bl_tree),i(a,h-3,2)):h<=10?(j(a,_,a.bl_tree),i(a,h-3,3)):(j(a,aa,a.bl_tree),i(a,h-11,7));h=0,f=e,0===g?(k=138,l=3):e===g?(k=6,l=3):(k=7,l=4)}}function y(a){var b;for(w(a,a.dyn_ltree,a.l_desc.max_code),w(a,a.dyn_dtree,a.d_desc.max_code),v(a,a.bl_desc),b=U-1;b>=3&&0===a.bl_tree[2*ea[b]+1];b--);return a.opt_len+=3*(b+1)+5+5+4,b}function z(a,b,c,d){var e;for(i(a,b-257,5),i(a,c-1,5),i(a,d-4,4),e=0;e<d;e++)i(a,a.bl_tree[2*ea[e]+1],3);x(a,a.dyn_ltree,b-1),x(a,a.dyn_dtree,c-1)}function A(a){var b,c=4093624447;for(b=0;b<=31;b++,c>>>=1)if(1&c&&0!==a.dyn_ltree[2*b])return I;if(0!==a.dyn_ltree[18]||0!==a.dyn_ltree[20]||0!==a.dyn_ltree[26])return J;for(b=32;b<R;b++)if(0!==a.dyn_ltree[2*b])return J;return I}function B(a){pa||(o(),pa=!0),a.l_desc=new f(a.dyn_ltree,ma),a.d_desc=new f(a.dyn_dtree,na),a.bl_desc=new f(a.bl_tree,oa),a.bi_buf=0,a.bi_valid=0,p(a)}function C(a,b,c,d){i(a,(L<<1)+(d?1:0),3),r(a,b,c,!0)}function D(a){i(a,M<<1,3),j(a,Z,ga),l(a)}function E(a,b,c,d){var e,f,g=0;a.level>0?(a.strm.data_type===K&&(a.strm.data_type=A(a)),v(a,a.l_desc),v(a,a.d_desc),g=y(a),e=a.opt_len+3+7>>>3,f=a.static_len+3+7>>>3,f<=e&&(e=f)):e=f=c+5,c+4<=e&&b!==-1?C(a,b,c,d):a.strategy===H||f===e?(i(a,(M<<1)+(d?1:0),3),u(a,ga,ha)):(i(a,(N<<1)+(d?1:0),3),z(a,a.l_desc.max_code+1,a.d_desc.max_code+1,g+1),u(a,a.dyn_ltree,a.dyn_dtree)),p(a),d&&q(a)}function F(a,b,c){return a.pending_buf[a.d_buf+2*a.last_lit]=b>>>8&255,a.pending_buf[a.d_buf+2*a.last_lit+1]=255&b,a.pending_buf[a.l_buf+a.last_lit]=255&c,a.last_lit++,0===b?a.dyn_ltree[2*c]++:(a.matches++,b--,a.dyn_ltree[2*(ja[c]+R+1)]++,a.dyn_dtree[2*g(b)]++),a.last_lit===a.lit_bufsize-1}var G=a("../utils/common"),H=4,I=0,J=1,K=2,L=0,M=1,N=2,O=3,P=258,Q=29,R=256,S=R+1+Q,T=30,U=19,V=2*S+1,W=15,X=16,Y=7,Z=256,$=16,_=17,aa=18,ba=[0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0],ca=[0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13],da=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,7],ea=[16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15],fa=512,ga=new Array(2*(S+2));d(ga);var ha=new Array(2*T);d(ha);var ia=new Array(fa);d(ia);var ja=new Array(P-O+1);d(ja);var ka=new Array(Q);d(ka);var la=new Array(T);d(la);var ma,na,oa,pa=!1;c._tr_init=B,c._tr_stored_block=C,c._tr_flush_block=E,c._tr_tally=F,c._tr_align=D},{"../utils/common":62}],74:[function(a,b,c){"use strict";function d(){this.input=null,this.next_in=0,this.avail_in=0,this.total_in=0,this.output=null,this.next_out=0,this.avail_out=0,this.total_out=0,this.msg="",this.state=null,this.data_type=2,this.adler=0}b.exports=d},{}]},{},[10])(10)});
}).call(this,typeof global !== "undefined" ? global : typeof self !== "undefined" ? self : typeof window !== "undefined" ? window : {},require("buffer").Buffer)

},{"buffer":2}],6:[function(require,module,exports){
(function (global){
/*!
    localForage -- Offline Storage, Improved
    Version 1.5.5
    https://localforage.github.io/localForage
    (c) 2013-2017 Mozilla, Apache License 2.0
*/
(function(f){if(typeof exports==="object"&&typeof module!=="undefined"){module.exports=f()}else if(typeof define==="function"&&define.amd){define([],f)}else{var g;if(typeof window!=="undefined"){g=window}else if(typeof global!=="undefined"){g=global}else if(typeof self!=="undefined"){g=self}else{g=this}g.localforage = f()}})(function(){var define,module,exports;return (function e(t,n,r){function s(o,u){if(!n[o]){if(!t[o]){var a=typeof require=="function"&&require;if(!u&&a)return a(o,!0);if(i)return i(o,!0);var f=new Error("Cannot find module '"+o+"'");throw (f.code="MODULE_NOT_FOUND", f)}var l=n[o]={exports:{}};t[o][0].call(l.exports,function(e){var n=t[o][1][e];return s(n?n:e)},l,l.exports,e,t,n,r)}return n[o].exports}var i=typeof require=="function"&&require;for(var o=0;o<r.length;o++)s(r[o]);return s})({1:[function(_dereq_,module,exports){
(function (global){
'use strict';
var Mutation = global.MutationObserver || global.WebKitMutationObserver;

var scheduleDrain;

{
  if (Mutation) {
    var called = 0;
    var observer = new Mutation(nextTick);
    var element = global.document.createTextNode('');
    observer.observe(element, {
      characterData: true
    });
    scheduleDrain = function () {
      element.data = (called = ++called % 2);
    };
  } else if (!global.setImmediate && typeof global.MessageChannel !== 'undefined') {
    var channel = new global.MessageChannel();
    channel.port1.onmessage = nextTick;
    scheduleDrain = function () {
      channel.port2.postMessage(0);
    };
  } else if ('document' in global && 'onreadystatechange' in global.document.createElement('script')) {
    scheduleDrain = function () {

      // Create a <script> element; its readystatechange event will be fired asynchronously once it is inserted
      // into the document. Do so, thus queuing up the task. Remember to clean up once it's been called.
      var scriptEl = global.document.createElement('script');
      scriptEl.onreadystatechange = function () {
        nextTick();

        scriptEl.onreadystatechange = null;
        scriptEl.parentNode.removeChild(scriptEl);
        scriptEl = null;
      };
      global.document.documentElement.appendChild(scriptEl);
    };
  } else {
    scheduleDrain = function () {
      setTimeout(nextTick, 0);
    };
  }
}

var draining;
var queue = [];
//named nextTick for less confusing stack traces
function nextTick() {
  draining = true;
  var i, oldQueue;
  var len = queue.length;
  while (len) {
    oldQueue = queue;
    queue = [];
    i = -1;
    while (++i < len) {
      oldQueue[i]();
    }
    len = queue.length;
  }
  draining = false;
}

module.exports = immediate;
function immediate(task) {
  if (queue.push(task) === 1 && !draining) {
    scheduleDrain();
  }
}

}).call(this,typeof global !== "undefined" ? global : typeof self !== "undefined" ? self : typeof window !== "undefined" ? window : {})
},{}],2:[function(_dereq_,module,exports){
'use strict';
var immediate = _dereq_(1);

/* istanbul ignore next */
function INTERNAL() {}

var handlers = {};

var REJECTED = ['REJECTED'];
var FULFILLED = ['FULFILLED'];
var PENDING = ['PENDING'];

module.exports = exports = Promise;

function Promise(resolver) {
  if (typeof resolver !== 'function') {
    throw new TypeError('resolver must be a function');
  }
  this.state = PENDING;
  this.queue = [];
  this.outcome = void 0;
  if (resolver !== INTERNAL) {
    safelyResolveThenable(this, resolver);
  }
}

Promise.prototype["catch"] = function (onRejected) {
  return this.then(null, onRejected);
};
Promise.prototype.then = function (onFulfilled, onRejected) {
  if (typeof onFulfilled !== 'function' && this.state === FULFILLED ||
    typeof onRejected !== 'function' && this.state === REJECTED) {
    return this;
  }
  var promise = new this.constructor(INTERNAL);
  if (this.state !== PENDING) {
    var resolver = this.state === FULFILLED ? onFulfilled : onRejected;
    unwrap(promise, resolver, this.outcome);
  } else {
    this.queue.push(new QueueItem(promise, onFulfilled, onRejected));
  }

  return promise;
};
function QueueItem(promise, onFulfilled, onRejected) {
  this.promise = promise;
  if (typeof onFulfilled === 'function') {
    this.onFulfilled = onFulfilled;
    this.callFulfilled = this.otherCallFulfilled;
  }
  if (typeof onRejected === 'function') {
    this.onRejected = onRejected;
    this.callRejected = this.otherCallRejected;
  }
}
QueueItem.prototype.callFulfilled = function (value) {
  handlers.resolve(this.promise, value);
};
QueueItem.prototype.otherCallFulfilled = function (value) {
  unwrap(this.promise, this.onFulfilled, value);
};
QueueItem.prototype.callRejected = function (value) {
  handlers.reject(this.promise, value);
};
QueueItem.prototype.otherCallRejected = function (value) {
  unwrap(this.promise, this.onRejected, value);
};

function unwrap(promise, func, value) {
  immediate(function () {
    var returnValue;
    try {
      returnValue = func(value);
    } catch (e) {
      return handlers.reject(promise, e);
    }
    if (returnValue === promise) {
      handlers.reject(promise, new TypeError('Cannot resolve promise with itself'));
    } else {
      handlers.resolve(promise, returnValue);
    }
  });
}

handlers.resolve = function (self, value) {
  var result = tryCatch(getThen, value);
  if (result.status === 'error') {
    return handlers.reject(self, result.value);
  }
  var thenable = result.value;

  if (thenable) {
    safelyResolveThenable(self, thenable);
  } else {
    self.state = FULFILLED;
    self.outcome = value;
    var i = -1;
    var len = self.queue.length;
    while (++i < len) {
      self.queue[i].callFulfilled(value);
    }
  }
  return self;
};
handlers.reject = function (self, error) {
  self.state = REJECTED;
  self.outcome = error;
  var i = -1;
  var len = self.queue.length;
  while (++i < len) {
    self.queue[i].callRejected(error);
  }
  return self;
};

function getThen(obj) {
  // Make sure we only access the accessor once as required by the spec
  var then = obj && obj.then;
  if (obj && typeof obj === 'object' && typeof then === 'function') {
    return function appyThen() {
      then.apply(obj, arguments);
    };
  }
}

function safelyResolveThenable(self, thenable) {
  // Either fulfill, reject or reject with error
  var called = false;
  function onError(value) {
    if (called) {
      return;
    }
    called = true;
    handlers.reject(self, value);
  }

  function onSuccess(value) {
    if (called) {
      return;
    }
    called = true;
    handlers.resolve(self, value);
  }

  function tryToUnwrap() {
    thenable(onSuccess, onError);
  }

  var result = tryCatch(tryToUnwrap);
  if (result.status === 'error') {
    onError(result.value);
  }
}

function tryCatch(func, value) {
  var out = {};
  try {
    out.value = func(value);
    out.status = 'success';
  } catch (e) {
    out.status = 'error';
    out.value = e;
  }
  return out;
}

exports.resolve = resolve;
function resolve(value) {
  if (value instanceof this) {
    return value;
  }
  return handlers.resolve(new this(INTERNAL), value);
}

exports.reject = reject;
function reject(reason) {
  var promise = new this(INTERNAL);
  return handlers.reject(promise, reason);
}

exports.all = all;
function all(iterable) {
  var self = this;
  if (Object.prototype.toString.call(iterable) !== '[object Array]') {
    return this.reject(new TypeError('must be an array'));
  }

  var len = iterable.length;
  var called = false;
  if (!len) {
    return this.resolve([]);
  }

  var values = new Array(len);
  var resolved = 0;
  var i = -1;
  var promise = new this(INTERNAL);

  while (++i < len) {
    allResolver(iterable[i], i);
  }
  return promise;
  function allResolver(value, i) {
    self.resolve(value).then(resolveFromAll, function (error) {
      if (!called) {
        called = true;
        handlers.reject(promise, error);
      }
    });
    function resolveFromAll(outValue) {
      values[i] = outValue;
      if (++resolved === len && !called) {
        called = true;
        handlers.resolve(promise, values);
      }
    }
  }
}

exports.race = race;
function race(iterable) {
  var self = this;
  if (Object.prototype.toString.call(iterable) !== '[object Array]') {
    return this.reject(new TypeError('must be an array'));
  }

  var len = iterable.length;
  var called = false;
  if (!len) {
    return this.resolve([]);
  }

  var i = -1;
  var promise = new this(INTERNAL);

  while (++i < len) {
    resolver(iterable[i]);
  }
  return promise;
  function resolver(value) {
    self.resolve(value).then(function (response) {
      if (!called) {
        called = true;
        handlers.resolve(promise, response);
      }
    }, function (error) {
      if (!called) {
        called = true;
        handlers.reject(promise, error);
      }
    });
  }
}

},{"1":1}],3:[function(_dereq_,module,exports){
(function (global){
'use strict';
if (typeof global.Promise !== 'function') {
  global.Promise = _dereq_(2);
}

}).call(this,typeof global !== "undefined" ? global : typeof self !== "undefined" ? self : typeof window !== "undefined" ? window : {})
},{"2":2}],4:[function(_dereq_,module,exports){
'use strict';

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function getIDB() {
    /* global indexedDB,webkitIndexedDB,mozIndexedDB,OIndexedDB,msIndexedDB */
    try {
        if (typeof indexedDB !== 'undefined') {
            return indexedDB;
        }
        if (typeof webkitIndexedDB !== 'undefined') {
            return webkitIndexedDB;
        }
        if (typeof mozIndexedDB !== 'undefined') {
            return mozIndexedDB;
        }
        if (typeof OIndexedDB !== 'undefined') {
            return OIndexedDB;
        }
        if (typeof msIndexedDB !== 'undefined') {
            return msIndexedDB;
        }
    } catch (e) {
        return;
    }
}

var idb = getIDB();

function isIndexedDBValid() {
    try {
        // Initialize IndexedDB; fall back to vendor-prefixed versions
        // if needed.
        if (!idb) {
            return false;
        }
        // We mimic PouchDB here;
        //
        // We test for openDatabase because IE Mobile identifies itself
        // as Safari. Oh the lulz...
        var isSafari = typeof openDatabase !== 'undefined' && /(Safari|iPhone|iPad|iPod)/.test(navigator.userAgent) && !/Chrome/.test(navigator.userAgent) && !/BlackBerry/.test(navigator.platform);

        var hasFetch = typeof fetch === 'function' && fetch.toString().indexOf('[native code') !== -1;

        // Safari <10.1 does not meet our requirements for IDB support (#5572)
        // since Safari 10.1 shipped with fetch, we can use that to detect it
        return (!isSafari || hasFetch) && typeof indexedDB !== 'undefined' &&
        // some outdated implementations of IDB that appear on Samsung
        // and HTC Android devices <4.4 are missing IDBKeyRange
        // See: https://github.com/mozilla/localForage/issues/128
        // See: https://github.com/mozilla/localForage/issues/272
        typeof IDBKeyRange !== 'undefined';
    } catch (e) {
        return false;
    }
}

// Abstracts constructing a Blob object, so it also works in older
// browsers that don't support the native Blob constructor. (i.e.
// old QtWebKit versions, at least).
// Abstracts constructing a Blob object, so it also works in older
// browsers that don't support the native Blob constructor. (i.e.
// old QtWebKit versions, at least).
function createBlob(parts, properties) {
    /* global BlobBuilder,MSBlobBuilder,MozBlobBuilder,WebKitBlobBuilder */
    parts = parts || [];
    properties = properties || {};
    try {
        return new Blob(parts, properties);
    } catch (e) {
        if (e.name !== 'TypeError') {
            throw e;
        }
        var Builder = typeof BlobBuilder !== 'undefined' ? BlobBuilder : typeof MSBlobBuilder !== 'undefined' ? MSBlobBuilder : typeof MozBlobBuilder !== 'undefined' ? MozBlobBuilder : WebKitBlobBuilder;
        var builder = new Builder();
        for (var i = 0; i < parts.length; i += 1) {
            builder.append(parts[i]);
        }
        return builder.getBlob(properties.type);
    }
}

// This is CommonJS because lie is an external dependency, so Rollup
// can just ignore it.
if (typeof Promise === 'undefined') {
    // In the "nopromises" build this will just throw if you don't have
    // a global promise object, but it would throw anyway later.
    _dereq_(3);
}
var Promise$1 = Promise;

function executeCallback(promise, callback) {
    if (callback) {
        promise.then(function (result) {
            callback(null, result);
        }, function (error) {
            callback(error);
        });
    }
}

function executeTwoCallbacks(promise, callback, errorCallback) {
    if (typeof callback === 'function') {
        promise.then(callback);
    }

    if (typeof errorCallback === 'function') {
        promise["catch"](errorCallback);
    }
}

function normalizeKey(key) {
    // Cast the key to a string, as that's all we can set as a key.
    if (typeof key !== 'string') {
        console.warn(key + ' used as a key, but it is not a string.');
        key = String(key);
    }

    return key;
}

// Some code originally from async_storage.js in
// [Gaia](https://github.com/mozilla-b2g/gaia).

var DETECT_BLOB_SUPPORT_STORE = 'local-forage-detect-blob-support';
var supportsBlobs;
var dbContexts;
var toString = Object.prototype.toString;

// Transaction Modes
var READ_ONLY = 'readonly';
var READ_WRITE = 'readwrite';

// Transform a binary string to an array buffer, because otherwise
// weird stuff happens when you try to work with the binary string directly.
// It is known.
// From http://stackoverflow.com/questions/14967647/ (continues on next line)
// encode-decode-image-with-base64-breaks-image (2013-04-21)
function _binStringToArrayBuffer(bin) {
    var length = bin.length;
    var buf = new ArrayBuffer(length);
    var arr = new Uint8Array(buf);
    for (var i = 0; i < length; i++) {
        arr[i] = bin.charCodeAt(i);
    }
    return buf;
}

//
// Blobs are not supported in all versions of IndexedDB, notably
// Chrome <37 and Android <5. In those versions, storing a blob will throw.
//
// Various other blob bugs exist in Chrome v37-42 (inclusive).
// Detecting them is expensive and confusing to users, and Chrome 37-42
// is at very low usage worldwide, so we do a hacky userAgent check instead.
//
// content-type bug: https://code.google.com/p/chromium/issues/detail?id=408120
// 404 bug: https://code.google.com/p/chromium/issues/detail?id=447916
// FileReader bug: https://code.google.com/p/chromium/issues/detail?id=447836
//
// Code borrowed from PouchDB. See:
// https://github.com/pouchdb/pouchdb/blob/master/packages/node_modules/pouchdb-adapter-idb/src/blobSupport.js
//
function _checkBlobSupportWithoutCaching(idb) {
    return new Promise$1(function (resolve) {
        var txn = idb.transaction(DETECT_BLOB_SUPPORT_STORE, READ_WRITE);
        var blob = createBlob(['']);
        txn.objectStore(DETECT_BLOB_SUPPORT_STORE).put(blob, 'key');

        txn.onabort = function (e) {
            // If the transaction aborts now its due to not being able to
            // write to the database, likely due to the disk being full
            e.preventDefault();
            e.stopPropagation();
            resolve(false);
        };

        txn.oncomplete = function () {
            var matchedChrome = navigator.userAgent.match(/Chrome\/(\d+)/);
            var matchedEdge = navigator.userAgent.match(/Edge\//);
            // MS Edge pretends to be Chrome 42:
            // https://msdn.microsoft.com/en-us/library/hh869301%28v=vs.85%29.aspx
            resolve(matchedEdge || !matchedChrome || parseInt(matchedChrome[1], 10) >= 43);
        };
    })["catch"](function () {
        return false; // error, so assume unsupported
    });
}

function _checkBlobSupport(idb) {
    if (typeof supportsBlobs === 'boolean') {
        return Promise$1.resolve(supportsBlobs);
    }
    return _checkBlobSupportWithoutCaching(idb).then(function (value) {
        supportsBlobs = value;
        return supportsBlobs;
    });
}

function _deferReadiness(dbInfo) {
    var dbContext = dbContexts[dbInfo.name];

    // Create a deferred object representing the current database operation.
    var deferredOperation = {};

    deferredOperation.promise = new Promise$1(function (resolve) {
        deferredOperation.resolve = resolve;
    });

    // Enqueue the deferred operation.
    dbContext.deferredOperations.push(deferredOperation);

    // Chain its promise to the database readiness.
    if (!dbContext.dbReady) {
        dbContext.dbReady = deferredOperation.promise;
    } else {
        dbContext.dbReady = dbContext.dbReady.then(function () {
            return deferredOperation.promise;
        });
    }
}

function _advanceReadiness(dbInfo) {
    var dbContext = dbContexts[dbInfo.name];

    // Dequeue a deferred operation.
    var deferredOperation = dbContext.deferredOperations.pop();

    // Resolve its promise (which is part of the database readiness
    // chain of promises).
    if (deferredOperation) {
        deferredOperation.resolve();
    }
}

function _rejectReadiness(dbInfo, err) {
    var dbContext = dbContexts[dbInfo.name];

    // Dequeue a deferred operation.
    var deferredOperation = dbContext.deferredOperations.pop();

    // Reject its promise (which is part of the database readiness
    // chain of promises).
    if (deferredOperation) {
        deferredOperation.reject(err);
    }
}

function _getConnection(dbInfo, upgradeNeeded) {
    return new Promise$1(function (resolve, reject) {

        if (dbInfo.db) {
            if (upgradeNeeded) {
                _deferReadiness(dbInfo);
                dbInfo.db.close();
            } else {
                return resolve(dbInfo.db);
            }
        }

        var dbArgs = [dbInfo.name];

        if (upgradeNeeded) {
            dbArgs.push(dbInfo.version);
        }

        var openreq = idb.open.apply(idb, dbArgs);

        if (upgradeNeeded) {
            openreq.onupgradeneeded = function (e) {
                var db = openreq.result;
                try {
                    db.createObjectStore(dbInfo.storeName);
                    if (e.oldVersion <= 1) {
                        // Added when support for blob shims was added
                        db.createObjectStore(DETECT_BLOB_SUPPORT_STORE);
                    }
                } catch (ex) {
                    if (ex.name === 'ConstraintError') {
                        console.warn('The database "' + dbInfo.name + '"' + ' has been upgraded from version ' + e.oldVersion + ' to version ' + e.newVersion + ', but the storage "' + dbInfo.storeName + '" already exists.');
                    } else {
                        throw ex;
                    }
                }
            };
        }

        openreq.onerror = function (e) {
            e.preventDefault();
            reject(openreq.error);
        };

        openreq.onsuccess = function () {
            resolve(openreq.result);
            _advanceReadiness(dbInfo);
        };
    });
}

function _getOriginalConnection(dbInfo) {
    return _getConnection(dbInfo, false);
}

function _getUpgradedConnection(dbInfo) {
    return _getConnection(dbInfo, true);
}

function _isUpgradeNeeded(dbInfo, defaultVersion) {
    if (!dbInfo.db) {
        return true;
    }

    var isNewStore = !dbInfo.db.objectStoreNames.contains(dbInfo.storeName);
    var isDowngrade = dbInfo.version < dbInfo.db.version;
    var isUpgrade = dbInfo.version > dbInfo.db.version;

    if (isDowngrade) {
        // If the version is not the default one
        // then warn for impossible downgrade.
        if (dbInfo.version !== defaultVersion) {
            console.warn('The database "' + dbInfo.name + '"' + ' can\'t be downgraded from version ' + dbInfo.db.version + ' to version ' + dbInfo.version + '.');
        }
        // Align the versions to prevent errors.
        dbInfo.version = dbInfo.db.version;
    }

    if (isUpgrade || isNewStore) {
        // If the store is new then increment the version (if needed).
        // This will trigger an "upgradeneeded" event which is required
        // for creating a store.
        if (isNewStore) {
            var incVersion = dbInfo.db.version + 1;
            if (incVersion > dbInfo.version) {
                dbInfo.version = incVersion;
            }
        }

        return true;
    }

    return false;
}

// encode a blob for indexeddb engines that don't support blobs
function _encodeBlob(blob) {
    return new Promise$1(function (resolve, reject) {
        var reader = new FileReader();
        reader.onerror = reject;
        reader.onloadend = function (e) {
            var base64 = btoa(e.target.result || '');
            resolve({
                __local_forage_encoded_blob: true,
                data: base64,
                type: blob.type
            });
        };
        reader.readAsBinaryString(blob);
    });
}

// decode an encoded blob
function _decodeBlob(encodedBlob) {
    var arrayBuff = _binStringToArrayBuffer(atob(encodedBlob.data));
    return createBlob([arrayBuff], { type: encodedBlob.type });
}

// is this one of our fancy encoded blobs?
function _isEncodedBlob(value) {
    return value && value.__local_forage_encoded_blob;
}

// Specialize the default `ready()` function by making it dependent
// on the current database operations. Thus, the driver will be actually
// ready when it's been initialized (default) *and* there are no pending
// operations on the database (initiated by some other instances).
function _fullyReady(callback) {
    var self = this;

    var promise = self._initReady().then(function () {
        var dbContext = dbContexts[self._dbInfo.name];

        if (dbContext && dbContext.dbReady) {
            return dbContext.dbReady;
        }
    });

    executeTwoCallbacks(promise, callback, callback);
    return promise;
}

// Try to establish a new db connection to replace the
// current one which is broken (i.e. experiencing
// InvalidStateError while creating a transaction).
function _tryReconnect(dbInfo) {
    _deferReadiness(dbInfo);

    var dbContext = dbContexts[dbInfo.name];
    var forages = dbContext.forages;

    for (var i = 0; i < forages.length; i++) {
        if (forages[i]._dbInfo.db) {
            forages[i]._dbInfo.db.close();
            forages[i]._dbInfo.db = null;
        }
    }

    return _getConnection(dbInfo, false).then(function (db) {
        for (var j = 0; j < forages.length; j++) {
            forages[j]._dbInfo.db = db;
        }
    })["catch"](function (err) {
        _rejectReadiness(dbInfo, err);
        throw err;
    });
}

// FF doesn't like Promises (micro-tasks) and IDDB store operations,
// so we have to do it with callbacks
function createTransaction(dbInfo, mode, callback) {
    try {
        var tx = dbInfo.db.transaction(dbInfo.storeName, mode);
        callback(null, tx);
    } catch (err) {
        if (!dbInfo.db || err.name === 'InvalidStateError') {
            return _tryReconnect(dbInfo).then(function () {

                var tx = dbInfo.db.transaction(dbInfo.storeName, mode);
                callback(null, tx);
            });
        }

        callback(err);
    }
}

// Open the IndexedDB database (automatically creates one if one didn't
// previously exist), using any options set in the config.
function _initStorage(options) {
    var self = this;
    var dbInfo = {
        db: null
    };

    if (options) {
        for (var i in options) {
            dbInfo[i] = options[i];
        }
    }

    // Initialize a singleton container for all running localForages.
    if (!dbContexts) {
        dbContexts = {};
    }

    // Get the current context of the database;
    var dbContext = dbContexts[dbInfo.name];

    // ...or create a new context.
    if (!dbContext) {
        dbContext = {
            // Running localForages sharing a database.
            forages: [],
            // Shared database.
            db: null,
            // Database readiness (promise).
            dbReady: null,
            // Deferred operations on the database.
            deferredOperations: []
        };
        // Register the new context in the global container.
        dbContexts[dbInfo.name] = dbContext;
    }

    // Register itself as a running localForage in the current context.
    dbContext.forages.push(self);

    // Replace the default `ready()` function with the specialized one.
    if (!self._initReady) {
        self._initReady = self.ready;
        self.ready = _fullyReady;
    }

    // Create an array of initialization states of the related localForages.
    var initPromises = [];

    function ignoreErrors() {
        // Don't handle errors here,
        // just makes sure related localForages aren't pending.
        return Promise$1.resolve();
    }

    for (var j = 0; j < dbContext.forages.length; j++) {
        var forage = dbContext.forages[j];
        if (forage !== self) {
            // Don't wait for itself...
            initPromises.push(forage._initReady()["catch"](ignoreErrors));
        }
    }

    // Take a snapshot of the related localForages.
    var forages = dbContext.forages.slice(0);

    // Initialize the connection process only when
    // all the related localForages aren't pending.
    return Promise$1.all(initPromises).then(function () {
        dbInfo.db = dbContext.db;
        // Get the connection or open a new one without upgrade.
        return _getOriginalConnection(dbInfo);
    }).then(function (db) {
        dbInfo.db = db;
        if (_isUpgradeNeeded(dbInfo, self._defaultConfig.version)) {
            // Reopen the database for upgrading.
            return _getUpgradedConnection(dbInfo);
        }
        return db;
    }).then(function (db) {
        dbInfo.db = dbContext.db = db;
        self._dbInfo = dbInfo;
        // Share the final connection amongst related localForages.
        for (var k = 0; k < forages.length; k++) {
            var forage = forages[k];
            if (forage !== self) {
                // Self is already up-to-date.
                forage._dbInfo.db = dbInfo.db;
                forage._dbInfo.version = dbInfo.version;
            }
        }
    });
}

function getItem(key, callback) {
    var self = this;

    key = normalizeKey(key);

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            createTransaction(self._dbInfo, READ_ONLY, function (err, transaction) {
                if (err) {
                    return reject(err);
                }

                try {
                    var store = transaction.objectStore(self._dbInfo.storeName);
                    var req = store.get(key);

                    req.onsuccess = function () {
                        var value = req.result;
                        if (value === undefined) {
                            value = null;
                        }
                        if (_isEncodedBlob(value)) {
                            value = _decodeBlob(value);
                        }
                        resolve(value);
                    };

                    req.onerror = function () {
                        reject(req.error);
                    };
                } catch (e) {
                    reject(e);
                }
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

// Iterate over all items stored in database.
function iterate(iterator, callback) {
    var self = this;

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            createTransaction(self._dbInfo, READ_ONLY, function (err, transaction) {
                if (err) {
                    return reject(err);
                }

                try {
                    var store = transaction.objectStore(self._dbInfo.storeName);
                    var req = store.openCursor();
                    var iterationNumber = 1;

                    req.onsuccess = function () {
                        var cursor = req.result;

                        if (cursor) {
                            var value = cursor.value;
                            if (_isEncodedBlob(value)) {
                                value = _decodeBlob(value);
                            }
                            var result = iterator(value, cursor.key, iterationNumber++);

                            // when the iterator callback retuns any
                            // (non-`undefined`) value, then we stop
                            // the iteration immediately
                            if (result !== void 0) {
                                resolve(result);
                            } else {
                                cursor["continue"]();
                            }
                        } else {
                            resolve();
                        }
                    };

                    req.onerror = function () {
                        reject(req.error);
                    };
                } catch (e) {
                    reject(e);
                }
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);

    return promise;
}

function setItem(key, value, callback) {
    var self = this;

    key = normalizeKey(key);

    var promise = new Promise$1(function (resolve, reject) {
        var dbInfo;
        self.ready().then(function () {
            dbInfo = self._dbInfo;
            if (toString.call(value) === '[object Blob]') {
                return _checkBlobSupport(dbInfo.db).then(function (blobSupport) {
                    if (blobSupport) {
                        return value;
                    }
                    return _encodeBlob(value);
                });
            }
            return value;
        }).then(function (value) {
            createTransaction(self._dbInfo, READ_WRITE, function (err, transaction) {
                if (err) {
                    return reject(err);
                }

                try {
                    var store = transaction.objectStore(self._dbInfo.storeName);

                    // The reason we don't _save_ null is because IE 10 does
                    // not support saving the `null` type in IndexedDB. How
                    // ironic, given the bug below!
                    // See: https://github.com/mozilla/localForage/issues/161
                    if (value === null) {
                        value = undefined;
                    }

                    var req = store.put(value, key);

                    transaction.oncomplete = function () {
                        // Cast to undefined so the value passed to
                        // callback/promise is the same as what one would get out
                        // of `getItem()` later. This leads to some weirdness
                        // (setItem('foo', undefined) will return `null`), but
                        // it's not my fault localStorage is our baseline and that
                        // it's weird.
                        if (value === undefined) {
                            value = null;
                        }

                        resolve(value);
                    };
                    transaction.onabort = transaction.onerror = function () {
                        var err = req.error ? req.error : req.transaction.error;
                        reject(err);
                    };
                } catch (e) {
                    reject(e);
                }
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

function removeItem(key, callback) {
    var self = this;

    key = normalizeKey(key);

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            createTransaction(self._dbInfo, READ_WRITE, function (err, transaction) {
                if (err) {
                    return reject(err);
                }

                try {
                    var store = transaction.objectStore(self._dbInfo.storeName);
                    // We use a Grunt task to make this safe for IE and some
                    // versions of Android (including those used by Cordova).
                    // Normally IE won't like `.delete()` and will insist on
                    // using `['delete']()`, but we have a build step that
                    // fixes this for us now.
                    var req = store["delete"](key);
                    transaction.oncomplete = function () {
                        resolve();
                    };

                    transaction.onerror = function () {
                        reject(req.error);
                    };

                    // The request will be also be aborted if we've exceeded our storage
                    // space.
                    transaction.onabort = function () {
                        var err = req.error ? req.error : req.transaction.error;
                        reject(err);
                    };
                } catch (e) {
                    reject(e);
                }
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

function clear(callback) {
    var self = this;

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            createTransaction(self._dbInfo, READ_WRITE, function (err, transaction) {
                if (err) {
                    return reject(err);
                }

                try {
                    var store = transaction.objectStore(self._dbInfo.storeName);
                    var req = store.clear();

                    transaction.oncomplete = function () {
                        resolve();
                    };

                    transaction.onabort = transaction.onerror = function () {
                        var err = req.error ? req.error : req.transaction.error;
                        reject(err);
                    };
                } catch (e) {
                    reject(e);
                }
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

function length(callback) {
    var self = this;

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            createTransaction(self._dbInfo, READ_ONLY, function (err, transaction) {
                if (err) {
                    return reject(err);
                }

                try {
                    var store = transaction.objectStore(self._dbInfo.storeName);
                    var req = store.count();

                    req.onsuccess = function () {
                        resolve(req.result);
                    };

                    req.onerror = function () {
                        reject(req.error);
                    };
                } catch (e) {
                    reject(e);
                }
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

function key(n, callback) {
    var self = this;

    var promise = new Promise$1(function (resolve, reject) {
        if (n < 0) {
            resolve(null);

            return;
        }

        self.ready().then(function () {
            createTransaction(self._dbInfo, READ_ONLY, function (err, transaction) {
                if (err) {
                    return reject(err);
                }

                try {
                    var store = transaction.objectStore(self._dbInfo.storeName);
                    var advanced = false;
                    var req = store.openCursor();

                    req.onsuccess = function () {
                        var cursor = req.result;
                        if (!cursor) {
                            // this means there weren't enough keys
                            resolve(null);

                            return;
                        }

                        if (n === 0) {
                            // We have the first key, return it if that's what they
                            // wanted.
                            resolve(cursor.key);
                        } else {
                            if (!advanced) {
                                // Otherwise, ask the cursor to skip ahead n
                                // records.
                                advanced = true;
                                cursor.advance(n);
                            } else {
                                // When we get here, we've got the nth key.
                                resolve(cursor.key);
                            }
                        }
                    };

                    req.onerror = function () {
                        reject(req.error);
                    };
                } catch (e) {
                    reject(e);
                }
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

function keys(callback) {
    var self = this;

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            createTransaction(self._dbInfo, READ_ONLY, function (err, transaction) {
                if (err) {
                    return reject(err);
                }

                try {
                    var store = transaction.objectStore(self._dbInfo.storeName);
                    var req = store.openCursor();
                    var keys = [];

                    req.onsuccess = function () {
                        var cursor = req.result;

                        if (!cursor) {
                            resolve(keys);
                            return;
                        }

                        keys.push(cursor.key);
                        cursor["continue"]();
                    };

                    req.onerror = function () {
                        reject(req.error);
                    };
                } catch (e) {
                    reject(e);
                }
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

var asyncStorage = {
    _driver: 'asyncStorage',
    _initStorage: _initStorage,
    _support: isIndexedDBValid(),
    iterate: iterate,
    getItem: getItem,
    setItem: setItem,
    removeItem: removeItem,
    clear: clear,
    length: length,
    key: key,
    keys: keys
};

function isWebSQLValid() {
    return typeof openDatabase === 'function';
}

// Sadly, the best way to save binary data in WebSQL/localStorage is serializing
// it to Base64, so this is how we store it to prevent very strange errors with less
// verbose ways of binary <-> string data storage.
var BASE_CHARS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/';

var BLOB_TYPE_PREFIX = '~~local_forage_type~';
var BLOB_TYPE_PREFIX_REGEX = /^~~local_forage_type~([^~]+)~/;

var SERIALIZED_MARKER = '__lfsc__:';
var SERIALIZED_MARKER_LENGTH = SERIALIZED_MARKER.length;

// OMG the serializations!
var TYPE_ARRAYBUFFER = 'arbf';
var TYPE_BLOB = 'blob';
var TYPE_INT8ARRAY = 'si08';
var TYPE_UINT8ARRAY = 'ui08';
var TYPE_UINT8CLAMPEDARRAY = 'uic8';
var TYPE_INT16ARRAY = 'si16';
var TYPE_INT32ARRAY = 'si32';
var TYPE_UINT16ARRAY = 'ur16';
var TYPE_UINT32ARRAY = 'ui32';
var TYPE_FLOAT32ARRAY = 'fl32';
var TYPE_FLOAT64ARRAY = 'fl64';
var TYPE_SERIALIZED_MARKER_LENGTH = SERIALIZED_MARKER_LENGTH + TYPE_ARRAYBUFFER.length;

var toString$1 = Object.prototype.toString;

function stringToBuffer(serializedString) {
    // Fill the string into a ArrayBuffer.
    var bufferLength = serializedString.length * 0.75;
    var len = serializedString.length;
    var i;
    var p = 0;
    var encoded1, encoded2, encoded3, encoded4;

    if (serializedString[serializedString.length - 1] === '=') {
        bufferLength--;
        if (serializedString[serializedString.length - 2] === '=') {
            bufferLength--;
        }
    }

    var buffer = new ArrayBuffer(bufferLength);
    var bytes = new Uint8Array(buffer);

    for (i = 0; i < len; i += 4) {
        encoded1 = BASE_CHARS.indexOf(serializedString[i]);
        encoded2 = BASE_CHARS.indexOf(serializedString[i + 1]);
        encoded3 = BASE_CHARS.indexOf(serializedString[i + 2]);
        encoded4 = BASE_CHARS.indexOf(serializedString[i + 3]);

        /*jslint bitwise: true */
        bytes[p++] = encoded1 << 2 | encoded2 >> 4;
        bytes[p++] = (encoded2 & 15) << 4 | encoded3 >> 2;
        bytes[p++] = (encoded3 & 3) << 6 | encoded4 & 63;
    }
    return buffer;
}

// Converts a buffer to a string to store, serialized, in the backend
// storage library.
function bufferToString(buffer) {
    // base64-arraybuffer
    var bytes = new Uint8Array(buffer);
    var base64String = '';
    var i;

    for (i = 0; i < bytes.length; i += 3) {
        /*jslint bitwise: true */
        base64String += BASE_CHARS[bytes[i] >> 2];
        base64String += BASE_CHARS[(bytes[i] & 3) << 4 | bytes[i + 1] >> 4];
        base64String += BASE_CHARS[(bytes[i + 1] & 15) << 2 | bytes[i + 2] >> 6];
        base64String += BASE_CHARS[bytes[i + 2] & 63];
    }

    if (bytes.length % 3 === 2) {
        base64String = base64String.substring(0, base64String.length - 1) + '=';
    } else if (bytes.length % 3 === 1) {
        base64String = base64String.substring(0, base64String.length - 2) + '==';
    }

    return base64String;
}

// Serialize a value, afterwards executing a callback (which usually
// instructs the `setItem()` callback/promise to be executed). This is how
// we store binary data with localStorage.
function serialize(value, callback) {
    var valueType = '';
    if (value) {
        valueType = toString$1.call(value);
    }

    // Cannot use `value instanceof ArrayBuffer` or such here, as these
    // checks fail when running the tests using casper.js...
    //
    // TODO: See why those tests fail and use a better solution.
    if (value && (valueType === '[object ArrayBuffer]' || value.buffer && toString$1.call(value.buffer) === '[object ArrayBuffer]')) {
        // Convert binary arrays to a string and prefix the string with
        // a special marker.
        var buffer;
        var marker = SERIALIZED_MARKER;

        if (value instanceof ArrayBuffer) {
            buffer = value;
            marker += TYPE_ARRAYBUFFER;
        } else {
            buffer = value.buffer;

            if (valueType === '[object Int8Array]') {
                marker += TYPE_INT8ARRAY;
            } else if (valueType === '[object Uint8Array]') {
                marker += TYPE_UINT8ARRAY;
            } else if (valueType === '[object Uint8ClampedArray]') {
                marker += TYPE_UINT8CLAMPEDARRAY;
            } else if (valueType === '[object Int16Array]') {
                marker += TYPE_INT16ARRAY;
            } else if (valueType === '[object Uint16Array]') {
                marker += TYPE_UINT16ARRAY;
            } else if (valueType === '[object Int32Array]') {
                marker += TYPE_INT32ARRAY;
            } else if (valueType === '[object Uint32Array]') {
                marker += TYPE_UINT32ARRAY;
            } else if (valueType === '[object Float32Array]') {
                marker += TYPE_FLOAT32ARRAY;
            } else if (valueType === '[object Float64Array]') {
                marker += TYPE_FLOAT64ARRAY;
            } else {
                callback(new Error('Failed to get type for BinaryArray'));
            }
        }

        callback(marker + bufferToString(buffer));
    } else if (valueType === '[object Blob]') {
        // Conver the blob to a binaryArray and then to a string.
        var fileReader = new FileReader();

        fileReader.onload = function () {
            // Backwards-compatible prefix for the blob type.
            var str = BLOB_TYPE_PREFIX + value.type + '~' + bufferToString(this.result);

            callback(SERIALIZED_MARKER + TYPE_BLOB + str);
        };

        fileReader.readAsArrayBuffer(value);
    } else {
        try {
            callback(JSON.stringify(value));
        } catch (e) {
            console.error("Couldn't convert value into a JSON string: ", value);

            callback(null, e);
        }
    }
}

// Deserialize data we've inserted into a value column/field. We place
// special markers into our strings to mark them as encoded; this isn't
// as nice as a meta field, but it's the only sane thing we can do whilst
// keeping localStorage support intact.
//
// Oftentimes this will just deserialize JSON content, but if we have a
// special marker (SERIALIZED_MARKER, defined above), we will extract
// some kind of arraybuffer/binary data/typed array out of the string.
function deserialize(value) {
    // If we haven't marked this string as being specially serialized (i.e.
    // something other than serialized JSON), we can just return it and be
    // done with it.
    if (value.substring(0, SERIALIZED_MARKER_LENGTH) !== SERIALIZED_MARKER) {
        return JSON.parse(value);
    }

    // The following code deals with deserializing some kind of Blob or
    // TypedArray. First we separate out the type of data we're dealing
    // with from the data itself.
    var serializedString = value.substring(TYPE_SERIALIZED_MARKER_LENGTH);
    var type = value.substring(SERIALIZED_MARKER_LENGTH, TYPE_SERIALIZED_MARKER_LENGTH);

    var blobType;
    // Backwards-compatible blob type serialization strategy.
    // DBs created with older versions of localForage will simply not have the blob type.
    if (type === TYPE_BLOB && BLOB_TYPE_PREFIX_REGEX.test(serializedString)) {
        var matcher = serializedString.match(BLOB_TYPE_PREFIX_REGEX);
        blobType = matcher[1];
        serializedString = serializedString.substring(matcher[0].length);
    }
    var buffer = stringToBuffer(serializedString);

    // Return the right type based on the code/type set during
    // serialization.
    switch (type) {
        case TYPE_ARRAYBUFFER:
            return buffer;
        case TYPE_BLOB:
            return createBlob([buffer], { type: blobType });
        case TYPE_INT8ARRAY:
            return new Int8Array(buffer);
        case TYPE_UINT8ARRAY:
            return new Uint8Array(buffer);
        case TYPE_UINT8CLAMPEDARRAY:
            return new Uint8ClampedArray(buffer);
        case TYPE_INT16ARRAY:
            return new Int16Array(buffer);
        case TYPE_UINT16ARRAY:
            return new Uint16Array(buffer);
        case TYPE_INT32ARRAY:
            return new Int32Array(buffer);
        case TYPE_UINT32ARRAY:
            return new Uint32Array(buffer);
        case TYPE_FLOAT32ARRAY:
            return new Float32Array(buffer);
        case TYPE_FLOAT64ARRAY:
            return new Float64Array(buffer);
        default:
            throw new Error('Unkown type: ' + type);
    }
}

var localforageSerializer = {
    serialize: serialize,
    deserialize: deserialize,
    stringToBuffer: stringToBuffer,
    bufferToString: bufferToString
};

/*
 * Includes code from:
 *
 * base64-arraybuffer
 * https://github.com/niklasvh/base64-arraybuffer
 *
 * Copyright (c) 2012 Niklas von Hertzen
 * Licensed under the MIT license.
 */
// Open the WebSQL database (automatically creates one if one didn't
// previously exist), using any options set in the config.
function _initStorage$1(options) {
    var self = this;
    var dbInfo = {
        db: null
    };

    if (options) {
        for (var i in options) {
            dbInfo[i] = typeof options[i] !== 'string' ? options[i].toString() : options[i];
        }
    }

    var dbInfoPromise = new Promise$1(function (resolve, reject) {
        // Open the database; the openDatabase API will automatically
        // create it for us if it doesn't exist.
        try {
            dbInfo.db = openDatabase(dbInfo.name, String(dbInfo.version), dbInfo.description, dbInfo.size);
        } catch (e) {
            return reject(e);
        }

        // Create our key/value table if it doesn't exist.
        dbInfo.db.transaction(function (t) {
            t.executeSql('CREATE TABLE IF NOT EXISTS ' + dbInfo.storeName + ' (id INTEGER PRIMARY KEY, key unique, value)', [], function () {
                self._dbInfo = dbInfo;
                resolve();
            }, function (t, error) {
                reject(error);
            });
        });
    });

    dbInfo.serializer = localforageSerializer;
    return dbInfoPromise;
}

function getItem$1(key, callback) {
    var self = this;

    key = normalizeKey(key);

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            var dbInfo = self._dbInfo;
            dbInfo.db.transaction(function (t) {
                t.executeSql('SELECT * FROM ' + dbInfo.storeName + ' WHERE key = ? LIMIT 1', [key], function (t, results) {
                    var result = results.rows.length ? results.rows.item(0).value : null;

                    // Check to see if this is serialized content we need to
                    // unpack.
                    if (result) {
                        result = dbInfo.serializer.deserialize(result);
                    }

                    resolve(result);
                }, function (t, error) {

                    reject(error);
                });
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

function iterate$1(iterator, callback) {
    var self = this;

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            var dbInfo = self._dbInfo;

            dbInfo.db.transaction(function (t) {
                t.executeSql('SELECT * FROM ' + dbInfo.storeName, [], function (t, results) {
                    var rows = results.rows;
                    var length = rows.length;

                    for (var i = 0; i < length; i++) {
                        var item = rows.item(i);
                        var result = item.value;

                        // Check to see if this is serialized content
                        // we need to unpack.
                        if (result) {
                            result = dbInfo.serializer.deserialize(result);
                        }

                        result = iterator(result, item.key, i + 1);

                        // void(0) prevents problems with redefinition
                        // of `undefined`.
                        if (result !== void 0) {
                            resolve(result);
                            return;
                        }
                    }

                    resolve();
                }, function (t, error) {
                    reject(error);
                });
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

function _setItem(key, value, callback, retriesLeft) {
    var self = this;

    key = normalizeKey(key);

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            // The localStorage API doesn't return undefined values in an
            // "expected" way, so undefined is always cast to null in all
            // drivers. See: https://github.com/mozilla/localForage/pull/42
            if (value === undefined) {
                value = null;
            }

            // Save the original value to pass to the callback.
            var originalValue = value;

            var dbInfo = self._dbInfo;
            dbInfo.serializer.serialize(value, function (value, error) {
                if (error) {
                    reject(error);
                } else {
                    dbInfo.db.transaction(function (t) {
                        t.executeSql('INSERT OR REPLACE INTO ' + dbInfo.storeName + ' (key, value) VALUES (?, ?)', [key, value], function () {
                            resolve(originalValue);
                        }, function (t, error) {
                            reject(error);
                        });
                    }, function (sqlError) {
                        // The transaction failed; check
                        // to see if it's a quota error.
                        if (sqlError.code === sqlError.QUOTA_ERR) {
                            // We reject the callback outright for now, but
                            // it's worth trying to re-run the transaction.
                            // Even if the user accepts the prompt to use
                            // more storage on Safari, this error will
                            // be called.
                            //
                            // Try to re-run the transaction.
                            if (retriesLeft > 0) {
                                resolve(_setItem.apply(self, [key, originalValue, callback, retriesLeft - 1]));
                                return;
                            }
                            reject(sqlError);
                        }
                    });
                }
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

function setItem$1(key, value, callback) {
    return _setItem.apply(this, [key, value, callback, 1]);
}

function removeItem$1(key, callback) {
    var self = this;

    key = normalizeKey(key);

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            var dbInfo = self._dbInfo;
            dbInfo.db.transaction(function (t) {
                t.executeSql('DELETE FROM ' + dbInfo.storeName + ' WHERE key = ?', [key], function () {
                    resolve();
                }, function (t, error) {
                    reject(error);
                });
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

// Deletes every item in the table.
// TODO: Find out if this resets the AUTO_INCREMENT number.
function clear$1(callback) {
    var self = this;

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            var dbInfo = self._dbInfo;
            dbInfo.db.transaction(function (t) {
                t.executeSql('DELETE FROM ' + dbInfo.storeName, [], function () {
                    resolve();
                }, function (t, error) {
                    reject(error);
                });
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

// Does a simple `COUNT(key)` to get the number of items stored in
// localForage.
function length$1(callback) {
    var self = this;

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            var dbInfo = self._dbInfo;
            dbInfo.db.transaction(function (t) {
                // Ahhh, SQL makes this one soooooo easy.
                t.executeSql('SELECT COUNT(key) as c FROM ' + dbInfo.storeName, [], function (t, results) {
                    var result = results.rows.item(0).c;

                    resolve(result);
                }, function (t, error) {
                    reject(error);
                });
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

// Return the key located at key index X; essentially gets the key from a
// `WHERE id = ?`. This is the most efficient way I can think to implement
// this rarely-used (in my experience) part of the API, but it can seem
// inconsistent, because we do `INSERT OR REPLACE INTO` on `setItem()`, so
// the ID of each key will change every time it's updated. Perhaps a stored
// procedure for the `setItem()` SQL would solve this problem?
// TODO: Don't change ID on `setItem()`.
function key$1(n, callback) {
    var self = this;

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            var dbInfo = self._dbInfo;
            dbInfo.db.transaction(function (t) {
                t.executeSql('SELECT key FROM ' + dbInfo.storeName + ' WHERE id = ? LIMIT 1', [n + 1], function (t, results) {
                    var result = results.rows.length ? results.rows.item(0).key : null;
                    resolve(result);
                }, function (t, error) {
                    reject(error);
                });
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

function keys$1(callback) {
    var self = this;

    var promise = new Promise$1(function (resolve, reject) {
        self.ready().then(function () {
            var dbInfo = self._dbInfo;
            dbInfo.db.transaction(function (t) {
                t.executeSql('SELECT key FROM ' + dbInfo.storeName, [], function (t, results) {
                    var keys = [];

                    for (var i = 0; i < results.rows.length; i++) {
                        keys.push(results.rows.item(i).key);
                    }

                    resolve(keys);
                }, function (t, error) {
                    reject(error);
                });
            });
        })["catch"](reject);
    });

    executeCallback(promise, callback);
    return promise;
}

var webSQLStorage = {
    _driver: 'webSQLStorage',
    _initStorage: _initStorage$1,
    _support: isWebSQLValid(),
    iterate: iterate$1,
    getItem: getItem$1,
    setItem: setItem$1,
    removeItem: removeItem$1,
    clear: clear$1,
    length: length$1,
    key: key$1,
    keys: keys$1
};

function isLocalStorageValid() {
    try {
        return typeof localStorage !== 'undefined' && 'setItem' in localStorage && typeof localStorage.setItem === 'function';
    } catch (e) {
        return false;
    }
}

// Check if localStorage throws when saving an item
function checkIfLocalStorageThrows() {
    var localStorageTestKey = '_localforage_support_test';

    try {
        localStorage.setItem(localStorageTestKey, true);
        localStorage.removeItem(localStorageTestKey);

        return false;
    } catch (e) {
        return true;
    }
}

// Check if localStorage is usable and allows to save an item
// This method checks if localStorage is usable in Safari Private Browsing
// mode, or in any other case where the available quota for localStorage
// is 0 and there wasn't any saved items yet.
function _isLocalStorageUsable() {
    return !checkIfLocalStorageThrows() || localStorage.length > 0;
}

// Config the localStorage backend, using options set in the config.
function _initStorage$2(options) {
    var self = this;
    var dbInfo = {};
    if (options) {
        for (var i in options) {
            dbInfo[i] = options[i];
        }
    }

    dbInfo.keyPrefix = dbInfo.name + '/';

    if (dbInfo.storeName !== self._defaultConfig.storeName) {
        dbInfo.keyPrefix += dbInfo.storeName + '/';
    }

    if (!_isLocalStorageUsable()) {
        return Promise$1.reject();
    }

    self._dbInfo = dbInfo;
    dbInfo.serializer = localforageSerializer;

    return Promise$1.resolve();
}

// Remove all keys from the datastore, effectively destroying all data in
// the app's key/value store!
function clear$2(callback) {
    var self = this;
    var promise = self.ready().then(function () {
        var keyPrefix = self._dbInfo.keyPrefix;

        for (var i = localStorage.length - 1; i >= 0; i--) {
            var key = localStorage.key(i);

            if (key.indexOf(keyPrefix) === 0) {
                localStorage.removeItem(key);
            }
        }
    });

    executeCallback(promise, callback);
    return promise;
}

// Retrieve an item from the store. Unlike the original async_storage
// library in Gaia, we don't modify return values at all. If a key's value
// is `undefined`, we pass that value to the callback function.
function getItem$2(key, callback) {
    var self = this;

    key = normalizeKey(key);

    var promise = self.ready().then(function () {
        var dbInfo = self._dbInfo;
        var result = localStorage.getItem(dbInfo.keyPrefix + key);

        // If a result was found, parse it from the serialized
        // string into a JS object. If result isn't truthy, the key
        // is likely undefined and we'll pass it straight to the
        // callback.
        if (result) {
            result = dbInfo.serializer.deserialize(result);
        }

        return result;
    });

    executeCallback(promise, callback);
    return promise;
}

// Iterate over all items in the store.
function iterate$2(iterator, callback) {
    var self = this;

    var promise = self.ready().then(function () {
        var dbInfo = self._dbInfo;
        var keyPrefix = dbInfo.keyPrefix;
        var keyPrefixLength = keyPrefix.length;
        var length = localStorage.length;

        // We use a dedicated iterator instead of the `i` variable below
        // so other keys we fetch in localStorage aren't counted in
        // the `iterationNumber` argument passed to the `iterate()`
        // callback.
        //
        // See: github.com/mozilla/localForage/pull/435#discussion_r38061530
        var iterationNumber = 1;

        for (var i = 0; i < length; i++) {
            var key = localStorage.key(i);
            if (key.indexOf(keyPrefix) !== 0) {
                continue;
            }
            var value = localStorage.getItem(key);

            // If a result was found, parse it from the serialized
            // string into a JS object. If result isn't truthy, the
            // key is likely undefined and we'll pass it straight
            // to the iterator.
            if (value) {
                value = dbInfo.serializer.deserialize(value);
            }

            value = iterator(value, key.substring(keyPrefixLength), iterationNumber++);

            if (value !== void 0) {
                return value;
            }
        }
    });

    executeCallback(promise, callback);
    return promise;
}

// Same as localStorage's key() method, except takes a callback.
function key$2(n, callback) {
    var self = this;
    var promise = self.ready().then(function () {
        var dbInfo = self._dbInfo;
        var result;
        try {
            result = localStorage.key(n);
        } catch (error) {
            result = null;
        }

        // Remove the prefix from the key, if a key is found.
        if (result) {
            result = result.substring(dbInfo.keyPrefix.length);
        }

        return result;
    });

    executeCallback(promise, callback);
    return promise;
}

function keys$2(callback) {
    var self = this;
    var promise = self.ready().then(function () {
        var dbInfo = self._dbInfo;
        var length = localStorage.length;
        var keys = [];

        for (var i = 0; i < length; i++) {
            var itemKey = localStorage.key(i);
            if (itemKey.indexOf(dbInfo.keyPrefix) === 0) {
                keys.push(itemKey.substring(dbInfo.keyPrefix.length));
            }
        }

        return keys;
    });

    executeCallback(promise, callback);
    return promise;
}

// Supply the number of keys in the datastore to the callback function.
function length$2(callback) {
    var self = this;
    var promise = self.keys().then(function (keys) {
        return keys.length;
    });

    executeCallback(promise, callback);
    return promise;
}

// Remove an item from the store, nice and simple.
function removeItem$2(key, callback) {
    var self = this;

    key = normalizeKey(key);

    var promise = self.ready().then(function () {
        var dbInfo = self._dbInfo;
        localStorage.removeItem(dbInfo.keyPrefix + key);
    });

    executeCallback(promise, callback);
    return promise;
}

// Set a key's value and run an optional callback once the value is set.
// Unlike Gaia's implementation, the callback function is passed the value,
// in case you want to operate on that value only after you're sure it
// saved, or something like that.
function setItem$2(key, value, callback) {
    var self = this;

    key = normalizeKey(key);

    var promise = self.ready().then(function () {
        // Convert undefined values to null.
        // https://github.com/mozilla/localForage/pull/42
        if (value === undefined) {
            value = null;
        }

        // Save the original value to pass to the callback.
        var originalValue = value;

        return new Promise$1(function (resolve, reject) {
            var dbInfo = self._dbInfo;
            dbInfo.serializer.serialize(value, function (value, error) {
                if (error) {
                    reject(error);
                } else {
                    try {
                        localStorage.setItem(dbInfo.keyPrefix + key, value);
                        resolve(originalValue);
                    } catch (e) {
                        // localStorage capacity exceeded.
                        // TODO: Make this a specific error/event.
                        if (e.name === 'QuotaExceededError' || e.name === 'NS_ERROR_DOM_QUOTA_REACHED') {
                            reject(e);
                        }
                        reject(e);
                    }
                }
            });
        });
    });

    executeCallback(promise, callback);
    return promise;
}

var localStorageWrapper = {
    _driver: 'localStorageWrapper',
    _initStorage: _initStorage$2,
    _support: isLocalStorageValid(),
    iterate: iterate$2,
    getItem: getItem$2,
    setItem: setItem$2,
    removeItem: removeItem$2,
    clear: clear$2,
    length: length$2,
    key: key$2,
    keys: keys$2
};

var isArray = Array.isArray || function (arg) {
    return Object.prototype.toString.call(arg) === '[object Array]';
};

// Drivers are stored here when `defineDriver()` is called.
// They are shared across all instances of localForage.
var DefinedDrivers = {};

var DriverSupport = {};

var DefaultDrivers = {
    INDEXEDDB: asyncStorage,
    WEBSQL: webSQLStorage,
    LOCALSTORAGE: localStorageWrapper
};

var DefaultDriverOrder = [DefaultDrivers.INDEXEDDB._driver, DefaultDrivers.WEBSQL._driver, DefaultDrivers.LOCALSTORAGE._driver];

var LibraryMethods = ['clear', 'getItem', 'iterate', 'key', 'keys', 'length', 'removeItem', 'setItem'];

var DefaultConfig = {
    description: '',
    driver: DefaultDriverOrder.slice(),
    name: 'localforage',
    // Default DB size is _JUST UNDER_ 5MB, as it's the highest size
    // we can use without a prompt.
    size: 4980736,
    storeName: 'keyvaluepairs',
    version: 1.0
};

function callWhenReady(localForageInstance, libraryMethod) {
    localForageInstance[libraryMethod] = function () {
        var _args = arguments;
        return localForageInstance.ready().then(function () {
            return localForageInstance[libraryMethod].apply(localForageInstance, _args);
        });
    };
}

function extend() {
    for (var i = 1; i < arguments.length; i++) {
        var arg = arguments[i];

        if (arg) {
            for (var _key in arg) {
                if (arg.hasOwnProperty(_key)) {
                    if (isArray(arg[_key])) {
                        arguments[0][_key] = arg[_key].slice();
                    } else {
                        arguments[0][_key] = arg[_key];
                    }
                }
            }
        }
    }

    return arguments[0];
}

var LocalForage = function () {
    function LocalForage(options) {
        _classCallCheck(this, LocalForage);

        for (var driverTypeKey in DefaultDrivers) {
            if (DefaultDrivers.hasOwnProperty(driverTypeKey)) {
                var driver = DefaultDrivers[driverTypeKey];
                var driverName = driver._driver;
                this[driverTypeKey] = driverName;

                if (!DefinedDrivers[driverName]) {
                    // we don't need to wait for the promise,
                    // since the default drivers can be defined
                    // in a blocking manner
                    this.defineDriver(driver);
                }
            }
        }

        this._defaultConfig = extend({}, DefaultConfig);
        this._config = extend({}, this._defaultConfig, options);
        this._driverSet = null;
        this._initDriver = null;
        this._ready = false;
        this._dbInfo = null;

        this._wrapLibraryMethodsWithReady();
        this.setDriver(this._config.driver)["catch"](function () {});
    }

    // Set any config values for localForage; can be called anytime before
    // the first API call (e.g. `getItem`, `setItem`).
    // We loop through options so we don't overwrite existing config
    // values.


    LocalForage.prototype.config = function config(options) {
        // If the options argument is an object, we use it to set values.
        // Otherwise, we return either a specified config value or all
        // config values.
        if ((typeof options === 'undefined' ? 'undefined' : _typeof(options)) === 'object') {
            // If localforage is ready and fully initialized, we can't set
            // any new configuration values. Instead, we return an error.
            if (this._ready) {
                return new Error('Can\'t call config() after localforage ' + 'has been used.');
            }

            for (var i in options) {
                if (i === 'storeName') {
                    options[i] = options[i].replace(/\W/g, '_');
                }

                if (i === 'version' && typeof options[i] !== 'number') {
                    return new Error('Database version must be a number.');
                }

                this._config[i] = options[i];
            }

            // after all config options are set and
            // the driver option is used, try setting it
            if ('driver' in options && options.driver) {
                return this.setDriver(this._config.driver);
            }

            return true;
        } else if (typeof options === 'string') {
            return this._config[options];
        } else {
            return this._config;
        }
    };

    // Used to define a custom driver, shared across all instances of
    // localForage.


    LocalForage.prototype.defineDriver = function defineDriver(driverObject, callback, errorCallback) {
        var promise = new Promise$1(function (resolve, reject) {
            try {
                var driverName = driverObject._driver;
                var complianceError = new Error('Custom driver not compliant; see ' + 'https://mozilla.github.io/localForage/#definedriver');

                // A driver name should be defined and not overlap with the
                // library-defined, default drivers.
                if (!driverObject._driver) {
                    reject(complianceError);
                    return;
                }

                var driverMethods = LibraryMethods.concat('_initStorage');
                for (var i = 0, len = driverMethods.length; i < len; i++) {
                    var customDriverMethod = driverMethods[i];
                    if (!customDriverMethod || !driverObject[customDriverMethod] || typeof driverObject[customDriverMethod] !== 'function') {
                        reject(complianceError);
                        return;
                    }
                }

                var setDriverSupport = function setDriverSupport(support) {
                    if (DefinedDrivers[driverName]) {
                        console.info('Redefining LocalForage driver: ' + driverName);
                    }
                    DefinedDrivers[driverName] = driverObject;
                    DriverSupport[driverName] = support;
                    // don't use a then, so that we can define
                    // drivers that have simple _support methods
                    // in a blocking manner
                    resolve();
                };

                if ('_support' in driverObject) {
                    if (driverObject._support && typeof driverObject._support === 'function') {
                        driverObject._support().then(setDriverSupport, reject);
                    } else {
                        setDriverSupport(!!driverObject._support);
                    }
                } else {
                    setDriverSupport(true);
                }
            } catch (e) {
                reject(e);
            }
        });

        executeTwoCallbacks(promise, callback, errorCallback);
        return promise;
    };

    LocalForage.prototype.driver = function driver() {
        return this._driver || null;
    };

    LocalForage.prototype.getDriver = function getDriver(driverName, callback, errorCallback) {
        var getDriverPromise = DefinedDrivers[driverName] ? Promise$1.resolve(DefinedDrivers[driverName]) : Promise$1.reject(new Error('Driver not found.'));

        executeTwoCallbacks(getDriverPromise, callback, errorCallback);
        return getDriverPromise;
    };

    LocalForage.prototype.getSerializer = function getSerializer(callback) {
        var serializerPromise = Promise$1.resolve(localforageSerializer);
        executeTwoCallbacks(serializerPromise, callback);
        return serializerPromise;
    };

    LocalForage.prototype.ready = function ready(callback) {
        var self = this;

        var promise = self._driverSet.then(function () {
            if (self._ready === null) {
                self._ready = self._initDriver();
            }

            return self._ready;
        });

        executeTwoCallbacks(promise, callback, callback);
        return promise;
    };

    LocalForage.prototype.setDriver = function setDriver(drivers, callback, errorCallback) {
        var self = this;

        if (!isArray(drivers)) {
            drivers = [drivers];
        }

        var supportedDrivers = this._getSupportedDrivers(drivers);

        function setDriverToConfig() {
            self._config.driver = self.driver();
        }

        function extendSelfWithDriver(driver) {
            self._extend(driver);
            setDriverToConfig();

            self._ready = self._initStorage(self._config);
            return self._ready;
        }

        function initDriver(supportedDrivers) {
            return function () {
                var currentDriverIndex = 0;

                function driverPromiseLoop() {
                    while (currentDriverIndex < supportedDrivers.length) {
                        var driverName = supportedDrivers[currentDriverIndex];
                        currentDriverIndex++;

                        self._dbInfo = null;
                        self._ready = null;

                        return self.getDriver(driverName).then(extendSelfWithDriver)["catch"](driverPromiseLoop);
                    }

                    setDriverToConfig();
                    var error = new Error('No available storage method found.');
                    self._driverSet = Promise$1.reject(error);
                    return self._driverSet;
                }

                return driverPromiseLoop();
            };
        }

        // There might be a driver initialization in progress
        // so wait for it to finish in order to avoid a possible
        // race condition to set _dbInfo
        var oldDriverSetDone = this._driverSet !== null ? this._driverSet["catch"](function () {
            return Promise$1.resolve();
        }) : Promise$1.resolve();

        this._driverSet = oldDriverSetDone.then(function () {
            var driverName = supportedDrivers[0];
            self._dbInfo = null;
            self._ready = null;

            return self.getDriver(driverName).then(function (driver) {
                self._driver = driver._driver;
                setDriverToConfig();
                self._wrapLibraryMethodsWithReady();
                self._initDriver = initDriver(supportedDrivers);
            });
        })["catch"](function () {
            setDriverToConfig();
            var error = new Error('No available storage method found.');
            self._driverSet = Promise$1.reject(error);
            return self._driverSet;
        });

        executeTwoCallbacks(this._driverSet, callback, errorCallback);
        return this._driverSet;
    };

    LocalForage.prototype.supports = function supports(driverName) {
        return !!DriverSupport[driverName];
    };

    LocalForage.prototype._extend = function _extend(libraryMethodsAndProperties) {
        extend(this, libraryMethodsAndProperties);
    };

    LocalForage.prototype._getSupportedDrivers = function _getSupportedDrivers(drivers) {
        var supportedDrivers = [];
        for (var i = 0, len = drivers.length; i < len; i++) {
            var driverName = drivers[i];
            if (this.supports(driverName)) {
                supportedDrivers.push(driverName);
            }
        }
        return supportedDrivers;
    };

    LocalForage.prototype._wrapLibraryMethodsWithReady = function _wrapLibraryMethodsWithReady() {
        // Add a stub for each driver API method that delays the call to the
        // corresponding driver method until localForage is ready. These stubs
        // will be replaced by the driver methods as soon as the driver is
        // loaded, so there is no performance impact.
        for (var i = 0, len = LibraryMethods.length; i < len; i++) {
            callWhenReady(this, LibraryMethods[i]);
        }
    };

    LocalForage.prototype.createInstance = function createInstance(options) {
        return new LocalForage(options);
    };

    return LocalForage;
}();

// The actual localForage object that we expose as a module or via a
// global. It's extended by pulling in one of our other libraries.


var localforage_js = new LocalForage();

module.exports = localforage_js;

},{"3":3}]},{},[4])(4)
});
}).call(this,typeof global !== "undefined" ? global : typeof self !== "undefined" ? self : typeof window !== "undefined" ? window : {})

},{}],7:[function(require,module,exports){
'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _mvc = require('./lib/mvc.js');

var _IStore = require('./store/IStore.js');

var _IStore2 = _interopRequireDefault(_IStore);

var _dryDom = require('./lib/dry-dom.js');

var _dryDom2 = _interopRequireDefault(_dryDom);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _toConsumableArray(arr) { if (Array.isArray(arr)) { for (var i = 0, arr2 = Array(arr.length); i < arr.length; i++) { arr2[i] = arr[i]; } return arr2; } else { return Array.from(arr); } }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

window.strldr = require("./lib/strldr.js");

var controllers = {};

var App = function () {
	function App() {
		_classCallCheck(this, App);

		this.keySequences = {
			'Konami': ['ArrowUp', 'ArrowUp', 'ArrowDown', 'ArrowDown', 'ArrowLeft', 'ArrowRight', 'ArrowLeft', 'ArrowRight', 'KeyA', 'KeyB']
		};
		this.activeSequences = [];
		this.iemap = {
			"Up": "ArrowUp",
			"Down": "ArrowDown",
			"Left": "ArrowLeft",
			"Right": "ArrowRight",
			"Alt": "AltLeft",
			"CONTROL": "ControlLeft"
		};
		this.gamepadmap = {
			0: "ControlLeft",
			1: "AltLeft",
			2: "ControlLeft",
			3: "AltLeft",
			12: "ArrowUp",
			15: "ArrowRight",
			13: "ArrowDown",
			14: "ArrowLeft"
		};
		this.unblockable = {
			"F6": true,
			"F7": true,
			"F8": true
		};
		this.mappings = {};


		window.store = this.store;

		this.pool.add(this);

		this.models = [];

		this.store.onload = this.init.bind(this);
	}

	_createClass(App, [{
		key: 'remapKey',
		value: function remapKey(i, o) {
			if (i && (typeof i === 'undefined' ? 'undefined' : _typeof(i)) == "object") {
				for (var k in i) {
					this.mappings[k] = i[k];
				}return;
			}
			this.mappings[i] = o;
		}
	}, {
		key: 'init',
		value: function init() {
			var _this = this;

			this.plugins.forEach(function (p) {
				_this.pool.add(p);
			});

			this.initKeyboard();
			this.initControllers();

			this.pool.call("enterSplash");

			setInterval(this.commit.bind(this), 3000);

			var pending = 2,
			    dropFile = function dropFile(event) {
				_this.pool.call("onDropFile", new _dryDom2.default(event.target), event);
			};

			this.openModel("app", done.bind(this));
			setTimeout(done.bind(this), 1000);

			window.addEventListener("resize", function (_) {
				return _this.pool.call("onResizeWindow");
			});
			document.body.addEventListener("dragenter", cancelEvent);
			document.body.addEventListener("dragover", cancelEvent);
			document.body.addEventListener("drop", dropFile);

			function cancelEvent(event) {
				event.stopPropagation();
				event.preventDefault();
			}

			function done() {
				pending--;
				if (!pending) this.pool.call("exitSplash");
			}
		}
	}, {
		key: 'initControllers',
		value: function initControllers() {

			controllers = {};

			var connecthandler = function connecthandler(e) {
				controllers[e.gamepad.index] = { gamepad: e.gamepad, state: {} };
			};

			var disconnecthandler = function disconnecthandler(e) {
				delete controllers[e.gamepad.index];
			};

			window.addEventListener("gamepadconnected", connecthandler);
			window.addEventListener("gamepaddisconnected", disconnecthandler);
		}
	}, {
		key: 'onPollTickListeners',
		value: function onPollTickListeners(list) {
			list.push(this);
		}
	}, {
		key: 'tick',
		value: function tick() {

			Array.from((navigator.getGamepads || navigator.webkitGetGamepads || function (_) {
				return [];
			}).call(navigator)).filter(function (gp) {
				return !!gp;
			}).reduce(function (c, gp) {
				if (c[gp.index]) c[gp.index].gamepad = gp;else c[gp.index] = { gamepad: gp, state: {} };
				return c;
			}, controllers);

			for (var k in controllers) {
				var _controllers$k = controllers[k],
				    gamepad = _controllers$k.gamepad,
				    state = _controllers$k.state;


				var upDown = gamepad.axes[1],
				    leftRight = gamepad.axes[0];

				if (upDown < -0.5) upDown = -1;else if (upDown > 0.5) upDown = 1;else upDown = 0;

				if (leftRight < -0.5) leftRight = -1;else if (leftRight > 0.5) leftRight = 1;else leftRight = 0;

				if (upDown != state.upDown) {

					if (state.upDown < 0) this.inputUp("ArrowUp");else if (state.upDown > 0) this.inputUp("ArrowDown");

					state.upDown = upDown;

					if (upDown < 0) this.inputDown("ArrowUp");else if (upDown > 0) this.inputDown("ArrowDown");
				}

				if (leftRight != state.leftRight) {

					if (state.leftRight < 0) this.inputUp("ArrowLeft");else if (state.leftRight > 0) this.inputUp("ArrowRight");

					state.leftRight = leftRight;

					if (leftRight < 0) this.inputDown("ArrowLeft");else if (leftRight > 0) this.inputDown("ArrowRight");
				}

				for (var i in this.gamepadmap) {
					var pressed = gamepad.buttons[i];

					if ((typeof pressed === 'undefined' ? 'undefined' : _typeof(pressed)) == "object") pressed = pressed.pressed;else pressed = pressed >= 0.5;

					if (pressed != state[i]) {
						state[i] = pressed;

						if (pressed) this.inputDown(this.gamepadmap[i]);else this.inputUp(this.gamepadmap[i]);
					}
				}
			}
		}
	}, {
		key: 'inputDown',
		value: function inputDown(code) {
			code = this.mappings[code] || code;
			for (var i = 0; i < this.activeSequences.length; ++i) {
				var obj = this.activeSequences[i];
				var next = obj.seq[obj.pos++];

				if (next !== code || obj.pos >= obj.seq.length) {
					if (next == code) this.pool.call(obj.name);
					this.activeSequences.splice(i--, 1);
				}
			}

			for (var k in this.keySequences) {
				var seq = this.keySequences[k];
				if (seq[0] == code) this.activeSequences.push({
					name: k,
					pos: 1,
					seq: seq
				});
			}

			return this.pool.call("onPress" + code);
		}
	}, {
		key: 'inputUp',
		value: function inputUp(code) {
			return this.pool.call("onRelease" + (this.mappings[code] || code));
		}
	}, {
		key: 'initKeyboard',
		value: function initKeyboard() {
			var _this2 = this;

			document.body.addEventListener("keydown", function (evt) {

				var code = evt.code;
				if (code === undefined) code = _this2.iemap[evt.key] || "Key" + evt.key.toUpperCase();

				if ((evt.target.tagName == "INPUT" || evt.target.tagName == "TEXTAREA") && !_this2.unblockable[code]) return;

				var ret = _this2.inputDown(code);
				if (ret === true) {
					evt.preventDefault();
					evt.stopPropagation();
				}
			});

			document.body.addEventListener("keyup", function (evt) {

				var code = evt.code;
				if (code === undefined) code = _this2.iemap[evt.key] || "Key" + evt.key.toUpperCase();

				if ((evt.target.tagName == "INPUT" || evt.target.tagName == "TEXTAREA") && !_this2.unblockable[code]) return;

				var ret = _this2.inputUp(code);
				if (ret === true) {
					evt.preventDefault();
					evt.stopPropagation();
				}
			});

			this.controllers.forEach(function (controller) {
				_this2.pool.add(controller);
			});
		}
	}, {
		key: 'openModel',
		value: function openModel(name, cb, model) {
			var _this3 = this;

			var oldModel = this.models.find(function (obj) {
				return obj.name == name;
			});

			if (oldModel) {

				if (oldModel == model) return;
				this.closeModel(name);
			}

			var path = name;

			if (typeof model == "string") {
				path = model;
				model = null;
			}

			if (!model) model = new _mvc.Model();

			this.root.setItem(name, model.data);

			this.models[this.models.length] = {
				model: model,
				name: name,
				path: path,
				dirty: false
			};

			this.store.getTextItem(path, function (data) {

				var onGetModel = function onGetModel(data) {

					if (data) {

						model.load(data);
						if (model.getItem("color") === undefined) model.setItem("color", Math.random() * 10 | 0);

						if (model.version === 1) {
							model.dirty = false;
							cb.call();
							return;
						}

						model.setItem("version", 1);

						model.setItem("color", Math.random() * 10 | 0);
					} else if (model.getItem("color") === undefined) model.setItem("color", Math.random() * 10 | 0);

					_this3.pool.call(name + "ModelInit", model, cb);
				};

				if (data) {
					try {
						data = JSON.parse(data);
					} catch (ex) {
						data = null;
					}
				}

				if (!data || !Array.isArray(data)) return onGetModel(data);

				var map = {},
				    pending = data.length;

				data.forEach(function (de) {

					_this3.store.getTextItem(path + "/" + de, function (item) {
						if (typeof item == 'string') map[de] = JSON.parse(item);

						pending--;
						if (!pending) onGetModel(map);
					});
				});
			});
		}
	}, {
		key: 'closeModel',
		value: function closeModel(name) {
			// to-do: find, commit, remove from this.models
		}
	}, {
		key: 'appModelInit',
		value: function appModelInit(model, cb) {

			var repoURLs = model.getModel("repoList");

			model.setItem("proxy", this.root.getItem("proxy", ""));

			model.setItem("repoList", {
				GameJam3: "https://www.arduboy.com/a/GameJam3.json",
				Eried: "http://arduboy.ried.cl/repo.json",
				crait: "http://www.crait.net/arduboy/repo2.json",
				TeamARG: "repo.json"
			});

			cb();
		}
	}, {
		key: 'commit',
		value: function commit() {

			for (var i = 0; i < this.models.length; ++i) {

				var obj = this.models[i];
				if (!obj.dirty && obj.model.dirty) {

					obj.dirty = true;
					obj.model.dirty = false;
				} else if (obj.dirty && !obj.model.dirty) {

					obj.dirty = false;
					// this.store.setItem( obj.path, JSON.stringify(obj.model.data) );
					this.store.setItem(obj.path, JSON.stringify(Object.keys(obj.model.data)));
					for (var k in obj.model.data) {
						this.store.setItem(obj.path + "/" + k, JSON.stringify(obj.model.data[k]));
					}
				} else if (obj.dirty && obj.model.dirty) {

					obj.model.dirty = false;
				}
			}
		}
	}, {
		key: 'setActiveView',
		value: function setActiveView(view) {
			[].concat(_toConsumableArray(this.DOM.element.children)).forEach(function (node) {
				return node.parentElement.removeChild(node);
			});
		}
	}]);

	return App;
}();

App["@inject"] = {
	DOM: _dryDom2.default,
	store: _IStore2.default,
	pool: "pool",
	controllers: [_mvc.IController, []],
	root: [_mvc.Model, { scope: "root" }],
	plugins: ["Plugin", []]
};
exports.default = App;

},{"./lib/dry-dom.js":31,"./lib/mvc.js":35,"./lib/strldr.js":37,"./store/IStore.js":47}],8:[function(require,module,exports){
"use strict";

module.exports = {

    write: {
        0xC0: function _(value) {
            return this.UCSR0A = this.UCSR0A & 188 | value & 67;
        },
        0xC1: function _(value) {
            return this.UCSR0B = value;
        },
        0xC2: function _(value) {
            return this.UCSR0C = value;
        },
        0xC4: function _(value) {
            return this.UBRR0L = value;
        },
        0xC5: function _(value) {
            return this.UBRR0H = value;
        },
        0xC6: function _(value) {
            this.core.pins.serial0 = (this.core.pins.serial0 || "") + String.fromCharCode(value);return this.UDR0 = value;
        }
    },

    read: {
        0xC0: function _() {
            return this.UCSR0A;
        },
        0xC1: function _() {
            return this.UCSR0B;
        },
        0xC2: function _() {
            return this.UCSR0C;
        },
        0xC4: function _() {
            return this.UBRR0L;
        },
        0xC5: function _() {
            return this.UBRR0H & 0x0F;
        },
        0xC6: function _() {
            return this.UDR0;
        }
    },

    init: function init() {
        this.UCSR0A = 0x20;
        this.UCSR0B = 0;
        this.UCSR0C = 0x06;
        this.UBRR0L = 0; // USART Baud Rate 0 Register Low
        this.UBRR0H = 0; // USART Baud Rate 0 Register High
        this.UDR0 = 0;
    },

    update: function update(tick, ie) {}

};

},{}],9:[function(require,module,exports){
"use strict";

var _write, _write2, _write3;

function _defineProperty(obj, key, value) { if (key in obj) { Object.defineProperty(obj, key, { value: value, enumerable: true, configurable: true, writable: true }); } else { obj[key] = value; } return obj; }

module.exports = {

    PORTB: {
        write: (_write = {}, _defineProperty(_write, 0x04 + 0x20, function (value) {
            this.core.pins.DDRB = value;
        }), _defineProperty(_write, 0x05 + 0x20, function (value, oldValue) {

            if (oldValue == value) return;
            this.core.pins.PORTB = value;
        }), _write),
        read: _defineProperty({}, 0x03 + 0x20, function () {
            return this.PINB & 0xFF | 0;
        }),
        init: function init() {
            var _this = this;

            this.PINB = 0;
            Object.defineProperty(this.core.pins, "PINB", {
                set: function set(v) {
                    return _this.PINB = v >>> 0 & 0xFF;
                },
                get: function get() {
                    return _this.PINB;
                }
            });
        }
    },

    PORTC: {
        write: (_write2 = {}, _defineProperty(_write2, 0x07 + 0x20, function (value) {
            this.core.pins.DDRC = value;
        }), _defineProperty(_write2, 0x08 + 0x20, function (value) {
            this.core.pins.PORTC = value;
        }), _write2),
        read: _defineProperty({}, 0x06 + 0x20, function () {
            return this.core.pins.PINC = this.core.pins.PINC & 0xFF || 0;
        })
    },

    PORTD: {
        write: (_write3 = {}, _defineProperty(_write3, 0x0A + 0x20, function (value) {
            this.core.pins.DDRD = value;
        }), _defineProperty(_write3, 0x0B + 0x20, function (value) {
            this.core.pins.PORTD = value;
        }), _write3),
        read: _defineProperty({}, 0x09 + 0x20, function () {
            return this.core.pins.PIND = this.core.pins.PIND & 0xFF || 0;
        })
    },

    TC: require('./Timer8.js')({
        TIFR: 0x35,
        TCCR_A: 0x44,
        TCCR_B: 0x45,
        OCR_A: 0x47,
        OCR_B: 0x48,
        TIMSK: 0x6E,
        TCNT: 0x46,
        intOV: "TIMER0O",
        cmpA: "TIMER0A",
        cmpB: "TIMER0B"
    }),

    USART: require('./At328P-USART.js')

};

},{"./At328P-USART.js":8,"./Timer8.js":17}],10:[function(require,module,exports){
"use strict";

module.exports = {
			init: function init() {
						this.SPDR = 0;
						this.SPIF = 0;
						this.WCOL = 0;
						this.SPI2X = 0;
						this.SPIE = 0;
						this.SPE = 0;
						this.DORD = 0;
						this.MSTR = 0;
						this.CPOL = 0;
						this.CPHA = 0;
						this.SPR1 = 0;
						this.SPR0 = 0;
						this.core.pins.spiOut = this.core.pins.spiOut || [];
			},

			write: {
						0x4C: function _(value, oldValue) {
									this.SPIE = value >> 7;
									this.SPE = value >> 6;
									this.DORD = value >> 5;
									this.MSTR = value >> 4;
									this.CPOL = value >> 3;
									this.CPHA = value >> 2;
									this.SPR1 = value >> 1;
									this.SPR0 = value >> 0;
						},

						0x4D: function _(value, oldValue) {
									this.SPI2X = value & 1;
									return this.SPIF << 7 | this.WCOL << 6 | this.SPI2X;
						},
						0x4E: function _(value) {
									this.SPDR = value;
									this.core.pins.spiOut.push(value);
									this.SPIF = 1;
						}
			},

			read: {
						0x4D: function _() {
									// this.SPIF = (!!this.core.pins.spiIn.length) | 0;
									return this.SPIF << 7 | this.WCOL << 6 | this.SPI2X;
						},
						0x4E: function _() {
									var spiIn = this.core.pins.spiIn;
									if (spiIn.length) return this.SPDR = spiIn.shift();
									return this.SPDR;
						}
			},

			update: function update(tick, ie) {

						if (this.SPIF && this.SPIE && ie) {
									this.SPIF = 0;
									return "SPI";
						}
			}
};

},{}],11:[function(require,module,exports){
'use strict';

var _write, _read;

function _defineProperty(obj, key, value) { if (key in obj) { Object.defineProperty(obj, key, { value: value, enumerable: true, configurable: true, writable: true }); } else { obj[key] = value; } return obj; }

var USBINT = 0xDA,
    USBSTA = 0xD9,
    USBCON = 0xD8,
    UHWCON = 0xD7,
    UDIEN = 0xE2,
    UDINT = 0xE1;

var UEINT = 0xF4; //  - EPINT6:0
var UEBCHX = 0xF3; //  - - - - - BYCT10:8
var UEBCLX = 0xF2; //  BYCT7:0
var UEDATX = 0xF1; //  DAT7:0
var UEIENX = 0xF0; //  FLERRE NAKINE - NAKOUTE RXSTPE RXOUTE STALLEDE TXINE
var UESTA1X = 0xEF; //  - - - - - CTRLDIR CURRBK1:0
var UESTA0X = 0xEE; //  CFGOK OVERFI UNDERFI - DTSEQ1:0 NBUSYBK1:0
var UECFG1X = 0xED; //  EPSIZE2:0 EPBK1:0 ALLOC -
var UECFG0X = 0xEC; //  EPTYPE1:0 - - - - - EPDIR
var UECONX = 0xEB; //  - - STALLRQ STALLRQC RSTDT - - EPEN
var UERST = 0xEA; //  - EPRST6:0
var UENUM = 0xE9; //  - - - - - EPNUM2:0
var UEINTX = 0xE8; //  FIFOCON NAKINI RWAL NAKOUTI RXSTPI RXOUTI STALLEDI TXINI

module.exports = {

	write: (_write = {}, _defineProperty(_write, UENUM, function (v) {
		this.UENUM = v & 0x7;
		return this.UENUM;
	}), _defineProperty(_write, UEBCHX, function (v) {
		return this.EP[this.UENUM].UEBCHX;
	}), _defineProperty(_write, UEBCLX, function (v) {
		return this.EP[this.UENUM].UEBCLX;
	}), _defineProperty(_write, UEDATX, function (v) {
		//	    if( this.EP[this.UENUM].UECFG0X & 1 == 0 )

		this.core.pins.serial0Out.push(v);

		// this.EP[this.UENUM].write(v);
		//	    return v;
	}), _defineProperty(_write, UEIENX, function (v) {
		return this.EP[this.UENUM].UEIENX = v;
	}), _defineProperty(_write, UESTA1X, function (v) {
		return this.EP[this.UENUM].UESTA1X = v;
	}), _defineProperty(_write, UESTA0X, function (v) {
		return this.EP[this.UENUM].UESTA0X = v;
	}), _defineProperty(_write, UECFG1X, function (v) {
		this.EP[this.UENUM].UECFG1X = v;
		this.EP[this.UENUM].configure();
		return v;
	}), _defineProperty(_write, UECFG0X, function (v) {
		return this.EP[this.UENUM].UECFG0X = v;
	}), _defineProperty(_write, UECONX, function (v) {
		return this.EP[this.UENUM].UECONX = v;
	}), _defineProperty(_write, UERST, function (v) {
		return this.EP[this.UENUM].UERST = v;
	}), _defineProperty(_write, UENUM, function (v) {
		return this.EP[this.UENUM].UENUM = v;
	}), _defineProperty(_write, UEINTX, function (v) {
		return this.EP[this.UENUM].UEINTX &= v;
	}), _defineProperty(_write, USBCON, function (v) {
		var _this = this;

		this.VBUSTE = v & 1;
		this.OTGPADE = v >> 4 & 1;
		this.FRZCLK = v >> 5 & 1;
		this.USBE = v >> 7 & 1;

		if (this.USBE) {
			this.VBUSTI = 1;
			this.timeout = 100;
			this.timeoutCB = function (_) {

				_this.EORSTI = 1;
				var ep0 = _this.EP[0];
				ep0.UECONX = 1;
				ep0.UECFG1X = 3 << 4 | 1 << 1;
				ep0.configure();
				ep0.UEIENX = 1 << 3;
				ep0.UEINTX = 1 << 3;

				[0, // uint8_t bmRequestType;
				9, // uint8_t bRequest;
				1, // uint8_t wValueL;
				0, // uint8_t wValueH;
				0, 0, // uint16_t wIndex;
				0, 0 // uint16_t wLength;

				].forEach(function (b) {
					return ep0.write(b);
				});

				_this.timeout = 100;
				_this.timeoutCB = function (_) {
					ep0.UEINTX = 1 << 3;

					[0x21, // uint8_t bmRequestType;
					0x22, // uint8_t bRequest;
					3, // uint8_t wValueL;
					0, // uint8_t wValueH;
					0, 0, // uint16_t wIndex;
					0, 64 // uint16_t wLength;

					].forEach(function (b) {
						return ep0.write(b);
					});
				};
			};
		} else this.reset();

		return v;
	}), _defineProperty(_write, USBSTA, function (v) {
		this.VBUS = v & 1;
		return this.VBUS | 2;
	}), _defineProperty(_write, UHWCON, function (v) {
		this.UVREGE = v & 1;
		return this.UVREGE;
	}), _defineProperty(_write, UDIEN, function (v) {
		this.SUSPE = v & 1;
		this.MSOFE = v >> 1 & 1;
		this.SOFE = v >> 2 & 1;
		this.EORSTE = v >> 3 & 1;
		this.WAKEUPE = v >> 4 & 1;
		this.EORSME = v >> 5 & 1;
		this.UPRSME = v >> 6 & 1;
		return v;
	}), _defineProperty(_write, UDINT, function (v) {
		this.SUSPI = v >> 0 & 1;
		this.MSOFI = v >> 1 & 1;
		this.SOFI = v >> 2 & 1;
		this.EORSTI = v >> 3 & 1;
		this.WAKEUPI = v >> 4 & 1;
		this.EORSMI = v >> 5 & 1;
		this.UPRSMI = v >> 6 & 1;
		return v;
	}), _defineProperty(_write, USBINT, function (v) {
		return this.VBUSTI = v & 1;
	}), _write),

	read: (_read = {}, _defineProperty(_read, UEBCHX, function () {
		return this.EP[this.UENUM].UEBCHX;
	}), _defineProperty(_read, UEBCLX, function () {
		return this.EP[this.UENUM].UEBCLX;
	}), _defineProperty(_read, UEIENX, function () {
		return this.EP[this.UENUM].UEIENX;
	}), _defineProperty(_read, UESTA1X, function () {
		return this.EP[this.UENUM].UESTA1X;
	}), _defineProperty(_read, UESTA0X, function () {
		return this.EP[this.UENUM].UESTA0X;
	}), _defineProperty(_read, UECFG1X, function () {
		return this.EP[this.UENUM].UECFG1X;
	}), _defineProperty(_read, UECFG0X, function () {
		return this.EP[this.UENUM].UECFG0X;
	}), _defineProperty(_read, UECONX, function () {
		return this.EP[this.UENUM].UECONX;
	}), _defineProperty(_read, UERST, function () {
		return this.EP[this.UENUM].UERST;
	}), _defineProperty(_read, UENUM, function () {
		return this.EP[this.UENUM].UENUM;
	}), _defineProperty(_read, UEINTX, function () {
		var ep = this.EP[this.UENUM];
		var ret = ep.UEINTX;

		var rwal = (ep.buffer && ep.buffer.length > ep.length) | 0;

		this.UEINTX = ret = ret & ~(1 << 5) | rwal << 5;

		return ret;
	}), _defineProperty(_read, UEDATX, function () {
		var ep = this.EP[this.UENUM];
		return ep.read();
	}), _defineProperty(_read, UDINT, function () {
		return this.SUSPI | this.MSOFI << 1 | this.SOFI << 2 | this.EORSTI << 3 | this.WAKEUPI << 4 | this.EORSMI << 5 | this.UPRSMI << 6;
	}), _defineProperty(_read, USBINT, function () {
		return this.VBUSTI;
	}), _defineProperty(_read, USBCON, function () {
		return this.VBUSTE | this.OTGPADE << 4 | this.FRZCLK << 5 | this.USBE << 7;
	}), _read),

	init: function init() {
		var _this2 = this;

		var serial0INBuffer = this.serial0INBuffer = [];

		Object.defineProperty(this.core.pins, 'serial0In', {
			set: function set(v) {
				if (typeof v == 'string') {
					for (var i = 0, l = v.length; i < l; ++i) {
						serial0INBuffer.push(v.charCodeAt(i));
					}
				} else serial0INBuffer.push(v);
			}
		});

		this.timeout = 0;
		this.timeoutCB = null;
		this.DPRAM = new Uint8Array(832);
		this.EP = [];
		this.reset = function () {

			_this2.UENUM = 0;
			for (var i = 0; i < 8; ++i) {
				_this2.EP[i] = {
					buffer: null,
					UECONX: 0,
					UEIENX: 0,
					UEINTX: 0,
					UEDATX: 0,
					UEBCX: 0,
					UEBCLX: 0,
					UEBCHX: 0,
					length: 0,
					start: 0,
					end: 0,

					configure: function configure() {
						var size = 8 << (this.UECFG1X >> 4 & 7);
						var alloc = this.UECFG1X >> 1 & 1;
						if (alloc && (!this.buffer || size < this.buffer.length)) {
							this.buffer = new Uint8Array(size);
							this.start = this.end = 0;
						}
					},

					write: function write(v) {
						if (!this.buffer) return v;

						var nextEnd = (this.end + 1) % this.buffer.length;
						if (this.end == this.start && this.length) {
							// overflow
							return v;
						}

						this.buffer[this.end] = v;
						this.end = nextEnd;
						this.length++;
						this.UEBCLX = this.length & 0xFF;
						this.UEBCHX = this.length >> 8 & 0x7;

						this.UEDATX = v;
						return v;
					},

					read: function read() {
						if (!this.buffer) return 0;

						var nextStart = (this.start + 1) % this.buffer.length;
						if (!this.length) {
							// underflow
							return 0;
						}

						var v = this.buffer[this.start];
						this.start = nextStart;
						this.length--;
						this.UEBCLX = this.length & 0xFF;
						this.UEBCHX = this.length >> 8 & 0x7;

						this.UEDATX = v;

						return this.UEDATX;
					}
				};
			}

			_this2.VBUSTI = 0;
			_this2.VBUSTE = 0;
			_this2.OTGPADE = 0;
			_this2.FRZCLK = 1;
			_this2.USBE = 0;

			_this2.SUSPE = 0;
			_this2.MSOFE = 0;
			_this2.SOFE = 0;
			_this2.EORSTE = 0;
			_this2.WAKEUPE = 0;
			_this2.EORSME = 0;
			_this2.UPRSME = 0;

			_this2.SUSPI = 0;
			_this2.MSOFI = 0;
			_this2.SOFI = 0;
			_this2.EORSTI = 0;
			_this2.WAKEUPI = 0;
			_this2.EORSMI = 0;
			_this2.UPRSMI = 0;

			_this2.EPINT = 0;

			_this2.pluggedIn = false;
		};

		this.reset();
	},

	update: function update(tick, ie) {

		var ep0 = this.EP[0];
		while (ep0.buffer && this.serial0INBuffer.length && ep0.length < ep0.buffer.length) {
			ep0.write(this.serial0INBuffer.shift());
		}

		if (this.timeout && ! --this.timeout) this.timeoutCB();

		if (ie) {

			if (this.VBUSTI && this.VBUSTE) {
				this.VBUSTI = 0;
				return "USBGEN";
			}

			if (this.UPRSMI && this.UPRSME) {
				this.UPRSMI = 0;
				return "USBGEN";
			}

			if (this.EORSMI && this.EORSME) {
				this.EORSMI = 0;
				return "USBGEN";
			}

			if (this.WAKEUPI && this.WAKEUPE) {
				this.WAKEUPI = 0;
				return "USBGEN";
			}

			if (this.EORSTI && this.EORSTE) {
				this.EORSTI = 0;
				return "USBGEN";
			}

			if (this.SOFI && this.SOFE) {
				this.SOFI = 0;
				return "USBGEN";
			}

			if (this.SUSPI && this.SUSPE) {
				this.SUSPI = 0;
				return "USBGEN";
			}

			for (var i = 0; i < 8; ++i) {
				var ep = this.EP[i];
				var epi = ep.UEINTX & ep.UEIENX;
				if (epi) {
					this.UEINT = i;
					return "USBEND";
				}
			}
		}
	}
};

},{}],12:[function(require,module,exports){
'use strict';

function port(obj) {

	var out = { write: {}, read: {}, init: null };

	for (var k in obj) {

		var addr = obj[k];
		if (/DDR.|PORT./.test(k)) {

			out.write[addr] = setter(k);
		} else {

			out.read[addr] = getter(k);
			out.init = init(k);
		}
	}

	function setter(k) {
		return function (value, oldValue) {
			if (value != oldValue) this.core.pins[k] = value;
		};
	}

	function getter(k) {
		return function () {
			return this[k] & 0xFF | 0;
		};
	}

	function init(k) {
		return function () {
			this[k] = 0;
			var _this = this;
			Object.defineProperty(this.core.pins, k, {
				set: function set(v) {
					return _this[k] = v >>> 0 & 0xFF;
				},
				get: function get() {
					return _this[k];
				}
			});
		};
	}

	return out;
}

module.exports = {

	PORTB: port({ PINB: 0x23, DDRB: 0x24, PORTB: 0x25 }),
	PORTC: port({ PINC: 0x26, DDRC: 0x27, PORTC: 0x28 }),
	PORTD: port({ PIND: 0x29, DDRD: 0x2A, PORTD: 0x2B }),
	PORTE: port({ PINE: 0x2C, DDRE: 0x2D, PORTE: 0x2E }),
	PORTF: port({ PINF: 0x2F, DDRF: 0x30, PORTF: 0x31 }),

	TC0: require('./Timer8.js')({
		TIFR: 0x35,
		TCCR_A: 0x44,
		TCCR_B: 0x45,
		OCR_A: 0x47,
		OCR_B: 0x48,
		TIMSK: 0x6E,
		TCNT: 0x46,
		intOV: "TIMER0O"
	}),
	/* */
	TC1: require('./Timer16.js')({
		TIFR: 0x36,
		TCCR_A: 0x80,
		TCCR_B: 0x81,
		TCCR_C: 0x82,
		OCR_AH: 0x89,
		OCR_AL: 0x88,
		OCR_BH: 0x8B,
		OCR_BL: 0x8A,
		OCR_CH: 0x8D,
		OCR_CL: 0x8C,
		TIMSK: 0x6F,
		TCNTH: 0x85,
		TCNTL: 0x84,
		intOV: "TIMER1O",
		intCOMPA: "TIMER1A",
		intCOMPB: "TIMER1B",
		intCOMPC: "TIMER1C"
	}),
	/* */
	TC3: require('./Timer16.js')({
		TIFR: 0x38,
		TCCR_A: 0x90,
		TCCR_B: 0x91,
		TCCR_C: 0x92,
		TCNTH: 0x94,
		TCNTL: 0x95,
		OCR_AH: 0x99,
		OCR_AL: 0x98,
		OCR_BH: 0x9B,
		OCR_BL: 0x9A,
		OCR_CH: 0x9D,
		OCR_CL: 0x9C,
		TIMSK: 0x71,
		intOV: "TIMER3O",
		intCOMPA: "TIMER3A",
		intCOMPB: "TIMER3B",
		intCOMPC: "TIMER3C"
	}),

	// USART:require('./At328P-USART.js'),
	SERIAL: require('./At32u4-USBSerial.js'),

	PLL: {
		read: {
			0x49: function _(value) {
				return this.PINDIV << 4 | this.PLLE << 1 | this.PLOCK;
			}
		},
		write: {
			0x49: function _(value, oldValue) {
				if (value === oldValue) return;
				this.PINDIV = value >> 4 & 1;
				this.PLLE = value >> 1 & 1;
				this.PLOCK = 1;
			}
		},
		init: function init() {
			this.PINDIV = 0;
			this.PLLE = 0;
			this.PLOCK = 0;
		}
	},

	SPI: require('./At32u4-SPI.js'),

	EEPROM: {
		write: {
			0x3F: function _(value, oldValue) {
				var addr = this.core.memory[0x41] + (this.core.memory[0x42] << 8);

				if (value & 0x2) this.eeprom[addr] = this.core.memory[0x40];

				value &= ~2;
				return value;
			}
		},
		read: {
			0x40: function _() {
				var addr = this.core.memory[0x41] + (this.core.memory[0x42] << 8);
				return this.eeprom[addr] | 0;
			}
		},
		init: function init() {
			this.eeprom = new Uint8Array(1024);
			this.eeprom.fill(0xFF);
			if (typeof window !== "undefined") window.EEPROM = this.eeprom;
		}
	},

	ADCSRA: {

		write: {
			0x7A: function _(value, oldValue) {
				this.ADEN = value >> 7 & 1;
				this.ADSC = value >> 6 & 1;
				this.ADATE = value >> 5 & 1;
				this.ADIF = value >> 4 & 1;
				this.ADIE = value >> 3 & 1;
				this.ADPS2 = value >> 2 & 1;
				this.ADPS1 = value >> 1 & 1;
				this.ADPS0 = value & 1;
				if (this.ADEN) {
					if (this.ADSC) {
						this.ADCH = Math.random() * 0xFF >>> 0;
						this.ADCL = Math.random() * 0xFF >>> 0;
						this.ADSC = 0;
						value &= ~(1 << 6);
					}
				}
				return value;
			}
		},

		read: {
			0x79: function _() {
				return this.ADCH;
			},
			0x78: function _() {
				return this.ADCL;
			}
		},

		init: function init() {
			this.ADEN = 0;
			this.ADSC = 0;
			this.ADATE = 0;
			this.ADIF = 0;
			this.ADIE = 0;
			this.ADPS2 = 0;
			this.ADPS1 = 0;
			this.ADPS0 = 0;
		},

		update: function update(tick, ie) {
			if (this.ADEN && this.ADIE) {
				this.ADIF = 1;
				this.ADSC = 0;
				this.ADCH = Math.random() * 0xFF >>> 0;
				this.ADCL = Math.random() * 0xFF >>> 0;
			}

			if (this.ADIF && this.ADIE && ie) {
				this.ADIF = 0;
				return "ADC";
			}
		}

	}

};

},{"./At32u4-SPI.js":10,"./At32u4-USBSerial.js":11,"./Timer16.js":16,"./Timer8.js":17}],13:[function(require,module,exports){
(function (global){
"use strict";

// http://www.atmel.com/webdoc/avrassembler/avrassembler.wb_instruction_list.html

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function bin(bytes, size) {

				var s = (bytes >>> 0).toString(2);
				while (s.length < size) {
								s = "0" + s;
				}return s.replace(/([01]{4,4})/g, "$1 ") + "  #" + (bytes >>> 0).toString(16).toUpperCase();
}

if (typeof performance === "undefined") {
				if (Date.now) global.performance = { now: function now() {
												return Date.now();
								} };else global.performance = { now: function now() {
												return new Date().getTime();
								} };
}

var staticOp = { begin: "", end: "", srDirty: 0 };

var Atcore = function () {
				function Atcore(desc) {
								_classCallCheck(this, Atcore);

								if (!desc) return;

								this.sleeping = false;
								this.sreg = 0;
								this.pc = 0;
								this.sp = 0;
								this.clock = desc.clock;
								this.codec = desc.codec;
								this.interruptMap = desc.interrupt;
								this.error = 0;
								this.flags = desc.flags;
								this.tick = 0;
								this.startTick = 0;
								this.endTick = 0;
								this.execTime = 0;
								this.debuggerEnabled = false;
								this.time = performance.now();

								this.i8a = new Int8Array(4);

								this.breakpoints = {};
								this.readBreakpoints = {};
								this.writeBreakpoints = {};
								this.breakpointHit = false;
								this.minStack = 0xFFFFFFFF;
								this.lastReadValue = 0;
								this.lastReadAddr = 0;
								this.lastWriteValue = 0;
								this.lastWriteAddr = 0;
								this.history = window.execHistory = [];

								this.memory = new Uint8Array(32 // register file
								+ (0xFF - 0x1F) // io
								+ desc.sram);

								this.flash = new Uint8Array(desc.flash);
								this.eeprom = new Uint8Array(desc.eeprom);

								this.initMapping();
								this.instruction = null;
								this.periferals = {};
								this.pins = {};

								for (var periferalName in desc.periferals) {

												var addr = void 0,
												    periferal = desc.periferals[periferalName];
												var obj = this.periferals[periferalName] = { core: this };

												for (addr in periferal.write) {
																this.writeMap[addr] = periferal.write[addr].bind(obj);
												}for (addr in periferal.read) {
																this.readMap[addr] = periferal.read[addr].bind(obj);
												}if (periferal.update) this.updateList.push(periferal.update.bind(obj));

												if (periferal.init) periferal.init.call(obj);
								}

								this.initInterface();
				}

				_createClass(Atcore, [{
								key: "initInterface",
								value: function initInterface() {
												var _this = this;

												var THIS = this;

												self.core = {

																history: this.history,
																errors: [],
																breakpoints: this.breakpoints,
																readBreakpoints: this.readBreakpoints,
																writeBreakpoints: this.writeBreakpoints,
																enableDebugger: function enableDebugger() {
																				_this.enableDebugger();
																},
																memory: this.memory,

																get minStack() {
																				return THIS.minStack;
																},
																set minStack(v) {},

																da: function da() {
																				var a = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : null;
																				var l = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : 20;

																				var opc = _this.pc;

																				if (a === null) a = _this.pc;else a >>= 1;
																				_this.pc = a;

																				var out = [];
																				for (var i = 0; i < l; ++i) {
																								var op = (_this.pc << 1).toString(16).padStart(4, " ") + ": ";

																								var inst = _this.identify();
																								if (!inst) {
																												out.push(op + _this.error);
																												_this.pc++;
																												continue;
																								}

																								op += inst.name.toLowerCase();

																								var keys = Object.keys(inst.argv);
																								keys = keys.sort(function (a, b) {
																												a = a.toLowerCase();
																												b = b.toLowerCase();
																												if (a > b) return 1;
																												if (a < b) return -1;
																												return 0;
																								});

																								var sep = " ";
																								for (var ki = 0; ki < keys.length; ++ki) {
																												var k = keys[ki];
																												var v = void 0;
																												if (inst.print && inst.print[k]) v = inst.print[k](inst.argv[k], _this);else if (k == "r" || k == "d" || k == "x") v = "r" + inst.argv[k];else if (k == "A" || k == "k" || k == "K") v = "0x" + inst.argv[k].toString(16);else if (k == "b" || k == "s") v = inst.argv[k];else v = k + "=" + inst.argv[k].toString(16);

																												op += sep + v;
																												sep = ", ";
																								}

																								op += "\t\t; " + inst.decbytes.toString(2).padStart(8 * inst.bytes, "0").split(/(....)/).join(" ");

																								out.push(op);
																								_this.pc += inst.bytes >> 1;
																				}
																				_this.pc = opc;
																				return out.join("\n");
																},
																state: function state() {
																				return 'PC: #' + (_this.pc << 1).toString(16) + '\nSR: ' + _this.memory[0x5F].toString(2).padStart(8, '0') + '\nSP: #' + _this.sp.toString(16) + '\nStack: #' + (_this.sram.length + 0xFF - _this.minStack).toString(16) + '\n' + Array.prototype.map.call(_this.reg, function (v, i) {
																								return 'R' + (i + '') + ' ' + (i < 10 ? ' ' : '') + '=\t#' + v.toString(16).padStart(2, "0") + '\t' + v;
																				}).join('\n') + '\n' + Array.from(_this.wreg).map(function (v, i) {
																								return "WXYZ"[i] + '   =\t#' + v.toString(16).padStart(2, "0") + '\t' + v;
																				}).join('\n') + '\n' + '#' + _this.lastReadAddr.toString(16).padStart(4, "0") + " => #" + _this.lastReadValue.toString(16).padStart(2, "0") + '\n' + '#' + _this.lastWriteAddr.toString(16).padStart(4, "0") + " <= #" + _this.lastWriteValue.toString(16).padStart(2, "0") + '\n';
																}

												};
								}
				}, {
								key: "initMapping",
								value: function initMapping() {
												var _this2 = this;

												Object.defineProperties(this, {
																writeMap: { value: [], enumerable: false, writable: false },
																readMap: { value: [], enumerable: false, writable: false },
																updateList: { value: [], enumerable: false, writable: false },
																reg: { value: new Uint8Array(this.memory.buffer, 0, 0x20), enumerable: false },
																wreg: { value: new Uint16Array(this.memory.buffer, 0x20 - 8, 4), enumerable: false },
																sram: { value: new Uint8Array(this.memory.buffer, 0x100), enumerable: false },
																io: { value: new Uint8Array(this.memory.buffer, 0x20, 0xFF - 0x20), enumerable: false },
																prog: { value: new Uint16Array(this.flash.buffer), enumerable: false }
												});

												this.native = {};

												this.codec.forEach(function (op) {
																if (op.str) _this2.parse(op);
												});

												this.codec = this.codec.sort(function (a, b) {
																return b.score - a.score;
												});

												self.codecs = this.codec;

												var memory = this.memory;
												var readMap = this.readMap,
												    writeMap = this.writeMap;
												var THIS = this;

												this.read = function (addr) {

																var value = memory[addr];

																var periferal = readMap[addr];
																if (periferal) {
																				var ret = periferal(value);
																				if (ret !== undefined) value = ret;
																}

																if (THIS.debuggerEnabled) {

																				THIS.lastReadValue = value;
																				THIS.lastReadAddr = addr;

																				if (THIS.readBreakpoints[addr]) {
																								THIS.endTick = THIS.tick;
																								THIS.breakpointHit = true;
																				}
																}

																return value;
												};

												this.readBit = function (addr, bit) {
																debugger;

																var value = memory[addr];

																var periferal = readMap[addr];
																if (periferal) {
																				var ret = periferal(value);
																				if (ret !== undefined) value = ret;
																}

																THIS.lastReadValue = value;
																THIS.lastReadAddr = addr;

																if (THIS.debuggerEnabled && THIS.readBreakpoints[addr]) {
																				THIS.endTick = THIS.tick;
																				THIS.breakpointHit = true;
																}

																return value >>> bit & 1;
												};

												this.write = function (addr, value) {

																var periferal = writeMap[addr];

																if (periferal) {
																				var ret = periferal(value, memory[addr]);
																				if (ret === false) return;
																				if (ret !== undefined) value = ret;
																}

																if (THIS.debuggerEnabled) {

																				THIS.lastWriteValue = value;
																				THIS.lastWriteAddr = addr;

																				if (THIS.writeBreakpoints[addr]) {
																								THIS.endTick = THIS.tick;
																								THIS.breakpointHit = true;
																				}
																}

																return memory[addr] = value;
												};
								}
				}, {
								key: "readStatic",
								value: function readStatic(addr) {
												var periferal = this.readMap[addr];

												if (periferal) {
																return "(\nt2 = memory[" + addr + "],\nt1 = readMap[" + addr + "]( t2 ),\nt1 === undefined ? t2 : t1\n)";
												} else {
																return "memory[ " + addr + " ]";
												}
								}
				}, {
								key: "readRamStatic",
								value: function readRamStatic(addr) {
												return "memory[" + addr + "]";
								}
				}, {
								key: "writeStack",
								value: function writeStack(addr, value) {
												this.write(addr, value);
												if (addr < this.minStack) this.minStack = addr;
								}
				}, {
								key: "writeBit",
								value: function writeBit(addr, bit, bvalue) {
												bvalue = !!bvalue | 0;
												var value = this.memory[addr];
												value = value & ~(1 << bit) | bvalue << bit;

												var periferal = this.writeMap[addr];

												if (periferal) {
																var ret = periferal(value, this.memory[addr]);
																if (ret === false) return;
																if (ret !== undefined) value = ret;
												}

												this.lastWriteValue = value;
												this.lastWriteAddr = addr;

												this.memory[addr] = value;

												if (this.debuggerEnabled && this.writeBreakpoints[addr]) {
																this.endTick = this.tick;
																this.breakpointHit = true;
												}

												return this.memory[addr];
								}
				}, {
								key: "enableDebugger",
								value: function enableDebugger() {

												if (this.debuggerEnabled) return;

												this.debuggerEnabled = true;
												this.native = {};
								}
				}, {
								key: "exec",
								value: function exec(time) {

												var cycles = time * this.clock | 0;

												var start = this.tick;
												this.endTick = this.startTick + cycles;
												this.execTime = time;
												var lastUpdate = start;

												if (this.debuggerEnabled) {
																this.breakpointHit = false;

																while (this.tick < this.endTick) {

																				if (!this.breakpointHit && (this.tick >= this.endTick || this.tick - lastUpdate > 1000)) {
																								lastUpdate = this.tick;
																								this.updatePeriferals();
																				}

																				// while( this.history.length > 100 ) this.history.shift();
																				// this.history.push("#" + (this.pc<<1).toString(16));

																				if (!this.sleeping) {

																								if (this.pc > 0xFFFF) break;

																								var func = this.native[this.pc];
																								if (func) func.call(this);else if (!this.getBlock()) break;
																				} else {
																								this.tick += 1000;
																				}
																}
												} else {

																while (this.tick < this.endTick) {

																				if (!this.sleeping) {

																								var func = this.native[this.pc];
																								// if( !func ) 		    console.log( this.pc );
																								if (func) func.call(this);else if (!this.getBlock()) break;
																				} else {
																								this.tick += 100;
																				}

																				if (this.tick >= this.endTick || this.tick - lastUpdate > 200) {
																								lastUpdate = this.tick;
																								this.updatePeriferals();
																								while (this.history.length > 100) {
																												this.history.shift();
																								}
																				}
																}
												}

												this.startTick = this.endTick;
								}
				}, {
								key: "updatePeriferals",
								value: function updatePeriferals() {

												var interruptsEnabled = this.memory[0x5F] & 1 << 7;

												var updateList = this.updateList;

												for (var i = 0, l = updateList.length; i < l; ++i) {

																var ret = updateList[i](this.tick, interruptsEnabled);

																if (ret && interruptsEnabled) {
																				interruptsEnabled = 0;
																				this.sleeping = false;
																				this.interrupt(ret);
																}
												}
								}
				}, {
								key: "update",
								value: function update() {
												var now = performance.now();
												var delta = now - this.time;

												/* * /
            if( this.debuggerEnabled )
                delta = 16;
            else
                       delta = Math.max( 0, Math.min( 33, delta ) );
                /*/
												delta = 13.5;
												/* */

												this.exec(delta / 1000);

												this.time = now;
								}
				}, {
								key: "getBlock",
								value: function getBlock(execute) {
												var _this3 = this;

												if (execute == undefined) execute = true;

												if (this.pc >= 0xFFFF) {
																this.pc = 0;
																return false;
												}

												// this.history.push( "#" + (this.pc << 1).toString(16).padStart( 4, "0" ) );

												var startPC = this.pc;

												var skip = false,
												    prev = false,
												    dbg = this.debuggerEnabled;
												var cacheList = ['reg', 'wreg', 'io', 'memory', 'sram', 'flash'];

												if (dbg) cacheList.push("breakpoints");

												var code = '"use strict";\nvar sp=this.sp, r, t1, t2, i8a=this.i8a, SKIP=false, read=this.read, write=this.write, readMap=this.readMap, ';
												code += cacheList.map(function (c) {
																return c + " = this." + c;
												}).join(', ');
												code += ';\n';
												code += 'var sr = memory[0x5F]';
												for (var i = 0; i < 8; ++i) {
																code += ", sr" + i + " = (sr>>" + i + ")&1";
												}code += ';\n';

												code += 'switch( this.pc ){\n';
												// code += 'while(1){\n';

												var addrs = [];
												var doTickCheck = 1;
												do {

																var inst = this.identify();
																if (!inst) {
																				// inst = nop;
																				if (execute) this.history.push(this.error);
																				this.pc++;
																				return;
																}

																code += "\ncase " + this.pc + ":";

																var chunk = "\n\tthis.pc = " + this.pc + ";\n";

																if (dbg) {
																				chunk += '\nif( !breakpoints.disableFirst ){\n';
																				chunk += "\n\tif( breakpoints[" + this.pc + "] && breakpoints[" + this.pc + "](" + this.pc + ",this.sp) )";
																				chunk += '{\n\t\tthis.endTick = this.tick;\n';
																				chunk += '\t\tthis.breakpointHit = true;\n';
																				chunk += '\t\tbreak;\n\t}\n';
																}

																chunk += "\tif( (this.tick += " + inst.cycles + ") >= this.endTick ) break;\n";

																if (dbg) {
																				chunk += '\n}else{\n';
																				chunk += '\tthis.tick += ' + inst.cycles + ';\n';
																				chunk += '\tbreakpoints.disableFirst = false;\n';
																				chunk += '}\n';
																}

																var op = this.getOpcodeImpl(inst, inst.impl);

																var srDirty = op.srDirty;
																var line = op.begin,
																    endline = op.end;
																if (inst.flags) {
																				for (var i = 0, l = inst.flags.length; i < l; ++i) {
																								var flagOp = this.getOpcodeImpl(inst, this.flags[inst.flags[i]]);
																								line += flagOp.begin;
																								endline += flagOp.end;
																								srDirty |= flagOp.srDirty;
																				}
																}

																if (srDirty) {
																				var pres = (~srDirty >>> 0 & 0xFF).toString(2);
																				endline += "sr = (sr&0b" + pres + ") ";
																				for (var i = 0; i < 8; i++) {
																								if (srDirty & 1 << i) endline += " | (sr" + i + "<<" + i + ")";
																				}endline += ';\nmemory[0x5F] = sr;\n';
																}

																chunk += line + endline;

																if (skip) code += "  if( !SKIP ){\n    " + chunk + "\n  }\nSKIP = false;\n";else code += chunk;

																prev = skip;
																skip = inst.skip;

																this.pc += inst.bytes >> 1;
												} while (this.pc < this.prog.length && (!inst.end || skip || prev));

												code += "\nthis.pc = " + this.pc + ";\n";
												code += "break;\n";

												code += '\n\n}';
												code += 'this.sp = sp;\n';

												var endPC = this.pc;

												code = "return (function _" + (startPC << 1).toString(16) + "(){\n" + code + "});";

												try {

																var func = new Function(code)();

																for (var i = startPC; i < endPC; ++i) {
																				this.native[i] = func;
																}if (execute) {
																				this.pc = startPC;
																				func.call(this);
																}
												} catch (ex) {

																this.history.push("Error on 0x" + startPC + ": " + ex.toString());

																setTimeout(function () {
																				debugger;
																				var func = new Function(code);
																				func.call(_this3);
																}, 1);
																throw ex;
												}

												return endPC;
								}
				}, {
								key: "identify",
								value: function identify() {

												var prog = this.prog,
												    codec = this.codec,
												    bytes = void 0,
												    h = void 0,
												    j = void 0,
												    i = 0,
												    l = codec.length,
												    pc = this.pc;

												var bytes2 = void 0,
												    bytes4 = void 0;
												bytes2 = prog[pc] >>> 0;
												bytes4 = (bytes2 << 16 | prog[pc + 1]) >>> 0;

												for (; i < l; ++i) {

																var desc = codec[i];
																var opcode = desc.opcode >>> 0;
																var mask = desc.mask >>> 0;
																var size = desc.bytes;

																if (size === 4) {

																				if ((bytes4 & mask) >>> 0 !== opcode) continue;
																				bytes = bytes4;
																} else {

																				if ((bytes2 & mask) >>> 0 !== opcode) continue;
																				bytes = bytes2;
																}

																this.instruction = desc;

																// var log = desc.name + " ";

																for (var k in desc.args) {
																				mask = desc.args[k];
																				var value = 0;
																				h = 0;
																				j = 0;
																				while (mask) {
																								if (mask & 1) {
																												value |= (bytes >> h & 1) << j;
																												j++;
																								}
																								mask = mask >>> 1;
																								h++;
																				}
																				if (desc.shift && desc.shift[k]) value <<= desc.shift[k];
																				if (desc.add && desc.add[k]) value += desc.add[k];
																				if (desc.resolve && desc.resolve[k]) value = this.readStatic(value);
																				desc.argv[k] = value;
																				// log += k + ":" + value + "  "
																}
																desc.decbytes = bytes;
																// console.log(log);

																return this.instruction;
												}

												this.error = "UNKNOWN OPCODE: " + bin(bytes2, 16);

												return null;
								}
				}, {
								key: "interrupt",
								value: function interrupt(source) {

												// console.log("INTERRUPT " + source);

												var addr = this.interruptMap[source];
												var pc = this.pc;
												this.memory[this.sp--] = pc >> 8;
												this.memory[this.sp--] = pc;
												this.memory[0x5F] &= ~(1 << 7); // disable interrupts
												this.pc = addr;
												this.tick += 5;

												/*
            let log = "#" + (this.pc<<1).toString(16).padStart(4, "0") + " INT " + source;
            if( this.history[this.history.length-1] != log )
                this.history.push( log );
            */
								}
				}, {
								key: "parse",
								value: function parse(out) {
												var opcode = 0;
												var mask = 0;
												var args = {};
												var argv = {};
												var nargv = 0x00080000;

												var str = out.str,
												    l = str.length;
												for (var i = 0; i < l; ++i) {
																var chr = str[i];
																var bit = l - i - 1 >>> 0;
																if (chr == '0') {
																				mask |= 1 << bit;
																} else if (chr == '1') {
																				mask |= 1 << bit;
																				opcode |= 1 << bit;
																} else {
																				if (!(chr in args)) {
																								args[chr] = 0;
																								argv[chr] = nargv++;
																				}
																				args[chr] |= 1 << bit;
																}
												}

												out.opcode = opcode;
												out.mask = mask;
												out.args = args;
												out.argv = argv;
												out.bytes = l / 8 | 0;
												out.bytes = out.bytes || 2;
												out.cycles = out.cycles || 1;
												out.score = out.score || 0;

												var op = this.getOpcodeImpl(out, out.impl);

												out.parsedBegin = getParsed(op.begin);
												out.parsedEnd = getParsed(op.end);
												out.parsedsrDirty = op.srDirty | 0;

												if (out.flags) {

																for (var i = 0, l = out.flags.length; i < l; ++i) {
																				var flagOp = this.getOpcodeImpl(out, this.flags[out.flags[i]]);

																				var tmp = getParsed(flagOp.begin);
																				out.parsedBegin[out.parsedBegin.length - 1] += tmp[0];
																				for (var j = 1; j < tmp.length; ++j) {
																								out.parsedBegin.push(tmp[j]);
																				}tmp = getParsed(flagOp.end);
																				out.parsedEnd[out.parsedEnd.length - 1] += tmp[0];
																				for (j = 1; j < tmp.length; ++j) {
																								out.parsedEnd.push(tmp[j]);
																				}out.parsedsrDirty |= flagOp.srDirty;
																}

																out.flags = null;
												}

												function getParsed(impl) {
																if (!impl) return [""];

																var parsed = [impl];

																for (var k in args) {

																				var klc = k.toLowerCase();
																				var val = argv[k];

																				for (var i = parsed.length - 1; i >= 0; i -= 2) {
																								var parts = parsed[i].split(val);
																								var oparts = [i, 1];
																								parts.forEach(function (p) {
																												return oparts.push(p, k);
																								});
																								oparts.pop();
																								parsed.splice.apply(parsed, oparts);
																				}
																}

																return parsed;
												}
								}
				}, {
								key: "getOpcodeImpl",
								value: function getOpcodeImpl(inst, str) {
												var useParsed = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : true;

												var i,
												    l,
												    op = staticOp;

												if (inst.parsedBegin && str === inst.impl && useParsed) {
																var getParsed = function getParsed(arr) {

																				var ret = "",
																				    i = 0;
																				if (arr.length > 1) for (i; i < arr.length - 1;) {
																								ret += arr[i++];
																								ret += inst.argv[arr[i++]];
																				}

																				ret += arr[i++];

																				return ret;
																};

																op.begin = getParsed(inst.parsedBegin);
																op.end = getParsed(inst.parsedEnd);
																op.srDirty = inst.parsedsrDirty;

																return op;
												}

												op = { begin: "", end: "", srDirty: 0 };

												if (Array.isArray(str)) {
																for (i = 0, l = str.length; i < l; ++i) {
																				var tmp = this.getOpcodeImpl(inst, str[i]);
																				op.begin += tmp.begin + "\n";
																				op.end += tmp.end + "\n";
																				op.srDirty |= tmp.srDirty;
																}
																return op;
												}

												var src = str,
												    argv = inst.argv;

												for (var k in argv) {
																str = str.split(k.toLowerCase()).join(argv[k]);
												}var SRSync = "",
												    SRDirty = 0;

												str = str.replace(/SR@([0-9]+)\s*←\s*1;?\s*$/g, function (m, bit, assign) {
																SRDirty |= 1 << bit;
																return "sr" + bit + " = 1;\n";
												});
												str = str.replace(/SR@([0-9]+)\s*←\s*0;?\s*$/g, function (m, bit, assign) {
																SRDirty |= 1 << bit;
																return "sr" + bit + " = 0;\n";
												});
												str = str.replace(/SR([0-9]+)\s*=(.*)/g, function (m, bit, assign) {
																SRDirty |= 1 << bit;
																return "sr" + bit + " = " + assign + ";\n";
												});
												str = str.replace(/SR\s*←/g, function () {
																SRSync = 'memory[0x5F] = sr; sr0=sr&1; sr1=(sr>>1)&1; sr2=(sr>>2)&1; sr3=(sr>>3)&1; sr4=(sr>>4)&1; sr5=(sr>>5)&1; sr6=(sr>>6)&1; sr7=(sr>>7)&1;';
																return 'sr =';
												});
												str = str.replace(/SR@([0-9]+)\s*←(.*)$/g, function (m, bit, assign) {
																SRDirty |= 1 << bit;
																return "sr" + bit + " = (!!(" + assign + "))|0;";
												});
												str = str.replace(/SR\s*¯/g, '(~sr)');
												str = str.replace(/SR@([0-9]+)\s*¯/g, '(~sr$1) ');
												str = str.replace(/SR@([0-9]+)\s*/g, '(sr$1) ');
												str = str.replace(/SR/g, 'sr');

												str = str.replace(/WR([0-9]+)\s*←/g, function (m, num) {
																op.end += "wreg[" + num + "] = r;\n";
																return 'r = ';
												});
												str = str.replace(/WR([0-9]+)@([0-9]+)\s*←(.*)$/g, function (m, num, bit, assign) {
																op.end += "wreg[" + num + "] = r;\n";
																return "r = (wreg[" + num + "] & ~(1<<" + bit + ")) | (((!!(" + assign + "))|0)<<" + bit + ");";
												});
												str = str.replace(/WR([0-9]+)\s*¯/g, '(~wreg[$1]) ');
												str = str.replace(/WR([0-9]+)@([0-9]+)\s*¯/g, '(~(wreg[$1]>>>$2)&1) ');
												str = str.replace(/WR([0-9]+)@([0-9]+)\s*/g, '((wreg[$1]>>>$2)&1) ');
												str = str.replace(/WR([0-9]+)/g, 'wreg[$1]');

												str = str.replace(/R([0-9<]+)(\+[0-9]+)?\s*←/g, function (m, num, numadd) {
																numadd = numadd || "";
																op.end += "reg[(" + num + ")" + numadd + "] = r;\n";
																return 'r = ';
												});
												str = str.replace(/R([0-9<]+)(\+[0-9]+)?@([0-9]+)\s*←(.*)$/g, function (m, num, numadd, bit, assign) {
																numadd = numadd || "";
																op.end += "reg[(" + num + ")" + numadd + "] = r;\n";
																return "r = (reg[(" + num + ")" + numadd + "] & ~(1<<" + bit + ")) | (((!!(" + assign + "))|0)<<" + bit + ");";
												});

												str = str.replace(/R([0-9<]+)(\+[0-9]+)?\s*=\s+/g, function (m, num, numadd) {
																numadd = numadd || "";
																return "r = reg[(" + num + ")" + numadd + "] = ";
												});
												str = str.replace(/R([0-9<]+)(\+[0-9]+)?@([0-9]+)\s*=\s+(.*)$/g, function (m, num, numadd, bit, assign) {
																numadd = numadd || "";
																return "r = reg[(" + num + ")" + numadd + "] = (reg[(" + num + ")" + numadd + "] & ~(1<<" + bit + ")) | (((!!(" + assign + "))|0)<<" + bit + ");";
												});

												str = str.replace(/R([0-9<]+)(\+[0-9]+)?\s*¯/g, '(~reg[($1)$2]) ');
												str = str.replace(/R([0-9<]+)(\+[0-9]+)?@([0-9]+)\s*¯/g, '(~(reg[($1)$2]>>>$3)&1) ');
												str = str.replace(/R([0-9<]+)(\+[0-9]+)?@([0-9]+)\s*/g, '((reg[($1)$2]>>>$3)&1) ');
												str = str.replace(/R([0-9<]+)(\+[0-9]+)?/g, '(reg[($1)$2]>>>0)');

												str = str.replace(/R@([0-9]+)\s*¯/g, '(~(r>>>$1)&1) ');
												str = str.replace(/R@([0-9]+)\s*/g, '((r>>>$1)&1) ');
												str = str.replace(/I\/O/g, 'io');
												str = str.replace(/R/g, 'r');

												str = str.replace(/FLASH\(([XYZ])\)\s*←(.*);?$/g, function (m, n, v) {
																return 'flash[ wreg[' + (n.charCodeAt(0) - 87) + '] ] = ' + v + ';';
												});
												str = str.replace(/FLASH\(([XYZ])\)/g, function (m, n) {
																return 'flash[ wreg[' + (n.charCodeAt(0) - 87) + '] ]';
												});
												str = str.replace(/\(([XYZ])(\+[0-9]+)?\)\s*←(.*);?$/g, function (m, n, off, v) {
																return 'write( wreg[' + (n.charCodeAt(0) - 87) + ']' + (off || '') + ', ' + v + ');';
												});
												str = str.replace(/\(([XYZ])(\+[0-9]+)?\)/g, function (m, n, off) {
																return 'read( wreg[' + (n.charCodeAt(0) - 87) + ']' + (off || '') + ', this.pc )';
												});

												if (!this.debuggerEnabled) {
																str = str.replace(/\(STACK\)\s*←(.*)/g, 'memory[sp--] = $1; if(sp<this.minStack) this.minStack = sp; ');
																str = str.replace(/\((STACK)\)/g, 'memory[++sp]');
																str = str.replace(/\(STACK2\)\s*←(.*)/g, 't1 = $1;\nmemory[sp--] = t1>>8;\nmemory[sp--] = t1;\n if(sp<this.minStack) this.minStack = sp;\n');
																str = str.replace(/\((STACK2)\)/g, '(memory[++sp] + (memory[++sp]<<8))');
												} else {
																str = str.replace(/\(STACK\)\s*←(.*)/g, 'this.writeStack(sp--, $1)');
																str = str.replace(/\((STACK)\)/g, 'read(++sp)');
																str = str.replace(/\(STACK2\)\s*←(.*)/g, 't1 = $1;\nthis.writeStack(sp--, t1>>8);\nthis.writeStack(sp--, t1);\n');
																str = str.replace(/\((STACK2)\)/g, '(read(++sp) + (read(++sp)<<8))');
												}

												str = str.replace(/⊕/g, '^');
												str = str.replace(/•/g, '&');

												str = str.replace(/io\[([0-9]+)\]\s*←(.*?);?$/g, 'write( 32+$1, $2 )');
												str = str.replace(/io\[([0-9]+)@([0-9]+)\]\s*←(.*?);?$/g, 'this.writeBit( 32+$1, $2, $3 )');
												str = str.replace(/io\[([0-9+<]+)@([0-9]+)\]/g, '((read( 32+$1 )>>>$2)&1)');
												str = str.replace(/io\[([0-9+<]+)\]/g, 'read( 32+$1 )');

												str = str.replace(/SP/g, 'sp');
												str = str.replace(/PC\s*←(.*)$/g, 't1 = $1;\n this.pc = t1; break;\n');
												str = str.replace(/PC/g, 'this.pc');
												str = str.replace(/←/g, '=');

												str = '// ' + src.replace(/[\n\r]+\s*/g, '\n\t// ') + "\n" + str + "\n";

												op.srDirty = SRDirty;

												op.begin = str;
												op.end += SRSync;

												return op;
								}
				}, {
								key: "statusI",
								get: function get() {
												return this.sreg & 1 << 7;
								}
				}, {
								key: "statusT",
								get: function get() {
												return this.sreg & 1 << 6;
								}
				}, {
								key: "statusH",
								get: function get() {
												return this.sreg & 1 << 5;
								}
				}, {
								key: "statusS",
								get: function get() {
												return this.sreg & 1 << 4;
								}
				}, {
								key: "statusV",
								get: function get() {
												return this.sreg & 1 << 3;
								}
				}, {
								key: "statusN",
								get: function get() {
												return this.sreg & 1 << 2;
								}
				}, {
								key: "statusZ",
								get: function get() {
												return this.sreg & 1 << 1;
								}
				}, {
								key: "statusC",
								get: function get() {
												return this.sreg & 1 << 0;
								}
				}], [{
								key: "ATmega328P",
								value: function ATmega328P() {

												var core = new Atcore({
																flash: 32 * 1024,
																eeprom: 1 * 1024,
																sram: 2 * 1024,
																codec: AtCODEC,
																flags: AtFlags,
																clock: 16 * 1000 * 1000, // speed in kHz
																periferals: require('./At328P-periferals.js'),
																interrupt: {
																				RESET: 0x0000, //  External pin, power-on reset, brown-out reset and watchdog system reset
																				INT0: 0x002, //  External interrupt request 0
																				INT1: 0x0004, //  External interrupt request 1
																				PCINT0: 0x0006, //  Pin change interrupt request 0
																				PCINT1: 0x0008, //  Pin change interrupt request 1
																				PCINT2: 0x000A, //  Pin change interrupt request 2
																				WDT: 0x000C, //  Watchdog time-out interrupt
																				TIMER2A: 0x000E, //  COMPA Timer/Counter2 compare match A
																				TIMER2B: 0x0010, //  COMPB Timer/Counter2 compare match B
																				TIMER2O: 0x0012, //  OVF Timer/Counter2 overflow
																				TIMER1C: 0x0014, //  CAPT Timer/Counter1 capture event
																				TIMER1A: 0x0016, //  COMPA Timer/Counter1 compare match A
																				TIMER1B: 0x0018, //  COMPB Timer/Counter1 compare match B
																				TIMER1O: 0x001A, //  OVF Timer/Counter1 overflow
																				TIMER0A: 0x001C, //  COMPA Timer/Counter0 compare match A
																				TIMER0B: 0x001E, //  COMPB Timer/Counter0 compare match B
																				TIMER0O: 0x0020, //  OVF Timer/Counter0 overflow
																				SPI: 0x0022, // , STC SPI serial transfer complete
																				USARTRX: 0x0024, // , RX USART Rx complete
																				USARTE: 0x0026, // , UDRE USART, data register empty
																				USARTTX: 0x0028, // , TX USART, Tx complete
																				ADC: 0x002A, //  ADC conversion complete
																				EEREADY: 0x002C, //  READY EEPROM ready
																				ANALOG: 0x002E, //  COMP Analog comparator
																				TWI: 0x0030, //  2-wire serial interface
																				SPM: 0x0032 //  READY Store program memory ready
																}
												});

												return core;
								}
				}, {
								key: "ATmega32u4",
								value: function ATmega32u4() {

												var core = new Atcore({
																flash: 32 * 1024,
																eeprom: 1 * 1024,
																sram: 2 * 1024 + 512,
																codec: AtCODEC,
																flags: AtFlags,
																clock: 16 * 1000 * 1000, // speed in kHz
																periferals: require('./At32u4-periferals.js'),
																interrupt: {
																				RESET: 0x0000, //  External pin, power-on reset, brown-out reset and watchdog system reset
																				INT0: 0x002, //  External interrupt request 0
																				INT1: 0x0004, //  External interrupt request 1
																				INT2: 0x0006, //  External interrupt request 2
																				INT3: 0x0008, //  External interrupt request 3
																				RESERVED0: 0x000A,
																				RESERVED1: 0x000C,
																				INT6: 0x000E, //  External interrupt request 6
																				PCINT0: 0x0012, //  Pin change interrupt request 0
																				USBGEN: 0x0014, // USB General Interrupt request
																				USBEND: 0x0016, // USB Endpoint Interrupt request
																				WDT: 0x0018, //  Watchdog time-out interrupt

																				TIMER1CAP: 0x0020, //  CAPT Timer/Counter1 capture event
																				TIMER1A: 0x0022, //  COMPA Timer/Counter1 compare match A
																				TIMER1B: 0x0024, //  COMPB Timer/Counter1 compare match B
																				TIMER1C: 0x0026, //  COMPC Timer/Counter1 compare match C
																				TIMER1O: 0x0028, //  OVF Timer/Counter1 overflow
																				TIMER0A: 0x002A, //  COMPA Timer/Counter0 compare match A
																				TIMER0B: 0x002C, //  COMPB Timer/Counter0 compare match B
																				TIMER0O: 0x002E, //  OVF Timer/Counter0 overflow

																				SPI: 0x0030, // , STC SPI serial transfer complete

																				USARTRX: 0x0032, // , RX USART Rx complete
																				USARTE: 0x0034, // , UDRE USART, data register empty
																				USARTTX: 0x0036, // , TX USART, Tx complete

																				ANALOG: 0x0038, // Analog Comparator
																				ADC: 0x003A, //  ADC conversion complete

																				EEREADY: 0x003C, //  EEPROM ready

																				TIMER3CAP: 0x003E, //  CAPT Timer/Counter1 capture event
																				TIMER3A: 0x0040, //  COMPA Timer/Counter1 compare match A
																				TIMER3B: 0x0042, //  COMPB Timer/Counter1 compare match B
																				TIMER3C: 0x0044, //  COMPC Timer/Counter1 compare match C
																				TIMER3O: 0x0046, //  OVF Timer/Counter1 overflow


																				TWI: 0x0048, //  2-wire serial interface

																				SPM: 0x004A, //  READY Store program memory ready

																				TIMER4A: 0x004C,
																				TIMER4B: 0x004E,
																				TIMER4D: 0x0050,
																				TIMER4O: 0x0052,
																				TIMER4FPF: 0x0054
																}
												});

												return core;
								}
				}]);

				return Atcore;
}();

var AtCODEC = require('./Opcodes');

var AtFlags = {

				h: 'SR@5 ← (Rd@3 • Rr@3) + (Rr@3 • R@3 ¯) | (R@3 ¯ • Rd@3)',
				H: '',
				z: 'SR1 = !(R&0xFF)|0',
				Z: 'SR1 = !(R&0xFFFF)|0',
				v: 'SR3 = (Rd@7 • Rr@7 • R@7 ¯) | (Rd@7 ¯ • Rr@7 ¯ • R@7)',
				V: 'SR3 = WRd@15 ¯ • R@15',
				n: 'SR2 = R@7',
				N: 'SR2 = R@15',
				s: 'SR4 = SR@2 ⊕ SR@3',
				S: 'SR4 = SR@2 ⊕ SR@3',
				c: 'SR0 = (Rd@7 • Rr@7) | (Rr@7 • R@7 ¯) | (R@7 ¯ • Rd@7)',
				C: 'SR0 = (R@15 ¯ • WRd@15)'

};

module.exports = Atcore;

}).call(this,typeof global !== "undefined" ? global : typeof self !== "undefined" ? self : typeof window !== "undefined" ? window : {})

},{"./At328P-periferals.js":9,"./At32u4-periferals.js":12,"./Opcodes":15}],14:[function(require,module,exports){
'use strict';

var Hex = {
    parseURL: function parseURL(url, buffer, cb) {

        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = function () {

            if (xhr.readyState === 4) {

                if (xhr.status == 200 || xhr.status == 0) {

                    try {
                        Hex.parse(xhr.responseText, buffer);
                    } catch (ex) {
                        cb(false);
                        return;
                    }
                    cb(true, xhr.responseText);
                } else cb(false);
            }
        };

        xhr.open("GET", url, true);
        xhr.send();
    },
    parse: function parse(src, buffer) {
        var max = 0;
        var state = 0,
            size = 0,
            num = void 0,
            byte = void 0,
            offset = void 0,
            sum = 0;

        for (var i = 0, l = src.length; i < l;) {

            byte = src.charCodeAt(i++);

            if (byte === 58) {
                state = 0;
                continue;
            }

            if (byte >= 65 && byte <= 70) {
                num = byte - 55 << 4;
            } else if (byte >= 48 && byte <= 57) {
                num = byte - 48 << 4;
            } else continue;

            while (i < l) {
                byte = src.charCodeAt(i++);
                if (byte >= 65 && byte <= 70) {
                    num += byte - 55;
                    break;
                } else if (byte >= 48 && byte <= 57) {
                    num += byte - 48;
                    break;
                } else continue;
            }

            switch (state) {
                case 0:
                    size = num;
                    state++;
                    sum = num;
                    break;

                case 1:
                    offset = num << 8;
                    state++;
                    sum += num;
                    break;

                case 2:
                    offset += num;
                    state++;
                    sum += num;
                    break;

                case 3:
                    if (num === 1) return max + 1;
                    if (num === 3 || num === 5) {
                        state++;
                    } else if (num !== 0) throw 'Unsupported record type: ' + num;
                    state++;
                    sum += num;
                    break;

                case 4:
                    if (offset > max) max = offset;
                    buffer[offset++] = num;
                case 5:
                    sum += num;
                    if (! --size) state = 6;
                    break;

                case 6:
                    sum += num;
                    sum = -sum & 0xFF;
                    if (!sum) state++;else throw 'Checksum mismatch: ' + sum;
                    break;

                case 7:
                default:
                    throw 'Illegal state ' + state;
            }
        }

        return max + 1;
    }
};

module.exports = Hex;

},{}],15:[function(require,module,exports){
'use strict';

module.exports = [{
    name: 'ADC',
    score: 141,
    str: '000111rdddddrrrr',
    impl: 'Rd ← Rd + Rr + SR@0;',
    flags: 'hzvnsc'
}, {
    name: 'ADD',
    score: 158,
    str: '000011rdddddrrrr',
    impl: 'Rd ← Rd + Rr;',
    flags: 'hzvnsc'
}, {
    name: 'MUL',
    str: '100111rdddddrrrr',
    impl: ['t1 = Rd * Rr', 'R0 = t1', 'R1 = t1 >> 8', 'SR1 = !t1|0', 'SR0 = (t1>>15)&1'],
    flags: 'hvnsc'
}, {
    name: 'ADIW',
    str: '10010110KKddKKKK',
    impl: ['WRd ← WRd + k;'],
    print: {
        d: function d(_d) {
            return "WXYZ"[_d];
        }
    },
    debug: true,
    flags: 'ZVNSC'
}, {
    name: 'AND',
    str: '001000rdddddrrrr',
    impl: ['Rd ← Rd • Rr;', 'SR@3 ← 0'],
    flags: 'zns'
}, {
    name: 'ANDI',
    str: '0111KKKKddddKKKK',
    impl: ['Rd ← Rd • k;', 'SR@3 ← 0'],
    add: { d: 16 },
    flags: 'zns'
}, {
    name: 'ASR',
    str: '1001010ddddd0101',
    impl: ['SR@0 ← Rd • 1', 'Rd ← Rd << 24 >> 25;'],
    flags: 'zns'
}, {
    name: 'BCLRi',
    str: '1001010011111000',
    impl: 'SR@7 ← 0'
}, {
    name: 'BCLRt',
    str: '1001010011101000',
    impl: 'SR@6 ← 0'
}, {
    name: 'BCLRh',
    str: '1001010011011000',
    impl: 'SR@5 ← 0'
}, {
    name: 'BCLRs',
    str: '1001010011001000',
    impl: 'SR@4 ← 0'
}, {
    name: 'BCLRv',
    str: '1001010010111000',
    impl: 'SR@3 ← 0'
}, {
    name: 'BCLRn',
    str: '1001010010101000',
    impl: 'SR@2 ← 0'
}, {
    name: 'BCLRz',
    str: '1001010010011000',
    impl: 'SR@1 ← 0'
}, {
    name: 'BCLRc',
    str: '1001010010001000',
    impl: 'SR@0 ← 0'
}, {
    name: 'BRCC',
    str: '111101kkkkkkk000',
    impl: ['if( !SR@0 ){', '  PC ← PC + (k << 25 >> 25) + 1;', '}'],
    cycles: 2,
    sex: { k: 25 },
    print: {
        k: function k(_k, core) {
            return "#" + (core.pc + (_k << 25 >> 25) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'BRBS',
    score: 105,
    str: '111100kkkkkkksss',
    impl: ['if( SR@s ){', '  PC ← PC + (k << 25 >> 25) + 1;', '}'],
    cycles: 2,
    print: {
        k: function k(_k2, core) {
            return "#" + (core.pc + (_k2 << 25 >> 25) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'BRCS',
    str: '111100kkkkkkk000',
    impl: ['if( SR@0 ){', '  PC ← PC + (k << 25 >> 25) + 1;', '}'],
    cycles: 2,
    print: {
        k: function k(_k3, core) {
            return "#" + (core.pc + (_k3 << 25 >> 25) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'BREQ',
    str: '111100kkkkkkk001',
    impl: ['if( SR@1 ){', '  PC ← PC + (k << 25 >> 25) + 1;', '}'],
    cycles: 3,
    print: {
        k: function k(_k4, core) {
            return "#" + (core.pc + (_k4 << 25 >> 25) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'BRLT',
    str: '111100kkkkkkk100',
    impl: ['if( SR@4 ){', '  PC ← PC + (k << 25 >> 25) + 1;', '}'],
    cycles: 3,
    print: {
        k: function k(_k5, core) {
            return "#" + (core.pc + (_k5 << 25 >> 25) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'BRGE',
    str: '111101kkkkkkk100',
    impl: ['if( !SR@4 ){', '  PC ← PC + (k << 25 >> 25) + 1;', '}'],
    cycles: 3,
    print: {
        k: function k(_k6, core) {
            return "#" + (core.pc + (_k6 << 25 >> 25) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'BRNE',
    score: 91,
    str: '111101kkkkkkk001',
    impl: ['if( !SR@1 ){', '  PC ← PC + (k << 25 >> 25) + 1;', '}'],
    cycles: 3,
    print: {
        k: function k(_k7, core) {
            return "#" + (core.pc + (_k7 << 25 >> 25) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'BRPL',
    str: '111101kkkkkkk010',
    impl: ['if( !SR@2 ){', '  PC ← PC + (k << 25 >> 25) + 1;', '}'],
    cycles: 2,
    print: {
        k: function k(_k8, core) {
            return "#" + (core.pc + (_k8 << 25 >> 25) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'BRMI',
    str: '111100kkkkkkk010',
    impl: ['if( SR@2 ){', '  PC ← PC + (k << 25 >> 25) + 1;', '}'],
    cycles: 2,
    print: {
        k: function k(_k9, core) {
            return "#" + (core.pc + (_k9 << 25 >> 25) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'BRTC',
    str: '111101kkkkkkk110',
    impl: ['if( !SR@6 ){', '  PC ← PC + (k << 25 >> 25) + 1;', '}'],
    cycles: 3,
    print: {
        k: function k(_k10, core) {
            return "#" + (core.pc + (_k10 << 25 >> 25) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'BRBC',
    str: '111101kkkkkkksss',
    impl: ['if( !SR@s ){', '  PC ← PC + (k << 25 >> 25) + 1;', '}'],
    cycles: 2,
    print: {
        k: function k(_k11, core) {
            return "#" + (core.pc + (_k11 << 25 >> 25) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'BST',
    str: '1111101ddddd0bbb',
    impl: 'SR6 = Rd@b'
    //,debug: true
}, {
    name: 'BLD',
    str: '1111100ddddd0bbb',
    impl: 'Rd@b ← SR@6'
}, {
    name: 'CALL',
    score: 143,
    str: '1001010kkkkk111kkkkkkkkkkkkkkkkk',
    cycles: 4,
    impl: ['(STACK2) ← PC + 2', 'PC ← k'],
    print: {
        k: function k(_k12, core) {
            return "#" + (_k12 << 1).toString(16);
        }
    }
}, {
    name: 'CBI',
    str: '10011000AAAAAbbb',
    impl: 'I/O[a@b] ← 0;'
}, {
    name: 'COM',
    str: '1001010ddddd0000',
    impl: ['Rd ← ~ Rd;', 'SR@3 ← 0', 'SR@0 ← 1'],
    flags: 'zns'
}, {
    name: 'FMUL',
    str: '000000110ddd1rrr',
    impl: ['t1 = Rd * Rr << 1', 'R0 = t1', 'R1 = t1 >> 8', 'SR1 = !t1|0', 'SR0 = (t1>>15)&1'],
    add: { d: 16, r: 16 }
}, {
    name: 'NOP',
    str: '0000000000000000',
    impl: ''
}, {
    name: 'NEG',
    str: '1001010ddddd0001',
    impl: ['Rd ← - Rd;', 'SR3 = R@7 • R@6 ¯ • R@5 ¯ • R@4 ¯ • R@3 ¯ • R@2 ¯ • R@1 ¯ • R@0 ¯', 'SR0 = (!!R)|0', 'SR@5 ← R@3 | Rd@3 ¯'],
    flags: 'zns'
}, {
    name: 'CP',
    str: '000101rdddddrrrr',
    impl: ['R = ((Rd - Rr) >>> 0) & 0xFF;', 'SR@5 ← (Rd@3 ¯ • Rr@3) | (Rr@3 • R@3) | (R@3 • Rd@3 ¯)', 'SR@0 ← (Rd@7 ¯ • Rr@7) | (Rr@7 • R@7) | (R@7 • Rd@7 ¯)', 'SR@3 ← (Rd@7 • Rr@7 ¯ • R@7 ¯) + (Rd@7 ¯ • Rr@7 • R@7)'],
    flags: 'zns'
}, {
    name: 'CPI',
    score: 83,
    str: '0011KKKKddddKKKK',
    impl: ['R = ((Rd - k) >>> 0) & 0xFF;', 'SR@5 ← (Rd@3 ¯ • ((k>>3)&1)) | (((k>>3)&1) • R@3) | (R@3 • Rd@3 ¯)', 'SR@0 ← (Rd@7 ¯ • ((k>>7)&1)) | (((k>>7)&1) • R@7) | (R@7 • Rd@7 ¯)', 'SR@3 ← (Rd@7 • ((k>>7)&1^1) • R@7 ¯) + (Rd@7 ¯ • ((k>>7)&1) • R@7)'],
    flags: 'zns',
    add: { d: 16 }
}, {
    name: 'CPC',
    score: 107,
    str: '000001rdddddrrrr',
    impl: ['R = (Rd - Rr - SR@0) & 0xFF', 'SR@5 ← (Rd@3 ¯ • Rr@3) | (Rr@3 • R@3) | (R@3 • Rd@3 ¯)', 'SR@0 ← (Rd@7 ¯ • Rr@7) | (Rr@7 • R@7) | (R@7 • Rd@7 ¯)', 'SR@3 ← (Rd@7 • Rr@7 ¯ • R@7 ¯) | (Rd@7 ¯ • Rr@7 • R@7)', 'SR@1 ← (!R) & SR@1'],
    flags: 'ns'
}, {
    name: 'CPSE',
    str: '000100rdddddrrrr',
    impl: 'SKIP ← Rr == Rd',
    skip: true
}, {
    name: 'DEC',
    str: '1001010ddddd1010',
    impl: ['Rd ← Rd - 1', 'SR@3 ← R@7 ¯ • R@6 • R@5 • R@4 • R@3 • R@2 • R@1 • R@0'],
    flags: 'zns'
}, {
    name: 'EOR',
    str: '001001rdddddrrrr',
    impl: ['Rd ← Rd ⊕ Rr;', 'SR@3 ← 0'],
    flags: 'zns'
}, {
    name: 'ICALL',
    str: '1001010100001001',
    cycles: 3,
    impl: ['(STACK2) ← PC + 1', 'PC ← WR3']
    // end:true
}, {
    name: 'INSR',
    str: '1011011ddddd1111',
    impl: 'Rd \u2190 SR',
    cycles: 1
    // debug: true
}, {
    name: 'IN',
    str: '1011011ddddd1110',
    impl: 'Rd \u2190 sp>>>8',
    cycles: 1
}, {
    name: 'IN',
    str: '1011011ddddd1101',
    impl: 'Rd \u2190 sp&0xFF',
    cycles: 1
}, {
    name: 'IN',
    str: '10110AAdddddAAAA',
    impl: 'Rd \u2190 a',
    add: { A: 32 },
    resolve: { A: true },
    cycles: 1
}, {
    name: 'INC',
    str: '1001010ddddd0011',
    impl: ['Rd ← Rd + 1;', 'SR@3 ← R@7 • R@6 ¯ • R@5 ¯ • R@4 ¯ • R@3 ¯ • R@2 ¯ • R@1 ¯ • R@0 ¯'],
    flags: 'zns'
}, {
    name: 'IJMP',
    str: '1001010000001001',
    impl: 'PC \u2190 WR3',
    cycles: 2,
    end: true
}, {
    name: 'JMP',
    str: '1001010kkkkk110kkkkkkkkkkkkkkkkk',
    impl: 'PC \u2190 k',
    cycles: 3,
    end: true,
    print: {
        k: function k(_k13, core) {
            return "0x" + (_k13 << 1).toString(16);
        }
    }
}, {
    name: 'LDI',
    score: 704,
    str: '1110KKKKddddKKKK',
    impl: 'Rd ← k',
    add: { d: 16 }
}, {
    name: 'LDS',
    score: 243,
    str: '1001000xxxxx0000kkkkkkkkkkkkkkkk',
    impl: 'Rx ← k',
    resolve: { k: true },
    bytes: 4
}, {
    name: 'LDX',
    str: '1001000ddddd1100',
    impl: 'Rd \u2190 (X);',
    cycles: 2
}, {
    name: 'LDX+',
    str: '1001000ddddd1101',
    impl: ['Rd \u2190 (X);', 'WR1 ++;'],
    cycles: 2
}, {
    name: 'LDX-',
    str: '1001000ddddd1110',
    impl: ['WR1 --;', 'Rd \u2190 (X);'],
    cycles: 2
}, {
    name: 'LDY',
    str: '1000000ddddd1000',
    impl: 'Rd \u2190 (Y)',
    cycles: 2
}, {
    name: 'LDY+',
    str: '1001000ddddd1001',
    impl: ['Rd \u2190 (Y);', 'WR2 ++;'],
    cycles: 2
}, {
    name: 'LDY-',
    str: '1001000ddddd1010',
    impl: ['WR2 --;', 'Rd \u2190 (Y);'],
    cycles: 2
}, {
    name: 'LDYQ',
    score: 81,
    str: '10q0qq0ddddd1qqq',
    impl: ['Rd \u2190 (Y+q);'],
    cycles: 2
}, {
    name: 'LDZ',
    str: '1000000ddddd0000',
    impl: 'Rd \u2190 (Z);',
    cycles: 2
}, {
    name: 'LDZ+',
    str: '1001000ddddd0001',
    impl: ['Rd \u2190 (Z);', 'WR3 ++;'],
    cycles: 2
}, {
    name: 'LDZ-',
    str: '1001000ddddd0010',
    impl: ['WR3 --;', 'Rd \u2190 (Z);'],
    cycles: 2
}, {
    name: 'LDZQ',
    str: '10q0qq0ddddd0qqq',
    impl: ['Rd \u2190 (Z+q);'],
    cycles: 2
}, {
    name: 'LPMi',
    str: '1001010111001000',
    impl: 'R0 ← FLASH(Z)'
}, {
    name: 'LPMii',
    str: '1001000ddddd0100',
    impl: 'Rd ← FLASH(Z)'
}, {
    name: 'LPMiii',
    str: '1001000ddddd0101',
    impl: ['Rd ← FLASH(Z);', 'WR3 ++;']
}, {
    name: 'LSR',
    str: '1001010ddddd0110',
    // debug:true,
    impl: ['SR0 = Rd@0', 'Rd ← Rd >>> 1', 'SR2 = 0', 'SR3 = SR@2 ^ SR0'],
    flags: 'zs'
}, {
    name: 'MOV',
    score: 173,
    str: '001011rdddddrrrr',
    impl: ['Rd ← Rr;']
}, {
    name: 'MOVW',
    score: 250,
    str: '00000001ddddrrrr',
    impl: ['Rd = Rr', 'Rd+1 = Rr+1'],
    shift: {
        d: 1,
        r: 1
    },
    print: {
        r: function r(_r) {
            return "r" + _r + ":r" + (_r + 1);
        },
        d: function d(_d2) {
            return "r" + _d2 + ":r" + (_d2 + 1);
        }
    }
}, {
    name: 'MULSU',
    str: '000000110ddd0rrr',
    impl: ['i8a[0] = Rd', 't1 = i8a[0] * Rr', 'R0 = t1', 'R1 = t1 >> 8', 'SR1 = !t1|0', 'SR0 = (t1>>15)&1'],
    add: { d: 16, r: 16 }
}, {
    name: 'MULS',
    str: '00000010ddddrrrr',
    impl: ['i8a[0] = Rd', 'i8a[1] = Rr', 't1 = i8a[0] * i8a[1]', 'R0 = t1', 'R1 = t1 >> 8', 'SR1 = !t1|0', 'SR0 = (t1>>15)&1'],
    add: { d: 16, r: 16 }
}, {
    name: 'OR',
    str: '001010rdddddrrrr',
    impl: ['Rd ← Rd | Rr;', 'SR@3 ← 0'],
    flags: 'zns'
}, {
    name: 'ORI',
    str: '0110KKKKddddKKKK',
    impl: ['Rd ← Rd | k;', 'SR@3 ← 0'],
    add: { d: 16 },
    flags: 'zns'
}, {
    name: 'OUTsr',
    str: '1011111rrrrr1111',
    impl: 'I/O[63] ← SR ← Rr',
    cycles: 1
}, {
    name: 'OUTsph',
    str: '1011111rrrrr1110',
    impl: ['I/O[62] ← Rr;', 'sp = (io[62]<<8) | (sp&0xFF);'],
    cycles: 1
}, {
    name: 'OUTspl',
    str: '1011111rrrrr1101',
    impl: ['I/O[61] ← Rr;', 'sp = (sp&0xFF00) | io[61];'],
    cycles: 1
}, {
    name: 'OUT',
    str: '10111AArrrrrAAAA',
    impl: 'I/O[a] \u2190 Rr',
    cycles: 1
}, {
    name: 'PUSH',
    score: 123,
    str: '1001001ddddd1111',
    impl: '(STACK) ← Rd',
    cycles: 2
}, {
    name: 'POP',
    score: 160,
    str: '1001000ddddd1111',
    impl: 'Rd ← (STACK)',
    cycles: 2
}, {
    name: 'RET',
    str: '1001010100001000',
    cycles: 4,
    end: true,
    impl: 'PC ← (STACK2)'
}, {
    name: 'RETI',
    str: '1001010100011000',
    cycles: 4,
    end: true,
    impl: ['memory[0x5F] = (SR |= 1<<7);', 'PC ← (STACK2)']
}, {
    name: 'ROR',
    str: '1001010ddddd0111',
    impl: ['SR0 = Rd@0', 'Rd ← Rd >>> 1 | (SR<<7&0x80)', 'SR2 = R>>7', 'SR3 = SR@2 ^ SR0'],
    flags: 'zs'
}, {
    name: 'HALT',
    str: '1100111111111111',
    impl: 'PC \u2190 PC - 1',
    end: true
}, {
    name: 'RCALL',
    str: '1101kkkkkkkkkkkk',
    cycles: 3,
    impl: ['(STACK2) ← PC + 1', 'PC \u2190 PC + (k << 20 >> 20) + 1'],
    end: false
}, {
    name: 'RJMP',
    score: 184,
    str: '1100kkkkkkkkkkkk',
    impl: 'PC \u2190 PC + (k << 20 >> 20) + 1',
    end: true,
    print: {
        k: function k(_k14, core) {
            return "#" + (core.pc + (_k14 << 20 >> 20) + 1 << 1).toString(16);
        }
    }
}, {
    name: 'SEC',
    str: '1001010000001000',
    impl: 'SR@0 \u2190 1'
}, {
    name: 'SET',
    str: '1001010001101000',
    impl: 'SR@6 \u2190 1'
}, {
    name: 'SEI',
    str: '1001010001111000',
    impl: 'SR@7 \u2190 1'
}, {
    name: 'SFMUL',
    str: '000000111ddd0rrr',
    impl: ['i8a[0] = Rd', 'i8a[1] = Rr', 't1 = i8a[0] * i8a[1] << 1', 'R0 = t1', 'R1 = t1 >> 8', 'SR1 = !t1|0', 'SR0 = (t1>>15)&1'],
    add: { d: 16, r: 16 }
}, {
    name: 'STS',
    score: 344,
    str: '1001001ddddd0000kkkkkkkkkkkkkkkk',
    impl: 'this.write( k, Rd )',
    bytes: 4
}, {
    name: 'STX',
    str: '1001001rrrrr1100',
    impl: '(X) \u2190 Rr'
}, {
    name: 'STX+',
    str: '1001001rrrrr1101',
    impl: ['(X) \u2190 Rr', 'WR1 ++;']
}, {
    name: 'STX-',
    str: '1001001rrrrr1110',
    impl: ['WR1 --;', '(X) \u2190 Rr']
}, {
    name: 'STY',
    str: '1000001rrrrr1000',
    impl: '(Y) \u2190 Rr'
}, {
    name: 'STY+',
    str: '1001001rrrrr1001',
    impl: ['(Y) \u2190 Rr', 'WR2 ++;']
}, {
    name: 'STY-',
    str: '1001001rrrrr1010',
    impl: ['WR2 --;', '(Y) \u2190 Rr']
}, {
    name: 'STYQ',
    score: 121,
    str: '10q0qq1rrrrr1qqq',
    impl: ['(Y+q) \u2190 Rr']
}, {
    name: 'STZ',
    str: '1000001rrrrr0000',
    impl: '(Z) \u2190 Rr'
}, {
    name: 'STZ+',
    str: '1001001rrrrr0001',
    impl: ['(Z) \u2190 Rr', 'WR3 ++;']
}, {
    name: 'STZ-',
    str: '1001001rrrrr0010',
    impl: ['WR3 --;', '(Z) \u2190 Rr']
}, {
    name: 'STZQ',
    str: '10q0qq1rrrrr0qqq',
    impl: ['(Z+q) \u2190 Rr']
}, {
    name: 'SBC',
    str: '000010rdddddrrrr',
    impl: ['Rd ← (Rd - Rr - SR@0) & 0xFF;', 'SR@5 ← (Rd@3 ¯ • Rr@3) | (Rr@3 • R@3) | (R@3 • Rd@3 ¯)', 'SR@0 ← (Rd@7 ¯ • Rr@7) | (Rr@7 • R@7) | (R@7 • Rd@7 ¯)', 'SR@3 ← (Rd@7 • Rr@7 ¯ • R@7 ¯) | (Rd@7 ¯ • Rr@7 • R@7)', 'SR@1 ← (!R) & SR@1'],
    flags: 'ns'
}, {
    name: 'SUB',
    str: '000110rdddddrrrr',
    impl: ['Rd ← (Rd - Rr)&0xFF;', 'SR@5 ← (Rd@3 ¯ • Rr@3) | (Rr@3 • R@3) | (R@3 • Rd@3 ¯)', 'SR@0 ← (Rd@7 ¯ • Rr@7) | (Rr@7 • R@7) | (R@7 • Rd@7 ¯)', 'SR@3 ← (Rd@7 • Rr@7 ¯ • R@7 ¯) | (Rd@7 ¯ • Rr@7 • R@7)'],
    flags: 'zns'
}, {
    name: 'SBCI',
    score: 70,
    str: '0100KKKKddddKKKK',
    impl: ['Rd ← (Rd - k - SR@0)&0xFF;', 'SR@5 ← (Rd@3 ¯ • ((k>>3)&1)) | (((k>>3)&1) • R@3) | (R@3 • Rd@3 ¯)', 'SR@0 ← (Rd@7 ¯ • ((k>>7)&1)) | (((k>>7)&1) • R@7) | (R@7 • Rd@7 ¯)', 'SR@3 ← (Rd@7 • ((k>>7)&1^1) • R@7 ¯) | (Rd@7 ¯ • ((k>>7)&1) • R@7)', 'SR@1 ← (!R) & SR@1'],
    add: { d: 16 },
    flags: 'ns'
}, {
    name: 'SUBI',
    str: '0101KKKKddddKKKK',
    impl: ['Rd ← Rd - k;', 'SR@5 ← (Rd@3 ¯ • ((k>>3)&1)) | (((k>>3)&1) • R@3) | (R@3 • Rd@3 ¯)', 'SR@0 ← (Rd@7 ¯ • ((k>>7)&1)) | (((k>>7)&1) • R@7) | (R@7 • Rd@7 ¯)', 'SR@3 ← (Rd@7 • ((k>>7)&1^1) • R@7 ¯) | (Rd@7 ¯ • ((k>>7)&1) • R@7)'],
    add: { d: 16 },
    flags: 'zns'
}, {
    name: 'SBI',
    str: '10011010AAAAAbbb',
    impl: 'I/O[a@b] ← 1;'
}, {
    name: 'SBIW',
    str: '10010111KKddKKKK',
    impl: ['WRd ← WRd - k;', 'SR@0 ← WRd@15 ¯ • R@15', 'SR@3 ← WRd@15 • R@15 ¯'],
    print: {
        d: function d(_d3) {
            return "WXYZ"[_d3];
        }
    },
    flags: 'ZNS'
}, {
    name: 'SBIC',
    str: '10011001AAAAAbbb',
    impl: 'SKIP ← !(a&(1<<b))',
    skip: true,
    add: { A: 32 },
    resolve: { A: true }
}, {
    name: 'SBIS',
    str: '10011011AAAAAbbb',
    impl: 'SKIP ← a&(1<<b)',
    skip: true,
    add: { A: 32 },
    resolve: { A: true }
}, {
    name: 'SBRC',
    str: '1111110rrrrr0bbb',
    // debug: true,
    impl: 'SKIP ← !(Rr & (1<<b))',
    skip: true
}, {
    name: 'SBRS',
    str: '1111111rrrrr0bbb',
    // debug: true,
    impl: 'SKIP ← Rr & (1<<b)',
    skip: true
}, {
    name: 'SLEEP',
    str: '1001010110001000',
    impl: ['this.sleeping = true', 'PC ← PC + 1'],
    // debug: true,
    cycles: 0
}, {
    name: 'SWAP',
    str: '1001010ddddd0010',
    impl: ['Rd ← (Rd >>> 4) | (Rd << 4)']
}, {
    name: "WDR",
    str: '1001010110101000',
    impl: ['']
}];

},{}],16:[function(require,module,exports){
"use strict";

function _defineProperty(obj, key, value) { if (key in obj) { Object.defineProperty(obj, key, { value: value, enumerable: true, configurable: true, writable: true }); } else { obj[key] = value; } return obj; }

module.exports = function (addrs) {
	var _write, _read;

	return {

		write: (_write = {}, _defineProperty(_write, addrs.TIFR, function (value) {

			this.TOVn = value & 1;
			this.OCFnA = 0; // (value>>1) & 1;
			this.OCFnB = 0; // (value>>2) & 1;
			this.OCFnC = 0;
		}), _defineProperty(_write, addrs.TCCR_A, function (value) {

			this.WGMn0 = value >> 0 & 1;
			this.WGMn1 = value >> 1 & 1;
			this.COM0C0 = value >> 2 & 1;
			this.COM0C1 = value >> 3 & 1;
			this.COM0B0 = value >> 4 & 1;
			this.COM0B1 = value >> 5 & 1;
			this.COM0A0 = value >> 6 & 1;
			this.COM0A1 = value >> 7 & 1;

			this.updateState();
		}), _defineProperty(_write, addrs.TCCR_B, function (value) {

			this.ICNC1 = value >> 7 & 1;
			this.ICES1 = value >> 6 & 1;

			this.WGMn3 = value >> 4 & 1;
			this.WGMn2 = value >> 3 & 1;
			this.CSn2 = value >> 2 & 1;
			this.CSn1 = value >> 1 & 1;
			this.CSn0 = value & 1;

			this.updateState();
		}), _defineProperty(_write, addrs.TCCR_C, function (value) {
			this.FOCnA = value >> 7 & 1;
			this.FOCnB = value >> 6 & 1;
			this.FOCnC = value >> 5 & 1;
		}), _defineProperty(_write, addrs.OCR_AH, function (value) {
			this.OCRnA = value << 8 | this.OCRnA & 0xFF;
		}), _defineProperty(_write, addrs.OCR_AL, function (value) {
			this.OCRnA = value | this.OCRnA & 0xFF00;
		}), _defineProperty(_write, addrs.OCR_BH, function (value) {
			this.OCRnB = value << 8 | this.OCRnB & 0xFF;
		}), _defineProperty(_write, addrs.OCR_BL, function (value) {
			this.OCRnB = value | this.OCRnB & 0xFF00;
		}), _defineProperty(_write, addrs.TCNTH, function (value) {
			this.TCNT = value << 8 | this.TCNT & 0xFF;
		}), _defineProperty(_write, addrs.TCNTL, function (value) {
			this.TCNT = value | this.TCNT & 0xFF00;
		}), _defineProperty(_write, addrs.TIMSK, function (value) {
			this.TOIE0 = value & 1;
			this.OCIEnA = value >> 1 & 1;
			this.OCIEnB = value >> 2 & 1;
			this.OCIEnC = value >> 3 & 1;
		}), _write),

		init: function init() {
			this.tick = 0;
			this.WGMn0 = 0;
			this.WGMn1 = 0;

			this.OCFnA = 0;
			this.OCFnB = 0;
			this.OCFnC = 0;

			this.COM0C0 = 0;
			this.COM0C1 = 0;
			this.COM0B0 = 0;
			this.COM0B1 = 0;
			this.COM0A0 = 0;
			this.COM0A1 = 0;

			this.ICNC1 = 0;
			this.ICES1 = 0;
			this.WGMn3 = 0;
			this.WGMn2 = 0;
			this.CSn2 = 0;
			this.CSn1 = 0;
			this.CSn0 = 0;

			this.TOVn = 0;
			this.FOCnA = 0;
			this.FOCnB = 0;
			this.FOCnC = 0;

			this.TOIE0 = 0;
			this.OCIEnA = 0;
			this.OCIEnB = 0;
			this.OCIEnC = 0;

			this.OCRnA = 0;
			this.OCRnB = 0;
			this.OCRnC = 0;

			this.TCNT = 0;
			this.prescale = 0;
			this.TOP = 0xFFFF;

			this._update = function (tick) {
				if (!this.prescale) return;

				tick = tick || this.core.tick;

				var ticksSinceOVF = tick - this.tick;
				var interval = ticksSinceOVF / this.prescale | 0;
				if (!interval) return;

				var oldTCNT = this.TCNT;

				var cnt = this.TCNT += interval;

				this.TCNT &= 0xFFFF;

				this.tick += interval * this.prescale;

				if (this.OCIEnA && oldTCNT < this.OCRnA && cnt >= this.OCRnA) {
					this.OCFnA++;
					if (this.CTC && !this.FOCnA) this.TCNT = cnt = 0;
				}
				if (this.OCIEnB && oldTCNT < this.OCRnB && cnt >= this.OCRnB) {
					this.OCFnB++;
					if (this.CTC && !this.FOCnB) this.TCNT = cnt = 0;
				}
				if (this.OCIEnC && oldTCNT < this.OCRnC && cnt >= this.OCRnC) {
					this.OCFnC++;
					if (this.CTC && !this.FOCnC) this.TCNT = cnt = 0;
				}

				this.core.memory[addrs.TCNTH] = cnt >>> 8;
				this.core.memory[addrs.TCNTL] = cnt & 0xFF;

				this.TOVn += cnt / this.TOP | 0;
			};

			this.updateState = function () {

				var MAX = 0xFF,
				    BOTTOM = 0,
				    WGMn0 = this.WGMn0,
				    WGMn1 = this.WGMn1,
				    WGMn2 = this.WGMn2,
				    WGMn3 = this.WGMn3;
				var WGM = this.WGMn3 << 3 | this.WGMn2 << 2 | this.WGMn1 << 1 | this.WGMn0;
				var CS = this.CSn0 | this.CSn1 << 1 | this.CSn2 << 2;

				if (WGM != this.oldWGM) {
					this.CTC = false;
					this.TOP = 0xFFFF;

					switch (WGM) {
						case 0:
							// console.log("0- Timer16=Normal TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							break;
						case 1:
							// console.log("1- Timer16=PWM,PC8b TOP=0xFF UpdateOCRnx=imm TOVn=MAX");
							this.TOP = 0xFF;
							break;
						case 2:
							// console.log("2- Timer16=PWM,PC9b TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							this.TOP = 0x1FF;
							break;
						case 3:
							// console.log("3- Timer16=PWM,PC10b TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							this.TOP = 0x3FF;
							break;
						case 4:
							// console.log("4- Timer16=CTC TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							this.CTC = true;
							break;
						case 5:
							// console.log("5- Timer16=FPWM8b TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							this.TOP = 0xFF;
							break;
						case 6:
							// console.log("6- Timer16=FPWM9b TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							this.TOP = 0x1FF;
							break;
						case 7:
							// console.log("7- Timer16=FPWM10b TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							this.TOP = 0x3FF;
							break;
						case 8:
							// console.log("8- Timer16=PWMPFC TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							break;
						case 9:
							// console.log("9- Timer16=PWMPFC TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							break;
						case 10:
							// console.log("10- Timer16=PWMPC TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							break;
						case 11:
							// console.log("11- Timer16=PWMPC TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							break;
						case 12:
							// console.log("12- Timer16=CTC TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							this.CTC = true;
							break;
						case 13:
							// console.log("13- Timer16=RESERVED");
							break;
						case 14:
							// console.log("14- Timer16=FPWM TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							break;
						case 15:
							// console.log("15- Timer16=FPWM TOP=0xFFFF UpdateOCRnx=imm TOVn=MAX");
							break;
					}
				}

				this.oldWGM = WGM;

				switch (CS) {
					case 0:
						this.prescale = 0;break;
					case 1:
						this.prescale = 1;break;
					case 2:
						this.prescale = 8;break;
					case 3:
						this.prescale = 64;break;
					case 4:
						this.prescale = 256;break;
					case 5:
						this.prescale = 1024;break;
					default:
						this.prescale = 1;break;
				}
			};
		},

		read: (_read = {}, _defineProperty(_read, addrs.TCCR_C, function (value) {
			return 0;
		}), _defineProperty(_read, addrs.TIFR, function () {
			return !!this.TOVn & 1 | this.OCFnA << 1 | this.OCFnB << 2;
		}), _defineProperty(_read, addrs.TCNTH, function () {

			this._update();

			return this.TCNT >>> 8;
		}), _defineProperty(_read, addrs.TCNTL, function () {

			this._update();

			return this.TCNT & 0xFF;
		}), _read),

		update: function update(tick, ie) {

			this._update();

			if (ie) {

				if (this.OCFnA > 0 && this.OCIEnA && !this.FOCnA) {
					this.OCFnA = 0;
					return addrs.intCOMPA;
				}

				if (this.OCFnB > 0 && this.OCIEnB && !this.FOCnB) {
					this.OCFnB = 0;
					return addrs.intCOMPB;
				}

				if (this.OCFnC > 0 && this.OCIEnC && !this.FOCnC) {
					this.OCFnC = 0;
					return addrs.intCOMPC;
				}

				if (this.TOVn > 0 && this.TOIE0) {
					this.TOVn = 0;
					return addrs.intOV;
				}
			}
		}

	};
};

},{}],17:[function(require,module,exports){
"use strict";

function _defineProperty(obj, key, value) { if (key in obj) { Object.defineProperty(obj, key, { value: value, enumerable: true, configurable: true, writable: true }); } else { obj[key] = value; } return obj; }

module.exports = function (addrs) {
		var _write, _read;

		return {

				write: (_write = {}, _defineProperty(_write, addrs.TIFR, function (value) {

						this.TOV0 = value & 1;
						this.OCF0A = value >> 1 & 1;
						this.OCF0B = value >> 2 & 1;
				}), _defineProperty(_write, addrs.TCCR_A, function (value) {

						this.WGM00 = value >> 0 & 1;
						this.WGM01 = value >> 1 & 1;
						this.COM0B0 = value >> 4 & 1;
						this.COM0B1 = value >> 5 & 1;
						this.COM0A0 = value >> 6 & 1;
						this.COM0A1 = value >> 7 & 1;

						this.updateState();

						// console.log(`TCCR0A:\n  WGM00:${this.WGM00}\n  WGM01:${this.WGM01}\n  COM0B0:${this.COM0B0}\n  COM0B1:${this.COM0B1}\n  COM0A0:${this.COM0A0}\n  COM0A1:${this.COM0A1}`);
				}), _defineProperty(_write, addrs.TCCR_B, function (value) {

						this.FOC0A = value >> 7 & 1 ^ 1;
						this.FOC0B = value >> 6 & 1 ^ 1;
						this.WGM02 = value >> 3 & 1;
						this.CS = value & 7;

						this.updateState();

						console.log("TCCR0B:\n  FOC0A:" + this.FOC0A + "\n  FOC0B:" + this.FOC0B + "\n  WGM02:" + this.WGM02 + " CS:" + this.CS + "\n");

						// console.log( "PC=" + (this.core.pc<<1).toString(16) + " WRITE TCCR0B: #" + value.toString(16) + " : " + value );
				}), _defineProperty(_write, addrs.OCR_A, function (value) {
						this.OCR0A = value;
				}), _defineProperty(_write, addrs.OCR_B, function (value) {
						this.OCR0B = value;
						// console.log( "OCR0B = " + value );
				}), _defineProperty(_write, addrs.TIMSK, function (value) {
						this.TOIE0 = value & 1;
						this.OCIE0A = value >> 1 & 1;
						this.OCIE0B = value >> 2 & 1;
				}), _write),

				init: function init() {
						this.tick = 0;
						this.WGM00 = 0;
						this.WGM01 = 0;
						this.COM0B0 = 0;
						this.COM0B1 = 0;
						this.COM0A0 = 0;
						this.COM0A1 = 0;
						this.FOC0A = 0;
						this.FOC0B = 0;
						this.WGM02 = 0;
						this.CS = 0;
						this.TOV0 = 0;

						this.TOIE0 = 0;
						this.OCIE0A = 0;
						this.OCIE0B = 0;
						this.OCR0A = 0;
						this.OCR0B = 0;

						this.time = 0;

						this.updateState = function () {

								var MAX = 0xFF,
								    BOTTOM = 0,
								    WGM00 = this.WGM00,
								    WGM01 = this.WGM01,
								    WGM02 = this.WGM02;

								var WGM = WGM02 << 2 | WGM02 << 1 | WGM00;

								var msg = null;

								switch (WGM) {
										case 0:
												msg = "Timer Mode: Normal";break;
										case 1:
												msg = "Timer Mode: PWM, phase correct";break;
										case 2:
												msg = "Timer Mode: CTC";break;
										case 3:
												msg = "Timer Mode: Fast PWM";break;
										case 4:
												msg = "Timer Mode: Reserved";break;
										case 5:
												msg = "Timer Mode: PWM, phase correct";break;
										case 6:
												msg = "Timer Mode: Reserved";break;
										case 7:
												msg = "Timer Mode: Fast PWM";break;
								}

								if (this.mode !== WGM) console.log(msg + " (" + WGM + ") CS=" + this.CS);

								this.mode = WGM;

								switch (this.CS) {
										case 0:
												this.prescale = 0;break;
										case 1:
												this.prescale = 1;break;
										case 2:
												this.prescale = 8;break;
										case 3:
												this.prescale = 64;break;
										case 4:
												this.prescale = 256;break;
										case 5:
												this.prescale = 1024;break;
										default:
												this.prescale = 1;break;
								}
						};
				},

				read: (_read = {}, _defineProperty(_read, addrs.TCCRB, function (v) {
						var FOC0A = !this.FOC0A << 7;
						var FOC0B = !this.FOC0B << 6;
						v = v & ~(3 << 6) | FOC0A | FOC0B;
						return v;
				}), _defineProperty(_read, addrs.TIFR, function () {
						return !!this.TOV0 & 1 | this.OCF0A << 1 | this.OCF0B << 2;
				}), _defineProperty(_read, addrs.TCNT, function () {

						var tick = this.core.tick;

						var ticksSinceOVF = tick - this.tick;
						var interval = ticksSinceOVF / this.prescale | 0;
						if (!interval) return;

						var cnt = this.core.memory[addrs.TCNT] + interval;

						if (cnt > this.OCR0A && this.TCNT < this.OCR0A) this.FOC0A += (cnt - this.OCR0A) / 0xFF | 0;

						if (cnt > this.OCR0B && this.TCNT < this.OCR0B) this.FOC0B += (cnt - this.OCR0B) / 0xFF | 0;

						this.core.memory[addrs.TCNT] = this.TCNT = cnt;

						var scaled = interval * this.prescale;
						this.tick += scaled;

						this.TOV0 += cnt / 0xFF | 0;

						return cnt;
				}), _read),

				update: function update(tick, ie) {

						var ticksSinceOVF = tick - this.tick;
						var interval = ticksSinceOVF / this.prescale | 0;

						if (interval) {

								var cnt = this.core.memory[addrs.TCNT] + interval;

								if (cnt > this.OCR0A && this.TCNT < this.OCR0A) this.FOC0A += (cnt - this.OCR0A) / 0xFF | 0;

								if (cnt > this.OCR0B && this.TCNT < this.OCR0B) this.FOC0B += (cnt - this.OCR0B) / 0xFF | 0;

								this.core.memory[addrs.TCNT] = this.TCNT = cnt;

								var scaled = interval * this.prescale;
								this.tick += scaled;

								this.TOV0 += cnt / 0xFF | 0;
						}

						if (ie) {

								if (this.TOV0 > 0 && this.TOIE0) {
										this.TOV0--;
										return addrs.intOV;
								}

								if (this.FOC0A > 0 && this.OCIE0A) {
										this.FOC0A--;
										return addrs.cmpA;
								}

								if (this.FOC0B > 0 && this.OCIE0B) {
										this.FOC0B--;
										return addrs.cmpB;
								}
						}
				}

		};
};

},{}],18:[function(require,module,exports){
"use strict";

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var compiler = "https://projectabe.herokuapp.com/";

var CloudCompiler = function () {
	function CloudCompiler() {
		_classCallCheck(this, CloudCompiler);
	}

	_createClass(CloudCompiler, [{
		key: "build",
		value: function build(src) {
			var _this = this;

			var ok = void 0,
			    nok = void 0;
			var p = new Promise(function (_ok, _nok) {
				ok = _ok;
				nok = _nok;
			});

			fetch(compiler + "build", {
				method: "POST",
				body: JSON.stringify(src)
			}).then(function (rsp) {
				return rsp.text();
			}).then(function (txt) {

				_this.compileId = parseInt(txt);
				_this.pollCompilerService(src, ok, nok);
			}).catch(function (err) {
				nok(err);
			});

			return p;
		}
	}, {
		key: "getSketchDir",
		value: function getSketchDir() {
			return '';
		}
	}, {
		key: "getUserGames",
		value: function getUserGames(out) {}
	}, {
		key: "pollCompilerService",
		value: function pollCompilerService(src, ok, nok) {
			var _this2 = this;

			fetch(compiler + "poll?id=" + this.compileId).then(function (rsp) {
				return rsp.text();
			}).then(function (txt) {

				if (txt == "DESTROYED") {

					_this2.compileId = null;
					_this2.build(src);
					return;
				} else if (txt[0] == "{") {

					var data = JSON.parse(txt);

					if (data.path) {

						fetch(compiler + data.path).then(function (rsp) {
							return rsp.text();
						}).then(function (text) {
							data.hex = text;
							data.path = compiler + data.path;
							ok(data);
						});
					} else ok(data);
				} else if (/^ERROR[\s\S]*/.test(txt)) {
					nok(txt);
				} else setTimeout(function (_) {
					return _this2.pollCompilerService(src, ok, nok);
				}, 3000);
			}).catch(function (err) {
				nok(err);
			});
		}
	}]);

	return CloudCompiler;
}();

module.exports = CloudCompiler;

},{}],19:[function(require,module,exports){
"use strict";

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var BTN = function () {
			function BTN(DOM) {
						var _this = this;

						_classCallCheck(this, BTN);

						this.on = {
									connect: null,
									init: function init() {
												this.on.value = !this.active;
									}
						};


						DOM.element.controller = this;
						DOM.element.dispatchEvent(new Event("addperiferal", { bubbles: true }));
						this.on.connect = DOM.element.getAttribute("pin-on");
						this.active = DOM.element.getAttribute("active") != "low";

						DOM.element.addEventListener("mousedown", function (_) {
									return _this.on.value = _this.active;
						});
						DOM.element.addEventListener("mouseup", function (_) {
									return _this.on.value = !_this.active;
						});
						DOM.element.addEventListener("touchstart", function (_) {
									return _this.on.value = _this.active;
						});
						DOM.element.addEventListener("touchend", function (_) {
									return _this.on.value = !_this.active;
						});

						(DOM.element.getAttribute("bind-key") || "").split(/\s*,\s*/).forEach(function (k) {
									_this["onPress" + k] = function (_) {
												return _this.on.value = _this.active;
									};
									_this["onRelease" + k] = function (_) {
												return _this.on.value = !_this.active;
									};
						});

						this.pool.add(this);
			}

			_createClass(BTN, [{
						key: "setActiveView",
						value: function setActiveView() {
									this.pool.remove(this);
						}
			}]);

			return BTN;
}();

BTN["@inject"] = {
			pool: "pool"
};


module.exports = BTN;

},{}],20:[function(require,module,exports){
'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _mvc = require('../lib/mvc.js');

var _IStore = require('../store/IStore.js');

var _IStore2 = _interopRequireDefault(_IStore);

var _jszipMin = require('jszip/dist/jszip.min.js');

var _jszipMin2 = _interopRequireDefault(_jszipMin);

var _dryDom = require('../lib/dry-dom.js');

var _dryDom2 = _interopRequireDefault(_dryDom);

var _image = require('../lib/image.js');

var _image2 = _interopRequireDefault(_image);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var fs = void 0,
    path = void 0;

if (window.require) {
	try {
		fs = window.require('fs');
		path = window.require('path');
	} catch (ex) {}
}

var Debugger = function () {
	function Debugger(DOM) {
		var _this = this;

		_classCallCheck(this, Debugger);

		this.saveHandles = {};
		this.disableCommits = false;

		this.model.setItem("ram.fuzzy", []);
		this.model.setItem("ram.blocksizes", []);

		this.pool.add(this);

		this.DOM = DOM;
		this.history = [];
		this.da = [];
		this.RAM = [];
		this.state = [];
		this.hints = {};
		this.comments = {};
		this.srcmap = [];
		this.rsrcmap = {};
		this.currentPC = null;
		this.ramComments = {};
		this.ramIndex = {};

		for (var i = 0; i < 32; ++i) {
			this.ramComments[i] = "@ R" + i;
		}["Reserved", "Reserved", "Reserved", "PINB", "DDRB", "PORTB", "PINC", "DDRC", "PORTC", "PIND", "DDRD", "PORTD", "PINE", "DDRE", "PORTE", "PINF", "DDRF", "PORTF", "Reserved", "Reserved", "Reserved", "TIFR0", "TIFR1", "Reserved", "TIFR3", "TIFR4", "Reserved", "PCIFR", "EIFR", "EIMSK", "GPIOR0", "EECR", "EEDR", "EEARL", "EEARH", "GTCCR", "TCCR0A", "TCCR0B", "TCNT0", "OCR0A", "OCR0B", "PLLCSR", "GPIOR1", "GPIOR2", "SPCR", "SPSR", "SPDR", "Reserved", "ACSR", "OCDR / MONDR", "PLLFRQ", "SMCR", "MCUSR", "MCUCR", "Reserved", "SPMCSR", "Reserved", "Reserved", "Reserved", "RAMPZ", "Reserved", "SPL", "SPH", "SREG", "WDTCSR", "CLKPR", "Reserved", "Reserved", "PRR0", "PRR1", "OSCCAL", "RCCTRL", "PCICR", "EICRA", "EICRB", "PCMSK0", "Reserved", "Reserved", "TIMSK0", "TIMSK1", "Reserved", "TIMSK3", "TIMSK4", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "ADCL", "ADCH", "ADCSRA", "ADCSRB", "ADMUX", "DIDR2", "DIDR0", "DIDR1", "TCCR1A", "TCCR1B", "TCCR1C", "Reserved", "TCNT1L", "TCNT1H", "ICR1L", "ICR1H", "OCR1AL", "OCR1AH", "OCR1BL", "OCR1BH", "OCR1CL", "OCR1CH", "Reserved", "Reserved", "TCCR3A", "TCCR3B", "TCCR3C", "Reserved", "TCNT3L", "TCNT3H", "ICR3L", "ICR3H", "OCR3AL", "OCR3AH", "OCR3BL", "OCR3BH", "OCR3CL", "OCR3CH", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "TWBR", "TWSR", "TWAR", "TWDR", "TWCR", "TWAMR", "TCNT4", "TC4H", "TCCR4A", "TCCR4B", "TCCR4C", "TCCR4D", "TCCR4E", "CLKSEL0", "CLKSEL1", "CLKSTA", "UCSR1A", "UCSR1B", "UCSR1C", "Reserved", "UBRR1L", "UBRR1H", "UDR1", "OCR4A", "OCR4B", "OCR4C", "OCR4D", "Reserved", "DT4", "Reserved", "Reserved", "UHWCON", "USBCON", "USBSTA", "USBINT", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "UDCON", "UDINT", "UDIEN", "UDADDR", "UDFNUML", "UDFNUMH", "UDMFN", "Reserved", "UEINTX", "UENUM", "UERST", "UECONX", "UECFG0X", "UECFG1X", "UESTA0X", "UESTA1X", "UEIENX", "UEDATX", "UEBCLX", "UEBCHX", "UEINT", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved"].forEach(function (name, i) {
			return _this.ramComments[i + 0x20] = name;
		});

		this.code = null;
		this.compileId = 0;

		// this.initSource();
	}

	_createClass(Debugger, [{
		key: 'setActiveView',
		value: function setActiveView() {
			this.pool.remove(this);
		}
	}, {
		key: 'initSource',
		value: function initSource() {
			var _this2 = this;

			if (this.source) return true;

			this.source = this.model.getModel(this.model.getItem("ram.srcpath"), true) || new _mvc.Model();

			var promise = null;

			var srcurl = this.model.getItem("ram.srcurl", "");
			var lsp = this.model.getItem("ram.localSourcePath", "");

			if (/.*\.ino$/.test(srcurl)) {

				promise = fetch(this.model.getItem("app.proxy") + srcurl).then(function (rsp) {
					return rsp.text();
				}).then(function (txt) {

					if (txt.charCodeAt(0) == 0xFEFF) txt = txt.substr(1);

					_this2.addNewFile("main.ino", txt);
				});
			} else if (srcurl) {

				promise = fetch(this.model.getItem("app.proxy") + srcurl).then(function (rsp) {
					return rsp.arrayBuffer();
				}).then(function (buff) {
					return _jszipMin2.default.loadAsync(buff);
				}).then(function (z) {
					return _this2.importZipSourceFiles(z);
				});
			} else if (lsp && fs) {
				this.pool.call("initWatches");

				var readDir = function readDir(d) {
					fs.readdir(d, function (err, files) {

						if (!files) {
							console.log("Could not read " + d);
							console.log(err);
							return;
						}

						files.forEach(function (file) {

							if (file[0] == '.') return;

							var ffp = d + path.sep + file;
							if (fs.lstatSync(ffp).isDirectory()) return readDir(ffp);

							var ext = file.match(/.*\.(h|hpp|c|cpp|ino)$/i);
							if (ext) _this2.pool.call("watchFile", ffp, function (name, txt) {
								if (txt === undefined) return;
								_this2.addNewFile(name, txt, true);

								if (ext[1].toLowerCase() == 'ino') {
									_this2.DOM.currentFile.value = name;
									_this2.changeSourceFile();
									ext = ["", ""];
								}
							});
						});
					});
				};

				setTimeout(function (_) {
					return readDir(lsp);
				}, 100);
			} else if (!Object.keys(this.source.data).length) {
				var name = prompt("Project Name:");
				lsp = '';
				if (fs) {
					lsp = this.compiler.getSketchDir() + ('/' + name);
					this.model.setItem("ram.localSourcePath", lsp);
				}

				this.addNewFile(name + '.ino', '\n\n// See: https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/\n#include <Arduboy2.h>\n \nArduboy2 arduboy;\n\nvoid setup() {\n  arduboy.begin();\n  arduboy.setFrameRate(30);\n\n  // initialize things here\n\n}\n\nvoid loop() {\n  if (!(arduboy.nextFrame()))\n    return;\n\n  arduboy.clear();\n\n  // game goes here\n\n  arduboy.display();\n}\n');
			}

			if (promise) promise.catch(function (err) {
				console.error(err.toString());
				core.history.push(err.toString());
				_this2.DOM.element.setAttribute("data-tab", "history");
				_this2.refreshHistory();
			});

			if (!this.source) return false;

			this.initEditor();

			return true;
		}
	}, {
		key: 'showDebugger',
		value: function showDebugger() {
			this.DOM.element.setAttribute("hidden", "false");
			this.DOM.element.setAttribute("data-tab", "source");
			this.initSource();
		}
	}, {
		key: 'initEditor',
		value: function initEditor() {
			var _this3 = this;

			if (this.code) return;
			var editor = this.code = ace.edit(this.DOM.ace);
			editor.$blockScrolling = Infinity;
			editor.setTheme("ace/theme/monokai");
			editor.getSession().setMode("ace/mode/c_cpp");
			editor.resize(true);

			editor.session.on("change", function (_) {
				return _this3.commit();
			});
			editor.session.setOptions({ tabSize: 2 });

			var getLineAddress = function getLineAddress(line) {
				var file = _this3.DOM.currentFile.value;
				return _this3.rsrcmap[file + ":" + (line + 1)];
			};

			editor.on("guttermousedown", function (e) {
				var target = e.domEvent.target;
				if (target.className.indexOf("ace_gutter-cell") == -1) return;
				if (!_this3.code.isFocused()) return;

				e.stop();
				var row = e.getDocumentPosition().row;
				var addr = getLineAddress(row);
				if (addr !== undefined) {
					if (core.breakpoints[addr]) core.breakpoints[addr] = false;else core.breakpoints[addr] = function () {
						return true;
					};

					core.enableDebugger();
					_this3.changeBreakpoints();
				} else {
					_this3.code.session.setBreakpoint(row, "invalid");
				}
			});

			var updateTooltip = function updateTooltip(position, text) {

				if (!text) {
					_this3.DOM.codeToolTip.style.display = "none";
					return;
				}

				var offX = 0,
				    offY = 0;
				var pe = _this3.DOM.codeToolTip.parentElement;
				while (pe) {
					offX += pe.offsetLeft;
					offY += pe.offsetTop;
					pe = pe.parentElement;
				}

				_this3.DOM.codeToolTip.setAttribute("data-text", text);
				_this3.DOM.codeToolTip.style.display = "block";
				_this3.DOM.codeToolTip.style.left = position.pageX - offX + 'px';
				_this3.DOM.codeToolTip.style.top = position.pageY - offY + 'px';
			};

			editor.on("mousemove", function (e) {
				var position = e.getDocumentPosition();
				if (!position) return;
				var wordRange = editor.getSession().getWordRange(position.row, position.column);
				var text = editor.session.getTextRange(wordRange);
				var pixelPosition = editor.renderer.textToScreenCoordinates(position);
				pixelPosition.pageY += editor.renderer.lineHeight;

				var addr = _this3.ramIndex[text];
				if (!text || !addr) {
					text = "";
				} else {
					var name = text;
					text += ":\n";

					if (_this3.ramIndex[name + "+0x3"] && !_this3.ramIndex[name + "+0x4"]) {
						// 32-bit
						var value = core.memory[addr] + (core.memory[addr + 1] << 7) + (core.memory[addr + 2] << 14) + (core.memory[addr + 3] << 21);
						if (value & 0x80000000) {
							text += "uint32: " + (value >>> 0) + " (0x" + (value >>> 0).toString(16) + ")\n";
							text += "int32: " + value + " (0x" + value.toString(16) + ")\n";
						} else text += "(u)int32: " + value + " (0x" + value.toString(16) + ")\n";
					} else if (_this3.ramIndex[name + "+0x1"] && !_this3.ramIndex[name + "+0x2"]) {
						var _value = core.memory[addr] + (core.memory[addr + 1] << 8);
						if (_value & 0x8000) {
							text += "uint16: " + (_value >>> 0) + " (0x" + (_value >>> 0).toString(16) + ")\n";
							text += "int16: " + (0xFFFF0000 | _value) + " (0x" + _value.toString(16) + ")\n";
						} else text += "(u)int16: " + _value + " (0x" + _value.toString(16) + ")\n";

						var src = _this3.srcmap[_value];
						if (src && !src.offset && _this3.hints[_value]) {
							_this3.hints[_value].replace(/(?:^|\n)[0-9a-f]+\s+<([^>]+)>:\n/, function (m, fname) {
								text += "fptr: ";
								fname = _this3.unmangle(fname); // fname.replace(/_Z[0-9]+(.*)v/, "$1");
								text += fname + "\n" + src.file + ":" + src.line;
							});
						}
					} else {
						var _value2 = core.memory[addr];
						if (_value2 & 0x80) {
							text += "uint8: " + (_value2 >>> 0) + " (0x" + (_value2 >>> 0).toString(16) + ")\n";
							text += "int8: " + (0xFFFFFF00 | _value2) + " (0x" + _value2.toString(16) + ")\n";
						} else text += "(u)int8: " + _value2 + " (0x" + _value2.toString(16) + ")\n";
					}
				}

				updateTooltip(pixelPosition, text);
			});

			this.code.commands.addCommand({
				name: "replace",
				bindKey: { win: "Ctrl-Enter", mac: "Command-Option-Enter" },
				exec: function exec() {
					return _this3.compile();
				}
			});

			this.code.commands.addCommand({
				name: "fuzzy",
				bindKey: { win: "Ctrl-P", mac: "Command-P" },
				exec: function exec() {
					return _this3.showFuzzyFinder();
				}
			});

			this.changeSourceFile();
		}
	}, {
		key: 'local',
		value: function local(str, cb) {
			if (!cb) cb = str;
			if (typeof str != "string") str = this.DOM.currentFile.value;
			if (typeof cb != "function") cb = null;

			var lsp = this.model.getItem("ram.localSourcePath", "");

			if (!lsp || !fs) return;

			var filePath = path.resolve(lsp, str);

			if (!cb) return filePath;

			return cb(filePath, lsp);
		}
	}, {
		key: 'deleteFile',
		value: function deleteFile() {
			if (!this.initSource()) return;

			if (!confirm("Are you sure you want to delete " + this.DOM.currentFile.value + "?")) return;

			if (this.local(function (file) {
				try {
					fs.unlinkSync(file);
				} catch (err) {
					alert("Unable to delete: " + err);
					return false;
				}
			}) === false) return;

			this.source.removeItem([this.DOM.currentFile.value]);
			this.DOM.currentFile.value = Object.keys(this.source.data)[0];
			this.changeSourceFile();
		}
	}, {
		key: 'renameFile',
		value: function renameFile() {
			if (!this.initSource()) return;

			var current = this.DOM.currentFile.value;
			var shortCurrent = current;

			var lsp = this.model.getItem("ram.localSourcePath", "");
			if (current.startsWith(lsp)) shortCurrent = current.substr(lsp.length + 1);

			var target = prompt("Rename " + shortCurrent + " to:", shortCurrent);
			target = (target || "").trim();

			if (target == "" || target == shortCurrent) return;

			var src = this.source.getItem([current]);

			this.pool.call("saveFile", target, src);

			if (this.local(function (file) {

				var del = function del(_) {
					try {
						fs.unlinkSync(file);
					} catch (ex) {
						alert("Error: " + ex);
						return false;
					}
				};

				target = lsp + path.sep + target;

				var fulltarget = path.resolve(target);
				if (fs.existsSync(target)) {
					alert("Error: New name not available");
					return false;
				}

				// if the only change was capitalization, delete first because of Windows / OS X
				if (target.toLowerCase() == shortCurrent.toLowerCase()) {
					del();
					save();
				} else {
					save();
					del();
				}
			}) === false) return;

			this.source.removeItem([current]);
			this.source.setItem([target], src);
			this.DOM.currentFile.value = target;
		}
	}, {
		key: 'addNewFile',
		value: function addNewFile(target, content, disableSave) {
			var _this4 = this;

			if (!this.initSource()) return;

			if (typeof target !== "string") target = (prompt("File name:") || "").trim();

			if (target == "") return;

			content += "";

			var old = this.source.getItem([target], undefined);

			this.source.setItem([target], content);

			this.local(target, function (ffp) {
				return clearTimeout(_this4.saveHandles[ffp]);
			});

			if (!disableSave && old !== content) {
				this.DOM.currentFile.value = target;
				this._saveFile(target, content, true);
			}

			if (this.DOM.currentFile.value == target) this.changeSourceFile();
		}
	}, {
		key: 'zip',
		value: function zip() {
			var _this5 = this;

			var zip = new _jszipMin2.default();
			var source = this.source.data;

			for (var name in source) {
				zip.file(name, source[name]);
			}zip.generateAsync({ type: "blob" }).then(function (content) {

				if (!_this5.saver) {

					var dom = new _dryDom2.default(_this5.DOM.create("div", {
						id: "el",
						className: "FileSaver",
						onclick: function onclick(_) {
							return _this5.saver.el.style.display = "none";
						}
					}, [["a", {
						id: "zip",
						textContent: "ZIP",
						attr: {
							download: "ArduboyProject.zip"
						}
					}], ["a", {
						id: "hex",
						textContent: "HEX",
						attr: {
							download: "ArduboyProject.hex"
						}
					}]], document.body));
					_this5.saver = dom.index("id");
				} else URL.revokeObjectURL(_this5.saver.href);

				_this5.saver.zip.href = URL.createObjectURL(content);
				_this5.saver.hex.href = URL.createObjectURL(new Blob([_this5.source.getItem(["build.hex"])], { type: "text/x-hex" }));
				_this5.saver.el.style.display = "block";
			});
		}
	}, {
		key: 'importZipSourceFiles',
		value: function importZipSourceFiles(z) {

			for (var k in z.files) {
				if (/.*\.(h|hpp|c|cpp|ino)$/i.test(k)) {
					addFile.call(this, k);
				}
			}

			function addFile(name) {
				var _this6 = this;

				z.file(name).async("text").then(function (txt) {

					if (txt.charCodeAt(0) == 0xFEFF) txt = txt.substr(1);

					_this6.addNewFile(name.replace(/\\/g, "/"), txt);
				}).catch(function (err) {
					console.error(err.toString());
					_this6.source.setItem([name], "// ERROR LOADING: " + err);
				});
			}
		}
	}, {
		key: 'onDropFile',
		value: function onDropFile(dom, event) {

			var dt = event.dataTransfer;
			var files = dt.files;

			event.stopPropagation();
			event.preventDefault();

			for (var i = 0; i < files.length; i++) {
				var file = files[i];
				if (/.*\.(png|jpg|gif|bmp)$/i.test(file.name)) loadImageFile.call(this, file);
				if (/.*\.zip$/i.test(file.name)) loadZipFile.call(this, file);
				if (/.*\.(cpp|ino|h|hpp)$/i.test(file.name)) loadSourceFile.call(this, file);
			}

			this.changeSourceFile();

			function loadSourceFile(file) {
				var _this7 = this;

				var fr = new FileReader();
				fr.onload = function (evt) {

					var txt = fr.result,
					    name = file.name;

					if (txt.charCodeAt(0) == 0xFEFF) txt = txt.substr(1);

					_this7.addNewFile(name.replace(/\\/g, "/"), txt);
				};
				fr.readAsText(file);
			}

			function loadZipFile(file) {
				var _this8 = this;

				var fr = new FileReader();
				fr.onload = function (evt) {

					_jszipMin2.default.loadAsync(fr.result).then(function (z) {
						return _this8.importZipSourceFiles(z);
					});
				};
				fr.readAsArrayBuffer(file);
			}

			function loadImageFile(file) {
				var _this9 = this;

				var fr = new FileReader();
				fr.onload = function (evt) {

					var cleanName = file.name.replace(/^.*?([^\/\\.]+)\..+$/, '$1');
					cleanName = cleanName.replace(/^([0-9])/, "_$1");

					var img = _dryDom2.default.create("img", {
						src: fr.result,
						onload: function onload(_) {

							var src = (0, _image2.default)(img, cleanName, /.*\.png$/i.test(file.name));

							var bmpcpp = _this9.source.getItem(["bmp.cpp"], "#include <Arduino.h>\n#include \"bmp.h\"\n");
							var hasHeader = false;
							var headerPath = "bmp/" + cleanName + ".h";

							bmpcpp.replace(/#include\s+"([^"]+)"/g, function (_, inc) {
								hasHeader = hasHeader || inc == headerPath;
								return "";
							});

							if (!hasHeader) bmpcpp += "\n#include \"" + headerPath + "\"\n";

							_this9.source.setItem(["bmp.cpp"], bmpcpp);
							_this9._saveFile("bmp.cpp", bmpcpp);

							var bmph = _this9.source.getItem(["bmp.h"], "");
							var hasExtern = false;

							bmph.replace(/extern\s+const\s+unsigned\s+char\s+\s+([^\[\s\[]+)/g, function (_, inc) {
								hasExtern = hasExtern || inc == cleanName;
							});

							if (!hasExtern) bmph = src.h + bmph;

							_this9.source.setItem(["bmp.h"], bmph);
							_this9._saveFile("bmp.h", bmph);

							_this9.addNewFile(headerPath, src.cpp);
						}

					});
				};
				fr.readAsDataURL(file);
			}
		}
	}, {
		key: 'changeBreakpoints',
		value: function changeBreakpoints() {

			this.code.session.clearBreakpoints();
			if (typeof core == "undefined") return;

			var paused = null,
			    currentFile = this.DOM.currentFile.value;
			for (var addr in core.breakpoints) {

				if (addr in this.srcmap && this.srcmap[addr].file == currentFile && core.breakpoints[addr]) {

					var c = "unconditional";
					if (addr == this.currentPC) {
						c += " paused";
						paused = true;
					}
					this.code.session.setBreakpoint(this.srcmap[addr].line - 1, c);
				}
			}

			if (!paused && this.srcmap[this.currentPC] && this.srcmap[this.currentPC].file == currentFile) {
				this.code.session.setBreakpoint(this.srcmap[this.currentPC].line - 1, "paused");
			}
		}
	}, {
		key: 'showFuzzyFinder',
		value: function showFuzzyFinder() {
			this.DOM.currentFile.style.display = "none";
			this.DOM.fuzzyContainer.style.display = "block";
			this.DOM.fuzzy.focus();
			this.DOM.fuzzy.setSelectionRange(0, this.DOM.fuzzy.value.length);
			this.updateFuzzyFind();
		}
	}, {
		key: 'escapeRegex',
		value: function escapeRegex(str) {
			return str.replace(/[|\\{}()\[\]^$+*?.]/g, '\\$&');
		}
	}, {
		key: 'updateFuzzyFind',
		value: function updateFuzzyFind() {

			var matches = void 0,
			    THIS = this;
			var str = this.DOM.fuzzy.value.trim().replace();

			if (str.length > 1) matches = fuzzy(str, Object.keys(this.source.data));else matches = [];

			this.model.setItem("ram.fuzzy", matches.sort(function (a, b) {
				return a.rank - b.rank;
			}).map(function (a) {
				return a.match;
			}));

			function fuzzy(str, args) {

				if (str === void 0) str = '';
				if (args === void 0) args = [];

				var escaped = THIS.escapeRegex(str);
				var regex = new RegExp(escaped.split(/(\\.|)/).filter(function (x) {
					return x.length;
				}).join('(.{0,3})'), "i");
				var length = str.length;

				return args.reduce(function (acc, possibleMatch) {
					var result = regex.exec(possibleMatch),
					    obj;

					while (result) {
						var err = 0;
						for (var i = 1; i < result.length; ++i) {
							err += result[i].length;
						}

						if (!obj) {
							obj = {
								match: possibleMatch,
								rank: err
							};
							acc.push(obj);
						} else if (obj.rank > err) obj.rank = err;

						possibleMatch = possibleMatch.substr(result.index + 1);
						result = regex.exec(possibleMatch);
					}
					return acc;
				}, []);
			}
		}
	}, {
		key: 'cancelFuzzyFind',
		value: function cancelFuzzyFind(dom, evt) {
			var _this10 = this;

			if (evt) return setTimeout(function (_) {
				return _this10.cancelFuzzyFind();
			}, 10);

			this.DOM.fuzzyContainer.style.display = "none";
			this.DOM.currentFile.style.display = "";
			this.code.focus();
		}
	}, {
		key: 'endFuzzyFind',
		value: function endFuzzyFind(dom, evt) {
			var _this11 = this;

			var results = this.model.getItem("ram.fuzzy", []);
			var result = null;

			if (evt) {
				if (evt.type == "keydown") {

					if (evt.key == "Escape") return this.cancelFuzzyFind();else if (evt.key != "Enter") return;else if (!results[0] && this.DOM.fuzzy.value.trim().length) {
						result = this.DOM.fuzzy.value.trim();
						this.addNewFile(result, "");
					}

					evt.preventDefault();
					evt.stopPropagation();
				} else if (evt.target.textContent in this.source.data) result = evt.target.textContent;
			}

			if (!result && results.length) result = results[0];

			if (result) {
				this.DOM.currentFile.value = result;
				setTimeout(function (_) {
					return _this11.changeSourceFile();
				}, 10);
			}

			this.cancelFuzzyFind();
		}
	}, {
		key: 'changeSourceFile',
		value: function changeSourceFile() {
			if (!this.code) return;
			this.disableCommits = true;
			try {
				this.code.setValue(this.source.getItem([this.DOM.currentFile.value], ""));
			} catch (ex) {
				throw ex;
			} finally {
				this.disableCommits = false;
			}
			this.changeBreakpoints();
		}
	}, {
		key: 'unmangle',
		value: function unmangle(str) {
			var end = 0,
			    sub = [];

			if (str[0] != "_" || str[1] != "Z") return str;
			str = str.substr(2);
			var srcstr = str;

			str = chunk(str);

			if (end < srcstr.length) {
				var a = args(srcstr.substr(end));
				str += "(" + a.join(", ") + ")";
			}

			return str;

			function chunk(str, noDict) {
				var m = str.match(/(L)?(N)?(Z)?([0-9]+)?/);
				if (!m) {
					end = str.length;
					return str;
				}
				var len = parseInt(m[4]);
				if (m[2]) {
					var acc = chunk(str.substr(1));
					var p = 1 + end;
					acc += "::" + chunk(str.substr(p), true);
					end += p + 1;
					return acc;
				} else if (m[3]) {
					var func = chunk(str.substr(1));
					m = str.match(/3__c_?([0-9A-Z]+)$/);
					var idx = 0;
					if (m) idx = parseInt(idx, 36) + 1;

					func = "Local String #" + idx + " in " + func;
					return func;
				} else if (m[4]) {
					var start = (m[1] ? 1 : 0) + m[4].length;
					end = start + len;
					var ret = str.substr(start, len);

					if (!noDict) sub.push(ret);

					return ret;
				} else {
					end = str.length;
					return str;
				}
			}
			//*
			function args(str) {
				var pos = 0,
				    acc = [];
				var nextPostfix = "",
				    nextPrefix = "";

				while (pos < str.length) {
					var c = {
						v: "void",
						w: "wchar_t",
						b: "bool",
						c: "char",
						a: "signed char",
						h: "unsigned char",
						s: "short",
						t: "unsigned short",
						i: "int",
						j: "unsigned int",
						l: "long",
						m: "unsigned long",
						x: "long long",
						y: "unsigned long long",
						n: "__int128",
						o: "unsigned __int128",
						f: "float",
						d: "double",
						e: "long double",
						g: "float128",
						z: "ellipsis"
					}[str[pos]] || str[pos];

					if (c == "P") {
						nextPostfix += "*";
					} else if (c == "R") {
						nextPostfix += "&";
					} else if (c == "K") {
						nextPrefix += "const ";
					} else if (c == "S") {
						var idx = void 0;
						if (str[pos + 1] == "_") {
							pos++;
							idx = 0;
						} else if (/[0-9A-Z]/.test(str[pos + 1])) {
							idx = parseInt(str.substr(pos + 1, 36)) + 1;
						} else {
							idx = "";
							pos += 2;
							acc.push("?");
							continue;
						}
						c = sub[idx];

						while (str[pos] != "_" && pos++ < str.length) {}
						acc.push(nextPrefix + c + nextPostfix);
						nextPrefix = nextPostfix = "";
					} else if (c == ".") {
						pos += str.length;
					} else {

						if (/[0-9]/.test(c)) {
							c = chunk(str.substr(pos));
							pos += end;
						}
						if (nextPrefix) {
							sub.push(nextPrefix + c);
						}
						if (nextPostfix) {
							sub.push(nextPrefix + c + nextPostfix);
						}
						acc.push(nextPrefix + c + nextPostfix);
						nextPrefix = nextPostfix = "";
					}
					pos++;
				}

				return acc;
			}
			/* */
		}
	}, {
		key: 'initSpacebar',
		value: function initSpacebar(txt) {
			var _this12 = this;

			var blockSizes = {};

			var prevBlock = null;
			var maxaddr = 28672;

			txt.replace(/\n([0-9a-f]{8,8})\s+<([^>]+)>:\s+([^\(:]+\()?/g, function (m, addr, name, func, index) {
				addr = parseInt(addr, 16);
				if (prevBlock) {
					prevBlock.end = addr;
					prevBlock.bytes = prevBlock.end - prevBlock.begin;
					prevBlock.endIndex = index;
				}

				var r = Math.random() * 100 + 155 | 0,
				    g = Math.random() * 100 + 155 | 0,
				    b = Math.random() * 100 + 155 | 0;

				prevBlock = {
					name: _this12.unmangle(name),
					index: index,
					endIndex: 0,
					deps: {},
					dependants: 0,
					isFunc: func !== undefined,
					begin: addr,
					end: addr,
					size: null,
					bytes: 0,
					color: 'rgb(' + r + ',' + g + ',' + b + ')',
					anticolor: 'rgb(' + (255 - r) + ',' + (255 - g) + ',' + (255 - b) + ')'
				};

				// name = this.unmangle(name); // .replace(/_ZL?[0-9]+(.*)v/, "$1");

				blockSizes[name] = prevBlock;

				return "";
			});

			var tinyBlock = {
				size: null,
				bytes: 0,
				color: "black",
				anticolor: "white",
				name: "Tiny (<0.5%)"
			};
			var freeBlock = {
				size: null,
				bytes: maxaddr - prevBlock.end,
				color: "white",
				anticolor: "black",
				name: "Free"
			};

			var sum = 0;

			for (var k in blockSizes) {
				var block = blockSizes[k];
				var blockCode = txt.substr(block.index, block.endIndex - block.index);

				var lines = {},
				    prevLine = null,
				    prevAddr,
				    accSize = 0;

				blockCode.replace(/(?:\n|^)([\/a-zA-Z0-9_.]+):([0-9]+)\n\s+([0-9a-f]{4,4}:)/gi, function (m, file, line, addr) {
					addr = parseInt(addr, 16);
					file = file.replace(/^\/app\/public\/builds\/[0-9]+\/sketch\//, '');
					var curLine = file + ':' + line;

					if (!(curLine in lines)) {
						lines[curLine] = {
							line: curLine,
							size: 0
						};
					}

					if (prevLine) prevLine.size += addr - prevAddr;

					prevLine = lines[curLine];
					prevAddr = addr;
				});

				if (prevLine) prevLine.size += block.end - prevAddr;

				block.lines = Object.keys(lines).map(function (l) {
					return lines[l];
				});

				blockCode.replace(/\s+call\s+[.+\-x0-9a-f]+\s+;\s+0x[0-9a-f]+\s+<([^>]+)>/gi, function (m, dep) {
					if (!blockSizes[dep]) {
						// console.error("Dependency not found: " + dep);
						return;
					}
					var count = block.deps[dep] || 0;
					block.deps[dep] = count + 1;
					blockSizes[dep].dependants++;
				});
			}
			/*
   this.source.setItem(["dependencies.txt"],
   		    Object.keys(blockSizes)
   		    .filter( a => blockSizes[a].isFunc )
   		    .sort( (a, b)=> blockSizes[b].bytes - blockSizes[a].bytes )
   		    .reduce((acc, key)=>{
   			let block = blockSizes[key];
   			return acc + block.name +
   			    " " + [
   				block.bytes + "b:",
   				'Internal:',
   				    ...block.lines.sort( (a,b)=> b.size - a.size ).map( l => `\t\t${l.line}\t${l.size}b` ),
   				'References:',
   				    ...Object.keys(block.deps).sort( (a, b) => {
   					let wa = blockSizes[a].bytes * (block.deps[a] / blockSizes[a].dependants);
   					let wb = blockSizes[b].bytes * (block.deps[b] / blockSizes[b].dependants);
   					return wb - wa;
   				    })
   				    .map( n => `\t\t${this.unmangle(n)} ${blockSizes[n].bytes}b ${block.deps[n]} of ${blockSizes[n].dependants}` )
   			    ].join("\n") + "\n\n"
   		    }, ""));
   */

			for (var k in blockSizes) {
				var block = blockSizes[k];

				var _size = block.bytes / maxaddr * 100;
				if (_size < 0.5) {
					delete blockSizes[k];
					tinyBlock.bytes += block.bytes;
					continue;
				}
				_size = Math.round(_size);
				sum += _size;
				block.size = _size + "%";
			}

			var size = Math.round(freeBlock.bytes / maxaddr * 100);
			sum += size;
			freeBlock.size = size + "%";
			if (freeBlock.bytes < 200) freeBlock.anticolor = "red";

			size = Math.round(tinyBlock.bytes / maxaddr * 100);
			sum += size;

			if (sum > 100) size -= sum - 100;
			tinyBlock.size = size + "%";

			blockSizes[tinyBlock.name] = tinyBlock;
			blockSizes[freeBlock.name] = freeBlock;

			this.model.setItem("ram.blocksizes", blockSizes);
		}
	}, {
		key: 'initHints',
		value: function initHints(txt) {
			var _this13 = this;

			var source = this.source.data;
			this.srcmap = [];
			this.rsrcmap = {};

			this.initSpacebar(txt);

			var lsp = this.model.getItem("ram.localSourcePath", "");
			var lbp = this.model.getItem("ram.localBuildPath", "");
			var root = lsp || /^\/app\/builds\/[0-9]+\//;
			var sketchExp = void 0;

			if (lsp) {
				sketchExp = new RegExp(this.escapeRegex(lbp) + this.escapeRegex(path.sep) + "sketch" + this.escapeRegex(path.sep) + "(.*)");
			} else sketchExp = /^\/app\/public\/builds\/[0-9]+\/sketch\/(.*)/;

			txt.replace(/\n([\/a-zA-Z0-9._\- ]+):([0-9]+)\n([\s\S]+?)(?=$|\n[0-9a-f]+ <[^>]+>:|\n(?:[\/a-zA-Z0-9._\-<> ]+:[0-9]+\n))/g, function (m, file, line, code) {
				if (!lsp) file = file.replace(root, '');

				file = file.replace(sketchExp, function (match, name) {
					name = name.replace(/\\/g, '/');
					for (var candidate in source) {
						var key = candidate;
						candidate = '/' + candidate.replace(/\\/g, '/');
						if (candidate.substr(candidate.length - name.length - 1) == "/" + name) {
							return key;
						}
					}
				});

				if (!(file in source)) return '';

				code = '\n' + code;
				var pos = 0;
				code.replace(/(?:[\s\S]*?\n)\s+([0-9a-f]+):\t[ a-f0-9]+\t(?:[^\n\r]+)/g, function (m, addr) {

					addr = parseInt(addr, 16) >> 1;

					if (!pos) _this13.rsrcmap[file + ":" + line] = addr;

					_this13.srcmap[addr] = { file: file, line: line, offset: pos++ };

					return '';
				});

				return '';
			});

			this.hints = {};
			txt = txt.replace(/\n([0-9a-f]+)\s+(<[^>]+>:)(?:\n\s+[0-9a-f]+:[^\n]+|\n+\s+\.\.\.[^\n]*)+/g, function (txt, addr, lbl) {
				_this13.hints[parseInt(addr, 16) >> 1] = lbl.trim();
				return '';
			});

			var oldttAddr = this.ttAddr;
			this.ramIndex = {};

			txt.replace(/([\s\S]*?\n)\s+([0-9a-f]+):\t[ a-f0-9]+\t([^\n\r]+)/g, function (txt, before, addr, after) {
				_this13.hints[parseInt(addr, 16) >> 1] = (before + after).trim();
				after.replace(/\s+;\s+0x8([0-9a-f]{5,5})\s<([^>]+)>/g, function (m, addr, name) {
					var startAddr = parseInt(addr, 16);
					var endAddr = startAddr + 1;
					name = name.replace(/^(.*?)\+0x([0-9a-f]+)$/, function (m, name, off) {
						off = parseInt(off, 16);
						startAddr -= off;
						return name;
					});
					if (startAddr < 0 || name == "__bss_end") return;

					var offset = 0;

					for (addr = startAddr; addr < endAddr; addr++) {

						var offName = name + (offset ? "+0x" + offset.toString(16) : "");
						offset++;

						_this13.ramIndex[offName] = addr;

						offName = "@ " + offName;

						if (_this13.ramComments[addr] && (_this13.ramComments[addr][0] != "@" || _this13.ramComments[addr] == offName)) {
							continue;
						}

						_this13.ttAddr = addr;
						_this13.setTTComment(offName);
					}
				});
				return '';
			});

			this.ttAddr = oldttAddr;
		}
	}, {
		key: 'commit',
		value: function commit(force) {

			if (this.disableCommits) return;

			var code = this.code.getValue();

			var old = this.source.getItem([this.DOM.currentFile.value]);

			this.source.setItem([this.DOM.currentFile.value], code);

			if (force || old !== code) this._saveFile(this.DOM.currentFile.value, code, force);
		}
	}, {
		key: '_saveFile',
		value: function _saveFile(name, code, force) {
			var lsp = this.model.getItem("ram.localSourcePath", "");

			if (!lsp || !fs) return;

			var filePath = path.resolve(lsp, name);

			if (this.saveHandles[filePath]) clearTimeout(this.saveHandles[filePath]);

			if (force) write.call(this);else this.saveHandles[filePath] = setTimeout(write.bind(this), 10000);

			function write() {
				this.pool.call("saveFile", filePath, code);
			}
		}
	}, {
		key: 'initQRCGen',
		value: function initQRCGen() {
			if (typeof QRCode == "undefined") {
				self.QRCode = false;
				_dryDom2.default.create("script", { src: "qrcode.min.js" }, document.head);
			}
		}
	}, {
		key: 'updateQRCode',
		value: function updateQRCode(url) {
			var _this14 = this;

			this.initQRCGen();

			if (!self.QRCode) return;

			url = url.replace(/^https?:/i, "arduboy:");

			if (!this.qrcode) {

				this.qrcode = new QRCode(this.DOM.qrcContainer, {
					text: url,
					correctLevel: QRCode.CorrectLevel.L
				});
			} else {

				this.qrcode.clear();
				this.qrcode.makeCode(url);
			}

			this.DOM.qrc.style.display = "inline";

			if (this.qrcClearTH) clearTimeout(this.qrcClearTH);

			this.qrcClearTH = setTimeout(function (_) {

				_this14.qrcode.clear();
				_this14.DOM.qrc.style.display = "none";
				if (_this14.DOM.element.getAttribute("data-tab") == "qr") _this14.DOM.element.setAttribute("data-tab", "source");
			}, 50000);
		}
	}, {
		key: 'compile',
		value: function compile() {
			var _this15 = this;

			if (this.DOM.compile.style.display == "none") return;

			this.commit(true);

			var src = {},
			    main = null,
			    base = null;
			for (var key in this.source.data) {
				if (/.*\.(?:hpp|h|c|cpp|ino)$/i.test(key)) src[key] = this.source.data[key];
				if (/.*\.ino$/i.test(key)) {
					var mainParts = key.split(/[/\\]/);
					var parentDir = mainParts.pop();
					if (!main || (parentDir == key || parentDir + "-master" == key) && base.length < mainParts.length) {
						main = key;
						base = mainParts;
					}
				}
			}

			if (!main) return alert("Project does not contain an ino file");

			this.DOM.compile.style.display = "none";

			this.initQRCGen();
			this.compiler.build(src, main).then(function (data) {

				_this15.DOM.compile.style.display = "initial";
				_this15.model.removeItem("app.AT32u4");

				if (data.path) _this15.updateQRCode(data.path);

				if (data.disassembly) {
					_this15.initHints(data.disassembly);
					_this15.source.setItem(["disassembly.s"], data.disassembly);
				}

				_this15.model.setItem("app.AT32u4.hex", data.hex);
				_this15.source.setItem(["build.hex"], data.hex);
				_this15.pool.call("loadFlash");
			}).catch(function (err) {

				if (typeof err != "string") err = err.toString();

				err.split("\n").forEach(function (p) {
					return core.history.push(p);
				});

				_this15.DOM.element.setAttribute("data-tab", "history");
				_this15.refreshHistory();
				_this15.DOM.compile.style.display = "initial";
			});
		}
	}, {
		key: 'refreshRAM',
		value: function refreshRAM() {
			var _this16 = this;

			if (this.DOM.autoRefreshRAM.checked && this.DOM.element.getAttribute("data-tab") == "ram") {
				if (this.refreshRAMHND) clearTimeout(this.refreshRAMHND);
				this.refreshRAMHND = setTimeout(function (_) {
					return _this16.refreshRAM();
				}, 1000);
			}

			var src = core.memory;

			while (this.RAM.length > src.length) {
				this.DOM.RAM.removeChild(this.RAM.pop());
			}var oldttAddr = this.ttAddr;
			while (this.RAM.length < src.length) {

				this.ttAddr = this.RAM.length;
				this.RAM.push(this.DOM.create("li", this.DOM.RAM, {
					title: "0x" + this.RAM.length.toString(16).padStart(4, "0")
				}));

				if (this.ramComments[this.ttAddr]) this.setTTComment(this.ramComments[this.ttAddr]);
			}

			this.ttAddr = oldttAddr;

			this.RAM.forEach(function (li, idx) {
				li.textContent = src[idx].toString(16).padStart(2, "0");
				if (idx >= self.core.minStack) li.classList.add("stack");else li.classList.remove("stack");
			});
		}
	}, {
		key: 'openRAMTT',
		value: function openRAMTT(_, evt) {
			var tt = this.DOM.RAMTT;

			var addr = parseInt(evt.target.title, 16) || 0;

			this.ttAddr = addr;

			Object.assign(tt.style, {
				top: evt.target.offsetTop + "px",
				left: evt.target.offsetLeft + "px",
				display: "block"
			});

			this.DOM.RAMTTvalue.value = core.memory[addr].toString(16).padStart(2, "0");
			this.DOM.RAMTTread.checked = !!core.readBreakpoints[addr];
			this.DOM.RAMTTwrite.checked = !!core.writeBreakpoints[addr];
			this.DOM.comment.value = this.ramComments[addr] || "";
			this.DOM.RAMTTaddr.textContent = "0x" + addr.toString(16).padStart(4, "0");
		}
	}, {
		key: 'toggleRAMReadBP',
		value: function toggleRAMReadBP() {
			var addr = this.ttAddr || 0;
			core.readBreakpoints[addr] = !core.readBreakpoints[addr];
			this.updateRAMColor();
		}
	}, {
		key: 'toggleRAMWriteBP',
		value: function toggleRAMWriteBP() {
			var addr = this.ttAddr || 0;
			core.writeBreakpoints[addr] = !core.writeBreakpoints[addr];
			this.updateRAMColor();
		}
	}, {
		key: 'updateRAMColor',
		value: function updateRAMColor() {
			var color = [0, 0, 0];
			if (core.readBreakpoints[this.ttAddr]) color[0] = 255;
			if (core.writeBreakpoints[this.ttAddr]) color[1] = 255;
			if (this.ramComments[this.ttAddr]) color[2] = parseInt(this.ramComments[this.ttAddr].toLowerCase().replace(/\+.*|[^a-z0-9]+/g, ""), 36) * 16807 % 127 + 127;

			color = color.join(",");
			if (color == "0,0,0") color = '';else color = "rgba(" + color + ",0.5)";
			this.RAM[this.ttAddr].style.backgroundColor = color;
		}
	}, {
		key: 'closeRAMTT',
		value: function closeRAMTT() {
			this.DOM.RAMTT.style.display = "none";
		}
	}, {
		key: 'setTTvalue',
		value: function setTTvalue() {
			core.memory[this.ttAddr] = parseInt(this.DOM.RAMTTvalue.value.trim().replace(/^#|^0x/, ''), 16) || 0;
			this.RAM[this.ttAddr].textContent = core.memory[this.ttAddr];
		}
	}, {
		key: 'setTTComment',
		value: function setTTComment(value) {
			if (typeof value !== "string") value = this.DOM.comment.value.trim();

			if (!this.RAM || !this.RAM.length) this.refreshRAM(true);

			if (!this.RAM[this.ttAddr || 0]) return;

			this.ramComments[this.ttAddr] = value;

			this.RAM[this.ttAddr || 0].title = "0x" + this.ttAddr.toString(16).padStart(4, "0") + " " + value;
			this.updateRAMColor();
		}
	}, {
		key: 'refreshState',
		value: function refreshState(ignoreAuto) {
			var _this17 = this;

			if (this.DOM.autoRefreshState.checked && this.DOM.element.getAttribute("data-tab") == "state") {
				if (this.refreshStateHND) clearTimeout(this.refreshStateHND);
				this.refreshStateHND = setTimeout(function (_) {
					return _this17.refreshState();
				}, 500);
			}

			var src = core.state().replace(/\t/g, "    ").replace(/ /g, "&nbsp;").split("\n");

			while (this.state.length > src.length) {
				this.DOM.state.removeChild(this.state.shift());
			}while (this.state.length < src.length) {
				this.state.push(this.DOM.create("li", this.DOM.state, [["code"]]));
			}this.state.forEach(function (li, idx) {
				li.children[0].innerHTML = src[idx];
			});
		}
	}, {
		key: 'refreshDa',
		value: function refreshDa() {
			var _this18 = this;

			this.refreshState(true);
			var pc = this.currentPC;

			var addr = parseInt(this.DOM.daAddress.value.replace(/^.*[x#]/, ""), 16) | 0;
			this.DOM.daAddress.value = addr.toString(16).padStart(4, "0");

			var src = core.da(addr, 50) /*.replace(/\t/g, "    ").replace(/ /g, "&nbsp;")*/.split("\n");

			while (this.da.length > src.length) {
				this.DOM.da.removeChild(this.da.shift());
			}while (this.da.length < src.length) {
				var el = this.DOM.create("li", this.DOM.da, [["pre", { className: "opContainer" }, [["div", { className: "breakpoint" }], ["code", { className: "op" }]]], ["pre", { className: "commentContainer" }, [["code", { className: "comment" }]]]], {
					onclick: function onclick(evt) {
						return _this18.onClickDAItem(evt.currentTarget);
					}
				});
				el.dom = new _dryDom2.default(el).index(["id", "className"]);
				this.da.push(el);
			}

			this.da.forEach(function (li, idx) {

				var addr = parseInt(src[idx].replace(/&nbsp;/g, ''), 16) >> 1;

				li.address = addr;

				if (core.breakpoints[addr]) li.setAttribute('breakpoint', 'true');else li.setAttribute('breakpoint', 'false');

				if (addr === pc) li.setAttribute('pc', 'true');else li.setAttribute('pc', 'false');

				var srcparts = src[idx].split(';');
				li.dom.op.textContent = srcparts.shift();

				var hint = _this18.hints[addr];
				if (hint) {
					li.dom.comment.textContent = hint;
				} else {
					li.dom.comment.textContent = srcparts.join(';');
				}
			});
		}
	}, {
		key: 'onHitBreakpoint',
		value: function onHitBreakpoint(pc) {
			this.currentPC = pc;
			var srcref = this.srcmap[pc];

			if (srcref && srcref.offset && !(pc in core.breakpoints || pc in core.readBreakpoints || pc in core.writeBreakpoints) && this.DOM.element.getAttribute("data-tab") == "source") {
				this.reqStep();
				return;
			}

			this.DOM.daAddress.value = (Math.max(pc - 5, 0) << 1).toString(16);
			this.refreshDa();

			if (srcref && this.source.getItem([srcref.file])) {
				this.DOM.currentFile.value = srcref.file;
				this.changeSourceFile();
				this.code.scrollToLine(srcref.line, true, true, function (_) {});
				this.code.gotoLine(srcref.line, 0, true);
			}

			if (srcref && !srcref.offset && this.source.getItem([srcref.file])) {
				this.DOM.element.setAttribute("data-tab", "source");
			} else {
				this.DOM.element.setAttribute("data-tab", "da");
			}
			this.DOM.element.setAttribute("paused", "true");
		}
	}, {
		key: 'onScrollDA',
		value: function onScrollDA(DOM, evt) {
			var off = (evt.deltaY > 0 ? -2 : 2) * 4;
			this.DOM.daAddress.value = Math.max(0, parseInt(this.DOM.daAddress.value, 16) - off).toString(16);
			this.refreshDa();
		}
	}, {
		key: 'onClickDAItem',
		value: function onClickDAItem(item) {
			var addr = item.address || 0;
			if (item.getAttribute("breakpoint") !== "true") {
				item.setAttribute("breakpoint", "true");

				core.breakpoints[addr] = function (pc, sp) {
					return true;
				};

				core.enableDebugger();
			} else {

				item.setAttribute("breakpoint", "false");
				core.breakpoints[addr] = null;
			}
		}
	}, {
		key: 'reqReset',
		value: function reqReset() {
			this.pool.call("reset");
		}
	}, {
		key: 'reqResume',
		value: function reqResume() {
			this.DOM.element.setAttribute("paused", "false");
			this.pool.call("resume");
		}
	}, {
		key: 'reqStep',
		value: function reqStep() {
			this.pool.call("step");
		}
	}, {
		key: 'pollForPeriferals',
		value: function pollForPeriferals(periferals) {
			var _this19 = this;

			var serial0OUTBuffer = '';

			periferals.push({
				logger: {
					connect: "cpu.0",
					init: function init(_) {
						return serial0OUTBuffer = '';
					},
					serial0: function serial0(v) {

						var str = String.fromCharCode(v);
						// str = (str || "").replace(/\r/g,'');
						serial0OUTBuffer += str;

						var br = serial0OUTBuffer.indexOf("\n");
						if (br != -1) {

							var parts = serial0OUTBuffer.split("\n");
							while (parts.length > 1) {
								self.core.history.push(parts.shift().replace(/\r/g, ''));
							}_this19.refreshHistory();

							serial0OUTBuffer = parts[0];
						}
					}
				}
			});
		}
	}, {
		key: 'refreshHistory',
		value: function refreshHistory() {
			var _this20 = this;

			while (core.history.length > this.history.length) {
				this.history.push(this.DOM.create("li", this.DOM.history, {
					onclick: function onclick(evt) {
						var m = evt.target.dataset.text.match(/^#([0-9a-f]{4,})\s?.*$/);
						if (m) {
							_this20.DOM.element.setAttribute("data-tab", "da");
							_this20.DOM.daAddress.value = m[1];
							_this20.refreshDa();
						}
					}
				}));
			}while (this.history.length > core.history.length) {
				this.DOM.history.removeChild(this.history.shift());
			}this.history.forEach(function (li, idx) {
				if (li.dataset.text != core.history[idx]) li.setAttribute("data-text", core.history[idx]);
			});

			this.DOM.history.scrollTop = this.DOM.history.scrollHeight - this.DOM.history.clientHeight;
		}
	}]);

	return Debugger;
}();

Debugger["@inject"] = {
	pool: "pool",
	compiler: "Compiler",
	model: [_mvc.Model, { scope: "root" }],
	store: _IStore2.default
};
;

exports.default = Debugger;

},{"../lib/dry-dom.js":31,"../lib/image.js":34,"../lib/mvc.js":35,"../store/IStore.js":47,"jszip/dist/jszip.min.js":5}],21:[function(require,module,exports){
"use strict";

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var LED = function LED(DOM) {
			_classCallCheck(this, LED);

			this.on = {

						connect: null,

						onLowToHigh: function onLowToHigh() {
									this.el.style.opacity = "0";
						},
						onHighToLow: function onHighToLow() {
									this.el.style.opacity = "1";
						}
			};


			this.el = DOM.element;
			DOM.element.controller = this;
			DOM.element.dispatchEvent(new Event("addperiferal", { bubbles: true }));
			this.on.connect = DOM.element.getAttribute("pin-on");
			this.el.style.opacity = 0;
};

module.exports = LED;

},{}],22:[function(require,module,exports){
"use strict";

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _gif1b = require("../lib/gif1b.js");

var _gif1b2 = _interopRequireDefault(_gif1b);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var SCREEN = function () {
			function SCREEN(DOM) {
						var _this = this;

						_classCallCheck(this, SCREEN);

						this.state = function (data) {
									// console.log( "DATA: " + data.toString(16) );
									var cs = this.colStart;
									var ce = this.colEnd;
									var cd = ce - cs;
									var ps = this.pageStart;
									var pe = this.pageEnd;
									var pd = pe - ps;

									var x = cs + this.col;
									var y = (ps + this.page) * 8;
									var fbdata = this.fb.data;
									var iOffset = (y * 128 + x) * 4;
									var inverted = this.inverted;

									if (!this.fade) {

												for (var i = 0; i < 8; ++i, iOffset += 128 * 4) {
															var offset = iOffset; // ((y+i)*128 + x) * 4;
															var bit = (data >>> i & 1 ^ inverted) * 0xFF;
															// fbdata[ offset++ ] = bit;
															// fbdata[ offset++ ] = bit;
															// fbdata[ offset++ ] = bit;
															fbdata[offset + 3] = bit;
												}
									} else {

												for (var _i = 0; _i < 8; ++_i, iOffset += 128 * 4) {
															var _offset = iOffset; // ((y+i)*128 + x) * 4;
															var _bit = (data >>> _i & 1 ^ inverted) * 0xFF;
															if (!_bit) _bit = fbdata[_offset + 3] >> 1;
															fbdata[_offset + 3] = _bit;
												}
									}

									this.vblank = false;
									this.col++;
									if (this.col > cd) {
												this.col = 0;
												this.page++;
												if (this.page > pd) {
															this.vblank = true;
															this.page = 0;
												}
									}

									this.dirty = true;
						};

						this.sck = {
									connect: null
						};
						this.sda = {
									connect: null,
									init: function init() {
												this.reset();
									},
									MOSI: function MOSI(data) {

												if (this.mode == 0) {
															// data is a command
															var cmd = "cmd" + data.toString(16).toUpperCase();
															if (this.cmd.length) {
																		this.cmd.push(data);
																		cmd = this.cmd[0];
															} else this.cmd.push(cmd);

															var fnc = this[cmd];

															if (!fnc) return console.warn("Unknown SSD1306 command: " + cmd.toString(16));

															if (fnc.length == this.cmd.length - 1) {
																		this.cmd.shift();
																		this[cmd].apply(this, this.cmd);
																		this.cmd.length = 0;
															}
												} else {
															this.state(data);
												}
									}
						};
						this.res = {
									connect: null,
									onLowToHigh: function onLowToHigh() {
												this.reset();
									}
						};
						this.dc = {
									connect: null,
									onLowToHigh: function onLowToHigh() {
												this.mode = 1; // data
									},
									onHighToLow: function onHighToLow() {
												this.mode = 0; // command
									}

									// Display Off
						};

						this.DOM = DOM;

						var canvas = this.canvas = DOM.screen;
						if (!canvas) throw "No canvas in Arduboy element";

						this.pool.add(this);

						canvas.width = 128;
						canvas.height = 64;

						this.ctx = canvas.getContext("2d");
						this.ctx.imageSmoothingEnabled = false;
						this.ctx.msImageSmoothingEnabled = false;

						this.gif = null;
						this.isRecording = false;
						this.recordingSkip = 0;
						this.saver = null;

						this.fb = this.createBuffer();
						this.fbON = this.createBuffer();
						this.fbOFF = this.createBuffer();
						this.activeBuffer = this.createBuffer(); // this.fbON;
						this.dirty = true;
						this.fade = false;

						this.fbON.data.fill(0xFF);

						DOM.element.controller = this;
						DOM.element.dispatchEvent(new Event("addperiferal", { bubbles: true }));

						this.sck.connect = DOM.element.getAttribute("pin-sck");
						this.sda.connect = DOM.element.getAttribute("pin-sda");
						this.res.connect = DOM.element.getAttribute("pin-res");
						this.dc.connect = DOM.element.getAttribute("pin-dc");

						var _loop = function _loop(i) {
									var cmd = "cmd" + i.toString(16).toUpperCase();
									if (_this[cmd]) return "continue";
									_this[cmd] = function () {
												console.log("SSD1306 stub command " + i.toString(16));
									}.bind(null, i);
						};

						for (var i = 0; i < 255; ++i) {
									var _ret = _loop(i);

									if (_ret === "continue") continue;
						}

						this.reset();
			}

			_createClass(SCREEN, [{
						key: "CRTFade",
						value: function CRTFade(enabled) {
									this.fade = enabled;
						}
			}, {
						key: "toggleCRTFade",
						value: function toggleCRTFade() {
									this.fade = !this.fade;
						}
			}, {
						key: "toggleGIFRecording",
						value: function toggleGIFRecording() {

									var gif = this.gif1b;
									if (!gif) {
												this.lastFrameTime = performance.now();

												if (!this.blinker) this.blinker = this.DOM.create("span", {
															className: "RecordingIndicator"
												}, document.body);

												/* * /
            (this.gif = new GIF({
            workerScript:"gif.worker.js",
            width: this.canvas.width,
            height: this.canvas.height
            })).on('finished', (blob, data) => this._onFinishedRecording(data) );
            /*/
												this.gif1b = new _gif1b2.default();
												/* */
									}

									if (this.isRecording) {
												this.blinker.style.display = "none";
												/* * /
            this.gif.render();
            /*/
												this._onFinishedRecording1b(this.gif1b.write());
												/* */
									} else {
												this.blinker.style.display = "block";
												if (this.saver) this.saver.style.display = "none";
									}

									this.isRecording = !this.isRecording;
						}
			}, {
						key: "_onFinishedRecording",
						value: function _onFinishedRecording(data) {
									var _this2 = this;

									if (!this.saver) {

												this.saver = this.DOM.create("a", {
															className: "FileSaver",
															textContent: "GIF",
															attr: { download: "ArduboyRecording.gif" },
															onclick: function onclick(_) {
																		return _this2.saver.style.display = "none";
															}
												}, document.body);
									} else URL.revokeObjectURL(this.saver.href);

									this.saver.href = URL.createObjectURL(new Blob([data.buffer], { type: 'image/gif' }));
									this.saver.style.display = "block";
									this.gif = null;
						}
			}, {
						key: "_onFinishedRecording1b",
						value: function _onFinishedRecording1b(data) {
									var _this3 = this;

									if (!this.saver1b) {

												this.saver1b = this.DOM.create("a", {
															className: "FileSaver",
															textContent: "GIF",
															attr: { download: "ArduboyRecording.gif" },
															onclick: function onclick(_) {
																		return _this3.saver1b.style.display = "none";
															}
												}, document.body);
									} else URL.revokeObjectURL(this.saver1b.href);

									this.saver1b.href = URL.createObjectURL(new Blob([data.buffer], { type: 'image/gif' }));
									this.saver1b.style.display = "block";
									this.gif1b = null;
						}
			}, {
						key: "setActiveView",
						value: function setActiveView() {
									this.pool.remove(this);
						}
			}, {
						key: "onPressF2",
						value: function onPressF2() {
									if (this.canvas.width == 128) {
												this.canvas.width = 256;
												this.canvas.height = 128;
												this.activeBuffer = this.createBuffer();
												this.activeBuffer.data.fill(0xFF);
									} else {
												this.canvas.width = 128;
												this.canvas.height = 64;
												this.activeBuffer = this.createBuffer();
												this.activeBuffer.data.fill(0xFF);
									}
						}
			}, {
						key: "onPressKeyP",
						value: function onPressKeyP() {
									var c = document.createElement("canvas");
									c.width = this.canvas.width * 4;
									c.height = this.canvas.height * 4;
									c.setAttribute('style', "\n  image-rendering: -moz-crisp-edges;\n  image-rendering: -webkit-crisp-edges;\n  image-rendering: pixelated;\n  image-rendering: crisp-edges;\n        ");
									var ctx = c.getContext("2d");
									ctx.imageSmoothingEnabled = false;
									ctx.msImageSmoothingEnabled = false;
									ctx.fillStyle = "black";
									ctx.fillRect(0, 0, c.width, c.height);
									ctx.drawImage(this.canvas, 0, 0, c.width, c.height);
									var url = c.toDataURL("image/png");
									// window.open( url );

									var saver = this.DOM.create("a", {
												href: url,
												className: "FileSaver",
												textContent: "PNG",
												attr: { download: "ArduboyCapture.png" },
												onclick: function onclick(_) {
															return document.body.removeChild(saver);
												}
									}, document.body);
						}
			}, {
						key: "onPressKeyR",
						value: function onPressKeyR() {
									this.toggleGIFRecording();
						}
			}, {
						key: "onPressKeyF",
						value: function onPressKeyF() {
									var docEl = this.canvas; // doc.documentElement;

									toggleFullScreen();

									return;

									function isFullScreen() {
												var doc = window.document;
												return doc.fullscreenElement || doc.mozFullScreenElement || doc.webkitFullscreenElement || doc.msFullscreenElement || false;
									}

									function toggleFullScreen(toggle) {
												var doc = window.document;

												var requestFullScreen = docEl.requestFullscreen || docEl.mozRequestFullScreen || docEl.webkitRequestFullScreen || docEl.msRequestFullscreen;
												var cancelFullScreen = doc.exitFullscreen || doc.mozCancelFullScreen || doc.webkitExitFullscreen || doc.msExitFullscreen;
												var state = isFullScreen();

												if (toggle == undefined) toggle = !state;else if (toggle == state) return;

												if (toggle) requestFullScreen.call(docEl);else cancelFullScreen.call(doc);
									}
						}
			}, {
						key: "blit",
						value: function blit(src, auto) {
									this.autoBlit = auto;

									if (src.data.length == this.activeBuffer.data.length) {
												this.activeBuffer.data.set(src.data);
												return;
									}

									var inp = src.data,
									    out = this.activeBuffer.data;

									for (var y = 0; y < src.height; ++y) {
												for (var x = 0; x < src.width; ++x) {

															var Pi = y * src.width + x;
															var P = inp[Pi * 4 + 3] || 0;
															var A = inp[Pi * 4 - src.width * 4 + 3] || 0;
															var B = inp[Pi * 4 + 4 + 3] || 0;
															var C = inp[Pi * 4 - 4 + 3] || 0;
															var D = inp[Pi * 4 + src.width * 4 + 3] || 0;

															var O1 = P,
															    O2 = P,
															    O3 = P,
															    O4 = P;

															if (C == A && C != D && A != B) O1 = A;
															if (A == B && A != C && B != D) O2 = B;
															if (D == C && D != B && C != A) O3 = C;
															if (B == D && B != A && D != C) O4 = D;

															var Di = (y << 1) * (src.width << 1) + (x << 1) << 2;
															out[Di + 3] = O1;
															out[Di + 4 + 3] = O2;
															out[Di + (src.width << 3) + 3] = O3;
															out[Di + (src.width << 3) + 4 + 3] = O4;
												}
									}
						}
			}, {
						key: "tick",
						value: function tick() {
									if (!this.dirty) //|| (this.activeBuffer == this.fb && !(this.col == 0 && this.page == 0) ) )
												return;

									if (this.autoBlit) {

												if (this.fb.data.length != this.activeBuffer.data.length) {
															this.blit(this.fb, true);
															this.ctx.putImageData(this.activeBuffer, 0, 0);
												} else {
															this.ctx.putImageData(this.fb, 0, 0);
												}
									} else {

												this.ctx.putImageData(this.activeBuffer, 0, 0);
									}

									this.dirty = false;

									var now = performance.now();
									var delay = now - this.lastFrameTime;
									if (this.isRecording && this.recordingSkip-- <= 0) {
												this.recordingSkip = 2;
												this.lastFrameTime = now;
												/* * /
            this.gif.addFrame( this.ctx, { copy:true, delay:delay/10 } );
            //*/
												if (this.fb.data.length != this.activeBuffer.data.length) {
															this.gif1b.add(this.activeBuffer, delay);
												} else {
															this.gif1b.add(this.fb, delay);
												}
												/* */
									}
						}
			}, {
						key: "createBuffer",
						value: function createBuffer() {
									var canvas = this.canvas;
									try {
												return new ImageData(new Uint8ClampedArray(canvas.width * canvas.height * 4), canvas.width, canvas.height);
									} catch (e) {
												return this.ctx.createImageData(canvas.width, canvas.height);
									}
						}
			}, {
						key: "reset",
						value: function reset() {
									this.mode = 0;
									this.clockDivisor = 0x80;
									this.cmd = [];
									this.colStart = 0;
									this.colEnd = 127;
									this.pageStart = 0;
									this.pageEnd = 7;
									this.col = 0;
									this.page = 0;
									this.fb.data.fill(0xFF);
									this.inverted = 0;
									this.dirty = true;
									this.autoBlit = true;
						}
			}, {
						key: "cmdAE",
						value: function cmdAE() {
									this.blit(this.fbOFF, 0);
									this.dirty = true;
						}

						// Set Display Clock Divisor v = 0xF0

			}, {
						key: "cmdD5",
						value: function cmdD5(v) {
									this.clockDivisor = v;
						}

						// Charge Pump Setting v = enable (0x14)

			}, {
						key: "cmd8D",
						value: function cmd8D(v) {
									this.chargePumpEnabled = v;
						}

						// Set Segment Re-map (A0) | (b0001)

			}, {
						key: "cmdA0",
						value: function cmdA0() {
									this.segmentRemap = 0;
						}
			}, {
						key: "cmdA1",
						value: function cmdA1() {
									this.segmentRemap = 1;
						}
			}, {
						key: "cmdA5",
						value: function cmdA5() {
									this.blit(this.fbON, 0);
									this.dirty = true;
						}
			}, {
						key: "cmd0",
						value: function cmd0() {
									this.colStart = this.colStart & 0xF0 | 0;
						}
			}, {
						key: "cmd1",
						value: function cmd1() {
									this.colStart = this.colStart & 0xF0 | 0x1;
						}
			}, {
						key: "cmd2",
						value: function cmd2() {
									this.colStart = this.colStart & 0xF0 | 0x2;
						}
			}, {
						key: "cmd3",
						value: function cmd3() {
									this.colStart = this.colStart & 0xF0 | 0x3;
						}
			}, {
						key: "cmd4",
						value: function cmd4() {
									this.colStart = this.colStart & 0xF0 | 0x4;
						}
			}, {
						key: "cmd5",
						value: function cmd5() {
									this.colStart = this.colStart & 0xF0 | 0x5;
						}
			}, {
						key: "cmd6",
						value: function cmd6() {
									this.colStart = this.colStart & 0xF0 | 0x6;
						}
			}, {
						key: "cmd7",
						value: function cmd7() {
									this.colStart = this.colStart & 0xF0 | 0x7;
						}
			}, {
						key: "cmd8",
						value: function cmd8() {
									this.colStart = this.colStart & 0xF0 | 0x8;
						}
			}, {
						key: "cmd9",
						value: function cmd9() {
									this.colStart = this.colStart & 0xF0 | 0x9;
						}
			}, {
						key: "cmdA",
						value: function cmdA() {
									this.colStart = this.colStart & 0xF0 | 0xA;
						}
			}, {
						key: "cmdB",
						value: function cmdB() {
									this.colStart = this.colStart & 0xF0 | 0xB;
						}
			}, {
						key: "cmdC",
						value: function cmdC() {
									this.colStart = this.colStart & 0xF0 | 0xC;
						}
			}, {
						key: "cmdD",
						value: function cmdD() {
									this.colStart = this.colStart & 0xF0 | 0xD;
						}
			}, {
						key: "cmdE",
						value: function cmdE() {
									this.colStart = this.colStart & 0xF0 | 0xE;
						}
			}, {
						key: "cmdF",
						value: function cmdF() {
									this.colStart = this.colStart & 0xF0 | 0xF;
						}
			}, {
						key: "cmd10",
						value: function cmd10() {
									this.colStart = this.colStart & 0x0F;
						}
			}, {
						key: "cmd11",
						value: function cmd11() {
									this.colStart = 0x1 << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd12",
						value: function cmd12() {
									this.colStart = 0x2 << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd13",
						value: function cmd13() {
									this.colStart = 0x3 << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd14",
						value: function cmd14() {
									this.colStart = 0x4 << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd15",
						value: function cmd15() {
									this.colStart = 0x5 << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd16",
						value: function cmd16() {
									this.colStart = 0x6 << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd17",
						value: function cmd17() {
									this.colStart = 0x7 << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd18",
						value: function cmd18() {
									this.colStart = 0x8 << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd19",
						value: function cmd19() {
									this.colStart = 0x9 << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd1A",
						value: function cmd1A() {
									this.colStart = 0xA << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd1B",
						value: function cmd1B() {
									this.colStart = 0xB << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd1C",
						value: function cmd1C() {
									this.colStart = 0xC << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd1D",
						value: function cmd1D() {
									this.colStart = 0xD << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd1E",
						value: function cmd1E() {
									this.colStart = 0xE << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmd1F",
						value: function cmd1F() {
									this.colStart = 0xF << 4 | this.colStart & 0x0F;
						}
			}, {
						key: "cmdB0",
						value: function cmdB0() {
									this.page = 0;
						}
			}, {
						key: "cmdB1",
						value: function cmdB1() {
									this.page = 1;
						}
			}, {
						key: "cmdB2",
						value: function cmdB2() {
									this.page = 2;
						}
			}, {
						key: "cmdB3",
						value: function cmdB3() {
									this.page = 3;
						}
			}, {
						key: "cmdB4",
						value: function cmdB4() {
									this.page = 4;
						}
			}, {
						key: "cmdB5",
						value: function cmdB5() {
									this.page = 5;
						}
			}, {
						key: "cmdB6",
						value: function cmdB6() {
									this.page = 6;
						}
			}, {
						key: "cmdB7",
						value: function cmdB7() {
									this.page = 7;
						}

						// Set COM Output Scan Direction

			}, {
						key: "cmdC8",
						value: function cmdC8() {
									console.log("set com output scan direction");
						}

						// Set COM Pins v

			}, {
						key: "cmdDA",
						value: function cmdDA(v) {
									console.log("set com pins " + v.toString(16));
						}

						// Set Contrast v = 0xCF

			}, {
						key: "cmd81",
						value: function cmd81(v) {
									// this.contrast = v;
									this.canvas.style.opacity = v / 255;
									// console.log("set contrast " + v.toString(16) );
						}

						// Set Precharge = 0xF1

			}, {
						key: "cmdD9",
						value: function cmdD9(v) {
									console.log("set precharge " + v.toString(16));
						}

						// Set VCom Detect

			}, {
						key: "cmdDB",
						value: function cmdDB(v) {
									console.log("set vcom detect");
						}

						// Entire Display ON

			}, {
						key: "cmdA4",
						value: function cmdA4() {
									this.autoBlit = 1;
									this.dirty = true;
						}
			}, {
						key: "_invert",
						value: function _invert() {
									this.dirty = true;
									var fbdata = this.fb.data;
									for (var i = 3; i < fbdata.length; i += 4) {
												fbdata[i] ^= 0xFF;
									}
						}

						// Set normal/inverse display

			}, {
						key: "cmdA6",
						value: function cmdA6() {
									if (!this.inverted) return;
									this.inverted = 0;
									this._invert();
									/*
         this.canvas.style.filter = "";
         this.canvas.style.background = "";
         */
						}
			}, {
						key: "cmdA7",
						value: function cmdA7() {
									if (this.inverted) return;
									this.inverted = 1;
									this._invert();
									// this.canvas.style.filter = "invert(100%)";
									// this.canvas.style.background = "black";
						}

						// Display On

			}, {
						key: "cmdAF",
						value: function cmdAF() {
									this.autoBlit = 1;
						}

						// set display mode = horizontal addressing mode (0x00)

			}, {
						key: "cmd20",
						value: function cmd20(v) {
									console.log("set display mode " + v.toString(16));
						}

						// set col address range

			}, {
						key: "cmd21",
						value: function cmd21(v, e) {
									this.colStart = v;
									this.colEnd = e;
									this.col = 0;
						}

						// set page address range

			}, {
						key: "cmd22",
						value: function cmd22(v, e) {
									this.pageStart = v;
									this.pageEnd = e;
									this.page = 0;
									this.vblank = true;
						}
			}]);

			return SCREEN;
}();

SCREEN["@inject"] = {
			pool: "pool"
};


module.exports = SCREEN;

},{"../lib/gif1b.js":33}],23:[function(require,module,exports){
"use strict";

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var SOUND = function () {
	function SOUND(DOM) {
		_classCallCheck(this, SOUND);

		this.ch1 = {
			connect: null,
			onLowToHigh: function onLowToHigh(tick) {
				tick = tick * this.cycles >>> 0;
				var it = this.bufferStart,
				    ch1 = this.ch1Buffer;
				var delta = tick - this.previousTick;
				this.prevDelta = delta;
				this.previousTick = tick;

				if (it >= ch1.length) {
					this.error = 0;
					return;
				}

				while (this.error-- > 0 && it < ch1.length) {
					ch1[it++] = 0.5;
				}var max = this.ch1Buffer.length - it;

				if (delta > max && delta < max + ch1.length) this.error = delta - max;else this.error = 0;
				delta = Math.min(max, delta);

				for (; delta; delta--) {
					ch1[it++] = 0;
				}ch1[it] = 0.5;
				this.bufferStart = it;
			},

			onHighToLow: function onHighToLow(tick) {
				tick = tick * this.cycles >>> 0;
				var it = this.bufferStart,
				    ch1 = this.ch1Buffer;
				var delta = tick - this.previousTick;
				this.prevDelta = delta;
				this.previousTick = tick;

				if (it >= ch1.length) {
					this.error = 0;
					return;
				}

				while (this.error-- > 0 && it < ch1.length) {
					ch1[it++] = 0;
				}var max = this.ch1Buffer.length - it;

				if (delta > max && delta < max + ch1.length) this.error = delta - max;else this.error = 0;
				delta = Math.min(max, delta);

				for (; delta; delta--) {
					ch1[it++] = 0.5;
				}ch1[it] = 0;
				this.bufferStart = it;
			}
		};
		this.ch2 = {
			connect: null,
			onLowToHigh: function onLowToHigh(tick) {
				tick = tick * this.cycles >>> 0;
				var it = this.bufferStart2,
				    ch1 = this.ch2Buffer;
				var delta = tick - this.previousTick2;
				this.prevDelta2 = delta;
				this.previousTick2 = tick;

				if (it >= ch1.length) {
					this.error2 = 0;
					return;
				}

				while (this.error2-- > 0 && it < ch1.length) {
					ch1[it++] = 0.25;
				}var max = this.ch2Buffer.length - it;

				if (delta > max && delta < max + ch1.length) this.error2 = delta - max;else this.error2 = 0;
				delta = Math.min(max, delta);

				for (; delta; delta--) {
					ch1[it++] = 0;
				}ch1[it] = 0.25;
				this.bufferStart2 = it;
			},

			onHighToLow: function onHighToLow(tick) {
				tick = tick * this.cycles >>> 0;
				var it = this.bufferStart2,
				    ch1 = this.ch2Buffer;
				var delta = tick - this.previousTick2;
				this.prevDelta2 = delta;
				this.previousTick2 = tick;

				if (it >= ch1.length) {
					this.error2 = 0;
					return;
				}

				while (this.error2-- > 0 && it < ch1.length) {
					ch1[it++] = 0;
				}var max = this.ch2Buffer.length - it;

				if (delta > max && delta < max + ch1.length) this.error2 = delta - max;else this.error2 = 0;
				delta = Math.min(max, delta);

				for (; delta; delta--) {
					ch1[it++] = 0.25;
				}ch1[it] = 0;
				this.bufferStart2 = it;
			}
		};

		this.DOM = DOM;

		if (typeof AudioContext == "undefined") {
			if (typeof webkitAudioContext == "undefined") return;
			window.AudioContext = webkitAudioContext;
		}

		this.pool.add(this);

		this.ctx = SOUND.context || new AudioContext();
		this.processor = null;
		var size = 4 * 1024;
		this.ch1Buffer = new Float32Array(size);
		this.ch2Buffer = new Float32Array(size);
		this.previousTick = 0;
		this.cycles = this.ctx.sampleRate / (16 * 1000 * 1000);
		this.bufferStart = 0;
		this.bufferEnd = 0;
		this.bufferStart2 = 0;
		this.bufferEnd2 = 0;
		this.prevDelta = 0;
		this.prevDelta2 = 0;
		this.error = 0;
		this.error2 = 0;

		DOM.element.controller = this;
		DOM.element.dispatchEvent(new Event("addperiferal", { bubbles: true }));

		this.ch1.connect = DOM.element.getAttribute("pin-ch1");
		this.ch2.connect = DOM.element.getAttribute("pin-ch2");

		this.enableSound();
	}

	_createClass(SOUND, [{
		key: "enableSound",
		value: function enableSound() {
			var _this = this;

			if (this.processor) return;

			this.splitter = this.ctx.createChannelSplitter(2);
			this.merger = this.ctx.createChannelMerger(2);
			this.splitter.connect(this.merger, 0, 0);
			this.splitter.connect(this.merger, 1, 0);
			this.splitter.connect(this.merger, 0, 1);
			this.splitter.connect(this.merger, 1, 1);
			this.merger.connect(this.ctx.destination);

			this.processor = this.ctx.createScriptProcessor(this.ch1Buffer.length, 0, 2);
			this.processor.onaudioprocess = function (evt) {

				var ch = evt.outputBuffer.getChannelData(0);

				if (_this.bufferStart) {
					ch.set(_this.ch1Buffer);

					var v = ch[_this.bufferStart],
					    d = _this.prevDelta;
					for (var i = _this.bufferStart + 1; i < ch.length; ++i) {

						ch[i] = v;
						d--;
						if (d == 0) {
							if (v) v = 0;else v = 0.5;
							d = _this.prevDelta;
						}
					}
					_this.ch1Buffer[0] = _this.ch1Buffer[_this.bufferStart];
				} else ch.fill(_this.ch1Buffer[0]);

				ch = evt.outputBuffer.getChannelData(1);

				if (_this.bufferStart2) {
					ch.set(_this.ch2Buffer);
					var _v = ch[_this.bufferStart2],
					    _d = _this.prevDelta2;
					for (var _i = _this.bufferStart2 + 1; _i < ch.length; ++_i) {

						ch[_i] = _v;
						_d--;
						if (_d == 0) {
							if (_v) _v = 0;else _v = 0.25;
							_d = _this.prevDelta2;
						}
					}
					_this.ch2Buffer[0] = _this.ch2Buffer[_this.bufferStart2];
				} else ch.fill(_this.ch2Buffer[0]);

				_this.bufferStart = 0;
				_this.bufferStart2 = 0;
			};

			this.processor.connect(this.splitter);
		}
	}, {
		key: "disableSound",
		value: function disableSound() {
			if (!this.processor) return;
			this.merger.disconnect(this.ctx.destination);
			this.processor = null;
		}
	}, {
		key: "toggleSoundEnabled",
		value: function toggleSoundEnabled() {
			if (this.processor) this.disableSound();else this.enableSound();
		}
	}, {
		key: "setActiveView",
		value: function setActiveView() {
			this.pool.remove(this);
			this.disableSound();
		}
	}, {
		key: "onPressKeyM",
		value: function onPressKeyM() {
			this.toggleSoundEnabled();
		}
	}]);

	return SOUND;
}();

SOUND["@inject"] = {
	pool: "pool"
};
SOUND.context = null;


module.exports = SOUND;

},{}],24:[function(require,module,exports){
'use strict';

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _mvc = require('../lib/mvc.js');

var _dryDi = require('dry-di');

var _Atcore = require('../atcore/Atcore.js');

var _Atcore2 = _interopRequireDefault(_Atcore);

var _Hex = require('../atcore/Hex.js');

var _Hex2 = _interopRequireDefault(_Hex);

var _Arduboy = require('./../skins/Arduboy.js');

var _Arduboy2 = _interopRequireDefault(_Arduboy);

var _Bare1x = require('./../skins/Bare1x1.js');

var _Bare1x2 = _interopRequireDefault(_Bare1x);

var _Bare2x = require('./../skins/Bare2x2.js');

var _Bare2x2 = _interopRequireDefault(_Bare2x);

var _BareFit = require('./../skins/BareFit.js');

var _BareFit2 = _interopRequireDefault(_BareFit);

var _Microcard = require('./../skins/Microcard.js');

var _Microcard2 = _interopRequireDefault(_Microcard);

var _Pipboy = require('./../skins/Pipboy.js');

var _Pipboy2 = _interopRequireDefault(_Pipboy);

var _PipboyMKIV = require('./../skins/PipboyMKIV.js');

var _PipboyMKIV2 = _interopRequireDefault(_PipboyMKIV);

var _Tama = require('./../skins/Tama.js');

var _Tama2 = _interopRequireDefault(_Tama);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var skins = {
	Arduboy: _Arduboy2.default,
	Bare1x1: _Bare1x2.default,
	Bare2x2: _Bare2x2.default,
	BareFit: _BareFit2.default,
	Microcard: _Microcard2.default,
	Pipboy: _Pipboy2.default,
	PipboyMKIV: _PipboyMKIV2.default,
	Tama: _Tama2.default
};
Object.freeze(skins);


var LOADING = 0;
var RUNNING = 1;
var PAUSED = 2;
var STEP = 3;

var emptySkin = { elements: {} },
    skinList = Object.keys(skins);

var Arduboy = function () {
	function Arduboy(DOM) {
		var _this = this;

		_classCallCheck(this, Arduboy);

		this.tick = [];


		this.pool.add(this);

		this.DOM = DOM;
		this.parent = DOM.element.parentElement;
		this.width = 0;
		this.height = 0;
		this.dead = false;
		this.skipResize = 0;
		this.firstInit = true;
		this.periferals = [];

		DOM.element.addEventListener("addperiferal", function (evt) {
			return _this.addPeriferal(evt.target.controller);
		});

		this.update = this._update.bind(this);

		this.loadFlash();

		var skin = skins[this.root.getItem("ram.skin", null) || this.root.getItem("app.skinName", "Arduboy")] || skins.Arduboy;

		this.initSkin(skin);
		this.resize();

		setTimeout(function (_) {
			return _this._update();
		}, 5);
	}

	_createClass(Arduboy, [{
		key: 'loadFlash',
		value: function loadFlash() {
			var _this2 = this;

			var preserve = null;

			if (this.core && this.core.breakpointsEnabled) {
				preserve = {
					breakpoints: core.breakpoints,
					readBreakpoints: core.readBreakpoints,
					writeBreakpoints: core.writeBreakpoints
				};
			}

			this.core = null;
			this.state = LOADING;

			var url = this.root.getItem("app.AT328P.url", null);
			if (url) {

				this.core = _Atcore2.default.ATmega328P();

				_Hex2.default.parseURL(url, this.core.flash, function (success) {
					if (success) _this2.initCore(preserve);
				});
				return;
			}

			var hex = this.root.getItem("app.AT328P.hex", null);
			if (hex) {

				this.core = _Atcore2.default.ATmega328P();
				_Hex2.default.parse(hex, this.core.flash);
				this.initCore(preserve);
				return;
			}

			url = this.root.getItem("app.AT32u4.url", null);
			if (url) {

				this.core = _Atcore2.default.ATmega32u4();
				if (!/.*\/?null$/.test(url)) {
					core.history.push("Loading hex from URL");
					_Hex2.default.parseURL(url, this.core.flash, function (success, hex) {

						if (success) {
							_this2.initCore(preserve);
							core.hex = hex;
						} else _this2.pool.call("showDebugger");
					});
				} else {
					this.pool.call("showDebugger");
				}

				return;
			}

			hex = this.root.getItem("app.AT32u4.hex", null);
			if (hex) {

				this.core = _Atcore2.default.ATmega32u4();
				try {
					_Hex2.default.parse(hex, this.core.flash);
				} catch (ex) {
					this.pool.call("showDebugger");
					return;
				}
				this.initCore(preserve);
				return;
			}

			console.error("Nothing to load");
		}
	}, {
		key: 'reset',
		value: function reset() {
			// prevent from running without a hex
			if (this.state == LOADING) return;

			var oldCore = this.core,
			    dbg = false;
			var oldCoreIF = core;
			this.core = new _Atcore2.default.ATmega32u4();
			if (oldCore) {
				this.core.flash.set(oldCore.flash);
			}
			this.initCore(oldCoreIF && {
				breakpoints: oldCoreIF.breakpoints,
				readBreakpoints: oldCoreIF.readBreakpoints,
				writeBreakpoints: oldCoreIF.writeBreakpoints
			});

			this.state = PAUSED;
		}
	}, {
		key: 'resume',
		value: function resume() {
			if (this.state == PAUSED) this.core.breakpoints.disableFirst = true;
			if (this.state != LOADING) this.state = RUNNING;
		}
	}, {
		key: 'step',
		value: function step() {
			if (this.state == PAUSED) {
				this.state = STEP;
				this.core.breakpoints.disableFirst = true;
			}
		}
	}, {
		key: 'pause',
		value: function pause() {
			if (this.state != LOADING) this.state = PAUSED;
		}
	}, {
		key: 'onPressF8',
		value: function onPressF8() {
			if (this.core.debuggerEnabled) {
				this.resume();
				return true;
			}
		}
	}, {
		key: 'onPressF6',
		value: function onPressF6() {
			this.reset();
		}
	}, {
		key: 'onPressF7',
		value: function onPressF7() {
			this.step();
			return true;
		}
	}, {
		key: 'onPressEscape',
		value: function onPressEscape() {
			if (!this.pool.call("hideFlasher")) this.powerOff();
		}
	}, {
		key: 'onPressF3',
		value: function onPressF3() {
			for (var i = 0, l = skinList.length; i < l; ++i) {
				var name = skinList[i];
				var skin = skins[name];
				if (skin == this.skin) {
					name = skinList[(i + 1) % skinList.length];
					this.root.setItem("app.skinName", name);
					this.initSkin(skins[name]);
					return true;
				}
			}
		}
	}, {
		key: 'initSkin',
		value: function initSkin(skin) {

			if (this.skin && this.skin != emptySkin && skin != emptySkin) {
				this.initSkin(emptySkin);
			}

			this.skin = skin;
			this.width = 0;
			document.body.style.background = skin.background;
			this.pool.call("CRTFade", skin.CRTFade);

			var root = this.DOM.element;

			var _loop = function _loop(qs) {
				var settings = skin.elements[qs];
				var eseqs = emptySkin.elements[qs] = emptySkin.elements[qs] || {};

				Array.from(root.querySelectorAll(qs)).forEach(function (e) {

					for (var path in settings) {

						eseqs[path] = "";

						var value = settings[path],
						    obj = e,
						    prop = void 0;

						path = path.split(".");
						prop = path.pop();

						if (path.length) {
							while (path.length) {
								obj = obj[path.shift()];
							}
						} else {
							if (prop in e.style && !(prop in e)) obj = e.style;
						}

						if (_typeof(obj[prop]) == "object" || e == obj) {
							if (value && (typeof value === 'undefined' ? 'undefined' : _typeof(value)) == "object") Object.assign(obj[prop], value);else e.setAttribute(prop, value);
						} else obj[prop] = value;
					}
				});
			};

			for (var qs in skin.elements) {
				_loop(qs);
			}

			if (!skin.remap) {
				this.pool.call("remapKey", {
					ArrowUp: "ArrowUp",
					ArrowRight: "ArrowRight",
					ArrowDown: "ArrowDown",
					ArrowLeft: "ArrowLeft"
				});
			} else {
				this.pool.call("remapKey", {
					ArrowUp: "ArrowRight",
					ArrowRight: "ArrowDown",
					ArrowDown: "ArrowLeft",
					ArrowLeft: "ArrowUp"
				});
			}

			if (skin != emptySkin) this.pool.call("onSetSkin", skin);
		}
	}, {
		key: 'arduboyMode',
		value: function arduboyMode() {
			this.initSkin(skins.Arduboy);
		}
	}, {
		key: 'microcardMode',
		value: function microcardMode() {
			this.initSkin(skins.Microcard);
		}
	}, {
		key: 'setActiveView',
		value: function setActiveView() {
			this.pool.remove(this);
		}
	}, {
		key: 'powerOff',
		value: function powerOff() {
			this.pool.remove(this);
			document.body.setAttribute("style", "");
			console.error = this._error;
			this.dead = true;
			self.core = null;
			this.DOM.element.dispatchEvent(new Event("poweroff", { bubbles: true }));
		}
	}, {
		key: 'initCore',
		value: function initCore(preserve) {
			var _this3 = this;

			if (this.firstInit) {
				this.pool.call("pollForPeriferals", this.periferals);
				this.firstInit = false;
			}
			window.onerror = function (evt) {
				self.core.history.push("ERROR: " + evt.toString());
			};
			this._error = console.error;
			console.error = function () {
				for (var _len = arguments.length, args = Array(_len), _key = 0; _key < _len; _key++) {
					args[_key] = arguments[_key];
				}

				self.core.history.push("ERROR: " + args.join(" "));
				_this3._error.apply(console, args);
			};

			this.root.setItem("ram.core", self.core);

			var core = this.core,
			    oldValues = {},
			    DDRB = void 0,
			    callbacks = {
				DDRB: {},
				DDRC: {},
				DDRD: {},
				PORTB: {},
				PORTC: {},
				PORTD: {},
				PORTE: {},
				PORTF: {}
			};

			if (preserve) {
				core.enableDebugger();
				for (var k in preserve) {
					Object.assign(core[k], preserve[k]);
				}
			}

			Object.keys(callbacks).forEach(function (k) {
				return Object.assign(callbacks[k], {
					onHighToLow: [],
					onLowToHigh: [],
					onChangePWM: []
				});
			});

			Object.defineProperties(core.pins, {

				onHighToLow: { value: function value(port, bit, cb) {
						(callbacks[port].onHighToLow[bit] = callbacks[port][bit] || []).push(cb);
					} },

				onLowToHigh: { value: function value(port, bit, cb) {
						(callbacks[port].onLowToHigh[bit] = callbacks[port][bit] || []).push(cb);
					} },

				onChangePWM: { value: function value(port, bit, cb) {
						(callbacks[port].onLowToHigh[bit] = callbacks[port][bit] || []).push(cb);
					} },

				0: { value: { out: { port: "PORTD", bit: 2 }, in: { port: "PIND", bit: 2 } } },
				1: { value: { out: { port: "PORTD", bit: 3 }, in: { port: "PIND", bit: 3 } } },
				2: { value: { out: { port: "PORTD", bit: 1 }, in: { port: "PIND", bit: 1 } } },
				3: { value: { out: { port: "PORTD", bit: 0 }, in: { port: "PIND", bit: 0 } } },
				4: { value: { out: { port: "PORTD", bit: 4 }, in: { port: "PIND", bit: 4 } } },
				30: { value: { out: { port: "PORTD", bit: 5 }, in: { port: "PIND", bit: 5 } } },
				5: { value: { out: { port: "PORTC", bit: 6 }, in: { port: "PINC", bit: 6 } } },
				13: { value: { out: { port: "PORTC", bit: 7 }, in: { port: "PINC", bit: 7 } } },
				6: { value: { out: { port: "PORTD", bit: 7 }, in: { port: "PIND", bit: 7 } } },
				7: { value: { out: { port: "PORTE", bit: 6 }, in: { port: "PINE", bit: 6 } } },
				8: { value: { out: { port: "PORTB", bit: 4 }, in: { port: "PINB", bit: 4 } } },
				9: { value: { out: { port: "PORTB", bit: 5 }, in: { port: "PINB", bit: 5 } } },
				10: { value: { out: { port: "PORTB", bit: 6 }, in: { port: "PINB", bit: 6 } } },
				11: { value: { out: { port: "PORTB", bit: 7 }, in: { port: "PINB", bit: 7 } } },

				16: { value: { out: { port: "PORTB", bit: 2 }, in: { port: "PINB", bit: 2 } } },
				14: { value: { out: { port: "PORTB", bit: 3 }, in: { port: "PINB", bit: 3 } } },
				15: { value: { out: { port: "PORTB", bit: 1 }, in: { port: "PINB", bit: 1 } } },
				17: { value: { out: { port: "PORTB", bit: 0 }, in: { port: "PINB", bit: 0 } } },

				18: { value: { out: { port: "PORTF", bit: 7 }, in: { port: "PINF", bit: 7 } } },
				A0: { value: { out: { port: "PORTF", bit: 7 }, in: { port: "PINF", bit: 7 } } },
				19: { value: { out: { port: "PORTF", bit: 6 }, in: { port: "PINF", bit: 6 } } },
				A1: { value: { out: { port: "PORTF", bit: 6 }, in: { port: "PINF", bit: 6 } } },
				20: { value: { out: { port: "PORTF", bit: 5 }, in: { port: "PINF", bit: 5 } } },
				A2: { value: { out: { port: "PORTF", bit: 5 }, in: { port: "PINF", bit: 5 } } },
				21: { value: { out: { port: "PORTF", bit: 4 }, in: { port: "PINF", bit: 4 } } },
				A3: { value: { out: { port: "PORTF", bit: 4 }, in: { port: "PINF", bit: 4 } } },

				MOSI: { value: {} },
				MISO: { value: {} },

				spiIn: {
					value: []
				},

				spiOut: {
					value: {
						listeners: [],
						push: function push(data) {
							var i = 0,
							    listeners = this.listeners,
							    l = listeners.length;
							for (; i < l; ++i) {
								listeners[i](data);
							}
						}
					}
				},

				serial0Out: {
					value: {
						listeners: [],
						push: function push(data) {
							var i = 0,
							    listeners = this.listeners,
							    l = listeners.length;
							for (; i < l; ++i) {
								listeners[i](data);
							}
						}
					}
				},

				DDRB: {
					set: setDDR.bind(null, "DDRB"),
					get: function get() {
						return oldValues.DDRB | 0;
					}
				},
				DDRC: {
					set: setDDR.bind(null, "DDRC")
				},
				DDRD: {
					set: setDDR.bind(null, "DDRD")
				},
				DDRE: {
					set: setDDR.bind(null, "DDRD")
				},
				DDRF: {
					set: setDDR.bind(null, "DDRD")
				},
				PORTB: {
					set: setPort.bind(null, "PORTB")
				},
				PORTC: {
					set: setPort.bind(null, "PORTC")
				},
				PORTD: {
					set: setPort.bind(null, "PORTD")
				},
				PORTE: {
					set: setPort.bind(null, "PORTE")
				},
				PORTF: {
					set: setPort.bind(null, "PORTF")
				}

			});

			self.pins = core.pins;

			setTimeout(function (_) {
				_this3.setupPeriferals();
				_this3.JITwarmup();
				_this3.state = RUNNING;
			}, 5);

			function setDDR(name, cur) {
				var old = oldValues[name];
				if (old === cur) return;
				oldValues[name] = cur;
			}

			function setPort(name, cur) {
				var old = oldValues[name];

				if (old === cur) return;
				var s,
				    j,
				    l,
				    lth = callbacks[name].onLowToHigh,
				    htl = callbacks[name].onHighToLow,
				    tick = core.tick;

				for (var i = 0; i < 8; ++i) {

					var ob = old >>> i & 1,
					    nb = cur >>> i & 1;
					if (lth[i] && !ob && nb) {
						for (j = 0, s = lth[i], l = s.length; j < l; ++j) {
							s[j](tick);
						}
					}
					if (htl[i] && ob && !nb) {
						for (j = 0, s = htl[i], l = s.length; j < l; ++j) {
							s[j](tick);
						}
					}
				}

				oldValues[name] = cur;
			}
		}
	}, {
		key: 'JITwarmup',
		value: function JITwarmup() {

			var core = this.core;
			var startPC = core.pc;
			core.pc = 0;

			while (core.pc < core.prog.length) {
				core.getBlock(false);
			}core.pc = startPC;
		}
	}, {
		key: 'addPeriferal',
		value: function addPeriferal(ctrl) {

			this.periferals.push(ctrl);
		}
	}, {
		key: 'setupPeriferals',
		value: function setupPeriferals() {
			var _this4 = this;

			var pins = this.core.pins;
			var map = { cpu: this.core.pins };
			this.tick = [];

			this.pool.call("onPollTickListeners", this.tick);

			this.periferals.forEach(function (ctrl) {

				if (ctrl.tick) _this4.tick.push(ctrl);

				for (var k in ctrl) {

					var v = ctrl[k];
					if (!v || !v.connect || typeof v.connect != "string") continue;

					v = ctrl[k] = Object.assign({}, v);

					var target = v.connect;
					if (typeof target == "number") target = "cpu." + target;

					var tobj = map;
					var tparts = target.split(".");
					while (tparts.length && tobj) {
						tobj = tobj[tparts.shift()];
					}if (v.MOSI) pins.spiOut.listeners.push(v.MOSI.bind(ctrl));

					if (v.serial0) pins.serial0Out.listeners.push(v.serial0.bind(ctrl));

					if (!tobj) {
						console.warn("Could not attach wire from ", k, " to ", target);
						continue;
					}

					if (v.onLowToHigh) pins.onLowToHigh(tobj.out.port, tobj.out.bit, v.onLowToHigh.bind(ctrl));

					if (v.onHighToLow) pins.onHighToLow(tobj.out.port, tobj.out.bit, v.onHighToLow.bind(ctrl));

					if (v.onChangePWM) pins.onChangePWM(tobj.out.port, tobj.out.bit, v.onChangePWM.bind(ctrl));

					var setter = function (tobj, nv) {

						if (nv) pins[tobj.in.port] |= 1 << tobj.in.bit;else pins[tobj.in.port] &= ~(1 << tobj.in.bit);
					}.bind(_this4, tobj);

					var getter = function (tobj) {
						return pins[tobj.out.port] >>> tobj.out.bit & 1;
					}.bind(_this4, tobj);

					Object.defineProperty(v, "value", {
						set: setter,
						get: getter
					});

					if (v.init) v.init.call(ctrl);
				}
			});
		}
	}, {
		key: '_update',
		value: function _update() {
			if (this.dead) return;

			requestAnimationFrame(this.update);

			switch (this.state) {
				case RUNNING:
					this.core.update();
					break;
				case STEP:
					this.core.exec((this.core.tick - this.core.endTick + 1) / this.core.clock);
					this.state = PAUSED;
					this.core.breakpointHit = true;
					break;
				default:
					this.resize();
					return;
			}

			if (this.core.breakpointHit) {
				this.core.breakpointHit = false;
				this.state = PAUSED;
				this.pool.call("onHitBreakpoint", this.core.pc);
			}

			for (var i = 0, l = this.tick.length; i < l; ++i) {
				this.tick[i].tick();
			}this.resize();
		}
	}, {
		key: 'onResizeWindow',
		value: function onResizeWindow() {
			this.skipResize = 0;
		}
	}, {
		key: 'resize',
		value: function resize() {
			if (--this.skipResize > 0) return;

			this.skipResize = 60;

			var el = this.DOM.element;
			el.parentElement.style.maxHeight = "";
			el.parentElement.style.maxWidth = "";

			var maxHeight = el.parentElement.clientHeight;
			var maxWidth = el.parentElement.clientWidth;

			if (this.width == maxWidth && this.height == maxHeight) return;

			this.width = maxWidth;
			this.height = maxHeight;

			var ratio = this.skin.width / this.skin.height;

			if (this.height * ratio >= this.width) {

				el.parentElement.style.maxWidth = el.style.width = this.width + "px";
				el.style.height = this.width / ratio + "px";
			} else {

				el.style.width = this.height * ratio + "px";
				el.parentElement.style.maxHeight = el.style.height = this.height + "px";
			}
		}
	}]);

	return Arduboy;
}();

Arduboy["@inject"] = {
	root: [_mvc.Model, { scope: "root" }],
	pool: "pool"
};


module.exports = Arduboy;

},{"../atcore/Atcore.js":13,"../atcore/Hex.js":14,"../lib/mvc.js":35,"./../skins/Arduboy.js":38,"./../skins/Bare1x1.js":39,"./../skins/Bare2x2.js":40,"./../skins/BareFit.js":41,"./../skins/Microcard.js":42,"./../skins/Pipboy.js":43,"./../skins/PipboyMKIV.js":44,"./../skins/Tama.js":45,"dry-di":3}],25:[function(require,module,exports){
"use strict";

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var Config = function Config(DOM) {
    _classCallCheck(this, Config);

    DOM.element.innerHTML = "C O N F I G";
};

module.exports = Config;

},{}],26:[function(require,module,exports){
"use strict";

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var Files = function Files(DOM) {
    _classCallCheck(this, Files);

    DOM.element.innerHTML = "C O N F I G";
};

module.exports = Files;

},{}],27:[function(require,module,exports){
"use strict";

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _mvc = require("../lib/mvc.js");

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var Market = function () {
    function Market(DOM) {
        _classCallCheck(this, Market);
    }

    _createClass(Market, [{
        key: "run",
        value: function run() {
            this.pool.call("runSim");
        }
    }]);

    return Market;
}();

Market["@inject"] = {
    root: [_mvc.Model, { scope: "root" }]
};


module.exports = Market;

},{"../lib/mvc.js":35}],28:[function(require,module,exports){
'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _IStore = require('../store/IStore.js');

var _IStore2 = _interopRequireDefault(_IStore);

var _mvc = require('../lib/mvc.js');

var _jszipMin = require('jszip/dist/jszip.min.js');

var _jszipMin2 = _interopRequireDefault(_jszipMin);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

var Env = function (_IController) {
	_inherits(Env, _IController);

	function Env() {
		_classCallCheck(this, Env);

		return _possibleConstructorReturn(this, (Env.__proto__ || Object.getPrototypeOf(Env)).apply(this, arguments));
	}

	_createClass(Env, [{
		key: 'exitSplash',
		value: function exitSplash() {
			var url = this.model.getItem("ram.autoRun");
			if (url) {
				this.play({ element: { dataset: { url: url } } });
			} else {
				this._init();
				this._show();
			}
		}
	}, {
		key: '_init',
		value: function _init() {
			var repoName = this.model.getItem("ram.currentRepo", "");
			var list = this.model.getItem("app.repoList");
			if (!repoName) repoName = Object.keys(list)[0];

			this.changeRepo({ element: { dataset: {
						key: repoName,
						url: list[repoName]
					} } });

			list = [];
			this.userGameSource.getUserGames(list);
			this.model.setItem("app.projList", list);
		}
	}, {
		key: 'exitSim',
		value: function exitSim() {
			this._init();
			this._show();
		}
	}, {
		key: 'changeRepo',
		value: function changeRepo(dom, event) {
			var _this2 = this;

			var repoName = dom.element.dataset.key;
			this.model.setItem("ram.currentRepo", repoName);

			var repoURL = dom.element.dataset.url;

			this.activateRepo(repoName);

			var proxy = this.model.getItem("proxy", "");
			repoURL = (/^https?.*/.test(repoURL) ? proxy : "") + repoURL;

			var age = new Date().valueOf() - this.model.getItem(["app", "repodata", repoName, "timestamp"], 0);

			if (age > 5 * 60 * 60 * 1000) fetch(repoURL).then(function (rsp) {
				return rsp.json();
			}).then(function (json) {
				return _this2._updateRepo(repoName, json);
			}).catch(function (err) {
				return console.warn(err);
			}); // might be offline, be subtle
		}
	}, {
		key: 'activateRepo',
		value: function activateRepo(repoName) {
			var clone = JSON.parse(JSON.stringify(this.model.getModel(["app", "repodata", repoName], true).data));
			this.model.setItem("ram.repo", clone);
		}
	}, {
		key: '_updateRepo',
		value: function _updateRepo(repoName, json) {
			if (!json || (typeof json === 'undefined' ? 'undefined' : _typeof(json)) != "object" || !Array.isArray(json.items)) {
				console.log("Repo JSON: ", json);
				return;
			}

			json.timestamp = new Date().valueOf();

			json.items.forEach(function (item) {

				item.author = item.author || "<<unknown>>";
				item.title = item.title || item.name || "Untitled";

				if ((item.banner || item.cover) && (!item.screenshots || !item.screenshots[0] || !item.screenshots[0].filename)) item.screenshots = [{ filename: item.banner || item.cover }];

				if ((item.arduboy || item.hex) && (!item.binaries || !item.binaries[0] || !item.binaries[0].filename)) item.binaries = [{ filename: item.arduboy || item.hex }];

				if (!item.sourceUrl && item.url) item.sourceUrl = item.url;
			});

			this.model.setItem(["app", "repodata", repoName], json);

			if (repoName == this.model.getItem("ram.currentRepo")) this.activateRepo(repoName);
		}
	}, {
		key: 'onDropFile',
		value: function onDropFile(dom, event) {
			var dt = event.dataTransfer;
			var files = dt.files;

			event.stopPropagation();
			event.preventDefault();

			this.model.removeItem("app.AT32u4");
			this.model.removeItem("ram.srcpath");

			for (var i = 0; i < files.length; i++) {
				var file = files[i];

				this.model.setItem('ram.srcpath', ["app", "sources", file.name]);

				if (/.*\.hex$/i.test(file.name)) return loadFileHex.call(this, file);

				if (/.*\.arduboy$/i.test(file.name)) return loadFileArduboy.call(this, file);
			}

			function loadFileHex(file) {
				var _this3 = this;

				var fr = new FileReader();
				fr.onload = function (evt) {
					_this3.model.removeItem("app.AT32u4");
					_this3.model.setItem("app.AT32u4.hex", fr.result);
					var source = _this3.model.getModel(_this3.model.getItem("ram.srcpath"), true);
					source.setItem(["build.hex"], fr.result);
					_this3.pool.call("runSim");
				};
				fr.readAsText(file);
			}

			function loadFileArduboy(file) {
				var _this4 = this;

				var fr = new FileReader();
				fr.onload = function (evt) {
					return _this4.loadArduboy(fr.result);
				};
				fr.readAsArrayBuffer(file);
			}
		}
	}, {
		key: 'embed',
		value: function embed(dom, evt) {
			if (dom && evt) this.pool.call("embed", evt.target.dataset.url);
		}
	}, {
		key: 'preview',
		value: function preview(opt) {
			var key = opt.element.dataset.key;
			var item = this.model.getModel(["ram", "repo", "items", key], false);
			item = JSON.parse(JSON.stringify(item.data));
			try {
				item.aburl = 'arduboy:' + item.binaries[0].filename;
			} catch (ex) {
				item.aburl = '';
			}
			this.model.setItem("ram.preview", item);
		}
	}, {
		key: 'upload',
		value: function upload(opt) {
			var _this5 = this;

			if (this.model.getItem("ram.hasFlasher")) this.load(opt, function (_) {
				return _this5.pool.call("doFlash");
			});else window.open(this.model.getItem("ram.preview.aburl"));
		}
	}, {
		key: 'Konami',
		value: function Konami() {
			if (!self.core) this.play({ element: { dataset: {
						title: 'Sky Knights',
						url: 'ArduboyProject.hex',
						proxy: false
					} } });
		}
	}, {
		key: 'play',
		value: function play(opt) {
			var _this6 = this;

			this.load(opt, function (_) {
				return _this6.pool.call("runSim");
			});
		}
	}, {
		key: 'load',
		value: function load(opt, cb) {
			var _this7 = this;

			var url = void 0,
			    srcurl = void 0,
			    localsrc = void 0,
			    title = void 0,
			    useProxy = true;

			if (opt.element.dataset.url) {
				url = opt.element.dataset.url;
				srcurl = opt.element.dataset.source;
				localsrc = opt.element.dataset.lsp;
				title = opt.element.dataset.title;
				useProxy = opt.element.dataset.proxy || opt.element.dataset.proxy == undefined;
				if (!title) {
					title = opt.element.querySelector && opt.element.querySelector('.gameName');
					if (title) title = title.dataset.name;
				}
			} else {
				url = this.model.getItem("ram.preview.binaries.0.filename", "");
				srcurl = this.model.getItem("ram.preview.sourceUrl");
				title = this.model.getItem("ram.preview.title");
			}

			if (title) document.title = title;

			if (!url || url == 'new') url = 'null';

			this.model.removeItem("app.AT32u4");
			this.model.removeItem("ram.srcpath");
			this.model.removeItem("ram.localSourcePath");
			this.model.setItem("ram.localSourcePath", localsrc);
			this.model.removeItem("ram.localBuildPath");
			this.model.setItem('ram.srcpath', ["app", "sources", localsrc || url]);
			var source = this.model.getModel(this.model.getItem("ram.srcpath"), true);

			var build = source.getItem(["build.hex"]);

			var finalURL = void 0,
			    proxy = "";
			if (useProxy) proxy = this.model.getItem("app.proxy");

			var github = url.match(/^https:\/\/raw.githubusercontent.com\/(.*)$/i);
			if (github && proxy) finalURL = 'https://gitcdn.xyz/repo/' + github[1];

			if (!finalURL) finalURL = proxy + url;

			if (build || url == "null") {

				if (build) this.model.setItem("app.AT32u4.hex", build);else this.model.setItem("app.AT32u4.url", url);

				cb();
			} else if (/\.arduboy$/i.test(url)) {

				var zip = null;
				fetch(finalURL).then(function (rsp) {
					return rsp.arrayBuffer();
				}).then(function (buff) {
					return _this7.loadArduboy(buff, cb);
				}).catch(function (err) {
					console.error(err);
				});
			} else {
				fetch(finalURL).then(function (rsp) {
					if (rsp.ok) return rsp.text();else if (finalURL != url && proxy) {
						return new Promise(function (ok, nok) {
							fetch(proxy + url).then(function (rsp) {
								return rsp.ok ? ok(rsp.text()) : nok("error");
							}).catch(function (ex) {
								return nok(ex);
							});
						});
					}
				}).then(function (hex) {
					source.setItem(["build.hex"], hex);
					_this7.model.setItem("app.AT32u4.hex", hex);
					cb();
				});
			}

			var ghmatch = srcurl && srcurl.match(/^(https\:\/\/(bitbucket\.org|framagit\.org|github\.com)\/[^/]+\/[^/]+).*/) || url.match(/^(https\:\/\/(bitbucket\.org|framagit\.org|github\.com)\/[^/]+\/[^/]+).*/);

			if (ghmatch) {
				srcurl = ghmatch[1] + "/archive/master.zip";
			} else if (srcurl && /.*\.(?:zip|ino)$/.test(srcurl)) {} else if (/.*\.(?:zip|ino)$/.test(url)) {
				srcurl = url;
			} else srcurl = null;

			this.model.setItem("ram.srcurl", srcurl);
		}
	}, {
		key: 'loadArduboy',
		value: function loadArduboy(buff, cb) {
			var _this8 = this;

			var zip = void 0;
			var source = this.model.getModel(this.model.getItem("ram.srcpath"), true);

			_jszipMin2.default.loadAsync(buff).then(function (z) {
				return (zip = z).file("info.json").async("text");
			}).then(function (info) {
				return zip.file(JSON.parse(fixJSON(info)).binaries[0].filename).async("text");
			}).then(function (hex) {
				source.setItem(["build.hex"], hex);
				_this8.model.setItem("app.AT32u4.hex", hex);
				cb();
			});

			function fixJSON(str) {

				if (str.charCodeAt(0) == 0xFEFF) str = str.substr(1);

				return str.replace(/\,(?!\s*?[\{\[\"\'\w])/g, '');
			}
		}
	}]);

	return Env;
}(_mvc.IController);

Env["@inject"] = {
	store: _IStore2.default,
	pool: "pool",
	viewFactory: [_mvc.IView, { controller: Env }],
	model: [_mvc.Model, { scope: "root" }],
	userGameSource: "Compiler"
};
exports.default = Env;

},{"../lib/mvc.js":35,"../store/IStore.js":47,"jszip/dist/jszip.min.js":5}],29:[function(require,module,exports){
"use strict";

Object.defineProperty(exports, "__esModule", {
    value: true
});

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _mvc = require("../lib/mvc.js");

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

var Sim = function (_IController) {
    _inherits(Sim, _IController);

    function Sim() {
        _classCallCheck(this, Sim);

        return _possibleConstructorReturn(this, (Sim.__proto__ || Object.getPrototypeOf(Sim)).apply(this, arguments));
    }

    _createClass(Sim, [{
        key: "runSim",
        value: function runSim() {
            this._show();
        }
    }, {
        key: "onEndSim",
        value: function onEndSim() {
            this.pool.call("exitSim");
        }
    }]);

    return Sim;
}(_mvc.IController);

Sim["@inject"] = {
    pool: "pool",
    viewFactory: [_mvc.IView, { controller: Sim }],
    model: [_mvc.Model, { scope: "root" }]
};
exports.default = Sim;

},{"../lib/mvc.js":35}],30:[function(require,module,exports){
"use strict";

Object.defineProperty(exports, "__esModule", {
    value: true
});

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _mvc = require("../lib/mvc.js");

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; } // import IStore from '../store/IStore.js';


var Splash = function (_IController) {
    _inherits(Splash, _IController);

    function Splash() {
        var _ref;

        var _temp, _this, _ret;

        _classCallCheck(this, Splash);

        for (var _len = arguments.length, args = Array(_len), _key = 0; _key < _len; _key++) {
            args[_key] = arguments[_key];
        }

        return _ret = (_temp = (_this = _possibleConstructorReturn(this, (_ref = Splash.__proto__ || Object.getPrototypeOf(Splash)).call.apply(_ref, [this].concat(args))), _this), _this.BODY = {
            bound: function bound(evt) {
                var target = evt.target;
            }
        }, _temp), _possibleConstructorReturn(_this, _ret);
    }

    _createClass(Splash, [{
        key: "enterSplash",
        value: function enterSplash() {
            this._show();
        }
    }]);

    return Splash;
}(_mvc.IController);

Splash["@inject"] = {
    pool: "pool",
    viewFactory: [_mvc.IView, { controller: Splash }]
};
exports.default = Splash;

},{"../lib/mvc.js":35}],31:[function(require,module,exports){
"use strict";

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

module.exports = DOM;

function DOM(element) {

    if (!element && typeof document !== "undefined" && document.body) element = document.body;

    this.element = element;

    Object.defineProperties(this, {

        length: {
            enumerable: false,

            get: function get() {
                if (!element || !element.children) return 0;
                return element.children.length;
            },

            set: function set(l) {
                while (l > element.children.length) {
                    this.pop();
                }return element.children.length;
            }

        }

    });
}

var spare = null;
function getThis(that) {

    if (!that || typeof that == "function") return spare = spare || new DOM();

    return that;
}

function prototype(obj) {

    var desc = {};
    for (var k in obj) {
        desc[k] = Object.assign({}, Object.getOwnPropertyDescriptor(obj, k), { enumerable: false });
    }

    var ret = {};
    Object.defineProperties(ret, desc);

    return ret;
}

var impl = {

    create: function create(strTagName, objProperties, arrChildren, elParent) {
        var args = Array.from(arguments);
        strTagName = objProperties = arrChildren = elParent = undefined;

        for (var i = 0, l = args.length; i < l; ++i) {
            var arg = args[i];
            if (typeof arg == "string") strTagName = arg;else if ((typeof arg === "undefined" ? "undefined" : _typeof(arg)) == "object") {
                if (Array.isArray(arg)) arrChildren = arg;else if (arg instanceof Element) elParent = arg;else objProperties = arg;
            }
        }

        if (!elParent && this.element) elParent = this.element;

        if (!strTagName) {
            if (!elParent) strTagName = "span";else strTagName = {
                table: "tr",
                tr: "td",
                select: "option",
                ul: "li",
                ol: "li",
                dl: "dt",
                optgroup: "option",
                datalist: "option"
            }[elParent.tagName] || elParent.tagName;
        }

        var element = document.createElement(strTagName);
        if (elParent) elParent.appendChild(element);

        var listener;

        for (var key in objProperties) {
            var value = objProperties[key];
            if (key == "text") element.appendChild(document.createTextNode(value));else if (key == "listener") listener = value;else if (key == "attr") {
                for (var attr in value) {
                    element.setAttribute(attr, value[attr]);
                }
            } else if (element[key] && _typeof(element[key]) == "object" && (typeof value === "undefined" ? "undefined" : _typeof(value)) == "object") Object.assign(element[key], value);else element[key] = value;
        }

        if (this.element && element.id) this[element.id] = element;

        for (i = 0, l = arrChildren && arrChildren.length; i < l; ++i) {
            this.create.apply(this, arrChildren[i].concat(element));
        }

        if (listener) new DOM(element).listen(listener);

        return element;
    },

    listen: function listen(listeners, that, prefix) {
        prefix = prefix || "";
        if (that === undefined) that = listeners;

        var THIS = getThis(this);

        var keys = Object.keys(listeners);

        THIS.forEach(function (element) {

            if (listeners[prefix + element.tagName]) bind(listeners[prefix + element.tagName], element);

            if (listeners[prefix + element.id]) bind(listeners[prefix + element.id], element);

            if (listeners[prefix + element.className]) bind(listeners[prefix + element.className], element);

            if (listeners[prefix + element.name]) bind(listeners[prefix + element.name], element);
        });

        return THIS;

        function bind(obj, element) {

            for (var event in obj) {
                var func = obj[event];
                if (!func.call) continue;
                element.addEventListener(event, that ? func.bind(that) : func);
            }
        }
    },

    shift: function shift() {

        var c = this.element.firstChild;
        if (c) this.element.removeChild(c);
        return c;
    },

    pop: function pop() {

        var c = this.element.lastChild;
        if (c) this.element.removeChild(c);
        return c;
    },

    clear: function clear() {
        while (this.element.children.length) {
            this.element.removeChild(this.element.lastChild);
        }
    },

    index: function index(keys, multiple, property) {
        var THIS = getThis(this);

        var index = Object.create(DOM.prototype);

        if (typeof keys == "string") keys = [keys];

        for (var i = 0, l = keys.length; i < l; ++i) {

            var key = keys[i];
            if (typeof key != "string") continue;

            if (!property && !multiple) {

                THIS.forEach(function (child) {
                    return child[key] !== undefined && (index[child[key]] = child);
                });
            } else if (property && !multiple) {

                THIS.forEach(function (child) {
                    if (child[property] && _typeof(child[property]) == "object" && child[property][key] !== undefined) index[child[property][key]] = child;
                });
            } else if (!property && typeof multiple == "function") {

                THIS.forEach(function (child) {
                    if (child[key] !== undefined) multiple(child[key], child);
                });
            } else if (property && typeof multiple == "function") {

                THIS.forEach(function (child) {

                    if (!child[property] || _typeof(child[property]) != "object") return;

                    var v = child[property][key];
                    if (v !== undefined) multiple(v, child);
                });
            } else if (!property && multiple) {

                THIS.forEach(function (child) {
                    if (child[key] !== undefined) {
                        if (!index[child[key]]) index[child[key]] = [child];else index[child[key]].push(child);
                    }
                });
            } else if (property && multiple) {

                THIS.forEach(function (child) {

                    if (!child[property] || _typeof(child[property]) != "object") return;

                    var v = child[property][key];
                    if (v !== undefined) {
                        if (!index[v]) index[v] = [child];else index[v].push(child);
                    }
                });
            }
        }

        return index;
    },

    forEach: function forEach(cb, element) {
        var THIS = getThis(this);

        element = element || THIS.element;

        if (!element) return;

        if (cb(element) === false) return;

        if (!element.children) return;

        for (var i = 0, l = element.children.length; i < l; ++i) {
            THIS.forEach(cb, element.children[i]);
        }
    }

};

Object.assign(DOM, impl);
DOM.prototype = prototype(impl);

},{}],32:[function(require,module,exports){
"use strict";

module.exports = function (url) {
	var cfg = { anystate: true };

	var promise = new Promise(function (ok, nok) {
		var tcfg = {};
		tcfg.ok = true;
		tcfg.status = 0;
		tcfg.text = function (_) {
			return new Promise(function (ok, nok) {
				getURL(url, function (data, code) {
					if (code == 200 || code == 0) ok(data);else nok(code);
				}, cfg);
			});
		};
		tcfg.arraybuffer = function (_) {
			cfg.arraybuffer = true;
			return tcfg.text();
		};

		ok(tcfg);
	});

	return promise;
};

function getURL(url, cb, cfg) {

	function bindec(v) {
		var r = '',
		    cc;
		for (var i = 0; i < v.length; ++i) {
			cc = v.charCodeAt(i);
			r += String.fromCharCode(cc & 0xFF);
		}
		return r;
	}

	var xhr = new XMLHttpRequest();
	cfg = cfg || {};
	if (cfg.binary) xhr.overrideMimeType("text/plain; charset=x-user-defined");
	xhr.open("GET", url, true);
	xhr.onreadystatechange = function () {
		if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status === 0 || cfg.anystate)) {
			var v = xhr.response || xhr.responseText;
			if (xhr.status == 0 && v == "" && cfg.proxy) {
				var domain = url.match(/([^:]*\/\/[^\/]+).*/);
				if (domain && domain[1].toLowerCase() != location.origin) {
					var altcfg = DOC.mergeTo({}, cfg);
					altcfg.proxy = null;
					getURL(cfg.proxy + encodeURIComponent(url), function (obj) {
						if (!obj) return;
						obj = JSON.parse(obj);
						var v = obj.contents;
						if (cfg.binary) v = bindec(v);
						cb(v);
					}, altcfg);
					return;
				}
			}

			if (cfg.binary) v = bindec(v);
			cb(v, xhr.status);
		}
	};
	xhr.send();
}

},{}],33:[function(require,module,exports){
"use strict";

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function uint8(data) {

	var len = data.length;
	var out = new Uint8Array(len);

	if (typeof data == "string") {
		for (var i = 0; i < len; ++i) {
			out[i] = data.charCodeAt(i);
		}
	} else out.set(data);

	return out;
}

function BYTE() {
	for (var _len = arguments.length, args = Array(_len), _key = 0; _key < _len; _key++) {
		args[_key] = arguments[_key];
	}

	var out = new Uint8Array(args.length);
	out.set(args);
	return out;
}

function WORD() {
	for (var _len2 = arguments.length, args = Array(_len2), _key2 = 0; _key2 < _len2; _key2++) {
		args[_key2] = arguments[_key2];
	}

	var tmp = new Uint16Array(args.length);
	tmp.set(args);
	var out = new Uint8Array(tmp.buffer);
	return out;
}

var gif1b = function () {
	function gif1b() {
		_classCallCheck(this, gif1b);
	}

	_createClass(gif1b, [{
		key: "add",
		value: function add(data, time) {

			if (!this.blocks) {
				var packed = 0x80,
				    bg = 0,
				    ratio = 0;

				this.blocks = [uint8("GIF87a"), WORD(data.width, data.height), BYTE(packed, bg, ratio, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0x21, 0xFF, 0x0B), uint8("NETSCAPE2.0"), BYTE(0x03, 0x01, 0, 0, 0)];
			}

			time = Math.round(time | 0) || 1;

			this.blocks.push(BYTE(0x21, 0xF9, 0x04, 0x04), WORD(time / 10, 0));

			this.blocks.push(BYTE(0x2C), WORD(0, 0, data.width, data.height), BYTE(0, 2));

			this.lzw(data.data, data.width);
		}
	}, {
		key: "lzw",
		value: function lzw(data, width) {
			var _this = this;

			var out;
			var CC = 4,
			    EOI = 5;
			var nc = 0,
			    op = 0,
			    opoff = 0,
			    bits = 3;
			var table, string;

			var end = function end() {
				out[0] = op - 1;
				// op++;
				_this.blocks.push(out.slice(0, op));
			};

			function init() {

				var needsCC = !!out;

				if (out) {
					// write(table[CC], 0);
					end();
				}

				out = new Uint8Array(256);

				nc = 0;
				table = [];
				string = table;
				table[0] = [,, nc++];
				table[1] = [,, nc++];
				table[2] = [];
				table[3] = [];
				table[CC] = [,, CC];
				table[EOI] = [,, EOI];
				nc = CC + 2;
				op = 0;
				opoff = 0;
				out[op++] = 0;

				write(table[CC], 0);
			}

			for (var i = 0; i < data.length; i += 4) {
				if (!out || (op > 128 || bits > 11) && !((bits + opoff) % 8)) {
					if (string) write(string, 0);
					init();
				}

				var ch = data[i + 3] > 128 ? 1 : 0;
				if (string[ch]) string = string[ch];else write(string, ch);
			}

			write(string, 0);
			op++;
			end();
			this.blocks.push(BYTE(0));

			function mask(bits) {
				return ~0 >>> 31 - (bits - 1);
			}

			function write(arr, next) {
				if (arr == table) return;

				var code = arr[2] | 0;
				// console.log( code );

				out[op] |= code << opoff;
				opoff += bits;
				while (opoff >= 8) {
					opoff -= 8;
					op++;
					out[op] = code >>> bits - opoff;
				}

				if (nc == mask(bits) + 1) bits++;

				if (arr != table[CC]) {
					arr[next] = [,, nc++];
					string = table[next];
				} else {
					string = table;
					bits = 3;
				}
			}
		}
	}, {
		key: "write",
		value: function write() {

			var sum = 1;

			for (var i = 0, l = this.blocks.length; i < l; ++i) {
				sum += this.blocks[i].length;
			}var acc = new Uint8Array(sum);
			sum = 0;
			for (var _i = 0, _l = this.blocks.length; _i < _l; ++_i) {
				acc.set(this.blocks[_i], sum);
				sum += this.blocks[_i].length;
			}

			acc[sum] = 0x3B; // trailer

			return acc;
		}
	}]);

	return gif1b;
}();

if (typeof module !== "undefined") module.exports = gif1b;

},{}],34:[function(require,module,exports){
"use strict";

var _dryDom = require("./dry-dom.js");

var _dryDom2 = _interopRequireDefault(_dryDom);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

/* Almost direct port of TEAMarg's Cabi */
// compression / decompression session state
var cs = {
   byte: 0,
   bit: 0,
   dest: "",
   src: null,
   src_pos: 0,
   out_pos: 0,
   w: 0,
   h: 0
};

// ----------------------------------------------------------------------------
// :: Compress
// ----------------------------------------------------------------------------

/*
	getcol

	pos is the index of the pixel:  0 .. w*h-1
*/
function getcol(pos) {

   var x = void 0,
       y = void 0;

   // display order

   // if (cs.src[pos/8] & (1 << (pos&7))) return 1;
   var bit = pos % 8;
   x = (pos / 8 | 0) % cs.w;
   y = ((pos / 8 | 0) / cs.w | 0) * 8 + bit;
   if (cs.src(y * cs.w + x)) return 1;
   return 0;
}

function find_rlen(pos, plen) {
   var col;
   var pos0;

   col = getcol(pos);
   pos0 = pos;

   while (getcol(pos) == col && pos < plen) {
      pos++;
   }return pos - pos0;
}

// write a bit to the stream. non-zero val means 1, otherwise 0.
function putbit(val) {

   if (val) cs.byte |= cs.bit;
   cs.bit <<= 1;
   if (cs.bit == 0x100) {
      //output byte
      if (cs.out_pos != 0) cs.dest += ",";
      if (cs.out_pos % 16 == 0) cs.dest += "\n";
      cs.dest += "0x" + cs.byte.toString(16).padStart(2, "0");

      cs.out_pos++;
      cs.bit = 0x1;
      cs.byte = 0;
   }
}

// write an n-bit (bits) number (val) to the output steam
function putval(val, bits) {

   var i;

   if (bits <= 0) return;
   for (i = 0; i < bits; i++) {
      putbit(val & 1 << i);
   }
}

// write a span length
// a string of bits encoding the number of bits needed to encode the length,
// and then the length.
function putsplen(len) {

   var blen = 1; // how bits needed to encode length
   while (1 << blen <= len) {
      blen += 2;
   } // write number of bits (1-terminated string of zeroes)
   putval(0, (blen - 1) / 2);
   putval(1, 1); // terminator
   // write length
   putval(len, blen);
}

/*
	comp

	compress plen 1-bit pixels from src to dest

*/
function compress_rle(src, w, h, prefix, suffix) {
   var pos = void 0,
       rlen = void 0,
       len = void 0;

   cs.dest = "const uint8_t  " + prefix + suffix + "[] = {";
   cs.src = src;
   cs.bit = 1;
   cs.byte = 0;
   cs.w = w;
   cs.h = h;
   cs.out_pos = 0;

   // header
   putval(w - 1, 8);
   putval(h - 1, 8);
   putval(getcol(0), 1); // first colour

   pos = 0;

   // span data

   while (pos < w * h) {
      rlen = find_rlen(pos, w * h);
      pos += rlen;
      putsplen(rlen - 1);
   }

   // pad with zeros and flush
   while (cs.bit != 0x1) {
      putbit(0);
   }cs.dest += "\n}; // " + cs.out_pos + "b vs " + w * h / 8 + "b uncompressed\n";

   return cs.out_pos; // bytes
}

// end port of Cabi

function cabi(data, cleanName, out) {
   var src = data.data;

   var white = function white(i) {
      return src[i * 4 + 3] > 128 ? (src[i * 4] + src[i * 4 + 1] + src[i * 4 + 2]) / 3 > 127 : 0;
   };
   compress_rle(white, data.width, Math.ceil(data.height / 8) * 8, cleanName, "_comp_w");
   out[cleanName + "_comp_w[]"] = cs.dest;

   var black = function black(i) {
      return src[i * 4 + 3] > 128 ? (src[i * 4] + src[i * 4 + 1] + src[i * 4 + 2]) / 3 < 127 : 0;
   };
   compress_rle(black, data.width, Math.ceil(data.height / 8) * 8, cleanName, "_comp_b");
   out[cleanName + "_comp_b[]"] = cs.dest;

   var alpha = function alpha(i) {
      return src[i * 4 + 3] > 128 ? 1 : 0;
   };
   compress_rle(alpha, data.width, Math.ceil(data.height / 8) * 8, cleanName, "_comp_a");
   out[cleanName + "_comp_a[]"] = cs.dest;
}

function loadImage(img, cleanName, isPNG) {

   var width = img.naturalWidth;

   var canvas = _dryDom2.default.create("canvas", {
      width: width,
      height: img.naturalHeight
   });

   var ctx = canvas.getContext("2d");
   ctx.drawImage(img, 0, 0);

   var data = ctx.getImageData(0, 0, canvas.width, canvas.height);

   var ret = {};

   cabi(data, cleanName, ret);

   var masksrc = "\nconst unsigned char  " + cleanName + "_mask[] = ";

   var spmasksrc = "\nconst unsigned char  " + cleanName + "_plus_mask[] = ";

   var src = '',
       ascii = "/*";

   src += "\n\nconst unsigned char  " + cleanName + "_bitmap[] = ";

   src += "{\n// width, height,\n" + width + ", " + img.naturalHeight;
   spmasksrc += "{\n// width, height,\n" + width + ", " + img.naturalHeight;
   masksrc += "{";

   var pageCount = Math.ceil(img.naturalHeight / 8);
   var currentByte = 0;

   for (var y = 0; y < data.height; ++y) {
      ascii += "\n";
      for (var x = 0; x < data.width; ++x) {

         var i = (y * data.width + x) * 4;
         var lum = (data.data[i] + data.data[i + 1] + data.data[i + 2]) / 3;
         if (data.data[i + 3] > 128) {
            if (lum > 128) ascii += "#";else ascii += " ";
         } else ascii += "-";
      }
   }

   // Read the sprite page-by-page
   for (var page = 0; page < pageCount; page++) {

      // Read the page column-by-column
      for (var column = 0; column < width; column++) {

         // Read the column into a byte
         var spriteByte = 0,
             maskByte = 0;
         for (var yPixel = 0; yPixel < 8; yPixel++) {

            var _i = ((page * 8 + yPixel) * data.width + column) * 4;
            var _lum = (data.data[_i] + data.data[_i + 1] + data.data[_i + 2]) / 3;

            if (_lum > 128) spriteByte |= 1 << yPixel;
            if (data.data[_i + 3] > 128) maskByte |= 1 << yPixel;
         }

         src += ",";
         spmasksrc += ",";

         if (currentByte % width == 0) {
            src += "\n";masksrc += "\n";
            spmasksrc += "\n";
         }

         var byte = "0x" + spriteByte.toString(16).padStart(2, "0");
         src += byte;
         spmasksrc += byte;

         if (isPNG) {
            if (currentByte != 0) masksrc += ",";
            byte = "0x" + maskByte.toString(16).padStart(2, "0");
            masksrc += byte;
         }
         spmasksrc += ", " + byte;

         currentByte++;
      }
   }
   src += "\n};\n\n";masksrc += "\n};\n\n";
   spmasksrc += "\n};\n\n";
   ascii += "\n*/\n\n";

   src += masksrc + "\n";
   src += spmasksrc + "\n";

   var headers = [cleanName + "_bitmap[]", cleanName + "_mask[]", cleanName + "_plus_mask[]"];
   for (var k in ret) {
      src += ret[k];
      headers.push(k);
   }

   return {
      cpp: ascii + src,
      h: "extern const unsigned char  " + headers.join(', ') + ";\n"
   };
}

module.exports = loadImage;

},{"./dry-dom.js":31}],35:[function(require,module,exports){
'use strict';

Object.defineProperty(exports, "__esModule", {
    value: true
});
exports.boot = exports.IController = exports.IView = exports.Model = undefined;

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _dryDi = require('dry-di');

var _strldr = require('./strldr.js');

var _strldr2 = _interopRequireDefault(_strldr);

var _IStore = require('../store/IStore.js');

var _IStore2 = _interopRequireDefault(_IStore);

var _dryDom = require('./dry-dom.js');

var _dryDom2 = _interopRequireDefault(_dryDom);

var _pool = require('./pool.js');

var _pool2 = _interopRequireDefault(_pool);

var _getURL = require('./getURL.js');

var _getURL2 = _interopRequireDefault(_getURL);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _toConsumableArray(arr) { if (Array.isArray(arr)) { for (var i = 0, arr2 = Array(arr.length); i < arr.length; i++) { arr2[i] = arr[i]; } return arr2; } else { return Array.from(arr); } }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function read(str, ctx) {

    var parts,
        i = 0;
    if (typeof str == "string") parts = str.split(".");else parts = str;

    while (i < parts.length && ctx) {
        ctx = ctx[parts[i++]];
    }return ctx;
}

function readMethod(str, ctx) {
    var _ctx;

    var parts = str.split("."),
        i = 0;

    var pctx = ctx;

    while (i < parts.length && ctx) {
        pctx = ctx;
        ctx = ctx[parts[i++]];
    }

    for (var _len = arguments.length, args = Array(_len > 2 ? _len - 2 : 0), _key = 2; _key < _len; _key++) {
        args[_key - 2] = arguments[_key];
    }

    if (ctx && typeof ctx === "function") return (_ctx = ctx).bind.apply(_ctx, [pctx].concat(args));

    return null;
}

function write(str, value, ctx) {

    var parts = str.split("."),
        i = 0;

    while (parts.length - 1 && ctx) {
        if (!(parts[i] in ctx)) ctx[parts[i]] = {};
        ctx = ctx[parts[i++]];
    }

    if (ctx) ctx[parts[i]] = value;

    return !!ctx;
}

var pending = [];
var nextModelId = 0;

var Model = function () {
    function Model() {
        var _this = this;

        _classCallCheck(this, Model);

        var listeners = {};
        var data = {};
        var children = {};
        var revChildren = {};
        var parents = {};

        Object.defineProperty(data, "__model__", { value: this, writable: false, enumerable: false });

        Object.defineProperties(this, {
            root: { value: this, enumerable: false, writable: true },
            listeners: { value: listeners, enumerable: false, writable: false },
            data: { value: data, enumerable: false, writable: true },
            children: { value: children, enumerable: false, writable: false },
            revChildren: { value: revChildren, enumerable: false, writable: false },
            parents: { value: parents, enumerable: false, writable: false },
            id: { value: ++nextModelId, enumerable: false, writable: false },
            dirty: {
                get: function get() {
                    return _this.root.__dirty;
                },
                set: function set(v) {
                    return _this.root.__dirty = v;
                }
            }
        });
    }

    _createClass(Model, [{
        key: 'store',
        value: function store() {
            var binary = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : true;

            return _strldr2.default.store(this.data, binary);
        }
    }, {
        key: 'load',
        value: function load(data) {
            var doRaise = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : true;


            if (typeof data === "string") {
                try {
                    data = JSON.parse(data);
                    data = _strldr2.default.load(data);
                } catch (ex) {}
            }

            if (data && data.buffer && data.buffer instanceof ArrayBuffer) {
                if (!(data instanceof Uint8Array)) data = new Uint8Array(data.buffer);
                data = _strldr2.default.load(data, true);
            }

            for (var k in data) {
                this.setItem([k], data[k], doRaise);
            }

            return this;
        }
    }, {
        key: 'setItem',
        value: function setItem(k, v) {
            var doRaise = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : true;


            var raiseToParents = false;
            if (k.charCodeAt) k = k.split(".");
            var prop = k.shift(),
                child;
            var data = this.data,
                children = this.children,
                revChildren = this.revChildren;

            if (k.length) {

                child = children[prop];
                if (!child) {
                    child = children[prop] = new Model();
                    child.root = this.root;
                    child.parents[this.id] = this;
                    data[prop] = child.data;
                    this.dirty = true;
                    revChildren[child.id] = [prop];
                    this.raise(prop, false);
                }

                var klength = k.length;
                var child = children[prop].setItem(k, v, klength != 1);

                if (klength == 1) this.raise(prop, doRaise);

                return child;
            }

            if (children[prop]) {

                if (children[prop].data !== v && v && (typeof v === 'undefined' ? 'undefined' : _typeof(v)) == "object") {
                    if (Array.isArray(children[prop].data) && Array.isArray(v)) {
                        var arr = children[prop].data;
                        while (arr.length > v.length) {
                            children[prop].removeItem([arr.length - 1], null, doRaise);
                        }for (var i = 0; i < v.length; ++i) {
                            if (v[i] !== arr[i]) children[prop].setItem([i], v[i], doRaise);
                        }
                    } else {
                        for (var ck in children[prop].data) {
                            if (!(ck in v)) children[prop].removeItem([ck], null, doRaise);else children[prop].setItem([ck], v[ck], doRaise);
                        }
                        for (var ck in v) {
                            if (!(ck in children[prop].data)) children[prop].setItem([ck], v[ck], doRaise);
                        }
                    }
                    return this;
                }

                child = children[prop];

                var index = revChildren[child.id].indexOf(prop);
                if (index === -1) throw new Error("Integrity compromised");

                revChildren[child.id].splice(index, 1);

                delete child.parents[this.id];
            } else raiseToParents = true;

            if (v && (typeof v === 'undefined' ? 'undefined' : _typeof(v)) == "object") {

                var doLoad = false;
                if (!v.__model__) {
                    child = new Model();
                    child.root = this.root;
                    doLoad = true;
                } else {
                    child = v.__model__;
                }

                if (!revChildren[child.id]) revChildren[child.id] = ["" + prop];else revChildren[child.id].push("" + prop);
                children[prop] = child;
                child.parents[this.id] = this;

                if (doLoad) {
                    child.load(v, false);
                    child.data = v;
                    Object.defineProperty(v, "__model__", { value: child, writable: false });
                }
            }

            data[prop] = v;

            this.dirty = true;
            this.raise(prop, doRaise);
            if (raiseToParents) {
                for (var pid in this.parents) {
                    this.parents[pid].raise(this.parents[pid].revChildren[this.id], doRaise);
                }
            }

            return this;
        }
    }, {
        key: 'getModel',
        value: function getModel(k, create) {

            if (k.charCodeAt) k = k.split(".");

            var ctx = this,
                i = 0;
            if (create) {
                while (ctx && i < k.length) {
                    if (!ctx.children[k[i]]) ctx.setItem([k[i]], {});
                    ctx = ctx.children[k[i++]];
                }
            } else {
                while (ctx && i < k.length) {
                    ctx = ctx.children[k[i++]];
                }
            }

            return ctx;
        }
    }, {
        key: 'getItem',
        value: function getItem(k, defaultValue) {
            var v = read(k, this.data);
            if (v === undefined) v = defaultValue;
            return v;
        }
    }, {
        key: 'removeItem',
        value: function removeItem(k, cb, doRaise) {

            var parent = k && k.split && k.split(".") || k;
            var key = parent.pop();

            var model = this.getModel(parent);
            var data = model.data,
                children = model.children;

            if (!(key in data)) return;

            if (children[key]) {

                var child = children[key],
                    revChildren = model.revChildren[child.id];

                var index = revChildren.indexOf("" + key);
                if (index == -1) throw "Integrity compromised";

                revChildren.splice(index, 1);

                if (revChildren.length == 0) {
                    delete child.parents[model.id];
                    delete model.revChildren[child.id];
                }

                delete children[key];
            }

            if (Array.isArray(data)) data.splice(key, 1);else delete data[key];

            model.raise(key, doRaise);
            for (var pid in this.parents) {
                this.parents[pid].raise(this.parents[pid].revChildren[this.id], doRaise);
            }
        }
    }, {
        key: 'raise',
        value: function raise(k, doRaise) {
            var _this2 = this;

            if (!pending.find(function (p) {
                return p.model == _this2 && p.key == k;
            })) pending[pending.length++] = { model: this, key: k };

            if (!doRaise) return;

            while (pending.length) {

                var p = pending.shift();
                k = p.key;
                var model = p.model;

                if (k) {

                    dispatch(model.listeners[k], model.data[k], k);
                } else {

                    for (var pid in model.parents) {

                        var parent = model.parents[pid];
                        var revChildren = parent.revChildren[model.id];
                        if (!revChildren) throw "Integrity compromised";

                        for (var j = 0, rcl = revChildren.length; j < rcl; ++j) {

                            dispatch(parent.listeners[revChildren[j]], parent.data, revChildren[j]);
                        }
                    }
                }
            }

            pending.length = 0;

            function dispatch(listeners, value, key) {

                if (!listeners) return;

                for (var i = 0, l = listeners.length; i < l; ++i) {
                    listeners[i](value, key);
                }
            }
        }

        // attach( k:String, cb:Function )
        // listen to notifications from a particular key
        // attach( cb:Function )
        // listen to key additions/removals

    }, {
        key: 'attach',
        value: function attach(k, cb) {
            var key = k.split && k.split(".") || k;
            var model;
            if (key.length == 1) {
                key = k;
                model = this;
            } else {
                k = key.pop();
                model = this.getModel(key, true);
                key.push(k);
                key = k;
            }

            if (!model.listeners[key]) model.listeners[key] = [cb];else model.listeners[key].push(cb);
        }

        // stop listening

    }, {
        key: 'detach',
        value: function detach(k, cb) {

            var index, listeners, model;

            k = k.split && k.split(".") || k;

            var key = k;

            if (key.length == 1) {
                key = k;
                model = this;
            } else {
                k = key.pop();
                model = this.getModel(key, false);
                key.push(k);
                if (!model) return;
                key = k;
            }

            listeners = model.listeners[k];
            if (!listeners[k]) return;

            index = listeners.indexOf(cb);
            if (index == -1) return;

            listeners.splice(index, 1);
        }
    }]);

    return Model;
}();

var cache = {};

var IView = function () {
    function IView(controller) {
        var _this3 = this;

        _classCallCheck(this, IView);

        var layout = "layouts/" + controller.constructor.name + ".html";
        this.controller = controller;
        this.dom = null;

        if (!cache[layout]) {

            (0, _getURL2.default)(layout).then(function (rsp) {

                if (!rsp.ok && rsp.status !== 0) throw new Error("Not OK!");
                return rsp.text();
            }).then(function (text) {
                return new window.DOMParser().parseFromString(text, "text/html");
            }).then(function (html) {
                cache[layout] = html;
                _this3.loadLayout(html);
            }).catch(function (ex) {

                _this3.parentElement.innerHTML = '<div>' + (ex.message || ex) + (': ' + layout + '!</div>') + (ex.stack || "").split("\n").map(function (e) {
                    return '<div>' + e + '</div>';
                }).join("");
            });
        } else this.loadLayout(cache[layout]);
    }

    _createClass(IView, [{
        key: 'loadLayout',
        value: function loadLayout(doc) {
            var _this4 = this;

            doc = doc.cloneNode(true);
            [].concat(_toConsumableArray(doc.body.children)).forEach(function (child) {
                return _this4.parentElement.appendChild(child);
            });

            var dom = new _dryDom2.default(this.parentElement);
            this.dom = dom;

            prepareDOM(dom, this.controller, this.model, dom);
        }
    }]);

    return IView;
}();

IView["@inject"] = {
    parentElement: "ParentElement",
    model: [Model, { scope: 'root' }]
};


function prepareDOM(dom, controller, _model, viewdom) {

    dom.forEach(function (element) {

        for (var i = 0; i < element.attributes.length; ++i) {
            var key = element.attributes[i].name;
            var value = element.attributes[i].value;

            var parts = key.replace(/-[0-9]+$/, "").split("-");

            if (parts.length == 2) switch (parts[1]) {
                case "hide":
                    bindVisibility(element, parts[0], value, "hidden");
                    break;
                case "show":
                    bindVisibility(element, parts[0], value, "visible");
                    break;

                case "call":

                    var target = readMethod(value, controller, dom);
                    if (target) element.addEventListener(parts[0], target);else console.warn("Could not bind event to " + controller.constructor.name + "." + name);

                    break;

                case "toggle":
                case "set":

                    value.split(";").forEach(function (sp) {
                        var vparts = sp.match(/^(?:([^@]+)\@)?([^=]+)\=(.*)$/);

                        if (vparts) bindAction(parts[1] == "toggle", element, parts[0], vparts);else console.warn("Could not parse " + parts[1] + ": " + value);
                    });

                    break;

            }

            var memo = { __src: value, __hnd: 0 };
            value.replace(/\{\{([^\}]+)\}\}/g, bindAttribute.bind(null, element.attributes[i], memo));
            updateAttribute(element.attributes[i], memo);
        }

        if ((element.dataset.src || element.dataset.srcIndirect) && !element.dataset.inject) {
            var attach = function attach(src) {
                if (Array.isArray(src)) src = [].concat(_toConsumableArray(src));

                if (prevattach) _model.detach(prevattach, attachcb);
                prevattach = src;

                if (Array.isArray(src)) src = [].concat(_toConsumableArray(src));
                _model.attach(src, attachcb);

                attachcb();
            };

            var indirect = element.dataset.srcIndirect,
                prevattach = void 0;

            if (indirect) {
                _model.attach(indirect, function (_) {
                    return attach(_model.getItem(indirect, ""));
                });
            }

            switch (element.tagName) {
                case 'UL':
                case 'OL':
                case 'SELECT':
                    var template = element.cloneNode(true);
                    var attachcb = function attachcb(_) {
                        return renderList(element, template, _model.getItem(indirect ? _model.getItem(indirect, "") : element.dataset.src));
                    };
                    attach(indirect ? _model.getItem(indirect, "") : element.dataset.src);
                    break;

                default:
                    break;
            }

            return false;
        }

        for (var i = 0; i < element.childNodes.length; ++i) {

            var childNode = element.childNodes[i];
            if (childNode.nodeType != XMLDocument.TEXT_NODE) continue;
            var memo = { __src: childNode.nodeValue, __hnd: 0 };
            childNode.nodeValue.replace(/\{\{([^\}]+)\}\}/g, bindNodeValue.bind(null, childNode, memo));
            updateNodeValue(childNode, memo);
        }

        if (element.dataset.inject && element != dom.element) {

            var childDom = new _dryDom2.default(element);
            Object.assign(childDom, childDom.index("id"));

            var ctrl = (0, _dryDi.getInstanceOf)(element.dataset.inject, childDom);
            dom[element.dataset.inject] = ctrl;

            prepareDOM(childDom, ctrl, _model, viewdom);

            return false;
        }
    });

    function bindVisibility(element, value, path, style) {

        if (value == "undefined") value = undefined;else {
            try {
                value = JSON.parse(value);
            } catch (ex) {}
        }
        var cb = function cb(v) {
            if (v == value) element.style.visibility = style;else if (element.style.visibility == style) element.style.visibility = "";
        };

        cb(_model.getItem(path));

        _model.attach(path, cb);
    }

    function bindAction(toggle, element, event, cmd) {

        element.addEventListener(event, function () {

            [].concat(_toConsumableArray(cmd[1] ? viewdom.element.querySelectorAll(cmd[1]) : [element])).forEach(function (target) {
                if (toggle && target.getAttribute(cmd[2]) == cmd[3]) target.removeAttribute(cmd[2]);else target.setAttribute(cmd[2], cmd[3]);
            });
        });
    }

    function renderList(element, template, arr) {

        var value = element.value;

        while (element.children.length) {
            element.removeChild(element.children[0]);
        }for (var key in arr) {

            var childModel = new Model();
            childModel.load(_model.data, false);
            childModel.setItem("key", key, false);
            childModel.setItem("value", arr[key], false);
            childModel.root = _model.root;

            [].concat(_toConsumableArray(template.cloneNode(true).children)).forEach(function (child) {

                element.appendChild(child);
                prepareDOM(new _dryDom2.default(child), controller, childModel, viewdom);
            });
        }

        element.value = value;
    }

    function bindAttribute(attr, memo, match, inner) {

        if (inner in memo) return "";

        _model.attach(inner, function (value) {
            memo[inner] = value;
            if (memo.__hnd) return;
            memo.__hnd = setTimeout(updateAttribute.bind(null, attr, memo), 1);
        });

        memo[inner] = _model.getItem(inner);

        return "";
    }

    function updateAttribute(attr, memo) {
        memo.__hnd = 0;
        attr.value = memo.__src.replace(/\{\{([^\}]+)\}\}/g, function (match, path) {
            return _typeof(memo[path]) == "object" ? JSON.stringify(memo[path]) : memo[path];
        });
    }

    function bindNodeValue(attr, memo, match, inner) {

        if (inner in memo) return "";

        _model.attach(inner, function (value) {
            memo[inner] = value;
            if (memo.__hnd) return;
            memo.__hnd = setTimeout(updateNodeValue.bind(null, attr, memo), 1);
        });

        memo[inner] = _model.getItem(inner);

        return "";
    }

    function updateNodeValue(attr, memo) {
        memo.__hnd = 0;
        attr.nodeValue = memo.__src.replace(/\{\{([^\}]+)\}\}/g, function (match, path) {
            return _typeof(memo[path]) == "object" ? JSON.stringify(memo[path]) : memo[path];
        });
    }
}

var defaultModel = null;

var IController = function () {
    function IController() {
        _classCallCheck(this, IController);

        this.pool.add(this);
    }

    _createClass(IController, [{
        key: '_show',
        value: function _show() {
            console.log("created view");
            this.pool.call("setActiveView", null);
            var view = this.viewFactory(this);
            return view;
        }
    }]);

    return IController;
}();

IController["@inject"] = {
    viewFactory: IView,
    pool: "pool",
    model: Model
};


function boot(_ref) {
    var main = _ref.main,
        element = _ref.element,
        components = _ref.components,
        entities = _ref.entities,
        model = _ref.model;


    (0, _dryDi.bind)(_pool2.default).to('pool').singleton();
    (0, _dryDi.bind)(Model).to(Model).withTags({ scope: 'root' }).singleton();

    if (model) {

        var root = (0, _dryDi.getPolicy)({
            _interface: Model,
            tags: { scope: 'root' },
            args: []
        });

        root.load(model);
    }

    for (var k in components) {
        (0, _dryDi.bind)(components[k]).to(k);
    }for (var k in entities) {
        var ctrl = entities[k];
        // console.log( "Adding entity " + k, ctrl );
        (0, _dryDi.bind)(ctrl).to(IController);
        (0, _dryDi.bind)(IView).to(IView).injecting([document.body, 'ParentElement']).withTags({ controller: ctrl }).factory();
    }

    (0, _dryDi.bind)(main).to(main).injecting([new _dryDom2.default(element), _dryDom2.default]);
    return (0, _dryDi.getInstanceOf)(main);
}

exports.Model = Model;
exports.IView = IView;
exports.IController = IController;
exports.boot = boot;

},{"../store/IStore.js":47,"./dry-dom.js":31,"./getURL.js":32,"./pool.js":36,"./strldr.js":37,"dry-di":3}],36:[function(require,module,exports){
"use strict";

var nextUID = 0;

if (!Object.getOwnPropertyDescriptors) {
    Object.getOwnPropertyDescriptors = function (value) {
        var o = {};
        Object.getOwnPropertyNames(value).forEach(function (k) {
            o[k] = Object.getOwnPropertyDescriptor(value, k);
        });
        return o;
    };
}

function getUID() {
    return ++nextUID;
}

function Pool() {
    var methods = {
        constructor: []
    };
    var silence = {
        "onTick": 1,
        "onPostTick": 1,
        "onRender": 1
    };
    var debug = null;
    var proxies = [];
    var contents = {};

    function onEvent(e) {
        var target = e.target;
        var names = (target.className || "").split(/\s+/).filter(function (n) {
            return n.length > 0;
        });

        var event = e.type;
        event = event.substr(0, 1).toUpperCase() + event.substr(1);

        while (target) {
            var id = target.id;
            if (target.onclick) return;
            if (id) {
                id = id.substr(0, 1).toUpperCase() + id.substr(1);

                var i = 0,
                    name;
                if (names.length) {
                    while (name = names[i++]) {
                        name = name.substr(0, 1).toUpperCase() + name.substr(1);
                        $$("on" + event + id + name, target);
                    }
                } else {
                    $$("on" + event + id, target);
                }
                break;
            }
            target = target.parentNode;
        }
    }

    this.registerEvents = function (target, args) {
        if (!args && target && DOC.typeOf(target) == "array") {
            args = target;
            target = null;
        }
        if (!target) target = document.body;
        if (!args) {
            args = [];
            for (var k in target) {
                var m = k.match(/^on(.+)/);
                if (!m) continue;
                args.push(m[1]);
            }
        }
        args.forEach(function (arg) {
            target.addEventListener(arg, onEvent);
        });
    };

    this.debug = function (m) {
        debug = m;
    };

    this.silence = function (m) {
        silence[m] = 1;
    };

    this.addProxy = function (obj) {
        if (obj && obj.call) proxies.push(obj);
    };

    this.removeProxy = function (obj) {
        var i = proxies.indexOf(obj);
        if (i == -1) return;
        proxies.splice(i, 1);
    };

    this.add = function (obj, enableDirectMsg) {
        if (!obj) return;
        if (debug && obj.constructor.name == debug) console.log("add", obj);

        if (!("__uid" in obj)) obj.__uid = getUID();

        if (!("__uid" in obj)) console.warn("Could not add __uid to ", obj, obj.constructor.name);

        contents[obj.__uid] = obj;
        var clazz = obj.constructor;
        if (obj.methods || clazz.methods) {
            var arr = obj.methods || clazz.methods;
            if (!(arr instanceof Array)) arr = Object.keys(arr);
            var l = arr.length;
            for (var i = 0; i < l; ++i) {
                var m = arr[i];
                if (m && m[0] != "_") {
                    this.listen(obj, m, enableDirectMsg);
                    if (clazz.meta[m] && clazz.meta[m].silence) this.silence(m);
                }
            }
        } else {
            var properties = {},
                cobj = obj;
            do {
                Object.assign(properties, Object.getOwnPropertyDescriptors(cobj));
            } while (cobj = Object.getPrototypeOf(cobj));

            for (var k in properties) {
                if (typeof obj[k] != "function") continue;
                if (k && k[0] != "_") this.listen(obj, k);
            }
        }
    };

    this.remove = function (obj) {
        if (obj.constructor.name == debug) console.log("remove", obj);

        delete contents[obj.__uid];

        if (obj.methods || obj.constructor.methods) {
            for (var k in obj.methods || obj.constructor.methods) {
                this.mute(obj, k);
            }
        } else {
            var properties = {},
                cobj = obj;
            do {
                Object.assign(properties, Object.getOwnPropertyDescriptors(cobj));
            } while (cobj = Object.getPrototypeOf(cobj));

            for (var k in properties) {
                this.mute(obj, k);
            }
        }
    };

    this.poll = function (t) {
        if (!t) return contents;
        var keys = Object.keys(contents);
        var ret = [];
        var count = 0;
        for (; count < keys.length; ++count) {
            ret.push(t(contents[keys[count]]));
        }return ret;
    };

    this.listen = function (obj, name, enableDirectMsg) {
        var method = obj[name];
        if (typeof method != "function") return;

        var arr = methods[name];
        if (!arr) arr = methods[name] = {};
        arr[obj.__uid] = {
            THIS: obj,
            method: method
        };

        if (enableDirectMsg) {
            arr = methods[name + obj.__uid];
            if (!arr) arr = methods[name + obj.__uid] = {};
            arr[obj.__uid] = {
                THIS: obj,
                method: method
            };
        }
    };

    this.mute = function (obj, name) {
        var method = obj[name];
        var listeners = methods[name];
        if (!listeners) return;
        delete listeners[obj.__uid];
    };

    this.call = function (method) {
        if (method === undefined) {
            console.error("Undefined call");
            return;
        }

        var i, l;

        /* * /
        var args = Array.prototype.slice.call(arguments, 1);
        /*/
        var args = new Array(arguments.length - 1);
        for (i = 1, l = arguments.length; i < l; i++) {
            args[i - 1] = arguments[i];
        } /* */

        for (i = 0; i < proxies.length; ++i) {
            proxies[i].call(method, args);
        }

        var listeners = methods[method];
        if (!listeners) {
            if (!(method in silence)) console.log(method + ": 0");
            return;
        }

        var keys = Object.keys(listeners);
        var ret; //=undefined
        var count = 0,
            c;
        for (; count < keys.length; ++count) {
            c = listeners[keys[count]];

            // DEBUG
            // if (debug && (method == debug || c.THIS.constructor.name == debug)) console.log(c.THIS, method, args);
            // END-DEBUG

            var lret = c && c.method.apply(c.THIS, args);
            if (lret !== undefined) ret = lret;
        }
        // if (!(method in silence)) console.log(method + ": " + count);
        return ret;
    };
}

module.exports = Pool;

},{}],37:[function(require,module,exports){
(function (global){
"use strict";

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

function store(obj, asBuffer) {

    if (typeof obj == "function") obj = undefined;
    if (!obj || (typeof obj === "undefined" ? "undefined" : _typeof(obj)) != "object") return obj;

    var inst = [],
        strIndex = { "Object": -2, "Array": -3 },
        arrIndex = {},
        objIndex = [];

    add(obj);

    if (asBuffer) return toBuffer(inst);

    return inst;

    function add(obj) {
        var type = typeof obj === "undefined" ? "undefined" : _typeof(obj);
        if (type == "function") {
            obj = undefined;
            type = typeof obj === "undefined" ? "undefined" : _typeof(obj);
        }

        var index;
        if (obj === undefined) {
            index = -4;
        } else if (type == "string") {
            index = strIndex[obj];
            if (index === undefined) index = -1;
        } else index = inst.indexOf(obj);

        if (index != -1) return index;

        if (type == "object") {
            index = objIndex.indexOf(obj);
            if (index != -1) return index;
        }

        index = inst.length;
        inst[index] = obj;

        if (type == "string") strIndex[obj] = index;

        if (!obj || type != "object") return index;

        objIndex[index] = obj;

        var ctorIndex = add(obj.constructor.fullName || obj.constructor.name);

        if (obj.buffer && obj.buffer instanceof ArrayBuffer) {

            if (!asBuffer) obj = Array.from(obj);

            inst[index] = [ctorIndex, -3, obj];
            return index;
        }

        var key,
            keySet = [];
        for (key in obj) {
            if (Object.prototype.hasOwnProperty.call(obj, key)) {
                var keyIndex = strIndex[key];
                if (keyIndex === undefined) {
                    keyIndex = inst.length;
                    inst[keyIndex] = key;
                    strIndex[key] = keyIndex;
                    keyIndex = -1;
                }
                keySet[keySet.length] = keyIndex;
            }
        }

        var strKeySet = JSON.stringify(keySet);
        keyIndex = arrIndex[strKeySet];
        if (keyIndex === undefined) {
            keyIndex = inst.length;
            inst[keyIndex] = keySet;
            arrIndex[strKeySet] = keyIndex;
        }

        var valueSet = [ctorIndex, keyIndex];

        for (key in obj) {
            if (obj.hasOwnProperty(key)) {
                var value = obj[key];
                var valueIndex = add(value);
                valueSet[valueSet.length] = valueIndex;
            }
        }

        strKeySet = JSON.stringify(valueSet);
        keyIndex = arrIndex[strKeySet];
        if (keyIndex === undefined) {
            arrIndex[strKeySet] = index;
            inst[index] = valueSet;
        } else {
            inst[index] = [keyIndex];
        }

        return index;
    }
}

function load(arr, isBuffer) {

    if (isBuffer || arr && arr.buffer) arr = fromBuffer(arr);

    var SELF = null;

    if (!arr || (typeof arr === "undefined" ? "undefined" : _typeof(arr)) !== "object") return arr;

    if (!Array.isArray(arr)) return undefined;

    (function () {
        try {
            SELF = window;
        } catch (ex) {}
    })();
    if (!SELF) (function () {
        try {
            SELF = global;
        } catch (ex) {}
    })();

    var objects = [];

    var cursor = 0;
    return read(-1);

    function read(pos) {

        switch (pos) {
            case -1:
                pos = cursor;
                break;
            case -2:
                return "Object";
            case -3:
                return "Array";
            default:
                if (objects[pos]) return objects[pos];

                break;
        }

        if (pos == cursor) cursor++;

        var value = arr[pos];
        if (!value) return value;

        var type = typeof value === "undefined" ? "undefined" : _typeof(value);
        if (type != "object") return value;

        if (value.length == 1) value = arr[value[0]];

        var className = read(value[0]);

        if (!className.split) console.log(className, value[0]);

        var ctor = SELF,
            obj;
        className.split(".").forEach(function (part) {
            return ctor = ctor[part];
        });

        if (value[1] !== -3) {
            obj = new ctor();
            objects[pos] = obj;

            var fieldRefList,
                mustAdd = value[1] > pos;

            fieldRefList = arr[value[1]];

            var fieldList = fieldRefList.map(function (ref) {
                return read(ref);
            });

            if (mustAdd) cursor++;

            for (var i = 2; i < value.length; ++i) {
                var vi = value[i];
                if (vi !== -4) obj[fieldList[i - 2]] = read(vi);
            }
        } else {

            obj = value[2];
            if (!isBuffer) objects[pos] = obj = ctor.from(obj);else objects[pos] = obj = new ctor(obj);

            cursor++;
        }

        return obj;
    }
}

function toBuffer(src) {
    var out = [];

    var dab = new Float64Array(1);
    var bab = new Uint8Array(dab.buffer);
    var sab = new Int32Array(dab.buffer);
    var fab = new Float32Array(dab.buffer);

    var p = 0;

    for (var i = 0, l = src.length; i < l; ++i) {
        var value = src[i],
            type = typeof value === "undefined" ? "undefined" : _typeof(value);

        switch (type) {
            case "boolean":
                // 1, 2
                out[p++] = 1 + (value | 0);
                break;

            case "number":
                var isFloat = Math.floor(value) !== value;
                if (isFloat) {

                    fab[0] = value;

                    if (fab[0] === value || isNaN(value)) {
                        out[p++] = 3;
                        out[p++] = bab[0];out[p++] = bab[1];
                        out[p++] = bab[2];out[p++] = bab[3];
                    } else {
                        dab[0] = value;
                        out[p++] = 4;
                        out[p++] = bab[0];out[p++] = bab[1];
                        out[p++] = bab[2];out[p++] = bab[3];
                        out[p++] = bab[4];out[p++] = bab[5];
                        out[p++] = bab[6];out[p++] = bab[7];
                    }
                } else {
                    saveInt(0, value);
                }
                break;

            case "string":
                var start = p,
                    restart = false;
                saveInt(1, value.length);
                for (var bi = 0, bl = value.length; bi < bl; ++bi) {
                    var byte = value.charCodeAt(bi);
                    if (byte > 0xFF) {
                        restart = true;
                        break;
                    }
                    out[p++] = byte;
                }

                if (!restart) break;

                p = start;
                saveInt(2, value.length);

                for (var bi = 0, bl = value.length; bi < bl; ++bi) {
                    var byte = value.charCodeAt(bi);
                    out[p++] = byte & 0xFF;
                    out[p++] = byte >> 8 & 0xFF;
                }

                break;

            case "object":
                if (_typeof(value[2]) == "object") {
                    var typed = new Uint8Array(value[2].buffer);

                    saveInt(3, -typed.length);
                    saveInt(0, value[0]);

                    for (var bi = 0, bl = typed.length; bi < bl; ++bi) {
                        out[p++] = typed[bi];
                    }
                } else {
                    saveInt(3, value.length);
                    for (var bi = 0, bl = value.length; bi < bl; ++bi) {
                        saveInt(0, value[bi]);
                    }
                }

                break;
        }
    }

    return Uint8Array.from(out);

    function saveInt(type, value) {

        var bitCount = Math.ceil(Math.log2(Math.abs(value)));
        var byte = type << 6;

        if (bitCount < 3 || value === -8) {
            byte |= 0x30;
            byte |= value & 0xF;
            out[p++] = byte;
            return;
        }

        if (bitCount <= 8 + 3 || value === -2048) {
            byte |= 0x10;
            byte |= value >>> 8 & 0xF;
            out[p++] = byte;
            out[p++] = value & 0xFF;
            return;
        }

        if (bitCount <= 16 + 3 || value === -524288) {
            byte |= 0x20;
            byte |= value >>> 16 & 0xF;
            out[p++] = byte;
            out[p++] = value >>> 8 & 0xFF;
            out[p++] = value & 0xFF;
            return;
        }

        sab[0] = value;
        out[p++] = byte;
        out[p++] = bab[0];out[p++] = bab[1];
        out[p++] = bab[2];out[p++] = bab[3];
        return;
    }
}

function fromBuffer(src) {
    var out = [];
    var dab = new Float64Array(1);
    var bab = new Uint8Array(dab.buffer);
    var sab = new Int32Array(dab.buffer);
    var fab = new Float32Array(dab.buffer);

    var pos = 0;

    for (var l = src.length; pos < l;) {
        out[out.length] = read();
    }return out;

    function read() {
        var tmp;
        var byte = src[pos++];
        switch (byte) {
            case 0:
                break;
            case 1:
                return false;
            case 2:
                return true;
            case 3:
                return decodeFloat32();
            case 4:
                return decodeFloat64();
        }

        var hb = byte >>> 4;
        var lb = byte & 0xF;
        switch (hb & 3) {
            case 0:
                // 32 bit int
                tmp = decodeInt32();
                break;
            case 1:
                // 12 bit int
                tmp = src[pos++] | lb << 28 >> 20;
                break;
            case 2:
                // 19 bit int
                tmp = lb << 28 >> 12 | src[pos] | src[pos + 1] << 8;
                pos += 2;
                break;
            case 3:
                // 4-bit int
                tmp = lb << 28 >> 28;
        }

        switch (hb >> 2) {
            case 0:
                return tmp;
            case 1:
                return decodeStr8(tmp);
            case 2:
                return decodeStr16(tmp);
            case 3:
                return decodeArray(tmp);
        }
    }

    function decodeStr8(size) {
        var acc = "";
        for (var i = 0; i < size; ++i) {
            acc += String.fromCharCode(src[pos++]);
        }return acc;
    }

    function decodeStr16(size) {
        var acc = "";
        for (var i = 0; i < size; ++i) {
            var h = src[pos++];
            acc += String.fromCharCode(h << 8 | src[pos++]);
        }
        return acc;
    }

    function decodeArray(size) {

        var ret = [];
        if (size < 0) {

            ret[0] = read(); // type
            ret[1] = -3;

            size = -size;

            var bytes = new Uint8Array(size);

            for (var i = 0; i < size; ++i) {
                bytes[i] = src[pos++];
            }ret[2] = bytes.buffer;
        } else {

            for (var i = 0; i < size; ++i) {
                ret[i] = read();
            }
        }

        return ret;
    }

    function decodeInt32() {
        bab[0] = src[pos++];bab[1] = src[pos++];
        bab[2] = src[pos++];bab[3] = src[pos++];
        return sab[0];
    }

    function decodeFloat32() {
        bab[0] = src[pos++];bab[1] = src[pos++];
        bab[2] = src[pos++];bab[3] = src[pos++];
        return fab[0];
    }

    function decodeFloat64() {
        bab[0] = src[pos++];bab[1] = src[pos++];
        bab[2] = src[pos++];bab[3] = src[pos++];
        bab[4] = src[pos++];bab[5] = src[pos++];
        bab[6] = src[pos++];bab[7] = src[pos++];
        return dab[0];
    }
}

module.exports = { store: store, load: load };

}).call(this,typeof global !== "undefined" ? global : typeof self !== "undefined" ? self : typeof window !== "undefined" ? window : {})

},{}],38:[function(require,module,exports){
"use strict";

module.exports = {

	width: 626,
	height: 1004,
	remap: false,

	elements: {

		"#bg": {
			"src": "Arduboy (" + Math.floor(Math.random() * 10) + ").png"
		},

		"led.tx": {
			style: {
				top: "85%",
				left: "22%",
				width: "4%"
			}
		},

		"led.rx": {
			style: {
				top: "85%",
				left: "19%",
				width: "4%"
			}
		},

		"#Up": {
			style: {
				top: "49%",
				left: "16%",
				width: "14%",
				height: "10%"
			}
		},

		"#Left": {
			style: {
				top: "57%",
				left: "2%",
				width: "15%",
				height: "9%"
			}
		},

		"#Down": {
			style: {
				top: "65%",
				left: "16%",
				width: "14%",
				height: "10%"
			}
		},
		"#Right": {
			style: {
				top: "58%",
				left: "28%",
				width: "15%",
				height: "9%"
			}
		},
		"#A": {
			style: {
				top: "59%",
				left: "64%",
				width: "14%",
				height: "8%",
				borderRadius: "50%"
			}
		},
		"#B": {
			style: {
				top: "54%",
				left: "79%",
				width: "15%",
				height: "9%",
				borderRadius: "50%"
			}
		},
		"#PWR": {
			style: {
				width: "14%",
				height: "7%",
				top: 0,
				left: "15%"
			}
		},
		"#MCM": {
			style: {
				width: "6%",
				height: "9%",
				top: "25%",
				left: "89%"
			}
		},
		"#ABM": {
			style: {
				width: "15%",
				height: "9%",
				top: "84%",
				left: "19%",
				display: "none"
			}
		},
		"#DBG": {
			style: {
				width: "21%",
				height: "10%",
				bottom: 0,
				left: "41%"
			}
		},
		"#RESET": {
			style: {
				width: "7%",
				height: "4%",
				bottom: "3%",
				left: "20%"
			}
		}

	}

};

},{}],39:[function(require,module,exports){
"use strict";

module.exports = {

    width: 128,
    height: 64,
    remap: false,

    elements: {

        "#bg": {
            "src": "",
            "style.display": "none"
        },

        "#screen": {
            style: "\n    top: 0;\n    left: 0;\n    right: 0;\n    bottom: 0;\n    height: 64px;\n    width: 128px;\n    margin: auto;\n    background: black;\n"
        }

    }

};

},{}],40:[function(require,module,exports){
"use strict";

module.exports = {

    width: 128,
    height: 64,
    remap: false,

    elements: {

        "#bg": {
            "src": "",
            "style.display": "none"
        },

        "#screen": {
            style: "\n    top: 0;\n    left: 0;\n    right: 0;\n    bottom: 0;\n    height: 128px;\n    width: 256px;\n    margin: auto;\n    background: black;\n"
        }

    }

};

},{}],41:[function(require,module,exports){
"use strict";

module.exports = {

    width: 128,
    height: 64,
    remap: false,

    elements: {

        "#bg": {
            "src": "",
            "style.display": "none"
        },

        "#screen": {
            style: "\n    top: 0;\n    left: 0;\n    height: 100%;\n    width: 100%;\n    background: black;\n"
        }

    }

};

},{}],42:[function(require,module,exports){
"use strict";

module.exports = {

	width: 91,
	height: 57,
	remap: true,

	elements: {
		"#bg": {
			"src": "mc.png",
			"dataset.pixelated": "true"
		},

		"#screen": {
			style: {
				"top": "34%",
				"height": "30.13%",
				"width": "38%",
				"left": "30.5%",
				"transform": "rotate(-90deg)"
			}
		},

		"#Right": {
			style: {
				"top": "20%",
				"left": "16%",
				"width": "9%",
				"height": "14%"
			}
		},

		"#Left": {
			style: {
				"top": "46%",
				"left": "16%",
				"width": "9%",
				"height": "14%"
			}
		},

		"#Up": {
			style: {
				"top": "31%",
				"width": "10%",
				"left": "7%",
				"height": "16%"
			}
		},

		"#Down": {
			style: {
				"top": "31%",
				"width": "10%",
				"left": "24%",
				"height": "16%"
			}
		},

		"#A": {
			style: {
				"top": "38%",
				"width": "9%",
				"left": "72%",
				"height": "15%"
			}
		},

		"#B": {
			style: {
				"top": "28%",
				"width": "9%",
				"left": "82%",
				"height": "15%"
			}
		},

		"#PWR": {
			style: {
				"left": "18%",
				"height": "14%"
			}
		},

		"#MCM": {
			style: {
				"display": "none"
			}
		},

		"#ABM": {
			style: {
				display: "block",
				width: "10%",
				height: "12%",
				bottom: "2%",
				left: "22%"
			}
		},

		"#DBG": {
			style: {
				"width": "11%",
				"left": "44%",
				"height": "14%"
			}
		},

		"#RESET": {
			style: {
				"width": "7%",
				"height": "10%",
				"top": "2%",
				"left": "71%"
			}
		}

	}

};

},{}],43:[function(require,module,exports){
"use strict";

module.exports = {

	width: 1352,
	height: 917,
	remap: false,
	CRTFade: true,
	background: "black",

	elements: {

		"#bg": {
			"src": "Pipboy3000.jpg",
			"style": "mix-blend-mode: screen"
		},

		"#screen": {
			style: "\n    top: 23.7%;\n    height: 30.25%;\n    left: 33%;\n    width: 41%;\n    filter: sepia(100%) hue-rotate(12deg) saturate(100);\n"
		},

		"led.tx": {
			style: {
				top: "85%",
				left: "22%",
				width: "4%"
			}
		},

		"led.rx": {
			style: {
				top: "85%",
				left: "19%",
				width: "4%"
			}
		},

		"#ABM": {
			style: {
				width: "15%",
				height: "9%",
				top: "84%",
				left: "19%",
				display: "none"
			}
		},
		"#DBG": {
			style: {
				width: "21%",
				height: "10%",
				bottom: 0,
				left: "41%"
			}
		}
	}

};

},{}],44:[function(require,module,exports){
"use strict";

module.exports = {

	width: 1380,
	height: 1047,
	remap: false,
	CRTFade: true,
	background: "black",

	elements: {

		"#bg": {
			"src": "Pipboy3000_MarkIV.jpg",
			"style": "mix-blend-mode: screen"
		},

		"#screen": {
			style: "\n    top: 26%;\n    height: 22.92%;\n    left: 22%;\n    width: 35%;\n    filter: sepia(100%) hue-rotate(12deg) saturate(100);\n"
		},

		"led.tx": {
			style: {
				top: "85%",
				left: "22%",
				width: "4%"
			}
		},

		"led.rx": {
			style: {
				top: "85%",
				left: "19%",
				width: "4%"
			}
		},

		"#ABM": {
			style: {
				width: "15%",
				height: "9%",
				top: "84%",
				left: "19%",
				display: "none"
			}
		},
		"#DBG": {
			style: {
				width: "21%",
				height: "10%",
				bottom: 0,
				left: "41%"
			}
		}
	}

};

},{}],45:[function(require,module,exports){
"use strict";

module.exports = {

    width: 686,
    height: 816,
    remap: false,
    background: "white",

    elements: {

        "#bg": {
            "src": "tama" + Math.floor(Math.random() * 3) + ".jpg"
        },

        "#screen": {
            style: "\n    top: 39.7%;\n    height: 17.653%;\n    left: 29%;\n    width: 42%;\n    filter: invert(100%) contrast(85%);\n"
        }

    }

};

},{}],46:[function(require,module,exports){
'use strict';

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

var IStore = require('./IStore.js');
var lf = require('localforage');

var ForageStore = function (_IStore) {
	_inherits(ForageStore, _IStore);

	function ForageStore() {
		_classCallCheck(this, ForageStore);

		var _this = _possibleConstructorReturn(this, (ForageStore.__proto__ || Object.getPrototypeOf(ForageStore)).call(this));

		_this.root = "";

		_this.fs = {
			mkdir: function mkdir(path, cb) {
				cb();
			},
			readFile: function readFile(path, enc, cb) {
				lf.getItem(path, cb);
			},
			writeFile: function writeFile(path, data, cb) {
				lf.setItem(path, data, cb);
			}
		};

		return _this;
	}

	return ForageStore;
}(IStore);

module.exports = ForageStore;

},{"./IStore.js":47,"localforage":6}],47:[function(require,module,exports){
'use strict';

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var fs = null;

function mkdirp(base, path, callback) {
    var acc = base || "";
    var paths = path.split(/[\/\\]+/);
    paths.pop(); // remove last file/empty entry
    work();
    return;

    function work() {
        if (!paths.length) return callback(true);
        var current = paths.shift();
        fs.mkdir(acc + current, function (err) {
            if (err && err.code != 'EEXIST') {
                callback(false);
            } else {
                acc += current + '/';
                work();
            }
        });
    }
}

var onload = [],
    wasInit = false;
var lock = {};

var IStore = function () {
    function IStore() {
        _classCallCheck(this, IStore);
    }

    _createClass(IStore, [{
        key: 'getTextItem',
        value: function getTextItem(k, cb) {

            if (lock[k]) cb(lock[k]);else fs.readFile(this.root + k, "utf-8", function (err, data) {
                return cb(data);
            });
        }
    }, {
        key: 'getItemBuffer',
        value: function getItemBuffer(k, cb) {

            if (lock[k]) cb(lock[k]);else {
                console.log("Reading ", k);
                fs.readFile(this.root + k, function (err, data) {
                    console.log("Read ", k, err);
                    cb(data);
                });
            }
        }
    }, {
        key: 'setItem',
        value: function setItem(k, v, cb) {
            var _this = this;

            mkdirp(this.root, k, function (success) {

                if (!success) {
                    cb(false);
                } else if (lock[k]) {
                    setTimeout(_this.setItem.bind(_this, k, v, cb), 200);
                } else {
                    lock[k] = v;
                    fs.writeFile(_this.root + k, v, function (err) {

                        delete lock[k];
                        if (cb) cb(!err);
                    });
                }
            });
        }
    }, {
        key: 'saveFile',
        value: function saveFile(k, v) {
            console.log("Writing " + k);

            var paths = k.split(/[\/\\]+/),
                acc = paths.shift();
            while (paths.length > 1) {
                acc = acc + '/' + paths.shift();
                if (fs.existsSync(acc)) continue;
                try {
                    fs.mkdirSync(acc);
                } catch (err) {
                    if (err.code !== 'EEXIST') {
                        alert("Could not create directory " + acc);
                        return false;
                    }
                }
            }

            try {
                fs.writeFileSync(k, v);
            } catch (err) {
                return err;
            }
        }
    }, {
        key: 'onload',
        set: function set(cb) {
            if (wasInit) cb();else onload.push(cb);
        }
    }, {
        key: 'fs',
        set: function set(_fs) {
            var _this2 = this;

            if (fs) return;

            fs = _fs;

            mkdirp(this.root, "store/", function () {

                _this2.root += "store/";

                wasInit = true;

                for (var i = 0, cb; cb = onload[i]; ++i) {
                    cb();
                }
            });
        }
    }]);

    return IStore;
}();

module.exports = IStore;

},{}],48:[function(require,module,exports){
'use strict';

var _dryDi = require('dry-di');

var _App = require('./App.js');

var _App2 = _interopRequireDefault(_App);

var _IStore = require('./store/IStore.js');

var _IStore2 = _interopRequireDefault(_IStore);

var _Forage = require('./store/Forage.js');

var _Forage2 = _interopRequireDefault(_Forage);

var _mvc = require('./lib/mvc.js');

var _CloudCompiler = require('./compilers/CloudCompiler.js');

var _CloudCompiler2 = _interopRequireDefault(_CloudCompiler);

var _Env = require('./entities/Env.js');

var _Env2 = _interopRequireDefault(_Env);

var _Sim = require('./entities/Sim.js');

var _Sim2 = _interopRequireDefault(_Sim);

var _Splash = require('./entities/Splash.js');

var _Splash2 = _interopRequireDefault(_Splash);

var _arduboy = require('./components/arduboy.js');

var _arduboy2 = _interopRequireDefault(_arduboy);

var _BTN = require('./components/BTN.js');

var _BTN2 = _interopRequireDefault(_BTN);

var _config = require('./components/config.js');

var _config2 = _interopRequireDefault(_config);

var _Debugger = require('./components/Debugger.js');

var _Debugger2 = _interopRequireDefault(_Debugger);

var _files = require('./components/files.js');

var _files2 = _interopRequireDefault(_files);

var _LED = require('./components/LED.js');

var _LED2 = _interopRequireDefault(_LED);

var _market = require('./components/market.js');

var _market2 = _interopRequireDefault(_market);

var _SCREEN = require('./components/SCREEN.js');

var _SCREEN2 = _interopRequireDefault(_SCREEN);

var _SOUND = require('./components/SOUND.js');

var _SOUND2 = _interopRequireDefault(_SOUND);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } } // let {bind, inject, getInstanceOf} = require('./lib/dry-di.js');


var entities = {
   Env: _Env2.default,
   Sim: _Sim2.default,
   Splash: _Splash2.default
};
Object.freeze(entities);
var components = {
   arduboy: _arduboy2.default,
   BTN: _BTN2.default,
   config: _config2.default,
   Debugger: _Debugger2.default,
   files: _files2.default,
   LED: _LED2.default,
   market: _market2.default,
   SCREEN: _SCREEN2.default,
   SOUND: _SOUND2.default
};
Object.freeze(components);


document.addEventListener("DOMContentLoaded", function () {
   setTimeout(function () {

      (0, _dryDi.bind)(_Forage2.default).to(_IStore2.default).singleton();
      (0, _dryDi.bind)(_CloudCompiler2.default).to('Compiler').singleton();

      var Dud = function Dud() {
         _classCallCheck(this, Dud);
      };

      ;
      (0, _dryDi.bind)(Dud).to('Plugin').singleton();

      var url = void 0;
      var match = location.search.match(/[?&](?:file|hex|url)=([^&]+)/);
      if (match) {
         url = match[1];
         if (/^https?%.*/.test(url)) url = decodeURIComponent(url);
      }

      (0, _mvc.boot)({
         main: _App2.default,
         element: document.body,
         components: components,
         entities: entities,
         model: {
            // proxy: "https://cors-anywhere.herokuapp.com/",
            ram: {
               skin: "BareFit",
               autoRun: "crates.ino.leonardo.hex",
               debuggerEnabled: false,
               isNativeBuild: false
            }
         }
      }).pool.add({
         embed: function embed(url) {
            window.open(url);
         }
      });
   }, 2000);
});

},{"./App.js":7,"./compilers/CloudCompiler.js":18,"./components/BTN.js":19,"./components/Debugger.js":20,"./components/LED.js":21,"./components/SCREEN.js":22,"./components/SOUND.js":23,"./components/arduboy.js":24,"./components/config.js":25,"./components/files.js":26,"./components/market.js":27,"./entities/Env.js":28,"./entities/Sim.js":29,"./entities/Splash.js":30,"./lib/mvc.js":35,"./store/Forage.js":46,"./store/IStore.js":47,"dry-di":3}]},{},[48])