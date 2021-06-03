// This is a basic web serial template for p5.js. Please see:
// https://makeabilitylab.github.io/physcomp/communication/p5js-serial
//
// By Jon E. Froehlich
// @jonfroehlich
// http://makeabilitylab.io/
//

let pHtmlMsg;
let serialOptions = { baudRate: 115200 };
let serial;
let ship1;
let ship2;
let healthPrev;
const ship1StartX = 900;
const ship1StartY = 50;
const ship2StartX = 100;
const ship2StartY = 100;
let bullets = [];
let healthPack;
let spec = {};
spec.update = 'qlearn'; // qlearn | sarsa
spec.gamma = 0.9; // discount factor, [0, 1)
spec.epsilon = 0.2; // initial epsilon for epsilon-greedy policy, [0, 1)
spec.alpha = 0.01; // value function learning rate
spec.experience_add_every = 10; // number of time steps before we add another experience to replay memory
spec.experience_size = 10000; // size of experience replay memory
spec.learning_steps_per_iteration = 20;
spec.tderror_clamp = 1.0; // for robustness
spec.num_hidden_units = 100; //1000; // number of neurons in hidden layer

function setup() {
	createCanvas(1000, 500);

	// Setup Web Serial using serial.js
	serial = new Serial();
	serial.on(SerialEvents.CONNECTION_OPENED, onSerialConnectionOpened);
	serial.on(SerialEvents.CONNECTION_CLOSED, onSerialConnectionClosed);
	serial.on(SerialEvents.DATA_RECEIVED, onSerialDataReceived);
	serial.on(SerialEvents.ERROR_OCCURRED, onSerialErrorOccurred);

	// If we have previously approved ports, attempt to connect with them
	serial.autoConnectAndOpenPreviouslyApprovedPort(serialOptions);

	// Add in a lil <p> element to provide messages. This is optional
	pHtmlMsg = createP('Click anywhere on this page to open the serial connection dialog');
	ship1 = new Ship(ship1StartX, ship1StartY, 0, 0, 0, width, height);
	healthPrev = ship1.health;
	// ship2 = new Ship(ship2StartX, ship2StartY, 0, 0, 0, width, height);
	healthPack = new Health(width / 2, height / 2);
	angleMode(DEGREES);
	// start();
	// start2();
}


async function serialWriteHealth() {

	if (serial.isOpen()) {
		console.log("Writing to serial: ", "HEATH:"+ship1.health*10);
		serial.writeLine("HEALTH:"+ship1.health*10);
	}
  }

function draw() {
	if(ship1.health != healthPrev) {
		serialWriteHealth();
		healthPrev = ship1.health;
	}
	ship1.updatePos();
	
	background(0);
	// console.log(ship1.angle);
	checkMovement();

	stroke(0, 0, 255);
	fill(0);

	push();
	let healthMult = ship1.getHeight() / 10;

	translate(ship1.x, ship1.y);

	rotate(ship1.angle);

	circle(0, 0, ship1.getHeight() + 1);
	triangle(
		-ship1.getWidth() / 2,
		ship1.getHeight() / 2,
		0,
		-ship1.getHeight() / 2,
		ship1.getWidth() / 2,
		ship1.getHeight() / 2
	);
	// rect(-ship1.getHeight() / 2, -ship1.getHeight() / 2 - 10, ship1.health * healthMult, 5);

	pop();
	// stroke(255);
	// fill(0);

	// push();
	// translate(ship2.x, ship2.y);

	// rotate(ship2.angle);

	// circle(0, 0, ship2.getHeight() + 1);
	// triangle(
	// 	-ship2.getWidth() / 2,
	// 	ship2.getHeight() / 2,
	// 	0,
	// 	-ship2.getHeight() / 2,
	// 	ship2.getWidth() / 2,
	// 	ship2.getHeight() / 2
	// );
	// rect(-ship2.getHeight() / 2, -ship2.getHeight() / 2 - 10, ship2.health * healthMult, 5);
	// pop();
	// rect(healthPack.x, healthPack.y, PACK_WIDTH, PACK_WIDTH);

	drawBullets();
}

function checkMovement() {
	if (keyIsDown(LEFT_ARROW)) {
		ship1.rotate(0);
	}
	if (keyIsDown(RIGHT_ARROW)) {
		ship1.rotate(1);
	}
	if (keyIsDown(UP_ARROW)) {
		ship1.thrust();
	}
	if (keyIsDown(32)) {
		// if (Date.now() - ship1.lastShotTime > 200 || ship1.lastShotTime == null) {
		// 	ship1.shoot();
		// 	bullets.push(new Bullet(ship1.x, ship1.y, ship1.angle, width, height));
		// }
	}
	if (keyIsDown(83)) {
		saveToFile();
	}
	if (keyIsDown(76)) {
		loadFile();
	}
}

function drawBullets() {
	for (var i = bullets.length - 1; i >= 0; i--) {
		circle(bullets[i].x, bullets[i].y, bullets[i].getDiam());
	}
}

function updateBullets() {
	for (var i = bullets.length - 1; i >= 0; i--) {
		bullets[i].updatePos();
		if (bullets[i].x < 0 || bullets[i].x > width || bullets[i].y < 0 || bullets[i].y > height) {
			bullets.splice(i, 1);
		}
	}
}

function checkBulletCollision() {
	for (var i = bullets.length - 1; i >= 0; i--) {
		let dx = ship1.x - bullets[i].x;
		let dy = ship1.y - bullets[i].y;
		let distance = Math.sqrt(dx * dx + dy * dy);
		if (distance < ship1.getHeight() / 2 + bullets[i].getDiam() / 2) {
			ship1.health = ship1.health == 0 ? 0 : ship1.health - 1;
			bullets.splice(i, 1);
		}
	}
	for (var i = bullets.length - 1; i >= 0; i--) {
		let dx = ship2.x - bullets[i].x;
		let dy = ship2.y - bullets[i].y;
		let distance = Math.sqrt(dx * dx + dy * dy);
		if (distance < ship2.getHeight() / 2 + bullets[i].getDiam() / 2) {
			ship2.health = ship2.health == 0 ? 0 : ship2.health - 1;
			bullets.splice(i, 1);
		}
	}
	// if(ship1.health == 0 || ship2.health == 0) {
	//   ship1 = new Ship(ship1StartX, ship1StartY, 0, 0, 0, width, height);
	//   ship2 = new Ship(ship2StartX, ship2StartY, 0, 0, 0, width, height);
	// }
}

function checkSingleHealthCollision(ship) {
	var distX = Math.abs(ship.x - healthPack.x - PACK_WIDTH / 2);
	var distY = Math.abs(ship.y - healthPack.y - PACK_WIDTH / 2);

	if (distX > PACK_WIDTH / 2 + ship.getHeight()) {
		return false;
	}
	if (distY > PACK_WIDTH / 2 + ship.getHeight()) {
		return false;
	}

	if (distX <= PACK_WIDTH / 2) {
		return true;
	}
	if (distY <= PACK_WIDTH / 2) {
		return true;
	}

	var dx = distX - PACK_WIDTH / 2;
	var dy = distY - PACK_WIDTH / 2;
	return dx * dx + dy * dy <= ship.getHeight() * ship.getHeight();
}

function checkHealthCollisions() {
	if (checkSingleHealthCollision(ship1)) {
		ship1.health = ship1.health + 10 > 10 ? 10 : ship1.health + 5;
		healthPack.updatePos(Math.floor(Math.random() * width), Math.floor(Math.random() * height));
	}
	if (checkSingleHealthCollision(ship2)) {
		ship2.health = ship2.health + 10 > 10 ? 10 : ship2.health + 5;
		healthPack.updatePos(Math.floor(Math.random() * width), Math.floor(Math.random() * height));
	}
}

let ShipWorld = function () {
	this.reset();
};
ShipWorld.prototype = {
	reset: function () {
		ship1 = new Ship(ship1StartX, ship1StartY, 0, 0, 0, width, height);
		ship2 = new Ship(ship2StartX, ship2StartY, 0, 0, 0, width, height);
	},
	getNumStates: function () {
		return 7;
	},
	getMaxNumActions: function () {
		return 3;
	},
	// getState: function () {
	// 	return [
	// 		ship1.x,
	// 		ship1.y,
	// 		ship2.x,
	// 		ship2.y,
	// 		ship1.vx,
	// 		ship1.vy,
	// 		ship2.vx,
	// 		ship2.vy,
	// 		ship1.angle,
	// 		ship2.angle,
	// 		healthPack.x,
	// 		healthPack.y,
	// 		bullets[0] ? bullets[0].vx : 0,
	// 		bullets[0] ? bullets[0].vy : 0,
	// 		ship1.health,
	// 		ship2.health,
	// 	];
	// },
	getState: function () {
		return [
			ship1.x,
			ship1.y,
			ship1.vx,
			ship1.vy,
			ship1.angle,
			healthPack.x,
			healthPack.y,
		];
	},
	sampleNextState: function (a) {
		if (a === 0) {
			// ship2.thrust();
			ship2.rotate(0);
		}
		if (a === 1) ship2.rotate(1);
		// if (a === 2) {
		// 	ship2.rotate(0);
		// }
		// if (a === 3){
		// 	if (ship2.lastShotTime > 20) {
		// 		ship2.shoot();
		// 		bullets.push(new Bullet(ship2.x, ship2.y, ship2.angle, width, height));
		// 	}
		// } 
		ship2.lastShotTime = ship2.lastShotTime + 1;
		ship1.lastShotTime = ship1.lastShotTime + 1;

		ship1.updatePos();
		ship2.updatePos();
		updateBullets();
		checkBulletCollision();
		

		// compute reward
		// let br = 0;
		// for (bullet of bullets) {
		// 	let d1 = Math.sqrt(
		// 		(bullet.x - ship1.x) * (bullet.x - ship1.x) + (bullet.y - ship1.y) * (bullet.y - ship1.y)
		// 	);
		// 	let d2 = Math.sqrt(
		// 		(bullet.x - ship2.x) * (bullet.x - ship2.x) + (bullet.y - ship2.y) * (bullet.y - ship2.y)
		// 	);
		// 	br = br + d1;
		// }
		// br = br/(bullets.length+1);
		// br = -br;

		let d3 = Math.sqrt(
			(healthPack.x - ship2.x) * (healthPack.x - ship2.x) + (healthPack.y - ship2.y) * (healthPack.y - ship2.y));
		// let d4 = Math.sqrt(
		// 	(width/2 - ship2.x) * (width/2 - ship2.x) + (height/2 - ship2.y) * (height/2 - ship2.y));
		// d3 = -d3/2;
		// d4 = -d4/2;
		// (ship2.health - ship1.health) * 200 + 
		let angle = 180 - Math.atan2((healthPack.x - ship2.x) , (healthPack.y - ship2.y))*180/Math.PI;
		
		let r = 0;// -d3;
		r = r - Math.abs(angle - ship2.angle) * 5;
		// if(checkSingleHealthCollision(ship2)) {
		// 	r = r + 100000;
		// }
		checkHealthCollisions();

		var ns = this.getState();
		var out = { ns: ns, r: r };
		return out;
	},
};

function start() {
	env = new ShipWorld();
	//agent = new RL.ActorCritic(env, spec);
	agent = new RL.DQNAgent(env, spec);
	initFlot();
	$('#slider').slider({
		min: 0,
		max: 1,
		value: agent.epsilon,
		step: 0.01,
		slide: function (event, ui) {
			agent.epsilon = ui.value;
			$('#eps').html(ui.value.toFixed(2));
		},
	});
	$('#eps').html(agent.epsilon.toFixed(2));
	togglelearn();
}

function gofast() { steps_per_tick = 500; }
function gonormal() { steps_per_tick = 10; }
function goslow() { steps_per_tick = 1; }

let ShipWorld2 = function () {
	this.reset();
};
ShipWorld2.prototype = {
	reset: function () {
		ship1 = new Ship(ship1StartX, ship1StartY, 0, 0, 0, width, height);
		ship2 = new Ship(ship2StartX, ship2StartY, 0, 0, 0, width, height);
	},
	getNumStates: function () {
		return 14;
	},
	getMaxNumActions: function () {
		return 5;
	},
	getState: function () {
		return [
			ship1.x,
			ship1.y,
			ship2.x,
			ship2.y,
			ship1.vx,
			ship1.vy,
			ship2.vx,
			ship2.vy,
			healthPack.x,
			healthPack.y,
			bullets[0] ? bullets[0].vx : 0,
			bullets[0] ? bullets[0].vy : 0,
			ship1.health,
			ship2.health,
		];
	},
	sampleNextState: function (a) {
		if (a === 0) {
			ship1.thrust();
		}
		if (a === 1) ship1.rotate(1);
		if (a === 2) {
			if (ship1.lastShotTime > 20) {
				ship1.shoot();
				bullets.push(new Bullet(ship1.x, ship1.y, ship1.angle, width, height));
			}
		}

		// compute reward
		let br = 0;
		for (bullet of bullets) {
			let d1 = Math.sqrt(
				(bullet.x - ship1.x) * (bullet.x - ship1.x) + (bullet.y - ship1.y) * (bullet.y - ship1.y)
			);
			let d2 = Math.sqrt(
				(bullet.x - ship2.x) * (bullet.x - ship2.x) + (bullet.y - ship2.y) * (bullet.y - ship2.y)
			);
			br = br + d1 - d2;
		}
		let r = (ship1.health - ship2.health) * 200 + br;
		var ns = this.getState();
		var out = { ns: ns, r: r };
		return out;
	},
};

function start2() {
	env2 = new ShipWorld2();
	//agent = new RL.ActorCritic(env, spec);
	agent2 = new RL.DQNAgent(env2, spec);
	togglelearn2();
}

function resetAgent() {
	agent = new RL.DQNAgent(env, spec);
}

function saveToFile() {
	var blob = new Blob([JSON.stringify(agent)], {
		//type: 'application/json'
		type: 'octet/stream',
	});
	console.log(blob);

	var anchor = document.createElement('a');
	anchor.download = 'output.json';
	anchor.href = window.URL.createObjectURL(blob);
	anchor.innerHTML = 'download';
	anchor.click();

	console.log(anchor);
	// document.getElementById('output').append(anchor);
}

function loadFile() {
	let request = new XMLHttpRequest();
	request.open('GET', './agent.json', false);
	request.send(null);
	let my_JSON_object = JSON.parse(request.responseText);
	agent.fromJSON(my_JSON_object);
	agent.epsilon = 0.05;
	// kill learning rate to not learn
	agent.alpha = 0;
}

var steps_per_tick = 1;
var sid = -1;
var action, state;
var smooth_reward_history = [];
var smooth_reward = null;
var flott = 0;
function togglelearn() {
	if (sid === -1) {
		sid = setInterval(function () {
			for (var k = 0; k < steps_per_tick; k++) {
				state = env.getState();
				action = agent.act(state);
				var obs = env.sampleNextState(action);
				agent.learn(obs.r);
				if (smooth_reward == null) {
					smooth_reward = obs.r;
				}
				smooth_reward = smooth_reward * 0.999 + obs.r * 0.001;
				flott += 1;
				if (flott === 200) {
					// record smooth reward
					if (smooth_reward_history.length >= nflot) {
						smooth_reward_history = smooth_reward_history.slice(1);
					}
					smooth_reward_history.push(smooth_reward);
					flott = 0;
				}
			}
		}, 20);
	} else {
		clearInterval(sid);
		sid = -1;
	}
}

 // flot stuff
 var nflot = 1000;
 function initFlot() {
   var container = $("#flotreward");
   var res = getFlotRewards();
   series = [{
	 data: res,
	 lines: {fill: true}
   }];
   var plot = $.plot(container, series, {
	 grid: {
	   borderWidth: 1,
	   minBorderMargin: 20,
	   labelMargin: 10,
	   backgroundColor: {
		 colors: ["#FFF", "#e4f4f4"]
	   },
	   margin: {
		 top: 10,
		 bottom: 10,
		 left: 10,
	   }
	 },
	 xaxis: {
	   min: 0,
	   max: nflot
	 },
	 yaxis: {
	   min: -1000,
	   max: 1000
	 }
   });
   setInterval(function(){
	 series[0].data = getFlotRewards();
	 plot.setData(series);
	 plot.draw();
   }, 100);
 }
 function getFlotRewards() {
   // zip rewards into flot data
   var res = [];
   for(var i=0,n=smooth_reward_history.length;i<n;i++) {
	 res.push([i, smooth_reward_history[i]]);
   }
   return res;
 }

var sid2 = -1;
var action2, state2;
var smooth_reward_history2 = [];
var smooth_reward2 = null;
var flott = 0;
function togglelearn2() {
	if (sid2 === -1) {
		sid2 = setInterval(function () {
			for (var k = 0; k < steps_per_tick; k++) {
				state2 = env2.getState();
				action2 = agent2.act(state);
				var obs = env2.sampleNextState(action2);
				agent2.learn(obs.r);
				if (smooth_reward2 == null) {
					smooth_reward2 = obs.r;
				}
				smooth_reward2 = smooth_reward2 * 0.999 + obs.r * 0.001;
			}
		}, 20);
	} else {
		clearInterval(sid2);
		sid2 = -1;
	}
}

/**
 * Callback function by serial.js when there is an error on web serial
 *
 * @param {} eventSender
 */
function onSerialErrorOccurred(eventSender, error) {
	console.log('onSerialErrorOccurred', error);
	pHtmlMsg.html(error);
}

/**
 * Callback function by serial.js when web serial connection is opened
 *
 * @param {} eventSender
 */
function onSerialConnectionOpened(eventSender) {
	console.log('onSerialConnectionOpened');
	pHtmlMsg.html('Serial connection opened successfully');
}

/**
 * Callback function by serial.js when web serial connection is closed
 *
 * @param {} eventSender
 */
function onSerialConnectionClosed(eventSender) {
	console.log('onSerialConnectionClosed');
	pHtmlMsg.html('onSerialConnectionClosed');
}

/**
 * Callback function serial.js when new web serial data is received
 *
 * @param {*} eventSender
 * @param {String} newData new data received over serial
 */
function onSerialDataReceived(eventSender, newData) {
	// console.log("onSerialDataReceived", newData);
	pHtmlMsg.html('onSerialDataReceived: ' + newData);
	let dataSplit = newData.split(':');
	if(dataSplit[0] === "ACC") {
		let acc = dataSplit[1];
		if(acc > 3) {
			ship1.rotate(1);
		} else if(acc < -3) {
			ship1.rotate(0);
		}
	} 
	
	if (dataSplit[0] === "THRUST") {
		ship1.thrust();
	}
	if(dataSplit[0] === "HEALTH") {
		console.log("got it");
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

