#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  /* vars */
  bShowHelp = true;
  angle = 0.0;
  
  /* of setup */
  ofSetVerticalSync(true);
  ofSetFrameRate(60);
  
  /* load model */
  model.loadModel("iso.obj");
  
  /* setup glitch stuff */
  myFbo.allocate(ofGetWidth(), ofGetHeight());
  myGlitch.setup(&myFbo);
  
  // 0 output channels,
  // 2 input channels
  // 44100 samples per second
  // 256 samples per buffer
  // 4 num buffers (latency)
  
  soundStream.listDevices();
  
  //if you want to set a different device id
  // soundcloud -> soundflower ch2 output, soundflowerbed
  soundStream.setDeviceID(7); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
  
  int bufferSize = 256;
  
  
  left.assign(bufferSize, 0.0);
  right.assign(bufferSize, 0.0);
  volHistory.assign(400, 0.0);
  
  bufferCounter	= 0;
  drawCounter		= 0;
  smoothedVol     = 0.0;
  scaledVol		= 0.0;
  
  soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
  //lets scale the vol up to a 0-1 range
  scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
  
  //lets record the volume into an array
  volHistory.push_back( scaledVol );
  
  //if we are bigger the the size we want to record - lets drop the oldest value
  if( volHistory.size() >= 400 ){
    volHistory.erase(volHistory.begin(), volHistory.begin()+1);
  }
  
  
  float scale = 0.15 + (0.55 * scaledVol);
  model.setScale(scale, scale, scale);
  model.setRotation(0, angle, 1, 0, 0);
  angle += 2.0;
  
  myFbo.begin();
  ofClear(0, 0, 0, 255);
  ofSetColor(255);
  
  
  /* fill mesh */
  model.setPosition(ofGetWidth() * 1/6, (float)ofGetHeight() * 0.5, 0);
  model.draw(OF_MESH_FILL); //same as model.drawFaces();

  /* wire mesh */
  model.setPosition(ofGetWidth() * 3/6, (float)ofGetHeight() * 0.5, 0);
  model.draw(OF_MESH_WIREFRAME); // same as model.drawWireframe();
  
  /* point mesh */
  model.setPosition(ofGetWidth() * 5/6, (float)ofGetHeight() * 0.5, 0);
  model.draw(OF_MESH_POINTS); // same as model.drawVertices();
  
  
  myFbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){

  /* apply gitch effect */
  myGlitch.generateFx();

  /* draw effected view */
  ofSetColor(255);
  myFbo.draw(0, 0);
  
  /* show information */
  string info = "";
  info += "0,1,2,3,4,5,6,7,8,9 : Apply glitch effects.\n";
  info += "a,z,e,r,t,y,u : Apply color remap effects.\n";
  info += "H key : Hide or show this information.";
  
  if (bShowHelp){
    ofSetColor(0, 200);
    ofRect(25, 17, 320, 60);
    ofSetColor(255);
    ofDrawBitmapString(info, 30,30);
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, true);
  if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW			, true);
  if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER			, true);
  if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, true);
  if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST			, true);
  if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, true);
  if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE			, true);
  if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, true);
  if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL			, true);
  if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT			, true);
  
  if (key == 'a') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
  if (key == 'z') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, true);
  if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, true);
  if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, true);
  if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, true);
  if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, true);
  if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, true);
  
  if (key == 'h') bShowHelp ^= true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, false);
  if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW			, false);
  if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER			, false);
  if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, false);
  if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST			, false);
  if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, false);
  if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE			, false);
  if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, false);
  if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL			, false);
  if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT			, false);
  
  if (key == 'a') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
  if (key == 'z') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, false);
  if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, false);
  if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, false);
  if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, false);
  if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, false);
  if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, false);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
  
  float curVol = 0.0;
  
  // samples are "interleaved"
  int numCounted = 0;
  
  //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
  for (int i = 0; i < bufferSize; i++){
    left[i]		= input[i*2]*0.5;
    right[i]	= input[i*2+1]*0.5;
    
    curVol += left[i] * left[i];
    curVol += right[i] * right[i];
    numCounted+=2;
  }
  
  //this is how we get the mean of rms :)
  curVol /= (float)numCounted;
  
  // this is how we get the root of rms :)
  curVol = sqrt( curVol );
  
  smoothedVol *= 0.93;
  smoothedVol += 0.07 * curVol;
  
  bufferCounter++;
  
}
