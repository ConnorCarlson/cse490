const TURRET_SIZE = 50;

class Turret {
    x;
    y;
    angle;
    lastShotTime;
    constructor(x, y,angle) {
        this.x = x;
        this.y = y;
        this.angle = angle;
        this.health = 10;
        this.lastShotTime = 0;
    }

    rotate(direction) {
        if(direction == 0) {
            this.angle = (this.angle + 2) % 360;
            if(this.angle > 250) {
                this.angle = 250;
            }
        } else {
            this.angle = this.angle - 2 < 0 ? 360 - 5 : this.angle-5;
            if(this.angle < 110) {
                this.angle = 110;
            }
        }

    }

    updatePos() {
        this.y = this.y+SCROLL_VEL;
    }

    canSpawn(otherTurrets, astroids, score, timeSinceLastTurret, packs) {
        for(let other of astroids) {
            if(collideCircleCircle(this.x, this.y, TURRET_SIZE, other.x, other.y, other.diam)) {
                return false;
            }
        }
        for(let other of otherTurrets) {
            if(collideCircleCircle(this.x, this.y, TURRET_SIZE, other.x, other.y, other.diam)) {
                return false;
            }
        }
        for(let pack of packs) {
            if(collideRectCircle(pack.x, pack.y, PACK_WIDTH, PACK_WIDTH, this.x, this.y, TURRET_SIZE)) {
                return false;
            }
        }
        return Math.floor(Math.random() * SCORE_ODDS) - score*.01 <= 0 && timeSinceLastTurret > TIME_BETWEEN_TURRETS;
    }

    checkAngle(ship) {
        let d = 180 - Math.atan2((ship.x - this.x) , (ship.y - this.y))*180/Math.PI;
        if(Math.abs(this.angle -d) < 5) {
            return true;
        } else if (d > this.angle){
            this.rotate(0);
        } else {
            this.rotate(1);
        }
        return false;
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

    canShoot() {
        return this.lastShotTime > TURRET_BULLET_DELAY;
    }

    draw() {
        push();
        stroke(237, 114, 128);
        noFill();
		translate(this.x, this.y);
        rotate(this.angle);
        rect(-5, -TURRET_SIZE/3, 10, TURRET_SIZE/3);
		circle(0, 0, TURRET_SIZE);
        
		pop();
    }

    hitShip(ship) {
        let triPoly = [];
		triPoly[0] = createVector(ship.x - SHIP_WIDTH / 2, ship.y + SHIP_HEIGHT / 2);
    	triPoly[1] = createVector(ship.x, ship.y - SHIP_HEIGHT / 2);
    	triPoly[2] = createVector(ship.x + SHIP_WIDTH / 2, ship.y + SHIP_HEIGHT / 2);
        return collideCirclePoly(this.x, this.y, TURRET_SIZE, triPoly);
    }

}
