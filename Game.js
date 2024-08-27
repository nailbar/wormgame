class Game {
  init() {
    this.canvas = document.getElementById('canvas');
    this.c = canvas.getContext('2d');

    this.canvas.width = window.innerWidth;
    this.canvas.height = window.innerHeight;

    this.keyDown = this.keyDown.bind(this);
    this.keyUp = this.keyUp.bind(this);
    document.onkeydown = this.keyDown;
    document.onkeyup = this.keyUp;

    this.worms = [];
    this.worms.push(new Worm(this, 'slim', Tools.randomVector()));
    this.worms.push(new Worm(this, 'fat', Tools.randomVector()));
    this.food = new Food(this, Tools.randomVector());
  }

  keyDown(event) {
    let keyPressed = true;
    switch (event.key) {
      case 'a': this.worms[0].manualTurn(-1); break;
      case 's': this.worms[0].manualBreak(true); break;
      case 'd': this.worms[0].manualTurn(1); break;

      case 'j': this.worms[1].manualTurn(-1); break;
      case 'k': this.worms[1].manualBreak(true); break;
      case 'l': this.worms[1].manualTurn(1); break;
      case 'ArrowLeft': this.worms[1].manualTurn(-1); break;
      case 'ArrowDown': this.worms[1].manualBreak(true); break;
      case 'ArrowRight': this.worms[1].manualTurn(1); break;

      default: keyPressed = false;
    }

    if (keyPressed) {
      document.getElementById('info').remove();
    }
  }

  keyUp(event) {
    switch (event.key) {
      case 'a': this.worms[0].manualTurn(0); break;
      case 's': this.worms[0].manualBreak(false); break;
      case 'd': this.worms[0].manualTurn(0); break;
      
      case 'j': this.worms[1].manualTurn(0); break;
      case 'k': this.worms[1].manualBreak(false); break;
      case 'l': this.worms[1].manualTurn(0); break;
      case 'ArrowLeft': this.worms[1].manualTurn(0); break;
      case 'ArrowDown': this.worms[1].manualBreak(false); break;
      case 'ArrowRight': this.worms[1].manualTurn(0); break;

      default: console.log(event.key);
    }
  }

  tick(seconds) {
    this.c.clearRect(0, 0, this.canvas.width, this.canvas.height);

    this.food.draw();
    this.worms.forEach((worm) => {
      worm.draw();
      worm.act(seconds, this.worms.filter((otherWorm) => {
        return otherWorm !== worm;
      }));
    });
  }
}
