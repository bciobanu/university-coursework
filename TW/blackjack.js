const NUM_CARDS = 52

class Card {
  constructor(id) {
    this.id = id;
  }
  
  get value() {
    var type = Math.floor(this.id / 4);
    if (type == 0) {  // ace
      return 11;
    }
    if (type > 9) {  // face card
      return 10;
    }
    return type + 1;
  }
}

class Deck {
  constructor() {
    this.card = new Array(NUM_CARDS);
    for (var i = 0; i < NUM_CARDS; ++i) {
      this.card[i] = new Card(i);
    } 
    this.shuffle();
  } 
  
  shuffle() {
    for (var i = NUM_CARDS - 1; i > 0; --i) {
      var index = Math.floor(i * Math.random());
      [this.card[i], this.card[index]] = [this.card[index], this.card[i]];
    }
  }
  
  deal() {
    return this.card.pop();
  }
  
  get length() {
    return this.card.length;
  }
}

class Player {
  constructor(sumTextId) {
    this.sum = 0;
    this.sumTextId = sumTextId;
  }
  
  hit(card) {
    this.sum += card.value;
    document.getElementById(this.sumTextId).innerHTML = this.sum;
  }
  
  stand() {
    this.stands = true;
  }
}

function game() {
  const myDeck = new Deck();
  const user   = new Player("user_sum");
  const dealer = new Player("dealer_sum");
  
  for (var i = 0; i < 2; ++i) {
    user.hit(myDeck.deal());
    dealer.hit(myDeck.deal());
  }
  
  var step = 0;
  while (!user.stands || !dealer.stands) {
    if (step == 0 && !user.stands) {  // user step
      if (Math.random() < 0.5) {
        user.stand();
      } else {
        user.hit(myDeck.deal());
        if (user.sum > 21) {
          return 1;
        }
      }
    } else {
      if (dealer.sum < 17) {  // should hit iff their sum is less than 17
        dealer.hit(myDeck.deal());
        if (dealer.sum > 21) {
          return -1;
        }
      } else {
        dealer.stand();
      }
    }
    step ^= 1;
  }
  
  return Math.sign(dealer.sum - user.sum);
}

function drawInitial() {}

function main() {
  drawInitial();
  console.log(game());
}

window.onload = function() {
  main();
}
