const VEL = 6;
const DIAM = 5;
const SHIP_HEIGHT = 40;
class Bullet {
    x;
    y;
    vx;
    vy;
    angle;
    constructor(x, y, angle, canvasW, canvasH) {
        
        this.angle = angle;
        this.vx = Math.sin(this.angle * Math.PI/180) * VEL;
        this.vy = -Math.cos(this.angle * Math.PI/180) * VEL;
        this.x = x + (SHIP_HEIGHT/4) * this.vx;
        this.y = y + (SHIP_HEIGHT/4) * this.vy;
        this.canvasW = canvasW;
        this.canvasH = canvasH;
    }


    updatePos() {
        this.x += this.vx;
        this.y += this.vy;
    }

    getDiam() {
        return DIAM;
    }
}
