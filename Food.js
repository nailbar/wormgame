class Food {
  constructor(game, position) {
    this.game = game;
    this.position = position;
  }

  draw(tip) {
    const c = this.game.c;
    c.save();
    c.translate(this.position.x, this.position.y);
    c.strokeRect(-3, -3, 6, 6);
    c.restore();
  }
}
