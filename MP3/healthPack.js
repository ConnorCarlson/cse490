
class Health extends Pack{

	constructor(x, y) {
		super(x, y);
	}

	draw() {
		stroke(76, 224, 101);
		fill(0);
		rect(this.x,this.y, PACK_WIDTH, PACK_WIDTH);
		fill(76, 224, 101);
		rect(this.x + PACK_WIDTH/5*2, this.y + PACK_WIDTH/5, PACK_WIDTH/5, PACK_WIDTH-PACK_WIDTH/5*2);
		rect(this.x + PACK_WIDTH/5, this.y + PACK_WIDTH/5*2, PACK_WIDTH-PACK_WIDTH/5*2, PACK_WIDTH/5);
	}
}
