class Tail {
  constructor(game, position) {
    this.game = game;
    this.position = position;
    this.normal = Tools.randomNormal();
  }

  follow(position) {
    const offset = Tools.subtractVector(position, this.position);
    const distance = Math.sqrt(offset.x * offset.x + offset.y * offset.y);
    this.normal = {
      x: offset.x / distance,
      y: offset.y / distance,
    };
    this.position = Tools.subtractVector(position, Tools.multiplyVector(this.normal, 15));
  }

  bonk(worms, previous, toothless) {
    worms.forEach((worm) => {
      const closestPoint = Tools.closestPointOnLine(previous, this.position, worm.head.position);
      const offset = Tools.subtractVector(worm.head.position, closestPoint);
      const distance = Math.sqrt(offset.x * offset.x + offset.y * offset.y);
      if (distance < 10 && distance > 0.01) {
        const normal = {
          x: offset.x / distance,
          y: offset.y / distance,
        };
        const velocity = worm.head.velocity.x * normal.x + worm.head.velocity.y * normal.y;
        worm.head.velocity = Tools.addVector(worm.head.velocity, Tools.multiplyVector(normal, velocity * -2 + 100));
        worm.head.position = Tools.addVector(closestPoint, Tools.multiplyVector(normal, 10));
        if (worm.tail.length > 1 && !toothless) {
          worm.tail.pop();
        }
      }
    });
  }

  draw(tip) {
    const c = this.game.c;
    c.save();
    c.translate(this.position.x, this.position.y);
    c.beginPath();

    if(tip) {
      Tools.cMoveTo(c, Tools.rotateByNormal(this.normal, { x: 7, y: -3}));
      Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: 7, y: 3}));
      Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: -7, y: 2}));
      Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: -7, y: -2}));
    } else {
      Tools.cMoveTo(c, Tools.rotateByNormal(this.normal, { x: 1.7, y: -2.5}));
      Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: 1.7, y: 2.5}));
      Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: -1.7, y: 3}));
      Tools.cLineTo(c, Tools.rotateByNormal(this.normal, { x: -1.7, y: -3}));
    }

    c.closePath();
    c.stroke();
    c.restore();
  }
}
