const NUM_CARDS = 52
const DOM_DELAY = 10

class Card {
  constructor(id) {
    this.id = id;
  }
  
  get value() {
    var type = Math.floor(this.id / 4);
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
    this.sumText = document.getElementById(sumTextId);
    this.sumText.innerHTML = 0;
  }
  
  hit(card) {
    this.sum += card.value;
    this.sumText.innerHTML = this.sum;
  }
}

function createButton(div, text, id, callback) {
  var button = document.createElement("button");
  button.innerHTML = text;
  button.id = id;
  button.addEventListener('click', callback, false);
  div.appendChild(button);
  return button;
}

var myDeck, user, dealer;
var dealButton, hitButton, standButton;
var userDiv;

function userMoves() {
  userDiv.removeChild(dealButton);
  
  for (var i = 0; i < 2; ++i) {
    user.hit(myDeck.deal());
    dealer.hit(myDeck.deal());
  }
  
  if (user.sum == 21) {
    finishGame();
    return;
  }
  
  hitButton = createButton(userDiv, "HIT", "hit-button", function() { 
    user.hit(myDeck.deal()); 
    if (user.sum >= 21) {
      finishGame();
    }
  });
  
  standButton = createButton(userDiv, "STAND", "stand-button", function() { 
    finishGame(); 
  });
}

function printResults(res) {
  window.alert(res == 1 ? "LOST" : (res == 0 ? "TIED" : "WON"));
}

function finishGame() {
  var endFunction = function(res) {
    setTimeout(function() {
      printResults(res);
      resetGame();
    }, DOM_DELAY);
  };
  
  if (user.sum > 21) {
    endFunction(1);
    return;
  }
  
  while (dealer.sum < 17) {
    dealer.hit(myDeck.deal());
  }
  endFunction((dealer.sum > 21) ? -1 : Math.sign(dealer.sum - user.sum));
}

function resetGame() {
  try {
    userDiv.removeChild(hitButton);
    userDiv.removeChild(standButton);
  } catch (err) {
  }
  
  myDeck = new Deck();
  user   = new Player("user-sum");
  dealer = new Player("dealer-sum");
  
  dealButton = createButton(userDiv, "DEAL", "deal-button", function() {
    userMoves();
  });
}

function init() {
  userDiv = document.getElementById("user-buttons");
}

window.onload = function() {
  init();
  resetGame();
}
