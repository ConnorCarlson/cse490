import logo from './logo.svg';
import './App.css';
import Sketch from "react-p5";
import * as ml5 from "ml5";
import reactDom from 'react-dom';
import colorc from './colorc.jpg'
import { prettyDOM } from '@testing-library/dom';
import React, { useState } from 'react';

let handpose;
let video;
let video2;
let predictions = [];
let curImg;
let colorGrad = [];
let chooseX;
let chooseY;
let port;
let writer;



function App() {
  const [connected, setConnected] = useState(false);
  const [colors, setColors] = useState([]);

  async function connectToPort() {
    port = await navigator.serial.requestPort();
    await port.open({ baudRate: 9600 });
    writer = port.writable.getWriter();
    setConnected(true);
  }

  async function writting(r, g, b) {
    if(writer) {
      let newArr = [r,g,b];
      setColors(newArr);
      await writer.write(new Uint8Array([r, g, b]));
    }
    
  }
  

  const setup = (p5, canvasParentRef) => {
      if(p5) {
        p5.createCanvas(640, 480).parent(canvasParentRef);

        video2 = p5.createCapture(p5.VIDEO);
        video2.size(320, 240);
        video = p5.createCapture(p5.VIDEO);
        video.size(640, 480);

        
  
        handpose = ml5.handpose(video, modelReady);
  
        handpose.on("predict", results => {
          predictions = results;
        });
        p5.loadImage(colorc, img => {
          curImg = img;
          p5.image(img, (p5.width - p5.height)/2, 0, p5.height, p5.height);
          
        });
        chooseX = p5.width/2;
        chooseY = p5.height/2;
        video.hide();
  
      }
    
		// use parent to render the canvas in this ref
		// (without that p5 will render the canvas outside of your component)
    
		
	};

  function modelReady() {
    console.log('Model Loaded!');
  }

  function drawKeypoints(p5) {
    for (let i = 0; i < predictions.length; i += 1) {
      const prediction = predictions[i];
      for (let j = 0; j < prediction.landmarks.length; j += 1) {
        const keypoint = prediction.landmarks[j];
        p5.fill(0, 255, 0);
        p5.noStroke();
        p5.ellipse(keypoint[0], keypoint[1], 10, 10);
      }
    }
  }
  

	const draw = (p5) => {
    if(p5 && video) {
      // console.log(p5);
      p5.translate(video.width, 0);
      p5.scale(-1, 1);
      // p5.image(video, 0, 0, p5.width, p5.height);
      if(curImg) {
        p5.image(curImg, (p5.width - p5.height)/2, 0, p5.height, p5.height);
      }
      if(predictions[0]) {
        let thumbPos = predictions[0].annotations.thumb[3];
        let indexPos = predictions[0].annotations.indexFinger[3];
        // console.log(Math.hypot(thumbPos[0] - indexPos[0], thumbPos[1] - indexPos[1]));
        if(Math.hypot((thumbPos[0]+indexPos[0]) / 2 - p5.width/2, (thumbPos[1]+indexPos[1]) / 2 - p5.height / 2) < p5.height/2) {
          if(Math.hypot(thumbPos[0] - indexPos[0], thumbPos[1] - indexPos[1]) < 30) {
            chooseX = (thumbPos[0]+indexPos[0]) / 2;
            chooseY = (thumbPos[1]+indexPos[1]) / 2;
            
            if(port) {
              let r = p5.get(chooseX,chooseY)[0];
              let g = p5.get(chooseX,chooseY)[2];
              let b = p5.get(chooseX,chooseY)[1];
              writting(r,g,b);
            }
            
          }
          p5.noStroke();
          p5.fill(255, 255, 255, 50);
          p5.circle((thumbPos[0]+indexPos[0]) / 2, (thumbPos[1]+indexPos[1]) / 2, 50, 50);
          
        }
        }
        
      p5.fill(255);
      p5.stroke(0);
      p5.circle(chooseX, chooseY, 10, 10);

      
      
      

      // drawKeypoints(p5);
        
    }
		
	};

  return (
    <div className="App" onClick={connectToPort}>
      <header className="App-header">
        {!connected ? 'Click to connect!': 'Connected! Use hand to select color'}
        <Sketch setup={setup} draw={draw} />
        {colors.length > 0 ? 'red: ' + colors[0] + ' green: ' + colors[1] + ' blue: ' + colors[2] : ''}
      </header>
      
    </div>
  );
}

export default App;
