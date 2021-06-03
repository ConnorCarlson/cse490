const PACK_WIDTH = 20;

class Health {
	x;
	y;

	constructor(x, y) {
		this.x = x;
		this.y = y;
	}

	updatePos(x, y) {
		this.x = x;
		this.y = y;
	}
	getWidth() {
		return PACK_WIDTH;
	}
}
