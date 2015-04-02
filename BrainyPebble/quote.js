var express = require('express');
var bodyParser = require('body-parser');
var app = express();

var quotes = [
  { author : 'Isaac Newton', quote : "I do not know what I may appear to the world, but to myself I seem to have been only like a boy playing on the seashore, and diverting myself in now and then finding a smoother pebble or a prettier shell than ordinary, whilst the great ocean of truth lay all undiscovered before me."},
  { author : 'Confucius', quote : "Better a diamond with a flaw than a pebble without one."},
  { author : 'Muhammad Ali', quote : "It isn't the mountains ahead to climb that wear you out; it's the pebble in your shoe."},
  { author : 'Joseph Roux', quote : "A fine quotation is a diamond in the hand of a man of wit and a pebble in the hand of a fool."},
  { author : 'Unknown', quote: "A person without a sense of humor is like a wagon without springs. It's jolted by every pebble on the road."}
];

app.use(bodyParser.json());

app.get('/', function(req, res) {
  res.json(quotes);
});

app.get('/quote/random', function(req, res) {
  var id = Math.floor(Math.random() * quotes.length);
  var q = quotes[id];
  res.json(q);
});

app.get('/quote/:id', function(req, res) {
  if(quotes.length <= req.params.id || req.params.id < 0) {
    res.statusCode = 404;
    return res.send('No quote found');
  }

  var q = quotes[req.params.id];
  res.json(q);
});

app.post('/quote', function(req, res) {
  if(!req.body.hasOwnProperty('author') || !req.body.hasOwnProperty('quote')) {
    res.statusCode = 400;
    return res.send('Post syntax incorrect.');
  }

  var newQuote = {
    author : req.body.author,
    quote : req.body.quote
  };

  quotes.push(newQuote);
  res.json(true);
});

app.delete('/quote/:id', function(req, res) {
  quotes.splice(req.params.id, 1);
  res.json(true);
});

app.listen(process.env.PORT || 4000);