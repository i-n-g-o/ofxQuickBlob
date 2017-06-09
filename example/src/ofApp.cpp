#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	// load an image
	image.load("blob1.jpg");
	
	const ofPixels& pix = image.getPixels().getChannel(0);
	
	// setup ofxQuickBlob in destructive mode
	detector.setup(pix, true);
	
	detector.threshold = 127; // use threshold (improves speed)
	detector.blobMinSize = 100; // set min blob size
	detector.blobMaxSize = -1; //no max blob size
	detector.blobMatchColor = 0; // match on black pixels
	
	// extract one image
	detector.extract(pix);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	// draw image
	ofSetColor(255);
	image.draw(0,0);
	
	// draw bounding boxes
	ofSetColor(200, 0, 0);
	ofNoFill();
	ofSetLineWidth(1);
	for (int i=0; i<detector.getBlobs().size(); i++) {		
		ofDrawRectangle(detector.getBlobs()[i].bb_x1, detector.getBlobs()[i].bb_y1, detector.getBlobs()[i].bb_x2 - detector.getBlobs()[i].bb_x1, detector.getBlobs()[i].bb_y2 - detector.getBlobs()[i].bb_y1);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key) {
		case ' ':
			uint64_t start = ofGetElapsedTimeMicros();
			
			detector.extract((image.getPixels().getChannel(0)));
			
			ofLogNotice() << "extraced in " << (ofGetElapsedTimeMicros() - start) << " us";

			break;
			
	}
	
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
