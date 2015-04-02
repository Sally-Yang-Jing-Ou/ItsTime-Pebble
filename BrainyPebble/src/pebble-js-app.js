var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function getQuotes() {
  // Construct URL
  var url = 'https://cherry-sundae-1584.herokuapp.com/quote/random';

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', function(responseText) {
      var json = JSON.parse(responseText);
      console.log("what is" + json);
    
      var author = json.author;
      var quote = json.quote;
    
      var display = quote.toString() + " Author: " + author.toString();
    console.log ("return value: " + display);
    
    var info = {
  'AUTHOR': author,
  'QUOTE': quote,
  'DISPLAY': display
};

   // Send to Pebble
  Pebble.sendAppMessage(info);
    }      
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');
    getQuotes();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    getQuotes();
  }                     
);
