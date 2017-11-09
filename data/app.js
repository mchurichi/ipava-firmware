var devEnvs = ['localhost', '127.0.0.1'];
var devEsp = '';
var baseUrl = (devEnvs.indexOf(document.location.hostname) >= 0) ? devEsp : ''; 

var ipava;

/**
 * 
 * @param { property: 1, property2: 2 } object 
 * @param { myInput: '=', myInput2: '#input2 } mapping 
 */
function inputMap(object, mapping) {
  this.object = object;

  Object.keys(mapping).forEach(function(key) {
    var elementSelector = (mapping[key] === '=') ? '#' + key : elementSelector; 
    var $el = $(elementSelector);

    if ($el && this.object.hasOwnProperty(key)) {
      $el.html(this.object[key]);
    }
  }, this);
}

$(function() {
  ipava = {
    wifi: {
      status: getStatus,
    }
  };

  function getStatus() {
    return $.get({ url: baseUrl + '/api/wifi/status' });
  }
});