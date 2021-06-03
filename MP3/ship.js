const ACC = .1;
const MAX_V = 5;
const WIDTH = 20;
const HEIGHT = 40;
const FRICT = .5;

class Ship {
    x;
    y;
    vx;
    vy;
    angle;
    lastShotTime;
    health;
    constructor(x, y, vx, vy, angle, canvasW, canvasH) {
        this.x = x;
        this.y = y;
        this.vx = vx;
        this.vy = vy;
        this.angle = angle;
        this.canvasW = canvasW;
        this.canvasH = canvasH;
        this.health = 10;
        this.lastShotTime = 0;
    }

    rotate(direction) {
        if(direction == 0) {
            this.angle = (this.angle + 5) % 360;
        } else {
            this.angle = this.angle - 5 < 0 ? 360 - 5 : this.angle-5;
        }

    }

    updatePos() {
        this.x += this.vx < 0 ?  (this.vx + FRICT > 0 ? 0 : this.vx + FRICT) : (this.vx - FRICT  < 0 ? 0 : this.vx - FRICT);
        this.y += this.vy < 0 ? (this.vy + FRICT  > 0 ? 0 : this.vy + FRICT) : (this.vy - FRICT  < 0 ? 0 : this.vy - FRICT);
        if(this.x + HEIGHT/2 > this.canvasW) {
            this.x = this.canvasW - HEIGHT;
            this.vx = this.vx * -1;
            this.health = this.health - 1;
        }
        if(this.x - HEIGHT/2 < 0) {
            this.x = HEIGHT/2;
            this.vx = this.vx * -1;
            this.health = this.health - 1;
        }
        if(this.y + HEIGHT/2 > this.canvasH) {
            this.y = this.canvasH - HEIGHT;
            this.vy = this.vy * -1;
            this.health = this.health - 1;
        }
        if(this.y - HEIGHT/2 < 0) {
            this.y = HEIGHT/2;
            this.vy = this.vy * -1;
            this.health = this.health - 1;
        }
        this.health = this.health < 0 ? 0 : this.health;
    }

    thrust() {
        let ax = Math.sin(this.angle * Math.PI/180) * ACC;
        let ay = -Math.cos(this.angle * Math.PI/180) * ACC;
        
        

        this.vx = this.vx + ax;
        this.vy = this.vy + ay;

        if(this.vx < -MAX_V) {
            this.vx = -MAX_V;
        }
        if(this.vy < -MAX_V) {
            this.vy = -MAX_V;
        }
        if(this.vx > MAX_V) {
            this.vx = MAX_V;
        }
        if(this.vy > MAX_V) {
            this.vy = MAX_V;
        }
    }
    shoot() {
        this.lastShotTime = 0;
    }
    getWidth() {
        return WIDTH;
    }
    getHeight() {
        return HEIGHT;
    }

}
