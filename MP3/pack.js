
class Pack {
	x;
	y;

	constructor(x, y) {
		this.x = x;
		this.y = y;
	}

	updatePos() {
		this.y = this.y + SCROLL_VEL;
	}

	canSpawn(astroids, packs, turrets) {
        for(let ast of astroids) {
            if(collideRectCircle(this.x, this.y, PACK_WIDTH, PACK_WIDTH, ast.x, ast.y, ast.diam)) {
                return false;
            }
        }
        for(let ast of turrets) {
            if(collideRectCircle(this.x, this.y, PACK_WIDTH, PACK_WIDTH, ast.x, ast.y, TURRET_SIZE)) {
                return false;
            }
        }
        for(let pack of packs) {
            if(collideRectRect(this.x, this.y, PACK_WIDTH, PACK_WIDTH, pack.x, pack.y, PACK_WIDTH, PACK_WIDTH)) {
                return false;
            }
        }
        return Math.floor(Math.random() * SCORE_ODDS*.2) <= 0;
    }

	hitShip(ship) {
        let triPoly = [];
		triPoly[0] = createVector(ship.x - SHIP_WIDTH / 2, ship.y + SHIP_HEIGHT / 2);
    	triPoly[1] = createVector(ship.x, ship.y - SHIP_HEIGHT / 2);
    	triPoly[2] = createVector(ship.x + SHIP_WIDTH / 2, ship.y + SHIP_HEIGHT / 2);
        return collideRectPoly(this.x, this.y, PACK_WIDTH, PACK_WIDTH, triPoly);
    }
}
