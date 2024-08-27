const game = new Game();
let lastTime = 0;

function init() {
  game.init();
  requestAnimationFrame(loop);
}

function loop(time) {
  const seconds = (time - lastTime) / 1000.0;
  lastTime = time;

  game.tick(seconds);

  requestAnimationFrame(loop);
}

init();