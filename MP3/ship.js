const MAX_SHIP_HEALTH = 100;
const MAX_SHIP_AMMO = 50;
const MAX_SHIP_ENERGY = 100;
class Ship {
	x;
	y;
	vx;
	vy;
	lastShotTime;
	health;
    ammo;
    energy;
    sheild;
	constructor(x, y, vx, vy, canvasW, canvasH) {
		this.x = x;
		this.y = y;
		this.vx = vx;
		this.vy = vy;
		this.canvasW = canvasW;
		this.canvasH = canvasH;
		this.health = MAX_SHIP_HEALTH;
		this.lastShotTime = 0;
        this.ammo = MAX_SHIP_AMMO;
        this.energy = MAX_SHIP_ENERGY;
        this.sheild = false;
	}

	updatePos() {
		this.x += this.vx;
		this.y += this.vy;
		if (this.x + SHIP_WIDTH / 2 > this.canvasW) {
			this.x = this.canvasW - SHIP_WIDTH;
		}
		if (this.x - SHIP_WIDTH / 2 < 0) {
			this.x = SHIP_WIDTH / 2;
		}

		this.health = this.health < 0 ? 0 : this.health;
	}

	shoot() {
		this.lastShotTime = 0;
        this.ammo = this.ammo - 1;
	}

	draw() {
		stroke(255);
        noFill();
        if(this.sheild) {
            circle(this.x, this.y, SHIP_HEIGHT+10);
        }
        fill(255);

		push();
		translate(ship1.x, ship1.y);
		triangle(-SHIP_WIDTH / 2, SHIP_HEIGHT / 2, 0, -SHIP_HEIGHT / 2, SHIP_WIDTH / 2, SHIP_HEIGHT / 2);
        
		pop();
	}

    canShoot() {
        return this.lastShotTime > BULLET_DELAY && this.ammo > 0;
    }

    damage(damage) {
        this.health = damage > this.health ? 0 : this.health - damage;
    }

    heal(health) {
        this.health = this.healht + health > MAX_SHIP_HEALTH ? MAX_SHIP_HEALTH : this.health + health;
    }

    reload() {
        this.ammo = MAX_SHIP_AMMO;
    }

    regenEnergy() {
        if(this.energy < MAX_SHIP_ENERGY) {
            this.energy = this.energy + 1;
        }
    }

    subtractEnergy(energy) {
        if(energy > this.energy) {
            this.energy = 0;
        } else {
            this.energy = this.energy - energy;
        }
    }
}
