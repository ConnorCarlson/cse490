const ASTROID_SHRINK = 20;
const MIN_SIZE_ASTROID = 10;
const MAX_SIZE_ASTROID = 50;
const ASTROID_DAMAGE = 1;
const TIME_BETWEEN_ASTROIDS = 10;
const TIME_BETWEEN_TURRETS = 100;

class Astroid {
    x;
    y;
    diam;
    r;
    g;
    b;
    constructor(x, y, diam) {
        this.x = x;
        this.y = y;
        this.diam = diam;
        this.r = Math.floor(Math.random() * 20 + 170);
        this.g = Math.floor(Math.random() * 20 + 120);
        this.b = Math.floor(Math.random() * 20 + 40);
    }

    updatePos() {
        this.y = this.y+SCROLL_VEL;
    }

    hit() {
        this.diam = ASTROID_SHRINK + 5 > this.diam ? 0 : this.diam - ASTROID_SHRINK;
    }

    canSpawn(otherAstroids, turrets, score, timeSinceLastAstroid, packs) {
        for(let other of otherAstroids) {
            if(collideCircleCircle(this.x, this.y, this.diam, other.x, other.y, other.diam)) {
                return false;
            }
        }
        for(let other of turrets) {
            if(collideCircleCircle(this.x, this.y, this.diam, other.x, other.y, TURRET_SIZE)) {
                return false;
            }
        }
        for(let pack of packs) {
            if(collideRectCircle(pack.x, pack.y, PACK_WIDTH, PACK_WIDTH, this.x, this.y, this.diam)) {
                return false;
            }
        }
        return Math.floor(Math.random() * SCORE_ODDS) - score*.1 <= 0 && timeSinceLastAstroid > TIME_BETWEEN_ASTROIDS;
    }

    draw() {
        
        fill(this.r, this.g, this.b);
        noStroke();
        circle(this.x, this.y, this.diam);
    }

    hitShip(ship) {
        let triPoly = [];
		triPoly[0] = createVector(ship.x - SHIP_WIDTH / 2, ship.y + SHIP_HEIGHT / 2);
    	triPoly[1] = createVector(ship.x, ship.y - SHIP_HEIGHT / 2);
    	triPoly[2] = createVector(ship.x + SHIP_WIDTH / 2, ship.y + SHIP_HEIGHT / 2);
        return collideCirclePoly(this.x, this.y, this.diam, triPoly);
    }


}
