var express = require('express');
// var bodyParser = require('body-parser');
var app = express();

var quotes = [
  { author : 'Isaac Newton', quote : "I do not know what I may appear to the world, but to myself I seem to have been only like a boy playing on the seashore, and diverting myself in now and then finding a smoother pebble or a prettier shell than ordinary, whilst the great ocean of truth lay all undiscovered before me."},
  { author : 'Confucius', quote : "Better a diamond with a flaw than a pebble without one."},
  { author : 'Muhammad Ali', quote : "It isn't the mountains ahead to climb that wear you out; it's the pebble in your shoe."},
  { author : 'Joseph Roux', quote : "A fine quotation is a diamond in the hand of a man of wit and a pebble in the hand of a fool."},
  { author : 'Henry Ward Beecher', quote: "A person without a sense of humor is like a wagon without springs. It's jolted by every pebble on the road."},
  { author : 'Joseph Roux', quote : "Everything we do, even the slightest thing we do, can have a ripple effect and repercussions that emanate. If you throw a pebble into the water on one side of the ocean, it can create a tidal wave on the other side."},
  { author : 'Henry Wadsworth Longfellow', quote : "There are moments in life, when the heart is so full of emotion That if by chance it be shaken, or into its depths like a pebble Drops some careless word, it overflows, and its secret, Spilt on the ground like water, can never be gathered together."},
  { author : 'Japanese Proverb quotes', quote : "Every little pebble in the stream believes itself to be a precious stone."},
  { author : 'Pierce Harris', quote : "Memory: a child walking along a seashore. You never can tell what small pebble it will pick up and store away among its treasured things."},
  { author : 'David Ogilvy', quote : "I never tell one client that I cannot attend his sales convention because I have a previous engagement with another client; successful polygamy depends upon pretending to each spouse that she is the only pebble on your beach."},
  { author : 'Jean Rostand', quote : "Somebody told me I should put a pebble in my mouth to cure my stuttering. Well, I tried it, and during a scene I swallowed the pebble. That was the end of that."},
  { author : 'Blaise Pascal', quote : "The least movement is of importance to all nature. The entire ocean is affected by a pebble."},
  { author : 'Victor Hugo', quote : "Nature has made a pebble and a female. The lapidary makes the diamond, and the lover makes the woman."},
  { author : 'Henry Wadsworth Longfellow', quote : "There are moments in life, when the heart is so full of emotion That if by chance it be shaken, or into its depths like a pebble Drops some careless word, it overflows, and its secret, Spilt on the ground like water, can never be gathered together."},
  { author : 'Henry Louis Mencken', quote : "How little it takes to make life unbearable: a pebble in the shoe, a cockroach in the spaghetti, a woman's laugh."},
  { author : 'Anton Balasingham', quote : "It is an unfortunate personal tragedy. However, when compared to the vast ocean of the collective tragedy faced by my people, my illness is merely a pebble. I am deeply sad that I am crippled by this illness, unable to contribute anything substantial towards the alleviation of the immense suffering and oppression of my people."},
  { author : 'Jack Nicklaus', quote : "I think that Pebble Beach is my favorite golf course to go to. I think Augusta is my favorite place to go play golf."},
  { author : 'Clint Eastwood', quote : "I think Pebble Beach is kind of a unique place on the planet."},
  { author : 'Lewis Black', quote : "I would like to play Pebble Beach at some point. I keep waiting for them to call and ask me to that little pro-am thing, but I'm not big enough."},
  { author : 'Mindy Grossman', quote : "If you throw the pebble in the pond and the rings start circulating that much wider, you've done things and created things for people that they didn't think they'd ever be able to do. That excites me."},
  { author : 'Jane Leavy', quote : "Trauma fractures comprehension as a pebble shatters a windshield. The wound at the site of impact spreads across the field of vision, obscuring reality and challenging belief."},
  { author : 'Dustin Johnson', quote : "At Pebble Beach, even on your good shots, you've got to hit it to the correct side of the holes to save pars."},
  { author : 'Natalie Gulbis', quote : "Pebble Beach. It is tough and the lay out is amazing."},
  { author : 'Steven H. Strogatz', quote : "Looking at numbers as groups of rocks may seem unusual, but actually it's as old as math itself. The word 'calculate' reflects that legacy -- it comes from the Latin word calculus, meaning a pebble used for counting. To enjoy working with numbers you don't have to be Einstein (German for 'one stone'), but it might help to have rocks in your head."},
  { author : 'Celia Luce', quote : "A small trouble is like a pebble. Hold it too close to your eye and it fills the whole world and puts everything out of focus. Hold it at a proper distance and it can be examined and properly classified. Throw it at your feet and it can be seen in its true setting, just one more tiny bump on the pathway to life."},
  { author : 'Native American Hopi Proverb', quote : "One finger cannot lift a pebble."},
  { author : 'Albanian Proverb', quote : "A pebble and a diamond are alike to a blind man."},
  { author : 'Afghan Proverb', quote : "In the shop of the sightless jeweler, the ruby and pebble are one."},
  { author : 'Nigerian Proverb', quote : "One pebble doesn't make a floor."},
  { author : 'Japanese Proverb', quote : "The pebble in the brook secretly thinks itself a precious stone."}
];

// app.use(bodyParser.json());

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

// app.post('/quote', function(req, res) {
//   if(!req.body.hasOwnProperty('author') || !req.body.hasOwnProperty('quote')) {
//     res.statusCode = 400;
//     return res.send('Post syntax incorrect.');
//   }

//   var newQuote = {
//     author : req.body.author,
//     quote : req.body.quote
//   };

//   quotes.push(newQuote);
//   res.json(true);
// });

app.delete('/quote/:id', function(req, res) {
  quotes.splice(req.params.id, 1);
  res.json(true);
});

app.listen(process.env.PORT || 4000);