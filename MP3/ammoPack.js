
class Ammo extends Pack{

	constructor(x, y) {
		super(x, y);
	}

	draw() {
		stroke(255);
		fill(0);
		rect(this.x,this.y, PACK_WIDTH, PACK_WIDTH);
		fill(255);
		circle(this.x + PACK_WIDTH/2, this.y + PACK_WIDTH/2, PACK_WIDTH/5);
	}
}
