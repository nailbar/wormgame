class Head {
  constructor(game, style, position) {
    this.game = game;
    this.style = style;
    this.position = position;
    this.normal = Tools.randomNormal();
    this.velocity = { x: 0, y: 0 };
    this.rotation = 0;
  }

  accelerate(seconds) {
    this.velocity = Tools.addVector(this.velocity, Tools.multiplyVector(this.normal, 300 * seconds));
  }

  rotate(seconds, amount) {
    this.rotation += amount * 0.5 * seconds;
  }

  move(seconds) {
    this.velocity = Tools.multiplyVector(this.velocity, 1 / (1 + (seconds * 1.5)));
    this.rotation *= 1 / (1 + (seconds * 3.5));

    this.normal = Tools.rotateByNormal(this.normal, Tools.normalFromRadian(this.rotation));
    this.position = Tools.addVector(this.position, Tools.multiplyVector(this.velocity, seconds));
  }

  bonk() {
    if (this.position.x < 0) {
      this.velocity.x = Math.abs(this.velocity.x);
      this.position.x = 0;
      this.rotation += this.velocity.y * 0.002;
    }
    if (this.position.x > this.game.canvas.width) {
      this.velocity.x = 0 - Math.abs(this.velocity.x);
      this.position.x = this.game.canvas.width;
      this.rotation -= this.velocity.y * 0.002;
    }
    if (this.position.y < 0) {
      this.velocity.y = Math.abs(this.velocity.y);
      this.position.y = 0;
      this.rotation -= this.velocity.x * 0.002;
    }
    if (this.position.y > this.game.canvas.height) {
      this.velocity.y = 0 - Math.abs(this.velocity.y);
      this.position.y = this.game.canvas.height;
      this.rotation += this.velocity.x * 0.002;
    }
  }

  draw() {
    const c = this.game.c;
    c.save();
    c.translate(this.position.x, this.position.y);
    c.beginPath();

    switch (this.style) {
      case 'slim':
        Tools.cMoveTo(c, Tools.rotateByNormal(this.normal, { x: 15, y: -2}));
        Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: 15, y: 2}));
        Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: -5, y: 7}));
        Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: -5, y: -7}));
        break;
      default:
        Tools.cMoveTo(c, Tools.rotateByNormal(this.normal, { x: 15, y: -4}));
        Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: 15, y: 4}));
        Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: -5, y: 8}));
        Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: -5, y: -8}));
    }

    c.closePath();
    c.stroke();
    c.restore();
  }
}
