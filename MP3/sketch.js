// This is a basic web serial template for p5.js. Please see:
// https://makeabilitylab.github.io/physcomp/communication/p5js-serial
//
// By Jon E. Froehlich
// @jonfroehlich
// http://makeabilitylab.io/
//

const SHIP_WIDTH = 20;
const SHIP_HEIGHT = 40;
const BULLET_DELAY = 10;
const TURRET_BULLET_DELAY = 40;
const SCORE_ODDS = 500;
const PACK_WIDTH = 20;
const SHEILD_COOLDOWN_LENGTH = 80;
const options = {
	flipHorizontal: false, // boolean value for if the video should be flipped, defaults to false
	maxContinuousChecks: Infinity, // How many frames to go without running the bounding box detector. Defaults to infinity, but try a lower value if the detector is consistently producing bad predictions.
	detectionConfidence: 0.5, // Threshold for discarding a prediction. Defaults to 0.8.
	scoreThreshold: 0.99, // A threshold for removing multiple (likely duplicate) detections based on a "non-maximum suppression" algorithm. Defaults to 0.75
	iouThreshold: 0.3, // A float representing the threshold for deciding whether boxes overlap too much in non-maximum suppression. Must be between [0, 1]. Defaults to 0.3.
};
const SCROLL_VEL = 5;

let mode = 0;
let timeSinceLastAstroid;
let timeSinceLastTurret;
let serialOptions = { baudRate: 115200 };
let serial;
let ship1;
let ship2;
let healthPrev;
let ammoPrev;
let energyPrev;
let bullets = [];
let enemyBullets = [];
let astroids = [];
let packs = [];
let turrets = [];
let healthPack;
let score;
let sheildCooldown;
let loadingModel;
let loadingVideo;

let handpose;
let video;
let predictions = [];

function setup() {
	createCanvas(1216, 912);
	video = createCapture(VIDEO);
	video.size(1216, 912);
	handpose = ml5.handpose(options, modelReady);
	// handpose.on('predict', (results) => {
	// 	predictions = results;
	// });
	frameRate(200);

	video.hide();

	// Setup Web Serial using serial.js
	serial = new Serial();
	serial.on(SerialEvents.CONNECTION_OPENED, onSerialConnectionOpened);
	serial.on(SerialEvents.CONNECTION_CLOSED, onSerialConnectionClosed);
	serial.on(SerialEvents.DATA_RECEIVED, onSerialDataReceived);
	serial.on(SerialEvents.ERROR_OCCURRED, onSerialErrorOccurred);

	// If we have previously approved ports, attempt to connect with them
	serial.autoConnectAndOpenPreviouslyApprovedPort(serialOptions);

	// Add in a lil <p> element to provide messages. This is optional
	ship1 = new Ship(width / 2, height - SHIP_HEIGHT - 5, 0, 0, width, height);
	healthPrev = ship1.health;
	ammoPrev = ship1.ammo;
	energyPrev = ship1.energy;
	score = 0;
	sheildCooldown = SHEILD_COOLDOWN_LENGTH;
	angleMode(DEGREES);
	timeSinceLastAstroid = 0;
	timeSinceLastTurret = 0;
	loadingModel = true;
	loadingVideo = true;
}

async function serialWriteHealth() {
	if (serial.isOpen()) {
		serial.writeLine('HEALTH:' + ship1.health);
	}
}

async function serialWriteAmmo() {
	if (serial.isOpen()) {
		serial.writeLine('AMMO:' + ship1.ammo);
	}
}

async function serialWriteEnergy() {
	if (serial.isOpen()) {
		serial.writeLine('ENERGY:' + ship1.energy);
	}
}

async function detectPose() {
	handpose.predict(video, (results) => {
		if (results.length > 0 && sheildCooldown > SHEILD_COOLDOWN_LENGTH && ship1.energy > 0) {
			var a = results[0].annotations.thumb[3][0] - results[0].annotations.indexFinger[3][0];
    		var b = results[0].annotations.thumb[3][1] - results[0].annotations.indexFinger[3][1];

    		var c = Math.sqrt( a*a + b*b );
			if(c < 50 && ship1.energy > 80) {
				astroids = [];
				turrets = [];
				ship1.subtractEnergy(80);
			} else {
				ship1.sheild = true;
				if(score%5 == 0) {
					ship1.subtractEnergy(1);
				}
				
			}
			
		} else {
			if (ship1.sheild) {
				sheildCooldown = 0;
			}
			ship1.sheild = false;
		}
	});
}

function modelReady() {
	console.log('Model ready!');
	loadingModel = false;
}

function loadeddata() {
	console.log('data loaded!');
	loadingVideo = false;
}

function draw() {
	if (loadingModel || !video.loadedmetadata) {
		console.log('loading');
		return;
	}
	background(0);

	if(mode == 0) {
		
		startScreen();

	} else if(mode == 1) {
		if (ship1.health != healthPrev) {
			serialWriteHealth();
			healthPrev = ship1.health;
		}
		if (ship1.ammo != ammoPrev) {
			serialWriteAmmo();
			ammoPrev = ship1.ammo;
		}
		if (ship1.energy != energyPrev) {
			serialWriteEnergy();
			energyPrev = ship1.energy;
		}
		if(ship1.health == 0) {
			mode = 2;
		}
		ship1.updatePos();
		ship1.lastShotTime = ship1.lastShotTime + 1;
		timeSinceLastAstroid = timeSinceLastAstroid + 1;
		timeSinceLastTurret = timeSinceLastTurret + 1;
		if (!ship1.sheild && score%30 == 0) {
			ship1.regenEnergy();
		}
		detectPose();
	
		background(0);
		ship1.draw();
		updateBullets();
		drawBullets();
		updateAstroids();
		drawAstroids();
		checkBulletCollision();
		updatePacks();
		drawPacks();
		checkPackCollisions();
		score = score + 1;
		sheildCooldown = sheildCooldown + 1;
		drawScore();
		
		
	} else {
		endScreen();
	}
	
}

function startScreen() {
	textSize(50);
	textAlign(CENTER);
	fill(255);
  	text("Asteroid Belt", width/2, height/2-50);
	textSize(30);
	text("Press Button To Start", width/2, height/2+60);
}

function endScreen() {
	textSize(50);
	textAlign(CENTER);
	fill(255);
  	text("Game Over", width/2, height/2-50);
	textSize(30);
	text("Score: " + score, width/2, height/2+60);
}

function drawScore() {
	fill(0);
	noStroke();
	rect(0, 0, width, 50);
	fill(255);
	textAlign(LEFT);
	textSize(20);
	
	text("Score: " + score, 10, 30);
}

function reset() {
	ship1 = new Ship(width / 2, height - SHIP_HEIGHT - 5, 0, 0, width, height);
	healthPrev = ship1.health;
	ammoPrev = ship1.ammo;
	energyPrev = ship1.energy;
	score = 0;
	angleMode(DEGREES);
	timeSinceLastAstroid = 0;
	timeSinceLastTurret = 0;
	bullets = [];
	enemyBullets = [];
	astroids = [];
	packs = [];
	turrets = [];
	mode = 1;

	serialWriteHealth();
	serialWriteEnergy();
	serialWriteAmmo();

}

function updateAstroids() {
	for (var i = astroids.length - 1; i >= 0; i--) {
		astroids[i].updatePos();
		if (astroids[i].y - astroids[i].diam > height) {
			astroids.splice(i, 1);
		}
		if (astroids[i]) {
			if (ship1.sheild) {
				if (
					collideCircleCircle(
						ship1.x,
						ship1.y,
						SHIP_HEIGHT + 10,
						astroids[i].x,
						astroids[i].y,
						astroids[i].diam
					)
				) {
					astroids.splice(i, 1);
				}
			} else if (astroids[i].hitShip(ship1)) {
				ship1.damage(ASTROID_DAMAGE * astroids[i].diam);
				astroids.splice(i, 1);
			}
		}
	}

	for (var i = turrets.length - 1; i >= 0; i--) {
		turrets[i].updatePos();
		turrets[i].lastShotTime = turrets[i].lastShotTime + 1;
		if (turrets[i].checkAngle(ship1) && turrets[i].canShoot()) {
			turrets[i].shoot();
			enemyBullets.push(new Bullet(turrets[i].x, turrets[i].y, turrets[i].angle));
		}
		if (turrets[i].y - turrets[i].diam > height) {
			turrets.splice(i, 1);
		}
		if (turrets[i]) {
			if (ship1.sheild) {
				if (
					collideCircleCircle(ship1.x, ship1.y, SHIP_HEIGHT + 10, turrets[i].x, turrets[i].y, TURRET_SIZE)
				) {
					turrets.splice(i, 1);
				}
			} else if (turrets[i].hitShip(ship1)) {
				ship1.damage(ASTROID_DAMAGE * 40);
				turrets.splice(i, 1);
			}
		}
	}
	let newAstroid = new Astroid(
		Math.floor(Math.random() * width),
		-MAX_SIZE_ASTROID,
		Math.floor(Math.random() * MAX_SIZE_ASTROID) + MIN_SIZE_ASTROID
	);
	if (newAstroid.canSpawn(astroids, turrets, score, timeSinceLastAstroid, packs)) {
		timeSinceLastAstroid = 0;
		astroids.push(newAstroid);
	}

	let newTurret = new Turret(Math.floor(Math.random() * width), -TURRET_SIZE, 180);
	if (newTurret.canSpawn(turrets, astroids, score, timeSinceLastTurret, packs)) {
		timeSinceLastTurret = 0;
		turrets.push(newTurret);
	}
}

function updatePacks() {
	for (let pack of packs) {
		pack.updatePos();
	}
	let newPack = new Health(Math.floor(Math.random() * width), -PACK_WIDTH);
	switch (Math.floor(Math.random() * 2)) {
		case 0:
			newPack = new Health(Math.floor(Math.random() * width), -PACK_WIDTH);
			break;
		case 1:
			newPack = new Ammo(Math.floor(Math.random() * width), -PACK_WIDTH);
			break;
	}
	if (newPack.canSpawn(astroids, packs, turrets)) {
		packs.push(newPack);
	}
}

function drawPacks() {
	for (let pack of packs) {
		pack.draw();
	}
}

function drawAstroids() {
	for (let a of astroids) {
		a.draw();
	}
	for (let t of turrets) {
		t.draw();
	}
}

function drawBullets() {
	for (var i = bullets.length - 1; i >= 0; i--) {
		bullets[i].draw();
	}
	for (var i = enemyBullets.length - 1; i >= 0; i--) {
		enemyBullets[i].draw();
	}
}

function updateBullets() {
	for (var i = bullets.length - 1; i >= 0; i--) {
		bullets[i].updatePos();
		if (bullets[i].x < 0 || bullets[i].x > width || bullets[i].y < 0 || bullets[i].y > height) {
			bullets.splice(i, 1);
		}
	}

	for (var i = enemyBullets.length - 1; i >= 0; i--) {
		enemyBullets[i].updatePos();
		if (enemyBullets[i].x < 0 || enemyBullets[i].x > width || enemyBullets[i].y < 0 || enemyBullets[i].y > height) {
			enemyBullets.splice(i, 1);
		}
	}
}

function checkBulletCollision() {
	for (var i = enemyBullets.length - 1; i >= 0; i--) {
		let triPoly = [];
		triPoly[0] = createVector(ship1.x - SHIP_WIDTH / 2, ship1.y + SHIP_HEIGHT / 2);
		triPoly[1] = createVector(ship1.x, ship1.y - SHIP_HEIGHT / 2);
		triPoly[2] = createVector(ship1.x + SHIP_WIDTH / 2, ship1.y + SHIP_HEIGHT / 2);
		if (
			ship1.sheild &&
			collideCircleCircle(ship1.x, ship1.y, SHIP_HEIGHT + 10, enemyBullets[i].x, enemyBullets[i].y, BULLET_DIAM)
		) {
			enemyBullets.splice(i, 1);
		} else if (collideCirclePoly(enemyBullets[i].x, enemyBullets[i].y, BULLET_DIAM, triPoly)) {
			ship1.health = ship1.health == 0 ? 0 : ship1.health - 20;
			enemyBullets.splice(i, 1);
		}
	}
	for (var i = bullets.length - 1; i >= 0; i--) {
		for (var j = astroids.length - 1; j >= 0; j--) {
			if (
				collideCircleCircle(
					bullets[i].x,
					bullets[i].y,
					BULLET_DIAM,
					astroids[j].x,
					astroids[j].y,
					astroids[j].diam
				)
			) {
				astroids[j].hit();
				bullets.splice(i, 1);
				break;
			}
		}
		for (var j = turrets.length - 1; j >= 0; j--) {
			if ( bullets[i] && 
				collideCircleCircle(
					bullets[i].x,
					bullets[i].y,
					BULLET_DIAM,
					turrets[j].x,
					turrets[j].y,
					TURRET_SIZE
				)
			) {
				turrets.splice(j, 1);
				bullets.splice(i, 1);
				break;
			}
		}
	}
}

function checkPackCollisions() {
	for (var i = packs.length - 1; i >= 0; i--) {
		if (packs[i].hitShip(ship1)) {
			if (packs[i] instanceof Health) {
				ship1.heal(20);
				packs.splice(i, 1);
			} else if (packs[i] instanceof Ammo) {
				ship1.reload();
				packs.splice(i, 1);
			}
		}
	}
}

/**
 * Callback function by serial.js when there is an error on web serial
 *
 * @param {} eventSender
 */
function onSerialErrorOccurred(eventSender, error) {
	console.log('onSerialErrorOccurred', error);
}

/**
 * Callback function by serial.js when web serial connection is opened
 *
 * @param {} eventSender
 */
function onSerialConnectionOpened(eventSender) {
	console.log('onSerialConnectionOpened');
	serialWriteHealth();
	serialWriteEnergy();
	serialWriteAmmo();
}

/**
 * Callback function by serial.js when web serial connection is closed
 *
 * @param {} eventSender
 */
function onSerialConnectionClosed(eventSender) {
	console.log('onSerialConnectionClosed');
}

/**
 * Callback function serial.js when new web serial data is received
 *
 * @param {*} eventSender
 * @param {String} newData new data received over serial
 */
function onSerialDataReceived(eventSender, newData) {
	// console.log("onSerialDataReceived", newData);
	let dataSplit = newData.split(':');
	if (dataSplit[0] === 'ACC') {
		let acc = dataSplit[1];
		ship1.vx = -parseFloat(acc) * 3;
	}

	if (dataSplit[0] === 'SHOOT') {
		if(mode == 0) {
			mode = 1;
		} else if(mode == 1) {
			if (ship1.canShoot()) {
				ship1.shoot();
				bullets.push(new Bullet(ship1.x, ship1.y, 0, width, height));
			}
		} else {
			reset();
		}
		
	}
}

/**
 * Called automatically by the browser through p5.js when mouse clicked
 */
function mouseClicked() {
	if (!serial.isOpen()) {
		serial.connectAndOpen(null, serialOptions);
	}
}
