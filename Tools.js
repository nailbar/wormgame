const Tools = {
  randomVector: () => {
    return {
      x: Math.random() * this.canvas.width,
      y: Math.random() * this.canvas.height,
    };
  },
  randomNormal: () => {
    const radian = Math.random() * Math.PI * 2.0;
    return {
      x: Math.cos(radian),
      y: Math.sin(radian),
    };
  },
  normalFromRadian: (radian) => {
    return {
      x: Math.cos(radian),
      y: Math.sin(radian),
    };
  },
  addVector: (vector1, vector2) => {
    return {
      x: vector1.x + vector2.x,
      y: vector1.y + vector2.y,
    }
  },
  subtractVector: (vector1, vector2) => {
    return {
      x: vector1.x - vector2.x,
      y: vector1.y - vector2.y,
    }
  },
  multiplyVector: (vector, amount) => {
    return {
      x: vector.x * amount,
      y: vector.y * amount,
    }
  },
  rotateByNormal: (normal, vector) => {
    return {
      x: normal.x * vector.x - normal.y * vector.y,
      y: normal.y * vector.x + normal.x * vector.y,
    };
  },
  closestPointOnLine: (line1, line2, vector) => {
    const offset = Tools.subtractVector(line1, line2);
    const distance = Math.sqrt(offset.x * offset.x + offset.y * offset.y);
    const normal = {
      x: offset.x / distance,
      y: offset.y / distance,
    };
    const relativePoint = Tools.subtractVector(vector, line1);
    const relativeDistance = normal.x * relativePoint.x + normal.y * relativePoint.y;
    if (relativeDistance < 0) {
      return {
        x: line1.x,
        y: line1.y,
      };
    }
    if (relativeDistance > distance) {
      return {
        x: line2.x,
        y: line2.y,
      };
    }
    return Tools.addVector(line1, Tools.multiplyVector(normal, relativeDistance));
  },
  cMoveTo: (c, vector) => {
    c.moveTo(vector.x, vector.y);
  },
  cLineTo: (c, vector) => {
    c.lineTo(vector.x, vector.y);
  },
};
