
// http://phrogz.net/JS/classes/OOPinJS2.html
// http://javascript.ru/tutorial/object/inheritance
//@NoGAS
var request = require('request');
var BaseAccessor = require('./interfaces/io.js').BaseAccessor;
var extend = require('./util_local.js').extend;

function Accessor(username, password)  {
  this.username_ = username;
  this.password_ = password;
}

extend(Accessor, BaseAccessor);

Accessor.prototype.makeBaseAuthGet = function (url, done, payload) {
	var options = {
	    url: url+'?'+require('querystring').stringify(payload),
	    //qs: require('querystring').stringify(payload),  // No work
	    auth: {
	      user: this.username_,
	      pass: this.password_,
	      sendImmediately: true
	    }  
	};
	request(options, done);
};

Accessor.prototype.makeNoAuthGet = function (action) {
  request(this.url_, action);
};

Accessor.prototype.post = function (payload, action) {
  var options = {
    uri: this.url_,
    auth: {
      user: this.username_,
      pass: this.password_,
      sendImmediately: true
    },
    headers: {'content-type': 'application/x-www-form-urlencoded'},
    body: require('querystring').stringify(payload)
  };
  request.post(options, action);
};


//@NoGAS
module.exports = {'Accessor': Accessor};

