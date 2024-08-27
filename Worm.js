class Worm {
  constructor(game, style, position) {
    this.game = game;
    this.idle = 0;
    this.break = false;
    this.turn = 0;
    this.gracetime = 1;
    this.head = new Head(game, style, position);
    this.tail = [];
    this.tail.push(new Tail(game, Tools.addVector(position, Tools.randomNormal())));

    this.foodOffset = { x: 0, y: 0 };
    this.foodDistance = 0;
  }

  act(seconds, others) {
    this.checkFood();

    if (this.gracetime > 0) {
      this.gracetime -= seconds;
    }

    if (this.idle > 0) {
      this.idle -= seconds;
    } else {
      this.think();
    }

    if (!this.break) {
      this.head.accelerate(seconds);
    }
    this.head.rotate(seconds, this.turn);

    this.head.move(seconds);
    this.head.bonk();
    this.tail.forEach((tail, i) => {
      tail.follow(i === 0 ? this.head.position : this.tail[i - 1].position);
      tail.bonk(others, i === 0 ? this.head.position : this.tail[i - 1].position, i === 0);
    });
  }

  manualTurn(value) {
    this.turn = value;
    this.idle = 5;
  }

  manualBreak(value) {
    this.break = value;
    this.idel = 5;
  }

  checkFood() {
    this.foodOffset = Tools.subtractVector(this.game.food.position, this.head.position);
    this.foodDistance = Math.sqrt(this.foodOffset.x * this.foodOffset.x + this.foodOffset.y * this.foodOffset.y);
    if(this.foodDistance < 10) {
      this.grow();
      this.game.food.position = Tools.randomVector();
    }
  }

  grow() {
    this.tail.push(new Tail(game, Tools.addVector(this.head.position, Tools.randomNormal())));
  }

  think() {
    this.break = false;
    this.turn = 0;

    const normal = {
      x: this.foodOffset.x / this.foodDistance,
      y: this.foodOffset.y / this.foodDistance,
    };
    const right = {
      x: -this.head.normal.y,
      y: this.head.normal.x,
    };
    if (normal.x * right.x + normal.y * right.y < 0) {
      this.turn = -1;
    } else {
      this.turn = 1;
    }
  }

  draw() {
    this.head.draw();
    this.tail.forEach((tail, i) => {
      tail.draw(i === this.tail.length - 1);
    });
  }
}
