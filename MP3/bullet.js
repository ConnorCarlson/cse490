const BULLET_VEL = 10;
const BULLET_DIAM = 5;
class Bullet {
    x;
    y;
    vx;
    vy;
    angle;
    constructor(x, y, angle, canvasW, canvasH) {
        
        this.angle = angle;
        this.vx = Math.sin(this.angle * Math.PI/180) * BULLET_VEL;
        this.vy = -Math.cos(this.angle * Math.PI/180) * BULLET_VEL;
        this.x = x + (SHIP_HEIGHT/10) * this.vx;
        this.y = y + (SHIP_HEIGHT/10) * this.vy;
        this.canvasW = canvasW;
        this.canvasH = canvasH;
    }


    updatePos() {
        this.x += this.vx;
        this.y += this.vy;
    }

    draw() {
        stroke(255);
		fill(255);
        circle(this.x, this.y, BULLET_DIAM);
    }

}
